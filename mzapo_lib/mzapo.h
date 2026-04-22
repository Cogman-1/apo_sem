#ifndef MZAPO_H
#define MZAPO_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320

typedef struct mzapo_state {
    char* spiled_reg_base;
    char* parlcd_reg_base;
    uint8_t calibrated_knobs[3];
} mzapo_state;

typedef union ledrgb {
    uint32_t raw;
    struct {
        unsigned b : 8;
        unsigned g : 8;
        unsigned r : 8;
    };
} ledrgb;

typedef union {
    uint32_t raw;
    struct {
        unsigned b : 8;
        unsigned g : 8;
        unsigned r : 8;
        bool bdown : 1;
        bool gdown : 1;
        bool rdown : 1;
    };
} knobs;

typedef union lcdpixel {
    uint16_t raw;
    struct {
        unsigned b : 5;
        unsigned g : 6;
        unsigned r : 5;
    };
} lcdpixel;

// All functions now take a pointer to mzapo_state
mzapo_state* init_mzapo();
uint32_t ledline_read(mzapo_state* state);
void ledline_write(mzapo_state* state, uint32_t value);
void ledrgb_write(mzapo_state* state, int side, ledrgb value);
knobs knobs_read(mzapo_state* state);
void parlcd_write_screen(mzapo_state* state, const lcdpixel* buffer);
void buffer_write_text(lcdpixel* buffer, size_t startx, size_t starty, char* text, lcdpixel color);

#endif