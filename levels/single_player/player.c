#include "player.h"

void update_player(Player* player, knobs inputs, float dt_msec)
{
    // 1. Handle inputs
    float dirx, diry;
    calculate_direction(inputs, &dirx, &diry);
    // 2. Update physics
    // update velocity
    float new_vx = player->vx + PLAYER_ACCELERATION_MOD * dirx * dt_msec;
    float new_vy = player->vy + PLAYER_ACCELERATION_MOD * diry * dt_msec;
    // clamp velocity
    if (fabsf(new_vx) > PLAYER_MAX_SPEED)
        new_vx = (new_vx > 0) ? PLAYER_MAX_SPEED : -PLAYER_MAX_SPEED;
    if (fabsf(new_vy) > PLAYER_MAX_SPEED)
        new_vy = (new_vy > 0) ? PLAYER_MAX_SPEED : -PLAYER_MAX_SPEED;
    // assign velocity to player
    player->vx = new_vx;
    player->vy = new_vy;
    // calculate new position
    float new_x = player->x + player->vx * dt_msec;
    float new_y = player->y + player->vy * dt_msec;
    if (new_x < 0)
        new_x = 0;
    if (new_x > (WORLD_WIDTH - PLAYER_WIDTH))
        new_x = (WORLD_WIDTH - PLAYER_WIDTH);
    if (new_y < 0)
        new_y = 0;
    if (new_y > (WORLD_HEIGHT - PLAYER_HEIGHT))
        new_y = (WORLD_HEIGHT - PLAYER_HEIGHT);
    player->x = new_x;
    player->y = new_y;
}

void take_damage(Player* player)
{
    player->health -= ENEMY_DAMAGE;
}