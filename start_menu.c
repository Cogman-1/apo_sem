#include "start_menu.h"
#include "Draw_lib/draw.h"
#include "Draw_lib/text.h"
#include "knobs.h"
#include "mzapo_lib/mzapo.h"
#include <stdlib.h>

// Menu options definitions
#define N_OPTIONS 5
char* labels[N_OPTIONS] = {"Single Player", "Multi Player", "Settings", "Controls", "Exit Game"};

#define TEXT_COLOR 0XFFFF
#define HIGH_COLOR 0xF800

static void draw_menu(int highlighted, mzapo_state* state);

int start_menu(mzapo_state* state)
{
    int highlighted = SINGLE_PLAYER;

    knobs_state* inputs = malloc(sizeof(*inputs));
    knobs_state_init(inputs, knobs_read(state));

    while (1) {
        draw_menu(highlighted, state);

        knobs k = knobs_read(state);
        knobs_update(inputs, k);
        knobs_delta inputs_delta = knobs_get_clamped_delta(inputs);
        highlighted = (highlighted + inputs_delta.r) % N_OPTIONS;
        if (k.rdown) {
            free(inputs);
            return highlighted;
        }
    }
}

static void draw_menu(int highlighted, mzapo_state* state)
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
              FONT_PROP14x16);
    parlcd_write_screen(state, fb);
}
