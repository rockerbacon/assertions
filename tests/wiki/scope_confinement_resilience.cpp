#include <assertions-test/test.h>

using namespace std;
using namespace chrono;

begin_tests {
	test_suite("test suites are resilient") {
		test_case("tests are terminated in an independant manner") {
			int* invalid_pointer = nullptr;
			*invalid_pointer = 0xDEAD;
		};

		test_case("tests can still pass after an unrelated emergency termination") {
			this_thread::sleep_for(2s);
			assert(true, ==, true);
		};
	}
} end_tests;

