#include "pause.h"
#include "../../knobs.h"
#include "../../sprites/sprites.h"
#include <stdlib.h>

#define NUMBER_OF_OPTIONS 4
#define TEXT_COLOR 0XFFFF
#define HIGH_COLOR 0xF800

char* pause_labels[NUMBER_OF_OPTIONS] = {"Resume", "Settings", "Controls", "Exit"};

static void draw_pause(mzapo_state* state, int highlighted, lcdpixel* fb)
{
    lcdpixel t_color;
    Vertex_2D start = {20, 0};
    draw_sprite(fb, (Vertex_2D){250, 0}, get_title_banner_sprite());
    for (int i = 0; i < NUMBER_OF_OPTIONS; i++) {
        t_color.raw = i == highlighted ? HIGH_COLOR : TEXT_COLOR;
        start.y = 200 + i * 25;
        draw_text(fb, start, pause_labels[i], t_color, FONT_ROM8x16);
    }
    parlcd_write_screen(state, fb);
}

int pause(mzapo_state* state, lcdpixel* fb)
{
    knobs_wait_for_buttons_released(state);
    int highlighted = P_RESUME;
    knobs_state* inputs = malloc(sizeof(knobs_state));
    knobs_state_init(inputs, knobs_read(state));
    darken_background(fb);
    while (1) {
        draw_pause(state, highlighted, fb);

        knobs k = knobs_read(state);
        knobs_update(inputs, k);
        knobs_delta inputs_delt = knobs_get_clamped_delta(inputs);
        highlighted = (highlighted + inputs_delt.r + NUMBER_OF_OPTIONS) % NUMBER_OF_OPTIONS;
        if (knobs_red_pressed(inputs)) {
            knobs_wait_for_buttons_released(state);
            switch (highlighted) {
            case P_RESUME:
                free(inputs);
                return 0;
            case P_EXIT_MAIN_MENU:
                free(inputs);
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
