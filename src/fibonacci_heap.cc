#include "fibonacci_heap.h"
#include "soft_heap.h"
#include "median_select.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

FibonacciHeap::FibonacciHeap()
    : heap_size(0), min_node(nullptr), last_node(nullptr)
{
  for (unsigned i = 0; i < MAX_RANK; i++)
  {
    rank_array[i] = nullptr;
  }
}

FibonacciHeap::~FibonacciHeap()
{
  // TBD
}

void FibonacciHeap::print_tree() {
  if (heap_size == 0)
    return;
  FibonacciHeapNode *tmp = last_node;
  std::cout << "TREE " << heap_size << std::endl;
  while (tmp != nullptr) {
    if (tmp->after == tmp) {
      throw std::runtime_error("Self Loop");
    }
    std::cout << tmp->value.get_value() << ' ' << tmp->rank << std::endl;
    tmp = tmp->after;
  }
  std::cout << std::endl;
}

INode *FibonacciHeap::insert(const Item &item)
{
  FibonacciHeapNode *x = new FibonacciHeapNode(item);
  maintain_min(x);
  push_tree(x);
  heap_size++;
  return x;
}

void FibonacciHeap::decrease_key(INode *node, const Item &item)
{
  FibonacciHeapNode *rep = dynamic_cast<FibonacciHeapNode *>(node), *x;
  if (rep == nullptr)
  {
    throw std::runtime_error("Invalid decrease-key node");
  }
  if (rep->value < item)
  {
    throw std::runtime_error("Decrease-key must decrease value");
  }
  rep->value = item;
  maintain_min(rep);
  if (rep->parent == nullptr)
  {
    return;
  }
  x = rep->parent;
  cut(rep);
  push_tree(rep);
  while (x->marked)
  { // root is always unmarked
    rep = x;
    x = x->parent;
    cut(rep);
    push_tree(rep);
  }
  if (x->parent != nullptr)
  {
    x->marked = true;
  }
}

Item FibonacciHeap::delete_min()
{
  if (size() == 0)
  {
    throw std::runtime_error("Delete from empty tree");
  }
  Item min_item = min_node->value;
  FibonacciHeapNode *x = last_node;
  unsigned max_rank = 0;
  while (x != nullptr)
  {
    FibonacciHeapNode *y = x;
    x = x->after;
    if (y != min_node)
    {
      y = propagate_link(y);
      max_rank = std::max(max_rank, y->rank);
    }
  }
  x = min_node->child;
  delete min_node;
  min_node = last_node = nullptr;
  while (x != nullptr)
  {
    FibonacciHeapNode *y = x;
    x = x->after;
    y = propagate_link(y);
    max_rank = std::max(max_rank, y->rank);
  }
  coalesce_nodes(max_rank);
  heap_size--;
  return min_item;
}

std::vector<Item> FibonacciHeap::delete_k(unsigned k)
{
  FibonacciHeapNode *root = new FibonacciHeapNode(Item(std::numeric_limits<int>::min()));
  root->child = last_node;
  std::vector<Item> result;
  std::vector<FibonacciHeapNode *> result_nodes;
  std::unordered_map<const Item *, FibonacciHeapNode *> contents;
  std::vector<FibonacciHeapNode *> todo;
  SoftHeap q(0.125);

  auto heapify_children = [](FibonacciHeapNode *child) {
    std::vector<FibonacciHeapNode *> nodes;
    while (child != nullptr)
    {
      nodes.push_back(child);
      child = child->after;
    }
    int rank = (int)nodes.size();
    for (int i = rank / 2 - 1; i >= 0; i--)
    {
      for (unsigned idx = i;;)
      {
        int l = (idx * 2 + 1), r = (idx * 2 + 2);
        Item vl = (l < rank) ? nodes[l]->value : Item();
        Item vr = (r < rank) ? nodes[r]->value : Item();
        if (l < rank)
        {
          if (vl < vr)
          {
            if (vl < nodes[idx]->value)
            {
              std::swap(nodes[l], nodes[idx]);
              idx = l;
              continue;
            }
          }
          else
          {
            if (vr < nodes[idx]->value)
            {
              std::swap(nodes[r], nodes[idx]);
              idx = r;
              continue;
            }
          }
        }
        break;
      }
    }
    for (FibonacciHeapNode *node : nodes)
    {
      node->left = node->right = nullptr;
    }
    for (int i = rank / 2 - 1; i >= 0; i--)
    {
      int l = (i * 2 + 1), r = (i * 2 + 2);
      nodes[i]->left = nodes[l];
      if (r < rank)
      {
        nodes[i]->right = nodes[r];
      }
    }
    return nodes[0];
  };

  std::set<int> inserted;
  auto update_todo = [&contents, &todo, &heapify_children, &inserted](const Item *item) {
    FibonacciHeapNode *x = contents[item];
    if (x->left != nullptr)
    {
      todo.push_back(x->left);
      if (!inserted.insert(x->left->value.get_value()).second)
      {
        throw std::runtime_error("disaster1");
      }
    }
    if (x->right != nullptr)
    {
      todo.push_back(x->right);
      if (!inserted.insert(x->right->value.get_value()).second)
      {
        throw std::runtime_error("disaster2");
      }
    }
    if (x->child != nullptr)
    {
      todo.push_back(heapify_children(x->child));
      if (!inserted.insert(todo.back()->value.get_value()).second)
      {
        throw std::runtime_error("disaster3");
      }
    }
  };

  std::set<int> qwer;
  auto push_item = [&result, &contents, &todo, &q, &update_todo, &result_nodes, &qwer]() {
    FibonacciHeapNode *node = todo.back();
    todo.pop_back();
    result.push_back(node->value);
    result_nodes.push_back(node);
    contents[&node->value] = node;
    SoftHeap::CorruptionList corrupted = q.insert(node->value);

    if (!qwer.insert(node->value.get_value()).second)
    {
      throw std::runtime_error("disaster4");
    }

    for (const Item *item : corrupted)
    {
      update_todo(item);
    }
  };

  todo.push_back(root);
  push_item();
  result.pop_back(); // remove root
  if (!todo.empty())
  {
    throw std::runtime_error("corrupted after first insert!?");
  }
  int min_cnt = 0;
  for (unsigned i = 1; i <= k; ++i)
  {
    const SoftHeapEntry &min = q.find_min();
    if (!min.corrupted)
    {
      min_cnt++;
      update_todo(&min.item);
    }
    SoftHeap::CorruptionList corrupted = q.delete_min();
    for (const Item *item : corrupted)
    {
      update_todo(item);
    }
    while (!todo.empty())
    {
      push_item();
    }
  }

  MedianSelect med_sel;
  for (auto item : result)
  {
    med_sel.insert(item);
  }
  result = med_sel.delete_k(k);

  Item mx = result[0];
  for (auto item : result)
  {
    if (mx < item)
      mx = item;
  }

  std::unordered_set<FibonacciHeapNode *> k_result_nodes;
  std::vector<FibonacciHeapNode *> k_result_nodes_vec;
  for (auto node : result_nodes)
  {
    if (!(mx < node->value))
    {
      k_result_nodes.emplace(node);
      k_result_nodes_vec.push_back(node);
    }
  }

  unsigned max_rank = 0;
  min_node = last_node = nullptr;
  for (auto node : k_result_nodes_vec)
  {
    FibonacciHeapNode *child = node->child;
    while (child != nullptr)
    {
      FibonacciHeapNode *c = child;
      child = child->after;
      if (k_result_nodes.find(c) != k_result_nodes.end())
        continue;
      propagate_link(c);
      max_rank = std::max(max_rank, c->rank);
    }
  }
  coalesce_nodes(max_rank);

  for (auto node : k_result_nodes_vec)
  {
    delete node;
  }
  heap_size -= k;
  return result;
}

unsigned FibonacciHeap::size() const
{
  return heap_size;
}

void FibonacciHeap::add_child(FibonacciHeapNode *x, FibonacciHeapNode *y)
{
  x->parent = y;
  FibonacciHeapNode *z = y->child;
  x->after = z;
  if (z != nullptr)
  {
    z->before = x;
  }
  y->child = x;
  y->rank++;
}

FibonacciHeap::FibonacciHeapNode *FibonacciHeap::link(FibonacciHeapNode *x, FibonacciHeapNode *y)
{
  if (y->value < x->value)
  {
    add_child(x, y);
    return y;
  }
  else
  {
    add_child(y, x);
    return x;
  }
}

FibonacciHeap::FibonacciHeapNode *FibonacciHeap::propagate_link(FibonacciHeapNode *x)
{
  x->marked = false;
  x->parent = x->before = x->after = nullptr; // clear orignal links
  while (rank_array[x->rank] != nullptr)
  {
    int prev_rank = x->rank;
    x = link(x, rank_array[x->rank]);
    rank_array[prev_rank] = nullptr;
  }
  rank_array[x->rank] = x;
  return x;
}

void FibonacciHeap::coalesce_nodes(unsigned max_rank)
{
  for (unsigned i = 0; i <= max_rank; i++)
  {
    if (rank_array[i] != nullptr)
    {
      push_tree(rank_array[i]);
      maintain_min(rank_array[i]);
      rank_array[i] = nullptr;
    }
  }
}

void FibonacciHeap::cut(FibonacciHeapNode *x)
{
  x->marked = false;
  FibonacciHeapNode *y = x->parent;
  if (y->child == x)
  {
    y->child = x->after;
  }
  if (x->before != nullptr)
  {
    x->before->after = x->after;
  }
  if (x->after != nullptr)
  {
    x->after->before = x->before;
  }
  x->before = x->after = nullptr;
  x->parent = nullptr;
  y->rank--;
}

void FibonacciHeap::maintain_min(FibonacciHeapNode *x)
{
  if (min_node == nullptr)
  {
    min_node = x;
  }
  else if (x->value < min_node->value)
  {
    min_node = x;
  }
}

void FibonacciHeap::push_tree(FibonacciHeapNode *x)
{
  if (last_node == nullptr)
  {
    last_node = x;
  }
  else
  {
    last_node->before = x;
    x->after = last_node;
    last_node = x;
  }
}
