#include "pause.h"

#define NUMBER_OF_OPTIONS 4
#define TEXT_COLOR 0XFFFF
#define HIGH_COLOR 0xF800

char* pause_labels[NUMBER_OF_OPTIONS] = {"Resume", "Settings", "Controls", "Exit"};

static void draw_pause(mzapo_state* state, int highlighted, lcdpixel* fb)
{
    lcdpixel t_color;
    Vertex_2D start = {20, 0};
    for (int i = 0; i < NUMBER_OF_OPTIONS; i++) {
        t_color.raw = i == highlighted ? HIGH_COLOR : TEXT_COLOR;
        start.y = 100 + i * 25;
        draw_text(fb, start, pause_labels[i], t_color, FONT_ROM8x16);
    }
    parlcd_write_screen(state, fb);
}

// darkens the screen the background screen by 50%
static void darken_background(lcdpixel* fb)
{
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            fb[y * SCREEN_WIDTH + x].r >>= 1;
            fb[y * SCREEN_WIDTH + x].g >>= 1;
            fb[y * SCREEN_WIDTH + x].b >>= 1;
        }
    }
}

static void wait_for_release(mzapo_state* state)
{
    knobs k;
    do {
        k = knobs_read(state);
    } while (k.rdown);
}

int pause(mzapo_state* state, lcdpixel* fb)
{
    int highlighted = P_RESUME;
    knobs inputs = knobs_read(state);
    unsigned knob_val = inputs.r;
    unsigned last_knob_val = knob_val;
    darken_background(fb);
    while (1) {
        draw_pause(state, highlighted, fb);
        inputs = knobs_read(state);
        knob_val = inputs.r;
        if (last_knob_val != knob_val) {
            if (last_knob_val < knob_val)
                highlighted++;
            else
                highlighted--;
            if (highlighted > P_EXIT_MAIN_MENU)
                highlighted = P_RESUME;
            if (highlighted < P_RESUME)
                highlighted = P_EXIT_MAIN_MENU;
            last_knob_val = knob_val;
        }
        // on knob press apply the appropriate action
        wait_for_release(state);
        if (inputs.rdown) {
            switch (highlighted) {
            case P_RESUME:
                return 0;
            case P_EXIT_MAIN_MENU:
                return P_EXIT_MAIN_MENU;
            case P_CONTROLS:
                controls();
                break;
            case P_SETTINGS:
                settings();
                break;
            default:
                break;
            }
        }
    }
}
