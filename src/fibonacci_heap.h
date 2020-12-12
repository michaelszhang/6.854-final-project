#ifndef _FIBONACCI_HEAP_H
#define _FIBONACCI_HEAP_H

#include "heap.h"
#include "item.h"

class FibonacciHeap : virtual public IHeap {
  public:
    FibonacciHeap();
    ~FibonacciHeap();

    INode* insert(const Item&) override;
    void decrease_key(INode*, const Item&) override;
    Item delete_min() override;
    std::vector<Item> delete_k(unsigned k) override;

    unsigned size() const override;

  private:
    class FibonacciHeapNode : virtual public INode {
      public:
    };

    FibonacciHeapNode* root;
};

#endif  // _FIBONACCI_HEAP_H
