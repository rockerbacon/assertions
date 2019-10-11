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

	test_suite("multiple test suites") {
		test_case("trying something") {
			ASSERT_NO_UNUSED_WARNING float pi = 3.14;
		};
	}

	test_suite("empty test suite") {

	}

	return 0;

}
