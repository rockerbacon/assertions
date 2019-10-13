#pragma once

#include <mutex>
#include <condition_variable>
#include <thread>
#include <list>
#include <functional>
#include <parallel/atomic.h>

namespace parallel {

	class execution_queue {
		private:
			std::mutex semaphore_mutex;
			std::condition_variable notifier;
			atomic<std::list<std::thread>> queue;
			const unsigned max_queue_size;
		public:
			execution_queue(unsigned queue_size);

			void push_back(const std::function<void(void)>& execution);
			void join_unfinished_executions (void);
	};

}
