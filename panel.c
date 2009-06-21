#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <cairo.h>
#include <cairo-xlib.h>
#include "panel.h"
#include "config.h" // freetype font path
#include "fonts.h"

static DockPanel panel;
static FontFace font;

//
// Create the panel
//
void panel_create(Display *d, int screen, StateMachine *state)
{
	printf("panel_create\n");
	
	// open font library
	init_font_library();
	init_font(&font, DOCKPANEL_LOGO_FONT_PATH);
	
	// create panel
	panel.parent  = RootWindow(d, screen); // owned by root
	panel.height  = DOCKPANEL_HEIGHT; // config.h
	panel.width   = DisplayWidth(d, screen); // stretch
	panel.visual  = DefaultVisual(d, screen);
	panel.display = d;
	panel.screen  = screen;
	panel.attributes.event_mask = ExposureMask;
	panel.attributes.background_pixel = BlackPixel(d, screen);
	panel.attributes.override_redirect = True;
	panel.attribute_mask = CWEventMask | CWBackPixel | CWOverrideRedirect;
	panel.window  = XCreateWindow(d, panel.parent, LAYOUT_MARGIN, 0, panel.width-(2*LAYOUT_MARGIN), panel.height+LAYOUT_MARGIN/*for shadow*/, 
																0, CopyFromParent, InputOutput, panel.visual, panel.attribute_mask, &panel.attributes);
	
	// reserve part of screen															
	printf("reserving top of screen %d\n", DOCKPANEL_HEIGHT);
	state->top_screen_reserved_height = DOCKPANEL_HEIGHT;
	state->panel_id = panel.window;
	
	// default cursor
	XDefineCursor(d, panel.window, state->cursors[CursorDefault]);
	
	// map to screen			
	printf("drawing panel to screen\n");
	XMapRaised(panel.display, panel.window);
}

//
// Handle events
//
Bool panel_handle_events(Display *d, XEvent *e, int screen, StateMachine *state)
{
	// only handle panel events
	if (e->xany.window == panel.window) // you can do this as Window is an XID, which is an unsigned long :)
	{
		if (e->type == Expose) {
			printf("EVENT: panel exposed\n");
			panel_redraw(d, screen);
			return True;
		}
	}
	return False; // pass event on to main event handle
}

//
// Redraw
//
void panel_redraw(Display *d, int screen)
{
	cairo_pattern_t *pattern; // for defining gradients
	cairo_surface_t *surface=cairo_xlib_surface_create(d, panel.window, panel.visual, panel.width, panel.height + LAYOUT_MARGIN/*for gradient*/);
	cairo_t *cr = cairo_create(surface);
	
	// clear surface with the configured panel bg colour
	const int panel_gradient_offset = 2;
	pattern = cairo_pattern_create_linear(0.0, panel_gradient_offset, 0.0, panel.height);
	cairo_pattern_add_color_stop_rgba(pattern, 0.0, DOCKPANEL_BG_RGB_LIGHT, 1);
	cairo_pattern_add_color_stop_rgba(pattern, 0.1, DOCKPANEL_BG_RGB_MEDIUM, 1);
	cairo_pattern_add_color_stop_rgba(pattern, 0.5, DOCKPANEL_BG_RGB_DARK, 1);
	cairo_set_source(cr, pattern);
	cairo_rectangle(cr, 0, 0, panel.width, panel.height);
	cairo_fill(cr);
	cairo_pattern_destroy(pattern);
	
	// draw shadow
	pattern = cairo_pattern_create_linear(0.0, panel.height, 0.0/*x stop coordinate*/, panel.height+LAYOUT_MARGIN/*specifies Y stop coordinate NOT height*/);
	cairo_pattern_add_color_stop_rgba(pattern, 0/*pos*/, 0, 0, 0, 1);
	cairo_pattern_add_color_stop_rgba(pattern, 1/*pos*/, ROOT_WINDOW_BG_RGB, 1); // TODO: don't fake transparency
	cairo_rectangle(cr, 0, panel.height, panel.width, LAYOUT_MARGIN);
	cairo_set_source(cr, pattern);
	//cairo_set_source_rgb(cr, 0.5, 0.5, 0.5);
	cairo_fill(cr);
	cairo_pattern_destroy(pattern);
	
	// TODO: add draw code here
	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_set_font_face(cr, font.cairo_font);
  cairo_set_font_size(cr, 26);
  cairo_move_to(cr, 10, 30);
  cairo_show_text(cr, DOCKPANEL_LOGO_LETTER);
	
	cairo_destroy(cr);
	cairo_surface_destroy(surface);
}

//
// Free from memory
//
void panel_free()
{
	free_font(font);
	free_font_library();
	XDestroyWindow(panel.display, panel.window);
}
