#ifndef APO_SEM_SINGLE_PLAYER_H
#define APO_SEM_SINGLE_PLAYER_H

// libraries for hardware interaction and graphics
#include "../../Draw_lib/draw.h"
#include "../../mzapo_lib/mzapo.h"

// files containing different objects in the game
#include "camera.h"
#include "const.h"
#include "enemy.h"
#include "pause.h"
#include "player.h"
#include "projectile.h"
#include "sp_UI.h"

void single_player(mzapo_state* game_state);

#endif // APO_SEM_SINGLE_PLAYER_H
