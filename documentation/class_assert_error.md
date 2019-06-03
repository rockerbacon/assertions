# Assert error class
## Definition
Declared in <src/assert.h>, line 12:
```
class assert_error : public std::exception
```
Defined in <src/assert.cpp>, lines 6 to 16
## Constructors
```
assert_error::assert_error (const string& actual_value, const string& comparator_description, const string& reference_value)
```
### Parameters
* _actual\_value_: the value that was tested;
* _comparator\_description_: description defining the evaluated comparison;
* _reference\_value_: the value used as reference for the comparison;
```
assert_error::assert_error (const string& comparator_description)
```
### Parameters
* _comparator\_description_: description defining the evaluated comparison;
