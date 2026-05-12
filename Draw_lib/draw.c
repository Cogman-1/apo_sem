#include "draw.h"

#include <stdlib.h>
#include <string.h>

#define IDX(x, y) ((y)*SCREEN_WIDTH + (x))



void draw_rectangle(lcdpixel* fb, Vertex_2D topLeft, Vertex_2D bottomRight, lcdpixel color)
{
    for (uint16_t y = topLeft.y; y < bottomRight.y; y++) {
        for (uint16_t x = topLeft.x; x < bottomRight.x; x++) {
            if (x < SCREEN_WIDTH && y < SCREEN_HEIGHT)
                fb[IDX(x, y)].raw = color.raw;
        }
    }
}

void draw_line(lcdpixel* fb, Vertex_2D start, Vertex_2D end, lcdpixel color)
{
    // Implementation of the Bresenham's line algorithm
    int x0 = (int)start.x;
    int y0 = (int)start.y;
    int x1 = (int)end.x;
    int y1 = (int)end.y;

    int dx = abs(x1 - x0);
    int sx = (x0 < x1) ? 1 : -1;

    int dy = -abs(y1 - y0);
    int sy = (y0 < y1) ? 1 : -1;

    int err = dx + dy;
    int e2;
    while (1) {
        if (x0 < SCREEN_WIDTH && y0 < SCREEN_HEIGHT)
            fb[IDX(x0, y0)].raw = color.raw;

        if (x0 == x1 && y0 == y1)
            break;

        e2 = err * 2;

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

void draw_ellipse(lcdpixel* fb, Vertex_2D center, int a, int b, lcdpixel color)
{
    for (int y = center.y - b; y < center.y + b; y++) {
        for (int x = center.x - a; x < center.x + a; x++) {
            if (x < SCREEN_WIDTH && y < SCREEN_HEIGHT && x >= 0 && y >= 0) {
                float dx = x - center.x;
                float dy = y - center.y;
                float ellipse_eq = (dx * dx) / (a * a) + (dy * dy) / (b * b);
                if (ellipse_eq <= 1.0f) {
                    fb[IDX(x, y)].raw = color.raw;
                }
            }
        }
    }
}

void draw_circle(lcdpixel* fb, Vertex_2D center, float radius, lcdpixel color)
{
    int radsqr = radius * radius;
    for (int y = center.y - radius; y < center.y + radius; y++) {
        for (int x = center.x - radius; x < center.x + radius; x++) {
            int dx = x - center.x;
            int dy = y - center.y;
            int circ = dx * dx + dy * dy;
            if (x < SCREEN_WIDTH && y < SCREEN_HEIGHT && x >= 0 && y >= 0) {
                if (circ < radsqr) {
                    fb[IDX(x, y)].raw = color.raw;
                }
            }
        }
    }
}

static void draw_sprite_data(lcdpixel* fb, Vertex_2D start, uint16_t width, uint16_t height, const uint16_t* data)
{
    for (uint16_t y = 0; y < height; y++) {
        for (uint16_t x = 0; x < width; x++) {
            uint16_t pixel = data[y * width + x];
            int px = start.x + (int)x;
            int py = start.y + (int)y;
            if (pixel && px >= 0 && py >= 0 && px < SCREEN_WIDTH && py < SCREEN_HEIGHT) {
                fb[IDX(px, py)].raw = pixel;
            }
        }
    }
}

void draw_sprite(lcdpixel* fb, Vertex_2D position, const Sprite* sprite)
{
    if (!sprite)
        return;
    draw_sprite_data(fb, position, sprite->width, sprite->height, sprite->data);
}

void draw_sprite_centered(lcdpixel* fb, Vertex_2D center, const Sprite* sprite)
{
    if (!sprite)
        return;
    Vertex_2D top_left;
    top_left.x = center.x - (int)sprite->width / 2;
    top_left.y = center.y - (int)sprite->height / 2;
    draw_sprite_data(fb, top_left, sprite->width, sprite->height, sprite->data);
}

void clear_display(lcdpixel* fb)
{
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            fb[IDX(x, y)].raw = 0x0000;
        }
    }
}
