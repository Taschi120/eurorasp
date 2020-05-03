#include <iostream>
#include <vector>
#include <cstdio>
#include "gpiod.h"
#include "RtMidi.h"

#define MY_DEVICE 1

void callback(double timestamp, std::vector<unsigned char> *message, void *userData) {
	std::cout << "[" << timestamp << "]\t";
	for(auto &it : *message) {
		std::cout << " ";
		printf("%d", it);
	}	
	std::cout << std::endl;
}

int main() {
	std::cout << "Hello World!" << std::endl;

	int value = gpiod_ctxless_get_value("gpiochip0", 13, false, "me");
	std::cout << "Input Value: " << value << std::endl;

	RtMidiIn *midi = new RtMidiIn();

	int num = midi->getPortCount();
	std::cout << "MIDI-Geräte verfügbar: " << num << std::endl;

	for (int cnt = 0; cnt < num; cnt++) {	
		
		std::cout << "Device #" << cnt << ": " << 
			midi->getPortName(cnt) << std::endl;
		
	}

	midi->openPort(MY_DEVICE);
	midi->setCallback(*callback);

	std::cout << "Press Key to End" << std::endl;

	char* blah;
	std::cin>>blah;

	midi->closePort();

	delete midi;
	return 0;
}

