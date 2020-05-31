#ifndef __INCLUDE_STATEMACHINE_HPP_
#define __INCLUDE_STATEMACHINE_HPP_

#include <string>
#include <any>
#include <vector>

enum State {
    main_screen,
    midi_device_select,
    error_screen
};

enum Event {
    confirm,
    cancel,
    editMidiDevice
};

typedef State (*TransitionFunction) (std::any);

struct Transition {
    Transition(State from, Event with, TransitionFunction transitionFunction);
    State from;
    Event with;
    TransitionFunction transitionFunction;
};

class StateMachine
{
    public:
        StateMachine();
        virtual ~StateMachine();
        State getState();
        void doTransition(Event event, std::any data);

        /**
         * Signals that some information has changed, which
         * requires a repaint of the screen. This is handled through
         * the State Machine because only it knows which screen
         * should be painted.
         */
        void doRefresh();

        static bool isInputState(State state);
        static bool isConfirmableState(State state);
        static bool isCancelableState(State state);

    protected:

    private:
        State state;
        std::vector<Transition> transitions;
        void enterState(State state);
};

#endif // __INCLUDE_STATEMACHINE_HPP_
