#pragma once

#include <memory>
#include <mutex>
#include <functional>

namespace parallel {

	template<typename T>
	class atomic {
		private:
			std::unique_ptr<std::mutex> sync_mutex;
			T& object;

		public:
			atomic(T& object)
			   :	sync_mutex(new std::mutex),
				   	object(object)
			{}

			void access(const std::function<void(T&)>& update_operation) {
				std::lock_guard<std::mutex> lock(*this->sync_mutex);
				update_operation(this->object);
			}

			T get_copy (void) {
				std::lock_guard<std::mutex> lock(*this->sync_mutex);
				T copy(this->object);
				return copy;
			}

	};

}
