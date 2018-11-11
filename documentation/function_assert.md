# Assert Function
## Definition
Defined and implemented in </src/assert.h>, line 60:
````
template<typename TypeActual, typename TypeExpected>
void assert (const TypeActual& actual_value, const Comparator& comparator, const TypeExpected& reference_value) throw(assert_error)
````
## Parameters
* _actual\_value_: the value to be tested;
* _comparator_: class used for comparing _actual\_value_ against _reference\_value_;
* _reference\_value_: reference value for testing _actual\_value_.

## Exceptions
* _assert\_error_: thrown when assertion fails, that is, when the call to `comparator(actual_value, reference_value)` returns false. Throwing this exception ensures that the testing script can capture the test's failure

## Usage example
````
int a = 2;
float b = 3.4;

assertions::assert(a, assertions::less_than, b); //asserts successfully, since a is less than b, and code continues its execution
assertions::assert(a, assertions::equals, b); //assert fails and exception is thrown, indicating to the test runner that the test has failed
````
