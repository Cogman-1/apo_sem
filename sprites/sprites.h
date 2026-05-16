#ifndef APO_SEM_SPRITES_H
#define APO_SEM_SPRITES_H

#include "../Draw_lib/draw.h"

#include <stddef.h>

typedef enum { K_RED = 0, K_GREEN, K_BLUE } KnobIconColors;

typedef enum {
    SPRITE_CLASS_BACKGROUND,
    SPRITE_CLASS_HUMAN_SOLDIER = 0,
    SPRITE_CLASS_MONSTER_SLIME,
    SPRITE_CLASS_COUNT
} SpriteClass;

typedef enum {
    SPRITE_ANIM_IDLE = 0,
    SPRITE_ANIM_WALK,
    SPRITE_ANIM_ATTACK1,
    SPRITE_ANIM_ATTACK2,
    SPRITE_ANIM_BLOCK,
    SPRITE_ANIM_JUMP_FALL,
    SPRITE_ANIM_HURT,
    SPRITE_ANIM_DEATH,
    SPRITE_ANIM_COUNT
} SpriteAnimation;

typedef struct {
    SpriteClass sprite_class;
    SpriteAnimation animation;
    size_t frame_index;
    float frame_time_sec;
    float accumulator_sec;
    char loop;
} SpriteAnimator;

void sprite_init();
size_t sprite_animation_frame_count(SpriteClass sprite_class, SpriteAnimation animation);
Sprite* sprite_class_frame(SpriteClass sprite_class, SpriteAnimation animation, size_t frame_index);
void sprite_animator_init(SpriteAnimator* animator, SpriteClass sprite_class, SpriteAnimation animation,
                          float frame_time_sec);
void sprite_animator_set_class(SpriteAnimator* animator, SpriteClass sprite_class);
void sprite_animator_play(SpriteAnimator* animator, SpriteAnimation animation, int loop);
void sprite_animator_update(SpriteAnimator* animator, float dt_sec);
Sprite* sprite_animator_current_frame(const SpriteAnimator* animator);
Sprite* get_bg_sprite();
Sprite* get_arrow_sprites();
Sprite* get_start_menu_sprite();
Sprite* get_title_banner_sprite();
Sprite* get_cog_wip_sprite();
Sprite* get_knob_icon(KnobIconColors color);
Sprite* get_controls_bckg_sprite();

#endif
