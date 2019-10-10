#include "assertions/assert.h"
#include <thread>

using namespace std;
int main (void) {
	test_suite("benchmark output") {
		test_case("wait 2652ms") {
			this_thread::sleep_for(chrono::milliseconds(2652));
		};
	}
}
