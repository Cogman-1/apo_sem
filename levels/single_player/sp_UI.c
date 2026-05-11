#include "sp_UI.h"

#include <stdio.h>
#include <string.h>

// color macros

static void draw_healthbar(lcdpixel* fb, Player* player)
{
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

void draw_ui(lcdpixel* fb, Player* player)
{
    draw_healthbar(fb, player);
    draw_score(fb, player);
}