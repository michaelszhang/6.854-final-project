#ifndef _HEAP_H
#define _HEAP_H

#include <vector>

template<typename T>
class INode {
  public:
    INode(const T& value) : value(value) {}
    virtual ~INode() {}
    T value;
};

template<typename T>
class IHeap {
  public:
    virtual ~IHeap() {}

    virtual const INode<T>* insert(const T&) = 0;
    virtual T delete_min() = 0;
    virtual std::vector<T> delete_k(unsigned k) = 0;

    virtual unsigned size() const = 0;
};

#endif  // _HEAP_H
