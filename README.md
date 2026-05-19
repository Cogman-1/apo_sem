# apo_sem

## Make

Project can be built with `make` and ran directly on mzapo with `TARGET_IP=192.168.xxx.xxx make run`

To emulate mzapo with SDL, run `make sdl`.

Before commiting your changes, make sure to format with `make format` and check that your code compiles with `make clean && make`.

## Architecture

### Menu

Every file or folder in `levels/single_player` represents an entry in the main menu.

### Game

The main game loop is defined in `levels/single_player/single_player.c`


Active abilities are defined in `levels/single_player/ability.c`

Camera movement in defined in `levels/single_player/camera.c`

The constants such as player speed, enemy damage, etc. are defined in `levels/single_player/enemy.c`

The death screen is defined in `levels/single_player/death.c`

Leveling is defined in `levels/single_player/leveling.c`

The pause screen is defined in `levels/single_player/pause.c`

PLayer damage, drawing, state and leveling is defined in `levels/single_player/player.c`

Projectile spawning and drawing is defined in `levels/single_player/projectile.c`

UI such as healthbar, score, levels and abilities are done in `levels/single_player/sp_UI.c`

### MZAPO Lib

`mzapo_lib/mzapo.c` contains the main abstractions including reading the knob state and writing to LED strips and side RGB LEDs. `sdl.c` serves as a drop-in replacement for `mzapo_lib/mzapo.c` to run the game in a mzapo emulator. 

LED RGB effects are defined in `LEDRGB_Effects/effects.c` and set globally with `set_effect`. It calls back to the mzapo abstraction layer each tick.

The LED strip gets interacted with in the main game loop.

The knob controller is defined in `knobs.c`.

### Sprites

Sprites are stored as c source files in `sprites` as exported by GIMP and defined in `sprites/sprites.c`

They can then be drawn using `draw_sprite` in `draw_lib/draw.c`

### Drawing

Basic shape and sprite drawing is in `draw_lib/draw.c`

Text drawing is defined in `draw_lib/text.c` and fonts are defined in `draw_lib/font_*.c`

Effects (currently unused) are defined in `draw_lib/effect.c`
