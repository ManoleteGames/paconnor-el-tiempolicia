#include "../engine.h"
#include "grenade.h"

Grenade grenade[GRENADE_MAX_GRENADES];

/** GRENADE :: Draw colission pixels
 *  - Draws colission pixels just for debug
 */
void GRENADE_DrawColissionPixels(Grenade g) {
	//  Draw colission pixels
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[g.sprite_num].screen_pos_x + g.colission_area.points[0][0], gfx_sprite_stack[g.sprite_num].screen_pos_y + g.colission_area.points[0][1], 229);
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[g.sprite_num].screen_pos_x + g.colission_area.points[1][0], gfx_sprite_stack[g.sprite_num].screen_pos_y + g.colission_area.points[1][1], 229);
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[g.sprite_num].screen_pos_x + g.colission_area.points[2][0], gfx_sprite_stack[g.sprite_num].screen_pos_y + g.colission_area.points[2][1], 229);
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[g.sprite_num].screen_pos_x + g.colission_area.points[3][0], gfx_sprite_stack[g.sprite_num].screen_pos_y + g.colission_area.points[3][1], 229);
}


/** GRENADE :: Initialize grenade
 */
int GRENADE_InitGrenade(int graphics_id, int entity_id, int pos_x, int pos_y, int max_time, int max_distance) {
	int i, number, sprite_slot;

	// Search free grenade slot
	number = 0;

	for (i = 0; i < GRENADE_MAX_GRENADES; i++) {
		if (!grenade[i].loaded) {
			number = i;
			break;
		}
	}

	// Check if max grenades is reached
	if (number >= GRENADE_MAX_GRENADES - 1) {
		Error("GRENADE_InitGrenade function error", "Max number of grenades", "", ERROR_SYSTEM);
	}

	grenade[number].current_time_ms = TIMER_GetMilliseconds();
	grenade[number].end_time_ms = grenade[number].current_time_ms + (actor->grenades_max_time * 1000);
	grenade[number].current_time = max_time;
	grenade[number].pos_x = pos_x;
	grenade[number].pos_y = pos_y;
	grenade[number].pos_z = 1;
	grenade[number].speed = 3;
	grenade[number].damage = GRENADE_DAMAGE;
	grenade[number].graphics_id = graphics_id;

	sprite_slot = GFX_FindEmptySpriteSlot();
	if (sprite_slot == -1) {
		sprintf(engine.system_error_message1, "GRENADE_InitGrenade function error");
		sprintf(engine.system_error_message2, "No empty sprite slot available");
		sprintf(engine.system_error_message3, " ");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
	} else {
		grenade[number].sprite_num = sprite_slot;
		GFX_InitSprite(entity_id, number, sprite_slot, 0, 4, 4);
	}

	// Set effect graphics
	GFX_SetSpriteGraphic(grenade[number].sprite_num, 0, graphics_id, 0, 0);
	GFX_SetSingleFrameAnimation(grenade[number].sprite_num, 0);
	GFX_SetSpriteBlinkingProperties(grenade[i].sprite_num, true, 20, GRENADE_BLINK_COLOR);

	grenade[number].width_px = GFX_GetSpriteWidth_px(grenade[number].sprite_num);
	grenade[number].height_px = GFX_GetSpriteHeight_px(grenade[number].sprite_num);

	// Set colission points
	//  ---------------
	// |               |
	// |  0         1  |
	// |               |
	// |               |
	// |  2         3  |
	//  ---------------
	// Point 0
	grenade[number].colission_area.points[0][0] = grenade[number].width_px >> 4;
	grenade[number].colission_area.points[0][1] = grenade[number].height_px >> 4;
	// Point 1
	grenade[number].colission_area.points[1][0] = grenade[number].width_px - (grenade[number].width_px >> 4);
	grenade[number].colission_area.points[1][1] = grenade[number].height_px >> 4;
	// Point 2
	grenade[number].colission_area.points[2][0] = grenade[number].width_px >> 4;
	grenade[number].colission_area.points[2][1] = grenade[number].height_px - (grenade[number].height_px >> 4);
	// Point 3
	grenade[number].colission_area.points[3][0] = grenade[number].width_px - (grenade[number].width_px >> 4);
	grenade[number].colission_area.points[3][1] = grenade[number].height_px - (grenade[number].height_px >> 4);

	grenade[number].loaded = true;
	grenade[number].shown = true;
	grenade[number].hold = true;

	return number;
}


/** GRENADE :: Throw grenade
 * 
 */
void GRENADE_ThrowGrenade(int number, int target_x, int target_y, int accuracy) {
	int dx, dy, dz;
	int distance;

	// Calculate target
	switch (accuracy) {
		case 0:
			grenade[number].target_x = target_x - 4 - 8 + (rand() % 32);
			grenade[number].target_y = target_y - 4 - 8 + (rand() % 32);
			break;
		case 1:
			grenade[number].target_x = target_x - 4 + (rand() % 24);
			grenade[number].target_y = target_y - 4 + (rand() % 24);
			break;
		case 2:
			grenade[number].target_x = target_x - 8 + (rand() % 16);
			grenade[number].target_y = target_y - 8 + (rand() % 16);
			break;
		case 3:
			grenade[number].target_x = target_x + 4 + (rand() % 8);
			grenade[number].target_y = target_y + 4 + (rand() % 8);
			break;
		case 4:
			grenade[number].target_x = target_x + 4;
			grenade[number].target_y = target_y + 4;
			break;
		default:
			grenade[number].target_x = target_x + 4;
			grenade[number].target_y = target_y + 4;
			break;
	}

	// calculate distance from source
	dx = grenade[number].target_x - grenade[number].pos_x;
	dy = grenade[number].target_y - grenade[number].pos_y;

	// calculate steps
	if (abs(dx) > abs(dy))
		distance = abs(dx);
	else
		distance = abs(dy);

	dz = distance >> 6;

	// calculate fixed point velocity and position
	grenade[number].vx_FP = (dx << FP) / distance;
	grenade[number].vy_FP = (dy << FP) / distance;
	grenade[number].vz_FP = (dz << FP);
	grenade[number].g_FP = 3000;

	grenade[number].x_FP = grenade[number].pos_x << FP;
	grenade[number].y_FP = grenade[number].pos_y << FP;
	grenade[number].z_FP = grenade[number].pos_z << FP;

	grenade[number].steps = distance;
	if (grenade[number].steps > actor->grenades_max_distance)
		grenade[number].steps = actor->grenades_max_distance;

	GFX_SetDefaultAnimation(grenade[number].sprite_num, false, true, 5);
	grenade[number].hold = false;
}

/**
 * 1= left colision
 * 2= right colision
 * 3= up colission
 * 4= down colission
 */
int GRENADE_CheckGrenadeColission(int number) {
	int i;
	int point1_x, point1_y;
	int point2_x, point2_y;
	int point3_x, point3_y;
	int point4_x, point4_y;
	int tile_type;

	point1_x = grenade[number].pos_x + grenade[number].colission_area.points[0][0];
	point1_y = grenade[number].pos_y + grenade[number].colission_area.points[0][1];
	point2_x = grenade[number].pos_x + grenade[number].colission_area.points[1][0];
	point2_y = grenade[number].pos_y + grenade[number].colission_area.points[1][1];
	point3_x = grenade[number].pos_x + grenade[number].colission_area.points[2][0];
	point3_y = grenade[number].pos_y + grenade[number].colission_area.points[2][1];
	point4_x = grenade[number].pos_x + grenade[number].colission_area.points[3][0];
	point4_y = grenade[number].pos_y + grenade[number].colission_area.points[3][1];

	/////// BACKGROUND COLISSIONS ///////////
	tile_type = MAP_CheckColissionTile(point1_x + 3, point1_y - 3);// Check point 1 for up colission
	switch (tile_type) {
		case 0:// no colission
			break;
		case 1:// full colission
			return 3;
			break;
		case 2:// half colission
		case 3:// breakable
		case 4:// ..
		case 5:///
			if (grenade[number].pos_z < 5) { return 3; }
			break;
		default:
			break;
	}
	tile_type = MAP_CheckColissionTile(point1_x - 3, point1_y + 3);// Check point 1 for left colission
	switch (tile_type) {
		case 0:// no colission
			break;
		case 1:// full colission
			return 3;
			break;
		case 2:// half colission
		case 3:// breakable
		case 4:// ..
		case 5:///
			if (grenade[number].pos_z < 5) { return 1; }
			break;
		default:
			break;
	}
	tile_type = MAP_CheckColissionTile(point2_x - 3, point2_y - 3);// Check point 2 for up colission
	switch (tile_type) {
		case 0:// no colission
			break;
		case 1:// full colission
			return 3;
			break;
		case 2:// half colission
		case 3:// breakable
		case 4:// ..
		case 5:///
			if (grenade[number].pos_z < 5) { return 3; }
			break;
		default:
			break;
	}
	tile_type = MAP_CheckColissionTile(point2_x + 3, point2_y + 3);// Check point 2 for right colission
	switch (tile_type) {
		case 0:// no colission
			break;
		case 1:// full colission
			return 3;
			break;
		case 2:// half colission
		case 3:// breakable
		case 4:// ..
		case 5:///
			if (grenade[number].pos_z < 5) { return 2; }
			break;
		default:
			break;
	}
	tile_type = MAP_CheckColissionTile(point3_x - 3, point3_y - 3);// Check point 3 for left colission
	switch (tile_type) {
		case 0:// no colission
			break;
		case 1:// full colission
			return 3;
			break;
		case 2:// half colission
		case 3:// breakable
		case 4:// ..
		case 5:///
			if (grenade[number].pos_z < 5) { return 1; }
			break;
		default:
			break;
	}
	tile_type = MAP_CheckColissionTile(point3_x + 3, point3_y + 3);// Check point 3 for down colission
	switch (tile_type) {
		case 0:// no colission
			break;
		case 1:// full colission
			return 3;
			break;
		case 2:// half colission
		case 3:// breakable
		case 4:// ..
		case 5:///
			if (grenade[number].pos_z < 5) { return 4; }
			break;
		default:
			break;
	}
	tile_type = MAP_CheckColissionTile(point4_x + 3, point4_y - 3);// Check point 4 for right colission
	switch (tile_type) {
		case 0:// no colission
			break;
		case 1:// full colission
			return 3;
			break;
		case 2:// half colission
		case 3:// breakable
		case 4:// ..
		case 5:///
			if (grenade[number].pos_z < 5) { return 2; }
			break;
		default:
			break;
	}

	tile_type = MAP_CheckColissionTile(point4_x - 3, point4_y + 3);// Check point 4 for down colission
	switch (tile_type) {
		case 0:// no colission
			break;
		case 1:// full colission
			return 3;
			break;
		case 2:// half colission
		case 3:// breakable
		case 4:// ..
		case 5:///
			if (grenade[number].pos_z < 5) { return 4; }
			break;
		default:
			break;
	}

	/////// OBJECTS COLISSIONS ///////////
	for (i = 0; i < OBJECT_MAX_OBJECTS; i++) {
		if (object[i].is_loaded) {
			if (gfx_sprite_stack[object[i].num_sprite].shown) {
				//Point 1

				if (point1_x < object[i].pos_x + object[i].hit_area.points[1][0]

					&& point2_x > object[i].pos_x + object[i].hit_area.points[0][0]

					&& point1_y > object[i].pos_y + object[i].hit_area.points[0][1]

					&& point3_y < object[i].pos_y + object[i].hit_area.points[2][1]) {
					return 3;
				}
			}
		}
	}

	return 0;
}


void GRENADE_UnloadGrenades(void) {
	int i;
	for (i = 0; i < GRENADE_MAX_GRENADES; i++) {
		if (grenade[i].loaded) {
			GFX_UnloadSprite(grenade[i].sprite_num);
			grenade[i].loaded = false;
			grenade[i].sprite_num = -1;
		}
	}
}

void GRENADE_UnloadGrenade(int number) {
	if (grenade[number].loaded) {
		GFX_UnloadSprite(grenade[number].sprite_num);
		grenade[number].loaded = false;
		grenade[number].sprite_num = -1;
	}
}

/** GRENADE :: Update grenades
 */
void GRENADE_Update(void) {
	int i, j;
	int aux;

	for (i = 0; i < GRENADE_MAX_GRENADES; i++) {
		if (grenade[i].loaded) {

			// debug
			//GRENADE_DrawColissionPixels(grenade[i]);

			grenade[i].current_time_ms = TIMER_GetMilliseconds();
			grenade[i].current_time = (grenade[i].end_time_ms - grenade[i].current_time_ms) / 1000;

			// Blink
			switch (grenade[i].current_time) {
				case 5:
					GFX_SetSpriteBlinkingProperties(grenade[i].sprite_num, true, 20, GRENADE_BLINK_COLOR);
					gfx_sprite_stack[grenade[i].sprite_num].blink.time = 20;
					break;
				case 4:
					GFX_SetSpriteBlinkingProperties(grenade[i].sprite_num, true, 15, GRENADE_BLINK_COLOR);
					gfx_sprite_stack[grenade[i].sprite_num].blink.time = 15;
					break;
				case 3:
					GFX_SetSpriteBlinkingProperties(grenade[i].sprite_num, true, 10, GRENADE_BLINK_COLOR);
					gfx_sprite_stack[grenade[i].sprite_num].blink.time = 10;
					break;
				case 2:
					GFX_SetSpriteBlinkingProperties(grenade[i].sprite_num, true, 5, GRENADE_BLINK_COLOR);
					gfx_sprite_stack[grenade[i].sprite_num].blink.time = 5;
					break;
				case 1:
					GFX_SetSpriteBlinkingProperties(grenade[i].sprite_num, true, 2, GRENADE_BLINK_COLOR);
					gfx_sprite_stack[grenade[i].sprite_num].blink.time = 2;
					break;
				case 0:
					GFX_SetSpriteBlinkingProperties(grenade[i].sprite_num, true, 1, GRENADE_BLINK_COLOR);
					gfx_sprite_stack[grenade[i].sprite_num].blink.time = 1;
					break;
				default:
					gfx_sprite_stack[grenade[i].sprite_num].blink.time = 1;
					GFX_SetSpriteBlinkingProperties(grenade[i].sprite_num, true, 1, GRENADE_BLINK_COLOR);
					break;
			}

			// Play tick tack sound
			if (gfx_sprite_stack[grenade[i].sprite_num].blink.blinked && !grenade[i].tick_played) {
				AUDIO_PlaySound(AUDIO_TICK_EFFECT, 0);
				grenade[i].tick_played = true;
				grenade[i].tack_played = false;
			}
			if (!gfx_sprite_stack[grenade[i].sprite_num].blink.blinked && !grenade[i].tack_played) {
				AUDIO_PlaySound(AUDIO_TACK_EFFECT, 0);
				grenade[i].tick_played = false;
				grenade[i].tack_played = true;
			}

			// Being hold
			if (grenade[i].hold) {
				switch (actor->last_facing) {
					case ACTOR_FACING_UP:
						grenade[i].pos_x = actor->pos_x - 2;
						grenade[i].pos_y = actor->pos_y + 12;
						GFX_SetSpritePriority(grenade[i].sprite_num, 0);
						break;
					case ACTOR_FACING_UP_RIGHT:
						grenade[i].pos_x = actor->pos_x + 4;
						grenade[i].pos_y = actor->pos_y + 8;
						GFX_SetSpritePriority(grenade[i].sprite_num, 0);
						break;
					case ACTOR_FACING_RIGHT:
						grenade[i].pos_x = actor->pos_x + 18;
						grenade[i].pos_y = actor->pos_y + 10;
						GFX_SetSpritePriority(grenade[i].sprite_num, 16);
						break;
					case ACTOR_FACING_DOWN_RIGHT:
						grenade[i].pos_x = actor->pos_x + 18;
						grenade[i].pos_y = actor->pos_y + 10;
						GFX_SetSpritePriority(grenade[i].sprite_num, 16);
						break;
					case ACTOR_FACING_DOWN:
						grenade[i].pos_x = actor->pos_x + 18;
						grenade[i].pos_y = actor->pos_y + 16;
						GFX_SetSpritePriority(grenade[i].sprite_num, 16);
						break;
					case ACTOR_FACING_DOWN_LEFT:
						grenade[i].pos_x = actor->pos_x + 18;
						grenade[i].pos_y = actor->pos_y + 14;
						GFX_SetSpritePriority(grenade[i].sprite_num, 16);
						break;
					case ACTOR_FACING_LEFT:
						grenade[i].pos_x = actor->pos_x + 10;
						grenade[i].pos_y = actor->pos_y + 17;
						GFX_SetSpritePriority(grenade[i].sprite_num, 16);
						break;
					case ACTOR_FACING_UP_LEFT:
						grenade[i].pos_x = actor->pos_x + 2;
						grenade[i].pos_y = actor->pos_y + 16;
						GFX_SetSpritePriority(grenade[i].sprite_num, 0);
						break;
				}
				GFX_SetSpritePosition(grenade[i].sprite_num, grenade[i].pos_x - camera->pos_x, grenade[i].pos_y - camera->pos_y);
			}

			// Not on target
			if (!grenade[i].hold && !grenade[i].on_target) {
				// Calculate new pos

				for (j = 0; j < grenade[i].speed; j++) {
					grenade[i].x_FP += grenade[i].vx_FP;
					grenade[i].y_FP += grenade[i].vy_FP;
					grenade[i].z_FP += grenade[i].vz_FP;
					aux = (int) grenade[i].z_FP >> 8;// get int part of fixed value
					if (aux > 0) {
						grenade[i].vz_FP -= grenade[i].g_FP;
					} else {
						grenade[i].z_FP = 0 << FP;
						grenade[i].vz_FP = (grenade[i].steps - grenade[i].current_step) << 10;
					}

					grenade[i].pos_x = (grenade[i].x_FP >> FP);
					grenade[i].pos_y = (grenade[i].y_FP >> FP);
					grenade[i].pos_z = (grenade[i].z_FP >> FP);

					if (grenade[i].pos_z < 0) grenade[i].pos_z = 0;

					// Update sprite screen pos
					gfx_sprite_stack[grenade[i].sprite_num].screen_pos_x = grenade[i].pos_x - camera->pos_x;
					gfx_sprite_stack[grenade[i].sprite_num].screen_pos_y = grenade[i].pos_y - grenade[i].pos_z - camera->pos_y;

					// Check if hits something
					grenade[i].hit_on = GRENADE_CheckGrenadeColission(i);

					// Check hit on background, object or enemy
					switch (grenade[i].hit_on) {
						case 1:// left
							grenade[i].vx_FP *= -1;
							break;
						case 2:// right
							grenade[i].vx_FP *= -1;
							break;
						case 3:// up
							grenade[i].vy_FP *= -1;
							break;
						case 4:// down
							grenade[i].vy_FP *= -1;
							break;
						default:
							break;
					}

					grenade[i].current_step++;

					if ((grenade[i].current_step >= grenade[i].steps) && (grenade[i].pos_z == 0)) {
						grenade[i].on_target = true;
						GFX_SetSingleFrameAnimation(grenade[i].sprite_num, 0);
						break;
					}
				}
			}

			// On target
			if (grenade[i].on_target) {
				// Update sprite screen pos
				gfx_sprite_stack[grenade[i].sprite_num].screen_pos_x = grenade[i].pos_x - camera->pos_x;
				gfx_sprite_stack[grenade[i].sprite_num].screen_pos_y = grenade[i].pos_y - camera->pos_y;
				grenade[i].pos_z = 0;
			}

			if (grenade[i].current_time <= 0) {
				grenade[i].on_target = false;
				grenade[i].shown = false;
				grenade[i].hold = false;
				grenade[i].current_step = 0;

				if (grenade[i].pos_z < 5) {// Explode on the floor
					PARTICLE_LoadParticle(SPRITE_GRAPHICS_ID_EXPLOSION1, ENTITY_ID_EXPLOSION, grenade[i].pos_x - 16, grenade[i].pos_y - grenade[i].pos_z - 16, grenade[i].pos_x - 16, grenade[i].pos_y - grenade[i].pos_z - 16, 3, grenade[i].damage, 20, 16);
				} else {// Explode on the air
					PARTICLE_LoadParticle(SPRITE_GRAPHICS_ID_EXPLOSION1, ENTITY_ID_EXPLOSION, grenade[i].pos_x - 16, grenade[i].pos_y - grenade[i].pos_z - 16, grenade[i].pos_x - 16, grenade[i].pos_y - grenade[i].pos_z - 16, 3, 0, 0, 0);
				}
				AUDIO_PlaySound(AUDIO_EXPLOSSION, 16);
				GRENADE_UnloadGrenade(i);
			}
		}
	}
}
