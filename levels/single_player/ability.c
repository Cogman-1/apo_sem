
#include "single_player.h"
#include "player.h"
#include "../../sprites/sprites.h"


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


void initialize_abilities(Player* player) {
    player->activeAbilities[0] = (ActiveAbility){(Ability){damage_all, 20.0f, &get_arrow_sprites()[0]}, time_ms() / 1000 +20.0f};
    player->activeAbilities[1] = (ActiveAbility){(Ability){damage_all, 10.0f, &get_arrow_sprites()[0]}, time_ms() / 1000 +10.0f};
    player->selected_active_ability = 0; 
}

void damage_all(void* gs) {
    GameState* game_state = (GameState*)gs;
    for (int i = 0; i < game_state->enemy_count; i++) {
        game_state->enemies[i].health -= 5;
    }
}