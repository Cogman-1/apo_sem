#include "leveling.h"
#include "../../knobs.h"
#include "player.h"

#include <stdlib.h>

#define NUMBER_OF_OPTIONS 6
#define TEXT_COLOR WHITE
#define HIGH_COLOR RED

char* level_labels[NUMBER_OF_OPTIONS] = {"Increase Damage +10%!",     "Increase Max Health +10%!",
                                         "Increase Fire Rate +10%!",  "Decrease Ability Cooldowns -10%!",
                                         "Increase Move Speed +10%!", "Heal to full health!"};

static void draw_leveling(mzapo_state* state, int highlighted, lcdpixel* fb)
{
    lcdpixel t_color = {.raw = WHITE};
    draw_text(fb, (Vertex_2D){SCREEN_WIDTH / 2 - 90, 50}, "Level up!", t_color, FONT_ROM8x16);
    Vertex_2D start = {20, 0};
    for (int i = 0; i < NUMBER_OF_OPTIONS; i++) {
        t_color.raw = i == highlighted ? HIGH_COLOR : TEXT_COLOR;
        start.y = 150 + i * 25;
        draw_text(fb, start, level_labels[i], t_color, FONT_ROM8x16);
    }
    parlcd_write_screen(state, fb);
}

void level(mzapo_state* state, lcdpixel* fb, Player* player)
{
    knobs_wait_for_buttons_released(state);
    int highlighted = L_DAMAGE;
    knobs_state* inputs = malloc(sizeof(knobs_state));
    knobs_state_init(inputs, knobs_read(state));
    darken_background(fb);
    while (1) {
        draw_leveling(state, highlighted, fb);
        knobs k = knobs_read(state);
        knobs_update(inputs, k);
        knobs_delta inputs_delt = knobs_get_clamped_delta(inputs);
        highlighted = (highlighted + inputs_delt.r + NUMBER_OF_OPTIONS) % NUMBER_OF_OPTIONS;
        if (knobs_red_pressed(inputs)) {
            knobs_wait_for_buttons_released(state);
            level_up(player, highlighted);
            return;
        }
    }
}
