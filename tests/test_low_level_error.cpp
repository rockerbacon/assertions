#include "assert.h"

int main (void) {
	test_case("raises low level error") {
		int* x = NULL;
		*x = 2;
		assert_equal(*x, 2);
	} end_test_case;

	return 0;
}
