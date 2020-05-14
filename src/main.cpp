#include <iostream>
#include <vector>
#include <cstdio>
#include <chrono>
#include <thread>

#include "gpiod.h"
#include "RtMidi.h"

#include "mcp4922.hpp"
#include "voct.hpp"
#include "pins.hpp"
#include "global.hpp"
#include "MidiInterface.hpp"

#define GPIO_DEVICE "gpiochip0"

int main() {
	auto* gpiod_chip = gpiod_chip_open_by_number(0);

	global::input = new Input(gpiod_chip);
    global::midi = new MidiInterface(gpiod_chip);

	std::cout << "Starting pulse control loop" << std::endl;
	while (true) {
		global::input->loop();
		global::midi->loop();
	}

	return 0;
}

