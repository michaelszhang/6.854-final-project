#include "soft_heap.h"

#include <algorithm>
#include <math.h>
#include <stdexcept>

unsigned compute_T(double epsilon) {
  int value = 3 / epsilon;
  while (__builtin_popcount(value) != 1) {
    value += value & -value;
  }
  return __builtin_ctz(value);
}

SoftHeap::SoftHeap(double epsilon)
  : T(compute_T(epsilon)), root(nullptr)
{}

SoftHeap::CorruptionList SoftHeap::insert(const Item& item) {
  CorruptionList corrupted;
  root = key_swap(meldable_insert(new SoftHeapNode(item), rank_swap(root), corrupted));
  return corrupted;
}

const SoftHeapEntry& SoftHeap::find_min() const {
  if (root == nullptr) {
    throw std::runtime_error("find-min on empty heap");
  }
  return root->set.front();
}

SoftHeap::CorruptionList SoftHeap::delete_min() {
  if (root->set.size() > 1) {
    root->set.pop_front();
    return {};
  } else {
    CorruptionList corrupted;
    root->set.clear();
    unsigned k = root->rank;
    if (root->left == nullptr) {
      SoftHeapNode* next = root->next;
      delete root;
      root = next;
    } else {
      defill(root, corrupted);
    }
    if (root != nullptr) {
      root = reorder(root, k);
    }
    return corrupted;
  }
}

void SoftHeap::defill(SoftHeapNode* node, CorruptionList& corrupted) {
  fill(node, corrupted);
  if (node->rank > T && node->rank % 2 == 0 && node->left != nullptr) {
    fill(node, corrupted);
  }
}

void SoftHeap::fill(SoftHeapNode* node, CorruptionList& corrupted) {
  if (node->left == nullptr) {
    throw std::runtime_error("left child is null");
  }
  if (node->right != nullptr && node->right->key < node->left->key) {
    std::swap(node->left, node->right);
  }

  node->key = node->left->key;
  if (!node->set.empty()) {
    if (!node->set.front().corrupted) {
      corrupted.push_back(&node->set.front().item);
      node->set.front().corrupted = true;
    }
  }
  node->set.splice(node->set.begin(), node->left->set);
  node->left->set.clear();

  if (node->left->left == nullptr) {
    node->left = std::move(node->right);
  } else {
    defill(node->left.get(), corrupted);
  }
}

SoftHeap::SoftHeapNode* SoftHeap::rank_swap(SoftHeapNode* node) {
  if (node == nullptr || node->next == nullptr) {
    return node;
  }
  SoftHeapNode* next = node->next;
  if (next->rank < node->rank) {
    node->next = next->next;
    next->next = node;
    return next;
  } else {
    return node;
  }
}

SoftHeap::SoftHeapNode* SoftHeap::key_swap(SoftHeapNode* node) {
  if (node == nullptr || node->next == nullptr) {
    return node;
  }
  SoftHeapNode* next = node->next;
  if (next->key < node->key) {
    node->next = next->next;
    next->next = node;
    return next;
  } else {
    return node;
  }
}

SoftHeap::SoftHeapNode* SoftHeap::reorder(SoftHeapNode* node, unsigned k) {
  if (node->next != nullptr && node->next->rank < k) {
    node = rank_swap(node);
    node->next = reorder(node->next, k);
  }
  return key_swap(node);
}

SoftHeap::SoftHeapNode* SoftHeap::meldable_insert(SoftHeapNode* tree, SoftHeapNode* heap, CorruptionList& corrupted) {
  if (heap == nullptr || tree->rank < heap->rank) {
    tree->next = key_swap(heap);
    return tree;
  } else {
    return meldable_insert(link(tree, heap, corrupted), rank_swap(heap->next), corrupted);
  }
}

SoftHeap::SoftHeapNode* SoftHeap::link(SoftHeapNode* tree1, SoftHeapNode* tree2, CorruptionList& corrupted) {
  SoftHeapNode* result = new SoftHeapNode;
  result->rank = tree1->rank + 1;
  result->left.reset(tree1);
  result->right.reset(tree2);
  defill(result, corrupted);
  return result;
}
