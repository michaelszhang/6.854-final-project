#include "fibonacci_heap.h"

FibonacciHeap::FibonacciHeap()
  : root(nullptr)
{}

FibonacciHeap::~FibonacciHeap() {}

INode* FibonacciHeap::insert(const Item& item) {
  (void) item;
  return nullptr;
}

void FibonacciHeap::decrease_key(INode* node, const Item& item) {
  (void) node;
  (void) item;
}

Item FibonacciHeap::delete_min() {
  return Item();
}

std::vector<Item> FibonacciHeap::delete_k(unsigned k) {
  // TODO[jerry]
  (void) k;
  return std::vector<Item>();
}

unsigned FibonacciHeap::size() const {
  return 0;
}
