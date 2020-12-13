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

    class MedianSelectItem : virtual public INode {
      public:
        MedianSelectItem(const Item& value)
          : INode(value)
        {}
    };

  private:
    /* Find k'th smallest item in a list, and if desired,
     * delete all items strictly smaller than it from the data structure,
     * returning the set of deleted items
     */
    std::list<MedianSelectItem>::iterator find_kth(
        std::vector<std::list<MedianSelectItem>::iterator>& list,
        unsigned k,
        std::vector<Item>* deleted = nullptr);

    std::list<MedianSelectItem> items;
};

#endif  // _MEDIAN_SELECT_H
