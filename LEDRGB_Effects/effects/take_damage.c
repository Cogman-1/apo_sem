#include "../effects.h"

static const Effect_Interval int1 = {.sec = 0.2f, .led1_state = 0xff0000, .led2_state = 0xff0000};

static const Effect_Interval int2 = {.sec = 0.2f, .led1_state = 0x000000, .led2_state = 0x000000};

static const Effect_Interval int3 = {.sec = 0.2f, .led1_state = 0xff0000, .led2_state = 0xff0000};

static const Effect_Interval int4 = {.sec = 0.2f, .led1_state = 0x000000, .led2_state = 0x000000};

const Effect damage = {.duration = 0.8f, .n_intervals = 4, .intervals = {int1, int2, int3, int4}, .type = DAMAGE};