#include "draw.h"

#include <stdlib.h>

#define IDX(x, y) ((y) * SCREEN_WIDTH + (x))

void draw_rectangle(lcdpixel* fb, Vertex_2D topLeft, Vertex_2D bottomRight, lcdpixel color) {
    for (uint16_t y=topLeft.y; y < bottomRight.y; y++) {
        for (uint16_t x=topLeft.x; x < bottomRight.x; x++) {
            if (x < SCREEN_WIDTH && y < SCREEN_HEIGHT) fb[IDX(x, y)].raw = color.raw;
        }
    }
}

void draw_line(lcdpixel* fb, Vertex_2D start, Vertex_2D end, lcdpixel color) {
    //Implementation of the Bresenham's line algorithm
    int x0 = (int) start.x;
    int y0 = (int) start.y;
    int x1 = (int) end.x;
    int y1 = (int) end.y;

    int dx = abs(x1 - x0);
    int sx = (x0 < x1) ? 1 : -1;

    int dy = -abs(y1 - y0);
    int sy = (y0 < y1) ? 1 : -1;

    int err = dx + dy;
    int e2;
    while (1) {
        if (x0<SCREEN_WIDTH && y0<SCREEN_HEIGHT) fb[IDX(x0, y0)].raw = color.raw;

        if (x0 == x1 && y0 == y1) break;

        e2 = err*2;

        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }

        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void draw_ellipse(lcdpixel* fb, Vertex_2D center, int a, int b, lcdpixel color) {
    for (int y = center.y-b; y < center.y+b; y++) {
        for (int x = center.x - a; x < center.x + a; x++) {
            if (x < SCREEN_WIDTH && y < SCREEN_HEIGHT && x>=0 && y>=0) {
                float dx = x-center.x;
                float dy = y-center.y;
                float ellipse_eq = (dx*dx)/(a*a) + (dy*dy)/(b*b);
                if (ellipse_eq <= 1.0f) {
                    fb[IDX(x, y)].raw = color.raw;
                }
            }
        }
    }
}

void draw_circle(lcdpixel* fb, Vertex_2D center, int radius, lcdpixel color) {
    int radsqr = radius*radius;
    for (int y = center.y - radius; y < center.y + radius; y++) {
        for (int x = center.x - radius; x < center.x + radius; x++) {
            int dx = x-center.x;
            int dy = y-center.y;
            int circ = dx*dx + dy*dy;
            if (x < SCREEN_WIDTH && y < SCREEN_HEIGHT && x>=0 && y>=0){
                if (circ<radsqr) {
                    fb[IDX(x,y)].raw = color.raw;
                }
            }
        }
    }
}

void draw_text(lcdpixel* buffer, Vertex_2D start, char* text, lcdpixel color)
{
    size_t offx = start.x;
    size_t offy = start.y;

    for (size_t i = 0; text[i]; i++) {
        if (text[i] == '\n') {
            offy += font_rom8x16.height;
            offx = start.x;
            continue;
        }
        for (size_t y = 0; y < font_rom8x16.height; y++) {
            uint16_t line = font_rom8x16.bits[text[i] * font_rom8x16.height + y];
            for (size_t x = 0; x < font_rom8x16.maxwidth; x++) {
                if (line & (1 << (16 - x))) {
                    buffer[(y + offy) * 480 + x + offx] = color;
                }
            }
        }
        offx += font_rom8x16.maxwidth;
    }
}