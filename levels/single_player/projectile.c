#include "projectile.h"

static int get_closest_enemy(Enemy* enemies, Player* player, float* len)
{
    float best_dist = -1;
    int idx = 0;
    float dist;
    float dx, dy;
    for (int i = 0; i < MAX_ENEMY_COUNT; i++) {
        if (enemies[i].active) {
            dx = enemies[i].x - player->x;
            dy = enemies[i].y - player->y;
            dist = sqrt(dx * dx + dy * dy);
            if (best_dist == -1 || dist < best_dist) {
                best_dist = dist;
                idx = i;
            }
        }
    }
    (*len) = best_dist;
    return idx;
}

void update_projectile(Projectile* projectile, float dt)
{
    projectile->x += projectile->vx * dt;
    projectile->y += projectile->vy * dt;
    // despawn if it is outside the world border
    if (projectile->x >= WORLD_WIDTH || projectile->x <= 0 || projectile->y >= WORLD_HEIGHT || projectile->y <= 0)
        projectile->active = 0;
}

void spawn_projectile(Projectile* projectiles, Enemy* enemies, Player* player, int* projectile_count)
{
    int i = 0;
    while (i < MAX_PROJECTILE_COUNT && projectiles[i].active)
        i++;
    projectiles[i].active = 1;
    projectiles[i].x = player->x + PLAYER_WIDTH / 2;
    projectiles[i].y = player->y + PLAYER_HEIGHT / 2;
    float len;
    int closest_idx = get_closest_enemy(enemies, player, &len);
    float dirx = (enemies[closest_idx].x - player->x) / len;
    float diry = (enemies[closest_idx].y - player->y) / len;
    projectiles[i].vx = dirx * PROJECTILE_SPEED;
    projectiles[i].vy = diry * PROJECTILE_SPEED;
    projectiles[i].damage = player->damage;
    (*projectile_count)++;
}