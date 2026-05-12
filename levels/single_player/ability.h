#pragma once

typedef struct Ability {
    void (*effect)(void*);
    float cooldown;
    Sprite* sprite;
} Ability;

typedef struct ActiveAbility {
    Ability ability;
    float cooldown_end;
} ActiveAbility;

typedef struct Player Player;

void initialize_abilities(Player* player);
void damage_all(void* game_state);
