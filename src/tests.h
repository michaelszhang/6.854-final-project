#ifndef _TESTS_H
#define _TESTS_H

#include <functional>
#include <memory>
#include <stdio.h>
#include <vector>

#include "binary_heap.h"
#include "fibonacci_heap.h"
#include "heap.h"
#include "item.h"

class TestRegistrar {
  public:
    TestRegistrar(const std::function<void()>& test) {
      all_tests().push_back(test);
    }

    static std::vector<std::function<void()>>& all_tests() {
      static std::vector<std::function<void()>> tests;
      return tests;
    }
};

inline void run_all_tests() {
  for (const auto& test: TestRegistrar::all_tests()) {
    test();
  }
}

#define MAKE_TEST(test_name, heap) \
  void test_name(IHeap* heap); \
  class Testlib_##test_name { \
    public: \
      static void run_testlib() { \
        DRIVE_TEST(test_name, BinaryHeap); \
        DRIVE_TEST(test_name, FibonacciHeap); \
      } \
      static void run_single_test(IHeap* heap); \
  }; \
  TestRegistrar registrar_instance_##test_name(Testlib_##test_name::run_testlib); \
  void Testlib_##test_name::run_single_test(IHeap* heap)

#define DRIVE_TEST(test_name, heap_type) \
  do { \
    printf("======= Beginning test: %s/%s\n", #test_name, #heap_type); \
    Item::reset_statistics(); \
    std::unique_ptr<IHeap> heap(new heap_type()); \
    run_single_test(heap.get()); \
    printf("======= Ending test: %s/%s\n\n", #test_name, #heap_type); \
  } while (0)

#define EXPECT_DELETE(heap, value) \
  do { \
    int v = heap->delete_min().get_value(); \
    if (v != value) { \
      printf("Test failed: expected to delete %d, not %d\n", value, v); \
    } \
  } while (0)

#define ASSERT_DELETE(heap, value) \
  do { \
    int v = heap->delete_min().get_value(); \
    if (v != value) { \
      printf("Test failed: expected to delete %d, not %d\n", value, v); \
      printf("Aborting test.\n"); \
      return; \
    } \
  } while (0)

#endif  // _TESTS_H
