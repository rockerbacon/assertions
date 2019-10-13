#include <parallel/execution_queue.h>

using namespace parallel;
using namespace std;

execution_queue::execution_queue (unsigned queue_size)
	:	max_queue_size(queue_size),
		active_executions(0),
		queued_executions(0)
{}

void execution_queue::push_back(const std::function<void(void)>& execution) {
	(**this->queued_executions)++;
	thread([this, execution]() {
		unique_lock<mutex> lock(this->semaphore_mutex);

		this->notifier.wait(lock, [this]() {
			return this->active_executions < this->max_queue_size;
		});

		(**this->queued_executions)--;
		this->active_executions++;

		execution();

		this->active_executions--;

		this->notifier.notify_one();
	}).detach();
}

void execution_queue::join_unfinished_executions (void) {
	unique_lock<mutex> lock(this->semaphore_mutex);

	this->notifier.wait(lock, [this]() {
		return this->active_executions == 0 && **this->queued_executions == 0;
	});
}

