
# Test Framework
### Simple unit test framework

Copyright (c) 2021-2022, John Ryland
All rights reserved.


## Introduction

Testing is important and so it should be simple to integrate testing of code.

This simple framework is designed for running unit tests.

There is an example set of unit tests as well. To simply compile and run the
unit tests included here, run the following in the current directory:

```
make test
```

It creates an executable wrapper around the code to be tested along with its
test cases, and a way to enumerate the tests and to run them. It can then
report the results in various ways as well as provide timing information.

When the code is linked with the test framework (TestMain.cpp), the executable
has the following usage:

```
Usage:
    ./build/test_app [OPTIONS]
OPTIONS:
    --list-tests    : Lists the tests in this test program
    --filter=FILTER : Runs tests where test name matches FILTER
    --output=OUTPUT : XML filename to write results to
    --help          : This help output
```

The testing code is designed to be enabled if the preprocessor define
ENABLE_UNIT_TESTS is defined. Compiling the TestMain.cpp with this defined will
add a main function. If it is not defined, you can add preprocessor checks for
this define if you want to strip the testing code from the executables and
to conditionally define your own main function.

Build system integration with the GenericMake project makes this easy. If you
choose the 'test' target, it will define this preprocessor define, will compile
the program and will execute it with '--list-tests' and run them and generate
XML output for each test suitable for parsing by a CI system and will generate
a test-report.txt summary of the testing.

To integrate with other build systems is possible but is left as an exercise
for the reader.

