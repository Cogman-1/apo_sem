#ifndef APO_SEM_PLAYER_H
#define APO_SEM_PLAYER_H

#include "../../Draw_lib/draw.h"
#include "../../LEDRGB_Effects/effects.h"
#include "../../knobs.h"
#include "../../mzapo_lib/mzapo.h"
#include "ability.h"
#include "camera.h"
#include "const.h"

typedef struct Enemy Enemy;

typedef struct Player {
    float x, y;     // current pos
    float vx, vy;   // native vel
    float kvx, kvy; // knockback vel
    int playerScore;
    int level;
    int maxHealth;
    int health;
    float damage;
    float fireCooldown;
    int invincible_frames;
    int stun_frames;
    float maxSpeed;
    int requiredScore;
    ActiveAbility activeAbilities[2];
    int selected_active_ability;
} Player;

void initialize_player(Player* player);
void update_player(Player* player, knob_directions inputs, float dt);
void take_damage(Player* player, Enemy* enemy);
void draw_player(Player* player, Camera* camera, const Sprite* sprite, lcdpixel* fb);
void level_up(Player* player, int stat);

#endif // APO_SEM_PLAYER_H
