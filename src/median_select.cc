#include "median_select.h"

#include <algorithm>
#include <list>
#include <stdexcept>
#include <vector>

INode* MedianSelect::insert(const Item& item) {
  items.emplace_back(item);
  return &items.back();
}

void MedianSelect::decrease_key(INode* node, const Item& item) {
  MedianSelectItem* rep = dynamic_cast<MedianSelectItem*>(node);
  if (rep == nullptr) {
    throw std::runtime_error("Invalid decrease-key node");
  }
  rep->value = item;
}

Item MedianSelect::delete_min() {
  if (size() == 0) {
    throw std::runtime_error("Delete from empty structure");
  }
  auto it = items.begin();
  for (auto it2 = ++items.begin(); it2 != items.end(); ++it2) {
    if (it2->value < it->value) {
      it = it2;
    }
  }
  Item result = it->value;
  items.erase(it);
  return result;
}

std::vector<Item> MedianSelect::delete_k(unsigned k) {
  if (k > size()) {
    throw std::out_of_range("Not enough elements to delete");
  } else if (k == size()) {
    std::vector<Item> result;
    for (const MedianSelectItem& item: items) {
      result.push_back(item.value);
    }
    return result;
  }
  std::vector<ItemIterator> all;
  for (auto it = items.begin(); it != items.end(); ++it) {
    all.push_back(it);
  }
  all = select_k(all, k);
  std::vector<Item> result;
  for (ItemIterator it: all) {
    result.push_back(it->value);
    items.erase(it);
  }
  return result;
}

unsigned MedianSelect::size() const {
  return items.size();
}

std::vector<MedianSelect::ItemIterator> MedianSelect::select_k(std::vector<ItemIterator>& list, unsigned k) {
  if (list.size() <= 4) {
    // TODO[jerry]: optimize this
    std::sort(list.begin(), list.end(), [](ItemIterator a, ItemIterator b)->bool { return a->value < b->value; });
    return std::vector<ItemIterator>(list.begin(), list.begin()+k);
  }

  std::vector<ItemIterator> group_medians;
  for (unsigned i = 0; ; i+=5) {
    if (i+5 <= list.size()) {
      group_medians.push_back(median_of_five(list.begin()+i, list.begin()+i+5));
    } else {
      for (unsigned j = i; j < list.size(); ++j) {
        group_medians.push_back(list[j]);
      }
      break;
    }
  }
  ItemIterator mid = select_k(group_medians, (group_medians.size()+1) / 2).back();
  std::vector<ItemIterator> smaller, larger;
  for (ItemIterator item: list) {
    if (item->value < mid->value) {
      smaller.push_back(item);
    } else if (mid != item) {
      larger.push_back(item);
    }
  }
  if (smaller.size()+1 > k) {
    return select_k(smaller, k);
  } else {
    smaller.push_back(mid);
    if (smaller.size() == k) {
      return smaller;
    } else {
      std::vector<ItemIterator> more = select_k(larger, k-smaller.size());
      smaller.insert(smaller.end(), more.begin(), more.end());
      return smaller;
    }
  }
}

MedianSelect::ItemIterator MedianSelect::median_of_five(std::vector<ItemIterator>::iterator begin, std::vector<ItemIterator>::iterator end) {
  // TODO[jerry]: optimize this
  std::sort(begin, end, [](ItemIterator a, ItemIterator b)->bool { return a->value < b->value; });
  return *(begin+2);
}
