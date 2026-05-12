#include "projectile.h"
#include <math.h>

void initialize_projectile(Projectile* projectile)
{
    projectile->active = 0;
}

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
            dist = dx * dx + dy * dy;
            if (best_dist == -1 || dist < best_dist) {
                best_dist = dist;
                idx = i;
            }
        }
    }
    (*len) = sqrtf(best_dist);
    return idx;
}

void update_projectile(Projectile* projectile, Camera* camera, int* projectile_count, float dt)
{
    projectile->x += projectile->vx * dt;
    projectile->y += projectile->vy * dt;
    // despawn if it is outside the screen
    if (projectile->x >= camera->x + SCREEN_WIDTH || projectile->x <= camera->x ||
        projectile->y >= camera->y + SCREEN_HEIGHT || projectile->y <= camera->y) {
        projectile->active = 0;
        (*projectile_count)--;
    }
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
    if (closest_idx < 0 || len <= 0.001f) {
        projectiles[i].active = 0;
        return;
    }
    float dirx = (enemies[closest_idx].x - player->x) / len;
    float diry = (enemies[closest_idx].y - player->y) / len;
    projectiles[i].vx = dirx * PROJECTILE_SPEED;
    projectiles[i].vy = diry * PROJECTILE_SPEED;
    projectiles[i].damage = player->damage;
    (*projectile_count)++;
}

void draw_projectile(Projectile* projectile, Camera* camera, lcdpixel* fb)
{
    int cx = (int)projectile->x + PROJECTILE_WIDTH / 2 - camera->x;
    int cy = (int)projectile->y + PROJECTILE_HEIGHT / 2 - camera->y;
    draw_circle(fb, (Vertex_2D){cx, cy}, PROJECTILE_HEIGHT / 2, (lcdpixel){.raw = PROJECTILE_COLOR});
}