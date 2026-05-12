#define _POSIX_C_SOURCE 200112L
#include "levels/levels.h"
#include "mzapo_lib/mzapo.h"
#include "mzapo_lib/serialize_lock.h"
#include "sprites/sprites.h"
#include "start_menu.h"

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;
    sprite_init();
    mzapo_state* state = init_mzapo();
    int exit = 0;
    int restart = 0;
    while (!exit) {
        int selected = start_menu(state);
        switch (selected) {
        case SINGLE_PLAYER:
            restart = single_player(state);
            while (restart) {
                restart = single_player(state);
            }
            break;
        case SETTINGS:
            settings();
            break;
        case MULTI_PLAYER:
            multi_player();
            break;
        case CONTROLS:
            controls();
            break;
        case EXIT_GAME:
            exit = 1;
            break;
        default:
            continue;
        }
    }
    lcdpixel fb[SCREEN_WIDTH * SCREEN_HEIGHT];
    clear_display(fb);
    parlcd_write_screen(state, fb);
#ifndef SDL
    serialize_unlock();
#endif
    return 0;
}
