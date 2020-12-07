#include "binary_heap.h"

#include <algorithm>
#include <stdexcept>

#include "item.h"

template<typename T>
BinaryHeap<T>::BinaryHeap()
  : root(nullptr)
{}

template<typename T>
const INode<T>* BinaryHeap<T>::insert(const T& item) {
  return static_cast<const INode<T>*>(insert(root, nullptr, item));
}

template<typename T>
void BinaryHeap<T>::decrease_key(const INode<T>* node) {
  BinaryHeapNode* at = const_cast<BinaryHeapNode*>(dynamic_cast<const BinaryHeapNode*>(node));
  if (at == nullptr) {
    throw std::runtime_error("Invalid decrease-key node");
  }
  while (at->parent != nullptr && at->value < at->parent->value) {
    std::swap(at->value, at->parent->value);
    at = at->parent;
  }
  if (at->parent == nullptr) {
    root = at;
  }
}

template<typename T>
T BinaryHeap<T>::delete_min() {
  if (root == nullptr) {
    throw std::logic_error("Delete from empty tree");
  }
  T result = root->value;
  for (auto at = root;;) {
    if (at->left == nullptr && at->right == nullptr) {
      if (at->parent == nullptr) {
        root = nullptr;
      } else {
        ((at->parent->left == at) ? at->parent->left : at->parent->right) = nullptr;
      }
      delete at;
      break;
    } else if (at->right == nullptr || (at->left != nullptr && at->left->value < at->right->value)) {
      at->value = at->left->value;
      at = at->left;
    } else {
      at->value = at->right->value;
      at = at->right;
    }
  }
  return result;
}

template<typename T>
std::vector<T> BinaryHeap<T>::delete_k(unsigned k) {
  if (size() < k) {
    throw std::out_of_range("k too large for delete-k");
  }
  std::vector<T> result;
  result.reserve(k);
  for (unsigned i = 0; i < k; ++i)
    result.push_back(delete_min());
  return result;
}

template<typename T>
unsigned BinaryHeap<T>::size() const {
  return size(root);
}

template<typename T>
const typename BinaryHeap<T>::BinaryHeapNode* BinaryHeap<T>::insert(BinaryHeapNode*& base, BinaryHeapNode* parent, const T& item) {
  if (base == nullptr) {
    base = new BinaryHeapNode(item, parent);
    return base;
  }

  const T next = std::max(base->value, item);
  base->value = std::min(base->value, item);
  base->size += 1;

  return (size(base->left) < size(base->right))
    ? insert(base->left, base, next)
    : insert(base->right, base, next);
}

template<typename T>
unsigned BinaryHeap<T>::size(const BinaryHeapNode* node) const {
  return (node == nullptr) ? 0 : node->size;
}

template class BinaryHeap<Item>;
