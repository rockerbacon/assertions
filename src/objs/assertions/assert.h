#pragma once

#include <functional>
#include <unordered_map>
#include <type_traits>
#include <list>
#include <thread>
#include <sstream>
#include "terminal/terminal.h"

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

#define test_suite(test_suite_description)\
	::assert::test_suite_map[test_suite_description] = ::assert::test_suite();\
	::assert::tests_output.update([](auto& terminal) {\
		terminal.ident(::assert::output_depth);\
		terminal.print_line(::std::string("Test suite '")+test_suite_description+"':");\
	});\
	::assert::output_depth++;\
	::assert::output_offset++;\
	if(false) {\
		ASSERT_GENERATE_LABEL(ASSERT_LABEL_END_TEST_SUITE_BLOCK):\
			::assert::output_depth--;\
	} else\
		for (NO_UNUSED_WARNING auto &assert_test_suite_scope = ::assert::test_suite_map[test_suite_description];;)\
			if (true)\
				goto ASSERT_GENERATE_LABEL(ASSERT_LABEL_BEGIN_TEST_SUITE_BLOCK);\
			else\
				while(true)\
					if (true) {\
						goto ASSERT_GENERATE_LABEL(ASSERT_LABEL_END_TEST_SUITE_BLOCK);\
					} else\
						ASSERT_GENERATE_LABEL(ASSERT_LABEL_BEGIN_TEST_SUITE_BLOCK):

#define test_case(test_case_description)\
	{\
		using namespace assert;\
		static_assert(std::is_same<decltype(assert_test_suite_scope), ::assert::test_suite&>(), "cannot declare test_case outside a test_suite");\
	}\
	::assert::tests_output.update([](auto& terminal) {\
		terminal.ident(::assert::output_depth);\
		terminal.print_line(::std::string("Test case '")+test_case_description+"': running...");\
	});\
	assert_test_suite_scope[test_case_description];\
	assert_test_suite_scope[test_case_description].output_offset = ::assert::output_offset++;\
	assert_test_suite_scope[test_case_description].depth = ::assert::output_depth;\
	goto ASSERT_GENERATE_LABEL(ASSERT_LABEL_BEGIN_TEST_CASE_BLOCK);\
	while(true)\
		if (true) {\
			/*assert_test_suite_scope.run_test_case(test_case_description, ::assert::tests_output);*/\
			break;\
		} else\
			ASSERT_GENERATE_LABEL(ASSERT_LABEL_BEGIN_TEST_CASE_BLOCK):\
				assert_test_suite_scope[test_case_description].execute = [](NO_UNUSED_WARNING ::terminal::output& test_output) \

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

#define begin_all_tests\
	::assert::tests_output.update([](auto& terminal) {\
		terminal.save_cursor_position();\
		terminal.hide_cursor();\
	});

#define end_all_tests\
	for (auto& suite : ::assert::test_suite_map) {\
		for (auto& test : suite.second) {\
			suite.second.run_test_case(test.first, ::assert::tests_output);\
		}\
	}\
	for (auto& suite : ::assert::test_suite_map) {\
		suite.second.wait_for_all_test_cases();\
	}\
	::assert::tests_output.update([](auto& terminal) {\
		terminal.show_cursor();\
	});

namespace assert {

	using namespace std;

	struct test_case {
		function<void(terminal::output&)> execute;
		unsigned output_offset;
		unsigned depth;
	};

	class assert_failed : public exception {
		private:
			const string message;
		public:
			assert_failed(const string &message);

			const char* what(void) const noexcept;
	};

	class test_suite {
		private:
			unordered_map<string, test_case> test_cases;
			list<thread> running_test_cases;
		public:
			void run_test_case (const string& test_case_description, terminal::output& test_output);
			void wait_for_all_test_cases (void);

			test_case& operator[](const string& test_case_description);

			decltype(test_cases)::iterator begin (void);
			decltype(test_cases)::iterator end (void);
	};

	extern unordered_map<string, test_suite> test_suite_map;

	extern terminal::output tests_output;

	extern unsigned output_offset;
	extern unsigned output_depth;

	// dummy for checking test_suite scope
	void assert_test_suite_scope(void);

};

