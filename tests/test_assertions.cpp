#include "assertions/assert.h"

int main (void) {

	test_suite("assert macros") {
		test_case("assert true succesfully") {
			assert_true(true);
		};

		test_case("assert equal succesfully") {
			assert(3, ==, 3);
		};

		test_case("assert greater than succesfully") {
			assert(4, >, 2);
		};

		test_case("assert less than succesfully") {
			assert(2, <, 4);
		};

		test_case("assert grater than or equal when value is greater") {
			assert(4, >=, 2);
		};

		test_case("assert grater than or equal when values are equal") {
			assert(4, >=, 4);
		};

		test_case("fail assert true") {
			assert_true(false);
		};

		test_case("fail assert equal") {
			assert(1, ==, 3);
		};

		test_case("multiple assertions in one test case - all successful") {
			assert(2, ==, 2);
			assert_true(true);
			assert(2, <, 4);
		};

		test_case("multiple assertions in one test case - one fails") {
			assert(2, ==, 2);
			assert_true(false);
			assert(2, <, 4);
		};
	}

}
