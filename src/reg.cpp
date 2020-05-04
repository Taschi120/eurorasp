#include "reg.hpp"
#include "gpiod.h"
#include <chrono>
#include <thread>

ShiftRegister::ShiftRegister(int data_pin, int latch_pin, int clock_pin) {
	this->data_pin = data_pin;
	this->latch_pin = latch_pin;
	this->clock_pin = clock_pin;

	chip = gpiod_chip_open_by_number(0);
	data_ln = gpiod_chip_get_line(chip, data_pin);
	latch_ln = gpiod_chip_get_line(chip, latch_pin);
	clock_ln = gpiod_chip_get_line(chip, clock_pin);

	// TODO error handling
	gpiod_line_request_output(data_ln, "midi2cv", 0);
	gpiod_line_request_output(latch_ln, "midi2cv" ,0);
	gpiod_line_request_output(clock_ln, "midi2cv", 0);
} 

ShiftRegister::~ShiftRegister() {
	// TODO unregister pins
}

void ShiftRegister::push(unsigned char in) {
	latch_on();
	for(int i = 0; i < 8; ++i) {
		bool lsb = ((in & 1) != 0);
		send_bit(lsb);
		clock();
		in = in >> 1;
	}
	latch_off();
}

void ShiftRegister::push(unsigned short in) {
	latch_on();
	for(int i = 0; i < 16; ++i) {
		bool lsb = ((in & 1) != 0);
		send_bit(lsb);
		clock();
		in = in >> 1;
	}
	latch_off();
}

void ShiftRegister::latch_on() {
	gpiod_line_set_value(latch_ln, 0);
}

void ShiftRegister::latch_off() {
	gpiod_line_set_value(latch_ln, 1);
}

void ShiftRegister::clock() {
	gpiod_line_set_value(clock_ln, 1);
	std::this_thread::sleep_for(std::chrono::nanoseconds(1));
	gpiod_line_set_value(clock_ln, 0);
}


void ShiftRegister::send_bit(bool bit) {
	gpiod_line_set_value(data_ln, bit ? 1 : 0);
	std::this_thread::sleep_for(std::chrono::nanoseconds(1));
}
