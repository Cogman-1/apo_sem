
#include "single_player.h"
#include "player.h"
#include "../../sprites/sprites.h"

void initialize_abilities(Player* player) {
    player->activeAbilities = {
        {(Ability){damage_all, 20.0f, get_arrow_sprites()[0]}, .0f}
    };
}

void damage_all(GameState* game_state) {
    for (int i = 0; i < game_state->enemy_count; i++) {
        game_state.enemies[i].health -= 5;
    }
}