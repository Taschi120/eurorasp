#ifndef __INCLUDE_INPUT_HPP_
#define __INCLUDE_INPUT_HPP_

#include <string>
#include <thread>
#include "pins.hpp"

#define HID_DEVICE_FILE "/dev/hidraw0"

using namespace std;

class Input {
	public:
		Input();
		~Input();

		// Poll for new input - is called from main loop
		void loop();

	private:

		string current = "";
		thread *io_reader;
		thread *keyboard_reader;
		bool was_key_pressed = false;
		char last_key_pressed = 0x0;

		void process_command(string cmd);
		void handle_keypress(char key);
		char convertScancodeToChar(unsigned int scancode);

		void cin_listen();
		void keyboard_listen();

};

#endif
