#include "player.h"
#include "../../knobs.h"
#include "ability.h"
#include "enemy.h"
#include <math.h>

static void knockback_player(Player* player, Enemy* enemy)
{
    int dx = player->x - enemy->x;
    int dy = player->y - enemy->y;
    float length = sqrtf(dx * dx + dy * dy);
    // prevent division by zero
    if (length > 0.001f) {
        player->kvx = (dx / length) * PLAYER_KNOCKBACK_GAIN;
        player->kvy = (dy / length) * PLAYER_KNOCKBACK_GAIN;
        player->stun_frames = PLAYER_STUN_GAIN;
    }
}

void initialize_player(Player* player)
{
    player->x = (float)WORLD_WIDTH / 2;
    player->y = (float)WORLD_HEIGHT / 2;
    player->vx = 0;
    player->vy = 0;
    player->kvx = 0;
    player->kvy = 0;
    player->maxHealth = PLAYER_START_MAX_HEALTH;
    player->health = player->maxHealth;
    player->damage = PLAYER_START_DAMAGE;
    player->fireCooldown = PLAYER_START_FIRE_COOLDOWN;
    player->playerScore = 0;
    player->invincible_frames = 0;
    player->stun_frames = 0;
    initialize_abilities(player);
}

void update_player(Player* player, knob_directions inputs, float dt)
{
    if (player->stun_frames <= 0) {
        // 1. Handle inputs
        float dirx = inputs.rx;
        float diry = inputs.ry;
        // 2. Update physics
        // update velocity
        float new_vx = player->vx + PLAYER_ACCELERATION_MOD * dirx * dt;
        float new_vy = player->vy + PLAYER_ACCELERATION_MOD * diry * dt;
        // clamp velocity
        if (fabsf(new_vx) > PLAYER_MAX_SPEED)
            new_vx = (new_vx > 0) ? PLAYER_MAX_SPEED : -PLAYER_MAX_SPEED;
        if (fabsf(new_vy) > PLAYER_MAX_SPEED)
            new_vy = (new_vy > 0) ? PLAYER_MAX_SPEED : -PLAYER_MAX_SPEED;
        // assign velocity to player
        player->vx = new_vx;
        player->vy = new_vy;
        // knockback friction
        player->kvx *= PLAYER_KNOCKBACK_FRICTION;
        player->kvy *= PLAYER_KNOCKBACK_FRICTION;
        if (fabsf(player->kvx) < 0.001f)
            player->kvx = 0;
        if (fabsf(player->kvy) < 0.001f)
            player->kvy = 0;
        // calculate new position
        float new_x = player->x + (player->vx + player->kvx) * dt;
        float new_y = player->y + (player->vy + player->kvy) * dt;
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

    // invincibility decay
    if (player->invincible_frames > 0) {
        player->invincible_frames--;
    }
    // stun decay
    if (player->stun_frames > 0) {
        player->stun_frames--;
    }
}

void take_damage(Player* player, Enemy* enemy)
{
    if (player->invincible_frames <= 0) {
        if (player->health >= 0)
            player->health -= ENEMY_DAMAGE;
        // set the LED_RGB Effect
        set_effect(DAMAGE);
        player->invincible_frames = PLAYER_INVINCIBLE_GAIN;
        knockback_player(player, enemy);
    }
}

void draw_player(Player* player, Camera* camera, const Sprite* sprite, lcdpixel* fb)
{
    Vertex_2D player_center = {0};
    player_center.x = (int)(player->x - camera->x) + PLAYER_WIDTH / 2;
    player_center.y = (int)(player->y - camera->y) + PLAYER_HEIGHT / 2;
    draw_sprite_centered(fb, player_center, sprite);
}
