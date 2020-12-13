#include "tests.h"

#include <vector>
#include <iostream>
#include <map>

#include "heap.h"
#include "item.h"

// Generators
double randomUniform() {
	return (double)rand() / RAND_MAX;
}

std::vector<int> value_sequence(int n, double alpha) {
	// Generate sequence and perform Fischer-Yates Shuffle
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

// Operation Sampling via Modified Random Walk
int next_state(int *operations_left, std::vector<double> &pdf) {
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

// DecreaseKeySampler
class DecreaseKeySampler {
public:
	void add(INode* n) {
		time_inserted[n] = time;
		distribution[time] = n;
		time++;
	}

	void remove(INode* n) {
		int t = time_inserted[n];
		time_inserted.erase(n);
		distribution.erase(t);
	}

	INode* sampleUniformTime() {
		int start = distribution.begin()->first;
		int end = distribution.rbegin()->first;
		double t = (start + randomUniform() * (end - start + 1) - 1);
		return distribution.lower_bound(t)->second;
	}

private:
	std::map<INode*, double> time_inserted;
	std::map<double, INode*> distribution;
	int time = 0;
};

// Testing
static const int LARGE_N = 1000000;
static const int K = 1;

MAKE_TEST(correctness_simple_insert_delete, heap) {
	for (int i: {6, 10, 2, 15, 8, 12, 3, 0, 14, 4, 1, 11, 5, 13, 9, 7}) {
		heap->insert(Item(i));
	}
	Item::dump_statistics();
	for (int i = 0; i < 16; ++i) {
		expect_delete(heap, i);
	}
	Item::dump_statistics();
}

MAKE_TEST(correctness_simple_decreasekey, heap) {
	std::vector<INode*> nodes;
	for (int i = 0; i < 16; ++i) {
		nodes.push_back(heap->insert(i));
	}
	Item::dump_statistics();
	for (int i = 0; i < 16; ++i) {
		heap->decrease_key(nodes[i], -i);
	}
	Item::dump_statistics();
	for (int i = 0; i < 16; ++i) {
		expect_delete(heap, i - 15);
	}
	Item::dump_statistics();
}

MAKE_TEST(correctness_simple_delete_1, heap) {
	for (int i = 0; i < 16; ++i) {
		heap->insert(i);
	}
	Item::dump_statistics();
	for (int i = 0; i < 16; ++i) {
		expect_delete(heap, std::vector<int>{i});
	}
	Item::dump_statistics();
}

MAKE_TEST(correctness_simple_delete_2, heap) {
	for (int i = 0; i < 16; ++i) {
		heap->insert(i);
	}
	Item::dump_statistics();
	for (int i = 0; i < 8; ++i) {
		expect_delete(heap, {i*2, i*2 + 1});
	}
	Item::dump_statistics();
}

MAKE_TEST(correctness_simple_delete_n_1, heap) {
	for (int i = 0; i < 16; ++i) {
		heap->insert(i);
	}
	Item::dump_statistics();
	expect_delete(heap, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14});
	Item::dump_statistics();
}

/*
   MAKE_TEST(benchmark_delete_n, heap) {
   const int n = LARGE_N;
   for (int i = 0; i < n; ++i) {
    heap->insert(i);
   }
   Item::dump_statistics();
   heap->delete_k(n);
   Item::dump_statistics();
   }

   MAKE_TEST(benchmark_delete_1, heap) {
   const int n = LARGE_N;
   for (int i = 0; i < n; ++i) {
    heap->insert(i);
   }
   Item::dump_statistics();
   for (int i = 0; i < n; ++i) {
    heap->delete_k(1);
   }
   Item::dump_statistics();
   }

   MAKE_TEST(benchmark_delete_2, heap) {
   const int n = LARGE_N;
   for (int i = 0; i < n; ++i) {
    heap->insert(i);
   }
   Item::dump_statistics();
   for (int i = 0; i < n/2; ++i) {
    heap->delete_k(2);
   }
   Item::dump_statistics();
   }*/

MAKE_TEST(benchmark_ordered_values_ordered_operations_uniform_deckey, heap) {
	const int n = 100;
	const int k = 3;
	const int d = n; // num dec key
  const int base = 10; // min range of dec key
	std::vector<int> values, operations;
	values = value_sequence(n, 0.5);
	// inserts -> dec key -> select k - > del min
	std::vector<std::vector<double> > transitions = {{0.999999, 0.000001, 0, 0},
		                                               {0, 0.999999, 0, 0.000001},
		                                               {0, 0, 1, 0},
		                                               {0, 0, 0.000001, 0.999999}};
	operations = operation_sequence(transitions, n, d, k);
	DecreaseKeySampler sampler;

	std::map<int, INode*> value_to_pointer;
	int idx = 0;
	for (int op : operations) {
		if (op == 0) {
      // insert
			int cur_value = values[idx++];
			INode* new_node = heap->insert(Item(cur_value));
			value_to_pointer[cur_value] = new_node;
			sampler.add(new_node);
		} else if (op == 1) {
      // dec key
			INode* node = sampler.sampleUniformTime();
			int new_value = randomUniform() * (node->value.get_value() + base) - base - 1;
			heap->decrease_key(node, Item(new_value));
		} else if (op == 2) {
      // del min
			Item minItem = heap->delete_min();
			//std::cout << minItem.get_value() << std::endl;
			sampler.remove(value_to_pointer[minItem.get_value()]);
		} else if (op == 3) {
      // select k
    }
	}
	Item::dump_statistics();
}
