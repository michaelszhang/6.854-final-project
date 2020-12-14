#ifndef _HEAP_ADAPTER_H
#define _HEAP_ADAPTER_H

#include <memory>
#include <vector>

#include "heap.h"
#include "item.h"

template<typename T>
class HeapAdapter : virtual public IHeap {
  public:
    HeapAdapter()
      : heap(new T)
    {}

    INode* insert(const Item& item) override { return heap->insert(item); }
    void decrease_key(INode* node, const Item& item) override { heap->decrease_key(node, item); }
    Item delete_min() override { return heap->delete_min(); }

    std::vector<Item> select_k(unsigned k) override {
      // This is actually delete_k, but not renaming yet for fear of version control conflict
      std::vector<Item> result;
      for (unsigned i = 0; i < k; ++i) {
        result.push_back(delete_min());
      }
      return result;
    }

    unsigned size() const override { return heap->size(); }

  private:
    std::unique_ptr<IHeap> heap;
};

#endif  // _HEAP_ADAPTER_H
