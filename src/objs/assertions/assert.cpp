#include "assertions/assert.h"
#include <iostream>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <thread>
#include "assertions/stopwatch.h"

#define SUCCESS_TEXT_STYLE ::terminal::stylize_color(::terminal::bright(::terminal::color_style::GREEN))
#define FAILURE_TEXT_STYLE ::terminal::stylize_color(::terminal::bright(::terminal::color_style::RED))

using namespace std;
using namespace assert;
using namespace benchmark;

unordered_map<string, assert::test_suite> assert::test_suite_map;
terminal::stream assert::tests_output(cout);

unsigned assert::output_offset = 0;
unsigned assert::output_depth = 0;

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

void test_suite::run_test_case (const string &test_case_description, terminal::stream &terminal) {
	this->running_test_cases.push_back(thread([this, test_case_description, &terminal]() {

		using namespace terminal;

		Stopwatch stopwatch;
		string testExecutionTime;
		auto& test = (*this)[test_case_description];

		try {
			test.execute(terminal);
			testExecutionTime = stopwatch.formatedTotalTime();

			terminal.update([&](auto& terminal) {
				terminal
					<< restore_cursor_position	<< cursor_down(test.output_offset) << clear_line

						<< style< bright<color::GREEN>() >
							<< ident(test.depth) << "Test case'" << test_case_description << "': OK"
						<< style< RESET_STYLE >

						<< " (" << testExecutionTime << ")"

						<< endl

					<< restore_cursor_position << cursor_down(assert::output_offset);
			});
		} catch (const exception &e) {
			testExecutionTime = stopwatch.formatedTotalTime();

			terminal.update([&](auto& terminal) {
				terminal
					<< restore_cursor_position << cursor_down(test.output_offset) << clear_line

						<< style< bright<color::RED>() >
							<< ident(test.depth) << "Test case '" << test_case_description << "' failed: " << e.what()
						<< style< RESET_STYLE >

						<< endl

					<< restore_cursor_position << cursor_down(assert::output_offset);
			});
		}

	}));
}

void test_suite::wait_for_all_test_cases (void) {
	for (auto &running_test_case : this->running_test_cases) {
		running_test_case.join();
	}
}

test_case& test_suite::operator[](const string &test_case_description) {
	return this->test_cases[test_case_description];
}

decltype(test_suite::test_cases)::iterator test_suite::begin (void) {
	return this->test_cases.begin();
}

decltype(test_suite::test_cases)::iterator test_suite::end (void) {
	return this->test_cases.end();
}

