# About

This program is intended for those wishing to use a Raspberry Pi as a MIDI input for a eurorack synthesizer (or similar formats). My motivation for using a Raspberry, rather than something Atmel-based (Arduino or similar), is that a) i want support for USB devices and b) i want to have some features like a trigger, gate, note speed and some more nice things like that, which exceed the usual ATtiny85-based designs floating around the internet.

My goal is for this repo to include the software, along with schematics for the required circuitry and config files for setting up Raspbian properly for embedded use.

Note that this software is not currently finished or fully functional.

# Building

Make sure to install the following tools and libraries on your Raspberry:

* gcc and g++
* (GNU) make
* screen
* libgpiod (including dev packages)
* alsa
* asound (including dev packages)
* pthread (including dev packages)
* boost (including dev packages)
* bcm2835 (from https://www.airspayce.com/mikem/bcm2835, not available as prebuilt package, as far as I know. Can be skipped if you pass the parameter "DISABLE_DISPLAY=true"
to make)

Alsa, asound and pthread can be replaced with Jack but I have not personally tested that. Those dependencies are required by RtMidi and further info can be
found on RtMidi's website at https://www.music.mcgill.ca/~gary/rtmidi/.

Obtain a copy of the source code (via git clone or download), navigate into the project root folder and run "make",
followed by "sudo make install" if you want to install the software into /usr/local and install a systemd unit
for automatic startup / restart of the program.

Please note that the software will require root privileges to run, unless built with the "DISABLE_DISPLAY=true" flag. This is due to low-level access to the system's SPI BUS.

# License

    Eurorasp, a MIDI-to-CV converter for the Raspberry Pi
    Copyright (C) 2020, S. Hillebrand

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

# Third-Party Acknowledgements

## This project uses libgpiod, which is licensed under the GNU LGPL 2.1.

Copyright (C) 2017-2019 Bartosz Golaszewski <bartekgola@gmail.com>

https://github.com/brgl/libgpiod

## This project uses RtMidi.

RtMidi: realtime MIDI i/o C++ classes

Copyright (c) 2003-2019 Gary P. Scavone

https://www.music.mcgill.ca/~gary/rtmidi/

Some example code for the Waveshare 1.5inch RGB OLED module is also used.

## This project uses libbcm2835.

https://www.airspayce.com/mikem/bcm2835/

libbcm2835 is licensed under the GNU GPL 3.
