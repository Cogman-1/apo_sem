#include "sp_UI.h"
#include "../../Draw_lib/effect.h"
#include "single_player.h"

#include <stdio.h>
#include <string.h>

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

static uint64_t time_ms()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

static void draw_healthbar(mzapo_state* state, lcdpixel* fb, Player* player)
{
    // LCD health bar
    lcdpixel color = {HEALTH_BAR_BACKGROUND};
    // draw base
    draw_rectangle(fb, (Vertex_2D){HEALTH_BAR_X, HEALTH_BAR_Y}, (Vertex_2D){HEALTH_BAR_ENDX, HEALTH_BAR_ENDY}, color);
    // draw red background
    color.raw = HEALTH_BAR_FOREGROUND_ONE;
    draw_rectangle(fb, (Vertex_2D){HEALTH_BAR_X + HEALTH_BAR_SPACING, HEALTH_BAR_Y + HEALTH_BAR_SPACING},
                   (Vertex_2D){HEALTH_BAR_ENDX - HEALTH_BAR_SPACING, HEALTH_BAR_ENDY - HEALTH_BAR_SPACING}, color);
    // calculate and draw green part
    color.raw = HEALTH_BAR_FOREGROUND_TWO;
    int health_pixels = (((float)player->health) / (player->maxHealth)) * (HEALTH_BAR_WIDTH - 2 * HEALTH_BAR_SPACING);
    draw_rectangle(fb, (Vertex_2D){HEALTH_BAR_X + HEALTH_BAR_SPACING, HEALTH_BAR_Y + HEALTH_BAR_SPACING},
                   (Vertex_2D){HEALTH_BAR_X + HEALTH_BAR_SPACING + health_pixels, HEALTH_BAR_ENDY - HEALTH_BAR_SPACING},
                   color);

    // LED health bar
    int n = ((float)player->health) / (player->maxHealth) * 32;
    uint32_t leds;
    if (n < 0)
        n = 0; // clamp to only non-negative values
    if (n < 32) {
        leds = (1u << n) - 1u;
    } else {
        leds = UINT32_MAX;
    }

    ledline_write(state, leds);
}

static void draw_score(lcdpixel* fb, Player* player)
{
    char score_text[SCORE_TEXT_LENGTH];
    const char* text = SCORE_TEXT;
    memcpy(score_text, text, SCORE_CHARS);
    sprintf(score_text + SCORE_CHARS, "%d", player->playerScore);
    score_text[SCORE_TEXT_LENGTH - 1] = '\0';
    draw_text(fb, (Vertex_2D){SCORE_X, SCORE_Y}, score_text, (lcdpixel){WHITE}, FONT_PROP14x16);
}

static void draw_abilities(lcdpixel* fb, Player* player)
{
    for (int i = 0; i < 2; i++) {
        Vertex_2D center = (Vertex_2D){420, 80 + 40 * i};
        Vertex_2D topleft = (Vertex_2D){center.x - 16, center.y - 16};
        Vertex_2D botright = (Vertex_2D){center.x + 16, center.y + 16};
        if (player->selected_active_ability == i) {
            draw_rectangle(fb, (Vertex_2D){center.x - 17, center.y - 17}, (Vertex_2D){center.x + 17, center.y + 17},
                           (lcdpixel){.r = 31, .g = 63, .b = 0});
        }
        draw_rectangle(fb, topleft, botright, (lcdpixel){.r = 16, .g = 32, .b = 16});
        draw_sprite_centered(fb, center, player->activeAbilities[i].ability.sprite);
        float delta = player->activeAbilities[i].cooldown_end - time_ms() / 1000.0;
        if (delta > 0) {
            darken_region(fb, topleft, botright, 0.5);
            char buffer[6] = "00.0s";
            sprintf(buffer, "%2.1f", delta);
            draw_text(fb, topleft, buffer, (lcdpixel){WHITE}, FONT_PROP14x16);
        }
    }
}

void draw_ui(lcdpixel* fb, mzapo_state* state, Player* player)
{
    draw_healthbar(state, fb, player);
    draw_score(fb, player);
    draw_abilities(fb, player);
}
