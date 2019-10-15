#include <test/assert.h>

#define FIXTURE 10

begin_tests {
	test_suite("assert mathematical constants") {
		test_case("best practice, literal fixture") {
			double pi = 3.1415;
			assert(pi, ==, 3.1415);
		};

		test_case("good practice, constant fixture. failure message won't look as nice, though") {
			double e = 2.7182;
			const double expected_value = 2.7182;
			assert(e, ==, expected_value);
		};

		test_case("failure message for when using a literal fixture") {
			double pi = 3.1415;
			assert(pi, !=, 3.1415);
		};

		test_case("failure message for when using a constant fixture") {
			double e = 2.7182;
			const double expected_value = 2.7182;
			assert(e, !=, expected_value);
		};

		test_case("bad practice, mutable fixture") {
			double laplace_limit = 0.6627;
			double mutable_fixture = 0.6627;
			mutable_fixture -= 0.02;
			assert(laplace_limit, ==, mutable_fixture);
		};

		test_case("won't even build when trying to assert without a logic operator, so I commented it out") {
			//double golden_ration = 1.618;
			//assert(golden_ration, -, 1.618);
		};

	}

	test_suite("test cases are independent") {
		//int mutable_value = 0;

		const int unchangeable_value = 0;

		test_case("won't even build, so I commented stuff out") {
			//assert(mutable_value, ==, 0);
		};

		test_case("const preserves test case independence, but where was unchangeable_value defined?") {
			assert(unchangeable_value, ==, 10);
		};

		test_case("won't terminate the proccess") {
			int* invalid_pointer = nullptr;
			*invalid_pointer = 0xDEAD;
			assert(*invalid_pointer, ==, 0xDEAD);
		};

		test_case("waits for the segmentation fault and still succeeds after that") {
			using namespace std::chrono;
			std::this_thread::sleep_for(3s);
			assert(2, ==, 2);
		};

		test_case("what do these labels mean? where was FIXTURE defined?") {
			int generic_value = 2;
			assert(generic_value, ==, FIXTURE);
		};
	}
} end_tests;

