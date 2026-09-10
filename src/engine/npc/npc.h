
#ifndef NPC_H
#define NPC_H

#include "../types/types.h"


#define NPC_TRANSP_COLOR 201

#define NPC_MAIN_GRAPHICS_INDEX 0

#define NPC_MAX_NPCS 16
#define NPC_SPEED 2
#define NPC_ANIMATION_SPEED 4
#define NPC_IDLE_TICKS 15

#define NPC_PATTERN_HOLD_ON 0
#define NPC_PATTERN_WALK_BACK 1
#define NPC_PATTERN_WALK_FORWARD 2
#define NPC_PATTERN_WALK_ARROUND 3
#define NPC_PATTERN_WALK_LEFT 4
#define NPC_PATTERN_WALK_RIGHT 5
#define NPC_PATTERN_WALK_UP 6
#define NPC_PATTERN_WALK_DOWN 7
#define NPC_PATTERN_WALK_RANDOM 8

#define NPC_STATUS_IDLE 0
#define NPC_STATUS_CHASE 1
#define NPC_STATUS_CHASE_IDLE 2
#define NPC_STATUS_AVOID 3
#define NPC_STATUS_HIDE 4
#define NPC_STATUS_STATIC 5
#define NPC_STATUS_WALKING_ARROUND 6
#define NPC_STATUS_WALKING_HORIZONTAL 7
#define NPC_STATUS_WALKING_VERTICAL 8
#define NPC_STATUS_WALKING_RANDOM 9

#define NPC_ACTION_STAND 1
#define NPC_ACTION_WALK 4

#define NPC_FACING_RIGHT 1
#define NPC_FACING_LEFT 2
#define NPC_FACING_DOWN 4
#define NPC_FACING_DOWN_RIGHT 5
#define NPC_FACING_DOWN_LEFT 6
#define NPC_FACING_UP 8
#define NPC_FACING_UP_RIGHT 9
#define NPC_FACING_UP_LEFT 10

#define NPC_MOVING_RIGHT 1
#define NPC_MOVING_LEFT 2
#define NPC_MOVING_DOWN 4
#define NPC_MOVING_UP 8

extern NPC *npc;
extern byte npc_counter;
void NPC_Init(void);
void NPC_Load(const char *dat_name, byte number, int x, int y, int face_gfx_id, int sprite_gfx_id, int facing, int behavior, int life);
void NPC_SetPosition(byte number, int x, int y);
void NPC_SetBehavior(byte number, int behavior);
void NPC_UnloadNpc(int npc_number);
void NPC_UnloadNpcs(void);
void NPC_Update(void);
void NPC_SetRange(int npc_number, int range);

#endif