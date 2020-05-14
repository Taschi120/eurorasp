#ifndef __INCLUDE_GLOBAL_HPP_
#define __INCLUDE_GLOBAL_HPP_

#include "input.hpp"
#include "MidiInterface.hpp"
// yes, i know this is hacky. Sue me.

namespace global {
    extern Input *input;
    extern MidiInterface *midi;
}

#endif // __INCLUDE_GLOBAL_HPP_
