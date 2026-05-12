#pragma once

#include "mzapo_lib/mzapo.h"

typedef struct knobs_state {
    knobs calibrated_knobs;
    knobs last_read_clamped_knobs;
    knobs last_knobs;
    knobs current_knobs;
} knobs_state;

typedef struct knobs_delta {
    int8_t b;
    int8_t g;
    int8_t r;
} knobs_delta;

typedef struct knob_directions {
    float b;
    float bx;
    float by;
    float g;
    float gx;
    float gy;
    float r;
    float rx;
    float ry;
} knob_directions;

void knobs_state_init(knobs_state* state, knobs current_knobs);
void knobs_update(knobs_state* state, knobs current_knobs);
knobs_delta knobs_get_clamped_delta(knobs_state* state);
knobs_delta knobs_get_delta(knobs_state* state);

int knobs_any_button_down(knobs k);
int knobs_red_pressed(knobs_state* state);
void knobs_wait_for_buttons_released(mzapo_state* state);

knob_directions calculate_direction(knobs_state* state);