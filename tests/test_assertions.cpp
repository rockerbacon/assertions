#include "assert.h"

int main (void) {
	test_case("assert true succesfully") {
		assert_true(true);
	} end_test_case;

	test_case("assert equal succesfully") {
		assert_equal(3, 3);
	} end_test_case;

	test_case("assert greater than succesfully") {
		assert_greater_than(4, 2);
	} end_test_case;

	test_case("assert less than succesfully") {
		assert_less_than(2, 4);
	} end_test_case;

	test_case("assert grater than or equal when value is greater") {
		assert_greater_than_or_equal(4, 2);
	} end_test_case;

	test_case("assert grater than or equal when values are equal") {
		assert_greater_than_or_equal(4, 4);
	} end_test_case;

	test_case("assert less than or equal when value is less") {
		assert_less_than_or_equal(2, 4);
	} end_test_case;

	test_case("assert less than or equal when values are equal") {
		assert_greater_than_or_equal(2, 2);
	} end_test_case;

	test_case("fail assert true") {
		assert_true(false);
	} end_test_case;

	test_case("fail assert equal") {
		assert_equal(1, 3);
	} end_test_case;

	test_case("fail assert greater than") {
		assert_greater_than(2, 4);
	} end_test_case;

	test_case("fail assert less than") {
		assert_less_than(4, 2);
	} end_test_case;

	test_case("fail assert grater than or equal") {
		assert_greater_than_or_equal(2, 4);
	} end_test_case;

	test_case("fail assert less than or equal") {
		assert_less_than_or_equal(4, 2);
	} end_test_case;

	test_case("multiple assertions in one test case - all successful") {
		assert_equal(2, 2);
		assert_true(true);
		assert_less_than(2, 4);
	} end_test_case;

	test_case("multiple assertions in one test case - one fails") {
		assert_equal(2, 2);
		assert_true(false);
		assert_less_than(2, 4);
	} end_test_case;

	test_case("assert not equal successfuly") {
		assert_not_equal(3, 5);
	} end_test_case;

	test_case("fail assert not equal") {
		assert_not_equal(1, 1);
	} end_test_case;

}
