#pragma once

#include <iostream>
#include <mutex>

namespace io {

	using namespace std;

	template<typename StreamType>
	class synchronous_stream {
		private:
			StreamType &stream;
			mutex stream_mutex;

		public:
			constexpr synchronous_stream(StreamType &stream) : stream(stream) {}

			void lock(void) {
				stream_mutex.lock();
			}

			void unlock(void) {
				stream_mutex.unlock();
			}

			template<typename T>
			StreamType& operator<< (const T &output) {
				this->stream << output;
				return this->stream;
			}

			template<typename T>
			StreamType& operator>> (const T &input) {
				this->stream >> input;
				return this->stream;
			}

	};
}
