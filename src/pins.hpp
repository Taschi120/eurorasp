#ifndef __INCLUDE_PINS_HPP_
#define __INCLUDE_PINS_HPP_

// Defines pin associations for various functions

#define PINOUT_GATE 21
#define PINOUT_RETRIGGER 20
#define PINOUT_TO_BE_DETERMINED 16

// These defines for the four DAC input lines
// are defined in the order of the pin
// numbers on the MCP4922 IC.

#define PINOUT_DAC1_CS 1 // PIN 3 on IC
#define PINOUT_DAC1_CLOCK 7 // PIN 4 on IC
#define PINOUT_DAC1_DATA 8 // PIN 5 on IC
#define PINOUT_DAC1_LATCH 25 // PIN 8 ON IC

#endif
