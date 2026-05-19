#include "player.h"
#include "../../knobs.h"
#include "../settings.h"
#include "ability.h"
#include "enemy.h"
#include "leveling.h"
#include <math.h>

static float approach_float(float current, float target, float max_delta)
{
    if (current < target) {
        current += max_delta;
        if (current > target)
            current = target;
    } else if (current > target) {
        current -= max_delta;
        if (current < target)
            current = target;
    }

    return current;
}

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
    GameSettings* settings = get_settings();
    player->x = (float)WORLD_WIDTH / 2;
    player->y = (float)WORLD_HEIGHT / 2;
    player->vx = 0;
    player->vy = 0;
    player->kvx = 0;
    player->kvy = 0;
    player->maxHealth = PLAYER_START_MAX_HEALTH;
    player->health = player->maxHealth;
    player->damage = (float)PLAYER_START_DAMAGE;
    player->fireCooldown = (float)PLAYER_START_FIRE_COOLDOWN;
    player->maxSpeed = PLAYER_MAX_SPEED;
    player->playerScore = 0;
    player->invincible_frames = 0;
    player->stun_frames = 0;
    player->level = 0;
    player->requiredScore = PLAYER_BASE_XP_NEEDED;
    if (settings->diff == EASY) {
        player->maxHealth += EASY_DIFF_START_HEALTH_INCREASE;
        player->damage += EASY_DIFF_PLAYER_DAMAGE_INCREASE;
    }

    initialize_abilities(player);
    player->nextDashTime = 0;
}

void update_player(Player* player, knob_directions inputs, float dt)
{
    if (player->stun_frames <= 0) {
        float target_vx = inputs.rx * player->maxSpeed;
        float target_vy = inputs.ry * player->maxSpeed;

        float max_delta = PLAYER_ACCELERATION_MOD * dt;

        player->vx = approach_float(player->vx, target_vx, max_delta);
        player->vy = approach_float(player->vy, target_vy, max_delta);

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
    GameSettings* settings = get_settings();
    if (player->invincible_frames <= 0) {
        if (player->health >= 0) {
            if (settings->diff == EASY)
                player->health += EASY_DIFF_ENEMY_DMG_DECREASE;
            player->health -= ENEMY_DAMAGE + ENEMY_DAMAGE_SCALING_FACTOR * player->level;
        }
        // set the LED_RGB Effect
        set_effect(DAMAGE);
        player->invincible_frames = PLAYER_INVINCIBLE_GAIN;
        if (settings->diff == EASY)
            player->invincible_frames += EASY_DIFF_PLAYER_INVINCIBILITY_INCREASE;
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

void level_up(Player* player, int stat)
{
    switch (stat) {
    case L_DAMAGE:
        player->damage *= 1.1f;
        break;
    case L_HEALTH:
        player->maxHealth *= 1.1f;
        player->health *= 1.1f;
        break;
    case L_FIRERATE:
        player->fireCooldown *= 0.9f;
        break;
    case L_ABILITY_COOLDOWN:
        for (int i = 0; i < ACTIVE_ABIlITY_COUNT; i++) {
            player->activeAbilities[i].ability.cooldown *= 0.9f;
        }
        break;
    case L_MOVE_SPEED:
        player->maxSpeed *= 1.1f;
        break;
    case L_HEAL:
        player->health = player->maxHealth;
        break;
    default:
        break;
    }
    player->level++;
    // scale the required XP proportionally to player level
    player->requiredScore += PLAYER_NEEDED_XP_GAIN * 0.5f * player->level;
}