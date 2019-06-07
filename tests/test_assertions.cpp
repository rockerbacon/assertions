#include "assert.h"

int main (void) {
	TEST_CASE("assert true succesfully") {
		ASSERT_TRUE(true);
	} END_TEST_CASE;

	TEST_CASE("assert equal succesfully") {
		ASSERT_EQUAL(3, 3);
	} END_TEST_CASE;

	TEST_CASE("assert greater than succesfully") {
		ASSERT_GREATER_THAN(4, 2);
	} END_TEST_CASE;

	TEST_CASE("assert less than succesfully") {
		ASSERT_LESS_THAN(2, 4);
	} END_TEST_CASE;

	TEST_CASE("assert grater than or equal when value is greater") {
		ASSERT_GREATER_THAN_OR_EQUAL(4, 2);
	} END_TEST_CASE;

	TEST_CASE("assert grater than or equal when values are equal") {
		ASSERT_GREATER_THAN_OR_EQUAL(4, 4);
	} END_TEST_CASE;

	TEST_CASE("assert less than or equal when value is less") {
		ASSERT_LESS_THAN_OR_EQUAL(2, 4);
	} END_TEST_CASE;

	TEST_CASE("assert less than or equal when values are equal") {
		ASSERT_GREATER_THAN_OR_EQUAL(2, 2);
	} END_TEST_CASE;

	TEST_CASE("fail assert true") {
		ASSERT_TRUE(false);
	} END_TEST_CASE;

	TEST_CASE("fail assert equal") {
		ASSERT_EQUAL(1, 3);
	} END_TEST_CASE;

	TEST_CASE("fail assert greater than") {
		ASSERT_GREATER_THAN(2, 4);
	} END_TEST_CASE;

	TEST_CASE("fail assert less than") {
		ASSERT_LESS_THAN(4, 2);
	} END_TEST_CASE;

	TEST_CASE("fail assert grater than or equal") {
		ASSERT_GREATER_THAN_OR_EQUAL(2, 4);
	} END_TEST_CASE;

	TEST_CASE("fail assert less than or equal") {
		ASSERT_LESS_THAN_OR_EQUAL(4, 2);
	} END_TEST_CASE;

	TEST_CASE("multiple assertions in one test case - all successful") {
		ASSERT_EQUAL(2, 2);
		ASSERT_TRUE(true);
		ASSERT_LESS_THAN(2, 4);
	} END_TEST_CASE;

	TEST_CASE("multiple assertions in one test case - one fails") {
		ASSERT_EQUAL(2, 2);
		ASSERT_TRUE(false);
		ASSERT_LESS_THAN(2, 4);
	} END_TEST_CASE;

}
