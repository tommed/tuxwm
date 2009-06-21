#include <X11/Xlib.h>
#include "state.h"

#ifndef SIZE_AND_STACK_H
#define SIZE_AND_STACK_H

void handle_configure_event(XEvent e, int screen, StateMachine *state);

#endif
