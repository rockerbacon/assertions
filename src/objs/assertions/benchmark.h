#pragma once

#include <list>
#include "assertions/observer.h"

#define benchmark(title, runs) \
	for (auto o : benchmark::observers) {\
		o->notifyBenchmarkBegun(title, runs);\
	}\
   	for (benchmark::current_run = 0; benchmark::current_run < runs; benchmark::current_run++) {\
		for (auto o: benchmark::observers) {\
			o->notifyRunBegun();\
		}

#define end_benchmark \
		for (auto o: benchmark::observers) {\
			o->notifyRunEnded();\
		}\
	}\
	for (auto o: benchmark::observers) {\
		o->notifyBenchmarkEnded();\
	}

namespace benchmark {
	extern std::list<Observer*> observers;
	extern unsigned current_run;

	void register_observer(Observer *Observer);
	void delete_observers(void);

	template<typename T>
	void observe_variable(const std::string &variable_label, const T& variable) {
		for (auto o : observers) {
			o->observeVariable(variable_label, variable);
		}
	}
}
