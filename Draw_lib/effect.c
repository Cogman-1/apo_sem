#include "effect.h"
#include "math.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IDX(x, y) ((y) * SCREEN_WIDTH + (x))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

static inline lcdpixel blend_colors(lcdpixel bg, lcdpixel fg, float alpha)
{
    if (alpha >= 1.0f)
        return bg;
    if (alpha <= 0.0f)
        return fg;

    lcdpixel p = {.r = (uint8_t)(bg.r * alpha + fg.r * (1.0f - alpha)),
                  .g = (uint8_t)(bg.g * alpha + fg.g * (1.0f - alpha)),
                  .b = (uint8_t)(bg.b * alpha + fg.b * (1.0f - alpha))};
    return p;
}

static inline lcdpixel blend_additive(lcdpixel bg, lcdpixel fg, float factor)
{
    factor = MAX(0, MIN(1, factor));
    lcdpixel p = {.r = MIN(255, (uint16_t)(bg.r + fg.r * factor)),
                  .g = MIN(255, (uint16_t)(bg.g + fg.g * factor)),
                  .b = MIN(255, (uint16_t)(bg.b + fg.b * factor))};
    return p;
}

void fade_effect(lcdpixel* fb, float time, lcdpixel color)
{
    float alpha = (sinf(time * 2.0 * M_PI) + 1.0f) * 0.5f;

    for (size_t y = 0; y < SCREEN_HEIGHT; y++) {
        for (size_t x = 0; x < SCREEN_WIDTH; x++) {
            fb[IDX(x, y)] = blend_colors(color, fb[IDX(x, y)], alpha);
        }
    }
}

void flash_border(lcdpixel* fb, float time, float intensity, lcdpixel color)
{
    intensity = fmax(0, fmin(1, intensity));

    for (size_t y = 0; y < SCREEN_HEIGHT; y++) {
        for (size_t x = 0; x < SCREEN_WIDTH; x++) {
            float dx = (x - SCREEN_WIDTH / 2.0f) / (SCREEN_WIDTH / 2.0f);
            float dy = (y - SCREEN_HEIGHT / 2.0f) / (SCREEN_HEIGHT / 2.0f);
            float dist = sqrtf(dx * dx + dy * dy);
            dist = MIN(1.0f, dist);

            float alpha = intensity * (1.0f - dist) + (1.0f - intensity) * fabs(sinf(time * M_PI));
            alpha = MAX(0, MIN(1, alpha));

            fb[IDX(x, y)] = blend_colors(color, fb[IDX(x, y)], alpha);
        }
    }
}

void vignette(lcdpixel* fb, float intensity)
{
    intensity = MAX(0, MIN(1, intensity));

    lcdpixel black = {.r = 0, .g = 0, .b = 0};

    for (size_t y = 0; y < SCREEN_HEIGHT; y++) {
        for (size_t x = 0; x < SCREEN_WIDTH; x++) {
            float dx = (x - SCREEN_WIDTH / 2.0f) / (SCREEN_WIDTH / 2.0f);
            float dy = (y - SCREEN_HEIGHT / 2.0f) / (SCREEN_HEIGHT / 2.0f);
            float dist = sqrtf(dx * dx + dy * dy);

            float vignette_factor = 1.0f - (dist * dist) * intensity;
            vignette_factor = MAX(0, MIN(1, vignette_factor));

            fb[IDX(x, y)] = blend_colors(black, fb[IDX(x, y)], 1.0f - vignette_factor);
        }
    }
}

void scanlines(lcdpixel* fb, int line_spacing, int line_thickness, uint8_t darkness)
{
    int dark = darkness;
    dark = MIN(255, dark);
    lcdpixel black = {.r = 0, .g = 0, .b = 0};
    float darken_factor = dark / 255.0f;

    for (size_t y = 0; y < SCREEN_HEIGHT; y++) {
        int line_pos = y % line_spacing;
        if (line_pos < line_thickness) {
            for (size_t x = 0; x < SCREEN_WIDTH; x++) {
                fb[IDX(x, y)] = blend_colors(black, fb[IDX(x, y)], darken_factor);
            }
        }
    }
}

void pixelate(lcdpixel* fb, Vertex_2D top_left, Vertex_2D bottom_right, int pixel_size)
{
    if (pixel_size < 1)
        pixel_size = 1;

    lcdpixel temp_fb[SCREEN_WIDTH * SCREEN_HEIGHT];
    memcpy(temp_fb, fb, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(lcdpixel));

    for (int y = top_left.y; y < bottom_right.y; y += pixel_size) {
        for (int x = top_left.x; x < bottom_right.x; x += pixel_size) {
            uint32_t r = 0, g = 0, b = 0;
            int count = 0;

            for (int dy = 0; dy < pixel_size && y + dy < SCREEN_HEIGHT; dy++) {
                for (int dx = 0; dx < pixel_size && x + dx < SCREEN_WIDTH; dx++) {
                    lcdpixel p = temp_fb[IDX(x + dx, y + dy)];
                    r += p.r;
                    g += p.g;
                    b += p.b;
                    count++;
                }
            }

            if (count > 0) {
                lcdpixel avg = {.r = (uint8_t)(r / count), .g = (uint8_t)(g / count), .b = (uint8_t)(b / count)};

                for (int dy = 0; dy < pixel_size && y + dy < SCREEN_HEIGHT; dy++) {
                    for (int dx = 0; dx < pixel_size && x + dx < SCREEN_WIDTH; dx++) {
                        fb[IDX(x + dx, y + dy)] = avg;
                    }
                }
            }
        }
    }
}

void blur(lcdpixel* fb, Vertex_2D top_left, Vertex_2D bottom_right, int radius)
{
    if (radius < 1)
        radius = 1;

    lcdpixel temp_fb[SCREEN_WIDTH * SCREEN_HEIGHT];
    memcpy(temp_fb, fb, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(lcdpixel));

    for (int y = top_left.y; y < bottom_right.y; y++) {
        for (int x = top_left.x; x < bottom_right.x; x++) {
            if (x - radius >= 0 && y - radius >= 0 && x + radius < SCREEN_WIDTH && y + radius < SCREEN_HEIGHT) {

                uint32_t r = 0, g = 0, b = 0;
                int kernel_size = (2 * radius + 1) * (2 * radius + 1);

                for (int dy = -radius; dy <= radius; dy++) {
                    for (int dx = -radius; dx <= radius; dx++) {
                        lcdpixel p = temp_fb[IDX(x + dx, y + dy)];
                        r += p.r;
                        g += p.g;
                        b += p.b;
                    }
                }

                fb[IDX(x, y)] = (lcdpixel){
                    .r = (uint8_t)(r / kernel_size), .g = (uint8_t)(g / kernel_size), .b = (uint8_t)(b / kernel_size)};
            }
        }
    }
}

void color_shift(lcdpixel* fb, Vertex_2D top_left, Vertex_2D bottom_right, int hue_shift)
{
    hue_shift = hue_shift % 360;

    for (int y = top_left.y; y < bottom_right.y; y++) {
        for (int x = top_left.x; x < bottom_right.x; x++) {
            if (x < SCREEN_WIDTH && y < SCREEN_HEIGHT) {
                lcdpixel p = fb[IDX(x, y)];

                if (hue_shift < 120) {
                    float factor = hue_shift / 120.0f;
                    uint8_t new_r = (uint8_t)(p.r * (1.0f - factor));
                    uint8_t new_g = (uint8_t)MIN(255, p.g + p.r * factor);
                    fb[IDX(x, y)] = (lcdpixel){.r = new_r, .g = new_g, .b = p.b};
                } else if (hue_shift < 240) {
                    float factor = (hue_shift - 120) / 120.0f;
                    uint8_t new_g = (uint8_t)(p.g * (1.0f - factor));
                    uint8_t new_b = (uint8_t)MIN(255, p.b + p.g * factor);
                    fb[IDX(x, y)] = (lcdpixel){.r = p.r, .g = new_g, .b = new_b};
                } else {
                    float factor = (hue_shift - 240) / 120.0f;
                    uint8_t new_b = (uint8_t)(p.b * (1.0f - factor));
                    uint8_t new_r = (uint8_t)MIN(255, p.r + p.b * factor);
                    fb[IDX(x, y)] = (lcdpixel){.r = new_r, .g = p.g, .b = new_b};
                }
            }
        }
    }
}

void invert_region(lcdpixel* fb, Vertex_2D top_left, Vertex_2D bottom_right)
{
    for (int y = top_left.y; y < bottom_right.y; y++) {
        for (int x = top_left.x; x < bottom_right.x; x++) {
            if (x < SCREEN_WIDTH && y < SCREEN_HEIGHT) {
                lcdpixel p = fb[IDX(x, y)];
                fb[IDX(x, y)] = (lcdpixel){.r = 31 - p.r, .g = 63 - p.g, .b = 31 - p.b};
            }
        }
    }
}

void darken_region(lcdpixel* fb, Vertex_2D top_left, Vertex_2D bottom_right, float factor)
{
    factor = MAX(0, MIN(1, factor));
    lcdpixel black = {.r = 0, .g = 0, .b = 0};

    for (int y = top_left.y; y < bottom_right.y; y++) {
        for (int x = top_left.x; x < bottom_right.x; x++) {
            if (x < SCREEN_WIDTH && y < SCREEN_HEIGHT) {
                fb[IDX(x, y)] = blend_colors(black, fb[IDX(x, y)], factor);
            }
        }
    }
}

void lighten_region(lcdpixel* fb, Vertex_2D top_left, Vertex_2D bottom_right, float factor)
{
    factor = MAX(0, MIN(1, factor));
    lcdpixel white = {.r = 31, .g = 63, .b = 31}; // Max values for 5/6/5 bit depth

    for (int y = top_left.y; y < bottom_right.y; y++) {
        for (int x = top_left.x; x < bottom_right.x; x++) {
            if (x < SCREEN_WIDTH && y < SCREEN_HEIGHT) {
                fb[IDX(x, y)] = blend_colors(white, fb[IDX(x, y)], 1.0f - factor);
            }
        }
    }
}

void wave_distortion(lcdpixel* fb, float amplitude, float frequency, float time)
{
    amplitude = MAX(0, MIN(50, amplitude));

    lcdpixel temp_fb[SCREEN_WIDTH * SCREEN_HEIGHT];
    memcpy(temp_fb, fb, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(lcdpixel));

    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            float wave_x = x + amplitude * sinf(frequency * y / SCREEN_HEIGHT + time);
            float wave_y = y + amplitude * cosf(frequency * x / SCREEN_WIDTH + time);

            int src_x = (int)wave_x;
            int src_y = (int)wave_y;

            // Clamp to screen bounds
            src_x = MAX(0, MIN(SCREEN_WIDTH - 1, src_x));
            src_y = MAX(0, MIN(SCREEN_HEIGHT - 1, src_y));

            if (src_x < SCREEN_WIDTH && src_y < SCREEN_HEIGHT) {
                fb[IDX(x, y)] = temp_fb[IDX(src_x, src_y)];
            }
        }
    }
}

void chromatic_aberration(lcdpixel* fb, int offset)
{
    if (offset < 1 || offset > 10)
        offset = 2;

    lcdpixel temp_fb[SCREEN_WIDTH * SCREEN_HEIGHT];
    memcpy(temp_fb, fb, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(lcdpixel));

    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            uint8_t r = (x - offset >= 0) ? temp_fb[IDX(x - offset, y)].r : 0;

            uint8_t g = temp_fb[IDX(x, y)].g;

            uint8_t b = (x + offset < SCREEN_WIDTH) ? temp_fb[IDX(x + offset, y)].b : 0;

            fb[IDX(x, y)] = (lcdpixel){.r = r, .g = g, .b = b};
        }
    }
}
