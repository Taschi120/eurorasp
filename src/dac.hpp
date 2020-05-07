#ifndef __INCLUDE_DAC_HPP_
#define __INCLUDE_DAC_HPP_

#include "reg.hpp"

/**
 * Implementing a MCP4922 dual-channel Digital-Analog Converter
 */
class DualDac : ShiftRegister {
	public:
		DualDac(int data_pin, int latch_pin, 
				int clock_pin, int cs_pin);
		~DualDac();
		/**
		 * Sends 2 byte with preset config bytes
		 */
		virtual void push(unsigned short in);
		/**
		 * Sends 2 bytes, using the lower 12 bits as payload,
		 * and configuring the highter 4 bits (config bits)
		 * as per method parameters
		 */
		void push(unsigned short in, int channel);
		
		/**
		 * NOT SUPPORTED by this chip
		 */
		virtual void push(unsigned char in);
		
		const int channel_a = 0;
		const int channel_b = 1;

	protected:
		/** Chip Select output */
		struct gpiod_line* cs_ln;
		void cs_on();
		void cs_off();

};

#endif
