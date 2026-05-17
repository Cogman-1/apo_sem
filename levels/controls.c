#include "controls.h"

#include "multi_player.h"

#include <stdlib.h>

#include "../Draw_lib/draw.h"
#include "../Draw_lib/text.h"
#include "../LEDRGB_Effects/effects.h"
#include "../knobs.h"
#include "../mzapo_lib/mzapo.h"
#include "../sprites/sprites.h"

#define TEXT_ABOVE_KNOBS_Y 40
#define TEXT_UNDER_KNOBS_Y 200

#define TEXT_COLOR 0X0000

static void draw_controls(mzapo_state* state)
{
    // initialize and clean screen and rgb led effect
    lcdpixel fb[SCREEN_WIDTH * SCREEN_HEIGHT];
    // draw the controls
    // draw background
    draw_sprite(fb, (Vertex_2D){0, 0}, get_controls_bckg_sprite());
    // draw knobs sprites
    for (int i = 0; i <= 2; i++) {
        draw_sprite_centered(fb, (Vertex_2D){(60 + i * 180), 150}, get_knob_icon(i));
    }
    // red knob
    // above
    draw_text(fb, (Vertex_2D){30, TEXT_ABOVE_KNOBS_Y}, "Turn:\nChange\nmovement\ndirection",
              (lcdpixel){.raw = TEXT_COLOR}, FONT_PROP14x16);
    // below
    draw_text(fb, (Vertex_2D){30, TEXT_UNDER_KNOBS_Y}, "Press:\nDash", (lcdpixel){.raw = TEXT_COLOR}, FONT_PROP14x16);
    // green knob
    // above
    draw_text(fb, (Vertex_2D){210, TEXT_ABOVE_KNOBS_Y}, "Turn:\nNone", (lcdpixel){.raw = TEXT_COLOR}, FONT_PROP14x16);
    // below
    draw_text(fb, (Vertex_2D){210, TEXT_UNDER_KNOBS_Y}, "Press:\nPause\nGame", (lcdpixel){.raw = TEXT_COLOR},
              FONT_PROP14x16);
    // blue knob
    // above
    draw_text(fb, (Vertex_2D){390, TEXT_ABOVE_KNOBS_Y}, "Turn:\nSelect\nAbility", (lcdpixel){.raw = TEXT_COLOR},
              FONT_PROP14x16);
    // below
    draw_text(fb, (Vertex_2D){390, TEXT_UNDER_KNOBS_Y}, "Press:\nTrigger\nAbility", (lcdpixel){.raw = TEXT_COLOR},
              FONT_PROP14x16);

    draw_text(fb, (Vertex_2D){SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT - 20}, "Press red knob to return.",
              (lcdpixel){.raw = TEXT_COLOR}, FONT_PROP14x16);
    // write the screen to the display
    parlcd_write_screen(state, fb);
}

void controls(mzapo_state* state)
{
    int exit = 0;
    knobs_state* inputs = malloc(sizeof(knobs_state));
    knobs_state_init(inputs, knobs_read(state));

    draw_controls(state);
    while (!exit) {
        knobs k = knobs_read(state);
        knobs_update(inputs, k);

        if (knobs_red_pressed(inputs)) {
            knobs_wait_for_buttons_released(state);
            exit = 1;
        }
    }
    free(inputs);
}