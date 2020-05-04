#ifndef __INCLUDE_REG_HPP_
#define __INCLUDE_REG_HPP_

#include "gpiod.h"

class ShiftRegister {
	public:
		ShiftRegister(int data_pin, int latch_pin, int clock_pin);
		~ShiftRegister();
		void push(unsigned char in);
		void push(unsigned short in);

	private:
		void latch_on();
		void latch_off();
		void clock();
		void send_bit(bool bit);
		int data_pin;
		int latch_pin;
		int clock_pin;
		struct gpiod_chip *chip;
		struct gpiod_line *data_ln;
		struct gpiod_line *latch_ln;
		struct gpiod_line *clock_ln;
};		

#endif
