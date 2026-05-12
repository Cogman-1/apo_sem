#ifndef APO_SEM_PAUSE_H
#define APO_SEM_PAUSE_H

#include "../../Draw_lib/draw.h"
#include "../../Draw_lib/effect.h"
#include "../../Draw_lib/text.h"
#include "../../mzapo_lib/mzapo.h"
#include "../levels.h"

enum { P_RESUME = 0, P_SETTINGS, P_CONTROLS, P_EXIT_MAIN_MENU };

int pause(mzapo_state* state, lcdpixel* fb);

#endif // APO_SEM_PAUSE_H
