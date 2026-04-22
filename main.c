#define _POSIX_C_SOURCE 200112L

#include <stdint.h>
#include <stdio.h>

#include "mzapo_lib/font_rom8x16.c"
#include "mzapo_lib/mzapo.h"
#include "start_menu.h"

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;
    mzapo_state* state = init_mzapo();
#if 0
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
#endif
    start_menu(state);
    return 0;
}
