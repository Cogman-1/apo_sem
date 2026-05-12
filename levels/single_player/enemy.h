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

void initialize_enemy(Enemy* enemy);
void update_enemy(Enemy* enemy, Player* player, int* enemy_count, float dt);
void spawn_enemy(Enemy* enemies, Camera* cam, int* enemy_count);
void draw_enemy(Enemy* enemy, Camera* cam, const Sprite* sprite, lcdpixel* fb);

#endif // APO_SEM_ENEMY_H
