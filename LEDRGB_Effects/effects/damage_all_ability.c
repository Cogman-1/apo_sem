#include "../effects.h"

#define RED 0XFF0000
#define YEL 0XFFFF00

static const Effect_Interval ability_int1 = {.sec = 0.2f, .led1_state = RED, .led2_state = RED};

static const Effect_Interval ability_int2 = {.sec = 0.2f, .led1_state = YEL, .led2_state = YEL};

static const Effect_Interval ability_int3 = {.sec = 0.2f, .led1_state = RED, .led2_state = RED};

static const Effect_Interval ability_int4 = {.sec = 0.2f, .led1_state = YEL, .led2_state = YEL};

const Effect ability = {.duration = 0.8f,
                        .n_intervals = 4,
                        .intervals = {ability_int1, ability_int2, ability_int3, ability_int4},
                        .type = DAMAGE};