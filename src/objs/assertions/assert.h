#pragma once

#include <functional>
#include <type_traits>
#include <list>
#include <thread>
#include <sstream>
#include <terminal/terminal.h>
#include <parallel/atomic.h>
#include <parallel/execution_queue.h>
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
	ASSERT_LABEL_NOT_DEFINED(assert_test_suite_block, "cannot declare test_suite outside begin_tests");\
	::assert::lines_written += ::assert::terminal->test_suite_block_begun(test_suite_description);\
	if(false) {\
		ASSERT_GENERATE_LABEL(ASSERT_LABEL_END_TEST_SUITE_BLOCK):\
			::assert::lines_written += ::assert::terminal->test_suite_block_ended();\
	} else\
		for (::assert::test_case assert_test_case_block;;)\
			if (true)\
				goto ASSERT_GENERATE_LABEL(ASSERT_LABEL_BEGIN_TEST_SUITE_BLOCK);\
			else\
				while(true)\
					if (true) {\
						goto ASSERT_GENERATE_LABEL(ASSERT_LABEL_END_TEST_SUITE_BLOCK);\
					} else\
						ASSERT_GENERATE_LABEL(ASSERT_LABEL_BEGIN_TEST_SUITE_BLOCK):

#define test_case(test_case_description)\
	ASSERT_LABEL_NOT_DEFINED(assert_test_case_block, "cannot declare test_case outside test_suite");\
	goto ASSERT_GENERATE_LABEL(ASSERT_LABEL_BEGIN_TEST_CASE_BLOCK);\
	while(true)\
		if (true) {\
			::assert::lines_written += ::assert::terminal->test_case_discovered(test_case_description);\
			::assert::queue_test_for_execution(test_case_description, ::assert::lines_written-1, assert_test_case_block);\
			break;\
		} else\
			ASSERT_GENERATE_LABEL(ASSERT_LABEL_BEGIN_TEST_CASE_BLOCK):\
				assert_test_case_block = []()

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
		NO_UNUSED_WARNING int assert_test_suite_block;\
		::assert::lines_written += ::assert::terminal->tests_begun();\

#define end_tests\
		::assert::test_execution_queue.join_unfinished_executions();\
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

	extern parallel::execution_queue test_execution_queue;

	typedef std::function<void(void)> test_case;

	class assert_failed : public std::exception {
		private:
			const std::string message;
		public:
			assert_failed(const std::string &message);

			const char* what(void) const noexcept;
	};

	void queue_test_for_execution (const std::string &test_case_description, unsigned row_in_terminal, const test_case& test);

	// dummy for checking test_suite scope
	void assert_test_case_block(void);

	// dummy for checking begin_tests scope
	void assert_test_suite_block(void);

}

