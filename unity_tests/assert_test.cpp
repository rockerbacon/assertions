#include "assert.h"
#include <iostream>

using namespace assertion;
using namespace std;

int main (void) {

	cout << "Equals: " << Equals<int>().compare(10, 10) << endl;
	cout << "Not Equals: " << Not<int>(Equals<int>()).compare(10, 5) << endl;

	try {
		assert(5, LessThan<int>(), 10);
		assert(10, Equals<int>(), 5);
	} catch (assert_error &e) {
		cout << e.what() << endl;
	}

	cout << "END" << endl;

	return 0;
}
