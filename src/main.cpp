#include <iostream>
#include <vector>
#include <cstdio>
#include <chrono>
#include <thread>
#include <signal.h>
#include <execinfo.h>
#include <unistd.h>

#include "gpiod.h"
#include "RtMidi.h"

#include "mcp4922.hpp"
#include "voct.hpp"
#include "pins.hpp"
#include "global.hpp"
#include "MidiInterface.hpp"
#include "Display.hpp"

#define GPIO_DEVICE "gpiochip0"

#define SIGSEGV 11 // linux signal

using namespace std;

void on_signal(int signal) {
    if (signal == SIGSEGV) {
        void *array[10];
        size_t size;
        size = backtrace(array, 10);

        cerr << "SEGFAULT encountered at traceback:" << endl;
        backtrace_symbols_fd(array, size, STDERR_FILENO);
        exit(1);
    }
}

int main() {

    signal(SIGSEGV, &on_signal);

	auto* gpiod_chip = gpiod_chip_open_by_number(0);

	global::input = new Input(gpiod_chip);
    global::midi = new MidiInterface(gpiod_chip);
    global::display = new Display();

    global::display->drawDefaultImage();

	std::cout << "Starting pulse control loop" << std::endl;
	while (true) {
		global::input->loop();
		global::midi->loop();
	}

	return 0;
}

