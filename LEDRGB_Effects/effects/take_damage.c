#include "../effects.h"

static const Effect_Interval damage_int1 = {.sec = 0.2f, .led1_state = 0xff0000, .led2_state = 0xff0000};

static const Effect_Interval damage_int2 = {.sec = 0.2f, .led1_state = 0x000000, .led2_state = 0x000000};

static const Effect_Interval damage_int3 = {.sec = 0.2f, .led1_state = 0xff0000, .led2_state = 0xff0000};

static const Effect_Interval damage_int4 = {.sec = 0.2f, .led1_state = 0x000000, .led2_state = 0x000000};

const Effect damage = {.duration = 0.8f,
                       .n_intervals = 4,
                       .intervals = {damage_int1, damage_int2, damage_int3, damage_int4},
                       .type = DAMAGE};