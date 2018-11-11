#include "assert.h"
#include <iostream>

using namespace assertion;
using namespace std;

int main (void) {

	//testing assertion
	assert(5, less_than<int>(), 10);
	try {
		assert(10, equals<int>(), 5);
		cout << "Exception not thrown in ==" << endl;
		return 1;
	} catch (assert_error &e) {
		cout << e.what() << endl;
	}

	//testing chaining
	assert(5, greater_than<int>()._not(), 10);
	assert(5, less_than<int>()._or(equals<int>()), 5);
	try {
		assert(4, greater_than<int>()._or(equals<int>()), 5);
		cout << "Exception not thrown in >=" << endl;
		return 2;
	} catch (assert_error &e) {
		cout << e.what() << endl;
	}

	cout << "END" << endl;

	return 0;
}
