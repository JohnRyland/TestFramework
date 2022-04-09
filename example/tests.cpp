#include <limits.h>
#include "test.h"

//
// Demonstration of using the test framework
//

// Returns n! (the factorial of n).  For negative n, n! is defined to be 1.
int Factorial(int n)
{
  int result = 1;
  for (int i = 1; i <= n; i++) {
    result *= i;
  }
  return result;
}

#ifdef ENABLE_UNIT_TESTS

namespace {

// Tests factorial of negative numbers.
TEST(FactorialTest, Negative)
{
  EXPECT_EQ(1, Factorial(-5));
  EXPECT_EQ(1, Factorial(-1));
  EXPECT_GT(Factorial(-10), 0);
}

// Tests factorial of 0.
TEST(FactorialTest, Zero)
{
  EXPECT_EQ(1, Factorial(0));
}

// Tests factorial of positive numbers.
TEST(FactorialTest, Positive)
{
  EXPECT_EQ(1, Factorial(1));
  EXPECT_EQ(2, Factorial(2));
  EXPECT_EQ(6, Factorial(3));
  EXPECT_EQ(40320, Factorial(8));
  ASSERT_EQ(40320, Factorial(8));
}

BENCHMARK(FactorialTest, Calc, iterations)
{
  for (int i = 0; i < iterations; ++i)
  {
    Factorial(1000);
  }
}

}  // namespace

#endif

