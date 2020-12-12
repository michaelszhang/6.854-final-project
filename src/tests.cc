#include "tests.h"

#include "heap.h"
#include "item.h"

MAKE_TEST(correctness_simple_insert_delete, heap) {
  for (int i: {6, 10, 2, 15, 8, 12, 3, 0, 14, 4, 1, 11, 5, 13, 9, 7}) {
    heap->insert(Item(i));
  }
  Item::dump_statistics();
  for (int i = 0; i < 16; ++i) {
    EXPECT_DELETE(heap, i);
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
    EXPECT_DELETE(heap, i-15);
  }
  Item::dump_statistics();
}
