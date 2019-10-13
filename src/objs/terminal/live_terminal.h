#pragma once

#include <terminal/terminal.h>
#include <unordered_map>
#include <parallel/atomic.h>
#include <vector>

namespace terminal {

	class live_terminal : public terminal_interface {
		private:
			parallel::atomic<std::ostream&> out_stream;
			unsigned current_depth;
			std::vector<unsigned> depth_map;
		public:
			live_terminal(void);

			unsigned tests_begun (void);
			unsigned test_suite_block_begun (const std::string& test_suite_description);
			unsigned test_case_discovered (const std::string& test_case_description);
			unsigned test_suite_block_ended (void);
			void test_case_execution_begun (const std::string& test_case_description, unsigned row);
			void test_case_failed (const std::string& test_case_description, unsigned row, std::chrono::high_resolution_clock::duration test_duration, const std::string& reason);
			void test_case_succeeded (const std::string& test_case_description, unsigned row, std::chrono::high_resolution_clock::duration test_duration);
			void tests_ended (void);
	};

}
