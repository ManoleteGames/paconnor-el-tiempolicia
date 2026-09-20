#ifndef MISILE_H
#define MISILE_H

#include "../types/types.h"

#define MISILE_MAX_MISILES 16
#define MISILE_BLINK_COLOR 253
#define MISILE_DAMAGE 100

#define MISILE_FACING_RIGHT 1
#define MISILE_FACING_LEFT 2
#define MISILE_FACING_DOWN 4
#define MISILE_FACING_DOWN_RIGHT 5
#define MISILE_FACING_DOWN_LEFT 6
#define MISILE_FACING_UP 8
#define MISILE_FACING_UP_RIGHT 9
#define MISILE_FACING_UP_LEFT 10

void MISILE_Init(void);
void MISILE_DrawColissionPixels(Misile m);
int MISILE_LoadMisile(int graphics_id, int shadow_graphic_id, int entity_id, int pos_x, int pos_y, int target_x, int target_y, int speed, int damage, int max_time, int max_distance);
void MISILE_Update(void);
void MISILE_UnloadMisiles(void);

#endif