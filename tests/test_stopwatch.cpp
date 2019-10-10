#include "assertions/assert.h"
#include "assertions/stopwatch.h"

using namespace assert;
using namespace benchmark;

int main (void) {

	test_suite("stopwatch") {
		test_case("stopwatch constructor raises no errors") {
			Stopwatch stopwatch;
		};

		test_case("reseting stopwatch raises no errors") { 
			Stopwatch stopwatch;
			stopwatch.reset();
		};

		test_case("capturing formated total time returns non empty string") {
			Stopwatch stopwatch;
			std::string formatedTime = stopwatch.formatedTotalTime();
			assert(formatedTime.length(), >, 0);
		};

		test_case("capturing formated lap time returns non empty string") {
			Stopwatch stopwatch;
			std::string formatedTime = stopwatch.formatedLapTime();
			assert(formatedTime.length(), >, 0);
		};
	}

}
