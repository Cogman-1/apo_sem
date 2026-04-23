#include "start_menu.h"
#include "Draw_lib/draw.h"
#include "Draw_lib/text.h"
#include "mzapo_lib/mzapo.h"
// Menu Options

// text for options
#define N_OPTIONS 5
char* labels[N_OPTIONS] = {"Single Player", "Multi Player", "Settings", "Controls", "Exit Game"};

// Color definitions
#define BCKG_COLOR 0x0000
#define TEXT_COLOR 0XFFFF
#define HIGH_COLOR 0xF800

void draw_menu(int highlighted, mzapo_state* state);

int start_menu(mzapo_state* state)
{
    /*Init of values*/
    int highlighted = SINGLE_PLAYER;
    knobs inputs = knobs_read(state);
    unsigned knob_val = inputs.r;
    unsigned last_knob_val = knob_val;
    while (1) {
        draw_menu(highlighted, state);
        /* compare if the knob position has changed */
        inputs = knobs_read(state);
        knob_val = inputs.r;
        if (last_knob_val != knob_val) {
            if (last_knob_val < knob_val)
                highlighted++;
            else
                highlighted--;
            if (highlighted > EXIT_GAME)
                highlighted = 0;
            else if (highlighted < 0)
                highlighted = EXIT_GAME;
            last_knob_val = knob_val;
        }
        /* On click return the currently picked option*/
        if (inputs.rdown) {
            return highlighted;
        }
    }
}

void draw_menu(int highlighted, mzapo_state* state)
{
    lcdpixel fb[SCREEN_HEIGHT * SCREEN_WIDTH];
    clear_display(fb);
    lcdpixel t_color;
    Vertex_2D start = {20, 0};
    for (int i = 0; i < N_OPTIONS; i++) {
        t_color.raw = i == highlighted ? HIGH_COLOR : TEXT_COLOR;
        start.y = 100 + i * 25;
        draw_text(fb, start, labels[i], t_color, FONT_ROM8x16);
    }
    start.x = 20;
    start.y = 260;
    t_color.raw = TEXT_COLOR;
    draw_text(fb, start, "Use the red knob to control the menu!\nPress to select highlighted option", t_color,
              FONT_ROM8x16);
    parlcd_write_screen(state, fb);
}
