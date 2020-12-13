#ifndef _FIBONACCI_HEAP_H
#define _FIBONACCI_HEAP_H

#include <vector>

#include "heap.h"
#include "item.h"

class FibonacciHeap : virtual public IHeap {
  public:
    FibonacciHeap();
    ~FibonacciHeap();

    INode* insert(const Item&) override;
    void decrease_key(INode*, const Item&) override;
    Item delete_min() override;
    std::vector<Item> select_k(unsigned k) override;

    unsigned size() const override;

  private:
    static constexpr int MAX_RANK = 100;

    class FibonacciHeapNode : virtual public INode {
      public:
        bool marked;
        unsigned rank;
        FibonacciHeapNode *child, *parent, *before, *after;

        FibonacciHeapNode(const Item& value)
          : INode(value), marked(false), rank(0), child(nullptr)
        {}
    };

    int nodes;
    FibonacciHeapNode* root;
    FibonacciHeapNode* rank_array[MAX_RANK];

    FibonacciHeapNode* link(FibonacciHeapNode *x, FibonacciHeapNode *y);
    void cut(FibonacciHeapNode *x);
    void add_child(FibonacciHeapNode *x, FibonacciHeapNode *y);
    void decrease_ranks(FibonacciHeapNode *x);
};

#endif  // _FIBONACCI_HEAP_H
