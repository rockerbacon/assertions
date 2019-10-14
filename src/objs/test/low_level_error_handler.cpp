#include <test/low_level_error_handler.h>
#include <utils/warnings.h>
#include <unordered_map>
#include <test/assert.h>
#include <signal.h>
#include <string.h>

using namespace std;
using namespace test;

struct test_context {
	const string* test_case_description;
	const unsigned* row_in_terminal;
	const benchmark::Stopwatch* stopwatch;
};

unordered_map<thread::id, test_context> test_execution_context;

void segfault_signalled (NO_UNUSED_WARNING int signal) {
	auto context = test_execution_context[this_thread::get_id()];
	for (auto& observer : test::observers) {
		(**observer)->test_case_failed(*context.test_case_description, *context.row_in_terminal, context.stopwatch->totalTime(), "segmentation fault");
	}
	(**test::failed_tests_count)++;
	test::test_execution_queue.terminate(this_thread::get_id());
}

void test::setup_signal_handlers (const string* test_case_description, const unsigned* row_in_terminal, const benchmark::Stopwatch* stopwatch) {
	test_execution_context[this_thread::get_id()] = {
		test_case_description,
		row_in_terminal,
		stopwatch
	};

	struct sigaction signal_action;
	memset(&signal_action, 0, sizeof(decltype(signal_action)));
	sigemptyset(&signal_action.sa_mask);
	signal_action.sa_handler = segfault_signalled;

	sigaction(SIGSEGV, &signal_action, NULL);
}

