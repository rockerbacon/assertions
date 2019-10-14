#include <test/assert.h>
#include <iostream>
#include <stdio.h>
#include <signal.h>
#include <string>
#include <thread>
#include <benchmark/stopwatch.h>
#include <test/live_terminal.h>

#define SUCCESS_TEXT_STYLE ::terminal::stylize_color(::terminal::bright(::terminal::color_style::GREEN))
#define FAILURE_TEXT_STYLE ::terminal::stylize_color(::terminal::bright(::terminal::color_style::RED))

using namespace std;
using namespace test;

list<parallel::atomic<observer*>> test::observers;

unsigned test::elements_discovered = 0;

parallel::atomic<unsigned> test::successful_tests_count(0);
parallel::atomic<unsigned> test::failed_tests_count(0);

parallel::execution_queue test::test_execution_queue(thread::hardware_concurrency());

/*
void segfault_signalled (int signal) {
	auto elapsed_time = chrono::high_resolution_clock::now() - test::test_case_start;
	cout << ERROR_TEXT_COLOR << "Test case '" << test::test_case_title << "' failed: segmentation fault, testing cannot continue" << DEFAULT_TEXT_COLOR << " (" << elapsed_time << ")" << endl;
	exit(signal);
}

void test::run_first_setup_if_needed (void) {
	if (!test::first_setup_done) {
		memset(&signal_action, 0, sizeof(decltype(signal_action)));
		sigemptyset(&signal_action.sa_mask);
		signal_action.sa_handler = segfault_signalled;

		sigaction(SIGSEGV, &signal_action, NULL);
		test::first_setup_done = true;
	}
}
*/

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

