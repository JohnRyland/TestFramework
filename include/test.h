/*
   Test Framework
   Created by John Ryland on 17/3/19.
   Copyright (C) 2019-2022 John Ryland. All rights reserved.
*/

#pragma once


#include "Factory.hpp"
#include <string>


// Some other test frameworks are a bit easier for the linker to strip the tests
// when no test_main is provided. Disabling unit tests with this define may help.
//#define ENABLE_UNIT_TESTS   1

// Produce more meaningful output on test failures (however this uses the standard C++ library)
#define ENABLE_VALUES_OUTPUT


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


#ifdef ENABLE_VALUES_OUTPUT
// Note: This uses std::string but produces more meaningful output when there is a test failure.
// However if you want to avoid using the standard C++ library then undefine ENABLE_VALUES_OUTPUT
#  define EXPRESSION_AS_STRING(A, OP, B, exp)  (#A "   IS " OP "   " #B \
                                               "\n  Expression :\n    " #exp \
                                               "\n  Values :\n    " + std::to_string(A) + "   IS NOT " OP "   " + std::to_string(B)).c_str()
#else
#  define EXPRESSION_AS_STRING(A, OP, B, exp)  #exp
#endif // ENABLE_VALUES_OUTPUT


#define TEST(set, test)                        BENCHMARK(set, test, iters)       
#define CHECK(expression)                      ::UnitTest::UnitTestAssert(expression, #expression, __FILE__, __LINE__, false)
#define EXPECT(expression)                     ::UnitTest::UnitTestAssert(expression, #expression, __FILE__, __LINE__, true)
#define CHECK_OP_EXP(A, B, OP, expression)     ::UnitTest::UnitTestAssert(expression, EXPRESSION_AS_STRING(A, OP, B, expression), __FILE__, __LINE__, false)
#define EXPECT_OP_EXP(A, B, OP, expression)    ::UnitTest::UnitTestAssert(expression, EXPRESSION_AS_STRING(A, OP, B, expression), __FILE__, __LINE__, true)
#define ASSERT_EQ(A, B)                        CHECK_OP_EXP(A, B, "EQUALS",        (A) == (B)) 
#define ASSERT_GT(A, B)                        CHECK_OP_EXP(A, B, "GREATER THAN",  (A) > (B))
#define ASSERT_LT(A, B)                        CHECK_OP_EXP(A, B, "LESS THAN",     (A) < (B))
#define ASSERT_NEAR(A, B, FABS_ERROR)          CHECK_OP_EXP(A, B, "NEAR",          fabs((A) - (B)) < FABS_ERROR)
#define EXPECT_EQ(A, B)                        EXPECT_OP_EXP(A, B, "EQUALS",       (A) == (B))
#define EXPECT_GT(A, B)                        EXPECT_OP_EXP(A, B, "GREATER THAN", (A) > (B))
#define EXPECT_LT(A, B)                        EXPECT_OP_EXP(A, B, "LESS THAN",    (A) < (B))
#define EXPECT_NEAR(A, B, FABS_ERROR)          EXPECT_OP_EXP(A, B, "NEAR",         fabs((A) - (B)) < FABS_ERROR)


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


