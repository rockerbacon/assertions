#include "terminal/terminal.h"
#include <thread>

using namespace terminal;

string terminal::stylize (color_code color, font_style_code font) {
	return "\E"+to_string(font)+";"+to_string(color)+"m";
}

string terminal::stylize_color (color_code color) {
	return "\E["+to_string(color)+"m";
}

string terminal::stylize_font (font_style_code font) {
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

void modifiable_output::ident (unsigned identation_size) {
	for (unsigned i = 0; i < identation_size; i++) {
		this->stream << '\t';
	}
}
void modifiable_output::print_line(const string &str, const string &style) {
	this->stream << style << str << "\E[0m" << endl;
}

void modifiable_output::clear_current_line(void) {
	this->stream << "\E[2K";
}

void modifiable_output::save_cursor_position(void) {
	this->stream << "\E[s";
}

void modifiable_output::load_cursor_position(void) {
	this->stream << "\E[u";
}

void modifiable_output::flush (void) {
	this->stream.flush();
}

output::output (ostream &output_stream) 
	:	sync_mutex(new mutex),
		output_stream(output_stream)
{}

void output::update(const function<void(modifiable_output&)> &update_operation) {
	lock_guard<mutex> lock(*this->sync_mutex);
	update_operation(this->output_stream);
}
