#include "../effects.h"

#define RED 0XFF0000
#define YEL 0XFFFF00
#define GRN 0X00FF00
#define LBL 0X00FFFF
#define BLU 0X0000FF

static const Effect_Interval menu_int1 = {.sec = 0.3f, .led1_state = RED, .led2_state = RED};

static const Effect_Interval menu_int2 = {.sec = 0.3f, .led1_state = YEL, .led2_state = YEL};

static const Effect_Interval menu_int3 = {.sec = 0.3f, .led1_state = GRN, .led2_state = GRN};

static const Effect_Interval menu_int4 = {.sec = 0.3f, .led1_state = LBL, .led2_state = LBL};

static const Effect_Interval menu_int5 = {.sec = 0.3f, .led1_state = BLU, .led2_state = BLU};

const Effect menu = {.duration = 1.5f,
                     .n_intervals = 5,
                     .intervals = {menu_int1, menu_int2, menu_int3, menu_int4, menu_int5},
                     .type = MENU};