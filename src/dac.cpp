#include "dac.hpp"
#include "gpiod.h"
#include <iostream>
#include <thread>
#include <chrono>

#define MAX_PAYLOAD 0x0fff
#define CHANNEL_A_MASK 0x0000
#define CHANNEL_B_MASK 0x8000
#define OTHER_PARAMS_MASK 0x7000


using namespace std::chrono;
using namespace std;

DualDac::DualDac(int data_pin, int latch_pin,
		int clock_pin, int cs_pin) : 
	ShiftRegister(data_pin, latch_pin, clock_pin){

	cs_ln = gpiod_chip_get_line(chip, cs_pin);
	// CS input is active-low
	gpiod_line_request_output(cs_ln, "midi2cv", 1);	
}

DualDac::~DualDac() {
	// TODO
}

void DualDac::cs_on() {
	gpiod_line_set_value(cs_ln, 0);
	this_thread::sleep_for(nanoseconds(40));
}

void DualDac::cs_off() {
	gpiod_line_set_value(cs_ln, 1);
	this_thread::sleep_for(nanoseconds(15));
}

void DualDac::push(unsigned short in) {
	cs_on();
	ShiftRegister::push(in);
	cs_off();
}

void DualDac::push(unsigned char in) {
	std::cerr << "8bit operations not allowed on this DAC" << std::endl;
}

void DualDac::push(unsigned short in, int channel) {
	if (in > MAX_PAYLOAD) {
		std::cerr << "DAC payload " << in << " out of bounds (max " 
			<< MAX_PAYLOAD << ")" << std::endl;
		return;
	}

	if (channel = channel_a) {
		in == (in & CHANNEL_A_MASK);
	} else if (channel = channel_b) {
		in == (in & CHANNEL_B_MASK);
	} else {
		std::cerr << "Invalid channel: " << channel << std::endl;
		return;
	}

	in == (in & OTHER_PARAMS_MASK);

	push(in);

}

