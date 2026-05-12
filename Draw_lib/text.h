#ifndef APO_SEM_TEXT_H
#define APO_SEM_TEXT_H

#include "draw.h"
#include "font_types.h"

typedef enum { FONT_ROM8x16 = 0, FONT_PROP14x16 } FontId;

void draw_text(lcdpixel* fb, Vertex_2D start, char* text, lcdpixel color, int font);

#endif // APO_SEM_TEXT_H
