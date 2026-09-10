#ifndef CAMERA_H
#define CAMERA_H

#include "../types/types.h"

#define CAM_OFFSET_X 140
#define CAM_OFFSET_Y 80

#define CAM_VISIBLE_WIDTH 320
#define CAM_VISIBLE_HEIGHT 208

#define CAM_SPEED 1

/// CAMERA.C ///
extern Camera *camera;
void CAM_Init(int x_limit, int y_limit, int x_pos, int y_pos);
void CAM_MainPositionControl(bool player_follow, bool *map_update_flag);
void CAM_SetPosition(int x, int y);
void CAM_SetLimits(int x, int y);
void CAM_UpdateParalax(int delay1, int delay2, int delay3);

#endif