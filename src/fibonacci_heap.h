#ifndef _FIBONACCI_HEAP_H
#define _FIBONACCI_HEAP_H

#include <vector>

#include "heap.h"
#include "item.h"

class FibonacciHeap : virtual public IHeap {
  public:
    FibonacciHeap();

    INode* insert(const Item&) override;
    void decrease_key(INode*, const Item&) override;
    Item delete_min() override;
    std::vector<Item> delete_k(unsigned k) override;

    unsigned size() const override;

  private:
    static constexpr int MAX_RANK = 100;

    class FibonacciHeapNode : virtual public INode {
      public:
        bool marked;
        unsigned rank;
        FibonacciHeapNode *child, *parent, *before, *after;

        // for select k (middle implicit)
        FibonacciHeapNode *left, *right;

        FibonacciHeapNode(const Item& value)
          : INode(value), marked(false), rank(0),
            child(nullptr), parent(nullptr), before(nullptr), after(nullptr),
            left(nullptr), right(nullptr)
        {}
    };

    int heap_size;
    FibonacciHeapNode *min_node, *last_node;
    FibonacciHeapNode *rank_array[MAX_RANK];

    void add_child(FibonacciHeapNode *x, FibonacciHeapNode *y);
    FibonacciHeapNode* link(FibonacciHeapNode *x, FibonacciHeapNode *y);
    FibonacciHeapNode* propagate_link(FibonacciHeapNode *x);
    void coalesce_nodes(unsigned max_rank);
    void cut(FibonacciHeapNode *x);
    void maintain_min(FibonacciHeapNode *x);
    void push_tree(FibonacciHeapNode *x);
};

#endif  // _FIBONACCI_HEAP_H
