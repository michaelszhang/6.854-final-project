#include "item.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

Item::Item(int value)
  : value(value)
{}

int Item::get_value() const {
  return value;
}

bool Item::operator<(const Item& other) const {
  ++n_comparisons;
  return (value < other.get_value());
}

void Item::dump_statistics() {
  printf("Number of comparisons: %" PRId64 "\n", n_comparisons);
}

void Item::reset_statistics() {
  n_comparisons = 0;
}

int64_t Item::n_comparisons = 0;
