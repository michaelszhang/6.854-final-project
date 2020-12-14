#include "binary_heap.h"

#include <algorithm>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "item.h"
#include "soft_heap.h"

BinaryHeap::~BinaryHeap() {
  for (BinaryHeapNode* node: nodes) {
    delete node;
  }
  nodes.clear();
}

INode* BinaryHeap::insert(const Item& item) {
  unsigned idx = nodes.size();
  nodes.push_back(new BinaryHeapNode(item, idx));
  while (idx) {
    unsigned parent = (idx - 1) / 2;
    if (nodes[idx]->value < nodes[parent]->value) {
      swap_idx(idx, parent);
      idx = parent;
    } else {
      break;
    }
  }
  return nodes[idx];
}

void BinaryHeap::decrease_key(INode* node, const Item& item) {
  BinaryHeapNode* rep = dynamic_cast<BinaryHeapNode*>(node);
  if (rep == nullptr) {
    throw std::runtime_error("Invalid decrease-key node");
  }
  if (rep->value < item) {
    throw std::runtime_error("Decrease-key must decrease value");
  }
  rep->value = item;
  unsigned idx = rep->idx;
  while (idx) {
    unsigned parent = (idx - 1) / 2;
    if (nodes[idx]->value < nodes[parent]->value) {
      swap_idx(idx, parent);
      idx = parent;
    } else {
      break;
    }
  }
}

Item BinaryHeap::delete_min() {
  if (size() == 0) {
    throw std::runtime_error("Delete from empty tree");
  }
  swap_idx(0, size()-1);
  Item result = nodes.back()->value;
  delete nodes.back();
  nodes.pop_back();
  for (unsigned idx = 0; ;) {
    unsigned l = (idx * 2 + 1), r = (idx * 2 + 2);
    Item vl = (l < size()) ? nodes[l]->value : Item();
    Item vr = (r < size()) ? nodes[r]->value : Item();
    if (l < size()) {
      if (vl < vr) {
        if (vl < nodes[idx]->value) {
          swap_idx(idx, l);
          idx = l;
          continue;
        }
      } else {
        if (vr < nodes[idx]->value) {
          swap_idx(idx, r);
          idx = r;
          continue;
        }
      }
    }
    break;
  }
  return result;
}

std::vector<Item> BinaryHeap::delete_k(unsigned) {
  throw std::runtime_error("not implemented");
}

unsigned BinaryHeap::size() const {
  return nodes.size();
}

void BinaryHeap::swap_idx(unsigned i1, unsigned i2) {
  std::swap(nodes[i1], nodes[i2]);
  nodes[i1]->idx = i1;
  nodes[i2]->idx = i2;
}

std::vector<Item> BinaryHeap::select_k(unsigned k) {
  std::vector<Item> result;
  std::unordered_map<const Item*, unsigned> contents;
  std::vector<unsigned> todo;
  SoftHeap q(0.25);

  auto push_item = [this, &result, &contents, &todo, &q]() {
    unsigned idx = todo.back();
    todo.pop_back();
    if (idx >= nodes.size()) {
      return;
    }

    result.push_back(nodes[idx]->value);
    contents[&nodes[idx]->value] = idx;

    SoftHeap::CorruptionList corrupted = q.insert(nodes[idx]->value);
    for (const Item* item: corrupted) {
      todo.push_back(contents[item] * 2 + 1);
      todo.push_back(contents[item] * 2 + 2);
    }
  };

  todo.push_back(0);
  push_item();
  if (!todo.empty()) {
    throw std::runtime_error("corrupted after first insert!?");
  }
  for (unsigned i = 1; i < k; ++i) {
    const SoftHeapEntry& min = q.find_min();
    if (!min.corrupted) {
      todo.push_back(contents[&min.item] * 2 + 1);
      todo.push_back(contents[&min.item] * 2 + 2);
    }
    SoftHeap::CorruptionList corrupted = q.delete_min();
    for (const Item* item: corrupted) {
      todo.push_back(contents[item] * 2 + 1);
      todo.push_back(contents[item] * 2 + 2);
    }
    while (!todo.empty()) {
      push_item();
    }
  }
  // TODO[jerry]: do this better
  std::sort(result.begin(), result.end());
  while (result.size() > k) result.pop_back();
  return result;
}
