#include "assertions/benchmark.h"
#include <thread>

int main (void) {

	int runs = 10;
	int x, xsquared;
	float y, ysquared, xy;

	benchmark::register_observer(new benchmark::TerminalObserver());

	benchmark::observeVariable("x", x);
	benchmark::observeVariable("y", y);
	benchmark::observeVariable("x^2", xsquared);
	benchmark::observeVariable("y^2", ysquared);
	benchmark::observeVariable("x*y", xy);

	benchmark("counters", runs) {
		std::this_thread::sleep_for(std::chrono::seconds(2));

		x = benchmark::current_run*2;
		y = benchmark::current_run*3.0/2.0;
		xsquared = x*x;
		ysquared = y*y;
		xy = x*y;
	} end_multiple_runs;

	benchmark::delete_observers();

	return 0;

}
