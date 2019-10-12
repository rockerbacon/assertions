#include "terminal/stream.h"
#include <thread>

using namespace terminal;
using namespace std;

stream::stream (ostream &output_stream)
	:	sync_mutex(new mutex),
		output_stream(output_stream)
{}

void stream::update(const function<void(ostream&)> &update_operation) {
	lock_guard<mutex> lock(*this->sync_mutex);
	update_operation(this->output_stream);
}
