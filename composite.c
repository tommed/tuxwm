#include <stdio.h>
#include <stdlib.h>
#include <X11/extensions/Xrender.h>
#include <X11/Xatom.h>
#include "composite.h"
#include "desktop.h"

static Bool has_composite_extension;

//
// Check composition
//
Bool init_composite_extension(Display *d, Window root, int screen)
{
	has_composite_extension = False;
	int event_base, error_base;
	if (XCompositeQueryExtension(d, &event_base, &error_base)) {
		int major=0, minor=2; // this is the first version which allowed composite
		XCompositeQueryVersion(d, &major, &minor);
		has_composite_extension = (major >= 0 || minor > 2);
		// begin off-screen composition
		if (has_composite_extension) {
			XCompositeRedirectSubwindows(d, root, CompositeRedirectAutomatic);
		}
	}
	return has_composite_extension;
}

//
// Take a snapshot of the given window
//
void snapshot_window(Display *d, Window win_handle)
{
	printf("attempting to snapshot active window\n");
	
	// can only happen if composite is initialized and configured
	if (!has_composite_extension) {
		fprintf(stderr, "Cannot snapshot window as composite extension is not enabled\n");
		return;
	}
	
	printf("WARNING: snapshot_window does nothing currently\n");
	// add this window to the stack
	//push_stack(d, win_handle);
	//redraw_stack();

	/*
	// draw to root window with xlib
	int screen = DefaultScreen(d);
	Window root = RootWindow(d,screen);
	XGetWindowAttributes(d, root, &root_attribs);
	GC gc = XCreateGC(d, root, 0, NULL);
	XSetForeground(d, gc, WhitePixel(d,0)); // make sure the fg color is white
	XFillRectangle(d, root, gc, 0, 0, root_attribs.width, root_attribs.height); // clear root window with fg color
	XPutImage(d, root, gc, img, 0, 0, 400, 400, width, height); // draw snapshot to root window at an offset
	XDestroyImage(img);
	*/
	
	printf("snapshot taken\n");
}

//
// Set opacity for window
//
void set_opacity(Display *d, Window win, double opacity)
{
	int o = opacity * 0xffffffff; // convert from 0-1 to opacity 
	Atom atom_opacity = XInternAtom(d, "_NET_WM_WINDOW_OPACITY", False);
	XChangeProperty(d, win, atom_opacity, XA_CARDINAL, 32, PropModeReplace, (unsigned char *)&o, 1L);
  XSync(d, False);
}
