#ifndef APO_SEM_ENEMY_H
#define APO_SEM_ENEMY_H

#include "camera.h"
#include "const.h"
#include "player.h"

typedef struct Projectile Projectile;

typedef struct Enemy {
    char active;
    float x, y;
    float vx, vy;
    float kvx, kvy;
    short health;
    int stun_frames;
} Enemy;

void initialize_enemy(Enemy* enemy);
void update_enemy(Enemy* enemy, Player* player, int* enemy_count, float dt);
void spawn_enemy(Enemy* enemies, Player* player, Camera* cam, int* enemy_count);
void draw_enemy(Enemy* enemy, Camera* cam, const Sprite* sprite, lcdpixel* fb);
void deal_damage_enemy(Enemy* enemy, Projectile* projectile);
void deal_knockback_enemy(Enemy* enemy, Vertex_2D from, float multiplier);

#endif // APO_SEM_ENEMY_H
