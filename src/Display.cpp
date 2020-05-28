#ifndef DISABLE_DISPLAY
#include "Display.hpp"
#include "OLED_Driver.h"
#include "OLED_GFX.h"
#include "DEV_Config.h"
#endif

#include "global.hpp"

#include <string>
#include "stdlib.h"

#define HEIGHT 128
#define WIDTH 128
#define LINE_HEIGHT 20

#define BACKGROUND 0x0000
#define TEXT 0xffff
#define TEXT_OK 0x07E0
#define TEXT_ERR 0xF800

#define MAX_CHARS 16

using namespace std;

const string Display::notes[] =
    #ifdef GERMAN_NOTES
        {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "H"};
    #else
        {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
    #endif // GERMAN_NOTES

Display::Display()
{
#ifndef DISABLE_DISPLAY
    DEV_ModuleInit();
    Device_Init();
#endif
}

Display::~Display()
{
    //dtor
}

uint8_t* str2c(string in) {
#ifndef DISABLE_DISPLAY
    uint8_t* out = (uint8_t*) malloc(in.length() + 1);
    for (int i = 0; i < in.length(); i++) {
        if (in[i] == '[') {
            out[i] = 0x5b;
        } else if (in[i] == ']') {
            out[i] = 0x5d;
        } else {
            out[i] = in[i];
        }
    }
    out[in.length()] = 0;
    return out;
#else
    return 0;
#endif
}

void Display::drawDefaultImage()
{
#ifndef DISABLE_DISPLAY
    cout << "drawing default image" << endl;
    Set_Color(BACKGROUND);
    Fill_Rect(0, 0, HEIGHT - 1, WIDTH - 1);

    drawMidiStatus();
    drawStatus();
    drawNote();
#endif
}

void Display::drawErrorScreen()
{
#ifndef DISABLE_DISPLAY
    Set_Color(BACKGROUND);
    Fill_Rect(0, 0, HEIGHT - 1, WIDTH - 1);

    Set_Color(TEXT_ERR);
    print_String(0, 2, str2c("Invalid Input!"), FONT_8X16);
    Set_Color(TEXT);
    print_String(0, LINE_HEIGHT + 2,  str2c("Press ENTER to"), FONT_5X8);
    print_String(0, LINE_HEIGHT + 12, str2c("   continue"), FONT_5X8);

#endif // DISABLE_DISPLAY
}

void Display::drawMidiDeviceSelectionScreen()
{
#ifndef DISABLE_DISPLAY
    Set_Color(BACKGROUND);
    Fill_Rect(0, 0, HEIGHT - 1, WIDTH - 1);

    Set_Color(TEXT);
    print_String(0, 2, str2c("Enter MIDI dev #"), FONT_8X16);
    print_String(0, HEIGHT - LINE_HEIGHT + 2, str2c(global::input->getBuffer() + "_"), FONT_8X16);
#endif // DISABLE_DISPLAY
}


void Display::drawMidiStatus() {
#ifndef DISABLE_DISPLAY
    Set_Color(TEXT);

    string line1 = "[A] MIDI Device";

    print_String(0, 2, str2c(line1), FONT_8X16);

    line1 = "";
    line1.append(to_string(global::midi->getCurrentDeviceIndex() + 1));
    line1.append("/");
    line1.append(to_string(global::midi->getDeviceCount()));
    print_String(16, 2 + LINE_HEIGHT, str2c(line1), FONT_8X16);

    line1 = global::midi->getCurrentDeviceName();
    if (line1.length() > MAX_CHARS) {
        line1 = line1.substr(0, MAX_CHARS - 1);
    }
    print_String(0, 2 + 2*LINE_HEIGHT, str2c(line1), FONT_8X16);
#endif
}

void Display::setStatus(int status) {
#ifndef DISABLE_DISPLAY
    this->status = status;
    drawStatus();
#endif
}

void Display::drawStatus() {
#ifndef DISABLE_DISPLAY
    Set_Color(BACKGROUND);
    Fill_Rect(WIDTH - 40, HEIGHT - LINE_HEIGHT, 40, LINE_HEIGHT);

    if (status == STATUS_OK) {
        Set_Color(TEXT_OK);
        print_String(WIDTH - 16, HEIGHT - LINE_HEIGHT + 2, str2c("OK"), FONT_8X16);
    } else {
        Set_Color(TEXT_OK);
        print_String(WIDTH - 40, HEIGHT - LINE_HEIGHT + 2, str2c("ERROR"), FONT_8X16);
    }
#endif
}

void Display::setMidiNote(int note)
{
#ifndef DISABLE_DISPLAY
    this->midi_note = note;
    drawNote();
#endif
}

void Display::setMidiNoteOff()
{
#ifndef DISABLE_DISPLAY
    this->midi_note = NO_MIDI_NOTE;
    drawNote();
#endif
}


void Display::drawNote() {
#ifndef DISABLE_DISPLAY
    Set_Color(BACKGROUND);
    Fill_Rect(0, HEIGHT - LINE_HEIGHT, 16, LINE_HEIGHT);
    Set_Color(TEXT);

    string text;
    if (midi_note >= 0) {
        int idx = midi_note % 12; // MIDI note 0 is a C, our note array starts with a C
        // so we do not have to consider any offset here
        text = notes[idx];
    } else {
        text = "--";
    }
    print_String(0, HEIGHT - LINE_HEIGHT + 2, str2c(text), FONT_8X16);
#endif
}

void Display::shutdown() {
#ifndef DISABLE_DISPLAY
    Set_Color(BACKGROUND);
    Fill_Rect(0, 0, WIDTH, HEIGHT);
    Set_Color(TEXT_ERR);
    print_String(0, 2, str2c("OFF"), FONT_8X16);
#endif
}
