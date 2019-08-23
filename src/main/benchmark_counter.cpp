#include "assertions/benchmark.h"
#include <thread>

using namespace benchmark;

int main (void) {

	int runs = 10;
	int x, xsquared;
	float y, ysquared, xy;
	TerminalObserver observer;
	
	observer.observeVariable("x", x);
	observer.observeVariable("y", y);
	observer.observeVariable("x^2", xsquared);
	observer.observeVariable("y^2", ysquared);
	observer.observeVariable("x*y", xy);

	observer.notifyBenchmarkBegun("benchmark", runs);
	for (int i = 0; i < runs; i++) {
		observer.notifyRunBegun();

		std::this_thread::sleep_for(std::chrono::seconds(2));

		x = i*2;
		y = i*3.0/2.0;
		xsquared = x*x;
		ysquared = y*y;
		xy = x*y;

		observer.notifyRunEnded();	
	}
	observer.notifyBenchmarkEnded();

	return 0;

}
