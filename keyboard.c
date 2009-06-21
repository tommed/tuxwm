#include <stdlib.h>
#include <stdio.h>
#include "keyboard.h"
#include "config.h"
#include "composite.h"
#include "desktop.h"

// prototypes
Bool handle_release_event(XKeyEvent event, Window active_window, int screen, StateMachine *state);
Bool handle_press_event(XKeyEvent event, Window active_window, int screen, StateMachine *state);
void exec_launcher();
void kill_window(Display *d, Window win);
void next_window(Display *d, Window win, int screen);
void hide_all_windows(Display *d, int screen, StateMachine *state);

//
// Handle keyevents if present in XEvent
// @returns: True if app should quit
//
Bool handle_keyevent(XEvent event, Window active_window, int screen, StateMachine *state)
{

	if (event.type == KeyPress) {
		printf("EVENT: key-press[%d]\n", event.xkey.keycode);
		return handle_press_event(event.xkey, active_window, screen, state);
	}
	
	else if (event.type == KeyRelease) {
			printf("EVENT: key-release[%d]\n", event.xkey.keycode);
			return handle_release_event(event.xkey, active_window, screen, state);
	}	
	
	return False;
}

Bool handle_press_event(XKeyEvent event, Window active_window, int screen, StateMachine *state)
{	
	if (event.state & Mod1Mask || event.state == Mod2Mask) {
		printf("mod pressed\n");
		state->mod_down = True;
	}
	
	return False; // never quit app on press event
}

//
// Handle a keypress on any window (inc. Root window)
//
Bool handle_release_event(XKeyEvent event, Window active_window, int screen, StateMachine *state)
{
	Bool should_quit = False;
	XComposeStatus status;
	KeySym keysym;
	char keybuffer[2];
	int length;
	
	// translate the vendor-dependent keycode to a keysym and string
	length = XLookupString(&event, keybuffer, 1, &keysym, &status);
	
	// check for release of mod key
	printf("keysym=%x\n", (int)keysym);
	//if ( !(event.state & Mod1Mask || event.state & Mod2Mask) ) {
	//	printf("mod released\n");
	//	state->mod_down = False;
	//}
	
	if (keysym == XK_Hyper_L) {
		printf("hyper_l released\n");
	}
	
	if (keysym == XK_Meta_L) {
		printf("meta_l released\n");
	}
	
	if (keysym == XK_Super_L) {
		printf("super released\n");
	}
	
	// actions-keys are only action-keys when the Mod key is depressed
	if (event.state & Mod1Mask || event.state & Mod2Mask) {
		switch (keysym) {
			case ACTIONKEY_QUIT:
				should_quit = True;
				break;
			case ACTIONKEY_LAUNCHER:
				exec_launcher();
				break;				
			case ACTIONKEY_SNAPSHOT:
				snapshot_window(event.display, active_window);
				break;
			case ACTIONKEY_KILL_WINDOW:
				kill_window(event.display, active_window);
				break;
			case ACTIONKEY_NEXT_WINDOW:
				next_window(event.display, active_window, screen);
				break;
			case ACTIONKEY_HIDE_ALL_WINDOWS:
				hide_all_windows(event.display, screen, state);
				break;
		}
	}
	
	return should_quit;
}

//
// Launch the launcher
//
void exec_launcher()
{
	popen(LAUNCHER_CMD, "r");
}

//
// Kill a window
//
void kill_window(Display *d, Window win)
{
	int result = XDestroyWindow(d, win);
	if (result == BadWindow) {
		fprintf(stderr, "Could not kill window as it no longer exists\n");
	}
}

//
// Reshuffles windows (like Alt+Tab on most window managers)
//
void next_window(Display *d, Window win, int screen)
{
	printf("EVENT:KEY:next-window\n");
	stack_shuffle_next_window(d, win, screen); // desktop.h
}

void hide_all_windows(Display *d, int screen, StateMachine *state)
{
	printf("EVENT:KEY:hide-all-windows\n");
	stack_hide_all_windows(d, screen, state); // desktop.h
}
