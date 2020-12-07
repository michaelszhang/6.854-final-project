#ifndef _ITEM_H
#define _ITEM_H

#include <limits>

class Item {
  public:
    Item(int value=std::numeric_limits<int>::max());

    int getValue() const;
    bool operator<(const Item& other) const;

    static int getNumComparisons();

  private:
    static int nComparisons;
    int value;
};

#endif  // _ITEM_H
