#include "../engine.h"
#include "bullet.h"

Bullet *bullet;
int bullet_counter;

void BULLET_Init(void) {
	bullet = MM_PushChunk(sizeof(Bullet) * BULLET_MAX_BULLETS, CT_ENGINE);
}

/** BULLET :: Initialize bullet
 */
void BULLET_LoadBullet(int graphics_id, int entity_id, int width, int height, int source_x, int source_y, int target_x, int target_y, int accuracy, int max_distance, int speed, int damage) {
	int i;
	int number;
	int dx, dy;
	int distance;
	int sprite_slot;

	// Search free bullet slot
	number = 999;
	for (i = 0; i < BULLET_MAX_BULLETS; i++) {
		if (!bullet[i].loaded) {
			number = i;
			break;
		}
	}

	// Check if max bullets is reached
	if (number == 999) {
		Error("BULLET_LoadBullet function error", "Max number of bullets", "", ERROR_SYSTEM);
	}

	bullet[number].pos_x = source_x;
	bullet[number].pos_y = source_y;
	bullet[number].speed = speed;
	bullet[number].damage = damage;
	bullet[number].graphics_id = graphics_id;

	// Calculate target
	switch (accuracy) {
		case 0:
			bullet[number].target_x = target_x - 16 + (rand() % 32);
			bullet[number].target_y = target_y - 16 + (rand() % 32);
			break;
		case 1:
			bullet[number].target_x = target_x - 8 + (rand() % 16);
			bullet[number].target_y = target_y - 8 + (rand() % 16);
			break;
		case 2:
			bullet[number].target_x = target_x - 4 + (rand() % 8);
			bullet[number].target_y = target_y - 4 + (rand() % 8);
			break;
		case 3:
			bullet[number].target_x = target_x - 2 + (rand() % 4);
			bullet[number].target_y = target_y - 2 + (rand() % 4);
			break;
		case 4:
			bullet[number].target_x = target_x;
			bullet[number].target_y = target_y;
			break;
		default:
			bullet[number].target_x = target_x;
			bullet[number].target_y = target_y;
			break;
	}

	// calculate distance from source
	dx = bullet[number].target_x - bullet[number].pos_x;
	dy = bullet[number].target_y - bullet[number].pos_y;

	// calculate steps
	if (abs(dx) > abs(dy))
		distance = abs(dx);
	else
		distance = abs(dy);

	// Avoid 0 division or die!!
	if (distance == 0) distance = 1;

	// calculate fixed point velocity and position
	bullet[number].vx_FP = (dx << FP) / distance;
	bullet[number].vy_FP = (dy << FP) / distance;

	bullet[number].x_FP = bullet[number].pos_x << FP;
	bullet[number].y_FP = bullet[number].pos_y << FP;

	bullet[number].steps = max_distance;
	//if (bullet[number].steps > max_distance)
	//	bullet[number].steps = max_distance;

	sprite_slot = GFX_FindEmptySpriteSlot();
	if (gfx_sprite_stack[sprite_slot].loaded || (sprite_slot == -1)) {
		sprintf(engine.system_error_message1, "BULLET_LoadBullet function error");
		sprintf(engine.system_error_message2, "No empty sprite slot available");
		sprintf(engine.system_error_message3, " ");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
	} else {
		bullet[number].sprite_num = sprite_slot;
		GFX_InitSprite(entity_id, number, sprite_slot, 1, GFX_GetSpriteGraphicsWidth_px(graphics_id), GFX_GetSpriteGraphicsHeight_px(graphics_id));
	}

	bullet[number].width_px = width;
	bullet[number].height_px = height;

	// Set effect graphics
	GFX_SetSpriteGraphic(bullet[number].sprite_num, 0, graphics_id, 0, 0);
	GFX_SetSingleFrameAnimation(bullet[number].sprite_num, 0);

	// Set colission points
	//  ---------------
	// |  1         2  |
	// |               |
	// |               |
	// |               |
	// |  3         4  |
	//  ---------------
	// Point 1
	bullet[number].colission_area.points[0][0] = 0;
	bullet[number].colission_area.points[0][1] = 0;
	// Point 2
	bullet[number].colission_area.points[1][0] = width;
	bullet[number].colission_area.points[1][1] = 0;

	// Point 3
	bullet[number].colission_area.points[2][0] = 0;
	bullet[number].colission_area.points[2][1] = height;

	// Point 4
	bullet[number].colission_area.points[3][0] = width;
	bullet[number].colission_area.points[3][1] = height;


	bullet[number].loaded = true;
	bullet[number].shown = true;
}

/** ACTOR :: Draw colission pixels
 *  - Draws colission pixels just for debug
 */
void BULLET_DrawColissionPixels(Bullet b) {
	//  Draw colission pixels
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[b.sprite_num].screen_pos_x + b.colission_area.points[0][0], gfx_sprite_stack[b.sprite_num].screen_pos_y + b.colission_area.points[0][1], 229);
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[b.sprite_num].screen_pos_x + b.colission_area.points[1][0], gfx_sprite_stack[b.sprite_num].screen_pos_y + b.colission_area.points[1][1], 229);
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[b.sprite_num].screen_pos_x + b.colission_area.points[2][0], gfx_sprite_stack[b.sprite_num].screen_pos_y + b.colission_area.points[2][1], 229);
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[b.sprite_num].screen_pos_x + b.colission_area.points[3][0], gfx_sprite_stack[b.sprite_num].screen_pos_y + b.colission_area.points[3][1], 229);
}

int BULLET_CheckHitBreakableTile(int number) {
	int point1_x, point1_y;
	int point2_x, point2_y;
	int point3_x, point3_y;
	int point4_x, point4_y;
	int tile_type;

	point1_x = bullet[number].pos_x + bullet[number].colission_area.points[0][0];
	point1_y = bullet[number].pos_y + bullet[number].colission_area.points[0][1];
	point2_x = bullet[number].pos_x + bullet[number].colission_area.points[1][0];
	point2_y = bullet[number].pos_y + bullet[number].colission_area.points[1][1];
	point3_x = bullet[number].pos_x + bullet[number].colission_area.points[2][0];
	point3_y = bullet[number].pos_y + bullet[number].colission_area.points[2][1];
	point4_x = bullet[number].pos_x + bullet[number].colission_area.points[3][0];
	point4_y = bullet[number].pos_y + bullet[number].colission_area.points[3][1];

	/////// BACKGROUND ANIMATION ///////////
	tile_type = MAP_CheckAnimationTile(point1_x, point1_y);// Check point 1
	if (tile_type != 0) {
		return ((tile_type & 0xFF) << 8) | ENTITY_ID_BREAKABLE_BACKGROUND;
	}
	tile_type = MAP_CheckAnimationTile(point2_x, point2_y);// Check point 2
	if (tile_type != 0) {
		return ((tile_type & 0xFF) << 8) | ENTITY_ID_BREAKABLE_BACKGROUND;
	}
	tile_type = MAP_CheckAnimationTile(point3_x, point3_y);// Check point 3
	if (tile_type != 0) {
		return ((tile_type & 0xFF) << 8) | ENTITY_ID_BREAKABLE_BACKGROUND;
	}
	tile_type = MAP_CheckAnimationTile(point4_x, point4_y);// Check point 4
	if (tile_type != 0) {
		return ((tile_type & 0xFF) << 8) | ENTITY_ID_BREAKABLE_BACKGROUND;
	}

	return 0;
}

int BULLET_CheckBulletColission(int number) {
	int i;
	int point1_x, point1_y;
	int point2_x, point2_y;
	int point3_x, point3_y;
	int point4_x, point4_y;
	int tile_type;
	bool collision_detected;

	point1_x = bullet[number].pos_x + bullet[number].colission_area.points[0][0];
	point1_y = bullet[number].pos_y + bullet[number].colission_area.points[0][1];
	point2_x = bullet[number].pos_x + bullet[number].colission_area.points[1][0];
	point2_y = bullet[number].pos_y + bullet[number].colission_area.points[1][1];
	point3_x = bullet[number].pos_x + bullet[number].colission_area.points[2][0];
	point3_y = bullet[number].pos_y + bullet[number].colission_area.points[2][1];
	point4_x = bullet[number].pos_x + bullet[number].colission_area.points[3][0];
	point4_y = bullet[number].pos_y + bullet[number].colission_area.points[3][1];

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

	/////// BOSS COLISSIONS /////////////
	collision_detected = true;
	if (!boss->action_dead && gfx_sprite_stack[bullet[number].sprite_num].entity_id != ENTITY_ID_ENEMY_BULLET) {
		if (point1_x > boss->pos_x + boss->hit_area.points[1][0]) collision_detected = false;// Out of left side
		if (point2_x < boss->pos_x + boss->hit_area.points[0][0]) collision_detected = false;//  Out of right side
		if (point3_y < boss->pos_y + boss->hit_area.points[0][1]) collision_detected = false;//  Out of top side
		if (point1_y > boss->pos_y + boss->hit_area.points[2][1]) collision_detected = false;//  Out of bottom side
		if (collision_detected) return gfx_sprite_stack[boss->sprite_num].id;
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

void BULLET_UnloadBullet(int number) {
	if (bullet[number].loaded) {
		GFX_UnloadSprite(bullet[number].sprite_num);
		bullet[number].loaded = false;
		bullet[number].sprite_num = -1;
	}
}

/** BULLET :: Update bullets	
 */
void BULLET_Update(void) {
	int i, j;
	int obj_number, enemy_number;
	byte tile_type;
	int tile_number, tile_data, tileset_index;
	int tile_x, tile_y;
	int tile_screen_x, tile_screen_y;


	bullet_counter = 0;
	for (i = 0; i < BULLET_MAX_BULLETS; i++) {
		if (bullet[i].loaded) {

			bullet_counter++;

			// debug
			//BULLET_DrawColissionPixels(bullet[i]);

			// trace bullet path, even not drawing points
			for (j = 0; j < bullet[i].speed; j++) {

				// Not on target
				if (!bullet[i].on_target) {

					bullet[i].x_FP += bullet[i].vx_FP;
					bullet[i].y_FP += bullet[i].vy_FP;

					bullet[i].pos_x = (bullet[i].x_FP >> FP);
					bullet[i].pos_y = (bullet[i].y_FP >> FP);

					// Update sprite screen pos
					gfx_sprite_stack[bullet[i].sprite_num].screen_pos_x = bullet[i].pos_x - camera->pos_x;
					gfx_sprite_stack[bullet[i].sprite_num].screen_pos_y = bullet[i].pos_y - camera->pos_y;

					if (gfx_sprite_stack[bullet[i].sprite_num].screen_pos_x < 0) BULLET_UnloadBullet(i);
					if (gfx_sprite_stack[bullet[i].sprite_num].screen_pos_x > CAM_VISIBLE_WIDTH) BULLET_UnloadBullet(i);
					if (gfx_sprite_stack[bullet[i].sprite_num].screen_pos_y < 0) BULLET_UnloadBullet(i);
					if (gfx_sprite_stack[bullet[i].sprite_num].screen_pos_y > CAM_VISIBLE_HEIGHT) BULLET_UnloadBullet(i);
					if (!bullet[i].loaded) return;

					// Check if hits something breakable
					bullet[i].hit_on = BULLET_CheckHitBreakableTile(i);
					// Check hit on background, object or enemy
					switch (bullet[i].hit_on & 0xFF) {
						case ENTITY_ID_BREAKABLE_BACKGROUND:
							tile_type = (bullet[i].hit_on & 0xFF00) >> 8;
							switch (tile_type) {
								case 1:// breaking tile. Set colission data to half colission and breakable to null
									tile_number = MAP_GetTileNumber(bullet[i].pos_x, bullet[i].pos_y);
									map->layer[MAP_ANIM_LAYER][tile_number] = 0;
									// Get map tile position on screen
									tile_screen_x = ((bullet[i].pos_x >> 4) << 4) - camera->pos_x;
									tile_screen_y = ((bullet[i].pos_y >> 4) << 4) - camera->pos_y;
									// Get map tile position on video buffer
									tile_x = camera->pos_abs_x + tile_screen_x;
									if (tile_x >= video->map_buffer_width) {
										tile_x = tile_x - video->map_buffer_width;
									}
									tile_y = camera->pos_abs_y + tile_screen_y;
									if (tile_y >= video->map_buffer_height) {
										tile_y = tile_y - video->map_buffer_height;
									}
									// redraw tile
									tile_data = map->layer[MAP_BACKGROUND_LAYER][tile_number];
									tileset_index = tile_data << 8;
									MAP_DrawToVideoBuffer(tile_x, tile_y, MAP_TILE_WIDTH, MAP_TILE_HEIGHT, &map->layer_tiles[MAP_BACKGROUND_LAYER].buffer[tileset_index], video->map_buffer[MAP_BACKGROUND_LAYER]);
									GFX_SetDefaultAnimation(bullet[i].sprite_num, false, false, 3);
									break;
								case 4:// breaking tile with electric effect. Set colission data to half colission and breakable to null
									tile_number = MAP_GetTileNumber(bullet[i].pos_x, bullet[i].pos_y);
									map->layer[MAP_ANIM_LAYER][tile_number] = 0;
									// Get map tile position on screen
									tile_screen_x = ((bullet[i].pos_x >> 4) << 4) - camera->pos_x;
									tile_screen_y = ((bullet[i].pos_y >> 4) << 4) - camera->pos_y;
									// Get map tile position on video buffer
									tile_x = camera->pos_abs_x + tile_screen_x;
									if (tile_x >= video->map_buffer_width) {
										tile_x = tile_x - video->map_buffer_width;
									}
									tile_y = camera->pos_abs_y + tile_screen_y;
									if (tile_y >= video->map_buffer_height) {
										tile_y = tile_y - video->map_buffer_height;
									}
									// redraw tile
									tile_data = map->layer[MAP_BACKGROUND_LAYER][tile_number];
									tileset_index = tile_data << 8;
									MAP_DrawToVideoBuffer(tile_x, tile_y, MAP_TILE_WIDTH, MAP_TILE_HEIGHT, &map->layer_tiles[MAP_BACKGROUND_LAYER].buffer[tileset_index], video->map_buffer[MAP_BACKGROUND_LAYER]);
									EFFECT_LoadEffect(ENTITY_ID_EMPTY, SPRITE_GRAPHICS_ID_ELECTRIC1, ((bullet[i].pos_x >> 4) << 4) + 2, ((bullet[i].pos_y >> 4) << 4) + 2, true, 0, false, false, 5);
									GFX_SetDefaultAnimation(bullet[i].sprite_num, false, false, 3);
									break;
								case 5:// break tile with fire. Set colission data to half colission and breakable to null
									tile_number = MAP_GetTileNumber(bullet[i].pos_x, bullet[i].pos_y);
									map->layer[MAP_ANIM_LAYER][tile_number] = 0;
									// Get map tile position on screen
									tile_screen_x = ((bullet[i].pos_x >> 4) << 4) - camera->pos_x;
									tile_screen_y = ((bullet[i].pos_y >> 4) << 4) - camera->pos_y;
									// Get map tile position on video buffer
									tile_x = camera->pos_abs_x + tile_screen_x;
									if (tile_x >= video->map_buffer_width) {
										tile_x = tile_x - video->map_buffer_width;
									}
									tile_y = camera->pos_abs_y + tile_screen_y;
									if (tile_y >= video->map_buffer_height) {
										tile_y = tile_y - video->map_buffer_height;
									}
									// redraw tile
									tile_data = map->layer[MAP_BACKGROUND_LAYER][tile_number];
									tileset_index = tile_data << 8;
									MAP_DrawToVideoBuffer(tile_x, tile_y, MAP_TILE_WIDTH, MAP_TILE_HEIGHT, &map->layer_tiles[MAP_BACKGROUND_LAYER].buffer[tileset_index], video->map_buffer[MAP_BACKGROUND_LAYER]);
									EFFECT_LoadEffect(ENTITY_ID_EMPTY, SPRITE_GRAPHICS_ID_FIRE1, ((bullet[i].pos_x >> 4) << 4) + 2, ((bullet[i].pos_y >> 4) << 4) + 2, true, 0, false, false, 5);
									GFX_SetDefaultAnimation(bullet[i].sprite_num, false, false, 3);

									break;
								case 6:// break tile with water. Set colission data to half colission and breakable to null
									tile_number = MAP_GetTileNumber(bullet[i].pos_x, bullet[i].pos_y);
									map->layer[MAP_ANIM_LAYER][tile_number] = 0;
									// Get map tile position on screen
									tile_screen_x = ((bullet[i].pos_x >> 4) << 4) - camera->pos_x;
									tile_screen_y = ((bullet[i].pos_y >> 4) << 4) - camera->pos_y;
									// Get map tile position on video buffer
									tile_x = camera->pos_abs_x + tile_screen_x;
									if (tile_x >= video->map_buffer_width) {
										tile_x = tile_x - video->map_buffer_width;
									}
									tile_y = camera->pos_abs_y + tile_screen_y;
									if (tile_y >= video->map_buffer_height) {
										tile_y = tile_y - video->map_buffer_height;
									}
									// redraw tile
									tile_data = map->layer[MAP_BACKGROUND_LAYER][tile_number];
									tileset_index = tile_data << 8;
									MAP_DrawToVideoBuffer(tile_x, tile_y, MAP_TILE_WIDTH, MAP_TILE_HEIGHT, &map->layer_tiles[MAP_BACKGROUND_LAYER].buffer[tileset_index], video->map_buffer[MAP_BACKGROUND_LAYER]);
									EFFECT_LoadEffect(ENTITY_ID_EMPTY, SPRITE_GRAPHICS_ID_WATER1, ((bullet[i].pos_x >> 4) << 4) + 2, ((bullet[i].pos_y >> 4) << 4) + 2, true, 0, false, false, 5);
									GFX_SetDefaultAnimation(bullet[i].sprite_num, false, false, 3);

									break;
								default:
									break;
							}
							break;
						default:
							break;
					}

					// Check if hits possible targets
					bullet[i].hit_on = BULLET_CheckBulletColission(i);

					// Check hit on background, object or enemy
					switch (bullet[i].hit_on & 0xFF) {
						case ENTITY_ID_BACKGROUND:
							tile_type = (bullet[i].hit_on & 0xFF00) >> 8;
							switch (tile_type) {
								case 1:// full colission
									EFFECT_LoadEffect(ENTITY_ID_EMPTY, SPRITE_GRAPHICS_ID_SPARK_EFFECT, bullet[i].pos_x, bullet[i].pos_y, true, 0, false, false, 1);
									bullet[i].on_target = true;
									GFX_SetDefaultAnimation(bullet[i].sprite_num, false, false, 3);
									break;
								case 2:// half colission
									break;
							}
							break;
						case ENTITY_ID_TARGET:
						case ENTITY_ID_BARREL:
							bullet[i].on_target = true;
							// Get object number
							obj_number = (bullet[i].hit_on & 0xFF00) >> 8;
							object[obj_number].is_hit = true;
							object[obj_number].hit_by = gfx_sprite_stack[bullet[i].sprite_num].entity_id;
							object[obj_number].damage += bullet[i].damage;
							GFX_SetDefaultAnimation(bullet[i].sprite_num, false, false, 3);

							break;
						case ENTITY_ID_ENEMY:
							switch (gfx_sprite_stack[bullet[i].sprite_num].entity_id) {
								case ENTITY_ID_ACTOR_BULLET:
									bullet[i].on_target = true;
									enemy_number = (bullet[i].hit_on & 0xFF00) >> 8;
									enemy[enemy_number].is_hit = true;
									enemy[enemy_number].hit_by = gfx_sprite_stack[bullet[i].sprite_num].entity_id;
									enemy[enemy_number].hit_vx_FP = bullet[i].vx_FP;
									enemy[enemy_number].hit_vy_FP = bullet[i].vy_FP;
									enemy[enemy_number].damage += bullet[i].damage;
									GFX_SetDefaultAnimation(bullet[i].sprite_num, false, false, 3);
									break;
								case ENTITY_ID_ACTOR_PUNCH:
									bullet[i].on_target = true;
									enemy_number = (bullet[i].hit_on & 0xFF00) >> 8;
									enemy[enemy_number].is_hit = true;
									enemy[enemy_number].hit_by = gfx_sprite_stack[bullet[i].sprite_num].entity_id;
									enemy[enemy_number].hit_vx_FP = bullet[i].vx_FP;
									enemy[enemy_number].hit_vy_FP = bullet[i].vy_FP;
									enemy[enemy_number].damage += bullet[i].damage;
									EFFECT_LoadEffect(ENTITY_ID_EMPTY, SPRITE_GRAPHICS_ID_PUNCH_EFFECT, bullet[i].pos_x, bullet[i].pos_y, false, false, false, (rand() % 3), 1);
									break;
								case ENTITY_ID_ACTOR_KICK:
									bullet[i].on_target = true;
									enemy_number = (bullet[i].hit_on & 0xFF00) >> 8;
									enemy[enemy_number].is_hit = true;
									enemy[enemy_number].hit_by = gfx_sprite_stack[bullet[i].sprite_num].entity_id;
									enemy[enemy_number].hit_vx_FP = bullet[i].vx_FP;
									enemy[enemy_number].hit_vy_FP = bullet[i].vy_FP;
									enemy[enemy_number].damage += bullet[i].damage;
									EFFECT_LoadEffect(ENTITY_ID_EMPTY, SPRITE_GRAPHICS_ID_PUNCH_EFFECT, bullet[i].pos_x, bullet[i].pos_y, false, (rand() % 3), false, false, 1);
									break;
								default:
									break;
							}
							break;
						case ENTITY_ID_ACTOR:
							if (gfx_sprite_stack[bullet[i].sprite_num].entity_id == ENTITY_ID_ENEMY_BULLET) {
								bullet[i].on_target = true;
								actor->is_hit = true;
								actor->hit_by = ENTITY_ID_ENEMY_BULLET;
								actor->damage += bullet[i].damage;
								GFX_SetDefaultAnimation(bullet[i].sprite_num, false, false, 3);
							}
							break;
						case ENTITY_ID_BOSS:
							if (gfx_sprite_stack[bullet[i].sprite_num].entity_id == ENTITY_ID_ACTOR_BULLET) {
								bullet[i].on_target = true;
								boss->is_hit = true;
								boss->hit_by = ENTITY_ID_ACTOR_BULLET;
								boss->damage += bullet[i].damage;
								GFX_SetDefaultAnimation(bullet[i].sprite_num, false, false, 3);
							}
							break;
						default:
							break;
					}

					bullet[i].current_step++;
					if (bullet[i].current_step >= bullet[i].steps) {
						bullet[i].on_target = true;
						GFX_SetDefaultAnimation(bullet[i].sprite_num, false, false, 3);
					}
				}
			}

			// On target
			if (bullet[i].on_target) {
				bullet[i].damage = 0;

				// Update sprite screen pos
				gfx_sprite_stack[bullet[i].sprite_num].screen_pos_x = bullet[i].pos_x - camera->pos_x;
				gfx_sprite_stack[bullet[i].sprite_num].screen_pos_y = bullet[i].pos_y - camera->pos_y;

				if (GFX_IsSpriteAnimationEnded(bullet[i].sprite_num, 0)) {
					bullet[i].on_target = false;
					bullet[i].shown = false;
					bullet[i].current_step = 0;
					BULLET_UnloadBullet(i);
				}
			}
		}
	}
}

void BULLET_UnloadBullets(void) {
	int i;
	for (i = 0; i < BULLET_MAX_BULLETS; i++) {
		if (bullet[i].loaded) {
			GFX_UnloadSprite(bullet[i].sprite_num);
			bullet[i].loaded = false;
			bullet[i].sprite_num = -1;
		}
	}
}