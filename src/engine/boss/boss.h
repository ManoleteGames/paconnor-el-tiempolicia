#ifndef BOSS_H
#define BOSS_H

#include "../types/types.h"

#define BOSS_SPEED 5
#define BOSS_RAMPAGE_SPEED 10
#define BOSS_LIFE 3000
#define BOSS_ANIMATION_SPEED 5

#define BOSS_PUNCH_RANGE 16
#define BOSS_SHOOT_RANGE 92
#define BOSS_REACTION_TIME 10

#define BOSS_IDLE_TICKS 5

#define BOSS_STATUS_CHASE 1
#define BOSS_STATUS_SHORT_ATTACK 2
#define BOSS_STATUS_LONG_ATTACK 3
#define BOSS_STATUS_RAMPAGE 4
#define BOSS_STATUS_STATIC 5

#define BOSS_ACTION_STAND 1
#define BOSS_ACTION_SHOOT 2
#define BOSS_ACTION_WALK 3
#define BOSS_ACTION_PUNCH 4

#define BOSS_MOVING_RIGHT 1
#define BOSS_MOVING_LEFT 2
#define BOSS_MOVING_DOWN 4
#define BOSS_MOVING_UP 8

#define BOSS_PATTERN_HOLD_ON 0
#define BOSS_PATTERN_WALK_BACK 1
#define BOSS_PATTERN_WALK_FORWARD 2
#define BOSS_PATTERN_WALK_ARROUND 3
#define BOSS_PATTERN_ATTACK 4

#define BOSS_ANIM_FEET_INDEX 0
#define BOSS_ANIM_BODY_INDEX 1
#define BOSS_ANIM_HEAD_INDEX 2
#define BOSS_ANIM_LARM_INDEX 3
#define BOSS_ANIM_RARM_INDEX 4

#define BOSS_ANIM_STAND 1
#define BOSS_ANIM_WALK 2
#define BOSS_ANIM_PUNCH 3
#define BOSS_ANIM_SHOT 4
#define BOSS_ANIM_HIT 5
#define BOSS_ANIM_FALL 6
#define BOSS_ANIM_DEAD 7

#define BOSS_TYPE_FAST_SPEED 1
#define BOSS_TYPE_MEDIUM_SPEED 2
#define BOSS_TYPE_SLOW 3
#define BOSS_TYPE_STRONG 4

extern Boss *boss;
void BOSS_Init(void);
void BOSS_LoadAnimations(const char *dat_name, int total_anims);
void BOSS_Load(const char *dat_name, int x, int y, int type, int face_gfx_id, int portait_gfx_id, int feet_gfx_id, int body_gfx_id, int head_gfx_id, int larm_gfx_id, int rarm_gfx_id, int facing, int gun_type, int gun_graphics_id, int bullet_graphics_id, int behavior, int life);
void BOSS_SetPosition(byte number, int x, int y);
void BOSS_Unload(void);
void BOSS_Update(void);
void BOSS_SetBehavior(int behavior);


#endif
