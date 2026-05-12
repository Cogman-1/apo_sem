#include "single_player.h"

#include "../../knobs.h"
#include "../../sprites/sprites.h"

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Type definitions

// helper functions
// get current time in miliseconds
uint64_t time_ms()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

// get delta time between frames in miliseconds
uint64_t get_dt()
{
    static uint64_t last = 0;
    uint64_t now = time_ms();
    if (last == 0) {
        last = now;
        return 0;
    }
    uint64_t dt = now - last;
    last = now;
    return dt;
}

// collision check
int AABBCollision(float ax, float ay, float aw, float ah, float bx, float by, float bw, float bh)
{
    return (ax < bx + bw) && (ax + aw > bx) && (ay < by + bh) && (ay + ah > by);
}

// prepare the gamestate
static void setup(GameState* state, uint32_t seed)
{
    state->gameSeed = seed;
    srand(seed);
    initialize_player(&state->player);
    initialize_camera(&state->cam);
    // prepare enemies
    for (int i = 0; i < MAX_ENEMY_COUNT; i++) {
        initialize_enemy(&state->enemies[i]);
        state->enemies[i].active = 0;
        sprite_animator_init(&state->enemy_animators[i], SPRITE_CLASS_MONSTER_SLIME, SPRITE_ANIM_IDLE, 0.1f);
    }
    // prepare projectiles
    for (int i = 0; i < MAX_PROJECTILE_COUNT; i++) {
        initialize_projectile(&state->projectiles[i]);
    }
    // initialize counters for projectiles and enemies
    state->enemy_count = 0;
    state->projectile_count = 0;
    // initialize cooldown timers
    state->enemy_spawn_timer = 0;
    state->shoot_cooldown_timer = 0;
    sprite_animator_init(&state->player_animator, SPRITE_CLASS_HUMAN_SOLDIER, SPRITE_ANIM_IDLE, 0.1f);
}

// Singleplayer main function
int single_player(mzapo_state* hw_state)
{
    GameState game_state;
    setup(&game_state, (uint32_t)time_ms());
    lcdpixel fb[SCREEN_WIDTH * SCREEN_HEIGHT];
    clear_display(fb);
    int start_again = 0;
    // Main Game loop
    int exit = 0;
    knobs_state* ks = malloc(sizeof(*ks));
    knobs_state_init(ks, knobs_read(hw_state));
    while (!exit) {
        // 0. get dt
        uint64_t dt_msec = get_dt();
        float dt = dt_msec / 1000.0f;
        // 1. read inputs
        knobs k = knobs_read(hw_state);
        knobs_update(ks, k);
        knobs_delta kd = knobs_get_clamped_delta(ks);
        // check for a pause request
        if (k.gdown) {
            int p_ret = pause(hw_state, fb);
            if (p_ret == P_EXIT_MAIN_MENU) {
                exit = 1;
            }
        }
        ActiveAbility* ab = &game_state.player.activeAbilities[game_state.player.selected_active_ability];
        if (k.bdown && ab->cooldown_end < time_ms() / 1000.0) {
            ab->ability.effect((void*)&game_state);
            ab->cooldown_end = time_ms() / 1000.0 + ab->ability.cooldown;
            fprintf(stderr, "active ability %d: %f -> %f\n", game_state.player.selected_active_ability,
                    time_ms() / 1000.0, ab->cooldown_end);
        }
        game_state.player.selected_active_ability = (game_state.player.selected_active_ability + kd.b + 2) % 2;

        // 2. Update player
        knob_directions dirs = calculate_direction(ks);
        update_player(&game_state.player, dirs, dt);
        if (fabsf(game_state.player.vx) + fabsf(game_state.player.vy) > 0.1f)
            sprite_animator_play(&game_state.player_animator, SPRITE_ANIM_WALK, 1);
        else
            sprite_animator_play(&game_state.player_animator, SPRITE_ANIM_IDLE, 1);
        sprite_animator_update(&game_state.player_animator, dt);
        // 3. Update Camera
        update_camera(&game_state.cam, &game_state.player, dt);
        // 4. Update projectiles + check for collisions projectile vs enemies
        // spawn new projectile if the player is shooting
        game_state.shoot_cooldown_timer += dt;
        if (game_state.shoot_cooldown_timer >= game_state.player.fireCooldown && game_state.enemy_count > 0) {
            spawn_projectile(game_state.projectiles, game_state.enemies, &game_state.player,
                             &game_state.projectile_count);
            game_state.shoot_cooldown_timer = 0.0f;
        }
        // update all the projectiles
        for (int i = 0; i < MAX_PROJECTILE_COUNT; i++) {
            if (game_state.projectiles[i].active) {
                update_projectile(&game_state.projectiles[i], &game_state.cam, &game_state.projectile_count, dt);
            }
        }
        // check for collisions projectile vs enemy
        for (int i = 0; i < MAX_PROJECTILE_COUNT; i++) {
            if (game_state.projectiles[i].active) {
                float ax = game_state.projectiles[i].x;
                float ay = game_state.projectiles[i].y;
                for (int j = 0; j < MAX_ENEMY_COUNT; j++) {
                    if (game_state.enemies[j].active) {
                        float bx = game_state.enemies[j].x;
                        float by = game_state.enemies[j].y;
                        int colliding = AABBCollision(ax, ay, PROJECTILE_WIDTH, PROJECTILE_HEIGHT, bx, by, ENEMY_WIDTH,
                                                      ENEMY_HEIGHT);
                        if (colliding) {
                            deal_damage_enemy(&game_state.enemies[j], &game_state.projectiles[i]);
                            game_state.projectiles[i].active = 0;
                            game_state.projectile_count--;
                            break;
                        }
                    }
                }
            }
        }
        // 5. spawn new enemies + update enemies + check for collisions enemy vs player
        // spawn new enemies
        game_state.enemy_spawn_timer += dt;
        if (game_state.enemy_spawn_timer >= ENEMY_SPAWN_COOL && game_state.enemy_count < MAX_ENEMY_COUNT) {
            spawn_enemy(game_state.enemies, &game_state.cam, &game_state.enemy_count);
            game_state.enemy_spawn_timer -= ENEMY_SPAWN_COOL;
        }
        // update enemies
        for (int i = 0; i < MAX_ENEMY_COUNT; i++) {
            if (game_state.enemies[i].active) {
                update_enemy(&game_state.enemies[i], &game_state.player, &game_state.enemy_count, dt);
                if (fabsf(game_state.enemies[i].vx) + fabsf(game_state.enemies[i].vy) > 0.1f)
                    sprite_animator_play(&game_state.enemy_animators[i], SPRITE_ANIM_WALK, 1);
                else
                    sprite_animator_play(&game_state.enemy_animators[i], SPRITE_ANIM_IDLE, 1);
                sprite_animator_update(&game_state.enemy_animators[i], dt);
            }
        }
        // check for collisions enemy vs. player
        for (int i = 0; i < MAX_ENEMY_COUNT; i++) {
            if (game_state.enemies[i].active) {
                if (AABBCollision(game_state.enemies[i].x, game_state.enemies[i].y, ENEMY_WIDTH, ENEMY_HEIGHT,
                                  game_state.player.x, game_state.player.y, PLAYER_WIDTH, PLAYER_HEIGHT)) {
                    take_damage(&game_state.player, &game_state.enemies[i]);
                    deal_knockback_enemy(&game_state.enemies[i], (Vertex_2D){game_state.player.x, game_state.player.y});
                }
            }
        }
        // 6. draw frame
        draw_tiled_background(fb, (Vertex_2D){.x = -game_state.cam.x, .y = -game_state.cam.y}, get_bg_sprite());
        draw_player(&game_state.player, &game_state.cam, sprite_animator_current_frame(&game_state.player_animator),
                    fb);
        // draw enemies
        for (int i = 0; i < MAX_ENEMY_COUNT; i++) {
            if (game_state.enemies[i].active) {
                draw_enemy(&game_state.enemies[i], &game_state.cam,
                           sprite_animator_current_frame(&game_state.enemy_animators[i]), fb);
            }
        }
        for (int i = 0; i < MAX_PROJECTILE_COUNT; i++) {
            if (game_state.projectiles[i].active) {
                draw_projectile(&game_state.projectiles[i], &game_state.cam, fb, get_arrow_sprites());
            }
        }
        draw_ui(fb, hw_state, &game_state.player);
        parlcd_write_screen(hw_state, fb);
        // 7. update rgb leds
        update_effect(hw_state, dt);
        //8. check if the player is dead
        if (game_state.player.health <= 0) {
            exit = 1;
            int option = death(&game_state.player, fb, hw_state);
            if (option == RESTART) {
                start_again = 1;
            }
        }
    }
    return start_again;
}