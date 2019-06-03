# Comparator
## Definition
Declared and defined in <src/assert.h>, line 23:
```
template<typename T>
class comparator
```
## Constructors
```
comparator (const std::string& description, const std::function<bool(const T&, const T&)>& comparison_function)
```
### Parameters
* _description_: message describing the evaluation operation. Used during throws of class _assert\_error_;
* _comparison\_function_: function used for comparing two values. Returns a boolean indicating whether the two values satisfy a condition;
## Methods
```
bool operator() (const T& actual\_value, const T& reference\_value)
```
Evaluates the current comparison function for the two values passed as parameters
```
comparator& \_not (void)
```
Negates this comparator and returns a reference to _this_. In case the changes should be made on a copy instead of in-place _operator !()_ should be used instead
```
comparator& \_and (const comparator<T>& c)
```
Chains this comparator with comparator c and returns a reference to _this_. In case the changes should be made on a copy instead of in-place _operator &&(const comparator<T>& c)_ should be used instead.
```
