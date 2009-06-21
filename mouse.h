#include <X11/Xlib.h>
#include "state.h"

#ifndef MOUSE_H
#define MOUSE_H

void handle_mouseevent(XEvent event, Window active_window, StateMachine *state);

#endif
