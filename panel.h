#include <X11/Xlib.h>
#include "state.h"
#include "config.h"

#ifndef PANEL_H
#define PANEL_H

typedef struct dock_panel_t DockPanel;
struct dock_panel_t 
{
	Window window, parent;
	Display *display;
	int screen;
	int width, height;
	Visual *visual;
	unsigned long attribute_mask;
	XSetWindowAttributes attributes;
};

void panel_create(Display *d, int screen, StateMachine *state);
Bool panel_handle_events(Display *d, XEvent *e, int screen, StateMachine *state);
void panel_redraw(Display *d, int screen);
void panel_free();

#endif
