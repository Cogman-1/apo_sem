#ifndef APO_SEM_TEXT_H
#define APO_SEM_TEXT_H

#include "draw.h"

enum {
    FONT_ROM8x16=0, FONT_PROP14x16
}font_id;

void draw_text(lcdpixel* fb, Vertex_2D start, char* text, lcdpixel color, int font);

#endif //APO_SEM_TEXT_H
