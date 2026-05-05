#include "sp_UI.h"

#include <stdio.h>
#include <string.h>

// health bar params
#define HEALTH_BAR_X 3
#define HEALTH_BAR_Y 5
#define HEALTH_BAR_WIDTH 100
#define HEALTH_BAR_HEIGHT 30
#define HEALTH_BAR_SPACING 4

// score counter params
#define SCORE_X 280
#define SCORE_Y 5
#define SCORE_DIGITS 10
#define SCORE_TEXT "Score: "
#define SCORE_CHARS strlen(SCORE_TEXT)
#define SCORE_TEXT_LENGTH (SCORE_DIGITS + SCORE_CHARS + 1)

// color macros
#define RED 0XF800
#define GREEN 0X07E0
#define GRAY 0X9CF3
#define WHITE 0XFFFF

static void draw_healthbar(lcdpixel* fb, Player* player)
{
    lcdpixel color = {GRAY};
    // draw base
    draw_rectangle(fb, (Vertex_2D){HEALTH_BAR_X, HEALTH_BAR_Y},
                   (Vertex_2D){HEALTH_BAR_X + HEALTH_BAR_WIDTH, HEALTH_BAR_Y + HEALTH_BAR_HEIGHT}, color);
    // draw red background
    color.raw = RED;
    draw_rectangle(fb, (Vertex_2D){HEALTH_BAR_X + HEALTH_BAR_SPACING, HEALTH_BAR_Y + HEALTH_BAR_SPACING},
                   (Vertex_2D){HEALTH_BAR_X + HEALTH_BAR_WIDTH - HEALTH_BAR_SPACING,
                               HEALTH_BAR_Y + HEALTH_BAR_HEIGHT - HEALTH_BAR_SPACING},
                   color);
    // calculate and draw green part
    color.raw = GREEN;
    int health_pixels = (((float)player->health) / (player->maxHealth)) * (HEALTH_BAR_WIDTH - 2 * HEALTH_BAR_SPACING);
    draw_rectangle(fb, (Vertex_2D){HEALTH_BAR_X + HEALTH_BAR_SPACING, HEALTH_BAR_Y + HEALTH_BAR_SPACING},
                   (Vertex_2D){HEALTH_BAR_X + HEALTH_BAR_SPACING + health_pixels,
                               HEALTH_BAR_Y + HEALTH_BAR_HEIGHT - HEALTH_BAR_SPACING},
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