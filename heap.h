#ifndef _HEAP_H
#define _HEAP_H

#include <vector>

template<typename T>
class IHeap {
  public:
    virtual ~IHeap() {}

    virtual void insert(T) = 0;
    virtual T delete_min() = 0;
    virtual std::vector<T> delete_k(int k) = 0;

    virtual unsigned size() const = 0;
};

#endif  // _HEAP_H
