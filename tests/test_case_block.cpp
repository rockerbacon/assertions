#include "assert.h"

int main (void) {

	TEST_CASE("test case block") {
		int a = 2;
	} END_TEST_CASE;

	TEST_CASE("multiple test case blocks") {
		int b = 3;
	} END_TEST_CASE;

	return 0;

}
