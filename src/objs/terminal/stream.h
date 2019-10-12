#pragma once

#include <iostream>
#include <mutex>
#include <memory>
#include <functional>

namespace terminal {

	class stream {
		private:
			std::unique_ptr<std::mutex> sync_mutex;
			std::ostream &output_stream;

		public:
			stream(std::ostream& output_stream);

			void update(const std::function<void(std::ostream&)>& manipulator);

	};

}
