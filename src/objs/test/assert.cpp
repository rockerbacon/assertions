#include <test/assert.h>
#include <benchmark/stopwatch.h>
#include <test/live_terminal.h>
#include <unordered_map>
#include <test/low_level_error_handler.h>

#define SUCCESS_TEXT_STYLE ::terminal::stylize_color(::terminal::bright(::terminal::color_style::GREEN))
#define FAILURE_TEXT_STYLE ::terminal::stylize_color(::terminal::bright(::terminal::color_style::RED))

using namespace std;
using namespace test;

list<parallel::atomic<observer*>> test::observers;

unsigned test::elements_discovered = 0;

parallel::atomic<unsigned> test::successful_tests_count(0);
parallel::atomic<unsigned> test::failed_tests_count(0);

parallel::execution_queue test::test_execution_queue(thread::hardware_concurrency());

assert_failed::assert_failed (const string &message)
	: message(message)
{}

const char* assert_failed::what(void) const noexcept {
	return this->message.c_str();
}

void test::queue_test_for_execution (const string &test_case_description, unsigned row_in_terminal, const test_case& test) {
	test::test_execution_queue.push_back([=]() {
		benchmark::Stopwatch stopwatch;
		chrono::high_resolution_clock::duration test_duration;

		test::setup_signal_handlers(&test_case_description, &row_in_terminal, &stopwatch);

		for (auto& observer : test::observers) {
			(**observer)->test_case_execution_begun(test_case_description, row_in_terminal);
		}
		try {
			test();
			test_duration = stopwatch.totalTime();
			(**test::successful_tests_count)++;
			for (auto& observer : test::observers) {
				(**observer)->test_case_succeeded(test_case_description, row_in_terminal, test_duration);
			}
		} catch (const exception &e) {
			test_duration = stopwatch.totalTime();
			(**test::failed_tests_count)++;
			for (auto& observer : test::observers) {
				(**observer)->test_case_failed(test_case_description, row_in_terminal, test_duration, e.what());
			}
		}
	});
}

