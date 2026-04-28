#ifndef APO_SEM_ENEMY_H
#define APO_SEM_ENEMY_H

#include "camera.h"
#include "const.h"
#include "player.h"

typedef struct {
    char active;
    float x, y;
    float vx, vy;
    short health;
} Enemy;

void update_enemy(Enemy* enemy, Player* player, float dt);

void spawn_enemy(Enemy* enemies, Camera* cam, int* enemy_count);
#endif // APO_SEM_ENEMY_H
