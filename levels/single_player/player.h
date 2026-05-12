#ifndef APO_SEM_PLAYER_H
#define APO_SEM_PLAYER_H

#include "../../Draw_lib/draw.h"
#include "../../LEDRGB_Effects/effects.h"
#include "../../mzapo_lib/mzapo.h"
#include "../../LEDRGB_Effects/effects.h"
#include "camera.h"
#include "const.h"
#include "ability.h"

typedef struct Player {
    float x, y;
    float vx, vy;
    int playerScore;
    short maxHealth;
    short health;
    int damage;
    float fireCooldown;
    int invincible_frames;
    ActiveAbility activeAbilities[1];
} Player;

void initialize_player(Player* player);
void update_player(Player* player, knobs inputs, float dt_msec);
void take_damage(Player* player);
void draw_player(Player* player, Camera* camera, const Sprite* sprite, lcdpixel* fb);

#endif // APO_SEM_PLAYER_H
