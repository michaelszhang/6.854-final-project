#include "tests_generation.h"

#include <vector>

void DecreaseKeySampler::add(INode* n) {
  time_inserted[n] = time;
  distribution[time] = n;
  time++;
}

void DecreaseKeySampler::remove(INode* n) {
  int t = time_inserted[n];
  time_inserted.erase(n);
  distribution.erase(t);
}

INode* DecreaseKeySampler::sampleUniformTime() {
  int start = distribution.begin()->first;
  int end = distribution.rbegin()->first;
  double t = (start + randomUniform() * (end - start + 1) - 1);
  return distribution.lower_bound(t)->second;
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
	for (int i = 0; i < pdf.size(); i++) {
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
	for (int i = 0; i < cdf.size() - 1; i++) {
		if (r < cdf[i]) {
			return sample_space[i];
		}
	}
	return sample_space[cdf.size() - 1];
}

std::vector<int> operation_sequence(std::vector<std::vector<double> > &transitions, int n0, int n1, int k) {
	// transitions contain the PDFs of the chain matrix
	// 0, 1, 2, 3 are insert, dec key, del min, select k
	int operations_left[4] = {n0, n1, 0, 0};
	int cur_state = 0;
	int heap_size = 0;
	std::vector<int> v;
	while (cur_state != -1 && (operations_left[0] + operations_left[2]) > 0) {
		v.push_back(cur_state);
		operations_left[cur_state]--;
		if (cur_state == 0) {
			heap_size++;
			operations_left[3] += (heap_size % k == 0);
			operations_left[2]++;
		} else if (cur_state == 2) {
			operations_left[3] -= (heap_size % k == 0);
			heap_size--;
		}
		cur_state = next_state(operations_left, transitions[cur_state]);
	}
	return v;
}
