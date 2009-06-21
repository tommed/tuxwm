#include <X11/Xlib.h>
#include "state.h"

#ifndef ROOT_WINDOW_H
#define ROOT_WINDOW_H

Bool root_window_handle_events(Display *d, XEvent *e, int screen, StateMachine *state);
void root_window_redraw(XExposeEvent *e, Display *d, int screen, StateMachine *state);
void root_window_free();

#endif
