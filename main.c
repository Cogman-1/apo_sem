#define _POSIX_C_SOURCE 200112L

#include <stdint.h>
#include <stdio.h>

#include "mzapo_lib/font_rom8x16.c"
#include "mzapo_lib/mzapo.h"

void buffer_write_text(lcdpixel* buffer, size_t startx, size_t starty, char* text, lcdpixel color)
{
    size_t offx = startx;
    size_t offy = starty;

    for (size_t i = 0; text[i]; i++) {
        if (text[i] == '\n') {
            offy += font_rom8x16.height;
            offx = startx;
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

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;
    mzapo_state* state = init_mzapo();

    while (1) {
        knobs k = knobs_read(state);
        ledrgb ledrgb_value = {.r = k.r * 255 / 31, .g = k.g * 255 / 63, .b = k.b * 255 / 31};
        ledrgb_write(state, 1, ledrgb_value);
        ledrgb_write(state, 2, ledrgb_value);

        lcdpixel buffer[480][320];
        for (int y = 0; y < 320; y++) {
            for (int x = 0; x < 480; x++) {
                lcdpixel pixel = {.r = k.r, .g = k.g, .b = k.b};
                buffer[x][y] = pixel;
            }
        }

        lcdpixel pixel;
        if (k.r + k.g + k.b > 256 + 128) {
            pixel = (lcdpixel){.r = 0, .g = 0, .b = 0};
        } else {
            pixel = (lcdpixel){.r = 31, .g = 63, .b = 31};
        }

        char text[256];
        snprintf(text, sizeof(text), "knob=%d={.r=%d .g=%d .b=%d}\nledrgb=%d lcdpixel=%d", k.raw, k.r, k.g, k.b,
                 ledrgb_value.raw, buffer[0][0].raw);
        buffer_write_text((lcdpixel*)buffer, 20, 50, text, pixel);

        parlcd_write_screen(state, (lcdpixel*)buffer);
    }

    return 0;
}
