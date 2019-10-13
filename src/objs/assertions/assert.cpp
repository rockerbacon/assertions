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
using namespace terminal;

terminal::stream assert::cout(std::cout);

unsigned assert::lines_written = 0;
unsigned assert::depth = 0;

unsigned assert::number_of_tests = 0;
unsigned assert::successful_tests = 0;

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

void test_suite::run_all_test_cases (terminal::stream &terminal) {
	for (auto &test : *this) {
		this->running_test_cases.push_back(thread([&test, &terminal]() {

			benchmark::Stopwatch stopwatch;
			chrono::high_resolution_clock::duration testExecutionTime;

			try {
				test.execute(terminal);
				testExecutionTime = stopwatch.totalTime();

				terminal.update([&](auto& terminal) {
					terminal
						<< restore_cursor_position	<< cursor_down(test.output_offset) << clear_line

							<< style< bright<color::GREEN>() >
								<< ident(test.depth) << icon::CIRCLE << "  " << test.description
								<< " (" << testExecutionTime << ")"
							<< style< RESET_STYLE >

						<< endl;

				});
			} catch (const exception &e) {
				testExecutionTime = stopwatch.totalTime();

				terminal.update([&](auto& terminal) {
					terminal
						<< restore_cursor_position << cursor_down(test.output_offset) << clear_line

							<< style< bright<color::RED>() >
								<< ident(test.depth) << icon::CIRCLE << "  " << test.description
								<< " (" << testExecutionTime << ")"
								<< ": " << e.what()
							<< style< RESET_STYLE >


						<< endl;

				});
			}

		}));
	}
}

void test_suite::wait_for_all_test_cases (void) {
	for (auto &running_test_case : this->running_test_cases) {
		running_test_case.join();
	}
}

