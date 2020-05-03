#include <iostream>
#include <vector>
#include <cstdio>
#include "gpiod.h"
#include "RtMidi.h"
#include <chrono>

#define GPIO_DEVICE "gpiochip0"
#define MY_DEVICE 1
#define PINOUT_GATE 21
#define PINOUT_RETRIGGER 20
#define PINOUT_TO_BE_DETERMINED 16

#define GATE_ON true
#define GATE_OFF false

#define HIGH_MASK 0xf0
#define LOW_MASK 0x0f

#define NOTE_OFF 0
#define NOTE_OFF_CMD 0x80

#define NOTE_ON 1
#define NOTE_ON_CMD 0x90


struct gpiod_line *gate_line = 0;
struct gpiod_line *trigger_line = 0;

std::chrono::time_point last_trigger = std::chrono::high_resolution_clock::now();

bool trigger_active = false;

typedef std::chrono::duration<long,std::nano> nanos;

const nanos pulse_duration (100);

void set_gate(bool state) {
	int status = gpiod_line_set_value(gate_line, state ? 1:0);
	
	if (status != 0) {
		std::cerr << "Error when setting io pin " <<  PINOUT_GATE
			<< ": Return Code " << status << std::endl;

	} else {
		std::cout << "Set gate to " << state << std::endl;
	}
}

void trigger() {
	using namespace std::chrono;
	last_trigger = high_resolution_clock::now();
	gpiod_line_set_value(trigger_line, 1);
	trigger_active = true;
}

void callback(double timestamp, std::vector<unsigned char> *message, void *userData) {
	
	auto status_byte = message->front();
	auto opcode = status_byte & HIGH_MASK;
	auto channel = status_byte & LOW_MASK;

	if (opcode == NOTE_OFF_CMD) {
		if (channel == 0) {
			set_gate(GATE_OFF);
		}
	}
	else if (opcode == NOTE_ON_CMD) {
		if (channel == 0) {
			set_gate(GATE_ON);
		}
		trigger();
	}	



	std::cout << std::endl;
}

int main() {
	auto* gpiod_chip = gpiod_chip_open_by_number(0);
	gate_line = gpiod_chip_get_line(gpiod_chip, PINOUT_GATE);
	trigger_line = gpiod_chip_get_line(gpiod_chip, PINOUT_RETRIGGER);

	int rc = 0;
	rc |= gpiod_line_request_output(gate_line, "midi2cv", GPIOD_LINE_ACTIVE_STATE_HIGH);
	rc |= gpiod_line_request_output(trigger_line, "midi2cv", GPIOD_LINE_ACTIVE_STATE_HIGH);

	// turn the outputs off for now
	rc |= gpiod_line_set_value(gate_line, 0);
	rc |= gpiod_line_set_value(trigger_line, 0);

	std::cout << "GPIOD setup " << (rc ? "failed" : "ok") << std::endl;
	

	RtMidiIn *midi = new RtMidiIn();

	int num = midi->getPortCount();
	std::cout << "MIDI-Geräte verfügbar: " << num << std::endl;

	for (int cnt = 0; cnt < num; cnt++) {	
		
		std::cout << "Device #" << cnt << ": " << 
			midi->getPortName(cnt) << std::endl;
		
	}

	midi->openPort(MY_DEVICE);
	midi->setCallback(*callback);

	while (true) {
		using namespace std::chrono;
		// do not end until device shutdown because embedded
		if (trigger_active) {
			if (last_trigger + pulse_duration <= high_resolution_clock::now()) {
				gpiod_line_set_value(trigger_line, 0);
				trigger_active = false;
			}
		
		}
	}

	midi->closePort();

	delete midi;
	return 0;
}

