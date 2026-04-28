#include "camera.h"

#include <math.h>

void update_camera(Camera* camera, Player* player, float dt_msec)
{
    // get the target coordinates
    float c_target_x = player->x - SCREEN_WIDTH * 0.5f;
    float c_target_y = player->y - SCREEN_HEIGHT * 0.5f;

    // clamp the target position to the world dimensions
    if (c_target_x < 0)
        c_target_x = 0;
    if (c_target_x > (WORLD_WIDTH - SCREEN_WIDTH))
        c_target_x = WORLD_WIDTH - SCREEN_WIDTH;
    if (c_target_y < 0)
        c_target_y = 0;
    if (c_target_y > (WORLD_HEIGHT - SCREEN_HEIGHT))
        c_target_y = WORLD_HEIGHT - SCREEN_HEIGHT;

    // calculate direction of movement
    float dir_x = c_target_x - camera->x;
    float dir_y = c_target_y - camera->y;

    // move towards the target coordinates
    float alpha = 1 - expf(-CAMERA_CONVERGENCE_RATE * dt_msec);
    camera->x += alpha * dir_x;
    camera->y += alpha * dir_y;

    // clamp new coords to the world dimensions
    if (camera->x < 0)
        camera->x = 0;
    if (camera->x > WORLD_WIDTH - SCREEN_WIDTH)
        camera->x = WORLD_WIDTH - SCREEN_WIDTH;
    if (camera->y < 0)
        camera->y = 0;
    if (camera->y > WORLD_HEIGHT - SCREEN_HEIGHT)
        camera->y = WORLD_HEIGHT - SCREEN_HEIGHT;
}