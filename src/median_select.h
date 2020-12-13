#ifndef _MEDIAN_SELECT_H
#define _MEDIAN_SELECT_H

#include <list>
#include <vector>

#include "heap.h"

class MedianSelect : virtual public IHeap {
  public:
    INode* insert(const Item&) override;
    void decrease_key(INode*, const Item&) override;
    Item delete_min() override;
    std::vector<Item> select_k(unsigned k) override;

    unsigned size() const override;

  private:
    class MedianSelectItem : virtual public INode {
      public:
        MedianSelectItem(const Item& value)
          : INode(value)
        {}
    };

    static std::vector<Item> select_k(std::vector<Item>&, unsigned);

    std::list<MedianSelectItem> items;
};

#endif  // _MEDIAN_SELECT_H
