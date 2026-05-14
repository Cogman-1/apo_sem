#include "effects.h"

#include <stddef.h>

#define LEFT 0
#define RIGHT 1

static Effect_Type current_effect = NONE;
static float time = 0.0f;

static const Effect* get_effect(Effect_Type type)
{
    switch (type) {
    case DAMAGE:
        return &damage;
    case MENU:
        return &menu;
    default:
        return NULL;
    }
}

void set_effect(Effect_Type type)
{
    if (current_effect == NONE) {
        current_effect = type;
        time = 0.0f;
    }
}

void update_effect(mzapo_state* hw_state, float dt)
{
    if (current_effect != NONE) {
        time += dt;
        // get the effect
        const Effect* effect = get_effect(current_effect);
        if (!effect) {
            return;
        }
        // effect ended
        if (time >= effect->duration) {
            current_effect = NONE;
            time = 0.0f;
            // reset LEDS to off
            ledrgb_write(hw_state, LEFT, (ledrgb){.raw = 0});
            ledrgb_write(hw_state, RIGHT, (ledrgb){.raw = 0});
        }
        // find the current state the interval should be in
        float interval_start = 0;
        float interval_end = 0 + effect->intervals[0].sec;
        for (int i = 0; i < effect->n_intervals; i++) {
            // check if we are within interval
            if (time >= interval_start && time <= interval_end) {
                ledrgb_write(hw_state, LEFT, (ledrgb){.raw = effect->intervals[i].led1_state});
                ledrgb_write(hw_state, RIGHT, (ledrgb){.raw = effect->intervals[i].led2_state});
            }
            interval_start += effect->intervals[i].sec;
            interval_end += i != effect->n_intervals - 1 ? effect->intervals[i + 1].sec : 0;
        }
    } else {
        ledrgb_write(hw_state, LEFT, (ledrgb){.raw = 0});
        ledrgb_write(hw_state, RIGHT, (ledrgb){.raw = 0});
    }
}

void clear_effect()
{
    current_effect = NONE;
    time = 0.0f;
}