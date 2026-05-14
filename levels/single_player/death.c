#include "death.h"
#include "../../Draw_lib/draw.h"
#include "../../Draw_lib/effect.h"
#include "../../Draw_lib/text.h"
#include "../../knobs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N_OPTIONS 2
char* death_labels[N_OPTIONS] = {"Exit to main menu", "Start new run"};

#define TEXT_COLOR 0XFFFF
#define HIGH_COLOR 0XF800

static void draw_menu(int highlighted, lcdpixel* fb, mzapo_state* state, Player* player);

int death(Player* player, lcdpixel* fb, mzapo_state* state)
{
    knobs_wait_for_buttons_released(state);
    int highlighted = EXIT;
    darken_background(fb);
    clear_effect();
    knobs_state* inputs = malloc(sizeof(knobs_state));
    knobs_state_init(inputs, knobs_read(state));
    while (1) {
        draw_menu(highlighted, fb, state, player);
        knobs k = knobs_read(state);
        knobs_update(inputs, k);
        knobs_delta inputs_delta = knobs_get_clamped_delta(inputs);
        highlighted = (highlighted + inputs_delta.r + N_OPTIONS) % N_OPTIONS;
        if (knobs_red_pressed(inputs)) {
            free(inputs);
            return highlighted;
        }
    }
}

static void draw_menu(int highlighted, lcdpixel* fb, mzapo_state* state, Player* player)
{
    lcdpixel t_color;
    Vertex_2D start = {SCREEN_WIDTH / 2 - 60, 80};
    // title
    t_color.raw = TEXT_COLOR;
    draw_text(fb, start, "You Died!", t_color, FONT_ROM8x16);
    // score print
    char score_text[DEATH_SCORE_TEXT_LENGTH];
    const char* text = DEATH_SCORE_TEXT;
    memcpy(score_text, text, DEATH_SCORE_CHARS);
    sprintf(score_text + DEATH_SCORE_CHARS, "%d", player->playerScore);
    score_text[DEATH_SCORE_TEXT_LENGTH - 1] = '\0';
    start.x = SCREEN_WIDTH / 2 - 80;
    start.y = 120;
    draw_text(fb, start, score_text, t_color, FONT_ROM8x16);
    start.x = 20;
    // options
    for (int i = 0; i < N_OPTIONS; i++) {
        t_color.raw = i == highlighted ? HIGH_COLOR : TEXT_COLOR;
        start.y = 200 + i * 25;
        draw_text(fb, start, death_labels[i], t_color, FONT_ROM8x16);
    }
    parlcd_write_screen(state, fb);
}
