#ifndef APO_SEM_SINGLE_PLAYER_H
#define APO_SEM_SINGLE_PLAYER_H

// libraries for hardware interaction and graphics
#include "../../Draw_lib/draw.h"
#include "../../LEDRGB_Effects/effects.h"
#include "../../mzapo_lib/mzapo.h"
#include "../../sprites/sprites.h"

// files containing different objects in the game
#include "camera.h"
#include "const.h"
#include "enemy.h"
#include "pause.h"
#include "player.h"
#include "projectile.h"
#include "sp_UI.h"
#include "death.h"

typedef struct GameState {
    Player player;
    Camera cam;
    int enemy_count;
    Enemy enemies[MAX_ENEMY_COUNT];
    SpriteAnimator enemy_animators[MAX_ENEMY_COUNT];
    int projectile_count;
    Projectile projectiles[MAX_PROJECTILE_COUNT];
    SpriteAnimator player_animator;
    uint32_t gameSeed;
    float enemy_spawn_timer;
    float shoot_cooldown_timer;
} GameState;

int single_player(mzapo_state* game_state);

#endif // APO_SEM_SINGLE_PLAYER_H
