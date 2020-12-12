#ifndef _HEAP_H
#define _HEAP_H

#include <vector>

#include "item.h"

class INode {
  public:
    INode(const Item& value) : value(value) {}
    virtual ~INode() {}
    Item value;
};

class IHeap {
  public:
    virtual ~IHeap() {}

    virtual INode* insert(const Item&) = 0;
    virtual void decrease_key(INode* n, const Item&) = 0;
    virtual Item delete_min() = 0;
    virtual std::vector<Item> delete_k(unsigned k) = 0;

    virtual unsigned size() const = 0;
};

#endif  // _HEAP_H
