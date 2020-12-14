#include "tests.h"
#include "tests_generation.h"

#include <list>
#include <vector>
#include <iostream>
#include <map>

#include "binary_heap.h"
#include "fibonacci_heap.h"
#include "heap.h"
#include "item.h"
#include "median_select.h"
#include "soft_heap.h"

// Testing
static const int LARGE_N = 1000000;
static const int MIN_VALUE = -10000;

MAKE_TEST(soft_heap_tiny_epsilon, _) {
  RUN_ONCE_ONLY(_);

  SoftHeap heap(0.001);
  std::list<Item> stuff; // prevent deallocation of temporaries
  for (int i: {6, 10, 2, 15, 8, 12, 3, 0, 14, 4, 1, 11, 5, 13, 9, 7}) {
    stuff.emplace_back(i);
    heap.insert(stuff.back());
  }
  Item::dump_statistics();
  for (int i = 0; i < 16; ++i) {
    const SoftHeapEntry& value = heap.find_min();
    if (value.corrupted) {
      TestFail("corrupted heap value");
    } else if (value.item.get_value() != i) {
      TestFail("find-min returned %d, not %d", value.item.get_value(), i);
    }
    heap.delete_min();
  }
  Item::dump_statistics();
}

MAKE_TEST(soft_heap_corruption_small, _) {
  RUN_ONCE_ONLY(_);

  SoftHeap heap(0.5);
  std::list<Item> stuff; // prevent deallocation of temporaries
  for (int i: {6, 10, 2, 15, 8, 12, 3, 0, 14, 4, 1, 11, 5, 13, 9, 7}) {
    stuff.emplace_back(i);
    heap.insert(stuff.back());
  }
  Item::dump_statistics();
  for (int i = 0; i < 16; ++i) {
    const SoftHeapEntry& value = heap.find_min();
    if (value.item.get_value() != (i^1)) {
      TestFail("find-min returned %d, not %d", value.item.get_value(), i^1);
    }
    heap.delete_min();
  }
  Item::dump_statistics();
}

MAKE_TEST(soft_heap_corruption_larger, _) {
  RUN_ONCE_ONLY(_);

  SoftHeap heap(0.5);
  std::list<Item> stuff;
  for (int i: {97, 85, 71, 23, 64, 89, 99, 66, 93, 36,
               20,  7, 16, 86, 57, 47, 30, 52, 41, 11,
               69, 46, 50, 78,  6, 15, 48, 70, 17, 54,
               32, 80, 25, 40, 42, 43, 26, 82, 55, 76,
               91, 63, 12, 95, 77, 73,  8, 34, 58, 22,
                9, 96, 90, 33, 56, 53, 39, 13, 68, 24,
               81, 37,  2, 28, 21, 35, 79, 51, 67,  5,
               61, 74,  3, 75, 19, 10,  1, 60, 87,  4,
               27, 98, 38, 92, 18, 31, 62, 83, 59, 88,
                0, 49, 44, 29, 84, 65, 72, 45, 14, 94}) {
    stuff.emplace_back(i);
    heap.insert(stuff.back());
  }
  Item::dump_statistics();
  std::vector<int> answer{ 3,  1,  5,  4, 11,  6,  9,  2, 14, 16,
                           7, 12,  8, 18,  0, 19, 10, 22, 13, 17,
                          15, 26, 25, 23, 20, 29, 27, 32, 30, 28,
                          24, 35, 21, 38, 31, 40, 34, 37, 33, 45,
                          46, 41, 43, 42, 49, 44, 50, 48, 47, 36,
                          54, 52, 53, 39, 60, 51, 62, 59, 63, 55,
                          58, 56, 67, 61, 70, 69, 64, 57, 72, 75,
                          74, 76, 73, 71, 66, 81, 68, 80, 78, 83,
                          65, 86, 85, 82, 77, 87, 79, 88, 84, 94,
                          95, 91, 93, 89, 98, 92, 99, 97, 96, 90 };
  for (int i = 0; i < 100; ++i) {
    const SoftHeapEntry& value = heap.find_min();
    if (value.item.get_value() != answer[i]) {
      TestFail("find-min returned %d, not %d", value.item.get_value(), answer[i]);
    }
    heap.delete_min();
  }
  Item::dump_statistics();
}

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
		nodes.push_back(heap->insert(Item(i)));
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
    heap->insert(Item(i));
  }
  Item::dump_statistics();
  for (int i = 0; i < 16; ++i) {
    expect_delete(heap, std::vector<int>{i});
  }
  Item::dump_statistics();
}

MAKE_TEST(correctness_simple_delete_2, heap) {
  for (int i = 0; i < 16; ++i) {
    heap->insert(Item(i));
  }
  Item::dump_statistics();
  for (int i = 0; i < 8; ++i) {
    expect_delete(heap, {2*i, 2*i+1});
  }
  Item::dump_statistics();
}

MAKE_TEST(correctness_simple_delete_n_1, heap) {
  for (int i = 0; i < 16; ++i) {
    heap->insert(Item(i));
  }
  Item::dump_statistics();
  expect_delete(heap, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14});
  Item::dump_statistics();
}

MAKE_TEST(benchmark_delete_n, heap) {
  const int n = LARGE_N;
  for (int i = 0; i < n; ++i) {
    heap->insert(Item(i));
  }
  Item::dump_statistics();
  heap->delete_k(n);
  Item::dump_statistics();
}

MAKE_TEST(benchmark_delete_1, heap) {
  SKIP_HEAP(heap, MedianSelect);

  const int n = LARGE_N;
  for (int i = 0; i < n; ++i) {
    heap->insert(Item(i));
  }
  Item::dump_statistics();
  for (int i = 0; i < n; ++i) {
    assert_delete(heap, std::vector<int>{i});
  }
  Item::dump_statistics();
}

MAKE_TEST(benchmark_delete_2, heap) {
  SKIP_HEAP(heap, MedianSelect);

  const int n = LARGE_N;
  for (int i = 0; i < n; ++i) {
    heap->insert(Item(i));
  }
  Item::dump_statistics();
  for (int i = 0; i < n/2; ++i) {
    assert_delete(heap, {i*2, i*2+1});
  }
  Item::dump_statistics();
}

///////////////////////////////////
// BEGIN FULL BENCHMARKING TESTS //
///////////////////////////////////

void benchmark_test(std::vector<int> values,
										std::vector<int> operations,
										IHeap* heap) {
  DecreaseKeySampler sampler;
  std::map<int, INode*> value_to_pointer;
	int idx = 0, val;
	INode *node;

	for (int op: operations) {
		switch ((OPERATION)op) {
			case INSERT:
				val = values[idx++];
				node = heap->insert(Item(val));
				value_to_pointer[val] = node;
				sampler.add(node);
				break;
			case DECREASE_KEY:
				node = sampler.sampleUniformTime();
				val = randomUniform() * (node->value.get_value() + MIN_VALUE) - MIN_VALUE - 1;
				heap->decrease_key(node, Item(val));
				break;
			case DELETE_K:
				Item min_item = heap->delete_min(); // CHANGE TO DEL K
				sampler.remove(value_to_pointer[min_item.get_value()]);
				break;
		}
  }
	Item::dump_statistics();
}

void generate_test(int n, int k, double alpha,
                  std::vector<std::vector<double>> &transitions,
                  std::vector<int> &values,
                  std::vector<int> &operations) {
  values = value_sequence(n, alpha);
  operations = operation_sequence(transitions, n, n, k);
}

const int BENCHMARK_N = 10'000'000;
const std::vector<int> K_SIZES = {5, 27, 56, 3162, 20115, 100'000, 578360, 1'000'000, 3762874, 5'000'000};
// O(c), lg(N), N^0.25, N^0.5, N^0.75, 0.01N, N^0.9, 0.1N, N/lg(N), 0.5N

#define INSTANTIATE_TEST(test_name) \
  MAKE_TEST(test_name##_0, heap) { test_name(heap, K_SIZES[0]); } \
  MAKE_TEST(test_name##_1, heap) { test_name(heap, K_SIZES[1]); } \
  MAKE_TEST(test_name##_2, heap) { test_name(heap, K_SIZES[2]); } \
  MAKE_TEST(test_name##_3, heap) { test_name(heap, K_SIZES[3]); } \
  MAKE_TEST(test_name##_4, heap) { test_name(heap, K_SIZES[4]); } \
  MAKE_TEST(test_name##_5, heap) { test_name(heap, K_SIZES[5]); } \
  MAKE_TEST(test_name##_6, heap) { test_name(heap, K_SIZES[6]); } \
  MAKE_TEST(test_name##_7, heap) { test_name(heap, K_SIZES[7]); } \
  MAKE_TEST(test_name##_8, heap) { test_name(heap, K_SIZES[8]); } \
  MAKE_TEST(test_name##_9, heap) { test_name(heap, K_SIZES[9]); }

void benchmark_ordered_ordered(IHeap *heap, int k) {
  double alpha = 0;
  std::vector<int> values, operations;
	std::vector<std::vector<double>> transitions = {{1 - EPSILON, EPSILON, 0},
		                                              {0, 1 - EPSILON, EPSILON},
		                                              {0, 0, 1}};
  generate_test(BENCHMARK_N, k, alpha, transitions, values, operations);
  benchmark_test(values, operations, heap);
}

void benchmark_reverse_ordered_ordered(IHeap *heap, int k) {
  double alpha = 0;
  std::vector<int> values, operations;
	std::vector<std::vector<double>> transitions = {{1 - EPSILON, EPSILON, 0},
		                                              {0, 1 - EPSILON, EPSILON},
		                                              {0, 0, 1}};
  std::reverse(values.begin(), values.end());
  generate_test(BENCHMARK_N, k, alpha, transitions, values, operations);
  benchmark_test(values, operations, heap);
}

void benchmark_ordered_uniform_random(IHeap *heap, int k) {
  double alpha = 0;
  std::vector<int> values, operations;
	std::vector<std::vector<double>> transitions = {{1.0 / 3, 1.0 / 3, 1.0 / 3},
		                                              {1.0 / 3, 1.0 / 3, 1.0 / 3},
		                                              {1.0 / 3, 1.0 / 3, 1.0 / 3}};
  generate_test(BENCHMARK_N, k, alpha, transitions, values, operations);
  benchmark_test(values, operations, heap);
}

void benchmark_reverse_ordered_uniform_random(IHeap *heap, int k) {
  double alpha = 0;
  std::vector<int> values, operations;
	std::vector<std::vector<double>> transitions = {{1.0 / 3, 1.0 / 3, 1.0 / 3},
		                                              {1.0 / 3, 1.0 / 3, 1.0 / 3},
		                                              {1.0 / 3, 1.0 / 3, 1.0 / 3}};
  std::reverse(values.begin(), values.end());
  generate_test(BENCHMARK_N, k, alpha, transitions, values, operations);
  benchmark_test(values, operations, heap);
}

void benchmark_uniform_random_ordered(IHeap *heap, int k) {
  double alpha = 1;
  std::vector<int> values, operations;
	std::vector<std::vector<double>> transitions = {{1 - EPSILON, EPSILON, 0},
		                                              {0, 1 - EPSILON, EPSILON},
		                                              {0, 0, 1}};
  generate_test(BENCHMARK_N, k, alpha, transitions, values, operations);
  benchmark_test(values, operations, heap);
}

void benchmark_uniform_random_uniform_random_ordered(IHeap *heap, int k) {
  double alpha = 1;
  std::vector<int> values, operations;
  std::vector<std::vector<double>> transitions = {{1.0 / 3, 1.0 / 3, 1.0 / 3},
		                                              {1.0 / 3, 1.0 / 3, 1.0 / 3},
		                                              {1.0 / 3, 1.0 / 3, 1.0 / 3}};
  generate_test(BENCHMARK_N, k, alpha, transitions, values, operations);
  benchmark_test(values, operations, heap);
}

INSTANTIATE_TEST(benchmark_ordered_ordered);
INSTANTIATE_TEST(benchmark_reverse_ordered_ordered);
INSTANTIATE_TEST(benchmark_ordered_uniform_random);
INSTANTIATE_TEST(benchmark_reverse_ordered_uniform_random);
INSTANTIATE_TEST(benchmark_uniform_random_ordered);
INSTANTIATE_TEST(benchmark_uniform_random_uniform_random_ordered);
