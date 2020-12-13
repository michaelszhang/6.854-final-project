#include "tests.h"

#include <vector>

#include "heap.h"
#include "item.h"

static const int LARGE_N = 1000000;

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
    expect_delete(heap, i-15);
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
    expect_delete(heap, {i*2, i*2+1});
  }
  Item::dump_statistics();
}

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
}
