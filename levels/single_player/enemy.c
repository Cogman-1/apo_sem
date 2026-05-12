#include "enemy.h"
#include "projectile.h"
#include <math.h>
#include <stdlib.h>

enum { EDGE_TOP, EDGE_LEFT, EDGE_BOTTOM, EDGE_RIGHT };

void initialize_enemy(Enemy* enemy)
{
    enemy->active = 0;
}

void update_enemy(Enemy* enemy, Player* player, int* enemy_count, float dt)
{
    // death
    if (enemy->health <= 0) {
        enemy->active = 0;
        player->playerScore += PLAYER_SCORE_GAIN;
        (*enemy_count)--;
        return;
    }
    // physics update
    if (enemy->stun_frames <= 0) {
        float dx = player->x - enemy->x;
        float dy = player->y - enemy->y;
        float length = sqrt(dx * dx + dy * dy);
        if (length > 0.001f) {
            enemy->vx = (dx / length) * ENEMY_SPEED;
            enemy->vy = (dy / length) * ENEMY_SPEED;
        }

        enemy->x += (enemy->vx + enemy->kvx) * dt;
        enemy->y += (enemy->vy + enemy->kvy) * dt;
    }
    // knockback friction
    enemy->kvx *= ENEMY_KNOCKBACK_FRICTION;
    enemy->kvy *= ENEMY_KNOCKBACK_FRICTION;
    if (fabsf(enemy->kvx) < 0.001f)
        enemy->kvx = 0;
    if (fabsf(enemy->kvy) < 0.001f)
        enemy->kvy = 0;
    // stun decay
    if (enemy->stun_frames >= 0) {
        enemy->stun_frames--;
    }
}

void spawn_enemy(Enemy* enemies, Camera* cam, int* enemy_count)
{
    int i = 0;
    while (i < MAX_ENEMY_COUNT && enemies[i].active)
        i++;
    if (i >= MAX_ENEMY_COUNT)
        return;
    enemies[i].active = 1;
    enemies[i].health = ENEMY_SPAWN_HEALTH;
    // generate random position to spawn the enemy
    int edge = rand() % 4;
    switch (edge) {
    case EDGE_TOP:
    case EDGE_BOTTOM:
        enemies[i].x = rand() % (SCREEN_WIDTH - ENEMY_WIDTH);
        enemies[i].y = (edge == EDGE_BOTTOM) ? SCREEN_HEIGHT : -ENEMY_HEIGHT;
        break;
    case EDGE_LEFT:
    case EDGE_RIGHT:
        enemies[i].y = rand() % (SCREEN_HEIGHT - ENEMY_HEIGHT);
        enemies[i].x = (edge == EDGE_RIGHT) ? SCREEN_WIDTH : -ENEMY_WIDTH;
        break;
    default:
        break;
    }
    enemies[i].x += cam->x;
    enemies[i].y += cam->y;
    enemies[i].vx = 0;
    enemies[i].vy = 0;
    enemies[i].kvx = 0;
    enemies[i].kvy = 0;
    enemies[i].stun_frames = 0;
    (*enemy_count)++;
}

void draw_enemy(Enemy* enemy, Camera* cam, const Sprite* sprite, lcdpixel* fb)
{
    Vertex_2D enemy_center = {0};
    enemy_center.x = (int)(enemy->x - cam->x) + ENEMY_WIDTH / 2;
    enemy_center.y = (int)(enemy->y - cam->y) + ENEMY_HEIGHT / 2;
    draw_sprite_centered(fb, enemy_center, sprite);
}

void deal_knockback_enemy(Enemy* enemy, Vertex_2D from)
{
    int dx = enemy->x - from.x;
    int dy = enemy->y - from.y;
    float length = sqrtf(dx * dx + dy * dy);
    // prevent division by zero
    if (length > 0.001f) {
        enemy->kvx = (dx / length) * ENEMY_KNOCKBACK_GAIN;
        enemy->kvy = (dy / length) * ENEMY_KNOCKBACK_GAIN;
        enemy->stun_frames = ENEMY_STUN_GAIN;
    }
}

void deal_damage_enemy(Enemy* enemy, Projectile* projectile)
{
    enemy->health -= projectile->damage;
    deal_knockback_enemy(enemy, (Vertex_2D){projectile->x, projectile->y});
}