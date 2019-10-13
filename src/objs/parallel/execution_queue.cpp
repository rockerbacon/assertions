#include <parallel/execution_queue.h>

using namespace parallel;
using namespace std;

execution_queue::execution_queue (unsigned queue_size)
	:	max_queue_size(queue_size),
		available_threads(queue_size),
		queued_executions(0)
{}

void execution_queue::push_back(const std::function<void(void)>& execution) {
	thread([this, execution]() {
		{
			unique_lock<std::mutex> lock(this->mutex);

			this->queued_executions++;

			this->notifier.wait(lock, [this]() {
				return this->available_threads > 0;
			});

			this->queued_executions--;
			this->available_threads--;
		}

		execution();

		{
			unique_lock<std::mutex> lock(this->mutex);
			this->available_threads++;
			this->notifier.notify_one();
		}
	}).detach();
}

void execution_queue::join_unfinished_executions (void) {
	unique_lock<std::mutex> lock(this->mutex);

	this->notifier.wait(lock, [this]() {
		return this->available_threads == this->max_queue_size && this->queued_executions == 0;
	});
}

