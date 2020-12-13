#include "fibonacci_heap.h"

#include <algorithm>
#include <iostream>
#include <vector>

FibonacciHeap::FibonacciHeap()
  : heap_size(0), min_node(nullptr), last_node(nullptr) {
  for (unsigned i = 0; i < MAX_RANK; i++) {
    rank_array[i] = nullptr;
  }
}

FibonacciHeap::~FibonacciHeap() {
  delete min_node;
  for (unsigned i = 0; i < MAX_RANK; i++) {
    delete rank_array[i];
  }
}

INode* FibonacciHeap::insert(const Item& item) {
  FibonacciHeapNode* x = new FibonacciHeapNode(item);
  maintain_min(x);
  push_tree(x);
  FibonacciHeapNode *tmp = last_node;
  heap_size++;
  return x;
}

void FibonacciHeap::decrease_key(INode* node, const Item& item) {
  FibonacciHeapNode* rep = dynamic_cast<FibonacciHeapNode*>(node), *x = rep->parent;
  if (rep == nullptr) {
    throw std::runtime_error("Invalid decrease-key node");
  }
  if (rep->value < item) {
    throw std::runtime_error("Decrease-key must decrease value");
  }
  rep->value = item;
  maintain_min(rep);
  if (rep->parent == nullptr) {
    return;
  }
  cut(rep);
  while (x->marked) { // root is always unmarked
    rep = x;
    x = x->parent;
    cut(rep);
    push_tree(rep);
  }
  if (x->parent != nullptr) {
    x->marked = true;
  }
}

int cnt = 0;

Item FibonacciHeap::delete_min() {
  if (size() == 0) {
    throw std::runtime_error("Delete from empty tree");
  }
  Item min_item = min_node->value;
  FibonacciHeapNode *x = last_node;
  unsigned max_rank = 0;
  while (x != nullptr) {
    FibonacciHeapNode *y = x;
    x = x->before;
    if (y != min_node) {
      y = propagate_link(y);
      max_rank = std::max(max_rank, y->rank);
    }
  }
  x = min_node->child;
  delete min_node;
  min_node = last_node = nullptr;
  while (x != nullptr) {
    FibonacciHeapNode *y = x;
    x = x->after;
    y->before = y->after = nullptr;
    y = propagate_link(y);
    max_rank = std::max(max_rank, y->rank);
  }
  for (unsigned i = 0; i <= max_rank; i++) {
    if (rank_array[i] != nullptr) {
      push_tree(rank_array[i]);
      maintain_min(rank_array[i]);
      rank_array[i] = nullptr;
    }
  }
  heap_size--;
  return min_item;
}

std::vector<Item> FibonacciHeap::select_k(unsigned k) {
  // TODO[george]
  (void) k;
  heap_size -= k;
  return std::vector<Item>();
}

unsigned FibonacciHeap::size() const {
  return heap_size;
}

void FibonacciHeap::add_child(FibonacciHeapNode *x, FibonacciHeapNode *y) {
  x->parent = y;
  FibonacciHeapNode *z = y->child;
  x->after = z;
  if (z != nullptr) {
    z->before = x;
  }
  y->child = x;
  y->rank++;
}

FibonacciHeap::FibonacciHeapNode* FibonacciHeap::link(FibonacciHeapNode *x, FibonacciHeapNode *y) {
  if (y->value < x->value) {
    add_child(x, y);
    return y;
  } else {
    add_child(y, x);
    return x;
  }
}

FibonacciHeap::FibonacciHeapNode* FibonacciHeap::propagate_link(FibonacciHeapNode *x) {
  while (rank_array[x->rank] != nullptr) {
    int prev_rank = x->rank;
    x = link(x, rank_array[x->rank]);
    rank_array[prev_rank] = nullptr;
  }
  rank_array[x->rank] = x;
  return x;
}

void FibonacciHeap::cut(FibonacciHeapNode *x) {
  FibonacciHeapNode *y = x->parent;
  if (y->child == x) {
    y->child = x->after;
  }
  if (x->before != nullptr) {
    x->before->after = x->after;
  }
  if (x->after != nullptr) {
    x->after->before = x->before;
  }
  y->rank--;
}

void FibonacciHeap::maintain_min(FibonacciHeapNode *x) {
  if (min_node == nullptr) {
    min_node = x;
  } else if (x->value < min_node->value) {
    min_node = x;
  }
}

void FibonacciHeap::push_tree(FibonacciHeapNode *x) {
  if (last_node == nullptr) {
    last_node = x;
  } else {
    last_node->after = x;
    x->before = last_node;
    last_node = x;
  }
}
