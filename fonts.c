#include "fonts.h"

static FT_Library library;

//
//
//
void init_font_library()
{
	FT_Init_FreeType(&library);
}

//
//
//
void free_font_library()
{
	FT_Done_FreeType(library);
}

//
// 
//
unsigned int init_font(FontFace *font, char *path)
{
	unsigned int result = FT_New_Face(library, path, 0, &font->ft_font);
	if (0 == result) {
		font->cairo_font = cairo_ft_font_face_create_for_ft_face(font->ft_font, 0);
	}
	return result;
}

//
//
//
void free_font(FontFace font)
{
	FT_Done_Face(font.ft_font);
}
