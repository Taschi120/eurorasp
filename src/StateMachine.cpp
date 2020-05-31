#include "StateMachine.hpp"

#include "string"

#include "input.hpp"
#include "global.hpp"
#include "RtMidi/RtMidi.h"

State startEditMidiDevice(std::any data) {
    global::input->clearBuffer();
    return State::midi_device_select;
}

State endEditMidiDevice(std::any data) {
    try {
        int i = std::stoi(std::any_cast<string>(data));
        global::midi->switch_device(i);
        std::cout << "Changed MIDI channel to " << i << std::endl;
    } catch (const std::bad_any_cast& e) {
        return State::error_screen;
    } catch (const std::invalid_argument& e) {
        return State::error_screen;
    } catch (const RtMidiError& e) {
        global::midi->switch_device(0);
        return State::error_screen;
    }
    return State::main_screen;
}

State cancelEditMidiDevice(std::any data) {
    return State::main_screen;
}

State exitEventScreen(std::any data) {
    return State::main_screen;
}

Transition::Transition(State from, Event with, TransitionFunction transitionFunction)
{
    this->from = from;
    this->with = with;
    this->transitionFunction = transitionFunction;
}

StateMachine::StateMachine()
{
    transitions.push_back(Transition(State::main_screen, Event::editMidiDevice, &startEditMidiDevice));

    transitions.push_back(Transition(State::midi_device_select, Event::cancel,  &cancelEditMidiDevice));
    transitions.push_back(Transition(State::midi_device_select, Event::confirm, &endEditMidiDevice));

    transitions.push_back(Transition(State::error_screen, Event::confirm, &exitEventScreen));

    state = State::main_screen;
}

StateMachine::~StateMachine()
{
    //dtor
}

State StateMachine::getState()
{
    return this->state;
}

void StateMachine::doTransition(Event event, std::any data)
{
    for (int i = 0; i < transitions.size(); i++) {
        Transition* t = &transitions[i];
        if (t->from == state && t->with == event) {
            State newState = (t->transitionFunction)(data);
            enterState(newState);
            return;
        }
    }
    std::cerr << "No valid transition for event " << event << "in state " << state << std::endl;
}

void StateMachine::enterState(State newState)
{
    this->state = newState;
    doRefresh();
}

void StateMachine::doRefresh()
{
    if (state == main_screen) {
        global::display->drawDefaultImage();
    } else if (state == State::error_screen) {
        global::display->drawErrorScreen();
    } else if (state == State::midi_device_select) {
        global::display->drawMidiDeviceSelectionScreen();
    }

    // TODO implement other states
}


bool StateMachine::isInputState(State state)
{
    switch(state) {
        case State::midi_device_select:

        return true;

        default:
        return false;
    }
}

bool StateMachine::isConfirmableState(State state)
{
    switch(state) {
        case State::midi_device_select:
        case State::error_screen:
        return true;

        default:
        return false;
    }
}

bool StateMachine::isCancelableState(State state)
{
    switch(state) {
        case State::midi_device_select:
        return true;

        default:
        return false;
    }
}
