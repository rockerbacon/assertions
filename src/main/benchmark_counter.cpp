#include "assertions/benchmark.h"
#include <thread>

int main (void) {

	int runs = 10;
	int x, xsquared;
	float y, ysquared, xy;

	benchmark::register_observer(new benchmark::TerminalObserver());

	benchmark::observe_variable("x", x);
	benchmark::observe_variable("y", y, benchmark::observation_mode::AVERAGE_VALUE | benchmark::observation_mode::CURRENT_VALUE);
	benchmark::observe_variable("x^2", xsquared);
	benchmark::observe_variable("y^2", ysquared, benchmark::observation_mode::AVERAGE_VALUE);
	benchmark::observe_variable("x*y", xy);

	benchmark("counters", runs) {
		std::this_thread::sleep_for(std::chrono::seconds(2));

		x = benchmark::current_run*2;
		y = benchmark::current_run*3.0/2.0;
		xsquared = x*x;
		ysquared = y*y;
		xy = x*y;
	} end_benchmark;

	benchmark::delete_observers();

	return 0;

}
