#ifndef GRENADE_H
#define GRENADE_H

#include "../types/types.h"

#define GRENADE_MAX_GRENADES 16
#define GRENADE_BLINK_COLOR 253
#define GRENADE_DAMAGE 100
#define GRENADE_SPEED 5

void GRENADE_DrawColissionPixels(Grenade g);
void GRENADE_Init(void);
int GRENADE_LoadGrenade(int graphics_id, int entity_id, int pos_x, int pos_y, int max_time, int max_distance);
void GRENADE_ThrowGrenade(int number, int target_x, int target_y, int accuracy);
void GRENADE_Update(void);
void GRENADE_UnloadGrenades(void);

#endif