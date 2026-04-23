#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320

#define WINDOW_WIDTH (SCREEN_WIDTH + 100)
#define WINDOW_HEIGHT (SCREEN_HEIGHT + 80)

typedef struct mzapo_state {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* lcd_texture;

    uint32_t ledline;
    uint32_t rgb_left;
    uint32_t rgb_right;

    uint8_t knobs_val[3];
    bool knobs_pressed[3];

} mzapo_state;

typedef union ledrgb {
    uint32_t raw;
    struct {
        unsigned b : 8;
        unsigned g : 8;
        unsigned r : 8;
    };
} ledrgb;

typedef union knobs {
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

static uint32_t rgb565_to_888(lcdpixel p)
{
    uint8_t r = (p.r * 255) / 31;
    uint8_t g = (p.g * 255) / 63;
    uint8_t b = (p.b * 255) / 31;

    return (r << 16) | (g << 8) | b;
}

static void handle_input(mzapo_state* s)
{
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT)
            exit(0);

        if (e.type == SDL_KEYDOWN) {
            SDL_Keycode k = e.key.keysym.sym;

            // clockwise (789)
            if (k == SDLK_s)
                s->knobs_val[0]++;
            if (k == SDLK_d)
                s->knobs_val[1]++;
            if (k == SDLK_f)
                s->knobs_val[2]++;

            // counterclockwise (123)
            if (k == SDLK_j)
                s->knobs_val[0]--;
            if (k == SDLK_k)
                s->knobs_val[1]--;
            if (k == SDLK_l)
                s->knobs_val[2]--;

            // press (456)
            if (k == SDLK_b)
                s->knobs_pressed[0] = !s->knobs_pressed[0];
            if (k == SDLK_n)
                s->knobs_pressed[1] = !s->knobs_pressed[1];
            if (k == SDLK_m)
                s->knobs_pressed[2] = !s->knobs_pressed[2];
        }
    }
}

static void draw_ledline(mzapo_state* s)
{
    int start_x = (WINDOW_WIDTH - (32 * 6)) / 2;
    int y = SCREEN_HEIGHT + 20;

    for (int i = 0; i < 32; i++) {
        bool on = (s->ledline >> i) & 1;

        SDL_Rect r = {start_x + i * 6, y, 4, 4};

        if (on)
            SDL_SetRenderDrawColor(s->renderer, 255, 255, 0, 255);
        else
            SDL_SetRenderDrawColor(s->renderer, 50, 50, 50, 255);

        SDL_RenderFillRect(s->renderer, &r);
    }
}

static void draw_rgb_strip(mzapo_state* s, int side, uint32_t color)
{
    uint8_t r = (color >> 16) & 0xff;
    uint8_t g = (color >> 8) & 0xff;
    uint8_t b = color & 0xff;

    SDL_SetRenderDrawColor(s->renderer, r, g, b, 255);

    SDL_Rect rect;

    if (side == 1)
        rect = (SDL_Rect){0, 0, 40, SCREEN_HEIGHT};
    else
        rect = (SDL_Rect){50 + SCREEN_WIDTH + 10, 0, 40, SCREEN_HEIGHT};

    SDL_RenderFillRect(s->renderer, &rect);
}

static void render(mzapo_state* s)
{
    handle_input(s);

    SDL_SetRenderDrawColor(s->renderer, 0, 0, 0, 255);
    SDL_RenderClear(s->renderer);

    // LCD
    SDL_Rect lcd_rect = {50, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(s->renderer, s->lcd_texture, NULL, &lcd_rect);

    // RGB strips
    draw_rgb_strip(s, 1, s->rgb_left);
    draw_rgb_strip(s, 2, s->rgb_right);

    // LED line
    draw_ledline(s);

    SDL_RenderPresent(s->renderer);
}

mzapo_state* init_mzapo()
{
    SDL_Init(SDL_INIT_VIDEO);

    mzapo_state* s = calloc(1, sizeof(*s));

    s->window =
        SDL_CreateWindow("MZAPO SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    s->renderer = SDL_CreateRenderer(s->window, -1, SDL_RENDERER_ACCELERATED);

    s->lcd_texture = SDL_CreateTexture(s->renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH,
                                       SCREEN_HEIGHT);

    return s;
}

uint32_t ledline_read(mzapo_state* state)
{
    return state->ledline;
}

void ledline_write(mzapo_state* state, uint32_t value)
{
    state->ledline = value;
}

void ledrgb_write(mzapo_state* state, int side, ledrgb value)
{
    if (side == 1)
        state->rgb_left = value.raw;
    else
        state->rgb_right = value.raw;
}

knobs knobs_read(mzapo_state* state)
{
    knobs k = {0};

    k.r = state->knobs_val[0];
    k.g = state->knobs_val[1];
    k.b = state->knobs_val[2];

    k.rdown = state->knobs_pressed[0];
    k.gdown = state->knobs_pressed[1];
    k.bdown = state->knobs_pressed[2];

    return k;
}

void parlcd_write_screen(mzapo_state* state, const lcdpixel* buffer)
{
    uint32_t* pixels;
    int pitch;

    SDL_LockTexture(state->lcd_texture, NULL, (void**)&pixels, &pitch);

    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
        pixels[i] = rgb565_to_888(buffer[i]);

    SDL_UnlockTexture(state->lcd_texture);

    render(state);
}
