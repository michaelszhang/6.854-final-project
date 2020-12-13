#ifndef _SOFT_HEAP_H
#define _SOFT_HEAP_H

#include <limits>
#include <list>
#include <memory>

#include "item.h"

struct SoftHeapEntry {
  const Item& item;
  bool corrupted = false;

  SoftHeapEntry(const Item& item)
    : item(item)
  {}

  SoftHeapEntry(const SoftHeapEntry&) = delete;
  SoftHeapEntry& operator=(const SoftHeapEntry&) = delete;
};

class SoftHeap {
  public:
    typedef std::list<const Item*> CorruptionList;

    SoftHeap(double epsilon);

    CorruptionList insert(Item);
    const SoftHeapEntry& find_min() const;
    CorruptionList delete_min();

  private:
    struct SoftHeapNode {
      SoftHeapNode* next = nullptr;
      std::unique_ptr<SoftHeapNode> left = nullptr;
      std::unique_ptr<SoftHeapNode> right = nullptr;
      unsigned rank = std::numeric_limits<unsigned>::max();
      std::list<SoftHeapEntry> set;
      Item key;

      SoftHeapNode() {}
      SoftHeapNode(const Item& item)
        : rank(0), set(), key(item) {
          set.emplace_back(item);
      }
    };

    void defill(SoftHeapNode* node, CorruptionList& corrupted);
    void fill(SoftHeapNode* node, CorruptionList& corrupted);

    static SoftHeapNode* rank_swap(SoftHeapNode* node);
    static SoftHeapNode* key_swap(SoftHeapNode* node);

    static SoftHeapNode* reorder(SoftHeapNode* node, unsigned k);

    SoftHeapNode* meldable_insert(SoftHeapNode* tree, SoftHeapNode* heap, CorruptionList& corrupted);
    SoftHeapNode* link(SoftHeapNode* tree1, SoftHeapNode* tree2, CorruptionList& corrupted);

    const unsigned T;
    SoftHeapNode* root;
};

#endif  // _SOFT_HEAP_H
