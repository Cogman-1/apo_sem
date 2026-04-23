#ifndef APO_START_MENU_H
#define APO_START_MENU_H

#include "mzapo_lib/mzapo.h"

enum { SINGLE_PLAYER = 0, MULTI_PLAYER, SETTINGS, CONTROLS, EXIT_GAME };

int start_menu(mzapo_state* state);

#endif // APO_START_MENU_H
