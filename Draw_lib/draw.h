#ifndef APO_SEM_DRAW_H
#define APO_SEM_DRAW_H

#include "../mzapo_lib/mzapo.h"

typedef struct {
    uint16_t x, y;
} Vertex_2D;

//Basic 2D shapes
void draw_rectangle(lcdpixel* fb, Vertex_2D topLeft, Vertex_2D bottomRight, lcdpixel color);
void draw_line(lcdpixel* fb, Vertex_2D start, Vertex_2D end, lcdpixel color);
void draw_ellipse(lcdpixel* fb, Vertex_2D center, Vertex_2D a, Vertex_2D b, lcdpixel color);
void draw_circle(lcdpixel* fb, Vertex_2D center, Vertex_2D radius, lcdpixel color);

//print Text onto screen
void draw_text(lcdpixel* fb, Vertex_2D start, char* text, lcdpixel color);

#endif
