#include <iostream>
#include "src/binary_heap.h"
#include "src/item.h"
#include "src/tests.h"

int main() {
  run_test<BinaryHeap<Item>>();
  return 0;
}
