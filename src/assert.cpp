#include "assert.h"
#include <iostream>
#include <stdio.h>

#define ERROR_TEXT_COLOR "\033[91m"
#define SUCCESS_TEXT_COLOR "\033[92m"
#define DEFAULT_TEXT_COLOR "\033[0m"

const std::function<void(void)> assert::empty_function = [](void) -> void {};

std::string assert::test_case_title;
std::function<void(void)> assert::setup_test_case = assert::empty_function;
std::function<void(void)> assert::teardown_test_case = assert::empty_function;
std::stringstream assert::actual_value_str;
std::stringstream assert::expected_value_str;

assert::TerminalObserver concreteObserver;
const assert::Observer& assert::observer = concreteObserver;

bool assert::test_case_succeeded;

using namespace std;
using namespace assert;

error::error (const stringstream& actual_value, const string& comparator_description, const stringstream& expected_value) {
	ostringstream messageStream;
	messageStream << "expected value " << comparator_description << " " << expected_value.rdbuf() << " but got " << actual_value.rdbuf();
	this->message = messageStream.str();
}

error::error (const string& reason) {
	this->message = reason;
}

const char* error::what (void) const noexcept {
	return this->message.c_str();
}

void TerminalObserver::notify_test_case_failed (const exception& e, const string& test_case_title) const {
	cout << ERROR_TEXT_COLOR << "Test case '" << test_case_title << "' failed: " << e.what() << DEFAULT_TEXT_COLOR << endl;
}

void TerminalObserver::notify_test_case_succeeded (const string& test_case_title) const {
	cout << SUCCESS_TEXT_COLOR << "Test case '" << test_case_title << "' OK" << DEFAULT_TEXT_COLOR << endl;
}
