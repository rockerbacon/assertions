#include "assertions/benchmark.h"

using namespace std;
using namespace benchmark;

list<Observer*> benchmark::observers;
unsigned benchmark::current_run;

void benchmark::register_observer(Observer *observer) {
	benchmark::observers.push_back(observer);
}

void benchmark::delete_observers() {
	for (auto o : benchmark::observers) {
		delete o;
	}
}
