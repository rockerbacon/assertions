#include "terminal/stream.h"
#include <thread>

using namespace terminal;
using namespace std;

modifiable_stream::modifiable_stream(ostream &stream)
	:	stream(stream)
{}

modifiable_stream& modifiable_stream::operator<< (ostream& (*manipulator)(ostream&)) {
	manipulator(this->stream);
	return *this;
}

stream::stream (ostream &output_stream)
	:	sync_mutex(new mutex),
		output_stream(output_stream)
{}

void stream::update(const function<void(modifiable_stream&)> &update_operation) {
	lock_guard<mutex> lock(*this->sync_mutex);
	update_operation(this->output_stream);
}
