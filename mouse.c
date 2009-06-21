#include <stdio.h>
#include "mouse.h"
#include "keyboard.h"

// prototypes
void handle_buttonpress_event(XButtonEvent e, Window active_window, StateMachine *state);
void handle_buttonrelease_event(XButtonEvent e, Window active_window, StateMachine *state);
void handle_motion_event(XMotionEvent e, Window active_window, StateMachine *state);

//
// Handle mouse events in main event loop
//
void handle_mouseevent(XEvent event, Window active_window, StateMachine *state)
{
	if (event.type == ButtonPress) {
		printf("EVENT: button pressed\n");
		handle_buttonpress_event(event.xbutton, active_window, state);
	}
	
	else if (event.type == ButtonRelease) {
		printf("EVENT: button released\n");
		handle_buttonrelease_event(event.xbutton, active_window, state);
	}
	
	else if (event.type == MotionNotify) {
		handle_motion_event(event.xmotion, active_window, state);
	}
}

void handle_buttonpress_event(XButtonEvent e, Window active_window, StateMachine *state)
{
	if (e.button == Button1) {
		printf("button1 is down\n");
		state->button1_down = True;
	}
}


void handle_buttonrelease_event(XButtonEvent e, Window active_window, StateMachine *state)
{
	if (e.button == Button1) {
		printf("button1 is up\n");
		state->button1_down = False;
		state->is_moving = False;
	}
}


void handle_motion_event(XMotionEvent e, Window active_window, StateMachine *state)
{
	printf("is_mod_down %d\n", state->mod_down);
	printf("is_b1_down %d\n", state->button1_down);
	
	if (/*state->mod_down &&*/ state->button1_down) {
		// handle moving of windows
		int x=e.x_root, y=e.y_root;
		if (!state->is_moving) {
			state->is_moving = True;
			XWindowAttributes winposa;
			XGetWindowAttributes(e.display, e.window, &winposa); // instead use: http://tronche.com/gui/x/xlib/window-information/XGetGeometry.html
			state->move_mousex = e.x_root;
			state->move_mousey = e.y_root;
			state->move_winx = winposa.x; // always returns 0 :(
			state->move_winy = winposa.y; // "---------"
			XRaiseWindow(e.display, active_window);
		}
		
		printf("mouse[x=%d,y=%d], win[x=%d,y=%d]\n", e.x_root, e.y_root, state->move_winx, state->move_winy);
		
		x -= state->move_mousex;
		y -= state->move_mousey;
		printf("moving by x=%d,y=%d\n", x, y);
		
		x += state->move_winx;
		y += state->move_winy;
		printf("new location x=%d,y=%d\n", x, y);
		
		XMoveWindow(e.display, active_window, x, y);
	}
}
