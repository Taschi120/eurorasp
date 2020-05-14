#ifndef __INCLUDE_INPUT_HPP_
#define __INCLUDE_INPUT_HPP_

#include <string>
#include <thread>
#include "keypad.hpp"
#include "pins.hpp"

using namespace std;

class Input {
	public:
		Input(gpiod_chip *chip);
		~Input();

		// Poll for new input - is called from main loop
		void loop();

	private:
		static constexpr int col_pins[4] = {
			PIN_KP_C0, PIN_KP_C1, PIN_KP_C2, PIN_KP_C3};
		static constexpr int row_pins[4] = {
			PIN_KP_R0, PIN_KP_R1, PIN_KP_R2, PIN_KP_R3};

		Keypad *keypad;
		string current = "";
		thread *t;
		bool was_key_pressed = false;
		char last_key_pressed = 0x0;

		void process_command(string cmd);
		void cin_listen();

};

#endif
