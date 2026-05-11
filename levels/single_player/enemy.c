#include "enemy.h"
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
    }
    // physics update
    float dx = player->x - enemy->x;
    float dy = player->y - enemy->y;
    float length = sqrt(dx * dx + dy * dy);
    if (length > 0.001f) {
        enemy->vx = (dx / length) * ENEMY_SPEED;
        enemy->vy = (dy / length) * ENEMY_SPEED;
    }

    enemy->x += enemy->vx * dt;
    enemy->y += enemy->vy * dt;
}

void spawn_enemy(Enemy* enemies, Camera* cam, int* enemy_count)
{
    int i = 0;
    while (i < MAX_ENEMY_COUNT && enemies[i].active)
        i++;
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
    (*enemy_count)++;
}

void draw_enemy(Enemy* enemy, Camera* cam, lcdpixel* fb)
{
    Vertex_2D top_left;
    top_left.x = enemy->x - cam->x;
    top_left.y = enemy->y - cam->y;
    Vertex_2D bot_right;
    bot_right.x = top_left.x + ENEMY_WIDTH;
    bot_right.y = top_left.y + ENEMY_HEIGHT;
    lcdpixel enemy_color;
    enemy_color.raw = ENEMY_COLOR;
    draw_rectangle(fb, top_left, bot_right, enemy_color);
}