#pragma once

#include <exception>
#include <sstream>
#include <iostream>
#include <functional>

#define SUCCESS_STATUS_CODE 0
#define ASSERTION_ERROR_STATUS_CODE 1
#define TEST_IGNORED_STATUS_CODE 2
#define UNEXPECTED_EXCEPTION_STATUS_CODE 3

#define EXCEPTION_FOR_NEXT_TEST_CASE

#define BEFORE_NEXT_TEST_CASE(setup_function) assert::setup_test_case = setup_function
#define AFTER_NEXT_TEST_CASE(teardown_function) assert::teardown_test_case = teardown_function

#define IGNORE_REST_OF_TEST_CASE() throw assert::test_case_interruption()

/*
#define EXPECT_EXCEPTION_ON_NEXT_TEST(exception_type) #define EXCEPTION_FOR_NEXT_TEST_CASE\
	} catch (const exception_type& e) {\
		assert::expected_excetion_raised(e);
*/

#define TEST_CASE(str) assert::test_case_title = str;\
	assert::test_case_succeeded = true;\
	assert::setup_test_case();\
	try

#define CLEAR_TEST_CASE assert::teardown_test_case();\
	BEFORE_NEXT_TEST_CASE(assert::empty_function);\
	AFTER_NEXT_TEST_CASE(assert::empty_function)

#define END_TEST_CASE catch (const assert::error &e) {\
		assert::test_case_succeeded = false;\
		assert::observer.notify_test_case_failed(e, assert::test_case_title);\
		CLEAR_TEST_CASE;\
	} catch (const assert::test_case_interruption& e) {\
		CLEAR_TEST_CASE;\
	/* }\
	 EXCEPTION_FOR_NEXT_TEST_CASE\ */\
	} catch (const std::exception &e) {\
		assert::test_case_succeeded = false;\
		assert::teardown_test_case();\
		assert::observer.notify_test_case_failed(e, assert::test_case_title);\
	}\
	/*	#undef EXCEPTION_FOR_NEXT_TEST_CASE\
		#define EXCEPTION_FOR_NEXT_TEST_CASE\ */\
	if (assert::test_case_succeeded) {\
		assert::observer.notify_test_case_succeeded(assert::test_case_title);\
	}\
	CLEAR_TEST_CASE;

#define ASSERT_TRUE(condition) if (!condition) {\
		throw assert::error("condition was false");\
	}

#define ASSERT_EQUAL(actual_value, expected_value) assert::actual_value_str << actual_value;\
	assert::expected_value_str << expected_value;\
	if (actual_value != expected_value) {\
		throw assert::error(assert::actual_value_str, "equal", assert::expected_value_str);\
	}\
	std::stringstream().swap(assert::actual_value_str);\
	std::stringstream().swap(assert::expected_value_str)
#define ASSERT_GREATER_THAN(actual_value, expected_value) assert::actual_value_str << actual_value;\
	assert::expected_value_str << expected_value;\
	if (actual_value <= expected_value) {\
		throw assert::error(assert::actual_value_str, "greater than", assert::expected_value_str);\
	}\
	std::stringstream().swap(assert::actual_value_str);\
	std::stringstream().swap(assert::expected_value_str)
#define ASSERT_LESS_THAN(actual_value, expected_value) assert::actual_value_str << actual_value;\
	assert::expected_value_str << expected_value;\
	if (actual_value >= expected_value) {\
		throw assert::error(assert::actual_value_str, "less than", assert::expected_value_str);\
	}\
	std::stringstream().swap(assert::actual_value_str);\
	std::stringstream().swap(assert::expected_value_str)
#define ASSERT_GREATER_THAN_OR_EQUAL(actual_value, expected_value) assert::actual_value_str << actual_value;\
	assert::expected_value_str << expected_value;\
	if (actual_value < expected_value) {\
		throw assert::error(assert::actual_value_str, "greater than or equal", assert::expected_value_str);\
	}\
	std::stringstream().swap(assert::actual_value_str);\
	std::stringstream().swap(assert::expected_value_str)
#define ASSERT_LESS_THAN_OR_EQUAL(actual_value, expected_value) assert::actual_value_str << actual_value;\
	assert::expected_value_str << expected_value;\
	if (actual_value > expected_value) {\
		throw assert::error(assert::actual_value_str, "less than or equal", assert::expected_value_str);\
	}\
	std::stringstream().swap(assert::actual_value_str);\
	std::stringstream().swap(assert::expected_value_str)
#define FAIL_TEST(reason) throw assert::error(reason)



namespace assert {

	extern const std::function<void(void)> empty_function;

	extern std::string test_case_title;
	extern std::function<void(void)> setup_test_case;
	extern std::function<void(void)> teardown_test_case;
	extern std::stringstream actual_value_str;
	extern std::stringstream expected_value_str;
	extern bool test_case_succeeded;


	class error : public std::exception {
		private:
			std::string message;
		public:
			error (const std::stringstream& actual_value, const std::string& comparator_description, const std::stringstream& expected_value);
			error (const std::string& reason);

			virtual const char* what (void) const noexcept;
	};

	class test_case_interruption : public std::exception {};

	class Observer {
		public:
			virtual void notify_test_case_failed(const std::exception& e, const std::string& test_case_title) const = 0;
			virtual void notify_test_case_succeeded (const std::string& test_case_title) const = 0;
	};

	class TerminalObserver : public Observer {
		public:
			virtual void notify_test_case_failed(const std::exception& e, const std::string& test_case_title) const;
			virtual void notify_test_case_succeeded (const std::string& test_case_title) const;
	};

	extern const Observer& observer;

};
