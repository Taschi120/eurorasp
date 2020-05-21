#include "keypad.hpp"
#include "assert.h"
#include <vector>

Keypad::Keypad(std::vector<gpiod_line*> rows, std::vector<gpiod_line*> cols){
	assert(rows.size() == 4);
	assert(cols.size() == 4);

	this->rows = rows;
	this->cols = cols;
}

Keypad::~Keypad(){

}

char Keypad::read() {
	for(int x = 0; x < 4; x++) {
		int y = poll_col(x);
		if(y >= 0) {
			return keys[(x*4) + y];
		}
	}
	return 0;
}

int Keypad::poll_col(int x) {
	gpiod_line_set_value(cols.at(x), 1);
	int result = -1;
	for (int y = 0; y < 4; y++) {
		if(gpiod_line_get_value(rows.at(y)) == 1) {
			result = y;
			break;
		}
	}
	gpiod_line_set_value(cols.at(x), 0);
	return result;
}


