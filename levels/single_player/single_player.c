#include "single_player.h"

#include <stdint.h>
#include <stdlib.h>
#include <time.h>

// Type definitions
typedef struct {
    Player player;
    Camera cam;
    int enemy_count;
    Enemy enemies[MAX_ENEMY_COUNT];
    int projectile_count;
    Projectile projectiles[MAX_PROJECTILE_COUNT];
    uint32_t gameSeed;
    float enemy_spawn_timer;
    float shoot_cooldown_timer;
} GameState;

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
    // initialize player
    state->player.x = (float)WORLD_WIDTH / 2;
    state->player.y = (float)WORLD_HEIGHT / 2;
    state->player.vx = 0;
    state->player.vy = 0;
    state->player.maxHealth = PLAYER_START_MAX_HEALTH;
    state->player.health = state->player.maxHealth;
    state->player.damage = PLAYER_START_DAMAGE;
    state->player.fireCooldown = PLAYER_START_FIRE_COOLDOWN;
    state->player.playerScore = 0;
    // initialize camera
    state->cam.x = (float)WORLD_WIDTH / 2 - SCREEN_WIDTH / 2;
    state->cam.y = (float)WORLD_HEIGHT / 2 - SCREEN_HEIGHT / 2;
    state->cam.vx = 0;
    state->cam.vy = 0;
    // prepare enemies
    for (int i = 0; i < MAX_ENEMY_COUNT; i++) {
        state->enemies[i].active = 0;
    }
    // prepare projectiles
    for (int i = 0; i < MAX_PROJECTILE_COUNT; i++) {
        state->projectiles[i].active = 0;
    }
    // initialize counters for projectiles and enemies
    state->enemy_count = 0;
    state->projectile_count = 0;
    // initialize cooldown timers
    state->enemy_spawn_timer = 0;
    state->shoot_cooldown_timer = 0;
}

// Singleplayer main function
void single_player(mzapo_state* hw_state)
{
    GameState game_state;
    setup(&game_state, (uint32_t)time_ms());
    lcdpixel fb[SCREEN_WIDTH * SCREEN_HEIGHT];
    clear_display(fb);
    // Main Game loop
    int exit = 0;
    while (!exit) {
        // 0. get dt
        uint64_t dt_msec = get_dt();
        float dt = dt_msec / 1000.0f;
        // 1. read inputs
        knobs k = knobs_read(hw_state);
        // check for a pause request
        if (k.gdown) {
            int p_ret = pause(hw_state, fb);
            if (p_ret == P_EXIT_MAIN_MENU) {
                exit = 1;
            }
        }
        // 2. Update player
        update_player(&game_state.player, k, dt);
        // 3. Update Camera
        update_camera(&game_state.cam, &game_state.player, dt);
        // 4. Update projectiles + check for collisions projectile vs enemies
        // spawn new projectile if the player is shooting
        game_state.shoot_cooldown_timer += dt;
        if (game_state.shoot_cooldown_timer >= game_state.player.fireCooldown &&
            game_state.projectile_count <= MAX_PROJECTILE_COUNT && game_state.enemy_count > 0) {
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
                            game_state.enemies[j].health -= game_state.projectiles[i].damage;
                            game_state.projectiles[i].active = 0;
                            break;
                        }
                    }
                }
            }
        }
        // 5. spawn new enemies + update enemies + check for collisions enemy vs player
        // spawn new enemies
        game_state.enemy_spawn_timer += dt;
        if (game_state.enemy_spawn_timer >= ENEMY_SPAWN_COOL && game_state.enemy_count <= MAX_ENEMY_COUNT) {
            spawn_enemy(game_state.enemies, &game_state.cam, &game_state.enemy_count);
            game_state.enemy_spawn_timer -= ENEMY_SPAWN_COOL;
        }
        // update enemies
        for (int i = 0; i < MAX_ENEMY_COUNT; i++) {
            if (game_state.enemies[i].active)
                update_enemy(&game_state.enemies[i], &game_state.player, &game_state.enemy_count, dt);
        }
        // check for collisions enemy vs. player
        for (int i = 0; i < MAX_ENEMY_COUNT; i++) {
            if (game_state.enemies[i].active) {
                if (AABBCollision(game_state.enemies[i].x, game_state.enemies[i].y, ENEMY_WIDTH, ENEMY_HEIGHT,
                                  game_state.player.x, game_state.player.y, PLAYER_WIDTH, PLAYER_HEIGHT))
                    take_damage(&game_state.player);
            }
        }
        // check for collisions eneme vs. player
        for (int i = 0; i < MAX_ENEMY_COUNT; i++) {
            if (game_state.enemies[i].active) {
                if (AABBCollision(game_state.enemies[i].x, game_state.enemies[i].y, ENEMY_WIDTH, ENEMY_HEIGHT,
                                  game_state.player.x, game_state.player.y, PLAYER_WIDTH, PLAYER_HEIGHT))
                    take_damage(&game_state.player);
            }
        }
        // 6. draw frame
        clear_display(fb);
        // draw player
        Vertex_2D player_top_left = {0};
        player_top_left.x = game_state.player.x - game_state.cam.x;
        player_top_left.y = game_state.player.y - game_state.cam.y;
        Vertex_2D player_Bot_right = {0};
        player_Bot_right.x = player_top_left.x + PLAYER_WIDTH;
        player_Bot_right.y = player_top_left.y + PLAYER_HEIGHT;
        lcdpixel player_color;
        player_color.raw = PLAYER_COLOR;
        draw_rectangle(fb, player_top_left, player_Bot_right, player_color);
        // draw enemies
        for (int i = 0; i < MAX_ENEMY_COUNT; i++) {
            if (game_state.enemies[i].active) {
                Vertex_2D top_left;
                top_left.x = game_state.enemies[i].x - game_state.cam.x;
                top_left.y = game_state.enemies[i].y - game_state.cam.y;
                Vertex_2D bot_right;
                bot_right.x = top_left.x + ENEMY_WIDTH;
                bot_right.y = top_left.y + ENEMY_HEIGHT;
                lcdpixel enemy_color;
                enemy_color.raw = ENEMY_COLOR;
                draw_rectangle(fb, top_left, bot_right, enemy_color);
            }
        }
        // draw projectiles
        for (int i = 0; i < MAX_PROJECTILE_COUNT; i++) {
            if (game_state.projectiles[i].active) {
                int cx = (int)game_state.projectiles[i].x + PROJECTILE_WIDTH / 2 - game_state.cam.x;
                int cy = (int)game_state.projectiles[i].y + PROJECTILE_HEIGHT / 2 - game_state.cam.y;
                draw_circle(fb, (Vertex_2D){cx, cy}, PROJECTILE_HEIGHT / 2, (lcdpixel){.raw = PROJECTILE_COLOR});
            }
        }
        // show frame buffer
        parlcd_write_screen(hw_state, fb);
    }
}