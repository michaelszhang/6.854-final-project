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
static const int K = 1;

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

MAKE_TEST(correctness_simple_select_1, heap) {
  for (int i = 0; i < 16; ++i) {
    heap->insert(Item(i));
  }
  Item::dump_statistics();
  for (int i = 0; i < 16; ++i) {
    expect_select(heap, std::vector<int>{i});
    expect_delete(heap, i);
  }
  Item::dump_statistics();
}

MAKE_TEST(correctness_simple_select_2, heap) {
  for (int i = 0; i < 16; ++i) {
    heap->insert(Item(i));
  }
  Item::dump_statistics();
  for (int i = 0; i < 15; ++i) {
    expect_select(heap, {i, i+1});
    expect_delete(heap, i);
  }
  expect_delete(heap, 15);
  Item::dump_statistics();
}

MAKE_TEST(correctness_simple_select_n_1, heap) {
  for (int i = 0; i < 16; ++i) {
    heap->insert(Item(i));
  }
  Item::dump_statistics();
  expect_select(heap, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14});
  Item::dump_statistics();
}

MAKE_TEST(benchmark_select_n, heap) {
  const int n = LARGE_N;
  for (int i = 0; i < n; ++i) {
    heap->insert(Item(i));
  }
  Item::dump_statistics();
  heap->select_k(n);
  Item::dump_statistics();
}

MAKE_TEST(benchmark_select_1, heap) {
  SKIP_HEAP(heap, MedianSelect);

  const int n = LARGE_N;
  for (int i = 0; i < n; ++i) {
    heap->insert(Item(i));
  }
  Item::dump_statistics();
  for (int i = 0; i < n; ++i) {
    heap->select_k(1);
    heap->delete_min();
  }
  Item::dump_statistics();
}

MAKE_TEST(benchmark_select_2, heap) {
  SKIP_HEAP(heap, MedianSelect);

  const int n = LARGE_N;
  for (int i = 0; i < n; ++i) {
    heap->insert(Item(i));
  }
  Item::dump_statistics();
  for (int i = 0; i < n-1; ++i) {
    heap->select_k(2);
    heap->delete_min();
  }
  Item::dump_statistics();
}
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
