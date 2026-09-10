#ifndef OBJECT_H
#define OBJECT_H

#include "../types/types.h"

#define OBJECT_MAX_OBJECTS 32

#define OBJECT_BARREL_WIDTH 16
#define OBJECT_BARREL_HEIGHT 32
#define OBJECT_BARREL_NUM_FRAMES 13
#define OBJECT_BARREL_COLISSION_FRAME 13


/// OBJECT.C ///
extern Object *object;
void OBJECT_Init(void);
void OBJECT_LoadObject(const char *dat_name, byte number, byte entity_id, byte sprite_graphics_id, byte portait_graphics_id, int pos_x, int pos_y);
void OBJECT_UpdateObjects(void);
void OBJECT_UnloadObjects(void);
void OBJECT_UnloadObject(int number);
bool OBJECT_IsOnScreen(int number);
void OBJECT_SetPosition(int number, int pos_x, int pos_y);
bool OBJECT_IsLoaded(int number);

#endif