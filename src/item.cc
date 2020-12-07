#include "item.h"

Item::Item(int value)
  : value(value)
{}

int Item::getValue() const {
  return value;
}

bool Item::operator<(const Item& other) const {
  ++nComparisons;
  return (value < other.value);
}

int Item::getNumComparisons() {
  return nComparisons;
}

int Item::nComparisons = 0;
