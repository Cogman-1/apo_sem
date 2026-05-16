#include "start_menu.h"
#include "Draw_lib/draw.h"
#include "Draw_lib/text.h"
#include "LEDRGB_Effects/effects.h"
#include "knobs.h"
#include "mzapo_lib/mzapo.h"
#include "sprites/sprites.h"
#include <stdlib.h>
#include <time.h>

// Menu options definitions
#define N_OPTIONS 5
char* labels[N_OPTIONS] = {"Single Player", "Multi Player", "Settings", "Controls", "Exit Game"};

#define TEXT_COLOR 0XFFFF
#define HIGH_COLOR 0xF800

// helpers for dt for LEDRGB updates:
static uint64_t menu_last_time = 0;

static uint64_t menu_time_ms(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

static void menu_reset_dt(void)
{
    menu_last_time = menu_time_ms();
}

static float menu_get_dt(void)
{
    uint64_t now = menu_time_ms();
    uint64_t dt_ms = now - menu_last_time;
    menu_last_time = now;

    if (dt_ms > 100) {
        dt_ms = 100;
    }

    return dt_ms / 1000.0f;
}

static void draw_menu(int highlighted, mzapo_state* state);

int start_menu(mzapo_state* state)
{
    knobs_wait_for_buttons_released(state);
    int highlighted = SINGLE_PLAYER;
    clear_effect();
    knobs_state* inputs = malloc(sizeof(*inputs));
    knobs_state_init(inputs, knobs_read(state));
    menu_reset_dt();
    set_effect(MENU);
    while (1) {
        draw_menu(highlighted, state);

        knobs k = knobs_read(state);
        knobs_update(inputs, k);
        knobs_delta inputs_delta = knobs_get_clamped_delta(inputs);
        highlighted = (highlighted + inputs_delta.r + N_OPTIONS) % N_OPTIONS;
        if (knobs_red_pressed(inputs)) {
            knobs_wait_for_buttons_released(state);
            free(inputs);
            clear_effect();
            update_effect(state, 1);
            return highlighted;
        }
    }
}

static void draw_menu(int highlighted, mzapo_state* state)
{
    lcdpixel fb[SCREEN_HEIGHT * SCREEN_WIDTH];
    clear_display(fb);
    // background, draw first!!!!
    draw_sprite(fb, (Vertex_2D){0, 0}, get_start_menu_sprite());
    draw_sprite(fb, (Vertex_2D){250, 0}, get_title_banner_sprite());
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
    // add blinking RGB_LEDS
    set_effect(MENU);
    float dt = menu_get_dt();
    update_effect(state, dt);
}
