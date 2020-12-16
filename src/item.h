#ifndef _ITEM_H
#define _ITEM_H

#include <stdint.h>

#include <limits>

class Item {
  public:
    Item(int value=std::numeric_limits<int>::max());

    int get_value() const;
    bool operator<(const Item& other) const;

    static void dump_statistics();
    static void reset_statistics();

  private:
    static int64_t n_comparisons;
    int value;
};

#endif  // _ITEM_H
