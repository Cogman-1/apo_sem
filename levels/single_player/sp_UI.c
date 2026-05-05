#include "sp_UI.h"

//health bar params
#define HEALTH_BAR_X 3
#define HEALTH_BAR_Y 5
#define HEALTH_BAR_WIDTH 100
#define HEALTH_BAR_HEIGHT 30
#define HEALTH_BAR_SPACING 4


// color macros
#define RED 0XF800
#define GREEN 0X07E0
#define GRAY 0X9CF3
#define WHITE 0XFFFF

void draw_healthbar(lcdpixel* fb, Player* player)
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

void draw_ui(lcdpixel* fb, Player* player)
{
    draw_healthbar(fb, player);

}