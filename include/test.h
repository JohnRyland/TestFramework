/*
   Test Framework
   Created by John Ryland on 17/3/19.
   Copyright (C) 2019-2021 John Ryland. All rights reserved.
*/

#pragma once


#include "Factory.hpp"

// Some other test frameworks are a bit easier for the linker to strip the tests
// when no test_main is provided. Disabling unit tests with this define may help.
//#define ENABLE_UNIT_TESTS   1

//! Unit testing framework
namespace UnitTest
{
  typedef void(*UnitTestFunc)(int iters);
  struct UnitTestItem
  {
    const char*  m_name;
    UnitTestFunc m_func;
    bool         m_ran;
    bool         m_pass;
    long long    m_duration;
  };
  using UnitTestFactoryItem = FactoryItem<UnitTestItem>;
  void UnitTestAssert(bool a_expression, const char* a_expressionStr, const char* a_file, int a_line, bool a_expectNotAssert);
  extern int failures;

  // Can use C++11 range-based looping on this
  static inline UnitTestFactoryItem& testList() { return UnitTest::UnitTestFactoryItem::items(); }
}

#ifdef ENABLE_UNIT_TESTS
// Note: This macro doesn't evaluate the expression wrapped with a try/catch block. On failure will exit with -1.
#  define BENCHMARK(testSet, testName, iters)  DECLARE_UNIT_TEST_HELPER(#testSet "." #testName, NAME_AND_LINE(testSet##_##testName, __LINE__), iters)
#else
#  define BENCHMARK(testSet, testName, iters)  static void unitTestFunction_##testSet##_##testName(int iters)
#endif // ENABLE_UNIT_TESTS

#define TEST(set, test)                        BENCHMARK(set, test, iters)       
#define CHECK(expression)                      ::UnitTest::UnitTestAssert(expression, #expression, __FILE__, __LINE__, false)
#define EXPECT(expression)                     ::UnitTest::UnitTestAssert(expression, #expression, __FILE__, __LINE__, true)
#define ASSERT_EQ(A, B)                        CHECK(A == B) 
#define ASSERT_GT(A, B)                        CHECK(A > B)
#define ASSERT_LT(A, B)                        CHECK(A < B)
#define EXPECT_EQ(A, B)                        CHECK(A == B)
#define EXPECT_GT(A, B)                        CHECK(A > B)
#define EXPECT_LT(A, B)                        CHECK(A < B)

#define NAME_AND_LINE(x, y)                    NAME_AND_LINE_INTERNAL(x, y)
#define NAME_AND_LINE_INTERNAL(x, y)           x##y
#define DECLARE_UNIT_TEST_HELPER(name,fname,i) DECLARE_UNIT_TEST_HELPER_INTERNAL(name,fname,i)

#define DECLARE_UNIT_TEST_HELPER_INTERNAL(testName, funcName, iters) \
  namespace UnitTest \
  { \
    void unitTestFunction_##funcName(int); \
    ::UnitTest::UnitTestFactoryItem g_register_##funcName({ testName, &unitTestFunction_##funcName, false, false, 0 }); \
  } \
  void UnitTest::unitTestFunction_##funcName(int iters)


