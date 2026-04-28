#ifndef APO_SEM_CAMERA_H
#define APO_SEM_CAMERA_H


typedef struct {
    float x, y;
    float vx, vy;
} Camera;

#include "const.h"
#include "player.h"

void update_camera(Camera* camera, Player* player, float dt_msec);

#endif // APO_SEM_CAMERA_H
