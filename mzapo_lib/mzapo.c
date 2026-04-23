#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "mzapo.h"
#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "serialize_lock.h"

// Return pointer to mzapo_state
mzapo_state* init_mzapo()
{
    if (serialize_lock(1) <= 0) {
        printf("System is occupied\n");
        serialize_lock(0);
    }

    mzapo_state* state = malloc(sizeof(*state));
    state->spiled_reg_base = (char*)map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
    state->parlcd_reg_base = (char*)map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
    parlcd_hx8357_init((unsigned char*)state->parlcd_reg_base);

    knobs initial_knobs = knobs_read(state);
    state->calibrated_knobs[0] = initial_knobs.r;
    state->calibrated_knobs[1] = initial_knobs.g;
    state->calibrated_knobs[2] = initial_knobs.b;

    return state;
}

uint32_t ledline_read(mzapo_state* state)
{
    return *(volatile uint32_t*)(state->spiled_reg_base + SPILED_REG_LED_LINE_o);
}

void ledline_write(mzapo_state* state, uint32_t value)
{
    *(volatile uint32_t*)(state->spiled_reg_base + SPILED_REG_LED_LINE_o) = value;
}

void ledrgb_write(mzapo_state* state, int side, ledrgb value)
{
    size_t address = side == 1 ? SPILED_REG_LED_RGB1_o : SPILED_REG_LED_RGB2_o;
    *(volatile uint32_t*)(state->spiled_reg_base + address) = value.raw;
}

knobs knobs_read(mzapo_state* state)
{
    knobs k = {.raw = *(volatile uint32_t*)(state->spiled_reg_base + SPILED_REG_KNOBS_8BIT_o)};
    return k;
}

void parlcd_write_screen(mzapo_state* state, const lcdpixel* buffer)
{
    parlcd_write_cmd((unsigned char*)state->parlcd_reg_base, 0x2c);
    for (int i = 0; i < 480 * 320; i++) {
        parlcd_write_data((unsigned char*)state->parlcd_reg_base, buffer[i].raw);
    }
}
