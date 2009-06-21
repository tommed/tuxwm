#include <X11/Xutil.h>
#include <X11/keysym.h>
#include "state.h"

#ifndef KEYBOARD_H
#define KEYBOARD_H

enum { A_Quit, A_Launcher, A_Last };
typedef int ActionKey;

Bool handle_keyevent(XEvent event, Window active_window, int screen, StateMachine *state); // returns `should quit app?`

#endif
