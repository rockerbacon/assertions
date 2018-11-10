#pragma once

#include <exception>
#include <sstream>
#include <iostream>

namespace assertion {

	/*
	 * Exception that's thrown whenever an assertion error occurs
	 */
	class assert_error : public std::exception {
		private:
			std::string message;
		public:
			assert_error (const std::string& message);

			const char* what (void) const throw();
	};

	/*
	 * Classes used as arguments for the operator argument in the assert function
	 */
	template<typename T>
	class Comparator {
		public:
			virtual bool compare (const T& a, const T& b) const = 0;

			virtual std::string toString (void) const = 0;
	};
	//Class used for negating the comparator passed in the constructor
	template<typename T>
	class Not : public Comparator<T> {
		private:
			const Comparator<T>& cmp;
		public:
			inline Not (const Comparator<T>& cmp) : cmp(cmp) {}

			inline bool compare(const T& a, const T& b) const { return !this->cmp.compare(a, b); }

			inline std::string toString (void) const { return "not "+this->cmp.toString(); }
	};
	template<typename T>
	class Equals : public Comparator<T> {
		public:
			inline bool compare (const T& a, const T& b) const { return a == b; }

			inline std::string toString (void) const { return "=="; }
	};
	template<typename T>
	class GreaterThan : public Comparator<T> {
		public:
			inline bool compare (const T& a, const T& b) const { return a > b; }

			inline std::string toString (void) const { return ">"; }
	};
	template<typename T>
	class LessThan : public Comparator<T> {
		public:
			inline bool compare (const T& a, const T& b) const { return a < b; }

			inline std::string toString (void) const { return "<"; }
	};
	template<typename T>
	class GreaterOrEquals : public Comparator<T> {
		public:
			inline bool compare (const T& a, const T& b) const { return a >= b; }

			inline std::string toString (void) const { return ">="; }
	};
	template<typename T>
	class LessOrEquals : public Comparator<T> {
		public:
			inline bool compare (const T& a, const T& b) const { return a <= b; }

			inline std::string toString (void) const { return "<="; }
	};

	/*
	 * Function for asserting test values
	 *
	 * @param actual_value: value to be tested
	 * @param comparator: Comparator object used for testing actual_value against expected_value
	 * @param expected_value: value used for testing actual_value
	 *
	 * @throw assert_error: Thrown when actual_value fails assertion
	 *
	 */
	template<typename T>
	inline void assert (const T& actual_value, const Comparator<T>& comparator, const T& expected_value) {
		if (!comparator.compare(actual_value, expected_value)) {
			std::ostringstream errMsg;
			errMsg << "Assertion failed: expected a value " << comparator.toString() << " " << expected_value << " but got " << actual_value;
			throw assert_error(errMsg.str().c_str());
		}
	}

};
