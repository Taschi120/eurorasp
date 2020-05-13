#include <iostream>
#include <thread>
#include <chrono>

#include "mcp4922.hpp"
#include "gpiod.h"

using namespace std::chrono;

Mcp4922::Mcp4922(int data_pin, int clock_pin, int latch_pin, int cs_pin) {
	chip = gpiod_chip_open_by_number(0);
	data = gpiod_chip_get_line(chip, data_pin);
	clock = gpiod_chip_get_line(chip, clock_pin);
	latch = gpiod_chip_get_line(chip, latch_pin);
	cs = gpiod_chip_get_line(chip, cs_pin);

	gpiod_line_request_output(data, "midi2cv", LOW);
	gpiod_line_request_output(clock, "midi2cv", LOW);
	gpiod_line_request_output(latch, "midi2cv", HIGH);
	gpiod_line_request_output(cs, "midi2cv", HIGH);

}

Mcp4922::~Mcp4922() {
	//TODO
}

void Mcp4922::send(unsigned short payload, unsigned short channel){
	if (payload > MAX_PAYLOAD) {
		std::cerr << "exceeding max payload!" << std::endl;
		return;
	}

	unsigned short message = payload | channel | CONTROL_BITS;

	char hex [16];

	sprintf(hex, "%X", message);
	std::cout << "Sending to DAC: " << hex << std::endl;

	unsigned short bitmask = 0x8000;

	set_clock(LOW);
	busywait(microseconds(25000));
	set_cs(LOW);
	busywait(microseconds(25000));
	set_latch(HIGH);

	busywait(microseconds(250000));	

	for (int i = 0; i < 16; i++) {
		int bit = (message & bitmask) ? HIGH : LOW;
		
		bitmask = bitmask >> 1;
		
		set_data(bit);
		busywait(microseconds(250000)); // setup time
		set_clock(HIGH);
		busywait(microseconds(250000)); // hold time
		set_clock(LOW);
		// wait needed here? i don't think it is
	}

	busywait(microseconds(250000));
	set_cs(HIGH);
	set_latch(LOW);

}

void Mcp4922::set_clock(int val) {
	gpiod_line_set_value(clock, val);
}

void Mcp4922::set_data(int val) {
	gpiod_line_set_value(data, val);
}

void Mcp4922::set_latch(int val) {
	gpiod_line_set_value(latch, val);
}

void Mcp4922::set_cs(int val) {
	gpiod_line_set_value(cs, val);
}

void Mcp4922::busywait(nanoseconds d) {
	time_point end = high_resolution_clock::now() + d;
	while (high_resolution_clock::now() < end) {
		// NOOP
	}
}

