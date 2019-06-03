# Assert error class
## Definition
Declared in <src/assert.h>, line 12:
```
class assert\_error : public std::exception
```
Defined in <src/assert.cpp>, lines 6 to 16
## Constructors
```
assert\_error::assert\_error (const string& actual\_value, const string& comparator\_description, const string& reference\_value)
```
### Parameters
* _actual\_value_: the value that was tested;
* _comparator\_description_: description defining the evaluated comparison;
* _reference\_value_: the value used as reference for the comparison;
```
assert_error::assert\_error (const string& comparator\_description)
```
### Parameters
* _comparator\_description_: description defining the evaluated comparison;
