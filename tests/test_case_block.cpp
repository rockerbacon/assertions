#include "assertions/assert.h"
#include <iostream>

using namespace std;
int main (void) {

	test_suite("test suite 1") {

		test_case("test case block") {
			ASSERT_NO_UNUSED_WARNING int a = 2;
		};

		test_case("multiple test case blocks") {
			ASSERT_NO_UNUSED_WARNING int b = 3;
		};

	}

	return 0;

}
