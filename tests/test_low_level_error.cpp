#include "assertions/assert.h"

int main (void) {

	test_suite("segmentation fault") {
		test_case("raises low level error") {
			int* x = NULL;
			*x = 2;
			assert(*x, ==, 2);
		};
	}

	return 0;
}
