#include "tests_generation.h"

#include <assert.h>

#include <vector>


DecreaseKeySampler::DecreaseKeySampler(int num_total) {
  for (int i = 1; i <= 100 * num_total + 5; i++) {
    available_keys.insert(i);
  }
}

void DecreaseKeySampler::add(INode* n) {
  if (time_inserted.find(n) != time_inserted.end()) {
    throw std::runtime_error("Repeat Insert Sampler: Keys not Unique");
  }
  time_inserted[n] = time;
  distribution[time] = n;
  time++;
  num_items++;
}

void DecreaseKeySampler::remove(INode* n) {
  if (num_items == 0) {
    throw std::runtime_error("Empty Sampler");
  }
  int t = time_inserted[n];
  time_inserted.erase(n);
  distribution.erase(t);
  num_items--;
}

INode* DecreaseKeySampler::sampleUniformTime() {
  if (num_items == 0) {
    throw std::runtime_error("Empty Sampler");
  }
  int start = distribution.begin()->first;
  int end = distribution.rbegin()->first;
  double t = (start + randomUniform() * (end - start + 1) - 1);
  return distribution.lower_bound(t)->second;
}

int DecreaseKeySampler::next_unique_key(int key) {
  assert(available_keys.size() > 0);
  int next_key;
  if (key >= 0) { // cur_key > than all available_keys (negated dict)
    next_key = *available_keys.begin();
    available_keys.erase(next_key);
  } else {
    next_key = *available_keys.lower_bound(-key);
    available_keys.erase(next_key);
    available_keys.insert(-key);
  }
  return -next_key;
}

unsigned DecreaseKeySampler::size() {
  return num_items;
}

double randomUniform() {
  // Return value in [0, 1] sampled uniformly
  return (double)rand() / RAND_MAX;
}

std::vector<int> value_sequence(int n, double alpha) {
  // Generate value sequence via Fischer-Yates Shuffle
  // alpha controls how shuffled sequence is
  // i.e. alpha = 0 means in order while 1 produces an unbiased permuation
  std::vector<int> v;
  for (int i = 0; i < n; i++) {
    v.push_back(i);
  }
  for (int i = 0; i < n - 1; i++) {
    if (randomUniform() < alpha) {
      int j = i + rand() % (n - i);
      std::swap(v[i], v[j]);
    }
  }
  return v;
}

int next_state(int *operations_left, std::vector<double> &pdf) {
  // Return next valid state of random walk for operation generation
  double sum = 0;
  std::vector<double> cdf;
  std::vector<int> sample_space;
  for (unsigned i = 0; i < pdf.size(); i++) {
    if (operations_left[i] > 0) {
      sum += pdf[i];
      cdf.push_back(sum);
      sample_space.push_back(i);
    }
  }
  if (sum == 0) {
    return -1;
  }
  double r = randomUniform() * sum;
  for (unsigned i = 0; i < cdf.size() - 1; i++) {
    if (r < cdf[i]) {
      return sample_space[i];
    }
  }
  return sample_space[cdf.size() - 1];
}

std::vector<int> operation_sequence(std::vector<std::vector<double> > &transitions, int n, int k) {
  // Return sequence of operations based on parameters
  // Transitions contain the PDFs of the chain matrix, n0 num inserts, n1 num dec key
  int operations_left[NUM_OPERATIONS] = {n, 0, 0}; // 0, 1, 2 are insert, dec key, del k
  int cur_state = 0;
  int heap_size = 0;
  std::vector<int> v;
  while (cur_state != -1 && (operations_left[INSERT] + operations_left[DELETE_K]) > 0) {
    v.push_back(cur_state);
    operations_left[cur_state]--;
    if (cur_state == INSERT) {
      heap_size++;
      operations_left[DECREASE_KEY]++;
      operations_left[DELETE_K] += (heap_size % k == 0);
    } else if (cur_state == DELETE_K) {
      operations_left[DECREASE_KEY] = std::max(0, operations_left[DECREASE_KEY] - k);
      heap_size -= k;
    }
    cur_state = next_state(operations_left, transitions[cur_state]);
  }
  return v;
}
