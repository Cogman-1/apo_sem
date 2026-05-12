#include "knobs.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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

    // overflow is intended
    int16_t r = state->current_knobs.r - state->calibrated_knobs.r;
    if (abs(r) > 80)
        state->calibrated_knobs.r += r / 80 * 80;
    int16_t g = state->current_knobs.g - state->calibrated_knobs.g;
    if (abs(g) > 80)
        state->calibrated_knobs.g += g / 80 * 80;
    int16_t b = state->current_knobs.b - state->calibrated_knobs.b;
    if (abs(b) > 80)
        state->calibrated_knobs.b += b / 80 * 80;
}
knobs_delta knobs_get_clamped_delta(knobs_state* state)
{
    int16_t r = state->current_knobs.r - state->last_read_clamped_knobs.r;
    state->last_read_clamped_knobs.r =
        (state->current_knobs.r / 4) * 4 + 4 * (state->current_knobs.r % 4 != 0 && r < 0);
    int16_t g = state->current_knobs.g - state->last_read_clamped_knobs.g;
    state->last_read_clamped_knobs.g =
        (state->current_knobs.g / 4) * 4 + 4 * (state->current_knobs.g % 4 != 0 && r < 0);
    int16_t b = state->current_knobs.b - state->last_read_clamped_knobs.b;
    state->last_read_clamped_knobs.b =
        (state->current_knobs.b / 4) * 4 + 4 * (state->current_knobs.b % 4 != 0 && r < 0);

    knobs_delta delta = {.r = r / 4, .g = g / 4, .b = b / 4};
    return delta;
}

knobs_delta knobs_get_delta(knobs_state* state)
{
    knobs_delta delta = {
        .r = state->last_knobs.r - state->current_knobs.r,
        .g = state->last_knobs.g - state->current_knobs.g,
        .b = state->last_knobs.b - state->current_knobs.b,
    };
    return delta;
}

knob_directions calculate_direction(knobs_state* state)
{
    float r = (uint8_t)(state->current_knobs.r - state->calibrated_knobs.r) / 80.0 * 2.0 * M_PI;
    float g = (uint8_t)(state->current_knobs.g - state->calibrated_knobs.g) / 80.0 * 2.0 * M_PI;
    float b = (uint8_t)(state->current_knobs.b - state->calibrated_knobs.b) / 80.0 * 2.0 * M_PI;

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
