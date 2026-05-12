#include "single_player.h"


typedef struct Ability {
    void effect(GameState*);
    float cooldown;
    Sprite sprite;
} Ability;

typedef struct ActiveAbility {
    Ability ability;
    float cooldown_end;
} ActiveAbility;

void initialize_abilities(Player* player);
void damage_all(GameState* game_state);

