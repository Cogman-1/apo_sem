#ifndef APO_SEM_SETTINGS_H
#define APO_SEM_SETTINGS_H

#include "../mzapo_lib/mzapo.h"


typedef enum {EASY=0, HARD=1} Difficulty;

typedef struct {
    Difficulty diff;

    int UseLEDRGBEffects;
    int UseLEDHealthbar;
    int UseScreenHealthbar;
}GameSettings;

GameSettings* get_settings();
void settings_menu();

#endif // APO_SEM_SETTINGS_H
