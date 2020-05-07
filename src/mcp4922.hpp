#ifndef __INCLUDE_MCP4922_HPP_
#define __INCLUDE_MCP4922_HPP_

#include "gpiod.h"

class Mcp4922 {
	public: 
		Mcp4922(int data_pin, int clock_pin, int latch_pin, int cs_pin);
		~Mcp4922();
		void send(unsigned short payload, unsigned short channel);
		
		static const unsigned short CHANNEL_A = 0x0000;
		static const unsigned short CHANNEL_B = 0x8000;

	private:
		void set_clock(int);
		void set_data(int);
		void set_latch(int);
		void set_cs(int);
		void busywait(std::chrono::nanoseconds);

		static const int HIGH = 1;
		static const int LOW = 0;

		struct gpiod_chip *chip;
		struct gpiod_line *data, *clock, *latch, *cs;

		static const unsigned short MAX_PAYLOAD = 0x0fff;

		static const unsigned short CONTROL_BITS = 0x7000;


};


#endif
