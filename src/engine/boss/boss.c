#include "../engine.h"
#include "stdbool.h"
#include <iso646.h>
#include <stdlib.h>
#include "boss.h"

Boss *boss;

void BOSS_LoadAnimations(const char *dat_name, int total_anims) {
	GFX_LoadAnimations(dat_name, "FEET.ANI", boss->feet_animation, total_anims);     // feet animations
	GFX_LoadAnimations(dat_name, "BODY.ANI", boss->body_animation, total_anims);     // body animations
	GFX_LoadAnimations(dat_name, "HEAD.ANI", boss->head_animation, total_anims);     // head animations
	GFX_LoadAnimations(dat_name, "LARM.ANI", boss->left_arm_animation, total_anims); // left arm animations
	GFX_LoadAnimations(dat_name, "RARM.ANI", boss->right_arm_animation, total_anims);// right arm animations
}

void BOSS_ResetAnimations(void) {
	int i, j;
	for (i = 0; i < 128; i++) {
		boss->feet_animation[i].speed = 20;
		boss->feet_animation[i].inverted = false;
		boss->feet_animation[i].end = false;
		boss->feet_animation[i].current_frame = 0;
		boss->feet_animation[i].frames = 1;
		for (j = 0; j < 20; j++) {
			boss->feet_animation[i].anim_data[j].frame = 0;
			boss->feet_animation[i].anim_data[j].offset_x = 0;
			boss->feet_animation[i].anim_data[j].offset_y = 0;
		}
	}
}

void BOSS_Init(void) {
	boss = MM_PushChunk(sizeof(Boss), CT_ENGINE);
	boss->gun = MM_PushChunk(sizeof(Gun), CT_ENGINE);
	boss->cfg = MM_PushChunk(sizeof(SpriteConfig), CT_ENGINE);

	boss->feet_animation = MM_PushChunk(sizeof(SpriteAnimation) * 10, CT_ENGINE);
	boss->body_animation = MM_PushChunk(sizeof(SpriteAnimation) * 10, CT_ENGINE);
	boss->head_animation = MM_PushChunk(sizeof(SpriteAnimation) * 10, CT_ENGINE);
	boss->left_arm_animation = MM_PushChunk(sizeof(SpriteAnimation) * 10, CT_ENGINE);
	boss->right_arm_animation = MM_PushChunk(sizeof(SpriteAnimation) * 10, CT_ENGINE);

	boss->boss_hold_on_pattern = MM_PushChunk(sizeof(int) * 6, CT_ENGINE);
	boss->boss_chase_pattern = MM_PushChunk(sizeof(int) * 6, CT_ENGINE);
	boss->boss_attack_pattern = MM_PushChunk(sizeof(int) * 6, CT_ENGINE);
	boss->boss_rampage_pattern = MM_PushChunk(sizeof(int) * 6, CT_ENGINE);
	boss->boss_static_pattern = MM_PushChunk(sizeof(int) * 6, CT_ENGINE);

	boss->boss_hold_on_pattern[0] = BOSS_PATTERN_HOLD_ON;
	boss->boss_hold_on_pattern[1] = BOSS_PATTERN_HOLD_ON;
	boss->boss_hold_on_pattern[2] = BOSS_PATTERN_HOLD_ON;
	boss->boss_hold_on_pattern[3] = BOSS_PATTERN_HOLD_ON;
	boss->boss_hold_on_pattern[4] = BOSS_PATTERN_HOLD_ON;
	boss->boss_hold_on_pattern[5] = BOSS_PATTERN_HOLD_ON;

	boss->boss_chase_pattern[0] = BOSS_PATTERN_WALK_FORWARD;
	boss->boss_chase_pattern[1] = BOSS_PATTERN_WALK_FORWARD;
	boss->boss_chase_pattern[2] = BOSS_PATTERN_WALK_FORWARD;
	boss->boss_chase_pattern[3] = BOSS_PATTERN_WALK_ARROUND;
	boss->boss_chase_pattern[4] = BOSS_PATTERN_WALK_FORWARD;
	boss->boss_chase_pattern[5] = BOSS_PATTERN_WALK_FORWARD;

	boss->boss_attack_pattern[0] = BOSS_PATTERN_ATTACK;
	boss->boss_attack_pattern[1] = BOSS_PATTERN_ATTACK;
	boss->boss_attack_pattern[2] = BOSS_PATTERN_HOLD_ON;
	boss->boss_attack_pattern[3] = BOSS_PATTERN_ATTACK;
	boss->boss_attack_pattern[4] = BOSS_PATTERN_ATTACK;
	boss->boss_attack_pattern[5] = BOSS_PATTERN_HOLD_ON;

	boss->boss_rampage_pattern[0] = BOSS_PATTERN_ATTACK;
	boss->boss_rampage_pattern[1] = BOSS_PATTERN_ATTACK;
	boss->boss_rampage_pattern[2] = BOSS_PATTERN_WALK_FORWARD;
	boss->boss_rampage_pattern[3] = BOSS_PATTERN_ATTACK;
	boss->boss_rampage_pattern[4] = BOSS_PATTERN_ATTACK;
	boss->boss_rampage_pattern[5] = BOSS_PATTERN_WALK_FORWARD;

	boss->boss_static_pattern[0] = BOSS_PATTERN_HOLD_ON;
	boss->boss_static_pattern[1] = BOSS_PATTERN_HOLD_ON;
	boss->boss_static_pattern[2] = BOSS_PATTERN_HOLD_ON;
	boss->boss_static_pattern[3] = BOSS_PATTERN_HOLD_ON;
	boss->boss_static_pattern[4] = BOSS_PATTERN_HOLD_ON;
	boss->boss_static_pattern[5] = BOSS_PATTERN_HOLD_ON;

	boss->is_loaded = false;
	boss->sprite_num = -1;
	BOSS_ResetAnimations();
}

void BOSS_SetBehavior(int behavior) {
	boss->status_behavior = behavior;
}

/** BOSS :: Initializes boss data
 * - Sets default values for the enemy structure 
 */
void BOSS_Load(const char *dat_name, int x, int y, int type, int face_gfx_id, int portait_gfx_id, int feet_gfx_id, int body_gfx_id, int head_gfx_id, int larm_gfx_id, int rarm_gfx_id, int facing, int gun_type, int gun_graphics_id, int bullet_graphics_id, int behavior, int life) {

	int sprite_slot;

	// Check if enemy is already loaded
	if (boss->is_loaded) {
		sprintf(engine.system_error_message1, "BOSS_Load function error");
		sprintf(engine.system_error_message2, "Boss already loaded");
		sprintf(engine.system_error_message3, " ");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_SYSTEM);
	}

	// Load configuration
	FILE_LoadSpriteConfigFile(dat_name, "BOSS.CFG", boss->cfg);

	boss->width_px = boss->cfg->width;
	boss->height_px = boss->cfg->height;

	// Load graphics
	if (!gfx_sprite_graphics_stack[face_gfx_id].loaded) GFX_LoadSpriteGraphicsRLE(dat_name, "FACE.PCX", face_gfx_id, boss->cfg->face_width, boss->cfg->face_height, boss->cfg->face_frames, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_TEMPORARY_SPRITE);
	if (!gfx_sprite_graphics_stack[portait_gfx_id].loaded) GFX_LoadSpriteGraphicsRLE(dat_name, "PORTAIT.PCX", portait_gfx_id, boss->cfg->portait_width, boss->cfg->portait_height, boss->cfg->portait_frames, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_TEMPORARY_SPRITE);
	if (!gfx_sprite_graphics_stack[feet_gfx_id].loaded) GFX_LoadSpriteGraphicsRLE(dat_name, "FEET.PCX", feet_gfx_id, boss->cfg->feet_width, boss->cfg->feet_height, boss->cfg->feet_frames, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_TEMPORARY_SPRITE);
	if (!gfx_sprite_graphics_stack[body_gfx_id].loaded) GFX_LoadSpriteGraphicsRLE(dat_name, "BODY.PCX", body_gfx_id, boss->cfg->body_width, boss->cfg->body_height, boss->cfg->body_frames, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_TEMPORARY_SPRITE);
	if (!gfx_sprite_graphics_stack[head_gfx_id].loaded) GFX_LoadSpriteGraphicsRLE(dat_name, "HEAD.PCX", head_gfx_id, boss->cfg->head_width, boss->cfg->head_height, boss->cfg->head_frames, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_TEMPORARY_SPRITE);
	if (!gfx_sprite_graphics_stack[larm_gfx_id].loaded) GFX_LoadSpriteGraphicsRLE(dat_name, "LARM.PCX", larm_gfx_id, boss->cfg->larm_width, boss->cfg->larm_height, boss->cfg->larm_frames, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_TEMPORARY_SPRITE);
	if (!gfx_sprite_graphics_stack[rarm_gfx_id].loaded) GFX_LoadSpriteGraphicsRLE(dat_name, "RARM.PCX", rarm_gfx_id, boss->cfg->rarm_width, boss->cfg->rarm_height, boss->cfg->rarm_frames, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_TEMPORARY_SPRITE);

	boss->is_loaded = true;

	boss->pos_x = x;
	boss->pos_y = y;
	boss->x_FP = x << FP;
	boss->y_FP = y << FP;
	boss->vx_FP = 0;
	boss->vy_FP = 0;

	boss->speed_horizontal_FP = BOSS_SPEED << FP;
	boss->speed_vertical_FP = (BOSS_SPEED << FP) - ((BOSS_SPEED << FP) >> 3);
	boss->speed_diagonalv_FP = (boss->speed_vertical_FP * 10) / 14;
	boss->speed_diagonalh_FP = (boss->speed_horizontal_FP * 10) / 14;

	boss->max_life = life;
	boss->life = life;
	boss->damage = 0;
	boss->is_hit = false;

	boss->movement_allowed = true;

	boss->action_shoot = false;
	boss->action_punch = false;
	boss->action_stand = false;
	boss->action_walk = false;
	boss->action_hit = false;
	boss->action_dead = false;

	boss->status_behavior = behavior;

	boss->gfx_portait = portait_gfx_id;

	sprite_slot = GFX_FindEmptySpriteSlot();
	if (sprite_slot == -1) {
		sprintf(engine.system_error_message1, "BOSS_Load function error");
		sprintf(engine.system_error_message2, "No empty sprite slot available");
		sprintf(engine.system_error_message3, " ");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
	} else {
		boss->sprite_num = sprite_slot;
		GFX_InitSprite(ENTITY_ID_BOSS, 0, sprite_slot, 0, boss->cfg->width, boss->cfg->height);
	}

	// Set colission points
	//  ----------------
	// |                |
	// |                |
	// |                |
	// |  0          1  |
	// |                |
	// |                |
	// |                |
	// |  2          3  |
	//  ----------------
	// Point 1
	boss->colission_area.points[0][0] = gfx_sprite_stack[boss->sprite_num].width_px >> 3;
	boss->colission_area.points[0][1] = (gfx_sprite_stack[boss->sprite_num].height_px >> 1) + (gfx_sprite_stack[boss->sprite_num].height_px >> 3);
	// Point 2
	boss->colission_area.points[1][0] = gfx_sprite_stack[boss->sprite_num].width_px - (gfx_sprite_stack[boss->sprite_num].width_px >> 2);
	boss->colission_area.points[1][1] = (gfx_sprite_stack[boss->sprite_num].height_px >> 1) + (gfx_sprite_stack[boss->sprite_num].height_px >> 3);

	// Point 3
	boss->colission_area.points[2][0] = gfx_sprite_stack[boss->sprite_num].width_px >> 3;
	boss->colission_area.points[2][1] = gfx_sprite_stack[boss->sprite_num].height_px;
	// Point 4
	boss->colission_area.points[3][0] = gfx_sprite_stack[boss->sprite_num].width_px - (gfx_sprite_stack[boss->sprite_num].width_px >> 2);
	boss->colission_area.points[3][1] = gfx_sprite_stack[boss->sprite_num].height_px;

	// Set hit box
	//  ----------------
	// |                |
	// |                |
	// |  0          1  |
	// |                |
	// |                |
	// |                |
	// |                |
	// |  2          3  |
	//  ----------------
	// Point 1
	boss->hit_area.points[0][0] = gfx_sprite_stack[boss->sprite_num].width_px >> 3;
	boss->hit_area.points[0][1] = (gfx_sprite_stack[boss->sprite_num].height_px >> 1) - (gfx_sprite_stack[boss->sprite_num].height_px >> 3);
	// Point 2
	boss->hit_area.points[1][0] = gfx_sprite_stack[boss->sprite_num].width_px - (gfx_sprite_stack[boss->sprite_num].width_px >> 2);
	boss->hit_area.points[1][1] = (gfx_sprite_stack[boss->sprite_num].height_px >> 1) - (gfx_sprite_stack[boss->sprite_num].height_px >> 3);
	// Point 3
	boss->hit_area.points[2][0] = gfx_sprite_stack[boss->sprite_num].width_px >> 3;
	boss->hit_area.points[2][1] = gfx_sprite_stack[boss->sprite_num].height_px;
	// Point 4
	boss->hit_area.points[3][0] = gfx_sprite_stack[boss->sprite_num].width_px - (gfx_sprite_stack[boss->sprite_num].width_px >> 2);
	boss->hit_area.points[3][1] = gfx_sprite_stack[boss->sprite_num].height_px;

	GFX_SetSpriteGraphic(boss->sprite_num, 0, feet_gfx_id, 0, 0);// feet
	GFX_SetSpriteGraphic(boss->sprite_num, 1, body_gfx_id, 0, 0);// body
	GFX_SetSpriteGraphic(boss->sprite_num, 2, head_gfx_id, 0, 0);// head
	GFX_SetSpriteGraphic(boss->sprite_num, 3, larm_gfx_id, 0, 0);// left arm
	GFX_SetSpriteGraphic(boss->sprite_num, 4, rarm_gfx_id, 0, 0);// right arm

	gfx_sprite_stack[boss->sprite_num].screen_pos_x = boss->pos_x - camera->pos_x;
	gfx_sprite_stack[boss->sprite_num].screen_pos_y = boss->pos_y - camera->pos_y;

	boss->gun->graphics_id = gun_graphics_id;
	boss->gun->bullet_graphics_id = bullet_graphics_id;
	boss->gun->max_accuracy = 4;
	boss->gun->bullet_speed = 5;
	boss->gun->damage = 20;
	boss->gun->max_distance = 300;

	// Patterns
	boss->pattern_step = 0;
	boss->current_pattern = BOSS_PATTERN_HOLD_ON;

	// Load animations
	BOSS_LoadAnimations(dat_name, 10);

	switch (type) {
		case BOSS_TYPE_FAST_SPEED:
			// Load patterns
			boss->boss_hold_on_pattern[0] = BOSS_PATTERN_HOLD_ON;
			boss->boss_hold_on_pattern[1] = BOSS_PATTERN_WALK_ARROUND;
			boss->boss_hold_on_pattern[2] = BOSS_PATTERN_HOLD_ON;
			boss->boss_hold_on_pattern[3] = BOSS_PATTERN_HOLD_ON;
			boss->boss_hold_on_pattern[4] = BOSS_PATTERN_WALK_ARROUND;
			boss->boss_hold_on_pattern[5] = BOSS_PATTERN_HOLD_ON;

			boss->boss_chase_pattern[0] = BOSS_PATTERN_WALK_FORWARD;
			boss->boss_chase_pattern[1] = BOSS_PATTERN_WALK_FORWARD;
			boss->boss_chase_pattern[2] = BOSS_PATTERN_WALK_FORWARD;
			boss->boss_chase_pattern[3] = BOSS_PATTERN_WALK_ARROUND;
			boss->boss_chase_pattern[4] = BOSS_PATTERN_WALK_FORWARD;
			boss->boss_chase_pattern[5] = BOSS_PATTERN_WALK_FORWARD;

			boss->boss_attack_pattern[0] = BOSS_PATTERN_ATTACK;
			boss->boss_attack_pattern[1] = BOSS_PATTERN_WALK_FORWARD;
			boss->boss_attack_pattern[2] = BOSS_PATTERN_ATTACK;
			boss->boss_attack_pattern[3] = BOSS_PATTERN_WALK_FORWARD;
			boss->boss_attack_pattern[4] = BOSS_PATTERN_ATTACK;
			boss->boss_attack_pattern[5] = BOSS_PATTERN_WALK_FORWARD;

			boss->boss_rampage_pattern[0] = BOSS_PATTERN_ATTACK;
			boss->boss_rampage_pattern[1] = BOSS_PATTERN_ATTACK;
			boss->boss_rampage_pattern[2] = BOSS_PATTERN_WALK_FORWARD;
			boss->boss_rampage_pattern[3] = BOSS_PATTERN_ATTACK;
			boss->boss_rampage_pattern[4] = BOSS_PATTERN_ATTACK;
			boss->boss_rampage_pattern[5] = BOSS_PATTERN_WALK_FORWARD;

			boss->boss_static_pattern[0] = BOSS_PATTERN_HOLD_ON;
			boss->boss_static_pattern[1] = BOSS_PATTERN_HOLD_ON;
			boss->boss_static_pattern[2] = BOSS_PATTERN_HOLD_ON;
			boss->boss_static_pattern[3] = BOSS_PATTERN_HOLD_ON;
			boss->boss_static_pattern[4] = BOSS_PATTERN_HOLD_ON;
			boss->boss_static_pattern[5] = BOSS_PATTERN_HOLD_ON;

			boss->speed = 12;
			boss->reaction_time = 6;
			boss->punch_range = 32;
			boss->shoot_range = 92;

			break;

		case BOSS_TYPE_MEDIUM_SPEED:
			// Load patterns
			boss->boss_hold_on_pattern[0] = BOSS_PATTERN_HOLD_ON;
			boss->boss_hold_on_pattern[1] = BOSS_PATTERN_HOLD_ON;
			boss->boss_hold_on_pattern[2] = BOSS_PATTERN_HOLD_ON;
			boss->boss_hold_on_pattern[3] = BOSS_PATTERN_HOLD_ON;
			boss->boss_hold_on_pattern[4] = BOSS_PATTERN_HOLD_ON;
			boss->boss_hold_on_pattern[5] = BOSS_PATTERN_HOLD_ON;

			boss->boss_chase_pattern[0] = BOSS_PATTERN_WALK_FORWARD;
			boss->boss_chase_pattern[1] = BOSS_PATTERN_WALK_FORWARD;
			boss->boss_chase_pattern[2] = BOSS_PATTERN_WALK_FORWARD;
			boss->boss_chase_pattern[3] = BOSS_PATTERN_WALK_ARROUND;
			boss->boss_chase_pattern[4] = BOSS_PATTERN_WALK_FORWARD;
			boss->boss_chase_pattern[5] = BOSS_PATTERN_WALK_FORWARD;

			boss->boss_attack_pattern[0] = BOSS_PATTERN_ATTACK;
			boss->boss_attack_pattern[1] = BOSS_PATTERN_ATTACK;
			boss->boss_attack_pattern[2] = BOSS_PATTERN_WALK_FORWARD;
			boss->boss_attack_pattern[3] = BOSS_PATTERN_ATTACK;
			boss->boss_attack_pattern[4] = BOSS_PATTERN_ATTACK;
			boss->boss_attack_pattern[5] = BOSS_PATTERN_HOLD_ON;

			boss->boss_rampage_pattern[0] = BOSS_PATTERN_ATTACK;
			boss->boss_rampage_pattern[1] = BOSS_PATTERN_ATTACK;
			boss->boss_rampage_pattern[2] = BOSS_PATTERN_WALK_FORWARD;
			boss->boss_rampage_pattern[3] = BOSS_PATTERN_ATTACK;
			boss->boss_rampage_pattern[4] = BOSS_PATTERN_ATTACK;
			boss->boss_rampage_pattern[5] = BOSS_PATTERN_WALK_FORWARD;

			boss->boss_static_pattern[0] = BOSS_PATTERN_HOLD_ON;
			boss->boss_static_pattern[1] = BOSS_PATTERN_HOLD_ON;
			boss->boss_static_pattern[2] = BOSS_PATTERN_HOLD_ON;
			boss->boss_static_pattern[3] = BOSS_PATTERN_HOLD_ON;
			boss->boss_static_pattern[4] = BOSS_PATTERN_HOLD_ON;
			boss->boss_static_pattern[5] = BOSS_PATTERN_HOLD_ON;

			boss->speed = 10;
			boss->reaction_time = 8;
			boss->punch_range = 32;
			boss->shoot_range = 92;

			break;
		case BOSS_TYPE_SLOW:
			// Load patterns
			boss->boss_hold_on_pattern[0] = BOSS_PATTERN_HOLD_ON;
			boss->boss_hold_on_pattern[1] = BOSS_PATTERN_HOLD_ON;
			boss->boss_hold_on_pattern[2] = BOSS_PATTERN_HOLD_ON;
			boss->boss_hold_on_pattern[3] = BOSS_PATTERN_HOLD_ON;
			boss->boss_hold_on_pattern[4] = BOSS_PATTERN_HOLD_ON;
			boss->boss_hold_on_pattern[5] = BOSS_PATTERN_HOLD_ON;

			boss->boss_chase_pattern[0] = BOSS_PATTERN_WALK_FORWARD;
			boss->boss_chase_pattern[1] = BOSS_PATTERN_WALK_FORWARD;
			boss->boss_chase_pattern[2] = BOSS_PATTERN_WALK_FORWARD;
			boss->boss_chase_pattern[3] = BOSS_PATTERN_WALK_ARROUND;
			boss->boss_chase_pattern[4] = BOSS_PATTERN_WALK_FORWARD;
			boss->boss_chase_pattern[5] = BOSS_PATTERN_WALK_FORWARD;

			boss->boss_attack_pattern[0] = BOSS_PATTERN_ATTACK;
			boss->boss_attack_pattern[1] = BOSS_PATTERN_HOLD_ON;
			boss->boss_attack_pattern[2] = BOSS_PATTERN_ATTACK;
			boss->boss_attack_pattern[3] = BOSS_PATTERN_WALK_FORWARD;
			boss->boss_attack_pattern[4] = BOSS_PATTERN_ATTACK;
			boss->boss_attack_pattern[5] = BOSS_PATTERN_HOLD_ON;

			boss->boss_rampage_pattern[0] = BOSS_PATTERN_ATTACK;
			boss->boss_rampage_pattern[1] = BOSS_PATTERN_ATTACK;
			boss->boss_rampage_pattern[2] = BOSS_PATTERN_HOLD_ON;
			boss->boss_rampage_pattern[3] = BOSS_PATTERN_ATTACK;
			boss->boss_rampage_pattern[4] = BOSS_PATTERN_ATTACK;
			boss->boss_rampage_pattern[5] = BOSS_PATTERN_WALK_FORWARD;

			boss->boss_static_pattern[0] = BOSS_PATTERN_HOLD_ON;
			boss->boss_static_pattern[1] = BOSS_PATTERN_HOLD_ON;
			boss->boss_static_pattern[2] = BOSS_PATTERN_HOLD_ON;
			boss->boss_static_pattern[3] = BOSS_PATTERN_HOLD_ON;
			boss->boss_static_pattern[4] = BOSS_PATTERN_HOLD_ON;
			boss->boss_static_pattern[5] = BOSS_PATTERN_HOLD_ON;

			boss->speed = 3;
			boss->reaction_time = 15;
			boss->punch_range = 32;
			boss->shoot_range = 92;

			break;
		case BOSS_TYPE_STRONG:
			// Load patterns
			boss->boss_hold_on_pattern[0] = BOSS_PATTERN_HOLD_ON;
			boss->boss_hold_on_pattern[1] = BOSS_PATTERN_HOLD_ON;
			boss->boss_hold_on_pattern[2] = BOSS_PATTERN_HOLD_ON;
			boss->boss_hold_on_pattern[3] = BOSS_PATTERN_HOLD_ON;
			boss->boss_hold_on_pattern[4] = BOSS_PATTERN_HOLD_ON;
			boss->boss_hold_on_pattern[5] = BOSS_PATTERN_HOLD_ON;

			boss->boss_chase_pattern[0] = BOSS_PATTERN_WALK_FORWARD;
			boss->boss_chase_pattern[1] = BOSS_PATTERN_WALK_FORWARD;
			boss->boss_chase_pattern[2] = BOSS_PATTERN_WALK_FORWARD;
			boss->boss_chase_pattern[3] = BOSS_PATTERN_WALK_ARROUND;
			boss->boss_chase_pattern[4] = BOSS_PATTERN_WALK_FORWARD;
			boss->boss_chase_pattern[5] = BOSS_PATTERN_WALK_FORWARD;

			boss->boss_attack_pattern[0] = BOSS_PATTERN_ATTACK;
			boss->boss_attack_pattern[1] = BOSS_PATTERN_ATTACK;
			boss->boss_attack_pattern[2] = BOSS_PATTERN_WALK_FORWARD;
			boss->boss_attack_pattern[3] = BOSS_PATTERN_ATTACK;
			boss->boss_attack_pattern[4] = BOSS_PATTERN_ATTACK;
			boss->boss_attack_pattern[5] = BOSS_PATTERN_WALK_FORWARD;

			boss->boss_rampage_pattern[0] = BOSS_PATTERN_ATTACK;
			boss->boss_rampage_pattern[1] = BOSS_PATTERN_ATTACK;
			boss->boss_rampage_pattern[2] = BOSS_PATTERN_WALK_FORWARD;
			boss->boss_rampage_pattern[3] = BOSS_PATTERN_ATTACK;
			boss->boss_rampage_pattern[4] = BOSS_PATTERN_ATTACK;
			boss->boss_rampage_pattern[5] = BOSS_PATTERN_WALK_FORWARD;

			boss->boss_static_pattern[0] = BOSS_PATTERN_HOLD_ON;
			boss->boss_static_pattern[1] = BOSS_PATTERN_HOLD_ON;
			boss->boss_static_pattern[2] = BOSS_PATTERN_HOLD_ON;
			boss->boss_static_pattern[3] = BOSS_PATTERN_HOLD_ON;
			boss->boss_static_pattern[4] = BOSS_PATTERN_HOLD_ON;
			boss->boss_static_pattern[5] = BOSS_PATTERN_HOLD_ON;

			boss->speed = 4;
			boss->reaction_time = 8;
			boss->punch_range = 32;
			boss->shoot_range = 102;

			break;
	}
}

void BOSS_SetPosition(byte number, int x, int y) {
	if (boss->is_loaded) {
		boss->pos_x = x;
		boss->pos_y = y;
		boss->x_FP = x << FP;
		boss->y_FP = y << FP;
		boss->vx_FP = 0;
		boss->vy_FP = 0;
	}
}

void BOSS_DrawColissionPixels(void) {
	//  Draw colission pixels
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[boss->sprite_num].screen_pos_x + boss->colission_area.points[0][0], gfx_sprite_stack[boss->sprite_num].screen_pos_y + boss->colission_area.points[0][1], 228);
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[boss->sprite_num].screen_pos_x + boss->colission_area.points[1][0], gfx_sprite_stack[boss->sprite_num].screen_pos_y + boss->colission_area.points[1][1], 228);
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[boss->sprite_num].screen_pos_x + boss->colission_area.points[2][0], gfx_sprite_stack[boss->sprite_num].screen_pos_y + boss->colission_area.points[2][1], 228);
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[boss->sprite_num].screen_pos_x + boss->colission_area.points[3][0], gfx_sprite_stack[boss->sprite_num].screen_pos_y + boss->colission_area.points[3][1], 228);
}

void BOSS_DrawHitPixels(void) {
	//  Draw colission pixels
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[boss->sprite_num].screen_pos_x + boss->hit_area.points[0][0], gfx_sprite_stack[boss->sprite_num].screen_pos_y + boss->hit_area.points[0][1], 229);
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[boss->sprite_num].screen_pos_x + boss->hit_area.points[1][0], gfx_sprite_stack[boss->sprite_num].screen_pos_y + boss->hit_area.points[1][1], 229);
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[boss->sprite_num].screen_pos_x + boss->hit_area.points[2][0], gfx_sprite_stack[boss->sprite_num].screen_pos_y + boss->hit_area.points[2][1], 229);
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[boss->sprite_num].screen_pos_x + boss->hit_area.points[3][0], gfx_sprite_stack[boss->sprite_num].screen_pos_y + boss->hit_area.points[3][1], 229);
}

bool BOSS_CheckUpColission(void) {
	int i;
	int point1_x, point1_y;
	int point2_x, point2_y;

	point1_x = boss->pos_x + boss->colission_area.points[0][0];
	point1_y = boss->pos_y + boss->colission_area.points[0][1];
	point2_x = boss->pos_x + boss->colission_area.points[1][0];
	point2_y = boss->pos_y + boss->colission_area.points[1][1];

	/////// BACKGROUND COLISSIONS ///////////
	if (MAP_CheckColissionTile(point1_x, point1_y)) { return true; }// Check point 1
	if (MAP_CheckColissionTile(point2_x, point2_y)) { return true; }// Check point 2

	/////// OBJECTS COLISSIONS ///////////
	for (i = 0; i < OBJECT_MAX_OBJECTS; i++) {
		if (object[i].is_loaded) {
			if (gfx_sprite_stack[object[i].num_sprite].shown) {
				//Point 1
				if (point1_x <= object[i].pos_x + object[i].colission_area.points[3][0]

					&& point1_x >= object[i].pos_x + object[i].colission_area.points[2][0]

					&& point1_y >= object[i].pos_y + object[i].colission_area.points[1][1]

					&& point1_y <= object[i].pos_y + object[i].colission_area.points[3][1]) {
					return true;
				}

				//Point 2
				if (point2_x <= object[i].pos_x + object[i].colission_area.points[3][0]

					&& point2_x >= object[i].pos_x + object[i].colission_area.points[2][0]

					&& point2_y >= object[i].pos_y + object[i].colission_area.points[1][1]

					&& point2_y <= object[i].pos_y + object[i].colission_area.points[3][1]) {
					return true;
				}
			}
		}
	}

	/////// ENEMIES COLISSIONS ///////////
	for (i = 0; i < ENEMY_MAX_ENEMIES; i++) {
		if (enemy[i].is_loaded & !enemy[i].action_dead) {
			if (gfx_sprite_stack[enemy[i].sprite_num].shown) {
				//Point 1
				if (point1_x <= enemy[i].pos_x + enemy[i].colission_area.points[3][0]

					&& point1_x >= enemy[i].pos_x + enemy[i].colission_area.points[3][0]

					&& point1_y >= enemy[i].pos_y + enemy[i].colission_area.points[1][1]

					&& point1_y <= enemy[i].pos_y + enemy[i].colission_area.points[3][1]) {
					return true;
				}

				//Point 2
				if (point2_x <= enemy[i].pos_x + enemy[i].colission_area.points[3][0]

					&& point2_x >= enemy[i].pos_x + enemy[i].colission_area.points[2][0]

					&& point2_y >= enemy[i].pos_y + enemy[i].colission_area.points[1][1]

					&& point2_y <= enemy[i].pos_y + enemy[i].colission_area.points[3][1]) {
					return true;
				}
			}
		}
	}

	/////// ACTOR COLISSIONS ///////////
	//Point 1
	if (point1_x <= actor->pos_x + actor->colission_area.points[3][0]

		&& point1_x >= actor->pos_x + actor->colission_area.points[2][0]

		&& point1_y >= actor->pos_y + actor->colission_area.points[1][1]

		&& point1_y <= actor->pos_y + actor->colission_area.points[3][1]) {
		return true;
	}

	//Point 2
	if (point2_x <= actor->pos_x + actor->colission_area.points[3][0]

		&& point2_x >= actor->pos_x + actor->colission_area.points[2][0]

		&& point2_y >= actor->pos_y + actor->colission_area.points[1][1]

		&& point2_y <= actor->pos_y + actor->colission_area.points[3][1]) {
		return true;
	}

	return false;
}

bool BOSS_CheckDownColission(void) {
	int i;

	int point1_x, point1_y;
	int point2_x, point2_y;

	point1_x = boss->pos_x + boss->colission_area.points[2][0];
	point1_y = boss->pos_y + boss->colission_area.points[2][1];
	point2_x = boss->pos_x + boss->colission_area.points[3][0];
	point2_y = boss->pos_y + boss->colission_area.points[3][1];

	if (MAP_CheckColissionTile(point1_x, point1_y)) { return true; }// Check point 1
	if (MAP_CheckColissionTile(point2_x, point2_y)) { return true; }// Check point 2

	/////// OBJECTS COLISSIONS ///////////
	for (i = 0; i < OBJECT_MAX_OBJECTS; i++) {
		if (object[i].is_loaded) {
			if (gfx_sprite_stack[object[i].num_sprite].shown) {
				//Point 1
				if (point1_x <= object[i].pos_x + object[i].colission_area.points[1][0]

					&& point1_x >= object[i].pos_x + object[i].colission_area.points[0][0]

					&& point1_y >= object[i].pos_y + object[i].colission_area.points[1][1]

					&& point1_y <= object[i].pos_y + object[i].colission_area.points[3][1]) {
					return true;
				}

				//Point 2
				if (point2_x <= object[i].pos_x + object[i].colission_area.points[1][0]

					&& point2_x >= object[i].pos_x + object[i].colission_area.points[0][0]

					&& point2_y >= object[i].pos_y + object[i].colission_area.points[1][1]

					&& point2_y <= object[i].pos_y + object[i].colission_area.points[3][1]) {
					return true;
				}
			}
		}
	}

	/////// ENEMIES COLISSIONS ///////////
	for (i = 0; i < ENEMY_MAX_ENEMIES; i++) {
		if (enemy[i].is_loaded & !enemy[i].action_dead) {
			if (gfx_sprite_stack[enemy[i].sprite_num].shown) {
				//Point 1
				if (point1_x <= enemy[i].pos_x + enemy[i].colission_area.points[1][0]

					&& point1_x >= enemy[i].pos_x + enemy[i].colission_area.points[0][0]

					&& point1_y >= enemy[i].pos_y + enemy[i].colission_area.points[1][1]

					&& point1_y <= enemy[i].pos_y + enemy[i].colission_area.points[3][1]) {
					return true;
				}

				//Point 2
				if (point2_x <= enemy[i].pos_x + enemy[i].colission_area.points[1][0]

					&& point2_x >= enemy[i].pos_x + enemy[i].colission_area.points[0][0]

					&& point2_y >= enemy[i].pos_y + enemy[i].colission_area.points[1][1]

					&& point2_y <= enemy[i].pos_y + enemy[i].colission_area.points[3][1]) {
					return true;
				}
			}
		}
	}

	/////// ACTOR COLISSIONS ///////////
	//Point 1
	if (point1_x <= actor->pos_x + actor->colission_area.points[1][0]

		&& point1_x >= actor->pos_x + actor->colission_area.points[0][0]

		&& point1_y >= actor->pos_y + actor->colission_area.points[1][1]

		&& point1_y <= actor->pos_y + actor->colission_area.points[3][1]) {
		return true;
	}

	//Point 2
	if (point2_x <= actor->pos_x + actor->colission_area.points[1][0]

		&& point2_x >= actor->pos_x + actor->colission_area.points[0][0]

		&& point2_y >= actor->pos_y + actor->colission_area.points[1][1]

		&& point2_y <= actor->pos_y + actor->colission_area.points[3][1]) {
		return true;
	}

	return false;
}

bool BOSS_CheckLeftColission(void) {
	int i;

	int point1_x, point1_y;
	int point2_x, point2_y;

	point1_x = boss->pos_x + boss->colission_area.points[0][0];
	point1_y = boss->pos_y + boss->colission_area.points[0][1];
	point2_x = boss->pos_x + boss->colission_area.points[2][0];
	point2_y = boss->pos_y + boss->colission_area.points[2][1];

	if (MAP_CheckColissionTile(point1_x, point1_y)) { return true; }// Check point 1
	if (MAP_CheckColissionTile(point2_x, point2_y)) { return true; }// Check point 2

	/////// OBJECTS COLISSIONS ///////////
	for (i = 0; i < OBJECT_MAX_OBJECTS; i++) {
		if (object[i].is_loaded) {
			if (gfx_sprite_stack[object[i].num_sprite].shown) {
				//Point 1
				if (point1_x <= object[i].pos_x + object[i].colission_area.points[1][0]

					&& point1_x >= object[i].pos_x + object[i].colission_area.points[0][0]

					&& point1_y >= object[i].pos_y + object[i].colission_area.points[1][1]

					&& point1_y <= object[i].pos_y + object[i].colission_area.points[3][1]) {
					return true;
				}

				//Point 2
				if (point2_x <= object[i].pos_x + object[i].colission_area.points[1][0]

					&& point2_x >= object[i].pos_x + object[i].colission_area.points[0][0]

					&& point2_y >= object[i].pos_y + object[i].colission_area.points[1][1]

					&& point2_y <= object[i].pos_y + object[i].colission_area.points[3][1]) {
					return true;
				}
			}
		}
	}
	for (i = 0; i < ENEMY_MAX_ENEMIES; i++) {
		if (enemy[i].is_loaded && !enemy[i].action_dead) {
			if (gfx_sprite_stack[enemy[i].sprite_num].shown) {
				//Point 1
				if (point1_x <= enemy[i].pos_x + enemy[i].colission_area.points[1][0]

					&& point1_x >= enemy[i].pos_x + enemy[i].colission_area.points[0][0]

					&& point1_y >= enemy[i].pos_y + enemy[i].colission_area.points[0][1]

					&& point1_y <= enemy[i].pos_y + enemy[i].colission_area.points[3][1]) {
					return true;
				}

				//Point 2
				if (point2_x <= enemy[i].pos_x + enemy[i].colission_area.points[1][0]

					&& point2_x >= enemy[i].pos_x + enemy[i].colission_area.points[0][0]

					&& point2_y >= enemy[i].pos_y + enemy[i].colission_area.points[0][1]

					&& point2_y <= enemy[i].pos_y + enemy[i].colission_area.points[3][1]) {
					return true;
				}
			}
		}
	}

	/////// ACTOR COLISSIONS ///////////
	//Point 1
	if (point1_x <= actor->pos_x + actor->colission_area.points[1][0]

		&& point1_x >= actor->pos_x + actor->colission_area.points[0][0]

		&& point1_y >= actor->pos_y + actor->colission_area.points[0][1]

		&& point1_y <= actor->pos_y + actor->colission_area.points[2][1]) {
		return true;
	}

	//Point 2
	if (point2_x <= actor->pos_x + actor->colission_area.points[1][0]

		&& point2_x >= actor->pos_x + actor->colission_area.points[0][0]

		&& point2_y >= actor->pos_y + actor->colission_area.points[0][1]

		&& point2_y <= actor->pos_y + actor->colission_area.points[2][1]) {
		return true;
	}

	return false;
}

bool BOSS_CheckRightColission(void) {
	int i;

	int point1_x, point1_y;
	int point2_x, point2_y;

	point1_x = boss->pos_x + boss->colission_area.points[1][0];
	point1_y = boss->pos_y + boss->colission_area.points[1][1];
	point2_x = boss->pos_x + boss->colission_area.points[3][0];
	point2_y = boss->pos_y + boss->colission_area.points[3][1];

	if (MAP_CheckColissionTile(point1_x, point1_y)) { return true; }// Check point 1
	if (MAP_CheckColissionTile(point2_x, point2_y)) { return true; }// Check point 2

	/////// OBJECTS COLISSIONS ///////////
	for (i = 0; i < OBJECT_MAX_OBJECTS; i++) {
		if (object[i].is_loaded) {
			if (gfx_sprite_stack[object[i].num_sprite].shown) {
				//Point 1
				if (point1_x <= object[i].pos_x + object[i].colission_area.points[1][0]

					&& point1_x >= object[i].pos_x + object[i].colission_area.points[0][0]

					&& point1_y >= object[i].pos_y + object[i].colission_area.points[0][1]

					&& point1_y <= object[i].pos_y + object[i].colission_area.points[2][1]) {
					return true;
				}

				//Point 1
				if (point2_x <= object[i].pos_x + object[i].colission_area.points[1][0]

					&& point2_x >= object[i].pos_x + object[i].colission_area.points[0][0]

					&& point2_y >= object[i].pos_y + object[i].colission_area.points[0][1]

					&& point2_y <= object[i].pos_y + object[i].colission_area.points[2][1]) {
					return true;
				}
			}
		}
	}

	for (i = 0; i < ENEMY_MAX_ENEMIES; i++) {
		if (enemy[i].is_loaded && !enemy[i].action_dead) {
			if (gfx_sprite_stack[enemy[i].sprite_num].shown) {
				//Point 1
				if (point1_x <= enemy[i].pos_x + enemy[i].colission_area.points[1][0]

					&& point1_x >= enemy[i].pos_x + enemy[i].colission_area.points[0][0]

					&& point1_y >= enemy[i].pos_y + enemy[i].colission_area.points[0][1]

					&& point1_y <= enemy[i].pos_y + enemy[i].colission_area.points[2][1]) {
					return true;
				}

				//Point 2
				if (point2_x <= enemy[i].pos_x + enemy[i].colission_area.points[1][0]

					&& point2_x >= enemy[i].pos_x + enemy[i].colission_area.points[0][0]

					&& point2_y >= enemy[i].pos_y + enemy[i].colission_area.points[0][1]

					&& point2_y <= enemy[i].pos_y + enemy[i].colission_area.points[2][1]) {
					return true;
				}
			}
		}
	}

	/////// ACTOR COLISSIONS ///////////
	//Point 1
	if (point1_x <= actor->pos_x + actor->colission_area.points[1][0]

		&& point1_x >= actor->pos_x + actor->colission_area.points[0][0]

		&& point1_y >= actor->pos_y + actor->colission_area.points[0][1]

		&& point1_y <= actor->pos_y + actor->colission_area.points[2][1]) {
		return true;
	}

	//Point 2
	if (point2_x <= actor->pos_x + actor->colission_area.points[1][0]

		&& point2_x >= actor->pos_x + actor->colission_area.points[0][0]

		&& point2_y >= actor->pos_y + actor->colission_area.points[0][1]

		&& point2_y <= actor->pos_y + actor->colission_area.points[2][1]) {
		return true;
	}

	return false;
}

void BOSS_UpdateRangeStatus(void) {
	int i, dx, dy, distance, pos_x, pos_y;
	dword vx_FP, vy_FP, x_FP, y_FP;
	if (gfx_sprite_stack[boss->sprite_num].shown) {
		dx = actor->middle_x - boss->middle_x;
		dy = actor->middle_y - boss->middle_y;

		// calculate steps
		if (abs(dx) > abs(dy))
			distance = abs(dx);
		else
			distance = abs(dy);

		// Avoid 0 division or die!!
		if (distance == 0) distance = 1;

		// Check if is in shoot range
		if (distance < boss->punch_range) {
			boss->in_punch_range = true;
			boss->in_shoot_range = false;
		} else if (distance < boss->shoot_range) {
			boss->in_shoot_range = true;
			boss->in_punch_range = false;
		} else {
			boss->in_shoot_range = false;
			boss->in_punch_range = false;
		}

		// calculate fixed point velocity and position
		vx_FP = (dx << FP) / distance;
		vy_FP = (dy << FP) / distance;

		x_FP = (boss->pos_x + boss->width_px) << FP;
		y_FP = (boss->pos_y + boss->height_px) << FP;

		// Check if can see actor
		boss->can_see_actor = true;
		for (i = 0; i < distance; i++) {
			x_FP += vx_FP;
			y_FP += vy_FP;

			pos_x = (x_FP >> FP);
			pos_y = (y_FP >> FP);

			if (MAP_CheckColissionTile(pos_x, pos_y) == 1) boss->can_see_actor = false;
		}
	} else {
		boss->can_see_actor = false;
		boss->in_punch_range = false;
		boss->in_shoot_range = false;
	}
}

void BOSS_SetPunchAnimation(void) {
	GFX_SetSpriteAnimation(boss->sprite_num, BOSS_ANIM_FEET_INDEX, boss->feet_animation[BOSS_ANIM_PUNCH], true);
	GFX_SetSpriteAnimation(boss->sprite_num, BOSS_ANIM_BODY_INDEX, boss->body_animation[BOSS_ANIM_PUNCH], true);
	GFX_SetSpriteAnimation(boss->sprite_num, BOSS_ANIM_HEAD_INDEX, boss->head_animation[BOSS_ANIM_PUNCH], true);
	GFX_SetSpriteAnimation(boss->sprite_num, BOSS_ANIM_LARM_INDEX, boss->left_arm_animation[BOSS_ANIM_PUNCH], true);
	GFX_SetSpriteAnimation(boss->sprite_num, BOSS_ANIM_RARM_INDEX, boss->right_arm_animation[BOSS_ANIM_PUNCH], true);
}

void BOSS_SetWalkAnimation(void) {
	GFX_SetSpriteAnimation(boss->sprite_num, BOSS_ANIM_FEET_INDEX, boss->feet_animation[BOSS_ANIM_WALK], false);
	GFX_SetSpriteAnimation(boss->sprite_num, BOSS_ANIM_BODY_INDEX, boss->body_animation[BOSS_ANIM_WALK], false);
	GFX_SetSpriteAnimation(boss->sprite_num, BOSS_ANIM_HEAD_INDEX, boss->head_animation[BOSS_ANIM_WALK], false);
	GFX_SetSpriteAnimation(boss->sprite_num, BOSS_ANIM_LARM_INDEX, boss->left_arm_animation[BOSS_ANIM_WALK], false);
	GFX_SetSpriteAnimation(boss->sprite_num, BOSS_ANIM_RARM_INDEX, boss->right_arm_animation[BOSS_ANIM_WALK], false);
}

void BOSS_SetShotAnimation(void) {
	GFX_SetSpriteAnimation(boss->sprite_num, BOSS_ANIM_FEET_INDEX, boss->feet_animation[BOSS_ANIM_SHOT], true);
	GFX_SetSpriteAnimation(boss->sprite_num, BOSS_ANIM_BODY_INDEX, boss->body_animation[BOSS_ANIM_SHOT], true);
	GFX_SetSpriteAnimation(boss->sprite_num, BOSS_ANIM_HEAD_INDEX, boss->head_animation[BOSS_ANIM_SHOT], true);
	GFX_SetSpriteAnimation(boss->sprite_num, BOSS_ANIM_LARM_INDEX, boss->left_arm_animation[BOSS_ANIM_SHOT], true);
	GFX_SetSpriteAnimation(boss->sprite_num, BOSS_ANIM_RARM_INDEX, boss->right_arm_animation[BOSS_ANIM_SHOT], true);
}

void BOSS_SetStandAnimation(void) {
	GFX_SetSpriteAnimation(boss->sprite_num, BOSS_ANIM_FEET_INDEX, boss->feet_animation[BOSS_ANIM_STAND], true);
	GFX_SetSpriteAnimation(boss->sprite_num, BOSS_ANIM_BODY_INDEX, boss->body_animation[BOSS_ANIM_STAND], true);
	GFX_SetSpriteAnimation(boss->sprite_num, BOSS_ANIM_HEAD_INDEX, boss->head_animation[BOSS_ANIM_STAND], true);
	GFX_SetSpriteAnimation(boss->sprite_num, BOSS_ANIM_LARM_INDEX, boss->left_arm_animation[BOSS_ANIM_STAND], true);
	GFX_SetSpriteAnimation(boss->sprite_num, BOSS_ANIM_RARM_INDEX, boss->right_arm_animation[BOSS_ANIM_STAND], true);
}

void BOSS_SetDeadAnimation(void) {
	GFX_SetSpriteAnimation(boss->sprite_num, BOSS_ANIM_FEET_INDEX, boss->feet_animation[BOSS_ANIM_DEAD], false);
	GFX_SetSpriteAnimation(boss->sprite_num, BOSS_ANIM_BODY_INDEX, boss->body_animation[BOSS_ANIM_DEAD], false);
	GFX_SetSpriteAnimation(boss->sprite_num, BOSS_ANIM_HEAD_INDEX, boss->head_animation[BOSS_ANIM_DEAD], false);
	GFX_SetSpriteAnimation(boss->sprite_num, BOSS_ANIM_LARM_INDEX, boss->left_arm_animation[BOSS_ANIM_DEAD], false);
	GFX_SetSpriteAnimation(boss->sprite_num, BOSS_ANIM_RARM_INDEX, boss->right_arm_animation[BOSS_ANIM_DEAD], false);
}

void BOSS_SetHitAnimation(void) {
	GFX_SetSpriteAnimation(boss->sprite_num, BOSS_ANIM_FEET_INDEX, boss->feet_animation[BOSS_ANIM_HIT], true);
	GFX_SetSpriteAnimation(boss->sprite_num, BOSS_ANIM_BODY_INDEX, boss->body_animation[BOSS_ANIM_HIT], true);
	GFX_SetSpriteAnimation(boss->sprite_num, BOSS_ANIM_HEAD_INDEX, boss->head_animation[BOSS_ANIM_HIT], true);
	GFX_SetSpriteAnimation(boss->sprite_num, BOSS_ANIM_LARM_INDEX, boss->left_arm_animation[BOSS_ANIM_HIT], true);
	GFX_SetSpriteAnimation(boss->sprite_num, BOSS_ANIM_RARM_INDEX, boss->right_arm_animation[BOSS_ANIM_HIT], true);
}

void BOSS_MoveAway(int pos_x, int pos_y, byte *movement) {
	int dx, dy, angle;

	dx = boss->middle_x - pos_x;
	dy = boss->middle_y - pos_y;
	angle = UTIL_GetAngle(dx, dy);
	*movement = 0;
	if ((angle > 290) || (angle < 70)) *movement = *movement | 1;        // facing right
	if ((angle > 120) && (angle < 260)) *movement = *movement | (1 << 1);// facing left
	if ((angle > 20) && (angle < 170)) *movement = *movement | (1 << 2); // facing down
	if ((angle > 200) && (angle < 320)) *movement = *movement | (1 << 3);// facing up
}

void BOSS_MoveAhead(int pos_x, int pos_y, byte *movement) {
	int dx, dy, angle;

	dx = pos_x - boss->middle_x;
	dy = pos_y - boss->middle_y;

	angle = UTIL_GetAngle(dx, dy);
	*movement = 0;
	if ((angle > 290) || (angle < 70)) *movement = *movement | 1;        // facing right
	if ((angle > 120) && (angle < 260)) *movement = *movement | (1 << 1);// facing left
	if ((angle > 20) && (angle < 170)) *movement = *movement | (1 << 2); // facing down
	if ((angle > 200) && (angle < 320)) *movement = *movement | (1 << 3);// facing up
}

void BOSS_Unload(void) {
	if (boss->is_loaded) {
		boss->is_loaded = false;
		GFX_UnloadSprite(boss->sprite_num);
		boss->sprite_num = -1;
	}
}

/** BOSS :: In game update
 */
void BOSS_Update(void) {
	bool status_idle;
	int update_boss;

	boss->boss_update_counter++;
	if (boss->boss_update_counter > 6) {
		boss->boss_update_counter = 0;
	}

	boss->boss_counter = 0;

	if (boss->is_loaded) {
		boss->boss_counter++;

		// Calculate boss middle point
		boss->middle_x = GFX_GetSpriteScreenPosX(boss->sprite_num) + (boss->width_px >> 1);
		boss->middle_y = GFX_GetSpriteScreenPosY(boss->sprite_num) + (boss->height_px) + 16;

		// Update boss position on screen
		GFX_SetSpritePosition(boss->sprite_num, boss->pos_x - camera->pos_x, boss->pos_y - camera->pos_y);

		// On screen
		boss->on_screen = GFX_IsSpriteOnScreen(boss->sprite_num);

		// DEBUG: Draw collision and hit pixels
		//if (boss->on_screen) BOSS_DrawColissionPixels();
		//if (boss->on_screen) BOSS_DrawHitPixels();

		// Avoid to update enemies each cycle
		update_boss = (boss->boss_update_counter) & 1;
		if (update_boss) {
			// Update range status
			// - updates boss->can_see_actor
			// - updates boss->in_shoot_range
			BOSS_UpdateRangeStatus();
			if (boss->life < 1000) {
				boss->status_behavior = BOSS_STATUS_RAMPAGE;
				boss->speed = BOSS_RAMPAGE_SPEED;
			}

			// Set current behavior and pattern
			switch (boss->status_behavior) {
				case BOSS_STATUS_CHASE:
					if (boss->in_shoot_range) {
						boss->status_behavior = BOSS_STATUS_LONG_ATTACK;
						boss->pattern_step = 0;
					}
					if (boss->in_punch_range) {
						boss->status_behavior = BOSS_STATUS_SHORT_ATTACK;
						boss->pattern_step = 0;
					}
					break;
				case BOSS_STATUS_SHORT_ATTACK:
					if (!boss->in_punch_range) {
						boss->status_behavior = BOSS_STATUS_CHASE;
						boss->pattern_step = 0;
					}
					if (boss->in_shoot_range) {
						boss->status_behavior = BOSS_STATUS_LONG_ATTACK;
						boss->pattern_step = 0;
					}
					break;
				case BOSS_STATUS_LONG_ATTACK:
					if (!boss->in_shoot_range) {
						boss->status_behavior = BOSS_STATUS_CHASE;
						boss->pattern_step = 0;
					}
					if (boss->in_punch_range) {
						boss->status_behavior = BOSS_STATUS_SHORT_ATTACK;
						boss->pattern_step = 0;
					}
					break;
				case BOSS_STATUS_STATIC:
					break;
				case BOSS_STATUS_RAMPAGE:
					break;
				default:
					break;
			}

			// Check idle status
			status_idle = !boss->action_stand & !boss->action_walk & !boss->action_dead & !boss->action_shoot & !boss->action_punch & !boss->action_hit;

			//// ---------------- INITIALIZE ACTION ----------------

			if (status_idle) {
				boss->idle_counter = 0;
				boss->pattern_step++;
				if (boss->pattern_step > 5) boss->pattern_step = 0;

				// Set action
				switch (boss->status_behavior) {
					case BOSS_STATUS_CHASE:
						boss->current_pattern = boss->boss_chase_pattern[boss->pattern_step];
						break;
					case BOSS_STATUS_LONG_ATTACK:
						boss->current_pattern = boss->boss_attack_pattern[boss->pattern_step];
						break;
					case BOSS_STATUS_SHORT_ATTACK:
						boss->current_pattern = boss->boss_attack_pattern[boss->pattern_step];
						break;
					case BOSS_STATUS_RAMPAGE:
						boss->current_pattern = boss->boss_rampage_pattern[boss->pattern_step];
						break;
					case BOSS_STATUS_STATIC:
						boss->current_pattern = boss->boss_static_pattern[boss->pattern_step];
						break;
					default:
						boss->current_pattern = boss->boss_hold_on_pattern[boss->pattern_step];
						break;
				}

				switch (boss->current_pattern) {
					case BOSS_PATTERN_HOLD_ON:
						status_idle = false;
						boss->action_stand = true;
						boss->action_walk = false;
						boss->action_punch = false;
						boss->action_shoot = false;

						boss->movement = 0;
						boss->action_step = 0;
						boss->last_action = BOSS_ACTION_STAND;
						BOSS_SetStandAnimation();

						break;
					case BOSS_PATTERN_WALK_BACK:
						status_idle = false;
						boss->action_walk = true;
						boss->action_punch = false;
						boss->action_shoot = false;
						boss->action_stand = false;

						BOSS_MoveAway(actor->middle_x, actor->middle_y, &boss->movement);
						boss->action_step = 0;
						boss->last_action = BOSS_ACTION_WALK;
						BOSS_SetWalkAnimation();

						break;
					case BOSS_PATTERN_WALK_FORWARD:
						status_idle = false;
						boss->action_walk = true;
						boss->action_punch = false;
						boss->action_shoot = false;
						boss->action_stand = false;

						BOSS_MoveAhead(actor->middle_x, actor->middle_y, &boss->movement);
						boss->action_step = 0;
						boss->last_action = BOSS_ACTION_WALK;
						BOSS_SetWalkAnimation();

						break;
					case BOSS_PATTERN_WALK_ARROUND:
						status_idle = false;
						boss->action_walk = true;
						boss->action_punch = false;
						boss->action_shoot = false;
						boss->action_stand = false;

						BOSS_MoveAhead(actor->middle_x + rand() % 0xF, actor->middle_y + rand() % 0xF, &boss->movement);
						boss->action_step = 0;
						boss->last_action = BOSS_ACTION_WALK;
						BOSS_SetWalkAnimation();
						break;

					case BOSS_PATTERN_ATTACK:
						if (boss->in_punch_range) {
							boss->action_punch = true;
							boss->action_shoot = false;
							boss->action_walk = false;
							boss->action_stand = false;

							boss->movement = 0;
							boss->action_step = 0;
							boss->last_action = BOSS_ACTION_PUNCH;
						} else if (boss->in_shoot_range) {
							status_idle = false;
							boss->action_shoot = true;
							boss->action_punch = false;
							boss->action_walk = false;
							boss->action_stand = false;

							boss->movement = 0;
							boss->action_step = 0;
							boss->last_action = BOSS_ACTION_SHOOT;
						}
						break;

					default:
						break;
				}
			}

			//// ---------------- ACTION ANIMATION ----------------
			// Shooting and animation
			if (boss->action_shoot) {
				switch (boss->action_step) {
					case 0:// Calculate target

						boss->gun->current_recoil = 0;
						AUDIO_PlaySound(AUDIO_GUN_EFFECT, 1);
						BOSS_SetShotAnimation();
						boss->shoot_x = actor->pos_x + (actor->width_px >> 1) + 32;
						boss->shoot_y = actor->pos_y + (actor->height_px >> 1);
						BULLET_LoadBullet(boss->gun->bullet_graphics_id, ENTITY_ID_ENEMY_BULLET, 16, 16, boss->pos_x + (boss->width_px >> 1), boss->pos_y + (boss->height_px >> 1), boss->shoot_x, boss->shoot_y, boss->shoot_accuracy, boss->gun->max_distance, boss->gun->bullet_speed, boss->gun->damage);
						boss->shoot_x = actor->pos_x + (actor->width_px >> 1);
						boss->shoot_y = actor->pos_y + (actor->height_px >> 1);
						BULLET_LoadBullet(boss->gun->bullet_graphics_id, ENTITY_ID_ENEMY_BULLET, 16, 16, boss->pos_x + (boss->width_px >> 1), boss->pos_y + (boss->height_px >> 1), boss->shoot_x, boss->shoot_y, boss->shoot_accuracy, boss->gun->max_distance, boss->gun->bullet_speed, boss->gun->damage);
						boss->shoot_x = actor->pos_x + (actor->width_px >> 1) - 32;
						boss->shoot_y = actor->pos_y + (actor->height_px >> 1);
						BULLET_LoadBullet(boss->gun->bullet_graphics_id, ENTITY_ID_ENEMY_BULLET, 16, 16, boss->pos_x + (boss->width_px >> 1), boss->pos_y + (boss->height_px >> 1), boss->shoot_x, boss->shoot_y, boss->shoot_accuracy, boss->gun->max_distance, boss->gun->bullet_speed, boss->gun->damage);
						boss->action_step++;

						// Rampage add 2 more bullets
						if (boss->status_behavior == BOSS_STATUS_RAMPAGE) {
							boss->shoot_x = actor->pos_x + (actor->width_px >> 1) + 5;
							boss->shoot_y = actor->pos_y + (actor->height_px >> 1) + 5;
							BULLET_LoadBullet(boss->gun->bullet_graphics_id, ENTITY_ID_ENEMY_BULLET, 16, 16, boss->pos_x + (boss->width_px >> 1), boss->pos_y + (boss->height_px >> 1), boss->shoot_x, boss->shoot_y, boss->shoot_accuracy, boss->gun->max_distance, boss->gun->bullet_speed, boss->gun->damage);
							boss->shoot_x = actor->pos_x + (actor->width_px >> 1) - 5;
							boss->shoot_y = actor->pos_y + (actor->height_px >> 1) + 5;
							BULLET_LoadBullet(boss->gun->bullet_graphics_id, ENTITY_ID_ENEMY_BULLET, 16, 16, boss->pos_x + (boss->width_px >> 1), boss->pos_y + (boss->height_px >> 1), boss->shoot_x, boss->shoot_y, boss->shoot_accuracy, boss->gun->max_distance, boss->gun->bullet_speed, boss->gun->damage);
						}


						break;
					case 1:// animation end
						if (GFX_IsSpriteAnimationEnded(boss->sprite_num, 0)) boss->action_step++;
						break;
					case 2:// recoil time
						boss->gun->current_recoil++;
						if (boss->gun->current_recoil >= boss->gun->recoil_time) {
							boss->gun->current_recoil = 0;
							boss->action_step++;
						}
						break;
					case 3:// end
						boss->action_step = 0;
						boss->action_shoot = false;
						break;
					default:
						boss->action_step = 0;
						boss->action_shoot = false;
						break;
				}
			}

			// Puch and animation
			if (boss->action_punch) {
				switch (boss->action_step) {
					case 0:// set animation
						BOSS_SetPunchAnimation();
						BULLET_LoadBullet(SPRITE_GRAPHICS_ID_EMPTY, ENTITY_ID_ENEMY_BULLET, boss->width_px, boss->height_px + 16, boss->pos_x - 64, boss->pos_y, boss->pos_x + 64, boss->pos_y, 4, 128, 6, 30);
						boss->action_step = 0;
						boss->action_step++;
						break;
					case 1:// end
						if (GFX_IsSpriteAnimationEnded(boss->sprite_num, 0)) {
							boss->action_punch = false;
						}
						break;
					default:
						boss->action_step = 0;
						boss->action_punch = false;
						break;
				}
			}

			// Walking and animation
			if (boss->action_walk) {
				switch (boss->movement) {
					case 1:// moving right
						boss->x_FP += boss->speed_horizontal_FP;
						boss->pos_x = boss->x_FP >> FP;
						if (BOSS_CheckRightColission()) {
							boss->x_FP -= boss->speed_horizontal_FP;
							boss->pos_x = boss->x_FP >> FP;
							// Change direction >> up
							if (!BOSS_CheckUpColission()) {
								boss->movement = BOSS_MOVING_UP;
							}
							// Change direction >> down
							else if (!BOSS_CheckDownColission()) {
								boss->movement = BOSS_MOVING_DOWN;
							}
						}
						break;
					case 2:// moving left
						boss->x_FP -= boss->speed_horizontal_FP;
						boss->pos_x = boss->x_FP >> FP;
						if (BOSS_CheckLeftColission()) {
							boss->x_FP += boss->speed_horizontal_FP;
							boss->pos_x = boss->x_FP >> FP;
							// Change direction >> up
							if (!BOSS_CheckUpColission()) {
								boss->movement = BOSS_MOVING_UP;
							}
							// Change direction >> down
							else if (!BOSS_CheckDownColission()) {
								boss->movement = BOSS_MOVING_DOWN;
							}
						}
						break;
					case 4:// moving down
						boss->y_FP += boss->speed_vertical_FP;
						boss->pos_y = boss->y_FP >> FP;
						if (BOSS_CheckDownColission()) {
							boss->y_FP -= boss->speed_vertical_FP;
							boss->pos_y = boss->y_FP >> FP;
							// Change direction >> left
							if (!BOSS_CheckLeftColission()) {
								boss->movement = BOSS_MOVING_LEFT;
							}
							// Change direction >> up
							else if (!BOSS_CheckRightColission()) {
								boss->movement = BOSS_MOVING_RIGHT;
							}
						}
						break;
					case 5:// moving down-right
						boss->y_FP += boss->speed_diagonalv_FP;
						boss->pos_y = boss->y_FP >> FP;
						if (BOSS_CheckDownColission()) {
							boss->y_FP -= boss->speed_diagonalv_FP;
							boss->pos_y = boss->y_FP >> FP;
							// Change direction >> up
							if (!BOSS_CheckUpColission()) {
								boss->movement = BOSS_MOVING_UP;
							}
						}
						boss->x_FP += boss->speed_diagonalh_FP;
						boss->pos_x = boss->x_FP >> FP;
						if (BOSS_CheckRightColission()) {
							boss->x_FP -= boss->speed_diagonalh_FP;
							boss->pos_x = boss->x_FP >> FP;
							// Change direction >> left
							if (!BOSS_CheckLeftColission()) {
								boss->movement = BOSS_MOVING_LEFT;
							}
						}
						break;
					case 6:// moving down-left
						boss->y_FP += boss->speed_diagonalv_FP;
						boss->pos_y = boss->y_FP >> FP;
						if (BOSS_CheckDownColission()) {
							boss->y_FP -= boss->speed_diagonalv_FP;
							boss->pos_y = boss->y_FP >> FP;
							// Change direction >> up
							if (!BOSS_CheckUpColission()) {
								boss->movement = BOSS_MOVING_UP;
							}
						}
						boss->x_FP -= boss->speed_diagonalh_FP;
						boss->pos_x = boss->x_FP >> FP;
						if (BOSS_CheckLeftColission()) {
							boss->x_FP += boss->speed_diagonalh_FP;
							boss->pos_x = boss->x_FP >> FP;
							// Change direction >> right
							if (!BOSS_CheckRightColission()) {
								boss->movement = BOSS_MOVING_RIGHT;
							}
						}
						break;
					case 8:// moving up
						boss->y_FP -= boss->speed_vertical_FP;
						boss->pos_y = boss->y_FP >> FP;
						if (BOSS_CheckUpColission()) {
							boss->y_FP += boss->speed_vertical_FP;
							boss->pos_y = boss->y_FP >> FP;
							// Change direction >> left
							if (!BOSS_CheckLeftColission()) {
								boss->movement = BOSS_MOVING_LEFT;
							}
							// Change direction >> right
							else if (!BOSS_CheckRightColission()) {
								boss->movement = BOSS_MOVING_RIGHT;
							}
						}
						break;
					case 9:// moving up-right
						boss->y_FP -= boss->speed_diagonalv_FP;
						boss->pos_y = boss->y_FP >> FP;
						if (BOSS_CheckUpColission()) {
							boss->y_FP += boss->speed_diagonalv_FP;
							boss->pos_y = boss->y_FP >> FP;
							// Change direction >> down
							if (!BOSS_CheckDownColission()) {
								boss->movement = BOSS_MOVING_DOWN;
							}
						}
						boss->x_FP += boss->speed_diagonalh_FP;
						boss->pos_x = boss->x_FP >> FP;
						if (BOSS_CheckRightColission()) {
							boss->x_FP -= boss->speed_diagonalh_FP;
							boss->pos_x = boss->x_FP >> FP;
							// Change direction >> left
							if (!BOSS_CheckLeftColission()) {
								boss->movement = BOSS_MOVING_LEFT;
							}
						}
						break;
					case 10:// moving up-left
						boss->y_FP -= boss->speed_diagonalv_FP;
						boss->pos_y = boss->y_FP >> FP;
						if (BOSS_CheckUpColission()) {
							boss->y_FP += boss->speed_diagonalv_FP;
							boss->pos_y = boss->y_FP >> FP;
							// Change direction >> down
							if (!BOSS_CheckDownColission()) {
								boss->movement = BOSS_MOVING_DOWN;
							}
						}
						boss->x_FP -= boss->speed_diagonalh_FP;
						boss->pos_x = boss->x_FP >> FP;
						if (BOSS_CheckLeftColission()) {
							boss->x_FP += boss->speed_diagonalh_FP;
							boss->pos_x = boss->x_FP >> FP;
							// Change direction >> right
							if (!BOSS_CheckRightColission()) {
								boss->movement = BOSS_MOVING_RIGHT;
							}
						}
						break;
					default:
						break;
				}

				boss->idle_counter++;
				if (boss->idle_counter > BOSS_IDLE_TICKS) {
					boss->action_walk = false;
					boss->idle_counter = 0;
				}
			}

			// Stand and animation
			if (boss->action_stand) {
				boss->idle_counter++;
				if (boss->idle_counter > BOSS_IDLE_TICKS) {
					boss->action_stand = false;
					boss->idle_counter = 0;
				}
			}

			// Checks if is hit but still alive
			// - sets action hit or dead
			if (boss->is_hit && !boss->action_dead) {
				boss->is_hit = false;
				status_idle = false;
				// Cancel any action
				boss->action_punch = false;
				boss->action_shoot = false;
				boss->action_walk = false;
				boss->action_stand = false;
				boss->action_dead = false;

				if (boss->damage >= boss->life) {
					boss->life = 0;
					boss->damage = 0;
					boss->action_step = 0;
					boss->action_dead = true;
					boss->action_hit = false;

				} else {
					boss->life -= boss->damage;
					boss->damage = 0;
					if (!boss->action_hit) {
						boss->action_step = 0;
						boss->action_hit = true;
					}
				}
				GFX_SetPanelGraphics(gfx_enemy_status_panel, boss->gfx_portait, SPRITE_GRAPHICS_ID_LIFEBAR, SPRITE_GRAPHICS_ID_GUN0, SPRITE_GRAPHICS_ID_EMPTY);
				GFX_UpdatePanel(gfx_enemy_status_panel, boss->life, boss->life + boss->damage, boss->max_life, 1);
				GFX_ShowPanel(gfx_enemy_status_panel, true, 200);
			}

			// Check if is hit
			if (boss->action_hit) {
				switch (boss->action_step) {
					case 0:
						GFX_SetSpriteBlinkingProperties(boss->sprite_num, true, 1, SPRITE_HIT_COLOR);
						BOSS_SetHitAnimation();
						boss->action_step++;
						break;
					case 1:
						switch (boss->hit_by) {
							case ENTITY_ID_ACTOR_BULLET:
								break;
							case ENTITY_ID_ACTOR_PUNCH:
								break;
							case ENTITY_ID_ACTOR_KICK:
								boss->x_FP += boss->hit_vx_FP << 2;
								boss->y_FP += boss->hit_vy_FP << 2;
								boss->pos_x = boss->x_FP >> FP;
								boss->pos_y = boss->y_FP >> FP;
								break;
							default:
								break;
						}
						boss->action_step++;
						break;
					case 2:
						EFFECT_LoadEffect(ENTITY_ID_BLOOD, SPRITE_GRAPHICS_ID_BLOOD, boss->pos_x + boss->width_px + (rand() % 32), boss->pos_y + boss->height_px + (rand() % 32), true, 0, false, false, 5);
						EFFECT_LoadEffect(ENTITY_ID_BLOOD, SPRITE_GRAPHICS_ID_BLOOD, boss->pos_x - (rand() % 32), boss->pos_y + boss->height_px + (rand() % 32), true, 0, false, false, 5);
						boss->action_step++;
						break;
					case 3:// Move back
						boss->x_FP += boss->hit_vx_FP << 1;
						boss->y_FP += boss->hit_vy_FP << 1;
						boss->pos_x = boss->x_FP >> FP;
						boss->pos_y = boss->y_FP >> FP;
						boss->reaction_counter = 0;
						boss->action_step++;
						break;
					case 4:// finish action
						if (GFX_IsSpriteAnimationEnded(boss->sprite_num, 0)) boss->action_hit = false;
						boss->reaction_counter++;
						if (boss->reaction_counter > boss->reaction_time) {
							boss->action_hit = false;
							boss->reaction_counter = 0;
						}

						break;
					default:
						boss->action_hit = false;
						break;
				}
			}

			// Check if is dead
			if (boss->action_dead) {

				switch (boss->action_step) {
					case 0:// Set death animation
						BOSS_SetDeadAnimation();
						GFX_SetPanelPortait(gfx_enemy_status_panel, 2);
						boss->life = 0;
						boss->movement = 0;
						boss->action_step++;
						break;
					case 1:
						EFFECT_LoadEffect(ENTITY_ID_BLOOD, SPRITE_GRAPHICS_ID_BLOOD, boss->pos_x + (rand() % 32), boss->pos_y + (rand() % 32), true, 0, false, false, 5);
						if (GFX_IsSpriteAnimationEnded(boss->sprite_num, 0)) {
							boss->action_step++;
							boss->idle_counter = 0;
						}
						break;
					case 2:
						boss->idle_counter++;
						if (boss->idle_counter > 20) {
							boss->action_step++;
							boss->idle_counter = 0;
							GFX_SetSpriteBlinkingProperties(boss->sprite_num, true, 5, SPRITE_TRANSP_COLOR);
						}
						break;
					case 3:// Unload enemy
						boss->idle_counter++;
						if (boss->idle_counter > 20) {
							boss->action_step++;
							boss->idle_counter = 0;
						}
						break;
					case 4:
						BOSS_Unload();

						break;
					default:
						break;
				}
			}

			// Blink timeout
			if (GFX_IsBlinkActive(boss->sprite_num)) {
				boss->blink_counter++;
				if (boss->blink_counter > 5) {
					boss->blink_counter = 0;
					GFX_SetSpriteBlinkingProperties(boss->sprite_num, false, 0, SPRITE_HIT_COLOR);
				}
			}
		}
	}
}
