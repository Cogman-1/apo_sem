#ifndef APO_SEM_PROJECTILE_H
#define APO_SEM_PROJECTILE_H

#include "../../mzapo_lib/mzapo.h"
#include "const.h"
#include "enemy.h"
#include "player.h"
typedef struct {
    char active;
    float x, y;
    float vx, vy;
    int damage;
} Projectile;

void update_projectile(Projectile* projectile, Camera* camera, int* projectile_count, float dt);

void spawn_projectile(Projectile* projectiles, Enemy* enemies, Player* player, int* projectile_count);

#endif // APO_SEM_PROJECTILE_H
