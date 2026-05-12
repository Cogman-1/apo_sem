#ifndef APO_SEM_EFFECT_H
#define APO_SEM_EFFECT_H

#include "draw.h"

/* Basic Effects */

// Fade effect: blend entire screen with a color based on time (0-1 is full cycle)
void fade_effect(lcdpixel* fb, float time, lcdpixel color);

// Flash effect: create a pulsing flash at screen center
void flash_border(lcdpixel* fb, float time, float intensity, lcdpixel color);

// Vignette effect: darken edges of screen
void vignette(lcdpixel* fb, float intensity);

// Scanlines: add horizontal lines for CRT effect
void scanlines(lcdpixel* fb, int line_spacing, int line_thickness, uint8_t darkness);

// Pixelate: reduce resolution in a region
void pixelate(lcdpixel* fb, Vertex_2D top_left, Vertex_2D bottom_right, int pixel_size);

// Blur: gaussian-like blur in a region
void blur(lcdpixel* fb, Vertex_2D top_left, Vertex_2D bottom_right, int area);

// Color shift: shift hue of screen region
void color_shift(lcdpixel* fb, Vertex_2D top_left, Vertex_2D bottom_right, int hue_shift);

// Invert colors in region
void invert_region(lcdpixel* fb, Vertex_2D top_left, Vertex_2D bottom_right);

// Darken/Lighten region
void darken_region(lcdpixel* fb, Vertex_2D top_left, Vertex_2D bottom_right, float factor);
void lighten_region(lcdpixel* fb, Vertex_2D top_left, Vertex_2D bottom_right, float factor);

// Wave distortion
void wave_distortion(lcdpixel* fb, float amplitude, float frequency, float time);

// Chromatic aberration effect
void chromatic_aberration(lcdpixel* fb, int offset);

#endif
