#include "fibonacci_heap.h"

#include <algorithm>
#include <iostream>
#include <vector>

FibonacciHeap::FibonacciHeap()
  : nodes(0), root(nullptr) {
  for (unsigned i = 0; i < MAX_RANK; i++) {
    rank_array[i] = nullptr;
  }
}

FibonacciHeap::~FibonacciHeap() {
  delete root;
  for (unsigned i = 0; i < MAX_RANK; i++) {
    delete rank_array[i];
  }
}

INode* FibonacciHeap::insert(const Item& item) {
  FibonacciHeapNode* x = new FibonacciHeapNode(item);
  if (root == nullptr) {
    root = x;
  } else {
    root = link(x, root);
  }
  nodes++;
  return x;
}

void FibonacciHeap::decrease_key(INode* node, const Item& item) {
  FibonacciHeapNode* rep = dynamic_cast<FibonacciHeapNode*>(node);
  rep->value = item;
  if (rep == root) {//|| rep->parent->value <= item) { // heap-order heuristic
    return;
  }
  if (rep->rank < rep->parent->rank) {
    root->marked = false;
    decrease_ranks(rep);
  }
  /*
  root->marked = false;
  decrease_ranks(rep);*/
  cut(rep);
  root = link(rep, root);
}

Item FibonacciHeap::delete_min() {
  Item min_item = root->value;
  FibonacciHeapNode *x = root->child;
  unsigned max_rank = 0;
  while (x != nullptr) {
    FibonacciHeapNode *y = x;
    x = x->after;
    while (rank_array[y->rank] != nullptr) {
      y = link(y, rank_array[y->rank]);
      rank_array[y->rank] = nullptr;
      y->rank += 1;
    }
    rank_array[y->rank] = y;
    max_rank = std::max(max_rank, y->rank);
  }
  for (unsigned i = 0; i <= max_rank; i++) {
    if (rank_array[i] != nullptr) {
      if (x == nullptr) {
        x = rank_array[i];
      } else {
        x = link(x, rank_array[i]);
      }
      rank_array[i] = nullptr;
    }
  }
  root = x;
  nodes--;
  return min_item;
}

std::vector<Item> FibonacciHeap::delete_k(unsigned k) {
  // TODO[george]
  (void) k;
  nodes -= k;
  return std::vector<Item>();
}

unsigned FibonacciHeap::size() const {
  return nodes;
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
}

void FibonacciHeap::add_child(FibonacciHeapNode *x, FibonacciHeapNode *y) {
  x->parent = y;
  FibonacciHeapNode *z = y->child;
  x->before = nullptr;
  x->after = z;
  if (z != nullptr) {
    z->before = x;
  }
  y->child = x;
}

void FibonacciHeap::decrease_ranks(FibonacciHeapNode *x) {
  unsigned k;
  do {
    x = x->parent;
    x->marked = !x->marked;
    k = x->rank;
    x->rank -= 1;
  } while(!x->marked || k >= x->parent->rank);
  /*
  do {
    x = x->parent;
    if (x->rank > 0) {
      x->rank -= 1;
    }
    x->marked = !x->marked;
  } while(!x->marked);*/
}
