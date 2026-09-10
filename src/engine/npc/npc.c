#include "../engine.h"
#include "stdbool.h"
#include <iso646.h>
#include <stdlib.h>
#include "npc.h"

NPC *npc;

static byte npc_update_counter;
byte npc_counter;

static int facing_array[8] = {ENEMY_FACING_RIGHT, ENEMY_FACING_LEFT, ENEMY_FACING_DOWN, ENEMY_FACING_DOWN_RIGHT, ENEMY_FACING_DOWN_LEFT, ENEMY_FACING_UP, ENEMY_FACING_UP_RIGHT, ENEMY_FACING_UP_LEFT};
static int hold_on_pattern[6] = {ENEMY_PATTERN_HOLD_ON, ENEMY_PATTERN_HOLD_ON, ENEMY_PATTERN_HOLD_ON, ENEMY_PATTERN_HOLD_ON, ENEMY_PATTERN_HOLD_ON, ENEMY_PATTERN_HOLD_ON};
static int walk_random_pattern[6] = {ENEMY_PATTERN_HOLD_ON, ENEMY_PATTERN_NPC_WALK_RANDOM, ENEMY_PATTERN_NPC_WALK_RANDOM, ENEMY_PATTERN_NPC_WALK_RANDOM, ENEMY_PATTERN_NPC_WALK_RANDOM, ENEMY_PATTERN_NPC_WALK_RANDOM};
static int walk_arround_pattern[6] = {ENEMY_PATTERN_HOLD_ON, ENEMY_PATTERN_NPC_WALK_UP, ENEMY_PATTERN_NPC_WALK_RIGHT, ENEMY_PATTERN_NPC_WALK_DOWN, ENEMY_PATTERN_NPC_WALK_LEFT, ENEMY_PATTERN_HOLD_ON};
static int walk_horizontal_pattern[6] = {ENEMY_PATTERN_HOLD_ON, ENEMY_PATTERN_NPC_WALK_RIGHT, ENEMY_PATTERN_NPC_WALK_LEFT, ENEMY_PATTERN_NPC_WALK_LEFT, ENEMY_PATTERN_NPC_WALK_RIGHT, ENEMY_PATTERN_HOLD_ON};
static int walk_vertical_pattern[6] = {ENEMY_PATTERN_HOLD_ON, ENEMY_PATTERN_NPC_WALK_UP, ENEMY_PATTERN_NPC_WALK_DOWN, ENEMY_PATTERN_NPC_WALK_DOWN, ENEMY_PATTERN_NPC_WALK_UP, ENEMY_PATTERN_HOLD_ON};
static int chase_pattern[6] = {ENEMY_PATTERN_WALK_FORWARD, ENEMY_PATTERN_WALK_FORWARD, ENEMY_PATTERN_WALK_FORWARD, ENEMY_PATTERN_WALK_ARROUND, ENEMY_PATTERN_WALK_FORWARD, ENEMY_PATTERN_WALK_FORWARD};
static int avoid_pattern[6] = {ENEMY_PATTERN_WALK_BACK, ENEMY_PATTERN_WALK_ARROUND, ENEMY_PATTERN_WALK_FORWARD, ENEMY_PATTERN_WALK_ARROUND, ENEMY_PATTERN_WALK_FORWARD, ENEMY_PATTERN_ATTACK};
static int hide_pattern[6] = {ENEMY_PATTERN_WALK_BACK, ENEMY_PATTERN_WALK_BACK, ENEMY_PATTERN_WALK_ARROUND, ENEMY_PATTERN_WALK_BACK, ENEMY_PATTERN_WALK_BACK, ENEMY_PATTERN_WALK_BACK};

void NPC_Init(void) {
	int i;

	npc = MM_PushChunk(sizeof(NPC) * NPC_MAX_NPCS, CT_ENGINE);
	for (i = 0; i < NPC_MAX_NPCS; i++) {
		npc[i].is_loaded = false;
		npc[i].sprite_num = -1;
		npc[i].cfg = MM_PushChunk(sizeof(SpriteConfig), CT_ENGINE);
	}
}

void NPC_Load(const char *dat_name, byte number, int x, int y, int face_gfx_id, int sprite_gfx_id, int facing, int behavior, int life) {

	int sprite_slot;

	// Check if enemy is already loaded
	if (npc[number].is_loaded) {
		sprintf(engine.system_error_message1, "NPC_Init function error");
		sprintf(engine.system_error_message2, "NPC %u already loaded", number);
		sprintf(engine.system_error_message3, " ");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_SYSTEM);
	}

	// Load configuration
	FILE_LoadSpriteConfigFile(dat_name, "NPC.CFG", npc[number].cfg);

	npc[number].width_px = npc[number].cfg->width;
	npc[number].height_px = npc[number].cfg->height;

	// Load graphics
	if (!gfx_sprite_graphics_stack[face_gfx_id].loaded) GFX_LoadSpriteGraphicsRLE(dat_name, "FACE.PCX", face_gfx_id, npc[number].cfg->face_width, npc[number].cfg->face_height, npc[number].cfg->face_frames, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_TEMPORARY_SPRITE);
	if (!gfx_sprite_graphics_stack[sprite_gfx_id].loaded) GFX_LoadSpriteGraphicsRLE(dat_name, "SPRITE.PCX", sprite_gfx_id, npc[number].cfg->feet_width, npc[number].cfg->feet_height, npc[number].cfg->feet_frames, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_TEMPORARY_SPRITE);

	npc[number].is_loaded = true;

	npc[number].status_behavior = behavior;

	npc[number].pos_x = x;
	npc[number].pos_y = y;
	npc[number].x_FP = x << FP;
	npc[number].y_FP = y << FP;
	npc[number].vx_FP = 0;
	npc[number].vy_FP = 0;

	npc[number].status_facing = facing;

	npc[number].speed_horizontal_FP = NPC_SPEED << FP;
	npc[number].speed_vertical_FP = (NPC_SPEED << FP) - ((NPC_SPEED << FP) >> 3);
	npc[number].speed_diagonalv_FP = (npc[number].speed_vertical_FP * 10) / 14;
	npc[number].speed_diagonalh_FP = (npc[number].speed_horizontal_FP * 10) / 14;

	npc[number].speed = ENEMY_SPEED;
	npc[number].max_life = life;
	npc[number].life = life;
	npc[number].damage = 0;
	npc[number].is_hit = false;

	npc[number].movement_allowed = true;
	npc[number].reaction_time = 70;

	npc[number].action_stand = false;
	npc[number].action_walk = false;
	npc[number].action_hit = false;
	npc[number].action_dead = false;

	sprite_slot = GFX_FindEmptySpriteSlot();
	if (sprite_slot == -1) {
		sprintf(engine.system_error_message1, "NPC_Load function error");
		sprintf(engine.system_error_message2, "No empty sprite slot available");
		sprintf(engine.system_error_message3, " ");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
	} else {
		npc[number].sprite_num = sprite_slot;
		GFX_InitSprite(ENTITY_ID_NPC, number, sprite_slot, 0, npc[number].cfg->width, npc[number].cfg->height);
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
	npc[number].colission_area.points[0][0] = gfx_sprite_stack[npc[number].sprite_num].width_px >> 4;
	npc[number].colission_area.points[0][1] = (gfx_sprite_stack[npc[number].sprite_num].height_px >> 1) - (gfx_sprite_stack[npc[number].sprite_num].height_px >> 4);
	// Point 2
	npc[number].colission_area.points[1][0] = gfx_sprite_stack[npc[number].sprite_num].width_px - (gfx_sprite_stack[npc[number].sprite_num].width_px >> 4);
	npc[number].colission_area.points[1][1] = (gfx_sprite_stack[npc[number].sprite_num].height_px >> 1) - (gfx_sprite_stack[npc[number].sprite_num].height_px >> 4);

	// Point 3
	npc[number].colission_area.points[2][0] = gfx_sprite_stack[npc[number].sprite_num].width_px >> 4;
	npc[number].colission_area.points[2][1] = gfx_sprite_stack[npc[number].sprite_num].height_px;
	// Point 4
	npc[number].colission_area.points[3][0] = gfx_sprite_stack[npc[number].sprite_num].width_px - (gfx_sprite_stack[npc[number].sprite_num].width_px >> 4);
	npc[number].colission_area.points[3][1] = gfx_sprite_stack[npc[number].sprite_num].height_px;

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
	npc[number].hit_area.points[0][0] = gfx_sprite_stack[npc[number].sprite_num].width_px >> 3;
	npc[number].hit_area.points[0][1] = (gfx_sprite_stack[npc[number].sprite_num].height_px >> 1) - (gfx_sprite_stack[npc[number].sprite_num].height_px >> 3);
	// Point 2
	npc[number].hit_area.points[1][0] = gfx_sprite_stack[npc[number].sprite_num].width_px - (gfx_sprite_stack[npc[number].sprite_num].width_px >> 3);
	npc[number].hit_area.points[1][1] = (gfx_sprite_stack[npc[number].sprite_num].height_px >> 1) - (gfx_sprite_stack[npc[number].sprite_num].height_px >> 3);
	// Point 3
	npc[number].hit_area.points[2][0] = gfx_sprite_stack[npc[number].sprite_num].width_px >> 3;
	npc[number].hit_area.points[2][1] = gfx_sprite_stack[npc[number].sprite_num].height_px - (gfx_sprite_stack[npc[number].sprite_num].height_px >> 4);
	// Point 4
	npc[number].hit_area.points[3][0] = gfx_sprite_stack[npc[number].sprite_num].width_px - (gfx_sprite_stack[npc[number].sprite_num].width_px >> 3);
	npc[number].hit_area.points[3][1] = gfx_sprite_stack[npc[number].sprite_num].height_px - (gfx_sprite_stack[npc[number].sprite_num].height_px >> 4);

	GFX_SetSpriteGraphic(npc[number].sprite_num, 0, sprite_gfx_id, 0, 0);           // main graphic
	GFX_SetSpriteGraphic(npc[number].sprite_num, 1, SPRITE_GRAPHICS_ID_EMPTY, 0, 0);// empty
	GFX_SetSpriteGraphic(npc[number].sprite_num, 2, SPRITE_GRAPHICS_ID_EMPTY, 0, 0);// empty
	GFX_SetSpriteGraphic(npc[number].sprite_num, 3, SPRITE_GRAPHICS_ID_EMPTY, 0, 0);// empty
	GFX_SetSpriteGraphic(npc[number].sprite_num, 4, SPRITE_GRAPHICS_ID_EMPTY, 0, 0);// empty

	gfx_sprite_stack[npc[number].sprite_num].screen_pos_x = npc[number].pos_x - camera->pos_x;
	gfx_sprite_stack[npc[number].sprite_num].screen_pos_y = npc[number].pos_y - camera->pos_y;

	// Patterns
	npc[number].pattern_step = 0;
	npc[number].current_pattern = NPC_PATTERN_HOLD_ON;

	// Set animation
	GFX_SetDefaultAnimation(npc[number].sprite_num, false, true, 50);
}

void NPC_DrawColissionPixels(NPC n) {
	//  Draw colission pixels
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[n.sprite_num].screen_pos_x + n.colission_area.points[0][0], gfx_sprite_stack[n.sprite_num].screen_pos_y + n.colission_area.points[0][1], 228);
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[n.sprite_num].screen_pos_x + n.colission_area.points[1][0], gfx_sprite_stack[n.sprite_num].screen_pos_y + n.colission_area.points[1][1], 228);
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[n.sprite_num].screen_pos_x + n.colission_area.points[2][0], gfx_sprite_stack[n.sprite_num].screen_pos_y + n.colission_area.points[2][1], 228);
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[n.sprite_num].screen_pos_x + n.colission_area.points[3][0], gfx_sprite_stack[n.sprite_num].screen_pos_y + n.colission_area.points[3][1], 228);
}

void NPC_DrawHitPixels(NPC n) {
	//  Draw colission pixels
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[n.sprite_num].screen_pos_x + n.hit_area.points[0][0], gfx_sprite_stack[n.sprite_num].screen_pos_y + n.hit_area.points[0][1], 229);
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[n.sprite_num].screen_pos_x + n.hit_area.points[1][0], gfx_sprite_stack[n.sprite_num].screen_pos_y + n.hit_area.points[1][1], 229);
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[n.sprite_num].screen_pos_x + n.hit_area.points[2][0], gfx_sprite_stack[n.sprite_num].screen_pos_y + n.hit_area.points[2][1], 229);
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[n.sprite_num].screen_pos_x + n.hit_area.points[3][0], gfx_sprite_stack[n.sprite_num].screen_pos_y + n.hit_area.points[3][1], 229);
}

void NPC_SetPosition(byte number, int x, int y) {
	if (npc[number].is_loaded) {
		npc[number].pos_x = x;
		npc[number].pos_y = y;
		npc[number].x_FP = x << FP;
		npc[number].y_FP = y << FP;
		npc[number].vx_FP = 0;
		npc[number].vy_FP = 0;
	}
}

void NPC_SetBehavior(byte number, int behavior) {
	if (npc[number].is_loaded) {
		npc[number].status_behavior = behavior;
	}
}

bool NPC_CheckUpColission(NPC n) {
	int i;
	int point1_x, point1_y;
	int point2_x, point2_y;

	point1_x = n.pos_x + n.colission_area.points[0][0];
	point1_y = n.pos_y + n.colission_area.points[0][1];
	point2_x = n.pos_x + n.colission_area.points[1][0];
	point2_y = n.pos_y + n.colission_area.points[1][1];

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

	/////// NPCS COLISSIONS ///////////
	for (i = 0; i < NPC_MAX_NPCS; i++) {
		if (npc[i].is_loaded & !npc[i].action_dead) {
			if (gfx_sprite_stack[npc[i].sprite_num].shown && (npc[i].sprite_num != n.sprite_num)) {
				//Point 1
				if (point1_x <= npc[i].pos_x + npc[i].colission_area.points[3][0]

					&& point1_x >= npc[i].pos_x + npc[i].colission_area.points[3][0]

					&& point1_y >= npc[i].pos_y + npc[i].colission_area.points[1][1]

					&& point1_y <= npc[i].pos_y + npc[i].colission_area.points[3][1]) {
					return true;
				}

				//Point 2
				if (point2_x <= npc[i].pos_x + npc[i].colission_area.points[3][0]

					&& point2_x >= npc[i].pos_x + npc[i].colission_area.points[2][0]

					&& point2_y >= npc[i].pos_y + npc[i].colission_area.points[1][1]

					&& point2_y <= npc[i].pos_y + npc[i].colission_area.points[3][1]) {
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

bool NPC_CheckDownColission(NPC n) {
	int i;

	int point1_x, point1_y;
	int point2_x, point2_y;

	point1_x = n.pos_x + n.colission_area.points[2][0];
	point1_y = n.pos_y + n.colission_area.points[2][1];
	point2_x = n.pos_x + n.colission_area.points[3][0];
	point2_y = n.pos_y + n.colission_area.points[3][1];

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

	/////// NPCS COLISSIONS ///////////
	for (i = 0; i < NPC_MAX_NPCS; i++) {
		if (npc[i].is_loaded & !npc[i].action_dead) {
			if (gfx_sprite_stack[npc[i].sprite_num].shown && (npc[i].sprite_num != n.sprite_num)) {
				//Point 1
				if (point1_x <= npc[i].pos_x + npc[i].colission_area.points[1][0]

					&& point1_x >= npc[i].pos_x + npc[i].colission_area.points[0][0]

					&& point1_y >= npc[i].pos_y + npc[i].colission_area.points[1][1]

					&& point1_y <= npc[i].pos_y + npc[i].colission_area.points[3][1]) {
					return true;
				}

				//Point 2
				if (point2_x <= npc[i].pos_x + npc[i].colission_area.points[1][0]

					&& point2_x >= npc[i].pos_x + npc[i].colission_area.points[0][0]

					&& point2_y >= npc[i].pos_y + npc[i].colission_area.points[1][1]

					&& point2_y <= npc[i].pos_y + npc[i].colission_area.points[3][1]) {
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

bool NPC_CheckLeftColission(NPC n) {
	int i;

	int point1_x, point1_y;
	int point2_x, point2_y;

	point1_x = n.pos_x + n.colission_area.points[0][0];
	point1_y = n.pos_y + n.colission_area.points[0][1];
	point2_x = n.pos_x + n.colission_area.points[2][0];
	point2_y = n.pos_y + n.colission_area.points[2][1];

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
	for (i = 0; i < NPC_MAX_NPCS; i++) {
		if (npc[i].is_loaded && !npc[i].action_dead) {
			if (gfx_sprite_stack[npc[i].sprite_num].shown && (npc[i].sprite_num != n.sprite_num)) {
				//Point 1
				if (point1_x <= npc[i].pos_x + npc[i].colission_area.points[1][0]

					&& point1_x >= npc[i].pos_x + npc[i].colission_area.points[0][0]

					&& point1_y >= npc[i].pos_y + npc[i].colission_area.points[0][1]

					&& point1_y <= npc[i].pos_y + npc[i].colission_area.points[3][1]) {
					return true;
				}

				//Point 2
				if (point2_x <= npc[i].pos_x + npc[i].colission_area.points[1][0]

					&& point2_x >= npc[i].pos_x + npc[i].colission_area.points[0][0]

					&& point2_y >= npc[i].pos_y + npc[i].colission_area.points[0][1]

					&& point2_y <= npc[i].pos_y + npc[i].colission_area.points[3][1]) {
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

bool NPC_CheckRightColission(NPC n) {
	int i;

	int point1_x, point1_y;
	int point2_x, point2_y;

	point1_x = n.pos_x + n.colission_area.points[1][0];
	point1_y = n.pos_y + n.colission_area.points[1][1];
	point2_x = n.pos_x + n.colission_area.points[3][0];
	point2_y = n.pos_y + n.colission_area.points[3][1];

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

	for (i = 0; i < NPC_MAX_NPCS; i++) {
		if (npc[i].is_loaded && !npc[i].action_dead) {
			if (gfx_sprite_stack[npc[i].sprite_num].shown && (npc[i].sprite_num != n.sprite_num)) {
				//Point 1
				if (point1_x <= npc[i].pos_x + npc[i].colission_area.points[1][0]

					&& point1_x >= npc[i].pos_x + npc[i].colission_area.points[0][0]

					&& point1_y >= npc[i].pos_y + npc[i].colission_area.points[0][1]

					&& point1_y <= npc[i].pos_y + npc[i].colission_area.points[2][1]) {
					return true;
				}

				//Point 2
				if (point2_x <= npc[i].pos_x + npc[i].colission_area.points[1][0]

					&& point2_x >= npc[i].pos_x + enemy[i].colission_area.points[0][0]

					&& point2_y >= npc[i].pos_y + npc[i].colission_area.points[0][1]

					&& point2_y <= npc[i].pos_y + npc[i].colission_area.points[2][1]) {
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

void NPC_UnloadNpc(int npc_number) {
	npc[npc_number].is_loaded = false;
	GFX_UnloadSprite(npc[npc_number].sprite_num);
	npc[npc_number].sprite_num = -1;
}

void NPC_UnloadNpcs(void) {
	int i;
	for (i = 0; i < NPC_MAX_NPCS; i++) {
		if (npc[i].is_loaded) {
			GFX_UnloadSprite(npc[i].sprite_num);
			npc[i].is_loaded = false;
			npc[i].sprite_num = -1;
		}
	}
}

void NPC_UpdateRangeStatus(int npc_number) {
	int i, dx, dy, distance, pos_x, pos_y;
	dword vx_FP, vy_FP, x_FP, y_FP;
	if (gfx_sprite_stack[npc[npc_number].sprite_num].shown) {
		dx = (actor->pos_x + actor->width_px) - (npc[npc_number].pos_x + npc[npc_number].width_px);
		dy = (actor->pos_y + actor->height_px) - (npc[npc_number].pos_y + npc[npc_number].height_px);

		// calculate steps
		if (abs(dx) > abs(dy))
			distance = abs(dx);
		else
			distance = abs(dy);

		// Avoid 0 division or die!!
		if (distance == 0) distance = 1;

		// Check if is in shoot range
		if (distance < npc[npc_number].range) npc[npc_number].in_range = true;
		else
			npc[npc_number].in_range = false;

		// calculate fixed point velocity and position
		vx_FP = (dx << FP) / distance;
		vy_FP = (dy << FP) / distance;

		x_FP = (npc[npc_number].pos_x + npc[npc_number].width_px) << FP;
		y_FP = (npc[npc_number].pos_y + npc[npc_number].height_px) << FP;

		// Check if can see actor
		npc[npc_number].can_see_actor = true;
		for (i = 0; i < distance; i++) {
			x_FP += vx_FP;
			y_FP += vy_FP;

			pos_x = (x_FP >> FP);
			pos_y = (y_FP >> FP);

			if (MAP_CheckColissionTile(pos_x, pos_y) == 1) npc[npc_number].can_see_actor = false;
		}
	} else {
		npc[npc_number].can_see_actor = false;
		npc[npc_number].in_range = false;
	}
}

void NPC_RunAway(int npc_number, int pos_x, int pos_y, byte *movement) {
	int dx, dy, angle;

	dx = npc[npc_number].pos_x - pos_x;
	dy = npc[npc_number].pos_y - pos_y;
	angle = UTIL_GetAngle(dx, dy);
	*movement = 0;
	if ((angle > 280) || (angle < 80)) *movement = *movement | 1;        // moving right
	if ((angle > 100) && (angle < 260)) *movement = *movement | (1 << 1);// moving left
	if ((angle > 10) && (angle < 170)) *movement = *movement | (1 << 2); // moving down
	if ((angle > 190) && (angle < 350)) *movement = *movement | (1 << 3);// moving up
}

void NPC_LookAt(int npc_number, int pos_x, int pos_y, int *facing) {
	int dx, dy, angle;

	dx = pos_x - npc[npc_number].pos_x;
	dy = pos_y - npc[npc_number].pos_y;
	angle = UTIL_GetAngle(dx, dy);
	*facing = 0;
	if ((angle > 280) || (angle < 80)) *facing = *facing | 1;        // facing right
	if ((angle > 100) && (angle < 260)) *facing = *facing | (1 << 1);// facing left
	if ((angle > 10) && (angle < 170)) *facing = *facing | (1 << 2); // facing down
	if ((angle > 190) && (angle < 350)) *facing = *facing | (1 << 3);// facing up
}

void NPC_SetRange(int npc_number, int range) {
	npc[npc_number].range = range;
}

void NPC_Update(void) {
	bool status_idle;
	int i, j, update_npc;
	unsigned char chr = '!';
	int movement;

	npc_update_counter++;
	if (npc_update_counter > 6) {
		npc_update_counter = 0;
	}

	npc_counter = 0;
	// Calculate facing direction with npc and actor positions
	for (i = 0; i < NPC_MAX_NPCS; i++) {
		if (npc[i].is_loaded) {
			npc_counter++;

			// Update npc position on screen
			GFX_SetSpritePosition(npc[i].sprite_num, npc[i].pos_x - camera->pos_x, npc[i].pos_y - camera->pos_y);

			// On screen
			npc[i].on_screen = GFX_IsSpriteOnScreen(npc[i].sprite_num);

			// DEBUG: Draw collision and hit pixels
			//if (npc[i].on_screen) NPC_DrawColissionPixels(npc[i]);
			//if (npc[i].on_screen) NPC_DrawHitPixels(npc[i]);

			// Avoid to update enemies each cycle
			update_npc = (i + npc_update_counter) & 1;
			if (update_npc) {
				// Update range status
				// - updates npc.can_see_actor
				// - updates npc.in_range
				NPC_UpdateRangeStatus(i);

				// Set current behavior and pattern
				switch (npc[i].status_behavior) {
					case NPC_STATUS_IDLE:
						if (npc[i].can_see_actor) {
							if (npc[i].life < 10) {
								npc[i].status_behavior = NPC_STATUS_AVOID;
								npc[i].pattern_step = 0;
							}
						}
						break;
					case NPC_STATUS_CHASE:
						if (npc[i].in_range) {
							npc[i].status_behavior = NPC_STATUS_CHASE_IDLE;
							npc[i].pattern_step = 0;
						}
						break;
					case NPC_STATUS_CHASE_IDLE:
						if (!npc[i].in_range) {
							npc[i].status_behavior = NPC_STATUS_CHASE;
							npc[i].pattern_step = 0;
						}
						break;
					case NPC_STATUS_AVOID:
						if (!npc[i].in_range && npc[i].life < 10) {
							npc[i].status_behavior = NPC_STATUS_HIDE;
							npc[i].pattern_step = 0;
						}
						break;
					case NPC_STATUS_HIDE:
						if (!npc[i].can_see_actor) {
							npc[i].status_behavior = NPC_STATUS_IDLE;
							npc[i].pattern_step = 0;
						}
						if (npc[i].in_range) {
							npc[i].status_behavior = NPC_STATUS_AVOID;
							npc[i].pattern_step = 0;
						}
						break;
					case NPC_STATUS_WALKING_ARROUND:
					case NPC_STATUS_WALKING_HORIZONTAL:
					case NPC_STATUS_WALKING_VERTICAL:
					case NPC_STATUS_WALKING_RANDOM:
						if (npc[i].is_hit) {
							npc[i].status_behavior = NPC_STATUS_AVOID;
							npc[i].pattern_step = 0;
						}
						break;
					default:
						break;
				}

				// Check idle status
				status_idle = !npc[i].action_stand & !npc[i].action_walk & !npc[i].action_dead & !npc[i].action_hit;

				//// ---------------- INITIALIZE ACTION ----------------

				if (status_idle) {
					npc[i].pattern_step++;
					if (npc[i].pattern_step > 5) npc[i].pattern_step = 0;

					// Set action
					switch (npc[i].status_behavior) {
						case NPC_STATUS_IDLE:
							npc[i].current_pattern = hold_on_pattern[npc[i].pattern_step];
							break;
						case NPC_STATUS_CHASE:
							npc[i].current_pattern = chase_pattern[npc[i].pattern_step];
							break;
						case NPC_STATUS_CHASE_IDLE:
							npc[i].current_pattern = hold_on_pattern[npc[i].pattern_step];
							break;
						case NPC_STATUS_AVOID:
							npc[i].current_pattern = avoid_pattern[npc[i].pattern_step];
							break;
						case NPC_STATUS_HIDE:
							npc[i].current_pattern = hide_pattern[npc[i].pattern_step];
							break;
						case NPC_STATUS_STATIC:
							npc[i].current_pattern = hold_on_pattern[npc[i].pattern_step];
							break;
						case NPC_STATUS_WALKING_ARROUND:
							npc[i].current_pattern = walk_arround_pattern[npc[i].pattern_step];
							break;
						case NPC_STATUS_WALKING_HORIZONTAL:
							npc[i].current_pattern = walk_horizontal_pattern[npc[i].pattern_step];
							break;
						case NPC_STATUS_WALKING_VERTICAL:
							npc[i].current_pattern = walk_vertical_pattern[npc[i].pattern_step];
							break;
						case NPC_STATUS_WALKING_RANDOM:
							npc[i].current_pattern = walk_random_pattern[npc[i].pattern_step];
							break;
						default:
							npc[i].current_pattern = hold_on_pattern[npc[i].pattern_step];
							break;
					}

					switch (npc[i].current_pattern) {
						case NPC_PATTERN_HOLD_ON:
							status_idle = false;
							npc[i].action_stand = true;
							npc[i].action_walk = false;

							npc[i].movement = 0;
							npc[i].action_step = 0;
							npc[i].last_action = NPC_ACTION_STAND;
							break;
						case NPC_PATTERN_WALK_BACK:
							status_idle = false;
							npc[i].action_walk = true;
							npc[i].action_stand = false;

							NPC_RunAway(i, actor->pos_x, actor->pos_y, &npc[i].movement);
							npc[i].status_facing = npc[i].movement;
							npc[i].action_step = 0;
							npc[i].last_action = NPC_ACTION_WALK;
							break;
						case NPC_PATTERN_WALK_FORWARD:
							status_idle = false;
							npc[i].action_walk = true;
							npc[i].action_stand = false;

							NPC_LookAt(i, actor->pos_x, actor->pos_y, &npc[i].status_facing);
							npc[i].movement = npc[i].status_facing;
							npc[i].action_step = 0;
							npc[i].last_action = NPC_ACTION_WALK;
							break;
						case NPC_PATTERN_WALK_ARROUND:
							status_idle = false;
							npc[i].action_walk = true;
							npc[i].action_stand = false;

							NPC_LookAt(i, actor->pos_x + rand() % 0xF, actor->pos_y + rand() % 0xF, &npc[i].status_facing);
							npc[i].movement = npc[i].status_facing;
							npc[i].action_step = 0;
							npc[i].last_action = NPC_ACTION_WALK;
							break;
						case NPC_PATTERN_WALK_UP:
							status_idle = false;
							npc[i].action_walk = true;
							npc[i].action_stand = false;

							npc[i].movement = NPC_FACING_UP;
							npc[i].action_step = 0;
							npc[i].last_action = NPC_ACTION_WALK;
							break;
						case NPC_PATTERN_WALK_RIGHT:
							status_idle = false;
							npc[i].action_walk = true;
							npc[i].action_stand = false;

							npc[i].movement = NPC_FACING_RIGHT;
							npc[i].action_step = 0;
							npc[i].last_action = NPC_ACTION_WALK;
							break;
						case NPC_PATTERN_WALK_DOWN:
							status_idle = false;
							npc[i].action_walk = true;
							npc[i].action_stand = false;

							npc[i].movement = NPC_FACING_DOWN;
							npc[i].action_step = 0;
							npc[i].last_action = NPC_ACTION_WALK;
							break;
						case NPC_PATTERN_WALK_LEFT:
							status_idle = false;
							npc[i].action_walk = true;
							npc[i].action_stand = false;

							npc[i].movement = NPC_FACING_LEFT;
							npc[i].action_step = 0;
							npc[i].last_action = NPC_ACTION_WALK;
							break;
						case NPC_PATTERN_WALK_RANDOM:
							status_idle = false;
							npc[i].action_walk = true;
							npc[i].action_stand = false;
							movement = rand() % 8;
							npc[i].movement = facing_array[movement];
							npc[i].action_step = 0;
							npc[i].last_action = NPC_ACTION_WALK;
							break;
						default:
							break;
					}
				}

				//// ---------------- ACTION ANIMATION ----------------

				// Walking and animation
				if (npc[i].action_walk) {
					switch (npc[i].movement) {
						case 1:// moving right
							npc[i].x_FP += npc[i].speed_horizontal_FP;
							npc[i].pos_x = npc[i].x_FP >> FP;
							if (NPC_CheckRightColission(npc[i])) {
								npc[i].x_FP -= npc[i].speed_horizontal_FP;
								npc[i].pos_x = npc[i].x_FP >> FP;
								// Change direction >> down
								if (!NPC_CheckUpColission(npc[i])) {
									npc[i].movement = NPC_MOVING_UP;
								}
								// Change direction >> up
								if (!NPC_CheckDownColission(npc[i])) {
									npc[i].movement = NPC_MOVING_DOWN;
								}
							}
							break;
						case 2:// moving left
							npc[i].x_FP -= npc[i].speed_horizontal_FP;
							npc[i].pos_x = npc[i].x_FP >> FP;
							if (NPC_CheckLeftColission(npc[i])) {
								npc[i].x_FP += npc[i].speed_horizontal_FP;
								npc[i].pos_x = npc[i].x_FP >> FP;
								// Change direction >> down
								if (!NPC_CheckUpColission(npc[i])) {
									npc[i].movement = NPC_MOVING_UP;
								}
								// Change direction >> up
								if (!NPC_CheckDownColission(npc[i])) {
									npc[i].movement = NPC_MOVING_DOWN;
								}
							}
							break;
						case 4:// moving down
							npc[i].y_FP += npc[i].speed_vertical_FP;
							npc[i].pos_y = npc[i].y_FP >> FP;
							if (NPC_CheckDownColission(npc[i])) {
								npc[i].y_FP -= npc[i].speed_vertical_FP;
								npc[i].pos_y = npc[i].y_FP >> FP;
								// Change direction >> left
								if (!NPC_CheckLeftColission(npc[i])) {
									npc[i].movement = NPC_MOVING_LEFT;
								}
								// Change direction >> up
								if (!NPC_CheckRightColission(npc[i])) {
									npc[i].movement = NPC_MOVING_RIGHT;
								}
							}
							break;
						case 5:// moving down-right
							npc[i].x_FP += npc[i].speed_diagonalh_FP;
							npc[i].y_FP += npc[i].speed_diagonalv_FP;
							npc[i].pos_x = npc[i].x_FP >> FP;
							npc[i].pos_y = npc[i].y_FP >> FP;
							if (NPC_CheckDownColission(npc[i])) {
								npc[i].y_FP -= npc[i].speed_diagonalv_FP;
								npc[i].pos_y = npc[i].y_FP >> FP;
								// Change direction >> left
								if (!NPC_CheckLeftColission(npc[i])) {
									npc[i].movement = NPC_MOVING_LEFT;
								}
								// Change direction >> right
								if (!NPC_CheckRightColission(npc[i])) {
									npc[i].movement = NPC_MOVING_RIGHT;
								}
							}
							if (NPC_CheckRightColission(npc[i])) {
								npc[i].x_FP -= npc[i].speed_diagonalh_FP;
								npc[i].pos_x = npc[i].x_FP >> FP;
								// Change direction >> up
								if (!NPC_CheckUpColission(npc[i])) {
									npc[i].movement = NPC_MOVING_UP;
								}
								// Change direction >> down
								if (!NPC_CheckDownColission(npc[i])) {
									npc[i].movement = NPC_MOVING_DOWN;
								}
							}

							break;
						case 6:// moving down-left
							npc[i].x_FP -= npc[i].speed_diagonalh_FP;
							npc[i].y_FP += npc[i].speed_diagonalv_FP;
							npc[i].pos_x = npc[i].x_FP >> FP;
							npc[i].pos_y = npc[i].y_FP >> FP;
							if (NPC_CheckDownColission(npc[i])) {
								npc[i].y_FP -= npc[i].speed_diagonalv_FP;
								npc[i].pos_y = npc[i].y_FP >> FP;
								// Change direction >> left
								if (!NPC_CheckLeftColission(npc[i])) {
									npc[i].movement = NPC_MOVING_LEFT;
								}
								// Change direction >> right
								if (!NPC_CheckRightColission(npc[i])) {
									npc[i].movement = NPC_MOVING_RIGHT;
								}
							}
							if (NPC_CheckLeftColission(npc[i])) {
								npc[i].x_FP += npc[i].speed_diagonalh_FP;
								npc[i].pos_x = npc[i].x_FP >> FP;
								// Change direction >> up
								if (!NPC_CheckUpColission(npc[i])) {
									npc[i].movement = NPC_MOVING_UP;
								}
								// Change direction >> down
								if (!NPC_CheckDownColission(npc[i])) {
									npc[i].movement = NPC_MOVING_DOWN;
								}
							}
							break;
						case 8:// moving up
							npc[i].y_FP -= npc[i].speed_vertical_FP;
							npc[i].pos_y = npc[i].y_FP >> FP;
							if (NPC_CheckUpColission(npc[i])) {
								npc[i].y_FP += npc[i].speed_vertical_FP;
								npc[i].pos_y = npc[i].y_FP >> FP;
								// Change direction >> left
								if (!NPC_CheckLeftColission(npc[i])) {
									npc[i].movement = NPC_MOVING_LEFT;
								}
								// Change direction >> right
								if (!NPC_CheckRightColission(npc[i])) {
									npc[i].movement = NPC_MOVING_RIGHT;
								}
							}
							break;
						case 9:// moving up-right
							npc[i].x_FP += npc[i].speed_diagonalh_FP;
							npc[i].y_FP -= npc[i].speed_diagonalv_FP;
							npc[i].pos_x = npc[i].x_FP >> FP;
							npc[i].pos_y = npc[i].y_FP >> FP;
							if (NPC_CheckUpColission(npc[i])) {
								npc[i].y_FP += npc[i].speed_diagonalv_FP;
								npc[i].pos_y = npc[i].y_FP >> FP;
								// Change direction >> left
								if (!NPC_CheckLeftColission(npc[i])) {
									npc[i].movement = NPC_MOVING_LEFT;
								}
								// Change direction >> right
								if (!NPC_CheckRightColission(npc[i])) {
									npc[i].movement = NPC_MOVING_RIGHT;
								}
							}
							if (NPC_CheckRightColission(npc[i])) {
								npc[i].x_FP -= npc[i].speed_diagonalh_FP;
								npc[i].pos_x = npc[i].x_FP >> FP;
								// Change direction >> down
								if (!NPC_CheckDownColission(npc[i])) {
									npc[i].movement = NPC_MOVING_DOWN;
								}
								// Change direction >> up
								if (!NPC_CheckUpColission(npc[i])) {
									npc[i].movement = NPC_MOVING_UP;
								}
							}
							break;
						case 10:// moving up-left
							npc[i].x_FP -= npc[i].speed_diagonalh_FP;
							npc[i].y_FP -= npc[i].speed_diagonalv_FP;
							npc[i].pos_x = npc[i].x_FP >> FP;
							npc[i].pos_y = npc[i].y_FP >> FP;
							if (NPC_CheckUpColission(npc[i])) {
								npc[i].y_FP += npc[i].speed_diagonalv_FP;
								npc[i].pos_y = npc[i].y_FP >> FP;
								// Change direction >> right
								if (!NPC_CheckRightColission(npc[i])) {
									npc[i].movement = NPC_MOVING_RIGHT;
								}
								// Change direction >> left
								if (!NPC_CheckLeftColission(npc[i])) {
									npc[i].movement = NPC_MOVING_LEFT;
								}
							}
							if (NPC_CheckLeftColission(npc[i])) {
								npc[i].x_FP += npc[i].speed_diagonalh_FP;
								npc[i].pos_x = npc[i].x_FP >> FP;
								// Change direction >> down
								if (!NPC_CheckDownColission(npc[i])) {
									npc[i].movement = NPC_MOVING_DOWN;
								}
								// Change direction >> up
								if (!NPC_CheckUpColission(npc[i])) {
									npc[i].movement = NPC_MOVING_UP;
								}
							}
							break;
						default:
							break;
					}

					npc[i].idle_counter++;
					if (npc[i].idle_counter > NPC_IDLE_TICKS) {
						npc[i].action_walk = false;
						npc[i].idle_counter = 0;
					}
				}

				// Stand and animation
				if (npc[i].action_stand) {
					npc[i].idle_counter++;
					if (npc[i].idle_counter > NPC_IDLE_TICKS) {
						npc[i].action_stand = false;
						npc[i].idle_counter = 0;
					}
				}

				// Check hit by something
				//  - sets "is_hit" property
				if (!npc[i].action_dead) {// avoid double hit when is already hit

					// Check for particles
					for (j = 0; j < PARTICLE_MAX_PARTICLES; j++) {
						if (particle[j].loaded) {
							if (particle[j].pos_x + particle[j].colission_area.points[0][0] < npc[i].pos_x + npc[i].hit_area.points[1][0]

								&& particle[j].pos_x + particle[j].colission_area.points[1][0] > npc[i].pos_x + npc[i].hit_area.points[0][0]

								&& particle[j].pos_y + particle[j].colission_area.points[0][1] < npc[i].pos_y + npc[i].hit_area.points[2][1]

								&& particle[j].pos_y + particle[j].colission_area.points[2][1] > npc[i].pos_y + npc[i].hit_area.points[0][1]) {
								npc[i].hit_by = gfx_sprite_stack[particle[j].sprite_num].id;
								npc[i].is_hit = true;
								npc[i].damage += particle[j].damage;

								npc[i].hit_vx_FP = particle[j].vx_FP;
								npc[i].hit_vy_FP = particle[j].vy_FP;
							}
						}
					}

					// Check for bullets
					for (j = 0; j < BULLET_MAX_BULLETS; j++) {
						if (bullet[j].loaded) {
							switch (gfx_sprite_stack[bullet[j].sprite_num].entity_id) {
								case ENTITY_ID_ENEMY_BULLET:
								case ENTITY_ID_ACTOR_BULLET:
								case ENTITY_ID_ACTOR_PUNCH:
								case ENTITY_ID_ACTOR_KICK:
									if (bullet[j].pos_x + bullet[j].colission_area.points[0][0] < npc[i].pos_x + npc[i].hit_area.points[1][0]

										&& bullet[j].pos_x + bullet[j].colission_area.points[1][0] > npc[i].pos_x + npc[i].hit_area.points[0][0]

										&& bullet[j].pos_y + bullet[j].colission_area.points[0][1] < npc[i].pos_y + npc[i].hit_area.points[2][1]

										&& bullet[j].pos_y + bullet[j].colission_area.points[2][1] > npc[i].pos_y + npc[i].hit_area.points[0][1]) {
										npc[i].hit_by = gfx_sprite_stack[bullet[j].sprite_num].id;
										npc[i].is_hit = true;
										npc[i].damage += bullet[j].damage;

										npc[i].hit_vx_FP = bullet[j].vx_FP;
										npc[i].hit_vy_FP = bullet[j].vy_FP;
									}
									break;
							}
						}
					}
				}

				// Checks if is hit but still alive
				// - sets action hit or dead
				if (npc[i].is_hit && !npc[i].action_dead) {
					npc[i].is_hit = false;
					status_idle = false;
					// Cancel any action
					npc[i].action_walk = false;
					npc[i].action_stand = false;
					npc[i].action_hit = false;
					npc[i].action_dead = false;

					if (npc[i].damage >= npc[i].life) {
						npc[i].life = 0;
						npc[i].damage = 0;
						npc[i].action_step = 0;
						npc[i].action_dead = true;
						npc[i].action_hit = false;

					} else {
						npc[i].life -= npc[i].damage;
						npc[i].damage = 0;
						if (!npc[i].action_hit) {
							npc[i].action_step = 0;
							npc[i].action_hit = true;
						}
					}

					EFFECT_LoadEffect(ENTITY_ID_BLOOD, SPRITE_GRAPHICS_ID_BLOOD, npc[i].pos_x + (rand() % 32), npc[i].pos_y + (rand() % 32), true, 0, false, false, 5);
				}

				// Check if is hit
				if (npc[i].action_hit) {
					npc[i].action_hit = false;
					switch (npc[i].action_step) {
						case 0:
							npc[i].action_step++;
							break;
						case 1:
							EFFECT_LoadEffect(ENTITY_ID_BLOOD, SPRITE_GRAPHICS_ID_BLOOD, npc[i].pos_x + (rand() % 32), npc[i].pos_y + (rand() % 32), true, 0, false, false, 5);
							EFFECT_LoadEffect(ENTITY_ID_BLOOD, SPRITE_GRAPHICS_ID_BLOOD, npc[i].pos_x + (rand() % 32), npc[i].pos_y + (rand() % 32), true, 0, false, false, 5);
							npc[i].action_step++;
							break;
						case 2:// Move back
							npc[i].x_FP += npc[i].hit_vx_FP;
							npc[i].y_FP += npc[i].hit_vy_FP;
							npc[i].pos_x = npc[i].x_FP >> FP;
							npc[i].pos_y = npc[i].y_FP >> FP;
							npc[i].action_step++;
							break;
						case 3:// finish action
							npc[i].action_hit = false;
							break;
						default:
							break;
					}
				}

				// Check if is dead
				if (npc[i].action_dead) {

					switch (npc[i].action_step) {
						case 0:// Set death animation
							npc[i].life = 0;
							npc[i].movement = 0;
							npc[i].action_step++;
							break;
						case 1:
							EFFECT_LoadEffect(ENTITY_ID_BLOOD, SPRITE_GRAPHICS_ID_BLOOD, npc[i].pos_x + (rand() % 32), npc[i].pos_y + (rand() % 32), true, 0, false, false, 5);
							npc[i].x_FP += npc[i].hit_vx_FP;
							npc[i].y_FP += npc[i].hit_vy_FP;
							npc[i].pos_x = npc[i].x_FP >> FP;
							npc[i].pos_y = npc[i].y_FP >> FP;
							npc[i].action_step++;
							npc[i].idle_counter = 0;
							break;
						case 2:
							npc[i].idle_counter++;
							if (npc[i].idle_counter > NPC_IDLE_TICKS) {
								npc[i].action_step++;
								npc[i].idle_counter = 0;
								GFX_SetSpriteBlinkingProperties(npc[i].sprite_num, true, 5, SPRITE_TRANSP_COLOR);
							}
							break;
						case 3:// Unload npc
							npc[i].idle_counter++;
							if (npc[i].idle_counter > NPC_IDLE_TICKS) {
								npc[i].action_step++;
								npc[i].idle_counter = 0;
							}
							break;
						case 4:
							NPC_UnloadNpc(i);
							npc[i].is_loaded = false;
							break;
						default:
							break;
					}
				}
			}
		}
	}
}