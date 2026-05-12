#ifndef APO_SEM_DRAW_H
#define APO_SEM_DRAW_H

#include "../mzapo_lib/mzapo.h"

typedef struct {
    int x, y;
} Vertex_2D;

typedef struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    const unsigned char* pixel_data;
    size_t pixel_data_length;
} gimpsprite;

#define GIMP_SPRITE(name, sprite_width, sprite_height, sprite_bpp, sprite_pixels)                                      \
    static const gimpsprite name = {(sprite_width), (sprite_height), (sprite_bpp),                                     \
                                    (const unsigned char*)(sprite_pixels), sizeof(sprite_pixels)}

typedef struct {
    const char* name;
    uint16_t width;
    uint16_t height;
    const uint16_t* data;
} Sprite;

// Basic 2D shapes
void draw_rectangle(lcdpixel* fb, Vertex_2D topLeft, Vertex_2D bottomRight, lcdpixel color);
void draw_line(lcdpixel* fb, Vertex_2D start, Vertex_2D end, lcdpixel color);
void draw_ellipse(lcdpixel* fb, Vertex_2D center, int a, int b, lcdpixel color);
void draw_circle(lcdpixel* fb, Vertex_2D center, float radius, lcdpixel color);

void draw_sprite(lcdpixel* fb, Vertex_2D position, const Sprite* sprite);
void draw_sprite_centered(lcdpixel* fb, Vertex_2D center, const Sprite* sprite);

void clear_display(lcdpixel* fb);

#endif
