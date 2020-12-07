#ifndef _BINARY_HEAP_H
#define _BINARY_HEAP_H

#include "heap.h"

template<typename T>
class BinaryHeap : virtual public IHeap<T> {
  public:
    BinaryHeap();

    void insert(const T&) override;
    T delete_min() override;
    std::vector<T> delete_k() override;

    unsigned size() const override;

  private:
    struct Node {
      Node *left, *right;
      T value;
    };

    Node* root;
};

#endif  // _BINARY_HEAP_H
