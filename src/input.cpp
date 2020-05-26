#include "input.hpp"

#include "keypad.hpp"

#include "global.hpp"
#include <gpiod.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <vector>
#include <cassert>

using namespace std;
using namespace std::chrono;
using namespace global;

Input::Input(gpiod_chip *chip) {
	// get gpiod lines for keypad
	vector<gpiod_line*> rows;
	vector<gpiod_line*> cols;

	for(int i = 0; i < 4; i++) {
		gpiod_line *line = gpiod_chip_get_line(chip, col_pins[i]);
		gpiod_line_request_output(line, "keypad", 0);
		cols.push_back(line);
	}

	for(int i = 0; i < 4; ++i) {
		gpiod_line *line = gpiod_chip_get_line(chip, row_pins[i]);
		gpiod_line_request_input(line, "keypad");
		rows.push_back(line);
	}

	keypad = new Keypad(rows, cols);
	t = new thread(&Input::cin_listen, this);
}

Input::~Input() {

}

void Input::loop() {
	char c = keypad->read();

	if (c == 0x0) {
        if (was_key_pressed) {
            cout << "Keystroke: " << last_key_pressed << endl;
        }
		was_key_pressed = false;
	} else if ((!was_key_pressed) || (c != last_key_pressed)) {
		was_key_pressed = true;
		last_key_pressed = c;

		if (c == '#' && current.length() > 0) {
			// treat as "OK"
			process_command(current);
			current = "";
		} else if (c == '*' && current.length() > 0) {
			current.pop_back();
		} else {
			current.push_back(c);
		}
	}
}

void Input::process_command(string cmd) {
	std::cout << "Running Command: " << cmd << std::endl;
	if (cmd[0] == 'A') {
		// Switch MIDI device
		string num = cmd.substr(1, -1);
		try {
			int device = stoi(num);
			// TODO change midi device
		} catch (const invalid_argument& e) {
			std::cerr << "Not a number: " << num << std::endl;
		}
	} else {
        cerr << "Invalid command: " << cmd << endl;
	}
}

void Input::cin_listen() {
	while(true) {
		cout << "Enter command: " << endl;
		string val;
		cin >> val;

		process_command(val);
	}
}


