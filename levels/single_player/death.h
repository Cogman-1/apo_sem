#ifndef APO_SEM_DEATH_H
#define APO_SEM_DEATH_H
#include "../../LEDRGB_Effects/effects.h"
#include "../../mzapo_lib/mzapo.h"

#include "player.h"

enum { EXIT = 0, RESTART = 1 };

int death(Player* player, lcdpixel* fb, mzapo_state* state);

#endif // APO_SEM_DEATH_H
