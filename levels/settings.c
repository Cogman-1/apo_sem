#include "settings.h"

#include <stdlib.h>

#include "../Draw_lib/draw.h"
#include "../Draw_lib/text.h"
#include "../knobs.h"
#include "../sprites/sprites.h"

#define OPTIONS_OFFSET_X 120
#define OPTIONS_LOWER_LINE_Y 240

#define NUMBER_OF_SETTINGS 3
#define NUMBER_OF_OPTIONS (NUMBER_OF_SETTINGS + 2)
enum { S_DIFFICULTY = 0, S_HEALTHBAR, S_LEDRGBEFFECTS, S_RESET, S_EXIT };
static char* settings_labels[3] = {"Difficulty: ", "Healthbar Display: ", "RGB LED effects: "};
static char* diff_labels[2] = {"< Easy >", "< Hard >"};
static char* labels_switch_healthbar[3] = {"< On Screen >", "< LED Bar >", "< Both >"};
static char* off_on[2] = {"< Off >", "< On >"};

#define TEXT_COLOR 0XFFFF
#define HIGH_COLOR 0XF800

// initialize the settings and
static GameSettings settings = {
    .diff = HARD, .UseHealthBar = BOTH, .UseScreenHealthbar = 1, .UseLEDHealthbar = 1, .UseLEDRGBEffects = 1};

static void reset_settings()
{
    settings.diff = HARD;
    settings.UseHealthBar = BOTH;
    settings.UseScreenHealthbar = 1;
    settings.UseLEDHealthbar = 1;
    settings.UseLEDRGBEffects = 1;
}

GameSettings* get_settings()
{
    return &settings;
}

static void draw_settings(mzapo_state* state, int highlighted)
{
    lcdpixel fb[SCREEN_WIDTH * SCREEN_HEIGHT];
    clear_display(fb);
    // draw the menu
    draw_text(fb, (Vertex_2D){SCREEN_WIDTH / 2 - 50, 20}, "Settings:", (lcdpixel){.raw = TEXT_COLOR}, FONT_PROP14x16);
    lcdpixel t_color;
    Vertex_2D start = {0, 0};
    for (int i = 0; i < NUMBER_OF_SETTINGS; i++) {
        start.y = 80 + i * 50;
        draw_text(fb, start, settings_labels[i], (lcdpixel){.raw = TEXT_COLOR}, FONT_PROP14x16);
        // draw option switch
        int idx;
        t_color.raw = i == highlighted ? HIGH_COLOR : TEXT_COLOR;
        switch (i) {
        case S_DIFFICULTY:
            idx = settings.diff == EASY ? 0 : 1;
            draw_text(fb, (Vertex_2D){SCREEN_WIDTH - OPTIONS_OFFSET_X, start.y}, diff_labels[idx], t_color,
                      FONT_PROP14x16);
            break;
        case S_HEALTHBAR:
            idx = settings.UseHealthBar;
            draw_text(fb, (Vertex_2D){SCREEN_WIDTH - OPTIONS_OFFSET_X, start.y}, labels_switch_healthbar[idx], t_color,
                      FONT_PROP14x16);
            break;
        case S_LEDRGBEFFECTS:
            idx = settings.UseLEDRGBEffects;
            draw_text(fb, (Vertex_2D){SCREEN_WIDTH - OPTIONS_OFFSET_X, start.y}, off_on[idx], t_color, FONT_PROP14x16);
            break;
        default:
            break;
        }
        t_color.raw = TEXT_COLOR;
    }

    if (highlighted == S_RESET)
        t_color.raw = HIGH_COLOR;
    draw_text(fb, (Vertex_2D){SCREEN_WIDTH / 3 - 120, OPTIONS_LOWER_LINE_Y}, "Reset to default", t_color,
              FONT_PROP14x16);
    t_color.raw = TEXT_COLOR;

    if (highlighted == S_EXIT)
        t_color.raw = HIGH_COLOR;
    draw_text(fb, (Vertex_2D){SCREEN_WIDTH / 3 + 160, OPTIONS_LOWER_LINE_Y}, "Return to menu", t_color, FONT_PROP14x16);
    t_color.raw = TEXT_COLOR;

    draw_text(fb, (Vertex_2D){SCREEN_HEIGHT / 2 - 60, OPTIONS_LOWER_LINE_Y + 30},
              "Turn red knob to navigate the settings.", t_color, FONT_PROP14x16);
    draw_text(fb, (Vertex_2D){SCREEN_HEIGHT / 2 - 60, OPTIONS_LOWER_LINE_Y + 50},
              "Press red knob to change the settings/select option.", t_color, FONT_PROP14x16);

    parlcd_write_screen(state, fb);
}

static void cycle_healthbar_options()
{
    settings.UseHealthBar = (settings.UseHealthBar + 1) % 3;
    switch (settings.UseHealthBar) {
    case DISPLAY:
        settings.UseLEDHealthbar = 0;
        settings.UseScreenHealthbar = 1;
        break;
    case LED:
        settings.UseLEDHealthbar = 1;
        settings.UseScreenHealthbar = 0;
        break;
    case BOTH:
        settings.UseLEDHealthbar = 1;
        settings.UseScreenHealthbar = 1;
        break;
    }
}

void settings_menu(mzapo_state* state)
{
    int exit = 0;
    int highlighted = S_DIFFICULTY;
    knobs_state* inputs = malloc(sizeof(knobs_state));
    knobs_state_init(inputs, knobs_read(state));

    while (!exit) {
        draw_settings(state, highlighted);
        knobs k = knobs_read(state);
        knobs_update(inputs, k);
        knobs_delta inputs_delta = knobs_get_clamped_delta(inputs);
        highlighted = (highlighted + inputs_delta.r + NUMBER_OF_OPTIONS) % NUMBER_OF_OPTIONS;
        if (knobs_red_pressed(inputs)) {
            knobs_wait_for_buttons_released(state);
            switch (highlighted) {
            case S_DIFFICULTY:
                settings.diff = settings.diff == EASY ? HARD : EASY;
                break;
            case S_HEALTHBAR:
                cycle_healthbar_options();
                break;
            case S_LEDRGBEFFECTS:
                settings.UseLEDRGBEffects = settings.UseLEDRGBEffects ? 0 : 1;
                break;
            case S_RESET:
                reset_settings();
                break;
            case S_EXIT:
                exit = 1;
                break;
            default:
                break;
            }
        }
    }
    free(inputs);
}