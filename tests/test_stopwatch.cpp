#include "assertions/assert.h"
#include "assertions/stopwatch.h"

using namespace assert;
using namespace benchmark;

int main (void) {

	test_case("stopwatch constructor raises no errors") {
		Stopwatch stopwatch;
	} end_test_case;

	test_case("reseting stopwatch raises no errors") { 
		Stopwatch stopwatch;
		stopwatch.reset();
	} end_test_case;

	test_case("capturing formated total time returns non empty string") {
		Stopwatch stopwatch;
		std::string formatedTime = stopwatch.formatedTotalTime();
		assert_greater_than(formatedTime.length(), 0);
	} end_test_case;

	test_case("capturing formated lap time returns non empty string") {
		Stopwatch stopwatch;
		std::string formatedTime = stopwatch.formatedLapTime();
		assert_greater_than(formatedTime.length(), 0);
	} end_test_case;

}
