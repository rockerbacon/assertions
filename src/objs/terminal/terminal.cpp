#include "terminal/terminal.h"

using namespace terminal;

string terminal::stylize (color_code color, font_style_code font) {
	return "\E"+to_string(font)+";"+to_string(color)+"m";
}

string stylize_color (color_code color) {
	return "\E["+to_string(color)+"m";
}

string stylize_font (font_style_code font) {
	return "\E["+to_string(font)+"m";
}

modifiable_output::modifiable_output(ostream &stream)
	:	stream(stream)
{}

void modifiable_output::cursor_up(unsigned offset) {
	this->stream << "\E[" << offset << "A";
}

void modifiable_output::cursor_down(unsigned offset) {
	this->stream << "\E[" << offset << "B";
}

void modifiable_output::cursor_left(unsigned offset) {
	this->stream << "\E[" << offset << "D";
}

void modifiable_output::cursor_right(unsigned offset) {
	this->stream << "\E[" << offset << "C";
}

void modifiable_output::print(const string &str, const string &style) {
	this->stream << style << str << "\E[0m";
}

void modifiable_output::clear_current_line(void) {
	this->stream << "\E[2K";
}

output::output (ostream &output_stream) 
	:	sync_mutex(new mutex),
		output_stream(output_stream)
{}

void output::update(const function<void(modifiable_output&)> &update_operation) {
	lock_guard<mutex>(*this->sync_mutex);
	update_operation(this->output_stream);
}
