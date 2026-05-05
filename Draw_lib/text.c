#include "text.h"
#include <stddef.h>

#define CHAR_SPACING 1

// helper functions
static const font_descriptor_t* get_font_descriptor(int font)
{
    switch (font) {
    case FONT_ROM8x16:
        return &font_rom8x16;
    case FONT_PROP14x16:
        return &font_winFreeSystem14x16;
    default:
        return NULL;
    }
}

static inline unsigned int get_char_width(const font_descriptor_t* font, int glyph_index)
{
    if (font->width) {
        return font->width[glyph_index];
    }
    return font->maxwidth;
}

static inline unsigned int get_char_offset(const font_descriptor_t* font, int glyph_index)
{
    if (font->offset) {
        return font->offset[glyph_index];
    }
    return glyph_index * font->height;
}

static void draw_char(lcdpixel* fb, int dst_x, int dst_y, char ch, lcdpixel color, const font_descriptor_t* font)
{
    if ((unsigned int)ch < font->firstchar || (unsigned int)ch > font->firstchar + font->size) {
        ch = font->defaultchar;
    }
    int glyph_index = ch - font->firstchar;
    int glyph_width = get_char_width(font, glyph_index);
    int glyph_offset = get_char_offset(font, glyph_index);

    for (int y = 0; y < (int)font->height; y++) {
        uint16_t line = font->bits[glyph_offset + y];
        for (int x = 0; x < glyph_width; x++) {
            int px = dst_x + x;
            int py = dst_y + y;

            if (px < 0 || px >= SCREEN_WIDTH || py < 0 || py >= SCREEN_HEIGHT) {
                continue;
            }

            if (line & (1u << (15 - x))) {
                fb[py * SCREEN_WIDTH + px].raw = color.raw;
            }
        }
    }
}

void draw_text(lcdpixel* fb, Vertex_2D start, char* text, lcdpixel color, int font)
{
    const font_descriptor_t* font_d = get_font_descriptor(font);

    size_t offx = start.x;
    size_t offy = start.y;

    for (size_t i = 0; text[i]; i++) {
        if (text[i] == '\n') {
            offy += font_d->height;
            offx = start.x;
            continue;
        }
        draw_char(fb, offx, offy, text[i], color, font_d);
        int glyph_index = text[i] - font_d->firstchar;
        offx += get_char_width(font_d, glyph_index);
        if (font == FONT_PROP14x16)
            offx += CHAR_SPACING;
    }
}