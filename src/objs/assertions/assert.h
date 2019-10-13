#pragma once

#include <functional>
#include <type_traits>
#include <list>
#include <thread>
#include <sstream>
#include <terminal/terminal.h>
#include <parallel/atomic.h>
#include <iostream>

#if __cplusplus >= 201703L
	#define NO_UNUSED_WARNING [[maybe_unused]]
#else
	#ifdef __GNUC__
		#define NO_UNUSED_WARNING __attribute__((unused))
	#else
		#define NO_UNUSED_WARNING
	#endif
#endif

#define ASSERT_GENERATE_LABEL_PASTE_EXPAND(labelid, line) labelid ## _ ## line
#define ASSERT_GENERATE_LABEL_PASTE(labelid, line) ASSERT_GENERATE_LABEL_PASTE_EXPAND(labelid, line)
#define ASSERT_GENERATE_LABEL(labelid) ASSERT_GENERATE_LABEL_PASTE(labelid, __LINE__)

#define ASSERT_LABEL_NOT_DEFINED(label, error_message)\
	{\
		using namespace assert;\
		static_assert(!::std::is_same<decltype(label), decltype(::assert::label)>(), error_message);\
	}

#define test_suite(test_suite_description)\
	ASSERT_LABEL_NOT_DEFINED(assert_tests_scope, "cannot declare test_suite outside begin_tests");\
	::assert::lines_written += ::assert::terminal->test_suite_block_begun(test_suite_description);\
	assert_tests_scope.emplace_back();\
	if(false) {\
		ASSERT_GENERATE_LABEL(ASSERT_LABEL_END_TEST_SUITE_BLOCK):\
			::assert::lines_written += ::assert::terminal->test_suite_block_ended();\
	} else\
		for (NO_UNUSED_WARNING auto &assert_test_suite_scope = assert_tests_scope.back();;)\
			if (true)\
				goto ASSERT_GENERATE_LABEL(ASSERT_LABEL_BEGIN_TEST_SUITE_BLOCK);\
			else\
				while(true)\
					if (true) {\
						goto ASSERT_GENERATE_LABEL(ASSERT_LABEL_END_TEST_SUITE_BLOCK);\
					} else\
						ASSERT_GENERATE_LABEL(ASSERT_LABEL_BEGIN_TEST_SUITE_BLOCK):

#define test_case(test_case_description)\
	ASSERT_LABEL_NOT_DEFINED(assert_test_suite_scope, "cannot declare test_case outside test_suite");\
	assert_test_suite_scope.emplace_back();\
	assert_test_suite_scope.back().row_in_terminal = ::assert::lines_written;\
	assert_test_suite_scope.back().description = test_case_description;\
	::assert::lines_written += ::assert::terminal->test_case_discovered(test_case_description);\
	goto ASSERT_GENERATE_LABEL(ASSERT_LABEL_BEGIN_TEST_CASE_BLOCK);\
	while(true)\
		if (true) {\
			break;\
		} else\
			ASSERT_GENERATE_LABEL(ASSERT_LABEL_BEGIN_TEST_CASE_BLOCK):\
				assert_test_suite_scope.back().execute = []()

#define ASSERT_FAIL_MESSAGE(actual_value, comprasion_operator_string, expected_value)\
	"expected " << actual_value << " to be " << comprasion_operator_string << ' ' << expected_value

#define assert_true(condition)\
   	if (!(condition)) {\
		throw ::assert::assert_failed("condition was false");\
	}

#define assert(actual_value, comparison_operator, expected_value)\
	if (!((actual_value) comparison_operator (expected_value))) {\
		stringstream error_message;\
		error_message << ASSERT_FAIL_MESSAGE(actual_value, #comparison_operator, expected_value);\
		throw ::assert::assert_failed(error_message.str());\
	}

#define begin_tests\
	int main (void) {\
		::assert::lines_written += ::assert::terminal->tests_begun();\
		::std::list<::assert::test_suite> assert_tests_scope;\

#define end_tests\
		for (auto& suite : assert_tests_scope) {\
			suite.run_all_test_cases();\
		}\
		for (auto& suite : assert_tests_scope) {\
			suite.wait_for_all_test_cases();\
		}\
		::assert::terminal->tests_ended();\
		::std::cerr << "successful_tests=" << **::assert::successful_tests_count << ::std::endl;\
		::std::cerr << "failed_tests=" << **::assert::failed_tests_count<< std::endl;\
		return **::assert::failed_tests_count;\
	}

namespace assert {

	extern std::unique_ptr<terminal::terminal_interface> terminal;

	extern unsigned lines_written;

	extern parallel::atomic<unsigned> successful_tests_count;
	extern parallel::atomic<unsigned> failed_tests_count;

	struct test_case {
		std::function<void(void)> execute;
		unsigned row_in_terminal;
		std::string description;
	};

	class assert_failed : public std::exception {
		private:
			const std::string message;
		public:
			assert_failed(const std::string &message);

			const char* what(void) const noexcept;
	};

	class test_suite : public std::list<test_case> {
		private:
			std::list<std::thread> running_test_cases;
		public:
			void run_all_test_cases (void);
			void wait_for_all_test_cases (void);

	};

	// dummy for checking test_suite scope
	void assert_test_suite_scope(void);

	// dummy for checking begin_tests scope
	void assert_tests_scope(void);

};

