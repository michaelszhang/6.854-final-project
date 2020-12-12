#include "binary_heap.h"

#include <stdexcept>

#include "item.h"

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

std::vector<Item> BinaryHeap::delete_k(unsigned k) {
  // TODO[jerry]
  (void) k;
  return std::vector<Item>();
}

unsigned BinaryHeap::size() const {
  return nodes.size();
}

void BinaryHeap::swap_idx(unsigned i1, unsigned i2) {
  std::swap(nodes[i1], nodes[i2]);
  nodes[i1]->idx = i1;
  nodes[i2]->idx = i2;
}
