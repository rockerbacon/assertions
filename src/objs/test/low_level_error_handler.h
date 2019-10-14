#pragma once

#include <benchmark/stopwatch.h>
#include <thread>

namespace test {
	void setup_signal_handlers (const std::string* test_case_description, const unsigned* row_in_terminal, const benchmark::Stopwatch* stopwatch);
}

