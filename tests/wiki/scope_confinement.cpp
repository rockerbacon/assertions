#include <assertions-test/test.h>

tests {
	test_suite("when constant variables are in the external scope") {
		const int constant_variable = 0;

		test_case("a test case can access them") {
			assert(constant_variable, ==, 0);
		};
	}

	test_suite("when mutable variables are in the external scope") {
		int mutable_variable = 1;

		test_case("a test case which attempts to modify them breaks the build") {
			//mutable_variable = 2;	// won't even build, so I commented it out
			assert(mutable_variable, ==, 1);
		};
	}
}

