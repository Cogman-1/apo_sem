#ifndef APO_SEM_PROJECTILE_H
#define APO_SEM_PROJECTILE_H

#include "const.h"
#include "enemy.h"
#include "player.h"

typedef struct Projectile{
    char active;
    float x, y;
    float vx, vy;
    int damage;
} Projectile;

void initialize_projectile(Projectile* projectile);

void update_projectile(Projectile* projectile, Camera* camera, int* projectile_count, float dt);

size_t spawn_projectile(Projectile* projectiles, Enemy* enemies, Player* player, int* projectile_count);

void draw_projectile(Projectile* projectile, Camera* camera, lcdpixel* fb, Sprite* sprites);

#endif // APO_SEM_PROJECTILE_H
