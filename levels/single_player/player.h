#ifndef APO_SEM_PLAYER_H
#define APO_SEM_PLAYER_H

#include <math.h>

#include "../../mzapo_lib/mzapo.h"
#include "const.h"

typedef struct {
    float x, y;
    float vx, vy;
    int playerScore;
    short maxHealth;
    short health;
    int damage;
    float fireCooldown;
} Player;

void update_player(Player* player, knobs inputs, float dt_msec);
void take_damage(Player* player);

#endif // APO_SEM_PLAYER_H
