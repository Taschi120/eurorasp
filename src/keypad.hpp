#ifndef __INCLUDE_KEYPAD_HPP_
#define __INCLUDE_KEYPAD_HPP_

#include "gpiod.h"
#include <vector>

class Keypad {

	public:
		Keypad(std::vector<gpiod_line*> rows, std::vector<gpiod_line*> cols);
		~Keypad();

		char read();

	private:
		std::vector<gpiod_line*> rows;
		std::vector<gpiod_line*> cols;
		int poll_col(int x);

		static constexpr char keys[16] = {
			'1', '2', '3', 'A',
			'4', '5', '6', 'B',
			'7', '8', '9', 'C',
			'*', '0', '#', 'D'
		};
};

#endif
