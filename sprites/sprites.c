#include "sprites.h"

#include <stddef.h>

#include "Human_Soldier_Sword_Shield/Human_Soldier_Sword_Shield_Attack1/Human_Soldier_Sword_Shield_Attack1_000.c"
#include "Human_Soldier_Sword_Shield/Human_Soldier_Sword_Shield_Attack1/Human_Soldier_Sword_Shield_Attack1_001.c"
#include "Human_Soldier_Sword_Shield/Human_Soldier_Sword_Shield_Attack2/Human_Soldier_Sword_Shield_Attack2_000.c"
#include "Human_Soldier_Sword_Shield/Human_Soldier_Sword_Shield_Attack2/Human_Soldier_Sword_Shield_Attack2_001.c"
#include "Human_Soldier_Sword_Shield/Human_Soldier_Sword_Shield_Block/Human_Soldier_Sword_Shield_Block_000.c"
#include "Human_Soldier_Sword_Shield/Human_Soldier_Sword_Shield_Block/Human_Soldier_Sword_Shield_Block_001.c"
#include "Human_Soldier_Sword_Shield/Human_Soldier_Sword_Shield_Death/Human_Soldier_Sword_Shield_Death_000.c"
#include "Human_Soldier_Sword_Shield/Human_Soldier_Sword_Shield_Death/Human_Soldier_Sword_Shield_Death_001.c"
#include "Human_Soldier_Sword_Shield/Human_Soldier_Sword_Shield_Hurt/Human_Soldier_Sword_Shield_Hurt_000.c"
#include "Human_Soldier_Sword_Shield/Human_Soldier_Sword_Shield_Hurt/Human_Soldier_Sword_Shield_Hurt_001.c"
#include "Human_Soldier_Sword_Shield/Human_Soldier_Sword_Shield_Idle/Human_Soldier_Sword_Shield_Idle_000.c"
#include "Human_Soldier_Sword_Shield/Human_Soldier_Sword_Shield_Idle/Human_Soldier_Sword_Shield_Idle_001.c"
#include "Human_Soldier_Sword_Shield/Human_Soldier_Sword_Shield_Jump_Fall/Human_Soldier_Sword_Shield_Jump_Fall_000.c"
#include "Human_Soldier_Sword_Shield/Human_Soldier_Sword_Shield_Jump_Fall/Human_Soldier_Sword_Shield_Jump_Fall_001.c"
#include "Human_Soldier_Sword_Shield/Human_Soldier_Sword_Shield_Walk/Human_Soldier_Sword_Shield_Walk_000.c"
#include "Human_Soldier_Sword_Shield/Human_Soldier_Sword_Shield_Walk/Human_Soldier_Sword_Shield_Walk_001.c"

#include "Monster_Slime/Monster_Slime_Attack1/Monster_Slime_Attack1_000.c"
#include "Monster_Slime/Monster_Slime_Attack1/Monster_Slime_Attack1_001.c"
#include "Monster_Slime/Monster_Slime_Attack2/Monster_Slime_Attack2_000.c"
#include "Monster_Slime/Monster_Slime_Attack2/Monster_Slime_Attack2_001.c"
#include "Monster_Slime/Monster_Slime_Block/Monster_Slime_Block_000.c"
#include "Monster_Slime/Monster_Slime_Block/Monster_Slime_Block_001.c"
#include "Monster_Slime/Monster_Slime_Death/Monster_Slime_Death_000.c"
#include "Monster_Slime/Monster_Slime_Death/Monster_Slime_Death_001.c"
#include "Monster_Slime/Monster_Slime_Hurt/Monster_Slime_Hurt_000.c"
#include "Monster_Slime/Monster_Slime_Hurt/Monster_Slime_Hurt_001.c"
#include "Monster_Slime/Monster_Slime_Idle/Monster_Slime_Idle_000.c"
#include "Monster_Slime/Monster_Slime_Idle/Monster_Slime_Idle_001.c"
#include "Monster_Slime/Monster_Slime_Jump_Fall/Monster_Slime_Jump_Fall_000.c"
#include "Monster_Slime/Monster_Slime_Jump_Fall/Monster_Slime_Jump_Fall_001.c"
#include "Monster_Slime/Monster_Slime_Walk/Monster_Slime_Walk_000.c"
#include "Monster_Slime/Monster_Slime_Walk/Monster_Slime_Walk_001.c"

#define FRAME_TO_SPRITE(symbol)                                                                                        \
    {                                                                                                                  \
#symbol, (uint16_t)(symbol).width, (uint16_t)(symbol).height, (const uint16_t*)(symbol).pixel_data             \
    }

typedef struct {
    const Sprite* frames;
    size_t frame_count;
    char default_loop;
} SpriteAnimationClip;

static const Sprite human_idle[] = {FRAME_TO_SPRITE(Human_Soldier_Sword_Shield_Idle_000),
                                    FRAME_TO_SPRITE(Human_Soldier_Sword_Shield_Idle_001)};
static const Sprite human_walk[] = {FRAME_TO_SPRITE(Human_Soldier_Sword_Shield_Walk_000),
                                    FRAME_TO_SPRITE(Human_Soldier_Sword_Shield_Walk_001)};
static const Sprite human_attack1[] = {FRAME_TO_SPRITE(Human_Soldier_Sword_Shield_Attack1_000),
                                       FRAME_TO_SPRITE(Human_Soldier_Sword_Shield_Attack1_001)};
static const Sprite human_attack2[] = {FRAME_TO_SPRITE(Human_Soldier_Sword_Shield_Attack2_000),
                                       FRAME_TO_SPRITE(Human_Soldier_Sword_Shield_Attack2_001)};
static const Sprite human_block[] = {FRAME_TO_SPRITE(Human_Soldier_Sword_Shield_Block_000),
                                     FRAME_TO_SPRITE(Human_Soldier_Sword_Shield_Block_001)};
static const Sprite human_jump_fall[] = {FRAME_TO_SPRITE(Human_Soldier_Sword_Shield_Jump_Fall_000),
                                         FRAME_TO_SPRITE(Human_Soldier_Sword_Shield_Jump_Fall_001)};
static const Sprite human_hurt[] = {FRAME_TO_SPRITE(Human_Soldier_Sword_Shield_Hurt_000),
                                    FRAME_TO_SPRITE(Human_Soldier_Sword_Shield_Hurt_001)};
static const Sprite human_death[] = {FRAME_TO_SPRITE(Human_Soldier_Sword_Shield_Death_000),
                                     FRAME_TO_SPRITE(Human_Soldier_Sword_Shield_Death_001)};

static const Sprite slime_idle[] = {FRAME_TO_SPRITE(Monster_Slime_Idle_000), FRAME_TO_SPRITE(Monster_Slime_Idle_001)};
static const Sprite slime_walk[] = {FRAME_TO_SPRITE(Monster_Slime_Walk_000), FRAME_TO_SPRITE(Monster_Slime_Walk_001)};
static const Sprite slime_attack1[] = {FRAME_TO_SPRITE(Monster_Slime_Attack1_000),
                                       FRAME_TO_SPRITE(Monster_Slime_Attack1_001)};
static const Sprite slime_attack2[] = {FRAME_TO_SPRITE(Monster_Slime_Attack2_000),
                                       FRAME_TO_SPRITE(Monster_Slime_Attack2_001)};
static const Sprite slime_block[] = {FRAME_TO_SPRITE(Monster_Slime_Block_000),
                                     FRAME_TO_SPRITE(Monster_Slime_Block_001)};
static const Sprite slime_jump_fall[] = {FRAME_TO_SPRITE(Monster_Slime_Jump_Fall_000),
                                         FRAME_TO_SPRITE(Monster_Slime_Jump_Fall_001)};
static const Sprite slime_hurt[] = {FRAME_TO_SPRITE(Monster_Slime_Hurt_000), FRAME_TO_SPRITE(Monster_Slime_Hurt_001)};
static const Sprite slime_death[] = {FRAME_TO_SPRITE(Monster_Slime_Death_000),
                                     FRAME_TO_SPRITE(Monster_Slime_Death_001)};

static const SpriteAnimationClip human_animations[SPRITE_ANIM_COUNT] = {
    [SPRITE_ANIM_IDLE] = {human_idle, sizeof(human_idle) / sizeof(human_idle[0]), 1},
    [SPRITE_ANIM_WALK] = {human_walk, sizeof(human_walk) / sizeof(human_walk[0]), 1},
    [SPRITE_ANIM_ATTACK1] = {human_attack1, sizeof(human_attack1) / sizeof(human_attack1[0]), 0},
    [SPRITE_ANIM_ATTACK2] = {human_attack2, sizeof(human_attack2) / sizeof(human_attack2[0]), 0},
    [SPRITE_ANIM_BLOCK] = {human_block, sizeof(human_block) / sizeof(human_block[0]), 1},
    [SPRITE_ANIM_JUMP_FALL] = {human_jump_fall, sizeof(human_jump_fall) / sizeof(human_jump_fall[0]), 1},
    [SPRITE_ANIM_HURT] = {human_hurt, sizeof(human_hurt) / sizeof(human_hurt[0]), 0},
    [SPRITE_ANIM_DEATH] = {human_death, sizeof(human_death) / sizeof(human_death[0]), 0},
};

static const SpriteAnimationClip slime_animations[SPRITE_ANIM_COUNT] = {
    [SPRITE_ANIM_IDLE] = {slime_idle, sizeof(slime_idle) / sizeof(slime_idle[0]), 1},
    [SPRITE_ANIM_WALK] = {slime_walk, sizeof(slime_walk) / sizeof(slime_walk[0]), 1},
    [SPRITE_ANIM_ATTACK1] = {slime_attack1, sizeof(slime_attack1) / sizeof(slime_attack1[0]), 0},
    [SPRITE_ANIM_ATTACK2] = {slime_attack2, sizeof(slime_attack2) / sizeof(slime_attack2[0]), 0},
    [SPRITE_ANIM_BLOCK] = {slime_block, sizeof(slime_block) / sizeof(slime_block[0]), 1},
    [SPRITE_ANIM_JUMP_FALL] = {slime_jump_fall, sizeof(slime_jump_fall) / sizeof(slime_jump_fall[0]), 1},
    [SPRITE_ANIM_HURT] = {slime_hurt, sizeof(slime_hurt) / sizeof(slime_hurt[0]), 0},
    [SPRITE_ANIM_DEATH] = {slime_death, sizeof(slime_death) / sizeof(slime_death[0]), 0},
};

static const SpriteAnimationClip* sprite_class_animations(SpriteClass sprite_class)
{
    switch (sprite_class) {
    case SPRITE_CLASS_HUMAN_SOLDIER:
        return human_animations;
    case SPRITE_CLASS_MONSTER_SLIME:
        return slime_animations;
    default:
        return NULL;
    }
}


size_t sprite_animation_frame_count(SpriteClass sprite_class, SpriteAnimation animation)
{
    const SpriteAnimationClip* animations = sprite_class_animations(sprite_class);
    return animations[animation].frame_count;
}

Sprite* sprite_class_frame(SpriteClass sprite_class, SpriteAnimation animation, size_t frame_index)
{
    const SpriteAnimationClip* animations = sprite_class_animations(sprite_class);
    const SpriteAnimationClip* clip = &animations[animation];
    return (Sprite*)&clip->frames[frame_index % clip->frame_count];
}

void sprite_animator_init(SpriteAnimator* animator, SpriteClass sprite_class, SpriteAnimation animation,
                          float frame_time_sec)
{
    animator->sprite_class = sprite_class;
    animator->animation = animation;
    animator->frame_index = 0;
    animator->frame_time_sec = frame_time_sec;
    animator->accumulator_sec = 0.0f;
    animator->loop = 1;
    sprite_animator_play(animator, animation, -1);
}

void sprite_animator_set_class(SpriteAnimator* animator, SpriteClass sprite_class)
{
    if (animator->sprite_class != sprite_class) {
        animator->sprite_class = sprite_class;
        animator->frame_index = 0;
        animator->accumulator_sec = 0.0f;
        sprite_animator_play(animator, animator->animation, -1);
    }
}

void sprite_animator_play(SpriteAnimator* animator, SpriteAnimation animation, int loop)
{
    const SpriteAnimationClip* animations;
    animations = sprite_class_animations(animator->sprite_class);
    if (animator->animation != animation) {
        animator->animation = animation;
        animator->frame_index = 0;
        animator->accumulator_sec = 0.0f;
    }
    if (loop < 0)
        animator->loop = animations[animation].default_loop;
    else
        animator->loop = (char)(loop != 0);
}

void sprite_animator_update(SpriteAnimator* animator, float dt_sec)
{
    size_t frame_count;
    frame_count = sprite_animation_frame_count(animator->sprite_class, animator->animation);
    animator->accumulator_sec += dt_sec;
    while (animator->accumulator_sec >= animator->frame_time_sec) {
        animator->accumulator_sec -= animator->frame_time_sec;
        if (animator->frame_index + 1 < frame_count) {
            animator->frame_index++;
        } else if (animator->loop) {
            animator->frame_index = 0;
        }
    }
}

Sprite* sprite_animator_current_frame(const SpriteAnimator* animator)
{
    return sprite_class_frame(animator->sprite_class, animator->animation, animator->frame_index);
}
