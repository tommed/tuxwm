#include <stdio.h>
#include <cairo.h>
#include <cairo-xlib.h>
#include "root_window.h"
#include "config.h"
#include "fonts.h"

static FontFace font;
static Bool is_init = False;

//
// handle events
//
Bool root_window_handle_events(Display *d, XEvent *e, int screen, StateMachine *state)
{
	if (e->xany.window == RootWindow(d, screen))
	{
		if (e->type == Expose) 
		{
			printf("EVENT: root window expose\n");
			root_window_redraw(&e->xexpose, d, screen, state);
			return True;
		}	
	}
	return False;
}

//
// redraw
//
void root_window_redraw(XExposeEvent *e, Display *d, int screen, StateMachine *state)
{
	// NOTE: if you wish t use `e` you MUST check if it is not NULL first
	
	if (!is_init) 
	{
		// init freetype font the first time
		init_font(&font, ROOT_WINDOW_FONT_PATH);
		is_init = True;
	}
	
	// required properties and structs for redraw
	Window root = RootWindow(d, screen);
	int width = DisplayWidth(d, screen);
	int height = DisplayHeight(d, screen);
	Visual *visual = DefaultVisual(d, screen);
	
	// create surface
	cairo_surface_t *surface=cairo_xlib_surface_create(d, root, visual, width, height);
	cairo_t *cr = cairo_create(surface);
	
	// clear surface with a solid colour
	cairo_set_source_rgb(cr, ROOT_WINDOW_BG_RGB);
	cairo_rectangle(cr, 0, 0, width, height);
	cairo_fill(cr);
	
	// draw product name and version on desktop
	cairo_set_source_rgb(cr, ROOT_WINDOW_FG_RGB);
	cairo_set_font_face(cr, font.cairo_font);
  cairo_set_font_size(cr, ROOT_WINDOW_FONT_SIZE);
  cairo_move_to(cr, LAYOUT_MARGIN, height-(2*LAYOUT_MARGIN));
  cairo_show_text(cr, PRODUCT_NAME_AND_VERSION);
	
	cairo_destroy(cr);
	cairo_surface_destroy(surface);
}

void root_window_free()
{
	if (is_init)
	{
		free_font(font); // free freetype font used
	}
}
