#ifndef APO_SEM_CAMERA_H
#define APO_SEM_CAMERA_H

#include "const.h"

typedef struct {
    float x, y;
    float vx, vy;
} Camera;


typedef struct Player Player;
void update_camera(Camera* camera, Player* player, float dt_msec);
void initialize_camera(Camera* camera);

#endif // APO_SEM_CAMERA_H
