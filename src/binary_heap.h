#ifndef _BINARY_HEAP_H
#define _BINARY_HEAP_H

#include <vector>

#include "heap.h"
#include "item.h"

class BinaryHeap : virtual public IHeap {
  public:
    ~BinaryHeap();

    INode* insert(const Item&) override;
    void decrease_key(INode*, const Item&) override;
    Item delete_min() override;
    std::vector<Item> delete_k(unsigned k) override;

    unsigned size() const override;

  private:
    class BinaryHeapNode : virtual public INode {
      public:
        unsigned idx;

        BinaryHeapNode(const Item& value, unsigned idx)
          : INode(value), idx(idx)
        {}
    };

    std::vector<BinaryHeapNode*> nodes;

    void swap_idx(unsigned i1, unsigned i2);
};

#endif  // _BINARY_HEAP_H
