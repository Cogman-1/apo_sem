#include "knobs.h"

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int16_t normalize_mod_80(int16_t value)
{
    value %= 80;
    if (value < 0)
        value += 80;
    return value;
}

void knobs_state_init(knobs_state* state, knobs current_knobs)
{
    state->calibrated_knobs = current_knobs;
    state->last_read_clamped_knobs = current_knobs;
    state->last_knobs = current_knobs;
    state->current_knobs = current_knobs;
}

void knobs_update(knobs_state* state, knobs current_knobs)
{
    state->last_knobs = state->current_knobs;
    state->current_knobs = current_knobs;

    int16_t r = state->current_knobs.r - state->calibrated_knobs.r;
    if (abs(r) > 80)
        state->calibrated_knobs.r += (r / 80) * 80;
    int16_t g = state->current_knobs.g - state->calibrated_knobs.g;
    if (abs(g) > 80)
        state->calibrated_knobs.g += (g / 80) * 80;
    int16_t b = state->current_knobs.b - state->calibrated_knobs.b;
    if (abs(b) > 80)
        state->calibrated_knobs.b += (b / 80) * 80;
}

knobs_delta knobs_get_clamped_delta(knobs_state* state)
{
    int8_t r = state->current_knobs.r - state->last_read_clamped_knobs.r;
    int8_t g = state->current_knobs.g - state->last_read_clamped_knobs.g;
    int8_t b = state->current_knobs.b - state->last_read_clamped_knobs.b;

    state->last_read_clamped_knobs.r =
        state->current_knobs.r - (state->current_knobs.r % 4) + ((state->current_knobs.r % 4) && r < 0 ? 4 : 0);
    state->last_read_clamped_knobs.g =
        state->current_knobs.g - (state->current_knobs.g % 4) + ((state->current_knobs.g % 4) && g < 0 ? 4 : 0);
    state->last_read_clamped_knobs.b =
        state->current_knobs.b - (state->current_knobs.b % 4) + ((state->current_knobs.b % 4) && b < 0 ? 4 : 0);

    return (knobs_delta){
        .r = r / 4,
        .g = g / 4,
        .b = b / 4,
    };
}

knobs_delta knobs_get_delta(knobs_state* state)
{
    return (knobs_delta){
        .r = state->current_knobs.r - state->last_knobs.r,
        .g = state->current_knobs.g - state->last_knobs.g,
        .b = state->current_knobs.b - state->last_knobs.b,
    };
}

knob_directions calculate_direction(knobs_state* state)
{
    int16_t dr = normalize_mod_80(state->current_knobs.r - state->calibrated_knobs.r);
    int16_t dg = normalize_mod_80(state->current_knobs.g - state->calibrated_knobs.g);
    int16_t db = normalize_mod_80(state->current_knobs.b - state->calibrated_knobs.b);

    float r = (float)dr / 80.0f * 2.0f * (float)M_PI;
    float g = (float)dg / 80.0f * 2.0f * (float)M_PI;
    float b = (float)db / 80.0f * 2.0f * (float)M_PI;

    return (knob_directions){
        .r = r,
        .rx = cosf(r),
        .ry = sinf(r),
        .g = g,
        .gx = cosf(g),
        .gy = sinf(g),
        .b = b,
        .bx = cosf(b),
        .by = sinf(b),
    };
}

int knobs_any_button_down(knobs k)
{
    return k.rdown || k.gdown || k.bdown;
}

int knobs_red_pressed(knobs_state* state)
{
    return !state->last_knobs.rdown && state->current_knobs.rdown;
}

void knobs_wait_for_buttons_released(mzapo_state* state)
{
    while (knobs_any_button_down(knobs_read(state))) {
        usleep(1000);
    }
}