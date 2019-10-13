#include <terminal/live_terminal.h>
#include <terminal/manipulators.h>
#include <iostream>
#include <assertions/stopwatch.h>

using namespace std;
using namespace terminal;

live_terminal::live_terminal (void)
	:	out_stream(cout),
		current_depth(0),
		depth_map()
{}

unsigned live_terminal::tests_begun (void) {
	**this->out_stream << hide_cursor;
	return 0;
}

unsigned live_terminal::test_suite_block_begun (const string& test_suite_description) {
	auto terminal = *this->out_stream;
	this->depth_map.push_back(this->current_depth);

	*terminal
		<< ident(this->current_depth)

		<< test_suite_description << ':'

		<< '\n'

		<< cursor_up(this->depth_map.size()) << save_cursor_position
		<< cursor_down(this->depth_map.size());

	this->current_depth++;
	return 1;
}

unsigned live_terminal::test_case_discovered (const string& test_case_description) {
	auto terminal = *this->out_stream;
	this->depth_map.push_back(this->current_depth);

	*terminal
		<< ident(this->current_depth)

		<< style<font::FAINT>
			<< style<font::BLINK>
				<< icon::CIRCLE << "  "
			<< style<RESET_STYLE> << style<font::FAINT>

			<< test_case_description
		<< style<RESET_STYLE>

		<< '\n'

		<< cursor_up(this->depth_map.size()) << save_cursor_position
		<< cursor_down(this->depth_map.size());

	return 1;
}

unsigned live_terminal::test_suite_block_ended (void) {
	this->current_depth--;
	return 0;
}

void live_terminal::test_case_execution_begun (const std::string& test_case_description, unsigned row) {
	auto terminal = *this->out_stream;
	*terminal
		<< restore_cursor_position << cursor_down(row) << clear_line

		<< ident(this->depth_map[row])

		<< style<font::FAINT>
			<< style<font::BLINK>
				<< icon::CIRCLE << "  "
			<< style<RESET_STYLE> << style<font::FAINT>

			<< test_case_description
		<< style<RESET_STYLE>

		<< '\n';
}

void live_terminal::test_case_failed (const std::string& test_case_description, unsigned row, chrono::high_resolution_clock::duration test_duration, const std::string& reason) {
	auto terminal = *this->out_stream;
	*terminal
		<< restore_cursor_position << cursor_down(row) << clear_line

			<< ident(this->depth_map[row])

			<< style<bright<color::RED>()>
				<< icon::CIRCLE << "  " << test_case_description
				<< " (" << test_duration << ")"
				<< ": " << reason

		<< '\n';
}

void live_terminal::test_case_succeeded (const string& test_case_description, unsigned row, chrono::high_resolution_clock::duration test_duration) {
	auto terminal = *this->out_stream;
	*terminal
		<< restore_cursor_position	<< cursor_down(row) << clear_line

			<< ident(this->depth_map[row])

			<< style< bright<color::GREEN>() >
				<< icon::CIRCLE << "  " << test_case_description
				<< " (" << test_duration << ")"

		<< '\n';
}

void live_terminal::tests_ended (void) {
	auto terminal = *this->out_stream;
	*terminal
		<< restore_cursor_position << cursor_down(this->depth_map.size())
		<< style<RESET_STYLE>
		<< show_cursor
		<< endl;
}

