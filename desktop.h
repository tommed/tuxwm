#include <X11/Xlib.h>
#include "state.h"

#ifndef DESKTOP_H
#define DESKTOP_H

typedef struct winstack_t WindowStack;
struct winstack_t {
	Display *display;
	Window window;
	XImage *screenshot;
	char *title;
	WindowStack *next;
};

Bool stack_init(Display *d, int screen);
WindowStack *push_stack(Display *d, Window window);
WindowStack *peek_stack();
WindowStack *get_window_with_title(char *title);
void stack_shuffle_next_window(Display *d, Window win, int screen);
void redraw_stack();
void stack_free();
void stack_hide_all_windows(Display *d, int screen, StateMachine *state);

#endif
