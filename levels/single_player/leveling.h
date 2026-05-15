#ifndef APO_SEM_LEVELING_H
#define APO_SEM_LEVELING_H

#include "../../Draw_lib/draw.h"
#include "../../Draw_lib/effect.h"
#include "../../Draw_lib/text.h"

enum { L_DAMAGE = 0, L_HEALTH, L_FIRERATE, L_ABILITY_COOLDOWN, L_MOVE_SPEED, L_HEAL };

typedef struct Player Player;

void level(mzapo_state* state, lcdpixel* fb, Player* player);

#endif // APO_SEM_LEVELING_H
