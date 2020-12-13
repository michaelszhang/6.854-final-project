#ifndef _TESTS_H
#define _TESTS_H

#include <algorithm>
#include <functional>
#include <memory>
#include <stdio.h>
#include <vector>

#include "binary_heap.h"
#include "fibonacci_heap.h"
#include "heap.h"
#include "item.h"
#include "median_select.h"

struct TestFail {
  TestFail(const char* format, ...) {
    fprintf(stderr, "Test failed: ");
    va_list arg;
    va_start(arg, format);
    vfprintf(stderr, format, arg);
    va_end(arg);
    fprintf(stderr, "\n");
  }
};

class TestFixture {
  public:
    static std::vector<std::function<void()>>& test_registrar() {
      static std::vector<std::function<void()>> registrar;
      return registrar;
    }

  protected:
    TestFixture() {
      test_registrar().push_back(std::bind(&TestFixture::run_testlib, this));
    }

    virtual void run_testlib() = 0;
    virtual void run_single_test(IHeap* heap) = 0;

    void expect_delete(IHeap* heap, int value) {
      int result = heap->delete_min().get_value();
      if (result != value) {
        TestFail("expected to delete %d, not %d\n", value, result);
      }
    }

    void assert_delete(IHeap* heap, int value) {
      int result = heap->delete_min().get_value();
      if (result != value) {
        throw TestFail("expected to delete %d, not %d\n", value, result);
      }
    }

    void expect_delete(IHeap* heap, std::vector<int> values) {
      std::sort(values.begin(), values.end());
      if (get_delete_result(heap, values.size()) != values) {
        TestFail("delete k returned incorrect result");
      }
    }

    void assert_delete(IHeap* heap, std::vector<int> values) {
      std::sort(values.begin(), values.end());
      if (get_delete_result(heap, values.size()) != values) {
        throw TestFail("delete k returned incorrect result");
      }
    }

  private:
    std::vector<int> get_delete_result(IHeap* heap, unsigned k) {
      std::vector<Item> result = heap->delete_k(k);
      std::vector<int> raw;
      std::transform(result.begin(), result.end(), std::back_inserter(raw), std::bind(&Item::get_value, std::placeholders::_1));
      std::sort(raw.begin(), raw.end());
      return raw;
    }
};

inline void run_all_tests() {
  for (const auto& test: TestFixture::test_registrar()) {
    test();
  }
}

#define MAKE_TEST(test_name, heap) \
  class Testlib_##test_name : protected TestFixture { \
    public: \
      void run_testlib() override { \
        DRIVE_TEST(test_name, BinaryHeap); \
        DRIVE_TEST(test_name, FibonacciHeap); \
        DRIVE_TEST(test_name, MedianSelect); \
      } \
      void run_single_test(IHeap* heap) override; \
  }; \
  Testlib_##test_name test_instance_##test_name; \
  void Testlib_##test_name::run_single_test(IHeap* heap)

#define DRIVE_TEST(test_name, heap_type) \
  do { \
    Item::reset_statistics(); \
    std::unique_ptr<IHeap> heap(new heap_type()); \
    try { \
      fprintf(stderr, "======= Beginning test: %s/%s\n", #test_name, #heap_type); \
      run_single_test(heap.get()); \
      fprintf(stderr, "======= Ending test: %s/%s\n\n", #test_name, #heap_type); \
    } catch (TestFail) { \
      fprintf(stderr, ">>>>>>> Aborting test\n"); \
      fprintf(stderr, "======= Failing test: %s/%s\n\n", #test_name, #heap_type); \
    } \
  } while (0)

#endif  // _TESTS_H
