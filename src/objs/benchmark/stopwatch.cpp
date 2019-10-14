#include <benchmark/stopwatch.h>
#include <vector>

using namespace benchmark;
using namespace std;

Stopwatch::Stopwatch (void) {
	this->reset();
}

void Stopwatch::reset (void) {
	this->stopwatchBegin = chrono::high_resolution_clock::now();
	this->lapBegin = this->stopwatchBegin;
}

chrono::high_resolution_clock::duration Stopwatch::totalTime (void) const {
	return chrono::high_resolution_clock::now() - this->stopwatchBegin;
}

chrono::high_resolution_clock::duration Stopwatch::lapTime (void) const {
	return chrono::high_resolution_clock::now() - this->lapBegin;
}

void Stopwatch::newLap (void) {
	this->lapBegin = chrono::high_resolution_clock::now();
}

struct TimeUnitCount {
	string unit;
	long long count;
};

ostream& operator<< (ostream &stream, chrono::high_resolution_clock::duration duration) {
	vector<TimeUnitCount> time_unit_counts;
	bool stream_altered;
	size_t i;

	auto hours = chrono::duration_cast<chrono::hours>(duration);
	duration -= chrono::duration_cast<decltype(duration)>(hours);
	auto minutes = chrono::duration_cast<chrono::minutes>(duration);
	duration -= chrono::duration_cast<decltype(duration)>(minutes);
	auto seconds = chrono::duration_cast<chrono::seconds>(duration);
	duration -= chrono::duration_cast<decltype(duration)>(seconds);
	auto milliseconds = chrono::duration_cast<chrono::milliseconds>(duration);
	duration -= chrono::duration_cast<decltype(duration)>(milliseconds);
	auto microseconds = chrono::duration_cast<chrono::microseconds>(duration);

	time_unit_counts.reserve(5);

	time_unit_counts.push_back({"h", hours.count()});
	time_unit_counts.push_back({"m", minutes.count()});
	time_unit_counts.push_back({"s", seconds.count()});
	time_unit_counts.push_back({"ms", milliseconds.count()});
	time_unit_counts.push_back({"us", microseconds.count()});

	stream_altered = false;
	for (i = 0; i < time_unit_counts.size()-1; i++) {
		if (time_unit_counts[i].count > 0) {
			if (stream_altered) {
				stream << ' ';
			}
			stream << time_unit_counts[i].count << time_unit_counts[i].unit;
			stream_altered = true;
		}
	}

	if (!stream_altered) {
		stream << time_unit_counts[i].count << time_unit_counts[i].unit;
	}

	return stream;
}
