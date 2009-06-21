#include "desktop.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <string.h>
#include <stdlib.h> // alloc/free
#include <stdio.h> // sprintf

static WindowStack *stack = NULL;
//static ClutterStage *stage;

//
// Init the stack
//
Bool stack_init(Display *d, int screen)
{
	return True;
}

//
// Add a new window to the stack
//
WindowStack *push_stack(Display *d, Window window)
{
/*
	WindowStack *sitem = NULL;
	// see if this window is already in the stack
	printf("see if window is already in stack\n");
	WindowStack *iter = stack;
	while (iter != NULL) {
		if (iter->window == window) { // we can do this as windows are just unsigned longs
			break;
		}
		iter = iter->next;
	}
	if (iter != NULL) { // if we haven't reached NULL, the break was called above and we have an existing window!
		
		// update existing stack item
		printf("update existing item\n");
		sitem = iter;
		
	} else {
		
		// create a new stack item
		printf("new window to add to stack\n");
		sitem = (WindowStack *)malloc(sizeof(WindowStack));
		sitem->window = window;
		sitem->display = d;
		if (stack != NULL) {
			sitem->next = stack; // add to stack
		}
		stack = sitem; // add item to top of stack	
	}
	
	if (sitem == NULL) {
		printf("sitem was NULL when unexpected\n");
		return stack;
	}
	
	// get/update screenshot
	printf("get window properties\n");
	XWindowAttributes a;
	if (XGetWindowAttributes(d, window, &a)) {
		
		// release old one if present
		//printf("release old screenshot if present\n");
		//if (sitem->screenshot != NULL) XDestroyImage(sitem->screenshot);
		
		// create new one
		//printf("create new screenshot\n");
		//sitem->screenshot = XGetImage(d, window, 0, 0, a.width, a.height, AllPlanes, ZPixmap);
		
	}
	
	// get window title
	printf("get window title\n");
	XTextProperty prop_title;
	int i=0;
	if ( !XGetWMName(d, window, &prop_title) ) {
		printf("title retrieved from Xlib\n");
		sitem->title = (char *)prop_title.value;
	} else {
		printf("titled created from scratch\n");
		char *title = (char *)malloc(sizeof(char) * 11); // upto 99 unnamed windows
		sprintf(title, "Untitled%d", ++i);
		sitem->title = title;
	}
*/	
	return stack; 
}

//
// Look at the top window in the stack
//
WindowStack *peek_stack()
{
	return stack;
}

//
// Get a windowstack by it's title
//
WindowStack *get_window_with_title(char *title)
{
	WindowStack *sitem = stack;
	while (sitem != NULL) {
		if (0 == strcmp(sitem->title, title)) {
			return sitem;
		}
		sitem = sitem->next;
	}
	return NULL;
}

//
// Free all screenshots and windowstacks
// 
void stack_free()
{
/*
	while (1) {
		WindowStack *sitem = stack;
		if (sitem == NULL) break; // nothing else to free
		//XDestroyImage(sitem->screenshot);
		stack = sitem->next;
		free(sitem);
	}
*/
}

//
// Shuffle windows (just like Alt+Tab in desktop managers)
//
void stack_shuffle_next_window(Display *d, Window win, int screen)
{
	printf("shuffling windows\n");
	XCirculateSubwindowsUp(d, RootWindow(d, screen));
	XFlush(d);
}

//
// Hide All Windows
//
void stack_hide_all_windows(Display *d, int screen, StateMachine *state)
{
	printf("Hiding all windows\n");
	Window target = RootWindow(d, screen);
	Window root, parent;
	Window *child;
	unsigned int i, count;
	unsigned int bottom_screen = (DisplayHeight(d, screen));
	if (XQueryTree(d, target, &root, &parent, &child, &count))
	{
		printf("found %d windows to hide\n", count);
		for (i=0; i<count; i++) {
			if (*child != state->panel_id) { // panel is exempt
				if (state->is_hidden) {
					XMoveWindow(d, *child, 0, state->top_screen_reserved_height); // restore all windows
				} else {
					XMoveWindow(d, *child, 0, bottom_screen+1); // hide all windows off screen
				}
			}
			child++;
		}
		state->is_hidden = !(state->is_hidden); // swap state->is_hidden marker
		XFlush(d);
	} else {
		printf("failed to query window tree\n");
	}
}
