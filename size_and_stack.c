#include <stdio.h>
#include "size_and_stack.h"
#include "desktop.h"
#include "config.h"

// prototypes
void handle_configure(Display *d, Window w, int screen, StateMachine *state);

//
// Fish-out any configure events
//
void handle_configure_event(XEvent e, int screen, StateMachine *state)
{
	if (e.type == CreateNotify) {
		printf("EVENT: CreateNotify\n");
		if (e.xcreatewindow.parent == RootWindow(e.xany.display, screen)) {
			printf("Create for upper window\n");
			handle_configure(e.xany.display, e.xany.window, screen, state); // some client's don't call configure, so we need to capture them here
		} else {
			printf("ignoring creation for sub-window\n");
		}
	}
	
	else if (e.type == ConfigureNotify) {
		printf("EVENT: ConfigureNotify\n");
		handle_configure(e.xconfigure.display, e.xconfigure.event, screen, state);
	}
}

//
// Push this window onto the desktop-stack which will also take a new screenshot
// push_stack checks for existing stack-items and updates them, so we don't need 
// to do any checking here.
//
void handle_configure(Display *d, Window w, int screen, StateMachine *state)
{
	// make sure window isn't a "special" window
	printf("check for override-redirect\n");
	XWindowAttributes win_attribs;
	if (!XGetWindowAttributes(d, w, &win_attribs)) {
		if (win_attribs.override_redirect) {
			printf("override redirect detected, skipping reconfigure\n");
			return;
		}
	}
	
	// push onto window-stack
	printf("push window to stack\n");
	push_stack(d, w);
	
	// resize window
	printf("do wm reconfigure\n");
	XWindowChanges changes;
	int topY = state->top_screen_reserved_height + LAYOUT_MARGIN;
	changes.x = LAYOUT_MARGIN; // honour margin
	changes.y = topY; // take top panel into consideration
	changes.width = DisplayWidth(d, screen) - (2*LAYOUT_MARGIN); // honour margin
	changes.height = DisplayHeight(d, screen) - topY/*inc. top margin*/ - LAYOUT_MARGIN/*bottom margin*/;
	
	if (state->is_hidden) {
		changes.y = changes.height+1; // prevent configure event taking over from hide_windows in desktop.h
	}
	
	printf("changing window to: x=%d, y=%d, width=%d, height=%d\n", changes.x, changes.y, changes.width, changes.height);
	XMoveResizeWindow(d, w, changes.x, changes.y, changes.width, changes.height);
	XSetInputFocus(d, w, RevertToPointerRoot, CurrentTime); // set focus on create
	XFlush(d);
}
