#ifndef __INCLUDE_PINS_HPP_
#define __INCLUDE_PINS_HPP_

// Defines pin associations for various functions

#define PINOUT_GATE 2
#define PINOUT_RETRIGGER 3


// These defines for the four DAC input lines
// are defined in the order of the pin
// numbers on the MCP4922 IC.

#define PINOUT_DAC1_CS 16 // PIN 3 on IC
#define PINOUT_DAC1_CLOCK 26 // PIN 4 on IC
#define PINOUT_DAC1_DATA 20 // PIN 5 on IC
#define PINOUT_DAC1_LATCH 21 // PIN 8 ON IC

#define PINOUT_MOSI 10
#define PINOUT_SCK 11 // SCLK
#define PINOUT_DISPLAY_CS 8 // CE0
#define PINOUT_SCREEN_DC 24 // GPI0
#define PINOUT_SCREEN_RESET 25 // GPIO


#define PIN_KP_C0 4
#define PIN_KP_C1 14
#define PIN_KP_C2 15
#define PIN_KP_C3 18

#define PIN_KP_R0 17
#define PIN_KP_R1 27
#define PIN_KP_R2 22
#define PIN_KP_R3 23

#endif
