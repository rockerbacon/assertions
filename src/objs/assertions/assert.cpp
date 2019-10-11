#include "assertions/assert.h"
#include <iostream>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include "assertions/stopwatch.h"

#define ERROR_TEXT_COLOR "\033[91m"
#define SUCCESS_TEXT_COLOR "\033[92m"
#define DEFAULT_TEXT_COLOR "\033[0m"

using namespace std;
using namespace assert;
using namespace benchmark;

unordered_map<string, assert::test_suite> assert::test_suite_map;
tests_output_stream assert::tests_output(cout);

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

void test_suite::run_test_case (const string &test_case_description, tests_output_stream &test_output) {
	this->running_test_cases.push_back(thread([this, test_case_description, &test_output]() {

		Stopwatch stopwatch;

		try {
			(*this)[test_case_description](test_output);

			test_output.lock();
				test_output << '\t' << SUCCESS_TEXT_COLOR << "Test case '" << test_case_description << "' OK"
							<< DEFAULT_TEXT_COLOR << " (" << stopwatch.formatedTotalTime() << ")" << endl;
			test_output.unlock();
		} catch (const exception &e) {
			test_output.lock();
				test_output << '\t' << ERROR_TEXT_COLOR << "Test case '" << test_case_description << "' failed: " << e.what()
							<< DEFAULT_TEXT_COLOR << " (" << stopwatch.formatedTotalTime() << ")" << endl;
			test_output.unlock();
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
