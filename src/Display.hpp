#ifndef __INCLUDE_DISPLAY_HPP_
#define __INCLUDE_DISPLAY_HPP_

#include <string>

#define STATUS_OK 0
#define STATUS_ERROR 1

#define NO_MIDI_NOTE -1

using namespace std;

class Display
{
    public:
        Display();
        virtual ~Display();

        void drawDefaultImage();

        void setStatus(int status);
        void setMidiNote(int note);
        void setMidiNoteOff();

    protected:

    private:
        void drawMidiStatus();
        void drawStatus();
        void drawNote();

        int status = STATUS_OK;
        int midi_note = NO_MIDI_NOTE;

        static const string notes[];

};

#endif // __INCLUDE_DISPLAY_HPP_
