#ifndef APO_SEM_SETTINGS_H
#define APO_SEM_SETTINGS_H

#include "../mzapo_lib/mzapo.h"

typedef enum { EASY = 0, HARD } Difficulty;
typedef enum { DISPLAY = 0, LED, BOTH } HealthBarSwitch;

typedef struct {
    Difficulty diff;

    int UseLEDRGBEffects;

    HealthBarSwitch UseHealthBar;
    int UseLEDHealthbar;
    int UseScreenHealthbar;
} GameSettings;

GameSettings* get_settings();
void settings_menu(mzapo_state* state);

#endif // APO_SEM_SETTINGS_H
