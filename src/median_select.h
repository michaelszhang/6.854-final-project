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
    std::vector<Item> delete_k(unsigned k) override;

    unsigned size() const override;

  private:
    class MedianSelectItem : virtual public INode {
      public:
        MedianSelectItem(const Item& value)
          : INode(value)
        {}
    };

    typedef std::list<MedianSelectItem>::iterator ItemIterator;

    static std::vector<ItemIterator> select_k(std::vector<ItemIterator>&, unsigned);
    static ItemIterator median_of_five(std::vector<ItemIterator>::iterator begin, std::vector<ItemIterator>::iterator end);

    std::list<MedianSelectItem> items;
};

#endif  // _MEDIAN_SELECT_H
