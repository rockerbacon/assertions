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

void test_suite::run_test_case (const string &test_case_description) {
	this->running_test_cases.push_back(thread([this, test_case_description]() {

		stringstream test_case_output;

		Stopwatch stopwatch;

		try {
			(*this)[test_case_description](test_case_output);

			cout << SUCCESS_TEXT_COLOR << "Test case '" << test_case_description << "' OK";
			cout << DEFAULT_TEXT_COLOR << " (" << stopwatch.formatedTotalTime() << ")" << endl;
		} catch (const assert_failed &e) {
			cout << ERROR_TEXT_COLOR << "Test case '" << test_case_description << "' failed: " << test_case_output.rdbuf();
			cout << DEFAULT_TEXT_COLOR << " (" << stopwatch.formatedTotalTime() << ")" << endl;
		} catch (const exception &e) {
			cout << ERROR_TEXT_COLOR << "Test case '" << test_case_description << "' failed: " << e.what();
			cout << DEFAULT_TEXT_COLOR << " (" << stopwatch.formatedTotalTime() << ")" << endl;
		}

	}));
}

void test_suite::wait_for_all_test_cases (void) {
	for (auto &running_test_case : this->running_test_cases) {
		running_test_case.join();
	}
}

function<void(iostream &)>& test_suite::operator[](const string &test_case_description) {
	return this->test_cases[test_case_description];
}
