#ifndef _ITEM_H
#define _ITEM_H

class Item {
  public:
    Item(int value)
      : value(value)
    {}

    int getValue() const {
      return value;
    }

    bool operator<(const Item& other) const {
      ++nComparisons;
      return (value < other.value);
    }

    static int getNumComparisons() {
      return nComparisons;
    }

  private:
    static int nComparisons;
    int value;
};

int Item::nComparisons = 0;

#endif  // _ITEM_H
