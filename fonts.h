#include <X11/cursorfont.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/freetype.h>
#include <cairo-ft.h>

#ifndef FONTS_H
#define FONTS_H

// font face package
typedef struct font_t FontFace;
struct font_t {
	FT_Face             ft_font;
	cairo_font_face_t  *cairo_font;
};

void init_font_library();
void free_font_library();

unsigned int init_font(FontFace *font, char *path);
void free_font(FontFace font);

#endif
