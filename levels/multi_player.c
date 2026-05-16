#include "multi_player.h"

#include <stdlib.h>

#include "../Draw_lib/draw.h"
#include "../Draw_lib/text.h"
#include "../LEDRGB_Effects/effects.h"
#include "../knobs.h"
#include "../mzapo_lib/mzapo.h"
#include "../sprites/sprites.h"

static void draw_message(mzapo_state* state)
{
    lcdpixel fb[SCREEN_WIDTH * SCREEN_HEIGHT];
    clear_display(fb);
    clear_effect();
    update_effect(state, 1);
    draw_sprite(fb, (Vertex_2D){160, 80}, get_cog_wip_sprite());
    draw_text(fb, (Vertex_2D){SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT - 50}, "Multiplayer is WIP.",
              (lcdpixel){.raw = 0xffff}, FONT_PROP14x16);
    draw_text(fb, (Vertex_2D){SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT - 20}, "Press red knob to return.",
              (lcdpixel){.raw = 0xffff}, FONT_PROP14x16);
    parlcd_write_screen(state, fb);
}

void multi_player(mzapo_state* state)
{
    int exit = 0;
    knobs_state* inputs = malloc(sizeof(knobs_state));
    knobs_state_init(inputs, knobs_read(state));

    draw_message(state);
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