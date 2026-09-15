#ifndef MISILE_H
#define MISILE_H

#include "../types/types.h"

#define MISILE_MAX_MISILES 16
#define MISILE_BLINK_COLOR 253
#define MISILE_DAMAGE 100

void MISILE_Init(void);
void MISILE_DrawColissionPixels(Misile m);
int MISILE_LoadMisile(int graphics_id, int shadow_graphic_id, int entity_id, int pos_x, int pos_y, int target_x, int target_y, int max_time, int max_distance);
void MISILE_Update(void);
void MISILE_UnloadMisiles(void);

#endif