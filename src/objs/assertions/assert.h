#pragma once

#include <exception>
#include <sstream>
#include <iostream>
#include <chrono>
#include <functional>
#include <unordered_map>
#include <type_traits>
#include <list>
#include <thread>

#if __cplusplus >= 201703L 
	#define ASSERT_NO_UNUSED_WARNING [[maybe_unused]]
#else
	#ifdef __GNUC__
		#define ASSERT_NO_UNUSED_WARNING __attribute__((unused))
	#else
		#define ASSERT_NO_UNUSED_WARNING
	#endif
#endif

#define ASSERT_GENERATE_LABEL_PASTE_EXPAND(labelid, line) labelid ## _ ## line
#define ASSERT_GENERATE_LABEL_PASTE(labelid, line) ASSERT_GENERATE_LABEL_PASTE_EXPAND(labelid, line)
#define ASSERT_GENERATE_LABEL(labelid) ASSERT_GENERATE_LABEL_PASTE(labelid, __LINE__)

#define test_suite(test_suite_description)\
	::assert::test_suite_map[test_suite_description] = ::assert::test_suite();\
	if(false) {\
		ASSERT_GENERATE_LABEL(ASSERT_LABEL_END_TEST_SUITE_BLOCK):\
			::assert::test_suite_map[test_suite_description].wait_for_all_test_cases();\
	} else\
		for (auto &assert_test_suite_scope = ::assert::test_suite_map[test_suite_description];;)\
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
	goto ASSERT_GENERATE_LABEL(ASSERT_LABEL_BEGIN_TEST_CASE_BLOCK);\
	while(true)\
		if (true) {\
			assert_test_suite_scope.run_test_case(test_case_description);\
			break;\
		} else\
			ASSERT_GENERATE_LABEL(ASSERT_LABEL_BEGIN_TEST_CASE_BLOCK):\
				assert_test_suite_scope[test_case_description] = [](ASSERT_NO_UNUSED_WARNING ::std::iostream& assert_output_stream) \

#define ASSERT_FAIL_MESSAGE(actual_value, comprasion_operator_string, expected_value)\
	"expected " << actual_value << " to be " << comprasion_operator_string << ' ' << expected_value

#define assert_true(condition)\
   	if (!(condition)) {\
		assert_output_stream << "condition was false";\
		throw ::assert::assert_failed();\
	}

#define assert(actual_value, comparison_operator, expected_value)\
	if (!((actual_value) comparison_operator (expected_value))) {\
		assert_output_stream << ASSERT_FAIL_MESSAGE(actual_value, #comparison_operator, expected_value);\
		throw ::assert::assert_failed();\
	}

namespace assert {

	using namespace std;

	typedef function<void(iostream &)> test_case;

	class assert_failed : public std::exception {};

	class test_suite {
		private:
			unordered_map<string, test_case> test_cases;
			list<thread> running_test_cases;
		public:
			void run_test_case (const string &test_case_description);
			void wait_for_all_test_cases (void);

			function<void(iostream &)>& operator[](const string &test_case_description);

	};

	extern unordered_map<string, test_suite> test_suite_map;

	// dummy for checking test_suite scope
	void assert_test_suite_scope(void);

};

constexpr bool assert_test_case_inside_test_suite_scope (void) {
	using namespace assert;
	return !std::is_same<decltype(assert_test_suite_scope), void*>();
}
