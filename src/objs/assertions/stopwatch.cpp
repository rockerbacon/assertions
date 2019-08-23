#include "assertions/stopwatch.h"

using namespace benchmark;
using namespace std;

Stopwatch::Stopwatch (void) {
	this->reset();
}

void Stopwatch::reset (void) {
	this->stopwatchBegin = chrono::high_resolution_clock::now();
	this->lapBegin = this->stopwatchBegin;
}

string Stopwatch::formatedTotalTime (void) const {
	auto timeSinceBeginning = chrono::high_resolution_clock::now() - this->stopwatchBegin;
	return format_chrono_duration(timeSinceBeginning);
}

string Stopwatch::formatedLapTime (void) const {
	auto timeSinceLapBegun = chrono::high_resolution_clock::now() - this->lapBegin;
	return format_chrono_duration(timeSinceLapBegun);
}

void Stopwatch::newLap (void) {
	this->lapBegin = chrono::high_resolution_clock::now();
}


string benchmark::format_chrono_duration (chrono::high_resolution_clock::duration duration) {
	ostringstream str_builder;
	auto hours = chrono::duration_cast<chrono::hours>(duration);
	duration -= chrono::duration_cast<decltype(duration)>(hours);
	auto minutes = chrono::duration_cast<chrono::minutes>(duration);
	duration -= chrono::duration_cast<decltype(duration)>(minutes);
	auto seconds = chrono::duration_cast<chrono::seconds>(duration);
	duration -= chrono::duration_cast<decltype(duration)>(seconds);
	auto milliseconds = chrono::duration_cast<chrono::milliseconds>(duration);
	duration -= chrono::duration_cast<decltype(duration)>(milliseconds);
	auto microseconds = chrono::duration_cast<chrono::microseconds>(duration);

	auto hours_count = hours.count();
	auto minutes_count = minutes.count();
	auto seconds_count = seconds.count();
	auto milliseconds_count = milliseconds.count();
	auto microseconds_count = microseconds.count();
	if (hours_count > 0) {
		str_builder << hours_count << 'h';
	}
	if (minutes_count > 0) {
		if (str_builder.tellp() > 0) {
			str_builder << ' ';
		}
		str_builder << minutes_count << 'm';
	}
	if (seconds_count > 0) {
		if (str_builder.tellp() > 0) {
			str_builder << ' ';
		}
		str_builder << seconds_count << 's';
	}
	if (milliseconds_count > 0) {
		if (str_builder.tellp() > 0) {
			str_builder << ' ';
		}
		str_builder << milliseconds_count << "ms";
	}
	if (str_builder.tellp() == 0 && microseconds_count > 0) {
		str_builder << ' ';
		str_builder << microseconds_count << "us";
	} else if (str_builder.tellp() == 0) {
		str_builder << "0us";
	}
	return str_builder.str();
}
