# Assertions
A framework for writing automated tests in C++14.
All classes and functions are contained inside the namespace _assertion_

This documentation is still a work in progress

## Configuring build
Inside the Makefile there are four variables which will need configuration:
```
SRC_DIR=relative path to the directory containing your source files
HEADERS_DIR=relative path to the directory containing your header files
TESTS_DIR=relative path to the directory containing your tests

HEADER_DEPS=space separated list of headers which are considered dependancies
```
By default objects are build inside _./objs_, tests are build in _./test\_build_ and program binaries are build inside _./build_.

These can be changed by editing the variables _OBJS\_BUILD\_DIR_, _TEST\_BUILD\_DIR_ and _BUILD\_DIR_ in the makefile, respectively.

## Running tests
Then execute the script _test.sh_ with the tests to be run as parameters. The script will automatilly build the tests if needed, but when executing all tests it's adivised to manually build them first with "make tests" since without it the regular expression \* will not capture the unexistant test builds.

The script must be run from the directory where the test script resides. Usage examples:
```
$ ./test.sh test_build/*
```
Runs all tests compiled in the _test\_build_ folder
```
$ ./test.sh test_build/assert_test
```
Runs the test _assert\_test_ compiled in the _test\_build_ folder
```
$ ./test.sh test_build/assert_test1 test_build/assert_test2
```
Runs both tests _assert\_test1_ and _assert\_test2_ compiled in the _test\_build_ folder


## Functions
[template<TypeActual, TypeExpected>void assert (const TypeActual& actual_value, const Comparator& comparator, const TypeExpected& expected_value) thow(assert_error)][function_assert];

```
void assert_fail(const std::string& message) throw assert_error
```
Forces an assert\_error to be thrown with the provided message
## Classes
[comparator][class_comparator]

[assert_error][class_assert_error]

[function_assert]: /documentation/function_assert.md
[class_comparator]: /documentation/class_comparator.md
[class_assert_error]: /documentation/class_assert_error.md
