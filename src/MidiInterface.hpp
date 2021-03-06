#ifndef __INCLUDE_MIDIINTERFACE_H_
#define __INCLUDE_MIDIINTERFACE_H_

#include <gpiod.h>
#include <chrono>
#include <vector>
#include "RtMidi/RtMidi.h"
#include "mcp4922.hpp"
#include "UniqueList.hpp"

#define NOTE_OFF 0
#define NOTE_OFF_CMD 0x80

#define NOTE_ON 1
#define NOTE_ON_CMD 0x90

#define DEFAULT_MIDI_DEVICE 0

#define GATE_ON true
#define GATE_OFF false

#define HIGH_MASK 0xf0
#define LOW_MASK 0x0f

using namespace std;
using namespace std::chrono;

void midi_event_callback(double timestamp, std::vector<unsigned char> *message, void *userData);

class MidiInterface
{
    public:
        MidiInterface(gpiod_chip *chip);
        virtual ~MidiInterface();

        void parse(std::vector<unsigned char> *message);

        void set_gate(bool);
        void trigger();
        void set_velo(int velo);
        void note_on(int note, int velo);
        void note_off(int);
        void loop();

        void switch_device(int new_device);

        int getDeviceCount();
        int getCurrentDeviceIndex();
        string getCurrentDeviceName();
        vector<string> getDeviceNames();

    private:
        static constexpr duration pulse_duration = milliseconds(5);

        int device_index;
        Mcp4922 *dac;
        gpiod_line *trigger_line, *gate_line;
        high_resolution_clock::time_point last_trigger;
        RtMidiIn *midi;
        bool trigger_active;
        UniqueList *activeNotes;

        void set_note(int);

        /** Loads the midi port last used (if configured & available), the default port otherwise */
        void initializeMidiPort();

};

#endif // __INCLUDE_MIDIINTERFACE_H_
