#pragma once

#include <iostream>
#include <mutex>
#include <memory>
#include <functional>

namespace terminal {

	using namespace std;
	typedef unsigned char color_code;
	typedef unsigned char font_style_code;

	namespace color_style {
		constexpr color_code BLACK 		= 30;
		constexpr color_code RED		= 31;
		constexpr color_code GREEN 		= 32;
		constexpr color_code BROWN 		= 33;
		constexpr color_code BLUE 		= 34;
		constexpr color_code MAGENTA 	= 35;
		constexpr color_code CYAN		= 36;
		constexpr color_code WHITE		= 37;
		constexpr color_code DEFAULT 	= 39;
	};
	constexpr color_code bright(color_code code) {
		return code+60;
	}

	namespace font_style {
		constexpr font_style_code BOLD		= 1;
		constexpr font_style_code FAINT		= 2;
		constexpr font_style_code ITALIC	= 3;
		constexpr font_style_code UNDERLINE	= 4;
	};

	string stylize (color_code color, font_style_code font);
	string stylize_color (color_code color);
	string stylize_font (font_style_code font);

	class modifiable_output {
		private:
			ostream& stream;
		public:
			modifiable_output(ostream &stream);	

			void cursor_up(unsigned offset);
			void cursor_down(unsigned offset);
			void cursor_left(unsigned offset);
			void cursor_right(unsigned offset);

			void print (const string &str, const string &style = "");

			void clear_current_line(void);

	};
	class output {
		private:
			unique_ptr<mutex> sync_mutex; 
			modifiable_output output_stream;

		public:
			output(ostream& output_stream);

			void update(const function<void(modifiable_output&)> &update_operation);

	};
}
