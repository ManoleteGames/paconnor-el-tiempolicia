#include "../engine.h"
#include "misile.h"

Misile *misile;

static byte misile_update_counter;
byte misile_counter;


void MISILE_Init(void) {
	misile = MM_PushChunk(sizeof(Misile) * MISILE_MAX_MISILES, CT_ENGINE);
}

/** MISILE :: Draw colission pixels
 *  - Draws colission pixels just for debug
 */
void MISILE_DrawColissionPixels(Misile m) {
	//  Draw colission pixels
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[m.sprite_num].screen_pos_x + m.colission_area.points[0][0], gfx_sprite_stack[m.sprite_num].screen_pos_y + m.colission_area.points[0][1], 229);
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[m.sprite_num].screen_pos_x + m.colission_area.points[1][0], gfx_sprite_stack[m.sprite_num].screen_pos_y + m.colission_area.points[1][1], 229);
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[m.sprite_num].screen_pos_x + m.colission_area.points[2][0], gfx_sprite_stack[m.sprite_num].screen_pos_y + m.colission_area.points[2][1], 229);
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[m.sprite_num].screen_pos_x + m.colission_area.points[3][0], gfx_sprite_stack[m.sprite_num].screen_pos_y + m.colission_area.points[3][1], 229);
}


/** MISILE :: Load misile
 */
int MISILE_LoadMisile(int graphics_id, int shadow_graphic_id, int entity_id, int pos_x, int pos_y, int target_x, int target_y, int speed, int damage, int max_time, int max_distance) {
	int i, sprite_slot, number;
	int dx, dy;
	int distance;

	// Search free misile slot
	number = 0;

	for (i = 0; i < MISILE_MAX_MISILES; i++) {
		if (!misile[i].loaded) {
			number = i;
			break;
		}
	}

	// Check if misile is already loaded
	if (misile[number].loaded) {
		Error("MISILE_InitMisile function error", "Misile already loaded", "", ERROR_SYSTEM);
	}

	// Check if max grenades is reached
	if (number >= MISILE_MAX_MISILES - 1) {
		Error("MISILE_InitMisile function error", "Max number of misiles", "", ERROR_SYSTEM);
	}

	misile[number].current_time_ms = TIMER_GetMilliseconds();
	misile[number].end_time_ms = misile[number].current_time_ms + (max_time * 1000);
	misile[number].current_time = max_time;
	misile[number].pos_x = pos_x;
	misile[number].pos_y = pos_y;
	misile[number].pos_z = 1;
	misile[number].speed = speed;
	misile[number].damage = damage;
	misile[number].graphics_id = graphics_id;
	misile[number].shadow_graphic_id = shadow_graphic_id;
	misile[number].current_step = 0;
	misile[number].hit_on = 0;

	// calculate fixed point velocity and position
	dx = 0;
	dy = 1;
	misile[number].vx_FP = (dx << FP);
	misile[number].vy_FP = (dy << FP);
	misile[number].x_FP = misile[number].pos_x << FP;
	misile[number].y_FP = misile[number].pos_y << FP;
	misile[number].steps = max_distance;
	misile[number].facing = MISILE_FACING_DOWN;

	sprite_slot = GFX_FindEmptySpriteSlot();
	if (sprite_slot == -1) {
		sprintf(engine.system_error_message1, "MISILE_InitMisile function error");
		sprintf(engine.system_error_message2, "No empty sprite slot available");
		sprintf(engine.system_error_message3, " ");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
	} else {
		misile[number].sprite_num = sprite_slot;
		GFX_InitSprite(entity_id, number, sprite_slot, 16, 4, 4);
	}

	// Set main graphics
	GFX_SetSpriteGraphic(misile[number].sprite_num, 0, graphics_id, 0, 0);
	GFX_SetSpriteGraphic(misile[number].sprite_num, 1, shadow_graphic_id, 0, 0);
	GFX_SetSingleFrameAnimation(misile[i].sprite_num, 4, false);

	misile[number].width_px = GFX_GetSpriteWidth_px(misile[number].sprite_num);
	misile[number].height_px = GFX_GetSpriteHeight_px(misile[number].sprite_num);

	// Set colission points
	//  ---------------
	// |               |
	// |  0         1  |
	// |               |
	// |               |
	// |  2         3  |
	//  ---------------
	// Point 0
	misile[number].colission_area.points[0][0] = -8;
	misile[number].colission_area.points[0][1] = -8;
	// Point 1
	misile[number].colission_area.points[1][0] = misile[number].width_px + 8;
	misile[number].colission_area.points[1][1] = -8;
	// Point 2
	misile[number].colission_area.points[2][0] = -8;
	misile[number].colission_area.points[2][1] = misile[number].height_px + 8;
	// Point 3
	misile[number].colission_area.points[3][0] = misile[number].width_px + 8;
	misile[number].colission_area.points[3][1] = misile[number].height_px + 8;

	misile[number].on_target = false;
	misile[number].loaded = true;
	misile[number].shown = true;

	return number;
}


int MISILE_CheckMisileColission(int number) {
	int i;
	int point1_x, point1_y;
	int point2_x, point2_y;
	int point3_x, point3_y;
	int point4_x, point4_y;
	int tile_type;
	bool collision_detected;

	point1_x = misile[number].pos_x + misile[number].colission_area.points[0][0];
	point1_y = misile[number].pos_y + misile[number].colission_area.points[0][1];
	point2_x = misile[number].pos_x + misile[number].colission_area.points[1][0];
	point2_y = misile[number].pos_y + misile[number].colission_area.points[1][1];
	point3_x = misile[number].pos_x + misile[number].colission_area.points[2][0];
	point3_y = misile[number].pos_y + misile[number].colission_area.points[2][1];
	point4_x = misile[number].pos_x + misile[number].colission_area.points[3][0];
	point4_y = misile[number].pos_y + misile[number].colission_area.points[3][1];

	/////// OBJECTS COLISSIONS ///////////
	for (i = 0; i < OBJECT_MAX_OBJECTS; i++) {
		collision_detected = true;
		if (object[i].is_loaded) {
			if (gfx_sprite_stack[object[i].num_sprite].shown) {
				if (point1_x > object[i].pos_x + object[i].hit_area.points[1][0]) collision_detected = false;// Out of left side
				if (point2_x < object[i].pos_x + object[i].hit_area.points[0][0]) collision_detected = false;//  Out of right side
				if (point3_y < object[i].pos_y + object[i].hit_area.points[0][1]) collision_detected = false;//  Out of top side
				if (point1_y > object[i].pos_y + object[i].hit_area.points[2][1]) collision_detected = false;//  Out of bottom side
				if (collision_detected) return ((i << 8) | object[i].type);
			}
		}
	}

	/////// ENEMIES COLISSIONS ///////////
	for (i = 0; i < ENEMY_MAX_ENEMIES; i++) {
		collision_detected = true;
		if (enemy[i].is_loaded && !enemy[i].action_dead && gfx_sprite_stack[bullet[number].sprite_num].entity_id != ENTITY_ID_ENEMY_BULLET) {
			if (gfx_sprite_stack[enemy[i].sprite_num].shown) {
				if (point1_x > enemy[i].pos_x + enemy[i].hit_area.points[1][0]) collision_detected = false;// Out of left side
				if (point2_x < enemy[i].pos_x + enemy[i].hit_area.points[0][0]) collision_detected = false;//  Out of right side
				if (point3_y < enemy[i].pos_y + enemy[i].hit_area.points[0][1]) collision_detected = false;//  Out of top side
				if (point1_y > enemy[i].pos_y + enemy[i].hit_area.points[2][1]) collision_detected = false;//  Out of bottom side
				if (collision_detected) return gfx_sprite_stack[enemy[i].sprite_num].id;
			}
		}
	}

	/////// ACTOR COLISSIONS /////////////
	collision_detected = true;
	if (!actor->action_dead && gfx_sprite_stack[bullet[number].sprite_num].entity_id != ENTITY_ID_ACTOR_BULLET) {
		if (point1_x > actor->pos_x + actor->hit_area.points[1][0]) collision_detected = false;// Out of left side
		if (point2_x < actor->pos_x + actor->hit_area.points[0][0]) collision_detected = false;//  Out of right side
		if (point3_y < actor->pos_y + actor->hit_area.points[0][1]) collision_detected = false;//  Out of top side
		if (point1_y > actor->pos_y + actor->hit_area.points[2][1]) collision_detected = false;//  Out of bottom side
		if (collision_detected) return gfx_sprite_stack[actor->sprite_num].id;
	}


	/////// BACKGROUND COLISSIONS ///////////
	tile_type = MAP_CheckColissionTile(point1_x, point1_y);// Check point 1
	if (tile_type != 0) {
		return ((tile_type & 0xFF) << 8) | ENTITY_ID_BACKGROUND;
	}
	tile_type = MAP_CheckColissionTile(point2_x, point2_y);// Check point 2
	if (tile_type != 0) {
		return ((tile_type & 0xFF) << 8) | ENTITY_ID_BACKGROUND;
	}
	tile_type = MAP_CheckColissionTile(point3_x, point3_y);// Check point 3
	if (tile_type != 0) {
		return ((tile_type & 0xFF) << 8) | ENTITY_ID_BACKGROUND;
	}
	tile_type = MAP_CheckColissionTile(point4_x, point4_y);// Check point 4
	if (tile_type != 0) {
		return ((tile_type & 0xFF) << 8) | ENTITY_ID_BACKGROUND;
	}

	return false;
}

void MISILE_UnloadMisiles(void) {
	int i;
	for (i = 0; i < MISILE_MAX_MISILES; i++) {
		if (misile[i].loaded) {
			GFX_UnloadSprite(misile[i].sprite_num);
			misile[i].loaded = false;
			misile[i].sprite_num = -1;
		}
	}
}

void MISILE_UnloadMisile(int number) {
	if (misile[number].loaded) {
		GFX_UnloadSprite(misile[number].sprite_num);
		misile[number].loaded = false;
		misile[number].sprite_num = -1;
	}
}

void MISILE_LookAt(int number, int pos_x, int pos_y, int *facing) {
	int dx, dy, angle;

	dx = pos_x - misile[number].pos_x;
	dy = pos_y - misile[number].pos_y;
	angle = UTIL_GetAngle(dx, dy);
	*facing = 0;
	if ((angle > 290) || (angle < 70)) *facing = *facing | 1;        // facing right
	if ((angle > 120) && (angle < 260)) *facing = *facing | (1 << 1);// facing left
	if ((angle > 20) && (angle < 170)) *facing = *facing | (1 << 2); // facing down
	if ((angle > 200) && (angle < 320)) *facing = *facing | (1 << 3);// facing up
}

/** MISILE :: Update misiles
 */
void MISILE_Update(void) {
	int i, j;
	int update_misile;
	int tile_type;
	int dx, dy;
	int obj_number, enemy_number;

	misile_update_counter++;
	if (misile_update_counter > 6) {
		misile_update_counter = 0;
	}

	misile_counter = 0;
	for (i = 0; i < MISILE_MAX_MISILES; i++) {
		if (misile[i].loaded) {

			// debug
			//MISILE_DrawColissionPixels(misile[i]);

			// Avoid to update enemies each cycle
			update_misile = (i + misile_update_counter) & 1;
			if (update_misile) {

				misile[i].current_time_ms = TIMER_GetMilliseconds();
				misile[i].current_time = (misile[i].end_time_ms - misile[i].current_time_ms) / 1000;

				// Not on target
				if (!misile[i].on_target) {

					// Update direction
					misile[i].current_step++;
					if ((misile[i].current_step >= misile[i].steps)) {
						misile[i].current_step = 0;

						switch (misile[i].facing) {
							case MISILE_FACING_RIGHT:// facing right
								// stay right by default
								dx = 1;
								dy = 0;
								// Go down right
								if (actor->pos_y > misile[i].pos_y + misile[i].height_px) {
									dx = 1;
									dy = 1;
									misile[i].facing = MISILE_FACING_DOWN_RIGHT;
									GFX_SetSingleFrameAnimation(misile[i].sprite_num, 3, false);
								}
								// Go up right
								if (actor->pos_y + actor->height_px < misile[i].pos_y) {
									dx = 1;
									dy = -1;
									misile[i].facing = MISILE_FACING_UP_RIGHT;
									GFX_SetSingleFrameAnimation(misile[i].sprite_num, 1, false);
								}
								// Go up right
								if (actor->pos_x + actor->width_px < misile[i].pos_x) {
									dx = 1;
									dy = -1;
									misile[i].facing = MISILE_FACING_UP_RIGHT;
									GFX_SetSingleFrameAnimation(misile[i].sprite_num, 1, false);
								}
								break;
							case MISILE_FACING_LEFT:// facing left
								// stay left by default
								dx = -1;
								dy = 0;
								// Go down left
								if (actor->pos_y > misile[i].pos_y + misile[i].height_px) {
									dx = -1;
									dy = 1;
									misile[i].facing = MISILE_FACING_DOWN_LEFT;
									GFX_SetSingleFrameAnimation(misile[i].sprite_num, 5, false);
								}
								// Go up left
								if (actor->pos_y + actor->height_px < misile[i].pos_y) {
									dx = -1;
									dy = -1;
									misile[i].facing = MISILE_FACING_UP_LEFT;
									GFX_SetSingleFrameAnimation(misile[i].sprite_num, 7, false);
								}
								// Go up left
								if (actor->pos_x > misile[i].pos_x + misile[i].width_px) {
									dx = 1;
									dy = -1;
									misile[i].facing = MISILE_FACING_UP_LEFT;
									GFX_SetSingleFrameAnimation(misile[i].sprite_num, 7, false);
								}
								break;
							case MISILE_FACING_DOWN:// facing down
								// stay down by default
								dx = 0;
								dy = 1;
								// Go down right
								if (actor->pos_x > misile[i].pos_x + misile[i].width_px) {
									dx = 1;
									dy = 1;
									misile[i].facing = MISILE_FACING_DOWN_RIGHT;
									GFX_SetSingleFrameAnimation(misile[i].sprite_num, 3, false);
								}
								// Go down left
								if (actor->pos_x + actor->width_px < misile[i].pos_x) {
									dx = -1;
									dy = 1;
									misile[i].facing = MISILE_FACING_DOWN_LEFT;
									GFX_SetSingleFrameAnimation(misile[i].sprite_num, 5, false);
								}
								break;
							case MISILE_FACING_DOWN_RIGHT:// facing down-right
								// stay down-right by default
								dx = 1;
								dy = 1;
								// Go right
								if (actor->pos_x > misile[i].pos_x + misile[i].width_px) {
									dx = 1;
									dy = 0;
									misile[i].facing = MISILE_FACING_RIGHT;
									GFX_SetSingleFrameAnimation(misile[i].sprite_num, 2, false);
								}
								// Go down
								if (actor->pos_x < misile[i].pos_x - misile[i].width_px) {
									dx = 0;
									dy = 1;
									misile[i].facing = MISILE_FACING_DOWN;
									GFX_SetSingleFrameAnimation(misile[i].sprite_num, 4, true);
								}
								// Go right
								if (actor->pos_y + actor->height_px < misile[i].pos_y) {
									dx = 1;
									dy = 0;
									misile[i].facing = MISILE_FACING_RIGHT;
									GFX_SetSingleFrameAnimation(misile[i].sprite_num, 2, false);
								}
								break;
							case MISILE_FACING_DOWN_LEFT:// facing down-left
								// stay down-left by default
								dx = -1;
								dy = 1;
								// Go left
								if (actor->pos_x + actor->width_px < misile[i].pos_x) {
									dx = -1;
									dy = 0;
									misile[i].facing = MISILE_FACING_LEFT;
									GFX_SetSingleFrameAnimation(misile[i].sprite_num, 6, false);
								}
								// Go down
								if (actor->pos_x > misile[i].pos_x + misile[i].width_px) {
									dx = 0;
									dy = 1;
									misile[i].facing = MISILE_FACING_DOWN;
									GFX_SetSingleFrameAnimation(misile[i].sprite_num, 4, false);
								}
								// Go down
								if (actor->pos_y + actor->height_px > misile[i].pos_y) {
									dx = 0;
									dy = 1;
									misile[i].facing = MISILE_FACING_DOWN;
									GFX_SetSingleFrameAnimation(misile[i].sprite_num, 4, false);
								}
								break;
							case MISILE_FACING_UP:// facing up
								// stay up by default
								dx = 0;
								dy = -1;
								// Go up left
								if (actor->pos_x + actor->width_px < misile[i].pos_x) {
									dx = -1;
									dy = -1;
									misile[i].facing = MISILE_FACING_UP_LEFT;
									GFX_SetSingleFrameAnimation(misile[i].sprite_num, 7, false);
								}
								// Go up right
								if (actor->pos_x > misile[i].pos_x + misile[i].width_px) {
									dx = 1;
									dy = -1;
									misile[i].facing = MISILE_FACING_UP_RIGHT;
									GFX_SetSingleFrameAnimation(misile[i].sprite_num, 1, false);
								}
								// Go up right
								if (actor->pos_y + actor->height_px > misile[i].pos_y) {
									dx = 1;
									dy = -1;
									misile[i].facing = MISILE_FACING_UP_RIGHT;
									GFX_SetSingleFrameAnimation(misile[i].sprite_num, 1, false);
								}
								break;
							case MISILE_FACING_UP_RIGHT:// facing up-right
								// stay up by default
								dx = 1;
								dy = -1;
								// Go up
								if (actor->pos_x + actor->width_px < misile[i].pos_x) {
									dx = 0;
									dy = -1;
									misile[i].facing = MISILE_FACING_UP;
									GFX_SetSingleFrameAnimation(misile[i].sprite_num, 0, false);
								}
								// Go right
								if (actor->pos_x + actor->width_px > misile[i].pos_x) {
									dx = 1;
									dy = 0;
									misile[i].facing = MISILE_FACING_RIGHT;
									GFX_SetSingleFrameAnimation(misile[i].sprite_num, 2, false);
								}
								// Go right
								if (actor->pos_y > misile[i].pos_y + misile[i].height_px) {
									dx = 1;
									dy = 0;
									misile[i].facing = MISILE_FACING_RIGHT;
									GFX_SetSingleFrameAnimation(misile[i].sprite_num, 2, false);
								}
								break;
							case MISILE_FACING_UP_LEFT:// facing up-left
								// stay up by default
								dx = -1;
								dy = -1;
								// Go up
								if (actor->pos_x > misile[i].pos_x + misile[i].width_px) {
									dx = 0;
									dy = -1;
									misile[i].facing = MISILE_FACING_UP;
									GFX_SetSingleFrameAnimation(misile[i].sprite_num, 0, false);
								}
								// Go left
								if (actor->pos_x + actor->width_px < misile[i].pos_x) {
									dx = -1;
									dy = 0;
									GFX_SetSingleFrameAnimation(misile[i].sprite_num, 6, false);
								}
								// Go left
								if (actor->pos_y > misile[i].pos_y + misile[i].height_px) {
									dx = -1;
									dy = 0;
									misile[i].facing = MISILE_FACING_LEFT;
									GFX_SetSingleFrameAnimation(misile[i].sprite_num, 6, false);
								}
								break;
							default:
								dx = 0;
								dy = 0;
								break;
						}

						misile[i].vx_FP = (dx << FP);
						misile[i].vy_FP = (dy << FP);
					}

					// Calculate new pos
					for (j = 0; j < misile[i].speed; j++) {
						misile[i].x_FP += misile[i].vx_FP;
						misile[i].y_FP += misile[i].vy_FP;

						misile[i].pos_x = (misile[i].x_FP >> FP);
						misile[i].pos_y = (misile[i].y_FP >> FP);
						misile[i].pos_z = 1;

						// Check if hits something
						misile[i].hit_on = MISILE_CheckMisileColission(i);
						// Check hit on background, object or enemy
						switch (misile[i].hit_on & 0xFF) {
							case ENTITY_ID_BACKGROUND:
								tile_type = (misile[i].hit_on & 0xFF00) >> 8;
								switch (tile_type) {
									case 1:// full colission
										misile[i].on_target = true;
										break;
									case 2:// half colission
										break;
								}
								break;
							case ENTITY_ID_BARREL:
								misile[i].on_target = true;
								break;
							case ENTITY_ID_ENEMY:
								misile[i].on_target = true;
								break;
							case ENTITY_ID_ACTOR:
								misile[i].on_target = true;
								break;
							default:
								break;
						}

						if (misile[i].current_time <= 0) {
							misile[i].on_target = true;
						}
					}
				}

				// On target
				if (misile[i].on_target) {
					// Update sprite screen pos
					gfx_sprite_stack[misile[i].sprite_num].screen_pos_x = misile[i].pos_x - camera->pos_x;
					gfx_sprite_stack[misile[i].sprite_num].screen_pos_y = misile[i].pos_y - camera->pos_y;
					misile[i].pos_z = 0;

					PARTICLE_LoadParticle(SPRITE_GRAPHICS_ID_EXPLOSION1, ENTITY_ID_EXPLOSION, misile[i].pos_x - 16, misile[i].pos_y - misile[i].pos_z - 16, misile[i].pos_x - 16, misile[i].pos_y - misile[i].pos_z - 16, 3, misile[i].damage, 32, 32);
					AUDIO_PlaySound(AUDIO_EXPLOSSION, 16);
					MISILE_UnloadMisile(i);
				}
			}

			// Update sprite screen pos
			gfx_sprite_stack[misile[i].sprite_num].screen_pos_x = misile[i].pos_x - camera->pos_x;
			gfx_sprite_stack[misile[i].sprite_num].screen_pos_y = misile[i].pos_y - misile[i].pos_z - camera->pos_y;
		}
	}
}
