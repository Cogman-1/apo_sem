#include "player.h"
#include "../../knobs.h"
#include <math.h>

void initialize_player(Player* player)
{
    player->x = (float)WORLD_WIDTH / 2;
    player->y = (float)WORLD_HEIGHT / 2;
    player->vx = 0;
    player->vy = 0;
    player->maxHealth = PLAYER_START_MAX_HEALTH;
    player->health = player->maxHealth;
    player->damage = PLAYER_START_DAMAGE;
    player->fireCooldown = PLAYER_START_FIRE_COOLDOWN;
    player->playerScore = 0;
    player->invincible_frames = 0;
}

void update_player(Player* player, knobs inputs, float dt_msec)
{
    // 1. Handle inputs
    knobs_state knobstate;
    knobs_state_init(&knobstate, (knobs){.r = 0, .g = 0, .b = 0});
    knobs_update(&knobstate, inputs);
    knob_directions directions = calculate_direction(&knobstate);
    float dirx = directions.rx;
    float diry = directions.ry;
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

    //invincibility decay
    if (player->invincible_frames > 0) {
        player->invincible_frames--;
    }
}

void take_damage(Player* player)
{
    if (player->invincible_frames <= 0) {
        if (player->health >= 0)
            player->health -= ENEMY_DAMAGE;
        // set the LED_RGB Effect
        set_effect(DAMAGE);
        player->invincible_frames = PLAYER_INVINCIBLE_GAIN;
    }
}

void draw_player(Player* player, Camera* camera, const Sprite* sprite, lcdpixel* fb)
{
    Vertex_2D player_top_left = {0};
    player_top_left.x = (int)(player->x - camera->x);
    player_top_left.y = (int)(player->y - camera->y);

    Vertex_2D player_bottom_right = {0};
    player_bottom_right.x = player_top_left.x + PLAYER_WIDTH;
    player_bottom_right.y = player_top_left.y + PLAYER_HEIGHT;

    draw_rectangle(fb, player_top_left, player_bottom_right, (lcdpixel){.raw = GRAY});

    Vertex_2D player_center = {0};
    player_center.x = (int)(player->x - camera->x) + PLAYER_WIDTH / 2;
    player_center.y = (int)(player->y - camera->y) + PLAYER_HEIGHT / 2;
    draw_sprite_centered(fb, player_center, sprite);
}