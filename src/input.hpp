#ifndef __INCLUDE_INPUT_HPP_
#define __INCLUDE_INPUT_HPP_

#include <string>
#include <thread>
#include "pins.hpp"

class Input {
	public:
		Input();
		~Input();

		std::string getBuffer();
		void clearBuffer();

	private:

		std::string buffer = "";
		std::thread *keyboard_reader;
		bool was_key_pressed = false;
		char last_key_pressed = 0x0;

		void handle_keypress(char key);
		char convertScancodeToChar(unsigned int scancode);

		void keyboard_listen();

};

#endif
