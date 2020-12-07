#ifndef _BINARY_HEAP_H
#define _BINARY_HEAP_H

#include "heap.h"

template<typename T>
class BinaryHeap : virtual public IHeap<T> {
  public:
    BinaryHeap();

    const INode<T>* insert(const T&) override;
    void decrease_key(const INode<T>* node) override;
    T delete_min() override;
    std::vector<T> delete_k(unsigned k) override;

    unsigned size() const override;

  private:
    class BinaryHeapNode : public INode<T> {
      public:
        BinaryHeapNode *left, *right, *parent;
        int size;

        BinaryHeapNode(const T& value, BinaryHeapNode* parent)
          : INode<T>(value), left(nullptr), right(nullptr), parent(parent), size(1)
        {}
    };

    const BinaryHeapNode* insert(BinaryHeapNode*& base, BinaryHeapNode* parent, const T& item);
    unsigned size(const BinaryHeapNode*) const;

    BinaryHeapNode* root;
};

#endif  // _BINARY_HEAP_H
