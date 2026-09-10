#ifndef BULLET_H
#define BULLET_H

#include "../types/types.h"

#define BULLET_MAX_BULLETS 128

extern Bullet *bullet;
extern int bullet_counter;
void BULLET_Init(void);
void BULLET_LoadBullet(int graphics_id, int entity_id, int width, int height, int source_x, int source_y, int target_x, int target_y, int accuracy, int max_distance, int speed, int damage);
int BULLET_CheckBulletColission(int number);
void BULLET_Update(void);
void BULLET_UnloadBullets(void);

#endif