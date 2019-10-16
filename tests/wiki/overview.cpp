#include <test/assert.h>

using namespace std;

class Developer {
	public:
		string greet (string part_of_the_day) {
			return "good " + part_of_the_day;
		}

		string make_software (string software) {
			if (software == "first program") {
				return "printf(\"Hello World!\\n\");";
			} else {
				return "broken software";
			}
		}
};

begin_tests {
	test_suite("developers are polite") {
		test_case("they greet you in the morning") {
			Developer dev;
			auto greeting = dev.greet("morning");
			assert(greeting, ==, "good morning");
		};

		test_case("they greet you in the evening") {
			Developer dev;
			auto greeting = dev.greet("evening");
			assert(greeting, ==, "good evening");
		};
	}

	test_suite("developers know how to make software") {
		test_case("all of them begin with their first \"Hello World\"") {
			Developer rockerbacon;
			auto rockerbacons_first_program = rockerbacon.make_software("first program");
			assert(rockerbacons_first_program, ==, "printf(\"Hello World!\\n\");");
		};

		test_case("developers make broken software sometimes") {
			Developer rockerbacon;
			auto rest_api = rockerbacon.make_software("REST API in Node.js");
			assert(rest_api, ==, "broken software");
		};
	}
} end_tests;

