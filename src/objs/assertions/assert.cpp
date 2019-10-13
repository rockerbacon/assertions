#include "assertions/assert.h"
#include <iostream>
#include <stdio.h>
#include <signal.h>
#include <string>
#include <thread>
#include <assertions/stopwatch.h>
#include <terminal/live_terminal.h>

#define SUCCESS_TEXT_STYLE ::terminal::stylize_color(::terminal::bright(::terminal::color_style::GREEN))
#define FAILURE_TEXT_STYLE ::terminal::stylize_color(::terminal::bright(::terminal::color_style::RED))

using namespace std;
using namespace assert;
using namespace terminal;

unique_ptr<terminal_interface> assert::terminal(new live_terminal);

unsigned assert::lines_written = 0;

parallel::atomic<unsigned> assert::successful_tests_count(0);
parallel::atomic<unsigned> assert::failed_tests_count(0);
/*
void segfault_signalled (int signal) {
	auto elapsed_time = chrono::high_resolution_clock::now() - assert::test_case_start;
	cout << ERROR_TEXT_COLOR << "Test case '" << assert::test_case_title << "' failed: segmentation fault, testing cannot continue" << DEFAULT_TEXT_COLOR << " (" << elapsed_time << ")" << endl;
	exit(signal);
}

void assert::run_first_setup_if_needed (void) {
	if (!assert::first_setup_done) {
		memset(&signal_action, 0, sizeof(decltype(signal_action)));
		sigemptyset(&signal_action.sa_mask);
		signal_action.sa_handler = segfault_signalled;

		sigaction(SIGSEGV, &signal_action, NULL);
		assert::first_setup_done = true;
	}
}
*/

assert_failed::assert_failed (const string &message)
	: message(message)
{}

const char* assert_failed::what(void) const noexcept {
	return this->message.c_str();
}

test_suite_t::test_suite_t (unsigned number_of_threads)
	:	running_test_cases(number_of_threads)
{}

void test_suite_t::run_all_test_cases (void) {
	for (auto &test : *this) {
		this->running_test_cases.push_back([&test]() {

			benchmark::Stopwatch stopwatch;
			chrono::high_resolution_clock::duration test_duration;

			assert::terminal->test_case_execution_begun(test.description, test.row_in_terminal);
			try {
				test.execute();
				test_duration = stopwatch.totalTime();
				(**assert::successful_tests_count)++;
				assert::terminal->test_case_succeeded(test.description, test.row_in_terminal, test_duration);
			} catch (const exception &e) {
				test_duration = stopwatch.totalTime();
				(**assert::failed_tests_count)++;
				assert::terminal->test_case_failed(test.description, test.row_in_terminal, test_duration, e.what());
			}
		});
	}
}

void test_suite_t::wait_for_all_test_cases (void) {
	this->running_test_cases.join_unfinished_executions();
}

