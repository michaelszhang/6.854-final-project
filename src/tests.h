#ifndef _TESTS_H
#define _TESTS_H

#include <algorithm>
#include <functional>
#include <memory>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <time.h>

#include "binary_heap.h"
#include "fibonacci_heap.h"
#include "heap.h"
#include "heap_adapter.h"
#include "item.h"
#include "median_select.h"

struct TestFail {
  TestFail(const char* format, ...) {
    has_failed() = true;
    fprintf(stderr, "Test failed: ");
    va_list arg;
    va_start(arg, format);
    vfprintf(stderr, format, arg);
    va_end(arg);
    fprintf(stderr, "\n");
  }

  static bool& has_failed() {
    static bool failed;
    return failed;
  }
};

struct TestSkip {};

class TestFixture {
  public:
    static std::vector<std::function<void()>>& test_registrar() {
      static std::vector<std::function<void()>> registrar;
      return registrar;
    }

    static int& passed_tests() {
      static int count;
      return count;
    }

    static int& skipped_tests() {
      static int count;
      return count;
    }

    static int& failed_tests() {
      static int count;
      return count;
    }

  protected:
    TestFixture() {
      test_registrar().push_back(std::bind(&TestFixture::run_testlib, this));
    }

    virtual void run_testlib() = 0;
    virtual void run_single_test(IHeap* heap) = 0;

    void expect_delete(IHeap* heap, int value) {
      try {
        assert_delete(heap, value);
      } catch (TestFail) {}
    }

    void expect_select(IHeap* heap, const std::vector<int>& values) {
      try {
        assert_select(heap, values);
      } catch (TestFail) {}
    }

    void assert_delete(IHeap* heap, int value) {
      int result = heap->delete_min().get_value();
      if (result != value) {
        throw TestFail("expected to delete %d, not %d", value, result);
      }
    }

    void assert_select(IHeap* heap, std::vector<int> values) {
      std::vector<Item> result = heap->select_k(values.size());
      std::vector<int> raw;
      std::transform(result.begin(), result.end(), std::back_inserter(raw), std::bind(&Item::get_value, std::placeholders::_1));

      std::sort(raw.begin(), raw.end());
      std::sort(values.begin(), values.end());
      if (raw.size() != values.size()) {
        throw TestFail("select-k returned %d items, not %d", (int)raw.size(), (int)values.size());
      }
      for (size_t i = 0; i < raw.size(); ++i) {
        if (raw[i] != values[i]) {
          throw TestFail("select-k returned incorrect result, got %d, not %d", raw[i], values[i]);
        }
      }
    }
};

inline void run_all_tests() {
  for (const auto& test: TestFixture::test_registrar()) {
    test();
  }
  fprintf(stderr, "Found %d tests: %d passed, %d skipped, %d failed.\n",
      TestFixture::passed_tests() + TestFixture::skipped_tests() + TestFixture::failed_tests(),
      TestFixture::passed_tests(),
      TestFixture::skipped_tests(),
      TestFixture::failed_tests());
}

#define MAKE_TEST(test_name, heap) \
  class Testlib_##test_name : protected TestFixture { \
    public: \
      void run_testlib() override { \
        DRIVE_TEST(test_name, BinaryHeap); \
        DRIVE_TEST(test_name, FibonacciHeap); \
        DRIVE_TEST(test_name, MedianSelect); \
        DRIVE_TEST(test_name, HeapAdapter<BinaryHeap>); \
        DRIVE_TEST(test_name, HeapAdapter<FibonacciHeap>); \
        DRIVE_TEST(test_name, HeapAdapter<MedianSelect>); \
      } \
      void run_single_test(IHeap* heap) override; \
  }; \
  Testlib_##test_name test_instance_##test_name; \
  void Testlib_##test_name::run_single_test(IHeap* heap)

#define SKIP_HEAP(heap, heap_type) \
  do { \
    if (dynamic_cast<heap_type*>(heap) != nullptr) { \
      throw TestSkip(); \
    } \
  } while (0)

#define RUN_ONCE_ONLY(heap) \
  do { \
    static int test_has_already_run = 0; \
    if (test_has_already_run++) { \
      throw TestSkip(); \
    } \
  } while (0)

#define DRIVE_TEST(test_name, heap_type) \
  do { \
    Item::reset_statistics(); \
    TestFail::has_failed() = false; \
    std::unique_ptr<IHeap> heap(new heap_type()); \
    try { \
      fprintf(stderr, "======= Beginning test: %s/%s\n", #test_name, #heap_type); \
      try { \
        run_single_test(heap.get()); \
      } catch (std::exception e) { \
        throw TestFail("terminating with uncaught exception: %s", e.what()); \
      } \
    } catch (TestFail) { \
      fprintf(stderr, ">>>>>>> Aborting test\n"); \
    } catch (TestSkip) { \
      fprintf(stderr, "======= Skipping test: %s/%s\n\n", #test_name, #heap_type); \
      skipped_tests() += 1; \
      break; \
    } \
    if (TestFail::has_failed()) { \
      fprintf(stderr, "======= Failing test: %s/%s\n\n", #test_name, #heap_type); \
      failed_tests() += 1; \
    } else { \
      fprintf(stderr, "======= Ending test: %s/%s\n\n", #test_name, #heap_type); \
      passed_tests() += 1; \
    } \
  } while (0)

#endif  // _TESTS_H
