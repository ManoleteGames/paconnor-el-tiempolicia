#include "../engine.h"
#include "stdbool.h"
#include <iso646.h>
#include <stdlib.h>
#include "enemy.h"

//Enemy enemy[ENEMY_MAX_ENEMIES];
Enemy *enemy;

static byte enemy_update_counter;
byte enemy_counter;

static int facing_array[8] = {ENEMY_FACING_RIGHT, ENEMY_FACING_LEFT, ENEMY_FACING_DOWN, ENEMY_FACING_DOWN_RIGHT, ENEMY_FACING_DOWN_LEFT, ENEMY_FACING_UP, ENEMY_FACING_UP_RIGHT, ENEMY_FACING_UP_LEFT};
static int hold_on_pattern[6] = {ENEMY_PATTERN_HOLD_ON, ENEMY_PATTERN_HOLD_ON, ENEMY_PATTERN_HOLD_ON, ENEMY_PATTERN_HOLD_ON, ENEMY_PATTERN_HOLD_ON, ENEMY_PATTERN_HOLD_ON};
static int walk_random_pattern[6] = {ENEMY_PATTERN_HOLD_ON, ENEMY_PATTERN_NPC_WALK_RANDOM, ENEMY_PATTERN_NPC_WALK_RANDOM, ENEMY_PATTERN_NPC_WALK_RANDOM, ENEMY_PATTERN_NPC_WALK_RANDOM, ENEMY_PATTERN_NPC_WALK_RANDOM};
static int walk_arround_pattern[6] = {ENEMY_PATTERN_HOLD_ON, ENEMY_PATTERN_NPC_WALK_UP, ENEMY_PATTERN_NPC_WALK_RIGHT, ENEMY_PATTERN_NPC_WALK_DOWN, ENEMY_PATTERN_NPC_WALK_LEFT, ENEMY_PATTERN_HOLD_ON};
static int walk_horizontal_pattern[6] = {ENEMY_PATTERN_HOLD_ON, ENEMY_PATTERN_NPC_WALK_RIGHT, ENEMY_PATTERN_NPC_WALK_LEFT, ENEMY_PATTERN_NPC_WALK_LEFT, ENEMY_PATTERN_NPC_WALK_RIGHT, ENEMY_PATTERN_HOLD_ON};
static int walk_vertical_pattern[6] = {ENEMY_PATTERN_HOLD_ON, ENEMY_PATTERN_NPC_WALK_UP, ENEMY_PATTERN_NPC_WALK_DOWN, ENEMY_PATTERN_NPC_WALK_DOWN, ENEMY_PATTERN_NPC_WALK_UP, ENEMY_PATTERN_HOLD_ON};
static int chase_pattern[6] = {ENEMY_PATTERN_WALK_FORWARD, ENEMY_PATTERN_WALK_FORWARD, ENEMY_PATTERN_WALK_FORWARD, ENEMY_PATTERN_WALK_ARROUND, ENEMY_PATTERN_WALK_FORWARD, ENEMY_PATTERN_WALK_FORWARD};
static int rampage_chase_pattern[6] = {ENEMY_PATTERN_WALK_FORWARD, ENEMY_PATTERN_WALK_FORWARD, ENEMY_PATTERN_WALK_FORWARD, ENEMY_PATTERN_WALK_FORWARD, ENEMY_PATTERN_WALK_FORWARD, ENEMY_PATTERN_WALK_FORWARD};
static int avoid_pattern[6] = {ENEMY_PATTERN_WALK_BACK, ENEMY_PATTERN_WALK_ARROUND, ENEMY_PATTERN_WALK_FORWARD, ENEMY_PATTERN_WALK_ARROUND, ENEMY_PATTERN_WALK_FORWARD, ENEMY_PATTERN_ATTACK};
static int hide_pattern[6] = {ENEMY_PATTERN_WALK_BACK, ENEMY_PATTERN_WALK_BACK, ENEMY_PATTERN_WALK_ARROUND, ENEMY_PATTERN_WALK_BACK, ENEMY_PATTERN_WALK_BACK, ENEMY_PATTERN_WALK_BACK};
static int attack_pattern[6] = {ENEMY_PATTERN_ATTACK, ENEMY_PATTERN_ATTACK, ENEMY_PATTERN_WALK_ARROUND, ENEMY_PATTERN_ATTACK, ENEMY_PATTERN_ATTACK, ENEMY_PATTERN_WALK_ARROUND};
static int shooter_pattern[6] = {ENEMY_PATTERN_ATTACK, ENEMY_PATTERN_ATTACK, ENEMY_PATTERN_HOLD_ON, ENEMY_PATTERN_ATTACK, ENEMY_PATTERN_ATTACK, ENEMY_PATTERN_HOLD_ON};
static int rampage_pattern[6] = {ENEMY_PATTERN_ATTACK, ENEMY_PATTERN_ATTACK, ENEMY_PATTERN_ATTACK, ENEMY_PATTERN_ATTACK, ENEMY_PATTERN_ATTACK, ENEMY_PATTERN_ATTACK};

void ENEMY_SetGun(int enemy_number, int type, int graphics_id, int bullet_graphics_id) {
	// Set graphics id
	enemy[enemy_number].gun->graphics_id = graphics_id;
	enemy[enemy_number].gun->bullet_graphics_id = bullet_graphics_id;

	// Set gun type
	enemy[enemy_number].gun->type = type;
	switch (type) {
		case ENEMY_GUN_BARE_HANDS:
			enemy[enemy_number].gun->max_accuracy = 4;
			enemy[enemy_number].gun->damage = 10;
			enemy[enemy_number].gun->recoil_time = 10;
			enemy[enemy_number].gun->max_distance = 6;
			enemy[enemy_number].gun->accurate = false;
			enemy[enemy_number].gun->shoots = 1;
			enemy[enemy_number].gun->bullet_speed = 1;
			enemy[enemy_number].shoot_accuracy = 4;
			enemy[enemy_number].shoot_range = 32;
			break;
		case ENEMY_GUN_PISTOL:
			enemy[enemy_number].gun->max_accuracy = 4;
			enemy[enemy_number].gun->damage = 20;
			enemy[enemy_number].gun->recoil_time = 5;
			enemy[enemy_number].gun->max_distance = 130;
			enemy[enemy_number].gun->accurate = true;
			enemy[enemy_number].gun->shoots = 1;
			enemy[enemy_number].gun->bullet_speed = 4;
			enemy[enemy_number].shoot_accuracy = 4;
			enemy[enemy_number].shoot_range = 120;
			break;
		case ENEMY_GUN_AK:
			enemy[enemy_number].gun->max_accuracy = 1;
			enemy[enemy_number].gun->damage = 20;
			enemy[enemy_number].gun->recoil_time = 1;
			enemy[enemy_number].gun->max_distance = 150;
			enemy[enemy_number].gun->accurate = false;
			enemy[enemy_number].gun->shoots = 1;
			enemy[enemy_number].gun->bullet_speed = 5;
			enemy[enemy_number].shoot_accuracy = 1;
			enemy[enemy_number].shoot_range = 130;
			break;
		case ENEMY_GUN_SHOTGUN:
			enemy[enemy_number].gun->max_accuracy = 1;
			enemy[enemy_number].gun->damage = 40;
			enemy[enemy_number].gun->recoil_time = 10;
			enemy[enemy_number].gun->max_distance = 100;
			enemy[enemy_number].gun->accurate = false;
			enemy[enemy_number].gun->shoots = 4;
			enemy[enemy_number].gun->bullet_speed = 2;
			enemy[enemy_number].shoot_accuracy = 1;
			enemy[enemy_number].shoot_range = 80;
			break;
		case ENEMY_GUN_RIFLE:
			enemy[enemy_number].gun->max_accuracy = 2;
			enemy[enemy_number].gun->damage = 30;
			enemy[enemy_number].gun->recoil_time = 7;
			enemy[enemy_number].gun->max_distance = 200;
			enemy[enemy_number].gun->accurate = true;
			enemy[enemy_number].gun->shoots = 1;
			enemy[enemy_number].gun->bullet_speed = 5;
			enemy[enemy_number].shoot_accuracy = 2;
			enemy[enemy_number].shoot_range = 180;
			break;
		default:
			sprintf(engine.system_error_message1, "enemy_SetGun function error");
			sprintf(engine.system_error_message2, "Undefined gun type %u ", type);
			sprintf(engine.system_error_message3, " ");
			Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_SYSTEM);
			break;
	}
}

void ENEMY_LoadAnimations(const char *dat_name, int enemy_number, int total_anims) {
	GFX_LoadAnimations(dat_name, "FEET.ANI", &enemy[enemy_number].feet_animation[0], total_anims);     // feet animations
	GFX_LoadAnimations(dat_name, "BODY.ANI", &enemy[enemy_number].body_animation[0], total_anims);     // body animations
	GFX_LoadAnimations(dat_name, "HEAD.ANI", &enemy[enemy_number].head_animation[0], total_anims);     // head animations
	GFX_LoadAnimations(dat_name, "LARM.ANI", &enemy[enemy_number].left_arm_animation[0], total_anims); // left arm animations
	GFX_LoadAnimations(dat_name, "RARM.ANI", &enemy[enemy_number].right_arm_animation[0], total_anims);// right arm animations
}

void ENEMY_ResetAnimations(int enemy_number) {
	int i, j;
	for (i = 0; i < 64; i++) {
		enemy[enemy_number].feet_animation[i].speed = 20;
		enemy[enemy_number].feet_animation[i].inverted = false;
		enemy[enemy_number].feet_animation[i].end = false;
		enemy[enemy_number].feet_animation[i].current_frame = 0;
		enemy[enemy_number].feet_animation[i].frames = 1;
		for (j = 0; j < 20; j++) {
			enemy[enemy_number].feet_animation[i].anim_data[j].frame = 0;
			enemy[enemy_number].feet_animation[i].anim_data[j].offset_x = 0;
			enemy[enemy_number].feet_animation[i].anim_data[j].offset_y = 0;
		}
	}
}

void ENEMY_Init(void) {
	int i;
	enemy = MM_PushChunk(sizeof(Enemy) * ENEMY_MAX_ENEMIES, CT_ENGINE);
	for (i = 0; i < ENEMY_MAX_ENEMIES; i++) {
		enemy[i].is_loaded = false;
		enemy[i].sprite_num = -1;
		enemy[i].gun = MM_PushChunk(sizeof(Gun), CT_ENGINE);
		enemy[i].cfg = MM_PushChunk(sizeof(SpriteConfig), CT_ENGINE);
		enemy[i].feet_animation = MM_PushChunk(sizeof(SpriteAnimation) * 69, CT_ENGINE);
		enemy[i].body_animation = MM_PushChunk(sizeof(SpriteAnimation) * 69, CT_ENGINE);
		enemy[i].head_animation = MM_PushChunk(sizeof(SpriteAnimation) * 69, CT_ENGINE);
		enemy[i].left_arm_animation = MM_PushChunk(sizeof(SpriteAnimation) * 69, CT_ENGINE);
		enemy[i].right_arm_animation = MM_PushChunk(sizeof(SpriteAnimation) * 69, CT_ENGINE);

		//ENEMY_ResetAnimations(i);
	}
}

/** ENEMY :: Initializes enemy data
 * - Sets default values for the enemy structure 
 */
void ENEMY_Load(const char *dat_name, byte number, int x, int y, int face_gfx_id, int portait_gfx_id, int feet_gfx_id, int body_gfx_id, int head_gfx_id, int larm_gfx_id, int rarm_gfx_id, int facing, int gun_type, int gun_graphics_id, int bullet_graphics_id, int behavior, int life) {

	int sprite_slot;

	// Check if enemy is already loaded
	if (enemy[number].is_loaded) {
		sprintf(engine.system_error_message1, "ENEMY_Init function error");
		sprintf(engine.system_error_message2, "Enemy %u already loaded", number);
		sprintf(engine.system_error_message3, " ");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_SYSTEM);
	}

	// Load configuration
	FILE_LoadSpriteConfigFile(dat_name, "ENEMY.CFG", enemy[number].cfg);

	// Load animations
	ENEMY_LoadAnimations(dat_name, number, 64);

	enemy[number].width_px = enemy[number].cfg->width;
	enemy[number].height_px = enemy[number].cfg->height;

	// Load graphics
	if (!gfx_sprite_graphics_stack[face_gfx_id].loaded) GFX_LoadSpriteGraphicsRLE(dat_name, "FACE.PCX", face_gfx_id, enemy[number].cfg->face_width, enemy[number].cfg->face_height, enemy[number].cfg->face_frames, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_TEMPORARY_SPRITE);
	if (!gfx_sprite_graphics_stack[portait_gfx_id].loaded) GFX_LoadSpriteGraphicsRLE(dat_name, "PORTAIT.PCX", portait_gfx_id, enemy[number].cfg->portait_width, enemy[number].cfg->portait_height, enemy[number].cfg->portait_frames, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_TEMPORARY_SPRITE);
	if (!gfx_sprite_graphics_stack[feet_gfx_id].loaded) GFX_LoadSpriteGraphicsRLE(dat_name, "FEET.PCX", feet_gfx_id, enemy[number].cfg->feet_width, enemy[number].cfg->feet_height, enemy[number].cfg->feet_frames, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_TEMPORARY_SPRITE);
	if (!gfx_sprite_graphics_stack[body_gfx_id].loaded) GFX_LoadSpriteGraphicsRLE(dat_name, "BODY.PCX", body_gfx_id, enemy[number].cfg->body_width, enemy[number].cfg->body_height, enemy[number].cfg->body_frames, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_TEMPORARY_SPRITE);
	if (!gfx_sprite_graphics_stack[head_gfx_id].loaded) GFX_LoadSpriteGraphicsRLE(dat_name, "HEAD.PCX", head_gfx_id, enemy[number].cfg->head_width, enemy[number].cfg->head_height, enemy[number].cfg->head_frames, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_TEMPORARY_SPRITE);
	if (!gfx_sprite_graphics_stack[larm_gfx_id].loaded) GFX_LoadSpriteGraphicsRLE(dat_name, "LARM.PCX", larm_gfx_id, enemy[number].cfg->larm_width, enemy[number].cfg->larm_height, enemy[number].cfg->larm_frames, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_TEMPORARY_SPRITE);
	if (!gfx_sprite_graphics_stack[rarm_gfx_id].loaded) GFX_LoadSpriteGraphicsRLE(dat_name, "RARM.PCX", rarm_gfx_id, enemy[number].cfg->rarm_width, enemy[number].cfg->rarm_height, enemy[number].cfg->rarm_frames, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_TEMPORARY_SPRITE);

	enemy[number].is_loaded = true;

	enemy[number].status_behavior = behavior;

	enemy[number].pos_x = x;
	enemy[number].pos_y = y;
	enemy[number].x_FP = x << FP;
	enemy[number].y_FP = y << FP;
	enemy[number].vx_FP = 0;
	enemy[number].vy_FP = 0;

	enemy[number].status_facing = facing;

	enemy[number].speed_horizontal_FP = ENEMY_SPEED << FP;
	enemy[number].speed_vertical_FP = (ENEMY_SPEED << FP) - ((ENEMY_SPEED << FP) >> 3);
	enemy[number].speed_diagonalv_FP = (enemy[number].speed_vertical_FP * 10) / 14;
	enemy[number].speed_diagonalh_FP = (enemy[number].speed_horizontal_FP * 10) / 14;

	enemy[number].speed = ENEMY_SPEED;
	enemy[number].max_life = life;
	enemy[number].life = life;
	enemy[number].damage = 0;
	enemy[number].is_hit = false;

	enemy[number].movement_allowed = true;
	enemy[number].reaction_time = 70;

	enemy[number].action_shoot = false;
	enemy[number].action_punch = false;
	enemy[number].action_stand = false;
	enemy[number].action_walk = false;
	enemy[number].action_hit = false;
	enemy[number].action_dead = false;

	enemy[number].gfx_portait = portait_gfx_id;

	sprite_slot = GFX_FindEmptySpriteSlot();
	if (sprite_slot == -1) {
		sprintf(engine.system_error_message1, "ENEMY_Load function error");
		sprintf(engine.system_error_message2, "No empty sprite slot available");
		sprintf(engine.system_error_message3, " ");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
	} else {
		enemy[number].sprite_num = sprite_slot;
		GFX_InitSprite(ENTITY_ID_ENEMY, number, sprite_slot, 0, enemy[number].width_px, enemy[number].height_px);
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
	enemy[number].colission_area.points[0][0] = gfx_sprite_stack[enemy[number].sprite_num].width_px >> 3;
	enemy[number].colission_area.points[0][1] = (gfx_sprite_stack[enemy[number].sprite_num].height_px >> 1) + (gfx_sprite_stack[enemy[number].sprite_num].height_px >> 3);
	// Point 2
	enemy[number].colission_area.points[1][0] = gfx_sprite_stack[enemy[number].sprite_num].width_px - (gfx_sprite_stack[enemy[number].sprite_num].width_px >> 2);
	enemy[number].colission_area.points[1][1] = (gfx_sprite_stack[enemy[number].sprite_num].height_px >> 1) + (gfx_sprite_stack[enemy[number].sprite_num].height_px >> 3);

	// Point 3
	enemy[number].colission_area.points[2][0] = gfx_sprite_stack[enemy[number].sprite_num].width_px >> 3;
	enemy[number].colission_area.points[2][1] = gfx_sprite_stack[enemy[number].sprite_num].height_px;
	// Point 4
	enemy[number].colission_area.points[3][0] = gfx_sprite_stack[enemy[number].sprite_num].width_px - (gfx_sprite_stack[enemy[number].sprite_num].width_px >> 2);
	enemy[number].colission_area.points[3][1] = gfx_sprite_stack[enemy[number].sprite_num].height_px;

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
	enemy[number].hit_area.points[0][0] = gfx_sprite_stack[enemy[number].sprite_num].width_px >> 3;
	enemy[number].hit_area.points[0][1] = (gfx_sprite_stack[enemy[number].sprite_num].height_px >> 1) - (gfx_sprite_stack[enemy[number].sprite_num].height_px >> 3);
	// Point 2
	enemy[number].hit_area.points[1][0] = gfx_sprite_stack[enemy[number].sprite_num].width_px - (gfx_sprite_stack[enemy[number].sprite_num].width_px >> 2);
	enemy[number].hit_area.points[1][1] = (gfx_sprite_stack[enemy[number].sprite_num].height_px >> 1) - (gfx_sprite_stack[enemy[number].sprite_num].height_px >> 3);
	// Point 3
	enemy[number].hit_area.points[2][0] = gfx_sprite_stack[enemy[number].sprite_num].width_px >> 3;
	enemy[number].hit_area.points[2][1] = gfx_sprite_stack[enemy[number].sprite_num].height_px;
	// Point 4
	enemy[number].hit_area.points[3][0] = gfx_sprite_stack[enemy[number].sprite_num].width_px - (gfx_sprite_stack[enemy[number].sprite_num].width_px >> 2);
	enemy[number].hit_area.points[3][1] = gfx_sprite_stack[enemy[number].sprite_num].height_px;

	GFX_SetSpriteGraphic(enemy[number].sprite_num, 0, feet_gfx_id, 0, 0);// feet
	GFX_SetSpriteGraphic(enemy[number].sprite_num, 1, body_gfx_id, 0, 0);// body
	GFX_SetSpriteGraphic(enemy[number].sprite_num, 2, head_gfx_id, 0, 0);// head
	GFX_SetSpriteGraphic(enemy[number].sprite_num, 3, larm_gfx_id, 0, 0);// left arm
	GFX_SetSpriteGraphic(enemy[number].sprite_num, 4, rarm_gfx_id, 0, 0);// right arm

	gfx_sprite_stack[enemy[number].sprite_num].screen_pos_x = enemy[number].pos_x - camera->pos_x;
	gfx_sprite_stack[enemy[number].sprite_num].screen_pos_y = enemy[number].pos_y - camera->pos_y;

	// Initialize gun
	ENEMY_SetGun(number, gun_type, gun_graphics_id, bullet_graphics_id);

	// Patterns
	enemy[number].pattern_step = 0;
	enemy[number].current_pattern = ENEMY_PATTERN_HOLD_ON;
}

void ENEMY_SetPosition(byte number, int x, int y) {
	if (enemy[number].is_loaded) {
		enemy[number].pos_x = x;
		enemy[number].pos_y = y;
		enemy[number].x_FP = x << FP;
		enemy[number].y_FP = y << FP;
		enemy[number].vx_FP = 0;
		enemy[number].vy_FP = 0;
	}
}

void ENEMY_SetBehavior(byte number, int behavior) {
	if (enemy[number].is_loaded) {
		enemy[number].status_behavior = behavior;
	}
}

void ENEMY_SetFacing(byte number, int facing) {
	enemy[number].status_facing = facing;
}

void ENEMY_DrawColissionPixels(Enemy e) {
	//  Draw colission pixels
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[e.sprite_num].screen_pos_x + e.colission_area.points[0][0], gfx_sprite_stack[e.sprite_num].screen_pos_y + e.colission_area.points[0][1], 228);
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[e.sprite_num].screen_pos_x + e.colission_area.points[1][0], gfx_sprite_stack[e.sprite_num].screen_pos_y + e.colission_area.points[1][1], 228);
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[e.sprite_num].screen_pos_x + e.colission_area.points[2][0], gfx_sprite_stack[e.sprite_num].screen_pos_y + e.colission_area.points[2][1], 228);
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[e.sprite_num].screen_pos_x + e.colission_area.points[3][0], gfx_sprite_stack[e.sprite_num].screen_pos_y + e.colission_area.points[3][1], 228);
}

void ENEMY_DrawHitPixels(Enemy e) {
	//  Draw colission pixels
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[e.sprite_num].screen_pos_x + e.hit_area.points[0][0], gfx_sprite_stack[e.sprite_num].screen_pos_y + e.hit_area.points[0][1], 229);
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[e.sprite_num].screen_pos_x + e.hit_area.points[1][0], gfx_sprite_stack[e.sprite_num].screen_pos_y + e.hit_area.points[1][1], 229);
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[e.sprite_num].screen_pos_x + e.hit_area.points[2][0], gfx_sprite_stack[e.sprite_num].screen_pos_y + e.hit_area.points[2][1], 229);
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[e.sprite_num].screen_pos_x + e.hit_area.points[3][0], gfx_sprite_stack[e.sprite_num].screen_pos_y + e.hit_area.points[3][1], 229);
}

bool ENEMY_CheckUpColission(Enemy e) {
	int i;
	int point1_x, point1_y;
	int point2_x, point2_y;

	point1_x = e.pos_x + e.colission_area.points[0][0];
	point1_y = e.pos_y + e.colission_area.points[0][1];
	point2_x = e.pos_x + e.colission_area.points[1][0];
	point2_y = e.pos_y + e.colission_area.points[1][1];

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
			if (gfx_sprite_stack[enemy[i].sprite_num].shown && (enemy[i].sprite_num != e.sprite_num)) {
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

bool ENEMY_CheckDownColission(Enemy e) {
	int i;

	int point1_x, point1_y;
	int point2_x, point2_y;

	point1_x = e.pos_x + e.colission_area.points[2][0];
	point1_y = e.pos_y + e.colission_area.points[2][1];
	point2_x = e.pos_x + e.colission_area.points[3][0];
	point2_y = e.pos_y + e.colission_area.points[3][1];

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
			if (gfx_sprite_stack[enemy[i].sprite_num].shown && (enemy[i].sprite_num != e.sprite_num)) {
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

bool ENEMY_CheckLeftColission(Enemy e) {
	int i;

	int point1_x, point1_y;
	int point2_x, point2_y;

	point1_x = e.pos_x + e.colission_area.points[0][0];
	point1_y = e.pos_y + e.colission_area.points[0][1];
	point2_x = e.pos_x + e.colission_area.points[2][0];
	point2_y = e.pos_y + e.colission_area.points[2][1];

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
			if (gfx_sprite_stack[enemy[i].sprite_num].shown && (enemy[i].sprite_num != e.sprite_num)) {
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

bool ENEMY_CheckRightColission(Enemy e) {
	int i;

	int point1_x, point1_y;
	int point2_x, point2_y;

	point1_x = e.pos_x + e.colission_area.points[1][0];
	point1_y = e.pos_y + e.colission_area.points[1][1];
	point2_x = e.pos_x + e.colission_area.points[3][0];
	point2_y = e.pos_y + e.colission_area.points[3][1];

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
			if (gfx_sprite_stack[enemy[i].sprite_num].shown && (enemy[i].sprite_num != e.sprite_num)) {
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

bool ENEMY_AreEnemiesNearby(void) {
	int i;
	for (i = 0; i < ENEMY_MAX_ENEMIES; i++) {
		if (enemy[i].is_loaded) {
			return true;
		}
	}
	return false;
}

void ENEMY_UpdateRangeStatus(int enemy_number) {
	int i, dx, dy, distance, pos_x, pos_y;
	dword vx_FP, vy_FP, x_FP, y_FP;
	if (gfx_sprite_stack[enemy[enemy_number].sprite_num].shown) {
		dx = actor->middle_x - enemy[enemy_number].middle_x;
		dy = actor->middle_y - enemy[enemy_number].middle_y;

		// calculate steps
		if (abs(dx) > abs(dy))
			distance = abs(dx);
		else
			distance = abs(dy);

		// Avoid 0 division or die!!
		if (distance == 0) distance = 1;

		// Check if is in shoot range
		if (distance < enemy[enemy_number].shoot_range) enemy[enemy_number].in_shoot_range = true;
		else
			enemy[enemy_number].in_shoot_range = false;

		// calculate fixed point velocity and position
		vx_FP = (dx << FP) / distance;
		vy_FP = (dy << FP) / distance;

		x_FP = (enemy[enemy_number].pos_x + enemy[enemy_number].width_px) << FP;
		y_FP = (enemy[enemy_number].pos_y + enemy[enemy_number].height_px) << FP;

		// Check if can see actor
		enemy[enemy_number].can_see_actor = true;
		for (i = 0; i < distance; i++) {
			x_FP += vx_FP;
			y_FP += vy_FP;

			pos_x = (x_FP >> FP);
			pos_y = (y_FP >> FP);

			if (MAP_CheckColissionTile(pos_x, pos_y) == 1) enemy[enemy_number].can_see_actor = false;
		}
	} else {
		enemy[enemy_number].can_see_actor = false;
		enemy[enemy_number].in_shoot_range = false;
	}
}

void ENEMY_SetPunchAnimation(int enemy_number, int facing) {
	switch (facing) {
		case 1:// facing right
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_PUNCH_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_PUNCH_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_PUNCH_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_PUNCH_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_PUNCH_RIGHT], true);
			break;
		case 2:// facing left
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_PUNCH_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_PUNCH_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_PUNCH_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_PUNCH_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_PUNCH_LEFT], true);
			break;
		case 4:// facing down
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_PUNCH_DOWN], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_PUNCH_DOWN], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_PUNCH_DOWN], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_PUNCH_DOWN], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_PUNCH_DOWN], true);
			break;
		case 5:// moving down-right
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_PUNCH_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_PUNCH_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_PUNCH_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_PUNCH_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_PUNCH_DOWN_RIGHT], true);
			break;
		case 6:// moving down-left
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_PUNCH_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_PUNCH_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_PUNCH_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_PUNCH_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_PUNCH_DOWN_LEFT], true);
			break;
		case 8:// moving up
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_PUNCH_UP], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_PUNCH_UP], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_PUNCH_UP], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_PUNCH_UP], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_PUNCH_UP], true);
			break;
		case 9:// moving up-right
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_PUNCH_UP_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_PUNCH_UP_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_PUNCH_UP_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_PUNCH_UP_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_PUNCH_UP_RIGHT], true);
			break;
		case 10:// moving up-left
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_PUNCH_UP_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_PUNCH_UP_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_PUNCH_UP_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_PUNCH_UP_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_PUNCH_UP_LEFT], true);
			break;
		default:
			break;
	}
}

void ENEMY_SetWalkAnimation(int enemy_number, int facing) {
	switch (facing) {
		case 1:// facing right
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_WALK_RIGHT], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_WALK_RIGHT], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_WALK_RIGHT], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_WALK_RIGHT], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_WALK_RIGHT], false);
			break;
		case 2:// facing left
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_WALK_LEFT], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_WALK_LEFT], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_WALK_LEFT], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_WALK_LEFT], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_WALK_LEFT], false);
			break;
		case 4:// facing down
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_WALK_DOWN], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_WALK_DOWN], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_WALK_DOWN], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_WALK_DOWN], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_WALK_DOWN], false);
			break;
		case 5:// facing down-right
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_WALK_DOWN_RIGHT], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_WALK_DOWN_RIGHT], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_WALK_DOWN_RIGHT], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_WALK_DOWN_RIGHT], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_WALK_DOWN_RIGHT], false);
			break;
		case 6:// facing down-left
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_WALK_DOWN_LEFT], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_WALK_DOWN_LEFT], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_WALK_DOWN_LEFT], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_WALK_DOWN_LEFT], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_WALK_DOWN_LEFT], false);
			break;
		case 8:// facing up
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_WALK_UP], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_WALK_UP], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_WALK_UP], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_WALK_UP], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_WALK_UP], false);
			break;
		case 9:// facing up-right
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_WALK_UP_RIGHT], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_WALK_UP_RIGHT], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_WALK_UP_RIGHT], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_WALK_UP_RIGHT], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_WALK_UP_RIGHT], false);
			break;
		case 10:// facing up-left
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_WALK_UP_LEFT], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_WALK_UP_LEFT], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_WALK_UP_LEFT], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_WALK_UP_LEFT], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_WALK_UP_LEFT], false);
			break;
		default:
			break;
	}
}

void ENEMY_SetShotAnimation(int enemy_number, int facing) {
	switch (facing) {
		case 1:// facing right
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_SHOT_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_SHOT_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_SHOT_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_SHOT_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_SHOT_RIGHT], true);
			break;
		case 2:// facing left
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_SHOT_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_SHOT_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_SHOT_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_SHOT_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_SHOT_LEFT], true);
			break;
		case 4:// facing down
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_SHOT_DOWN], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_SHOT_DOWN], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_SHOT_DOWN], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_SHOT_DOWN], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_SHOT_DOWN], true);
			break;
		case 5:// moving down-right
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_SHOT_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_SHOT_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_SHOT_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_SHOT_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_SHOT_DOWN_RIGHT], true);
			break;
		case 6:// moving down-left
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_SHOT_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_SHOT_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_SHOT_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_SHOT_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_SHOT_DOWN_LEFT], true);
			break;
		case 8:// moving up
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_SHOT_UP], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_SHOT_UP], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_SHOT_UP], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_SHOT_UP], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_SHOT_UP], true);
			break;
		case 9:// moving up-right
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_SHOT_UP_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_SHOT_UP_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_SHOT_UP_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_SHOT_UP_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_SHOT_UP_RIGHT], true);
			break;
		case 10:// moving up-left
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_SHOT_UP_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_SHOT_UP_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_SHOT_UP_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_SHOT_UP_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_SHOT_UP_LEFT], true);
			break;
		default:
			break;
	}
}

void ENEMY_SetStandAnimation(int enemy_number, int facing) {
	switch (facing) {
		case 1:// facing right
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_STAND_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_STAND_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_STAND_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_STAND_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_STAND_RIGHT], true);
			break;
		case 2:// facing left
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_STAND_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_STAND_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_STAND_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_STAND_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_STAND_LEFT], true);
			break;
		case 4:// facing down
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_STAND_DOWN], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_STAND_DOWN], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_STAND_DOWN], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_STAND_DOWN], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_STAND_DOWN], true);
			break;
		case 5:// facing down-right
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_STAND_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_STAND_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_STAND_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_STAND_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_STAND_DOWN_RIGHT], true);
			break;
		case 6:// facing down-left
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_STAND_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_STAND_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_STAND_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_STAND_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_STAND_DOWN_LEFT], true);
			break;
		case 8:// facing up
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_STAND_UP], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_STAND_UP], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_STAND_UP], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_STAND_UP], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_STAND_UP], true);
			break;
		case 9:// facing up-right
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_STAND_UP_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_STAND_UP_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_STAND_UP_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_STAND_UP_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_STAND_UP_RIGHT], true);
			break;
		case 10:// facing up-left
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_STAND_UP_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_STAND_UP_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_STAND_UP_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_STAND_UP_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_STAND_UP_LEFT], true);
			break;
		default:
			break;
	}
}

void ENEMY_SetDeadAnimation(int enemy_number, int facing) {
	switch (facing) {
		case 1:// facing right
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_DEAD_RIGHT], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_DEAD_RIGHT], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_DEAD_RIGHT], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_DEAD_RIGHT], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_DEAD_RIGHT], false);
			break;
		case 2:// facing left
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_DEAD_LEFT], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_DEAD_LEFT], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_DEAD_LEFT], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_DEAD_LEFT], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_DEAD_LEFT], false);
			break;
		case 4:// facing down
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_DEAD_DOWN], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_DEAD_DOWN], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_DEAD_DOWN], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_DEAD_DOWN], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_DEAD_DOWN], false);
			break;
		case 5:// facing down-right
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_DEAD_DOWN], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_DEAD_DOWN], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_DEAD_DOWN], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_DEAD_DOWN], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_DEAD_DOWN], false);
			break;
		case 6:// facing down-left
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_DEAD_DOWN], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_DEAD_DOWN], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_DEAD_DOWN], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_DEAD_DOWN], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_DEAD_DOWN], false);
			break;
		case 8:// facing up
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_DEAD_UP], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_DEAD_UP], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_DEAD_UP], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_DEAD_UP], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_DEAD_UP], false);
			break;
		case 9:// facing up-right
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_DEAD_UP], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_DEAD_UP], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_DEAD_UP], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_DEAD_UP], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_DEAD_UP], false);
			break;
		case 10:// facing up-left
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_DEAD_UP], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_DEAD_UP], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_DEAD_UP], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_DEAD_UP], false);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_DEAD_UP], false);
			break;
		default:
			break;
	}
}

void ENEMY_SetHitAnimation(int enemy_number, int facing) {
	switch (facing) {
		case 1:// facing right
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_HIT_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_HIT_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_HIT_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_HIT_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_HIT_RIGHT], true);
			break;
		case 2:// facing left
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_HIT_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_HIT_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_HIT_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_HIT_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_HIT_LEFT], true);
			break;
		case 4:// facing down
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_HIT_DOWN], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_HIT_DOWN], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_HIT_DOWN], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_HIT_DOWN], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_HIT_DOWN], true);
			break;
		case 5:// facing down-right
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_HIT_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_HIT_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_HIT_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_HIT_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_HIT_DOWN_RIGHT], true);
			break;
		case 6:// facing down-left
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_HIT_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_HIT_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_HIT_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_HIT_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_HIT_DOWN_LEFT], true);
			break;
		case 8:// facing up
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_HIT_UP], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_HIT_UP], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_HIT_UP], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_HIT_UP], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_HIT_UP], true);
			break;
		case 9:// facing up-right
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_HIT_UP_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_HIT_UP_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_HIT_UP_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_HIT_UP_RIGHT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_HIT_UP_RIGHT], true);
			break;
		case 10:// facing up-left
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_FEET_INDEX, enemy[enemy_number].feet_animation[ENEMY_ANIM_HIT_UP_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_BODY_INDEX, enemy[enemy_number].body_animation[ENEMY_ANIM_HIT_UP_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_HEAD_INDEX, enemy[enemy_number].head_animation[ENEMY_ANIM_HIT_UP_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_LARM_INDEX, enemy[enemy_number].left_arm_animation[ENEMY_ANIM_HIT_UP_LEFT], true);
			GFX_SetSpriteAnimation(enemy[enemy_number].sprite_num, ENEMY_ANIM_RARM_INDEX, enemy[enemy_number].right_arm_animation[ENEMY_ANIM_HIT_UP_LEFT], true);
			break;
		default:
			break;
	}
}

void ENEMY_LookAt(int enemy_number, int pos_x, int pos_y, int *facing) {
	int dx, dy, angle;

	dx = pos_x - enemy[enemy_number].middle_x;
	dy = pos_y - enemy[enemy_number].middle_y;
	angle = UTIL_GetAngle(dx, dy);
	*facing = 0;
	if ((angle > 290) || (angle < 70)) *facing = *facing | 1;        // facing right
	if ((angle > 120) && (angle < 260)) *facing = *facing | (1 << 1);// facing left
	if ((angle > 20) && (angle < 170)) *facing = *facing | (1 << 2); // facing down
	if ((angle > 200) && (angle < 320)) *facing = *facing | (1 << 3);// facing up
}

void ENEMY_RunAway(int enemy_number, int pos_x, int pos_y, byte *movement) {
	int dx, dy, angle;

	dx = enemy[enemy_number].pos_x - pos_x;
	dy = enemy[enemy_number].pos_y - pos_y;
	angle = UTIL_GetAngle(dx, dy);
	*movement = 0;
	if ((angle > 280) || (angle < 80)) *movement = *movement | 1;        // moving right
	if ((angle > 100) && (angle < 260)) *movement = *movement | (1 << 1);// moving left
	if ((angle > 10) && (angle < 170)) *movement = *movement | (1 << 2); // moving down
	if ((angle > 190) && (angle < 350)) *movement = *movement | (1 << 3);// moving up
}

void ENEMY_UnloadEnemy(int enemy_number) {
	GFX_UnloadSprite(enemy[enemy_number].sprite_num);
	enemy[enemy_number].is_loaded = false;
	enemy[enemy_number].sprite_num = -1;
}

/** ENEMY :: In game update
 */
void ENEMY_Update(void) {
	bool status_idle;
	int i, update_enemy;
	int movement;

	enemy_update_counter++;
	if (enemy_update_counter > 6) {
		enemy_update_counter = 0;
	}

	enemy_counter = 0;
	// Calculate facing direction with enemy and actor positions
	for (i = 0; i < ENEMY_MAX_ENEMIES; i++) {
		if (enemy[i].is_loaded) {
			enemy_counter++;

			// Calculate enemy middle point
			enemy[i].middle_x = GFX_GetSpriteScreenPosX(enemy[i].sprite_num) + (enemy[i].width_px >> 1);
			enemy[i].middle_y = GFX_GetSpriteScreenPosY(enemy[i].sprite_num) + (enemy[i].height_px >> 1);

			// Update enemy position on screen
			GFX_SetSpritePosition(enemy[i].sprite_num, enemy[i].pos_x - camera->pos_x, enemy[i].pos_y - camera->pos_y);

			// On screen
			enemy[i].on_screen = GFX_IsSpriteOnScreen(enemy[i].sprite_num);

			// DEBUG: Draw collision and hit pixels
			//if (enemy[i].on_screen) ENEMY_DrawColissionPixels(enemy[i]);
			//if (enemy[i].on_screen) ENEMY_DrawHitPixels(enemy[i]);

			// Avoid to update enemies each cycle
			update_enemy = (i + enemy_update_counter) & 1;
			if (update_enemy) {
				// Update range status
				// - updates enemy.can_see_actor
				// - updates enemy.in_shoot_range
				ENEMY_UpdateRangeStatus(i);

				// Set current behavior and pattern
				switch (enemy[i].status_behavior) {
					case ENEMY_STATUS_SLEEP:
						if (enemy[i].on_screen && actor->action_shoot) {
							enemy[i].status_behavior = ENEMY_STATUS_WARNING;
							enemy[i].pattern_step = 0;
						}
						if (enemy[i].is_hit) {
							enemy[i].status_behavior = ENEMY_STATUS_WARNING;
							enemy[i].pattern_step = 0;
						}
						break;
					case ENEMY_STATUS_IDLE:
						if (enemy[i].can_see_actor) {
							if (enemy[i].life >= 20) {
								enemy[i].status_behavior = ENEMY_STATUS_WARNING;
								enemy[i].pattern_step = 0;
							} else {
								enemy[i].status_behavior = ENEMY_STATUS_AVOID;
								enemy[i].pattern_step = 0;
							}
						}
						break;
					case ENEMY_STATUS_WARNING:
						enemy[i].status_behavior = ENEMY_STATUS_CHASE;
						enemy[i].pattern_step = 0;
						break;
					case ENEMY_STATUS_CHASE:
						if (enemy[i].is_hit) {
							enemy[i].status_behavior = ENEMY_STATUS_WARNING;
							enemy[i].pattern_step = 0;
						}
						if (enemy[i].in_shoot_range) {
							enemy[i].status_behavior = ENEMY_STATUS_ATTACK;
							enemy[i].pattern_step = 0;
						}
						break;
					case ENEMY_STATUS_RAMPAGE_CHASE:
						if (enemy[i].in_shoot_range) {
							enemy[i].status_behavior = ENEMY_STATUS_ATTACK;
							enemy[i].pattern_step = 0;
						}
						break;
					case ENEMY_STATUS_ATTACK:
						if (!enemy[i].in_shoot_range) {
							enemy[i].status_behavior = ENEMY_STATUS_CHASE;
							enemy[i].pattern_step = 0;
						}
						break;
					case ENEMY_STATUS_RAMPAGE:
						if (!enemy[i].in_shoot_range) {
							enemy[i].status_behavior = ENEMY_STATUS_RAMPAGE_CHASE;
							enemy[i].pattern_step = 0;
						}
						break;
					case ENEMY_STATUS_AVOID:
						if (enemy[i].life >= 10) {
							enemy[i].status_behavior = ENEMY_STATUS_CHASE;
							enemy[i].pattern_step = 0;
						}
						if (!enemy[i].in_shoot_range && enemy[i].life < 10) {
							enemy[i].status_behavior = ENEMY_STATUS_HIDE;
							enemy[i].pattern_step = 0;
						}
						break;
					case ENEMY_STATUS_HIDE:
						if (!enemy[i].can_see_actor) {
							enemy[i].status_behavior = ENEMY_STATUS_HIDE_IDLE;
							enemy[i].pattern_step = 0;
						}
						break;
					case ENEMY_STATUS_HIDE_IDLE:
						if (enemy[i].can_see_actor) {
							enemy[i].status_behavior = ENEMY_STATUS_HIDE;
							enemy[i].pattern_step = 0;
						}
						break;
					case ENEMY_STATUS_CHASE_NPC:
					case ENEMY_STATUS_STATIC_NPC:
					case ENEMY_STATUS_WALKING_ARROUND_NPC:
					case ENEMY_STATUS_WALKING_HORIZONTAL_NPC:
					case ENEMY_STATUS_WALKING_VERTICAL_NPC:
					case ENEMY_STATUS_WALKING_RANDOM_NPC:
					case ENEMY_STATUS_STATIC_SHOOTER:
						if (enemy[i].is_hit) {
							enemy[i].status_behavior = ENEMY_STATUS_WARNING;
							enemy[i].pattern_step = 0;
						}
						break;
					default:
						break;
				}

				// Check idle status
				status_idle = !enemy[i].action_stand & !enemy[i].action_walk & !enemy[i].action_dead & !enemy[i].action_shoot & !enemy[i].action_punch & !enemy[i].action_hit;

				//// ---------------- INITIALIZE ACTION ----------------

				if (status_idle) {
					enemy[i].idle_counter = 0;
					enemy[i].pattern_step++;
					if (enemy[i].pattern_step > 5) enemy[i].pattern_step = 0;

					// Set action
					switch (enemy[i].status_behavior) {
						case ENEMY_STATUS_SLEEP:
							enemy[i].current_pattern = hold_on_pattern[enemy[i].pattern_step];
							break;
						case ENEMY_STATUS_IDLE:
							enemy[i].current_pattern = hold_on_pattern[enemy[i].pattern_step];
							break;
						case ENEMY_STATUS_WARNING:
							enemy[i].current_pattern = hold_on_pattern[enemy[i].pattern_step];
							break;
						case ENEMY_STATUS_CHASE:
							enemy[i].current_pattern = chase_pattern[enemy[i].pattern_step];
							break;
						case ENEMY_STATUS_RAMPAGE_CHASE:
							enemy[i].current_pattern = rampage_chase_pattern[enemy[i].pattern_step];
							break;
						case ENEMY_STATUS_ATTACK:
							enemy[i].current_pattern = attack_pattern[enemy[i].pattern_step];
							break;
						case ENEMY_STATUS_RAMPAGE:
							enemy[i].current_pattern = rampage_pattern[enemy[i].pattern_step];
							break;
						case ENEMY_STATUS_AVOID:
							enemy[i].current_pattern = avoid_pattern[enemy[i].pattern_step];
							break;
						case ENEMY_STATUS_HIDE:
							enemy[i].current_pattern = hide_pattern[enemy[i].pattern_step];
							break;
						case ENEMY_STATUS_HIDE_IDLE:
							enemy[i].current_pattern = hold_on_pattern[enemy[i].pattern_step];
							break;
						case ENEMY_STATUS_STATIC_NPC:
							enemy[i].current_pattern = hold_on_pattern[enemy[i].pattern_step];
							break;
						case ENEMY_STATUS_CHASE_NPC:
							enemy[i].current_pattern = chase_pattern[enemy[i].pattern_step];
							break;
						case ENEMY_STATUS_WALKING_ARROUND_NPC:
							enemy[i].current_pattern = walk_arround_pattern[enemy[i].pattern_step];
							break;
						case ENEMY_STATUS_WALKING_HORIZONTAL_NPC:
							enemy[i].current_pattern = walk_horizontal_pattern[enemy[i].pattern_step];
							break;
						case ENEMY_STATUS_WALKING_VERTICAL_NPC:
							enemy[i].current_pattern = walk_vertical_pattern[enemy[i].pattern_step];
							break;
						case ENEMY_STATUS_WALKING_RANDOM_NPC:
							enemy[i].current_pattern = walk_random_pattern[enemy[i].pattern_step];
							break;
						case ENEMY_STATUS_STATIC_SHOOTER:
							enemy[i].current_pattern = shooter_pattern[enemy[i].pattern_step];
							break;
						default:
							enemy[i].current_pattern = hold_on_pattern[enemy[i].pattern_step];
							break;
					}

					switch (enemy[i].current_pattern) {
						case ENEMY_PATTERN_HOLD_ON:
							status_idle = false;
							enemy[i].action_stand = true;
							enemy[i].action_walk = false;
							enemy[i].action_punch = false;
							enemy[i].action_shoot = false;

							enemy[i].movement = 0;
							enemy[i].action_step = 0;
							enemy[i].last_action = ENEMY_ACTION_STAND;
							ENEMY_SetStandAnimation(i, enemy[i].status_facing);

							break;
						case ENEMY_PATTERN_WALK_BACK:
							status_idle = false;
							enemy[i].action_walk = true;
							enemy[i].action_punch = false;
							enemy[i].action_shoot = false;
							enemy[i].action_stand = false;

							ENEMY_RunAway(i, actor->middle_x, actor->middle_y, &enemy[i].movement);
							enemy[i].status_facing = enemy[i].movement;
							enemy[i].action_step = 0;
							enemy[i].last_action = ENEMY_ACTION_WALK;
							ENEMY_SetWalkAnimation(i, enemy[i].status_facing);

							break;
						case ENEMY_PATTERN_WALK_FORWARD:
							status_idle = false;
							enemy[i].action_walk = true;
							enemy[i].action_punch = false;
							enemy[i].action_shoot = false;
							enemy[i].action_stand = false;

							ENEMY_LookAt(i, actor->middle_x, actor->middle_y, &enemy[i].status_facing);
							enemy[i].movement = enemy[i].status_facing;
							enemy[i].action_step = 0;
							enemy[i].last_action = ENEMY_ACTION_WALK;
							ENEMY_SetWalkAnimation(i, enemy[i].status_facing);
							break;
						case ENEMY_PATTERN_WALK_ARROUND:
							status_idle = false;
							enemy[i].action_walk = true;
							enemy[i].action_punch = false;
							enemy[i].action_shoot = false;
							enemy[i].action_stand = false;

							ENEMY_LookAt(i, actor->middle_x + rand() % 0xF, actor->middle_y + rand() % 0xF, &enemy[i].status_facing);
							enemy[i].movement = enemy[i].status_facing;
							enemy[i].action_step = 0;
							enemy[i].last_action = ENEMY_ACTION_WALK;
							ENEMY_SetWalkAnimation(i, enemy[i].status_facing);
							break;

						case ENEMY_PATTERN_ATTACK:
							if (enemy[i].in_shoot_range) {
								status_idle = false;
								// Shot
								if (enemy[i].gun->type != ENEMY_GUN_BARE_HANDS) {

									enemy[i].action_shoot = true;
									enemy[i].action_punch = false;
									enemy[i].action_walk = false;
									enemy[i].action_stand = false;

									enemy[i].movement = 0;
									enemy[i].action_step = 0;
									enemy[i].last_action = ENEMY_ACTION_SHOOT;
								}

								// punch
								if (enemy[i].gun->type == ENEMY_GUN_BARE_HANDS) {

									enemy[i].action_punch = true;
									enemy[i].action_shoot = false;
									enemy[i].action_walk = false;
									enemy[i].action_stand = false;

									enemy[i].movement = 0;
									enemy[i].action_step = 0;
									enemy[i].last_action = ENEMY_ACTION_PUNCH;
								}
							}
							ENEMY_LookAt(i, actor->middle_x, actor->middle_y, &enemy[i].status_facing);
							break;
						case ENEMY_PATTERN_NPC_WALK_UP:
							status_idle = false;
							enemy[i].action_walk = true;
							enemy[i].action_punch = false;
							enemy[i].action_shoot = false;
							enemy[i].action_stand = false;

							enemy[i].movement = ENEMY_FACING_UP;
							enemy[i].action_step = 0;
							enemy[i].last_action = ENEMY_ACTION_WALK;
							ENEMY_SetWalkAnimation(i, ENEMY_FACING_UP);
							break;
						case ENEMY_PATTERN_NPC_WALK_RIGHT:
							status_idle = false;
							enemy[i].action_walk = true;
							enemy[i].action_punch = false;
							enemy[i].action_shoot = false;
							enemy[i].action_stand = false;

							enemy[i].movement = ENEMY_FACING_RIGHT;
							enemy[i].action_step = 0;
							enemy[i].last_action = ENEMY_ACTION_WALK;
							ENEMY_SetWalkAnimation(i, ENEMY_FACING_RIGHT);
							break;
						case ENEMY_PATTERN_NPC_WALK_DOWN:
							status_idle = false;
							enemy[i].action_walk = true;
							enemy[i].action_punch = false;
							enemy[i].action_shoot = false;
							enemy[i].action_stand = false;

							enemy[i].movement = ENEMY_FACING_DOWN;
							enemy[i].action_step = 0;
							enemy[i].last_action = ENEMY_ACTION_WALK;
							ENEMY_SetWalkAnimation(i, ENEMY_FACING_DOWN);
							break;
						case ENEMY_PATTERN_NPC_WALK_LEFT:
							status_idle = false;
							enemy[i].action_walk = true;
							enemy[i].action_punch = false;
							enemy[i].action_shoot = false;
							enemy[i].action_stand = false;

							enemy[i].movement = ENEMY_FACING_LEFT;
							enemy[i].action_step = 0;
							enemy[i].last_action = ENEMY_ACTION_WALK;
							ENEMY_SetWalkAnimation(i, ENEMY_FACING_LEFT);
							break;
						case ENEMY_PATTERN_NPC_WALK_RANDOM:
							status_idle = false;
							enemy[i].action_walk = true;
							enemy[i].action_punch = false;
							enemy[i].action_shoot = false;
							enemy[i].action_stand = false;
							movement = rand() % 8;
							enemy[i].movement = facing_array[movement];
							enemy[i].action_step = 0;
							enemy[i].last_action = ENEMY_ACTION_WALK;
							ENEMY_SetWalkAnimation(i, enemy[i].movement);
							break;
						default:
							break;
					}
				}

				//// ---------------- ACTION ANIMATION ----------------
				// Shooting and animation
				if (enemy[i].action_shoot) {
					switch (enemy[i].action_step) {
						case 0:// Calculate target
							enemy[i].shoot_x = actor->pos_x + (actor->width_px >> 1);
							enemy[i].shoot_y = actor->pos_y + (actor->height_px >> 1);
							enemy[i].gun->current_recoil = 0;
							AUDIO_PlaySound(AUDIO_GUN_EFFECT, 1);
							ENEMY_SetShotAnimation(i, enemy[i].status_facing);
							BULLET_LoadBullet(enemy[i].gun->bullet_graphics_id, ENTITY_ID_ENEMY_BULLET, 4, 4, enemy[i].pos_x + (enemy[i].width_px >> 1), enemy[i].pos_y + (enemy[i].height_px >> 1), enemy[i].shoot_x, enemy[i].shoot_y, enemy[i].shoot_accuracy, enemy[i].gun->max_distance, enemy[i].gun->bullet_speed, enemy[i].gun->damage);
							enemy[i].action_step++;
							break;
						case 1:// animation end
							if (GFX_IsSpriteAnimationEnded(enemy[i].sprite_num, 0)) enemy[i].action_step++;
							break;
						case 2:// recoil time
							enemy[i].gun->current_recoil++;
							if (enemy[i].gun->current_recoil >= enemy[i].gun->recoil_time) {
								enemy[i].gun->current_recoil = 0;
								enemy[i].action_step++;
							}
							break;
						case 3:// end
							enemy[i].action_step = 0;
							enemy[i].action_shoot = false;
							break;
						default:
							enemy[i].action_step = 0;
							enemy[i].action_shoot = false;
							break;
					}
				}

				// Puch and animation
				if (enemy[i].action_punch) {
					switch (enemy[i].action_step) {
						case 0:// set animation
							ENEMY_SetPunchAnimation(i, enemy[i].status_facing);
							enemy[i].action_step++;
							break;
						case 1:// Set hit box
							if (gfx_sprite_stack[enemy[i].sprite_num].gfx[ENEMY_ANIM_FEET_INDEX].anim_current_frame == 2) {
								switch (enemy[i].status_facing) {
									case 1:// facing right
										if (gfx_sprite_stack[enemy[i].sprite_num].gfx[ENEMY_ANIM_FEET_INDEX].anim_current_frame == 2) BULLET_LoadBullet(enemy[i].gun->bullet_graphics_id, ENTITY_ID_ENEMY_BULLET, 8, 8, enemy[i].pos_x + enemy[i].width_px + 2, enemy[i].pos_y + (enemy[i].height_px >> 1), enemy[i].pos_x + enemy[i].width_px + 2, enemy[i].pos_y + (enemy[i].height_px >> 1), enemy[i].shoot_accuracy, enemy[i].gun->max_distance, enemy[i].gun->bullet_speed, enemy[i].gun->damage);
										break;
									case 2:// facing left
										if (gfx_sprite_stack[enemy[i].sprite_num].gfx[ENEMY_ANIM_FEET_INDEX].anim_current_frame == 2) BULLET_LoadBullet(enemy[i].gun->bullet_graphics_id, ENTITY_ID_ENEMY_BULLET, 8, 8, enemy[i].pos_x - 2, enemy[i].pos_y + (enemy[i].height_px >> 1), enemy[i].pos_x - 2, enemy[i].pos_y + (enemy[i].height_px >> 1), enemy[i].shoot_accuracy, enemy[i].gun->max_distance, enemy[i].gun->bullet_speed, enemy[i].gun->damage);
										break;
									case 4:// facing down
										if (gfx_sprite_stack[enemy[i].sprite_num].gfx[ENEMY_ANIM_FEET_INDEX].anim_current_frame == 2) BULLET_LoadBullet(enemy[i].gun->bullet_graphics_id, ENTITY_ID_ENEMY_BULLET, 8, 8, enemy[i].pos_x + (enemy[i].width_px >> 2), enemy[i].pos_y + enemy[i].height_px + 2, enemy[i].pos_x + (enemy[i].width_px >> 2), enemy[i].pos_y + enemy[i].height_px + 2, enemy[i].shoot_accuracy, enemy[i].gun->max_distance, enemy[i].gun->bullet_speed, enemy[i].gun->damage);
										break;
									case 5:// moving down-right
										if (gfx_sprite_stack[enemy[i].sprite_num].gfx[ENEMY_ANIM_FEET_INDEX].anim_current_frame == 2) BULLET_LoadBullet(enemy[i].gun->bullet_graphics_id, ENTITY_ID_ENEMY_BULLET, 8, 8, enemy[i].pos_x + enemy[i].width_px + 2, enemy[i].pos_y + enemy[i].height_px + 2, enemy[i].pos_x + enemy[i].width_px + 2, enemy[i].pos_y + enemy[i].height_px + 2, enemy[i].shoot_accuracy, enemy[i].gun->max_distance, enemy[i].gun->bullet_speed, enemy[i].gun->damage);
										break;
									case 6:// moving down-left
										if (gfx_sprite_stack[enemy[i].sprite_num].gfx[ENEMY_ANIM_FEET_INDEX].anim_current_frame == 2) BULLET_LoadBullet(enemy[i].gun->bullet_graphics_id, ENTITY_ID_ENEMY_BULLET, 8, 8, enemy[i].pos_x - 2, enemy[i].pos_y + enemy[i].height_px + 2, enemy[i].pos_x - 2, enemy[i].pos_y + enemy[i].height_px + 2, enemy[i].shoot_accuracy, enemy[i].gun->max_distance, enemy[i].gun->bullet_speed, enemy[i].gun->damage);
										break;
									case 8:// moving up
										if (gfx_sprite_stack[enemy[i].sprite_num].gfx[ENEMY_ANIM_FEET_INDEX].anim_current_frame == 2) BULLET_LoadBullet(enemy[i].gun->bullet_graphics_id, ENTITY_ID_ENEMY_BULLET, 8, 8, enemy[i].pos_x + (enemy[i].width_px >> 2), enemy[i].pos_y - 2, enemy[i].pos_x + (enemy[i].width_px >> 2), enemy[i].pos_y - 2, enemy[i].shoot_accuracy, enemy[i].gun->max_distance, enemy[i].gun->bullet_speed, enemy[i].gun->damage);
										break;
									case 9:// moving up-right
										if (gfx_sprite_stack[enemy[i].sprite_num].gfx[ENEMY_ANIM_FEET_INDEX].anim_current_frame == 2) BULLET_LoadBullet(enemy[i].gun->bullet_graphics_id, ENTITY_ID_ENEMY_BULLET, 8, 8, enemy[i].pos_x + enemy[i].width_px + 2, enemy[i].pos_y - 2, enemy[i].pos_x + enemy[i].width_px + 2, enemy[i].pos_y - 2, enemy[i].shoot_accuracy, enemy[i].gun->max_distance, enemy[i].gun->bullet_speed, enemy[i].gun->damage);
										break;
									case 10:// moving up-left
										if (gfx_sprite_stack[enemy[i].sprite_num].gfx[ENEMY_ANIM_FEET_INDEX].anim_current_frame == 2) BULLET_LoadBullet(enemy[i].gun->bullet_graphics_id, ENTITY_ID_ENEMY_BULLET, 8, 8, enemy[i].pos_x - 2, enemy[i].pos_y - 2, enemy[i].pos_x - 2, enemy[i].pos_y - 2, enemy[i].shoot_accuracy, enemy[i].gun->max_distance, enemy[i].gun->bullet_speed, enemy[i].gun->damage);
										break;
									default:
										break;
								}
								enemy[i].action_step++;
							}
						case 2:// end
							if (GFX_IsSpriteAnimationEnded(enemy[i].sprite_num, 0)) {
								enemy[i].action_step = 0;
								enemy[i].action_punch = false;
							}
							break;
						default:
							enemy[i].action_step = 0;
							enemy[i].action_punch = false;
							break;
					}
				}

				// Walking and animation
				if (enemy[i].action_walk) {
					switch (enemy[i].movement) {
						case 1:// moving right
							enemy[i].x_FP += enemy[i].speed_horizontal_FP;
							enemy[i].pos_x = enemy[i].x_FP >> FP;
							if (ENEMY_CheckRightColission(enemy[i])) {
								enemy[i].x_FP -= enemy[i].speed_horizontal_FP;
								enemy[i].pos_x = enemy[i].x_FP >> FP;
								// Change direction >> up
								if (!ENEMY_CheckUpColission(enemy[i])) {
									enemy[i].movement = ENEMY_MOVING_UP;
								}
								// Change direction >> down
								else if (!ENEMY_CheckDownColission(enemy[i])) {
									enemy[i].movement = ENEMY_MOVING_DOWN;
								}
							}
							break;
						case 2:// moving left
							enemy[i].x_FP -= enemy[i].speed_horizontal_FP;
							enemy[i].pos_x = enemy[i].x_FP >> FP;
							if (ENEMY_CheckLeftColission(enemy[i])) {
								enemy[i].x_FP += enemy[i].speed_horizontal_FP;
								enemy[i].pos_x = enemy[i].x_FP >> FP;
								// Change direction >> up
								if (!ENEMY_CheckUpColission(enemy[i])) {
									enemy[i].movement = ENEMY_MOVING_UP;
								}
								// Change direction >> down
								else if (!ENEMY_CheckDownColission(enemy[i])) {
									enemy[i].movement = ENEMY_MOVING_DOWN;
								}
							}
							break;
						case 4:// moving down
							enemy[i].y_FP += enemy[i].speed_vertical_FP;
							enemy[i].pos_y = enemy[i].y_FP >> FP;
							if (ENEMY_CheckDownColission(enemy[i])) {
								enemy[i].y_FP -= enemy[i].speed_vertical_FP;
								enemy[i].pos_y = enemy[i].y_FP >> FP;
								// Change direction >> left
								if (!ENEMY_CheckLeftColission(enemy[i])) {
									enemy[i].movement = ENEMY_MOVING_LEFT;
								}
								// Change direction >> up
								else if (!ENEMY_CheckRightColission(enemy[i])) {
									enemy[i].movement = ENEMY_MOVING_RIGHT;
								}
							}
							break;
						case 5:// moving down-right
							enemy[i].y_FP += enemy[i].speed_diagonalv_FP;
							enemy[i].pos_y = enemy[i].y_FP >> FP;
							if (ENEMY_CheckDownColission(enemy[i])) {
								enemy[i].y_FP -= enemy[i].speed_diagonalv_FP;
								enemy[i].pos_y = enemy[i].y_FP >> FP;
								// Change direction >> up
								if (!ENEMY_CheckUpColission(enemy[i])) {
									enemy[i].movement = ENEMY_MOVING_UP;
								}
							}
							enemy[i].x_FP += enemy[i].speed_diagonalh_FP;
							enemy[i].pos_x = enemy[i].x_FP >> FP;
							if (ENEMY_CheckRightColission(enemy[i])) {
								enemy[i].x_FP -= enemy[i].speed_diagonalh_FP;
								enemy[i].pos_x = enemy[i].x_FP >> FP;
								// Change direction >> left
								if (!ENEMY_CheckLeftColission(enemy[i])) {
									enemy[i].movement = ENEMY_MOVING_LEFT;
								}
							}
							break;
						case 6:// moving down-left
							enemy[i].y_FP += enemy[i].speed_diagonalv_FP;
							enemy[i].pos_y = enemy[i].y_FP >> FP;
							if (ENEMY_CheckDownColission(enemy[i])) {
								enemy[i].y_FP -= enemy[i].speed_diagonalv_FP;
								enemy[i].pos_y = enemy[i].y_FP >> FP;
								// Change direction >> up
								if (!ENEMY_CheckUpColission(enemy[i])) {
									enemy[i].movement = ENEMY_MOVING_UP;
								}
							}
							enemy[i].x_FP -= enemy[i].speed_diagonalh_FP;
							enemy[i].pos_x = enemy[i].x_FP >> FP;
							if (ENEMY_CheckLeftColission(enemy[i])) {
								enemy[i].x_FP += enemy[i].speed_diagonalh_FP;
								enemy[i].pos_x = enemy[i].x_FP >> FP;
								// Change direction >> right
								if (!ENEMY_CheckRightColission(enemy[i])) {
									enemy[i].movement = ENEMY_MOVING_RIGHT;
								}
							}
							break;
						case 8:// moving up
							enemy[i].y_FP -= enemy[i].speed_vertical_FP;
							enemy[i].pos_y = enemy[i].y_FP >> FP;
							if (ENEMY_CheckUpColission(enemy[i])) {
								enemy[i].y_FP += enemy[i].speed_vertical_FP;
								enemy[i].pos_y = enemy[i].y_FP >> FP;
								// Change direction >> left
								if (!ENEMY_CheckLeftColission(enemy[i])) {
									enemy[i].movement = ENEMY_MOVING_LEFT;
								}
								// Change direction >> right
								else if (!ENEMY_CheckRightColission(enemy[i])) {
									enemy[i].movement = ENEMY_MOVING_RIGHT;
								}
							}
							break;
						case 9:// moving up-right
							enemy[i].y_FP -= enemy[i].speed_diagonalv_FP;
							enemy[i].pos_y = enemy[i].y_FP >> FP;
							if (ENEMY_CheckUpColission(enemy[i])) {
								enemy[i].y_FP += enemy[i].speed_diagonalv_FP;
								enemy[i].pos_y = enemy[i].y_FP >> FP;
								// Change direction >> down
								if (!ENEMY_CheckDownColission(enemy[i])) {
									enemy[i].movement = ENEMY_MOVING_DOWN;
								}
							}
							enemy[i].x_FP += enemy[i].speed_diagonalh_FP;
							enemy[i].pos_x = enemy[i].x_FP >> FP;
							if (ENEMY_CheckRightColission(enemy[i])) {
								enemy[i].x_FP -= enemy[i].speed_diagonalh_FP;
								enemy[i].pos_x = enemy[i].x_FP >> FP;
								// Change direction >> left
								if (!ENEMY_CheckLeftColission(enemy[i])) {
									enemy[i].movement = ENEMY_MOVING_LEFT;
								}
							}
							break;
						case 10:// moving up-left
							enemy[i].y_FP -= enemy[i].speed_diagonalv_FP;
							enemy[i].pos_y = enemy[i].y_FP >> FP;
							if (ENEMY_CheckUpColission(enemy[i])) {
								enemy[i].y_FP += enemy[i].speed_diagonalv_FP;
								enemy[i].pos_y = enemy[i].y_FP >> FP;
								// Change direction >> down
								if (!ENEMY_CheckDownColission(enemy[i])) {
									enemy[i].movement = ENEMY_MOVING_DOWN;
								}
							}
							enemy[i].x_FP -= enemy[i].speed_diagonalh_FP;
							enemy[i].pos_x = enemy[i].x_FP >> FP;
							if (ENEMY_CheckLeftColission(enemy[i])) {
								enemy[i].x_FP += enemy[i].speed_diagonalh_FP;
								enemy[i].pos_x = enemy[i].x_FP >> FP;
								// Change direction >> right
								if (!ENEMY_CheckRightColission(enemy[i])) {
									enemy[i].movement = ENEMY_MOVING_RIGHT;
								}
							}
							break;
						default:
							break;
					}

					enemy[i].idle_counter++;
					if (enemy[i].idle_counter > ENEMY_IDLE_TICKS) {
						enemy[i].action_walk = false;
						enemy[i].idle_counter = 0;
					}
				}

				// Stand and animation
				if (enemy[i].action_stand) {
					enemy[i].idle_counter++;
					if (enemy[i].idle_counter > ENEMY_IDLE_TICKS) {
						enemy[i].action_stand = false;
						enemy[i].idle_counter = 0;
					}
				}

				// Check hit by something
				//  - sets "is_hit" property
				if (!enemy[i].action_dead) {// avoid double hit when is already hit

					// Check for particles
					/*for (j = 0; j < PARTICLE_MAX_PARTICLES; j++) {
						if (particle[j].loaded) {
							if (particle[j].pos_x + particle[j].colission_area.points[0][0] < enemy[i].pos_x + enemy[i].hit_area.points[1][0]

								&& particle[j].pos_x + particle[j].colission_area.points[1][0] > enemy[i].pos_x + enemy[i].hit_area.points[0][0]

								&& particle[j].pos_y + particle[j].colission_area.points[0][1] < enemy[i].pos_y + enemy[i].hit_area.points[2][1]

								&& particle[j].pos_y + particle[j].colission_area.points[2][1] > enemy[i].pos_y + enemy[i].hit_area.points[0][1]) {
								enemy[i].hit_by = gfx_sprite_stack[particle[j].sprite_num].id;
								enemy[i].is_hit = true;
								enemy[i].damage += particle[j].damage;

								enemy[i].hit_vx_FP = particle[j].vx_FP;
								enemy[i].hit_vy_FP = particle[j].vy_FP;
							}
						}
					}

					// Check for bullets
					for (j = 0; j < BULLET_MAX_BULLETS; j++) {
						if (bullet[j].loaded) {
							switch (gfx_sprite_stack[bullet[j].sprite_num].entity_id) {
								case ENTITY_ID_ACTOR_BULLET:
								case ENTITY_ID_ACTOR_PUNCH:
								case ENTITY_ID_ACTOR_KICK:
									if (bullet[j].pos_x + bullet[j].colission_area.points[0][0] < enemy[i].pos_x + enemy[i].hit_area.points[1][0]

										&& bullet[j].pos_x + bullet[j].colission_area.points[1][0] > enemy[i].pos_x + enemy[i].hit_area.points[0][0]

										&& bullet[j].pos_y + bullet[j].colission_area.points[0][1] < enemy[i].pos_y + enemy[i].hit_area.points[2][1]

										&& bullet[j].pos_y + bullet[j].colission_area.points[2][1] > enemy[i].pos_y + enemy[i].hit_area.points[0][1]) {
										enemy[i].hit_by = gfx_sprite_stack[bullet[j].sprite_num].id;
										enemy[i].is_hit = true;
										enemy[i].damage += bullet[j].damage;

										enemy[i].hit_vx_FP = bullet[j].vx_FP;
										enemy[i].hit_vy_FP = bullet[j].vy_FP;
									}
									break;
							}
						}
					}*/
				}

				// Checks if is hit but still alive
				// - sets action hit or dead
				if (enemy[i].is_hit && !enemy[i].action_dead) {
					enemy[i].is_hit = false;
					status_idle = false;
					// Cancel any action
					enemy[i].action_punch = false;
					enemy[i].action_shoot = false;
					enemy[i].action_walk = false;
					enemy[i].action_stand = false;
					enemy[i].action_dead = false;

					if (enemy[i].damage >= enemy[i].life) {
						enemy[i].life = 0;
						enemy[i].damage = 0;
						enemy[i].action_step = 0;
						enemy[i].action_dead = true;
						enemy[i].action_hit = false;

					} else {
						enemy[i].life -= enemy[i].damage;
						enemy[i].damage = 0;
						if (!enemy[i].action_hit) {
							enemy[i].action_step = 0;
							enemy[i].action_hit = true;
						}
					}
					GFX_SetPanelGraphics(gfx_enemy_status_panel, enemy[i].gfx_portait, SPRITE_GRAPHICS_ID_LIFEBAR, SPRITE_GRAPHICS_ID_GUN0, SPRITE_GRAPHICS_ID_EMPTY);
					GFX_UpdatePanel(gfx_enemy_status_panel, enemy[i].life, enemy[i].life + enemy[i].damage, enemy[i].max_life, 1);
					GFX_ShowPanel(gfx_enemy_status_panel, true, 200);
				}

				// Check if is hit
				if (enemy[i].action_hit) {
					switch (enemy[i].action_step) {
						case 0:
							GFX_SetSpriteBlinkingProperties(enemy[i].sprite_num, true, 1, SPRITE_HIT_COLOR);
							enemy[i].action_step++;
							break;
						case 1:
							switch (enemy[i].hit_by) {
								case ENTITY_ID_ACTOR_BULLET:
									ENEMY_SetHitAnimation(i, enemy[i].status_facing);
									break;
								case ENTITY_ID_ACTOR_PUNCH:
									ENEMY_SetHitAnimation(i, enemy[i].status_facing);
									break;
								case ENTITY_ID_ACTOR_KICK:
									ENEMY_SetDeadAnimation(i, enemy[i].status_facing);
									enemy[i].x_FP += enemy[i].hit_vx_FP << 2;
									enemy[i].y_FP += enemy[i].hit_vy_FP << 2;
									enemy[i].pos_x = enemy[i].x_FP >> FP;
									enemy[i].pos_y = enemy[i].y_FP >> FP;
									break;
								default:
									break;
							}
							enemy[i].action_step++;
							break;
						case 2:
							EFFECT_LoadEffect(ENTITY_ID_BLOOD, SPRITE_GRAPHICS_ID_BLOOD, enemy[i].pos_x + (rand() % 32), enemy[i].pos_y + (rand() % 32), true, 0, false, false, 5);
							EFFECT_LoadEffect(ENTITY_ID_BLOOD, SPRITE_GRAPHICS_ID_BLOOD, enemy[i].pos_x + (rand() % 32), enemy[i].pos_y + (rand() % 32), true, 0, false, false, 5);
							enemy[i].action_step++;
							break;
						case 3:// Move back
							enemy[i].reaction_counter = 0;
							enemy[i].action_step++;
							break;
						case 4:// finish action
							if (GFX_IsSpriteAnimationEnded(enemy[i].sprite_num, 0)) {
								enemy[i].reaction_counter++;
								if (enemy[i].reaction_counter > 10) {
									enemy[i].action_hit = false;
									enemy[i].reaction_counter = 0;
									GFX_SetSpriteBlinkingProperties(enemy[i].sprite_num, false, 1, SPRITE_HIT_COLOR);
								}
							}
							break;
						default:
							enemy[i].action_hit = false;
							break;
					}
				}

				// Check if is dead
				if (enemy[i].action_dead) {

					switch (enemy[i].action_step) {
						case 0:// Set death animation
							ENEMY_SetDeadAnimation(i, enemy[i].status_facing);
							GFX_SetPanelPortait(gfx_enemy_status_panel, 2);
							enemy[i].life = 0;
							enemy[i].movement = 0;
							enemy[i].action_step++;
							break;
						case 1:
							EFFECT_LoadEffect(ENTITY_ID_BLOOD, SPRITE_GRAPHICS_ID_BLOOD, enemy[i].pos_x + (rand() % 32), enemy[i].pos_y + (rand() % 32), true, 0, false, false, 5);
							//enemy[i].x_FP += enemy[i].hit_vx_FP;
							//enemy[i].y_FP += enemy[i].hit_vy_FP;
							//enemy[i].pos_x = enemy[i].x_FP >> FP;
							//enemy[i].pos_y = enemy[i].y_FP >> FP;
							if (GFX_IsSpriteAnimationEnded(enemy[i].sprite_num, 0)) {
								enemy[i].action_step++;
								enemy[i].idle_counter = 0;
							}
							break;
						case 2:
							enemy[i].idle_counter++;
							if (enemy[i].idle_counter > ENEMY_IDLE_TICKS) {
								enemy[i].action_step++;
								enemy[i].idle_counter = 0;
								GFX_SetSpriteBlinkingProperties(enemy[i].sprite_num, true, 5, SPRITE_TRANSP_COLOR);
							}
							break;
						case 3:// Unload enemy
							enemy[i].idle_counter++;
							if (enemy[i].idle_counter > ENEMY_IDLE_TICKS) {
								enemy[i].action_step++;
								enemy[i].idle_counter = 0;
							}
							break;
						case 4:
							ENEMY_UnloadEnemy(i);

							break;
						default:
							break;
					}
				}
			}
		}
	}
}

void ENEMY_UnloadEnemies(void) {
	int i;
	for (i = 0; i < ENEMY_MAX_ENEMIES; i++) {
		if (enemy[i].is_loaded) {
			GFX_UnloadSprite(enemy[i].sprite_num);
			enemy[i].is_loaded = false;
			enemy[i].sprite_num = -1;
		}
	}
}

void ENEMY_Unload(int enemy_number) {
	if (enemy[enemy_number].is_loaded) {
		GFX_UnloadSprite(enemy[enemy_number].sprite_num);
		enemy[enemy_number].is_loaded = false;
		enemy[enemy_number].sprite_num = -1;
	}
}