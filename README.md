# Assertions
Assertions is a framework for writing automated tests in C++.

## Configuring the environment
Assertions requires the project's directories and files to follow a specific format in order to work as intended. The _makefile_ and _test.sh_ script must be in the root directory for the project, in this root directory 5 aditional directories are also necessary. All these directories are defined inside the _makefile_:
* SRC\_DIR: the directory where all the source (.cpp) files are stored. By default this is the _src_ folder;
* HEADERS\_DIR: the directory where all the headers (.h or .hpp) for your project are stored. Defaults to the _SRC\_DIR_ folder;
* TESTS\_DIR: the directory where all your tests must be written. Defaults to the _tests_ folder;
* TEST\_BUILD\_DIR: the directory where all your tests builds will be compiled to. Defaults to _test\_build_ and is created automatically during the build;
* OBJS\_BUILD\_DIR: the directory where all your object builds (.o files) will be compiled to. Defaults to _objs_ and is created automatically during the build;

## Running tests
To run already written tests the _test.sh_ script must be run from within the project's root directory. The usage follows this format:
```
./test.sh all | [space separated list of test names]
```
With the paramater _all_, all tests inside the _TESTS\_DIR_ folder will be build and run.

A test name can be one of the following:
* The name of a source file inside _TESTS\_DIR_. Eg: test_something.cpp;
* The name of a source file inside _TESTS\_DIR_ without the _test\__ prefix and _.cpp_ extension. Eg: something;
* The name of a test binary inside _TEST\_BUILD\_DIR_. Eg: test_something;
Usage examples:
```
./test.sh all
```
Run all tests
```
./test.sh something
./test.sh test_something
./test.sh test_something.cpp
```
All three will run the test written in test\_something.cpp

Alternatively the test binaries can also be run directly but the output will not have as much information as when run from the script.

## Writing tests
First step for writing a test is creating a _.cpp_ file inside _TESTS\_DIR_. The file must be prefixed with _test\__ but cannot be prefixed with _test\_test\__.

The tests should be written inside a _main_ function. The main function does not need to return anything and does not take any arguments.

### Test case block
A test case block is a block specifing something to be tested. A test block must contain a message describing what is going to be tested. Eg:
```
test_case("whether this functionality works") {
	// test code goes here
} end_test_case;
```

### Asserting values
Assertions ensure that a test condition is met and if not will cause the test case to fail. The first value is the value to be tested and the second value is the value expected. These are the provided assert macros:
* _assert\_equal_: checks if two values are equal:
```
test_case("a equals 2") {
	int a = 2;
	assert_equal(a, 2);
} end_test_case;
```
* _assert\_not\_equal_: checks if two values are different:
```
test_case("a not equal 2") {
        int a = 3;
        assert_not_equal(a, 2);
} end_test_case;
```
* _assert\_greater\_than_: checks if a value is greater than another value:
```
test_case("a greater than 2") {
        int a = 3;
        assert_greater_than(a, 2);
} end_test_case;
```
* _assert\_less\_than_: checks if a value is less than another value:
```
test_case("a less than 2") {
        int a = 1;
        assert_less_than(a, 2);
} end_test_case;
```
* _assert\_greater\_than\_or\_equal_: checks if a value is greater than or equal another value:
```
test_case("a greater than or equal 1") {
        int a = 2;
        assert_greater_than_or_equal(a, 2);
	assert_greater_than_or_equal(a, 1);
} end_test_case;
```
* _assert\_less\_than\_or\_equal_: checks if a value is less than or equal another value:
```
test_case("a less than or equal 1") {
        int a = 1;
        assert_less_than_or_equal(a, 2);
	assert_less_than_or_equal(a, 1);
} end_test_case;
```
* _assert\_true_: checks if a logic expression is true. Useful for doing more complex assertions:
```
test_case("a in range [0, 3)") {
        int a = 2;
        assert_true(a >= 0 && a < 3);
} end_test_case;
```
## Weird build errors
Assertions makes heavy use of macros in order to generate code during compilation time. This means that if a test is badly written you will get compilation error messages referencing code you did not write explicitly. Eg:
```
int main (void) {
	test_case("missing brackets")
		assert_true(true);
	end_test_case;
}
```
The compiler will complain near a "try" keyword. Although you did not write this keyword anywhere, it is created during the expansion for the _test\_case_ macro



