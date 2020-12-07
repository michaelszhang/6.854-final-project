#include "tests.h"

#include <assert.h>
#include <memory>
#include "heap.h"
#include "binary_heap.h"
#include "item.h"

template<typename T>
void run_test() {
  std::unique_ptr<IHeap<Item>> heap(new T());

  for (int i: {6, 2, 8, 3, 4, 1, 5, 9, 7}) {
    heap->insert(Item(i));
  }
  for (int i = 1; i < 10; ++i) {
    printf("%d\n", heap->delete_min().getValue());
  }
}

template void run_test<BinaryHeap<Item>>();
