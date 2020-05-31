#include "input.hpp"

#include "global.hpp"
#include <gpiod.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <vector>
#include <cassert>
#include <fstream>
#include <string>
#include <stdio.h>
#include <fcntl.h>
#include <err.h>
#include <any>
#include <libevdev-1.0/libevdev/libevdev.h>

#define VALUE_KEYUP 0x0
#define VALUE_KEYDOWN 0x1

using namespace std;
using namespace std::chrono;
using namespace global;

Input::Input() {
	keyboard_reader = new thread(&Input::keyboard_listen, this);
}

Input::~Input() {

}

char Input::convertScancodeToChar(unsigned int scancode) {
    cout << scancode << endl;
    switch(scancode) {
        case KEY_KP0: return '0';
        case KEY_KP1: return '1';
        case KEY_KP2: return '2';
        case KEY_KP3: return '3';
        case KEY_KP4: return '4';
        case KEY_KP5: return '5';
        case KEY_KP6: return '6';
        case KEY_KP7: return '7';
        case KEY_KP8: return '8';
        case KEY_KP9: return '9';
        case KEY_KPPLUS: return '+';
        case KEY_KPMINUS: return '-';
        case KEY_KPASTERISK: return '*';
        case KEY_KPSLASH: return '/';
        case KEY_BACKSPACE: return 'B';
        case KEY_ESC: return 'e';
        case KEY_ENTER:
        case 96: return 'E';
    }
    return '?';
}

void Input::handle_keypress(char key) {
    std::cout << key << std::endl;
    if (key == 'E' && StateMachine::isConfirmableState(stateMachine->getState())) {
        stateMachine->doTransition(Event::confirm, std::any(buffer));
    } else if (key == 'e' && StateMachine::isCancelableState(stateMachine->getState())) { // escape
        stateMachine->doTransition(Event::cancel, std::any(buffer));
    } else if (StateMachine::isInputState(stateMachine->getState())) {
        if (key == 'B' && buffer.length() != 0) {
            buffer.pop_back();
        } else if (key != '?' && key != 'e' && key != 'E') {
            buffer.push_back(key);
            stateMachine->doRefresh();
        }
    } else if (stateMachine->getState() == State::main_screen) {
        if (key == '1') {
            stateMachine->doTransition(Event::editMidiDevice, std::any(0));
        }
    }
}

string Input::getBuffer()
{
    return std::string(buffer);
}

void Input::clearBuffer()
{
    buffer = "";
}



void Input::keyboard_listen() {
    const char *eventDevice = "/dev/input/event0";

    const int fd = open(eventDevice, O_RDONLY | O_NONBLOCK);

    if (fd < 0) {
        std::cerr << "ERROR: cannot open device" << eventDevice << "[" << errno << "]" << std::endl;
        // TODO handle
    }
    struct libevdev *dev;

    int err = libevdev_new_from_fd(fd, &dev);

    if (err < 0) {
        std::cerr << "ERROR: cannot associate event device [" << errno << "]" << std::endl;
        // TODO handle
    }

    printf("Device %s is open and associated w/ libevent\n", eventDevice);
    do {
        struct input_event ev;

        err = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);

        if (err == 0 && ev.type == EV_KEY & ev.value == VALUE_KEYUP)
        {
            handle_keypress(convertScancodeToChar(ev.code));
        }
    } while (err == 1 || err == 0 || err == -EAGAIN);

}


