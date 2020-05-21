#include "voct.hpp"

#define VALUE_PER_NOTE 47.0690
unsigned short midi_to_cv(int midi){
	if (midi < 21) {
		midi = 21;
	} else if (midi > 108) {
		midi = 108;
	}
	float note = midi - 21;
	return (unsigned int) ((float) note * VALUE_PER_NOTE);
}

unsigned short velo_to_cv(unsigned char velo) {
    int val = velo * 16; // TODO: Tweak
    if (val > 4095) {
        return 4095;
    }
    return val;
}
