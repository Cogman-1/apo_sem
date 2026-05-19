
#include "../../sprites/sprites.h"
#include "../../LEDRGB_Effects/effects.h"
#include "const.h"
#include "player.h"
#include "single_player.h"
#include "enemy.h"

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

static uint64_t time_ms()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

void spawn_many_projectiles(void* gs)
{
    GameState* game_state = (GameState*)gs;
    for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
            size_t idx = spawn_projectile(game_state->projectiles, game_state->enemies, &game_state->player,
                                          &game_state->projectile_count);
            game_state->projectiles[idx].x += x * 32;
            game_state->projectiles[idx].y += y * 32;
            game_state->projectiles[idx].vx -= x * 4;
            game_state->projectiles[idx].vy -= x * 4;
        }
    }
}

void damage_all(void* gs)
{
    GameState* game_state = (GameState*)gs;
    for (int i = 0; i < MAX_ENEMY_COUNT; i++) {
        if (game_state->enemies[i].active)
        {
            game_state->enemies[i].health -= 5;
            deal_knockback_enemy(&game_state->enemies[i], (Vertex_2D){game_state->player.x, game_state->player.y}, 16.0);
        }
    }
    set_effect(ABILITY);
}

void initialize_abilities(Player* player)
{
    player->activeAbilities[0] =
        (ActiveAbility){(Ability){spawn_many_projectiles, 20.0f, &get_arrow_sprites()[0]}, time_ms() / 1000 + 5.0f};
    player->activeAbilities[1] =
        (ActiveAbility){(Ability){damage_all, 10.0f, &get_explosion_icon_sprite()[0]}, time_ms() / 1000 + 2.5f};
    player->selected_active_ability = 0;
}
