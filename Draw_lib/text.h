#ifndef APO_SEM_TEXT_H
#define APO_SEM_TEXT_H

#include "draw.h"
#include "font_types.h"

enum {
    FONT_ROM8x16=0, FONT_PROP14x16
};

void draw_text(lcdpixel* fb, Vertex_2D start, char* text, lcdpixel color, FontId font);

#endif //APO_SEM_TEXT_H
