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
/*
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
}*/

///////////////////////////////////
// BEGIN FULL BENCHMARKING TESTS //
///////////////////////////////////
using namespace std;

void printLine(string s) {
  cout << s << endl;
}

void benchmark_test(std::vector<int> values,
										std::vector<int> operations,
										IHeap* heap, int n, int k) {
  DecreaseKeySampler sampler(n);
  std::map<int, INode*> value_to_pointer;
	int idx = 0;
  int cnt = 0;

  //std::cout << operations.size() << std::endl;
	for (int op: operations) {
		switch ((OPERATION)op) {
			case INSERT: {
        printLine("INS");
				int value = values[idx++];
				INode *node = heap->insert(Item(value));
				value_to_pointer[value] = node;
				sampler.add(node);
				break;
        printLine("INSOUT");
      }
			case DECREASE_KEY: {
        printLine("DKIN");
				INode *node = sampler.sampleUniformTime();
        int old_value = node->value.get_value();
				int value = sampler.next_unique_key(old_value);
        printLine("A");
        value_to_pointer.erase(old_value);
        value_to_pointer[value] = node;
        printLine("B");
        std::cout << heap->size() << ' ' << sampler.size() << ' ' << old_value << ' ' << value << std::endl;
				heap->decrease_key(node, Item(value));
        printLine("DKOUT");
				break;
      }
			case DELETE_K: {
        //std::cout << heap->size() << std::endl;
        //Item min_item = heap->delete_min(k);
        //sampler.remove(value_to_pointer[item.get_value()]);
        printLine("DELIN");
        std::cout << heap->size() << ' ' << k << std::endl;
				std::vector<Item> min_items = heap->delete_k(k);
        printLine("METHOD");
        for (Item item: min_items) {
          int value = item.get_value();
				  sampler.remove(value_to_pointer[value]);
          //value_to_pointer.erase(value);
          //std::cout << value_to_pointer[item.get_value()] << std::endl;
        }
        printLine("DELOUT");
				break;
      }
		}
    cnt++;
  }
	Item::dump_statistics();
  //exit(0);
}

void generate_test(int n, int k, double alpha,
                  std::vector<std::vector<double>> &transitions,
                  std::vector<int> &values,
                  std::vector<int> &operations) {
  values = value_sequence(n, alpha);
  operations = operation_sequence(transitions, n, k);
}

const int BENCHMARK_N = 1000;
const std::vector<int> K_SIZES = {5, 27, 56, 3162, 20115, 100'000, 578360, 1'000'000, 3762874, 5'000'000};
// O(c), lg(N), N^0.25, N^0.5, N^0.75, 0.01N, N^0.9, 0.1N, N/lg(N), 0.5N


// Don't run tests that are O(n^2) time complexity and won't terminate in a sane amount of time
#define BENCHMARK_TEST_GUARD_K(test_name, kidx) \
  MAKE_TEST(test_name##kidx, heap) { \
    SKIP_HEAP(heap, FibonacciHeap); \
    if (kidx < 5) { \
      SKIP_HEAP(heap, MedianSelect); \
    } \
    /*if (kidx >= 5) { \
      SKIP_HEAP(HeapAdapter<BinaryHeap>); \
      SKIP_HEAP(HeapAdapter<FibonacciHeap>); \
    } */ \
    test_name(heap, K_SIZES[kidx]); \
  }

#define INSTANTIATE_TEST(test_name) \
  BENCHMARK_TEST_GUARD_K(test_name, 0) \

//  BENCHMARK_TEST_GUARD_K(test_name, 1) \
  BENCHMARK_TEST_GUARD_K(test_name, 2) \
  BENCHMARK_TEST_GUARD_K(test_name, 3) \
  BENCHMARK_TEST_GUARD_K(test_name, 4) \
  BENCHMARK_TEST_GUARD_K(test_name, 5) \
  BENCHMARK_TEST_GUARD_K(test_name, 6) \
  BENCHMARK_TEST_GUARD_K(test_name, 7) \
  BENCHMARK_TEST_GUARD_K(test_name, 8) \
  BENCHMARK_TEST_GUARD_K(test_name, 9) \

void benchmark_ordered_ordered(IHeap *heap, int k) {
  double alpha = 0;
  std::vector<int> values, operations;
	std::vector<std::vector<double>> transitions = {{1 - EPSILON, EPSILON, 0},
		                                              {0, 1 - EPSILON, EPSILON},
		                                              {0, 0, 1}};
  generate_test(BENCHMARK_N, k, alpha, transitions, values, operations);
  benchmark_test(values, operations, heap, BENCHMARK_N, k);
}

void benchmark_reverse_ordered_ordered(IHeap *heap, int k) {
  double alpha = 0;
  std::vector<int> values, operations;
	std::vector<std::vector<double>> transitions = {{1 - EPSILON, EPSILON, 0},
		                                              {0, 1 - EPSILON, EPSILON},
		                                              {0, 0, 1}};
  std::reverse(values.begin(), values.end());
  generate_test(BENCHMARK_N, k, alpha, transitions, values, operations);
  benchmark_test(values, operations, heap, BENCHMARK_N, k);
}

void benchmark_ordered_uniform_random(IHeap *heap, int k) {
  double alpha = 0;
  std::vector<int> values, operations;
	std::vector<std::vector<double>> transitions = {{1.0 / 3, 1.0 / 3, 1.0 / 3},
		                                              {1.0 / 3, 1.0 / 3, 1.0 / 3},
		                                              {1.0 / 3, 1.0 / 3, 1.0 / 3}};
  generate_test(BENCHMARK_N, k, alpha, transitions, values, operations);
  benchmark_test(values, operations, heap, BENCHMARK_N, k);

}

void benchmark_reverse_ordered_uniform_random(IHeap *heap, int k) {
  double alpha = 0;
  std::vector<int> values, operations;
	std::vector<std::vector<double>> transitions = {{1.0 / 3, 1.0 / 3, 1.0 / 3},
		                                              {1.0 / 3, 1.0 / 3, 1.0 / 3},
		                                              {1.0 / 3, 1.0 / 3, 1.0 / 3}};
  std::reverse(values.begin(), values.end());
  generate_test(BENCHMARK_N, k, alpha, transitions, values, operations);
  benchmark_test(values, operations, heap, BENCHMARK_N, k);
}

void benchmark_uniform_random_ordered(IHeap *heap, int k) {
  double alpha = 1;
  std::vector<int> values, operations;
	std::vector<std::vector<double>> transitions = {{1 - EPSILON, EPSILON, 0},
		                                              {0, 1 - EPSILON, EPSILON},
		                                              {0, 0, 1}};
  generate_test(BENCHMARK_N, k, alpha, transitions, values, operations);
  benchmark_test(values, operations, heap, BENCHMARK_N, k);
}

void benchmark_uniform_random_uniform_random_ordered(IHeap *heap, int k) {
  double alpha = 1;
  std::vector<int> values, operations;
  std::vector<std::vector<double>> transitions = {{1.0 / 3, 1.0 / 3, 1.0 / 3},
		                                              {1.0 / 3, 1.0 / 3, 1.0 / 3},
		                                              {1.0 / 3, 1.0 / 3, 1.0 / 3}};
  generate_test(BENCHMARK_N, k, alpha, transitions, values, operations);
  benchmark_test(values, operations, heap, BENCHMARK_N, k);
}

void benchmark_cliffs_uniform_random(IHeap *heap, int k) {
  const int N_SECTIONS = 10;
  const int N = BENCHMARK_N - BENCHMARK_N % N_SECTIONS;
  std::vector<int> values;
  for (int section = 0; section < N_SECTIONS; ++section) {
    for (int i = section; i < N; i+=10) {
      values.push_back(i);
    }
  }
  std::vector<std::vector<double>> transitions = {{1.0 / 3, 1.0 / 3, 1.0 / 3},
		                                              {1.0 / 3, 1.0 / 3, 1.0 / 3},
		                                              {1.0 / 3, 1.0 / 3, 1.0 / 3}};
  std::vector<int> operations = operation_sequence(transitions, N, k);
  benchmark_test(values, operations, heap, N, k);
}

void benchmark_hills_uniform_random(IHeap *heap, int k) {
  const int N_SECTIONS = 5;
  const int N = BENCHMARK_N - BENCHMARK_N % (2*N_SECTIONS);
  std::vector<int> values;
  for (int section = 0; section < N_SECTIONS; ++section) {
    int i = section;
    for (; i < N; i+=2*N_SECTIONS) {
      values.push_back(i);
    }
    for (i -= N_SECTIONS; i >= 0; i -= 2*N_SECTIONS) {
      values.push_back(i);
    }
  }
  std::vector<std::vector<double>> transitions = {{1.0 / 3, 1.0 / 3, 1.0 / 3},
		                                              {1.0 / 3, 1.0 / 3, 1.0 / 3},
		                                              {1.0 / 3, 1.0 / 3, 1.0 / 3}};
  std::vector<int> operations = operation_sequence(transitions, N, k);
  benchmark_test(values, operations, heap, N, k);
}

void benchmark_uniform_random_sequential_decreasekey(IHeap *heap, int k) {
  double alpha = 1.0;
  std::vector<int> values, operations;
  std::vector<std::vector<double>> transitions = {{1.0 / 3, 1.0 / 3, 1.0 / 3},
		                                              {0.1, 0.8, 0.1},
		                                              {1.0 / 3, 1.0 / 3, 1.0 / 3}};
  generate_test(BENCHMARK_N, k, alpha, transitions, values, operations);
  benchmark_test(values, operations, heap, BENCHMARK_N, k);
}

void benchmark_less_random_sequential_operations(IHeap *heap, int k) {
  double alpha = 0.25;
  std::vector<int> values, operations;
  std::vector<std::vector<double>> transitions = {{0.8, 0.1, 0.1},
		                                              {0.1, 0.8, 0.1},
		                                              {0.1, 0.1, 0.8}};
  generate_test(BENCHMARK_N, k, alpha, transitions, values, operations);
  benchmark_test(values, operations, heap, BENCHMARK_N, k);
}

void benchmark_more_random_sequential_operations(IHeap *heap, int k) {
  double alpha = 0.8;
  std::vector<int> values, operations;
  std::vector<std::vector<double>> transitions = {{0.8, 0.1, 0.1},
		                                              {0.1, 0.8, 0.1},
		                                              {0.1, 0.1, 0.8}};
  generate_test(BENCHMARK_N, k, alpha, transitions, values, operations);
  benchmark_test(values, operations, heap, BENCHMARK_N, k);
}

void benchmark_uniform_random_one_delete(IHeap *heap, int k) {
  double alpha = 1.0;
  std::vector<int> values, operations;
  std::vector<std::vector<double>> transitions = {{0.495, 0.495, 0.01},
		                                              {0.495, 0.495, 0.01},
		                                              {0.495, 0.495, 0.01}};
  generate_test(BENCHMARK_N, k, alpha, transitions, values, operations);
  benchmark_test(values, operations, heap, BENCHMARK_N, k);
}

INSTANTIATE_TEST(benchmark_ordered_ordered);
INSTANTIATE_TEST(benchmark_reverse_ordered_ordered);
INSTANTIATE_TEST(benchmark_ordered_uniform_random);
//INSTANTIATE_TEST(benchmark_reverse_ordered_uniform_random);
/*
INSTANTIATE_TEST(benchmark_uniform_random_ordered);
INSTANTIATE_TEST(benchmark_uniform_random_uniform_random_ordered);
INSTANTIATE_TEST(benchmark_cliffs_uniform_random);
INSTANTIATE_TEST(benchmark_hills_uniform_random);
INSTANTIATE_TEST(benchmark_uniform_random_sequential_decreasekey);
INSTANTIATE_TEST(benchmark_less_random_sequential_operations);
INSTANTIATE_TEST(benchmark_more_random_sequential_operations);
INSTANTIATE_TEST(benchmark_uniform_random_one_delete);*/
