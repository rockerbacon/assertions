#include "assertions/assert.h"
#include <thread>

using namespace std;
int main (void) {
	begin_all_tests;

	test_suite("3 test cases outputs") {
		test_case("wait 2652ms") {
			this_thread::sleep_for(2652ms);
		};

		test_case("wait 500ms") {
			this_thread::sleep_for(500ms);
		};

		test_case("wait 3512ms") {
			this_thread::sleep_for(3512ms);
		};
	}

	test_suite("4 test cases outputs") {
		test_case("wait 1s") {
			this_thread::sleep_for(1s);
		};

		test_case("wait 3s") {
			this_thread::sleep_for(3s);
		};

		test_case("wait 250ms") {
			this_thread::sleep_for(250ms);
		};

		test_case("wait 750ms") {
			this_thread::sleep_for(750ms);
		};
	}

	test_suite("2 test cases output") {
		test_case("wait 250ms") {
			this_thread::sleep_for(250ms);
		};

		test_case("wait 3s") {
			this_thread::sleep_for(3s);
		};
	}

	end_all_tests;
	return 0;
}
