/*
   Test Framework Implementation
   Copyright (C) 2019-2022 John Ryland. All rights reserved.
*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <chrono>

#include "test.h"

int UnitTest::failures = 0;

void UnitTest::UnitTestAssert(bool a_expression, const char* a_expressionStr, const char* a_file, int a_line, bool a_expectNotAssert)
{
  if (!a_expression)
  {
    fprintf(stderr, "\n%s:%i: Failure\n  Expected :\n    %s\n\n", a_file, a_line, a_expressionStr);
    assert(!a_expectNotAssert);
    ++failures;
  }
}

#ifdef ENABLE_UNIT_TESTS

int main(int argc, const char* argv[])
{
  FILE* output = 0;
  const char* filter = 0;

  // Process command-line arguments
  if (argc > 1)
  {
    for (int i = 1; i < argc; ++i)
    {
      if (strcmp(argv[i], "--list-tests") == 0)
      {
        for (auto item : UnitTest::testList())
        {
          printf("%s\n", item.m_name);
        }
        return 0;
      }
      else if (strncmp(argv[i], "--filter=", 9) == 0)
      {
        filter = argv[i] + 9;
      }
      else if (strncmp(argv[i], "--output=", 9) == 0)
      {
        output = fopen(argv[i] + 9, "w");
        if (!output)
        {
          printf("Error while opening files %s for writing\n", argv[i] + 9);
        }
      }
      else if (strcmp(argv[i], "--help") == 0)
      {
        printf("Usage:\n");
        printf("    %s [OPTIONS]\n", argv[0]);
        printf("OPTIONS:\n");
        printf("    --list-tests    : Lists the tests in this test program\n");
        printf("    --filter=FILTER : Runs tests where test name matches FILTER\n");
        printf("    --output=OUTPUT : XML filename to write results to\n");
        printf("    --help          : This help output\n");
        return 0;
      }
    }
  }

  // Count the number of tests to run
  int testsToRun = 0;
  for (auto item : UnitTest::testList())
  {
    if (!filter || strncmp(item.m_name, filter, std::min(strlen(filter), strlen(item.m_name))) == 0)
    {
      ++testsToRun;
    }
  }

  // Run the tests
  printf("[==========] Running %i test cases.\n", testsToRun);
  UnitTest::failures = 0;
  int testsRan = 0;
  for (auto& item : UnitTest::testList())
  {
    item.m_ran = false;
    if (!filter || strncmp(item.m_name, filter, std::min(strlen(filter), strlen(item.m_name))) == 0)
    {
      int lastFailures = UnitTest::failures;
      printf("[ RUN      ] %s\n", item.m_name);
      auto testStartTime = std::chrono::system_clock().now();

      try
      {
        item.m_func(1);
      }
      catch(...)
      {
        printf("Test failed with an exception: %s\n", item.m_name);
        exit(-1);
      }

      auto testEndTime = std::chrono::system_clock().now();
      item.m_ran = true;
      item.m_pass = lastFailures == UnitTest::failures;
      item.m_duration = std::chrono::nanoseconds(testEndTime - testStartTime).count();
      printf("[  %s ] %s (%llins)\n", (lastFailures == UnitTest::failures) ? "     OK" : "FAILED ", item.m_name, item.m_duration);
      ++testsRan;
    }
  }
  printf("[==========] %i test cases ran.\n", testsRan);
  printf("[  %s  ] %i of %i tests failed.\n", UnitTest::failures ? "FAILED" : "PASSED", UnitTest::failures, testsRan);

  // Write report to XML file
  if (output)
  {
    fprintf(output, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<testsuites tests=\"%d\" failures=\"%d\" name=\"All\">\n", testsRan, UnitTest::failures);
    fprintf(output, "  <testsuite name=\"Tests\" tests=\"%d\" failures=\"%d\">\n", testsRan, UnitTest::failures);
    for (auto& item : UnitTest::testList())
    {
      if (item.m_ran)
      {
        fprintf(output, "    <testcase name=\"%s\" status=\"run\" result=\"%s\" time=\"%lld\"/>\n",
            item.m_name, item.m_pass ? "completed" : "failed", item.m_duration);
      }
    }
    fprintf(output, "  </testsuite>\n");
    fprintf(output, "</testsuites>\n");
    fclose(output);
  }
  return UnitTest::failures;
}

#endif

