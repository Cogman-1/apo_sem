#ifndef APO_SEM_EFFECTS_H
#define APO_SEM_EFFECTS_H
#include <inttypes.h>

#include "../mzapo_lib/mzapo.h"

#define MAX_INTERVALS 20

typedef enum { NONE = -1, DAMAGE = 0 } Effect_Type;

typedef struct {
    float sec;
    uint32_t led1_state;
    uint32_t led2_state;
} Effect_Interval;

typedef struct {
    Effect_Interval intervals[MAX_INTERVALS];
    int n_intervals;
    float duration;
    Effect_Type type;
} Effect;

extern const Effect damage;

void set_effect(Effect_Type type);
void update_effect(mzapo_state* hw_state, float dt);

#endif // APO_SEM_EFFECTS_H
