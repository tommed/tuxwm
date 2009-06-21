#include <X11/Xlib.h> // main header
#include <X11/keysym.h> // keyboard events (includes keysymdef.h)
#include <X11/extensions/Xcomposite.h> // composite extension
#include <X11/cursorfont.h> // cursors
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "state.h"
#include "keyboard.h"
#include "mouse.h"
#include "composite.h"
#include "desktop.h"
#include "size_and_stack.h"
#include "panel.h"
#include "root_window.h"

#define ROOT_EVENT_MASK KeyPressMask|KeyReleaseMask|StructureNotifyMask|SubstructureNotifyMask|EnterWindowMask|LeaveWindowMask|Button1MotionMask|ButtonPress|ButtonRelease
#define WIN_EVENT_MASK KeyPressMask|KeyReleaseMask|StructureNotifyMask|SubstructureNotifyMask|EnterWindowMask|LeaveWindowMask

// prototype
static Window focused_win; // currently focused window
static Bool app_is_done = False; // when True, application should exit before next X-Event
static StateMachine state;

// handle errors
int handle_xerror(Display *dpy, XErrorEvent *ee) {
	int i=0, alen = sizeof(ee) / sizeof(XErrorEvent);
	for (i=0; i<alen; i++) {
		fprintf(stderr, "An error occurred: %d\n", ee[i].error_code);
	}
	return 0;
}

// program entry point
int main() 
{
	printf("opening display\n");
	Display *d = XOpenDisplay(NULL);
	
	printf("grabbing default screen\n");
	int screen = DefaultScreen(d);
	printf("default screen = %d\n", screen);
	
	printf("grabbing root window\n");
	Window root = DefaultRootWindow(d);
	push_stack(d, root); // add desktop window to stack
	
	printf("check that composite is configured\n");
	init_composite_extension(d, root, screen); // composite.h
	
	// this is where we will store our window references
	printf("init desktop\n");
	stack_init(d, screen); //desktop.h
	
	// create cursors
	state.cursors[CursorDefault] = XCreateFontCursor(d, XC_left_ptr);
	XDefineCursor(d, root, state.cursors[CursorDefault]);
	
	// tell root window you'll be capturing it's events
	XSetWindowAttributes a;
	a.event_mask = ExposureMask|KeyPressMask|KeyReleaseMask|StructureNotifyMask|SubstructureNotifyMask;
	XSelectInput(d, root, a.event_mask);
	
	// use a different event mask for child windows
	a.event_mask = KeyPressMask|KeyReleaseMask|StructureNotifyMask|SubstructureNotifyMask;
	
	// handle errors
	XSetErrorHandler(handle_xerror);
	
	// create panel
	panel_create(d, screen, &state);
	
	// draw root window
	root_window_redraw(NULL, d, screen, &state);

	// capture those events
	XEvent e;
	
	// main event loop
	while (!app_is_done) {
		XNextEvent(d, &e);
		
		if (panel_handle_events(d, &e, screen, &state)) {
			continue; // this event has been handled
		}
		
		else if (root_window_handle_events(d, &e, screen, &state)) {
			continue; // this event has been handled
		}
		
		else if (e.type == CreateNotify) {
			printf("Event: create\n");
			XSelectInput(e.xcreatewindow.display, e.xcreatewindow.window, a.event_mask);
		}
			
		else if (e.type == EnterNotify) {
			printf("EVENT: focus-in\n");
			focused_win = e.xcrossing.window;
			//set_opacity(e.xcrossing.display, e.xcrossing.window, 1.0);
		}
		
		else if (e.type == LeaveNotify) {
			printf("EVENT: focus-out\n");
			//set_opacity(e.xcrossing.display, e.xcrossing.window, 0.3); // make window dim if not focused
		}
		
		// handle other events
		app_is_done = handle_keyevent(e, focused_win, screen, &state); /* keyboard.h */
		handle_mouseevent(e, focused_win, &state); /* mouse.h */
		handle_configure_event(e, screen, &state);
	}
	
	// free cursors
	int i;
	for (i=0; i<CursorLast; i++) {
		XFreeCursor(d, state.cursors[i]);
	}
	root_window_free();
	panel_free();
	stack_free();
	XCloseDisplay(d); // shutdown xlib gracefully
	return 0;
}
