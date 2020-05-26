#include "MidiInterface.hpp"

#include "pins.hpp"
#include "voct.hpp"
#include "global.hpp"

MidiInterface::MidiInterface(gpiod_chip *chip)
{
    activeNotes = new UniqueList();
    trigger_active = false;
    last_trigger = high_resolution_clock::now();

    gate_line = gpiod_chip_get_line(chip, PINOUT_GATE);
	trigger_line = gpiod_chip_get_line(chip, PINOUT_RETRIGGER);

	int rc = 0;
	rc |= gpiod_line_request_output(gate_line, "midi2cv", GPIOD_LINE_ACTIVE_STATE_HIGH);
	rc |= gpiod_line_request_output(trigger_line, "midi2cv", GPIOD_LINE_ACTIVE_STATE_HIGH);

	// turn the outputs off for now
	rc |= gpiod_line_set_value(gate_line, 0);
	rc |= gpiod_line_set_value(trigger_line, 0);

	dac = new Mcp4922(PINOUT_DAC1_DATA, PINOUT_DAC1_CLOCK,
			PINOUT_DAC1_LATCH, PINOUT_DAC1_CS);

	// MIDI note 69 == A 4, might be 440Hz depending on octave shift
	dac->send(midi_to_cv(69), Mcp4922::CHANNEL_A);

    midi = new RtMidiIn();

	int num = midi->getPortCount();


	for (int cnt = 0; cnt < num; cnt++) {

		std::cout << "Device #" << cnt << ": " <<
			midi->getPortName(cnt) << std::endl;

	}

	midi->openPort(DEFAULT_MIDI_DEVICE);
	midi->setCallback(midi_event_callback);
}

MidiInterface::~MidiInterface()
{
    //dtor
}

void MidiInterface::set_gate(bool state) {
	int status = gpiod_line_set_value(gate_line, state ? 1:0);

	if (status != 0) {
		std::cerr << "Error when setting io pin " <<  PINOUT_GATE
			<< ": Return Code " << status << std::endl;

	} else {
		std::cout << "Set gate to " << state << std::endl;
	}
}

void MidiInterface::trigger() {
	using namespace std::chrono;
	last_trigger = high_resolution_clock::now();
	gpiod_line_set_value(trigger_line, 1);
	trigger_active = true;
}

void MidiInterface::set_note(int note) {
	unsigned short dac_value_voct = midi_to_cv(note);
	dac->send(dac_value_voct, Mcp4922::CHANNEL_A);
}

void MidiInterface::set_velo(int velo) {
	unsigned short dac_value_velo = velo_to_cv(velo);
	dac->send(dac_value_velo, Mcp4922::CHANNEL_B);
}

void MidiInterface::loop() {
    if (trigger_active) {
		if (last_trigger + pulse_duration <= high_resolution_clock::now()) {
			gpiod_line_set_value(trigger_line, 0);
			trigger_active = false;
		}
	}
}

void MidiInterface::note_on(int note, int velo) {
    set_note(note);
	set_gate(GATE_ON);
	set_velo(velo);
	trigger();

	global::display->setMidiNote(note);
	activeNotes->add(note);
}

void MidiInterface::note_off(int note) {
    activeNotes->rem(note);

    if(activeNotes->isEmpty()) {
        set_gate(GATE_OFF);
        global::display->setMidiNoteOff();
    } else {
        int note = activeNotes->getLast();
        set_note(note);
        trigger();
        // Velo is kept from previous note. There might be
        // a more sensible behavior, but this seems fine to me?

        global::display->setMidiNote(note);
    }
}

void MidiInterface::parse(std::vector<unsigned char> * message) {
    auto status_byte = message->front();
	auto opcode = status_byte & HIGH_MASK;
	auto channel = status_byte & LOW_MASK;

	if (opcode == NOTE_OFF_CMD) {
        if (message->size() != 3) {
            std::cerr << "invalid message format" << std::endl;
            return;
        } else {
            unsigned char note = message->at(1);

            note_off(note);
        }
	}
	else if (opcode == NOTE_ON_CMD) {
		if (message->size() != 3) {
			std::cerr << "invalid message format" << std::endl;
			return;
		} else {
            unsigned char note = message->at(1);
            unsigned char velo = message->at(2);
			note_on(note, velo);
		}
	}
}

void midi_event_callback(double timestamp, std::vector<unsigned char> *message, void *userData) {
    global::midi->parse(message);
}

int MidiInterface::getDeviceCount()
{
    return midi->getPortCount();
}

int MidiInterface::getCurrentDeviceIndex()
{
    return device_index;
}

string MidiInterface::getCurrentDeviceName()
{
    return midi->getPortName();
}
