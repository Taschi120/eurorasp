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

#define SIGINT 2 // linux signal: interrupt from keyboard
#define SIGQUIT 3 // linux signal: quit from keyboard
#define SIGSEGV 11 // linux signal: segmentation fault
#define SIGTERM 15 // linux signal: Termination request

using namespace std;
using namespace global;

bool sigterm_received = false;

void segfault(int signal) {
    void *array[10];
    size_t size;
    size = backtrace(array, 10);

    cerr << "SEGFAULT encountered at traceback:" << endl;
    backtrace_symbols_fd(array, size, STDERR_FILENO);

    if(display != 0) {
        display->shutdown();
    }
    exit(1);
}

void shutdown(int signal) {
    sigterm_received = true;
}

int main() {

    if(signal(SIGSEGV, segfault) == SIG_ERR) {
        cerr << "Can't register handler for SIGSEGV" << endl;
    }
    if(signal(SIGINT, shutdown) == SIG_ERR) {
        cerr << "Can't register handler for SIGINT" << endl;
    }
    if(signal(SIGTERM, shutdown) == SIG_ERR) {
        cerr << "Can't register handler for SIGTERM" << endl;
    }
    if(signal(SIGQUIT, shutdown) == SIG_ERR) {
        cerr << "Can't register handler for SIGQUIT" << endl;
    }

	auto* gpiod_chip = gpiod_chip_open_by_number(0);

	global::input = new Input();
    global::midi = new MidiInterface(gpiod_chip);
    global::display = new Display();

    global::display->drawDefaultImage();

	std::cout << "Starting pulse control loop" << std::endl;
	while (!sigterm_received) {
		global::input->loop();
		global::midi->loop();
	}

	cout << "SIGTERM received, quitting";
	display->shutdown();
	return 0;
}

