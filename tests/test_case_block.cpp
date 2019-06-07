#include "assert.h"

int main (void) {

	test_case("test case block") {
		int a = 2;
	} end_test_case;

	test_case("multiple test case blocks") {
		int b = 3;
	} end_test_case;

	return 0;

}
