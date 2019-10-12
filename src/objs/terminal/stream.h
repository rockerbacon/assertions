#pragma once

#include <iostream>
#include <mutex>
#include <memory>
#include <functional>

namespace terminal {

	class modifiable_stream {
		private:
			std::ostream& stream;
		public:
			modifiable_stream(std::ostream &stream);

			template<typename T>
			modifiable_stream& operator<< (const T& t) {
				this->stream << t;
				return *this;
			}

			modifiable_stream& operator<< (std::ostream& (*manipulator)(std::ostream&));
	};

	class stream {
		private:
			std::unique_ptr<std::mutex> sync_mutex;
			modifiable_stream output_stream;

		public:
			stream(std::ostream& output_stream);

			void update(const std::function<void(modifiable_stream&)>& manipulator);

	};

}
