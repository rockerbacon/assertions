#include <parallel/execution_queue.h>

using namespace parallel;
using namespace std;

execution_queue::execution_queue (unsigned queue_size)
	:	max_queue_size(queue_size)
{}

void execution_queue::push_back(const std::function<void(void)>& execution) {
	unique_lock<mutex> lock(this->semaphore_mutex);

	this->notifier.wait(lock, [this]() {
		return (**this->queue).size() < this->max_queue_size;
	});

	{
		auto queue = *this->queue;
		list<thread>::iterator thread_list_position;

		(*queue).emplace_front();
		thread_list_position = (*queue).begin();

		*thread_list_position = thread([this, thread_list_position, execution]() {

			execution();

			(**this->queue).erase(thread_list_position);
			this->notifier.notify_one();

		});
	}

}

void execution_queue::join_unfinished_executions (void) {
	for (auto& thread : **this->queue) {
		thread.join();
	}
}

