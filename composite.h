#include <X11/Xlib.h>
#include <X11/extensions/Xcomposite.h>

#ifndef COMPOSITE_H
#define COMPOSITE_H

Bool init_composite_extension(Display *d, Window win, int screen);
void snapshot_window(Display *d, Window win);
void set_opacity(Display *d, Window win, double opacity);

#endif
