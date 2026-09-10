
#include "../engine.h"
#include "particle.h"

Particle *particle;

void PARTICLE_Init(void) {
	particle = MM_PushChunk(sizeof(Particle) * PARTICLE_MAX_PARTICLES, CT_ENGINE);
}

/** PARTICLE :: Draw colission pixels
 *  - Draws colission pixels just for debug
 */
void PARTICLE_DrawColissionPixels(Particle p) {
	//  Draw colission pixels
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[p.sprite_num].screen_pos_x + p.colission_area.points[0][0], gfx_sprite_stack[p.sprite_num].screen_pos_y + p.colission_area.points[0][1], 229);
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[p.sprite_num].screen_pos_x + p.colission_area.points[1][0], gfx_sprite_stack[p.sprite_num].screen_pos_y + p.colission_area.points[1][1], 229);
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[p.sprite_num].screen_pos_x + p.colission_area.points[2][0], gfx_sprite_stack[p.sprite_num].screen_pos_y + p.colission_area.points[2][1], 229);
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[p.sprite_num].screen_pos_x + p.colission_area.points[3][0], gfx_sprite_stack[p.sprite_num].screen_pos_y + p.colission_area.points[3][1], 229);
}

/** ACTOR :: Initialize bullet
 */
void PARTICLE_LoadParticle(int graphics_id, int entity_id, int source_x, int source_y, int target_x, int target_y, int speed, int damage, int range_x, int range_y) {
	int i;
	int number;
	int dx, dy;
	int distance;
	int sprite_slot;

	// Search free particle slot
	number = 0;
	for (i = 0; i < PARTICLE_MAX_PARTICLES; i++) {
		if (!particle[i].loaded) {
			number = i;
			break;
		}
	}

	// Check if max bullets is reached
	if (number == PARTICLE_MAX_PARTICLES - 1) {
		Error("PARTICLE_LoadParticle function error", "Max number of bullets", "", ERROR_SYSTEM);
	}

	// Calculate target
	particle[number].target_x = target_x;
	particle[number].target_y = target_y;

	particle[number].pos_x = source_x;
	particle[number].pos_y = source_y;
	particle[number].speed = speed;

	particle[number].damage = damage;

	// calculate distance from source
	dx = particle[number].target_x - particle[number].pos_x;
	dy = particle[number].target_y - particle[number].pos_y;

	// calculate steps
	if (abs(dx) > abs(dy))
		//particle[number].steps = abs(dx);
		distance = abs(dx);
	else
		//particle[number].steps = abs(dy);
		distance = abs(dy);

	if (distance == 0) distance = 1;

	// calculate fixed point velocity and position
	particle[number].vx_FP = (dx << FP) / distance;
	particle[number].vy_FP = (dy << FP) / distance;

	particle[number].x_FP = particle[number].pos_x << FP;
	particle[number].y_FP = particle[number].pos_y << FP;

	particle[number].steps = distance;

	particle[number].graphics_id = graphics_id;

	sprite_slot = GFX_FindEmptySpriteSlot();
	if (sprite_slot == -1) {
		sprintf(engine.system_error_message1, "PARTICLE_LoadParticle function error");
		sprintf(engine.system_error_message2, "No empty sprite slot available");
		sprintf(engine.system_error_message3, " ");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
	} else {
		particle[number].sprite_num = sprite_slot;
		GFX_InitSprite(entity_id, number, sprite_slot, 0, gfx_sprite_graphics_stack[graphics_id].width_px, gfx_sprite_graphics_stack[graphics_id].height_px);
	}

	GFX_SetSpriteGraphic(particle[number].sprite_num, 0, graphics_id, 0, 0);
	//GFX_SetDefaultAnimation(particle[number].sprite_num, false, false, speed);
	GFX_SetSingleFrameAnimation(particle[number].sprite_num, 0);

	// Set colission points
	//  ---------------
	// |               |
	// |  0         1  |
	// |               |
	// |               |
	// |  2         3  |
	//  ---------------
	// Point 1
	particle[number].colission_area.points[0][0] = 0 - range_x;
	particle[number].colission_area.points[0][1] = (gfx_sprite_stack[particle[number].sprite_num].height_px >> 1) - range_y;

	// Point 2
	particle[number].colission_area.points[1][0] = gfx_sprite_stack[particle[number].sprite_num].width_px + range_x;
	particle[number].colission_area.points[1][1] = (gfx_sprite_stack[particle[number].sprite_num].height_px >> 1) - range_y;

	// Point 3
	particle[number].colission_area.points[2][0] = 0 - range_x;
	particle[number].colission_area.points[2][1] = gfx_sprite_stack[particle[number].sprite_num].height_px + range_y;

	// Point 4
	particle[number].colission_area.points[3][0] = gfx_sprite_stack[particle[number].sprite_num].width_px + range_x;
	particle[number].colission_area.points[3][1] = gfx_sprite_stack[particle[number].sprite_num].height_px + range_y;

	gfx_sprite_stack[particle[number].sprite_num].unmasked = true;

	gfx_sprite_stack[particle[number].sprite_num].screen_pos_x = particle[number].pos_x - camera->pos_x;
	gfx_sprite_stack[particle[number].sprite_num].screen_pos_y = particle[number].pos_y - camera->pos_y;

	particle[number].hit_on = 0;
	particle[number].on_target = false;
	particle[number].loaded = true;
	particle[number].shown = true;
}

int PARTICLE_CheckParticleColission(Particle p) {
	int i;
	int point1_x, point1_y;
	int point2_x, point2_y;
	int point3_x, point3_y;
	int point4_x, point4_y;
	bool collision_detected;

	point1_x = p.pos_x + p.colission_area.points[0][0];
	point1_y = p.pos_y + p.colission_area.points[0][1];
	point2_x = p.pos_x + p.colission_area.points[1][0];
	point2_y = p.pos_y + p.colission_area.points[1][1];
	point3_x = p.pos_x + p.colission_area.points[2][0];
	point3_y = p.pos_y + p.colission_area.points[2][1];
	point4_x = p.pos_x + p.colission_area.points[3][0];
	point4_y = p.pos_y + p.colission_area.points[3][1];

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
		if (enemy[i].is_loaded && !enemy[i].action_dead) {
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
	if (!actor->action_dead) {
		if (point1_x > actor->pos_x + actor->hit_area.points[1][0]) collision_detected = false;// Out of left side
		if (point2_x < actor->pos_x + actor->hit_area.points[0][0]) collision_detected = false;//  Out of right side
		if (point3_y < actor->pos_y + actor->hit_area.points[0][1]) collision_detected = false;//  Out of top side
		if (point1_y > actor->pos_y + actor->hit_area.points[2][1]) collision_detected = false;//  Out of bottom side
		if (collision_detected) return gfx_sprite_stack[actor->sprite_num].id;
	}

	/////// BOSS COLISSIONS /////////////
	collision_detected = true;
	if (!boss->action_dead) {
		if (point1_x > boss->pos_x + boss->hit_area.points[1][0]) collision_detected = false;// Out of left side
		if (point2_x < boss->pos_x + boss->hit_area.points[0][0]) collision_detected = false;//  Out of right side
		if (point3_y < boss->pos_y + boss->hit_area.points[0][1]) collision_detected = false;//  Out of top side
		if (point1_y > boss->pos_y + boss->hit_area.points[2][1]) collision_detected = false;//  Out of bottom side
		if (collision_detected) return gfx_sprite_stack[boss->sprite_num].id;
	}

	/////// BACKGROUND COLISSIONS ///////////
	if (MAP_CheckColissionTile(point1_x, point1_y) == 1) { return ENTITY_ID_BACKGROUND; }// Check point 1
	if (MAP_CheckColissionTile(point2_x, point2_y) == 1) { return ENTITY_ID_BACKGROUND; }// Check point 2
	if (MAP_CheckColissionTile(point3_x, point3_y) == 1) { return ENTITY_ID_BACKGROUND; }// Check point 3
	if (MAP_CheckColissionTile(point4_x, point4_y) == 1) { return ENTITY_ID_BACKGROUND; }// Check point 4


	return false;
}

void PARTICLE_SetExplosionDamage(Particle p) {
	int i;
	int point1_x, point1_y;
	int point2_x, point2_y;
	int point3_x, point3_y;
	int point4_x, point4_y;
	bool collision_detected;

	point1_x = p.pos_x + p.colission_area.points[0][0];
	point1_y = p.pos_y + p.colission_area.points[0][1];
	point2_x = p.pos_x + p.colission_area.points[1][0];
	point2_y = p.pos_y + p.colission_area.points[1][1];
	point3_x = p.pos_x + p.colission_area.points[2][0];
	point3_y = p.pos_y + p.colission_area.points[2][1];
	point4_x = p.pos_x + p.colission_area.points[3][0];
	point4_y = p.pos_y + p.colission_area.points[3][1];

	/////// OBJECTS COLISSIONS ///////////
	for (i = 0; i < OBJECT_MAX_OBJECTS; i++) {
		collision_detected = true;
		if (object[i].is_loaded) {
			if (gfx_sprite_stack[object[i].num_sprite].shown) {
				if (point1_x > object[i].pos_x + object[i].hit_area.points[1][0]) collision_detected = false;// Out of left side
				if (point2_x < object[i].pos_x + object[i].hit_area.points[0][0]) collision_detected = false;//  Out of right side
				if (point3_y < object[i].pos_y + object[i].hit_area.points[0][1]) collision_detected = false;//  Out of top side
				if (point1_y > object[i].pos_y + object[i].hit_area.points[2][1]) collision_detected = false;//  Out of bottom side
				if (collision_detected) {
					// Set damage to object
					object[i].is_hit = true;
					object[i].hit_by = ENTITY_ID_EXPLOSION;
					object[i].damage = p.damage;
				}
			}
		}
	}

	/////// ENEMIES COLISSIONS ///////////
	for (i = 0; i < ENEMY_MAX_ENEMIES; i++) {
		collision_detected = true;
		if (enemy[i].is_loaded && !enemy[i].action_dead) {
			if (gfx_sprite_stack[enemy[i].sprite_num].shown) {
				if (point1_x > enemy[i].pos_x + enemy[i].hit_area.points[1][0]) collision_detected = false;// Out of left side
				if (point2_x < enemy[i].pos_x + enemy[i].hit_area.points[0][0]) collision_detected = false;//  Out of right side
				if (point3_y < enemy[i].pos_y + enemy[i].hit_area.points[0][1]) collision_detected = false;//  Out of top side
				if (point1_y > enemy[i].pos_y + enemy[i].hit_area.points[2][1]) collision_detected = false;//  Out of bottom side
				if (collision_detected) {
					// Set damage to object
					enemy[i].is_hit = true;
					enemy[i].hit_by = ENTITY_ID_EXPLOSION;
					enemy[i].damage = p.damage;
				}
			}
		}
	}

	/////// ACTOR COLISSIONS /////////////
	collision_detected = true;
	if (!actor->action_dead) {
		if (point1_x > actor->pos_x + actor->hit_area.points[1][0]) collision_detected = false;// Out of left side
		if (point2_x < actor->pos_x + actor->hit_area.points[0][0]) collision_detected = false;//  Out of right side
		if (point3_y < actor->pos_y + actor->hit_area.points[0][1]) collision_detected = false;//  Out of top side
		if (point1_y > actor->pos_y + actor->hit_area.points[2][1]) collision_detected = false;//  Out of bottom side
		if (collision_detected) {
			actor->is_hit = true;
			actor->hit_by = ENTITY_ID_EXPLOSION;
			actor->damage = p.damage;
		}
	}

	/////// BOSS COLISSIONS /////////////
	collision_detected = true;
	if (!boss->action_dead) {
		if (point1_x > boss->pos_x + boss->hit_area.points[1][0]) collision_detected = false;// Out of left side
		if (point2_x < boss->pos_x + boss->hit_area.points[0][0]) collision_detected = false;//  Out of right side
		if (point3_y < boss->pos_y + boss->hit_area.points[0][1]) collision_detected = false;//  Out of top side
		if (point1_y > boss->pos_y + boss->hit_area.points[2][1]) collision_detected = false;//  Out of bottom side
		if (collision_detected) {
			boss->is_hit = true;
			boss->hit_by = ENTITY_ID_EXPLOSION;
			boss->damage = p.damage;
		}
	}
}

void PARTICLE_UnloadParticles(void) {
	int i;
	for (i = 0; i < PARTICLE_MAX_PARTICLES; i++) {
		if (particle[i].loaded) {
			GFX_UnloadSprite(particle[i].sprite_num);
			particle[i].loaded = false;
			particle[i].sprite_num = -1;
		}
	}
}

void PARTICLE_UnloadParticle(int number) {

	if (particle[number].loaded) {
		GFX_UnloadSprite(particle[number].sprite_num);
		particle[number].loaded = false;
		particle[number].sprite_num = -1;
	}
}

/** ACTOR :: Update bullets	
 */
void PARTICLE_UpdateParticles(void) {
	int i;

	for (i = 0; i < PARTICLE_MAX_PARTICLES; i++) {
		if (particle[i].loaded) {

			// debug
			//PARTICLE_DrawColissionPixels(particle[i]);

			// Not on target
			if (!particle[i].on_target) {
				// Calculate new pos
				particle[i].x_FP += particle[i].vx_FP;
				particle[i].pos_x = (particle[i].x_FP >> FP);
				particle[i].y_FP += particle[i].vy_FP;
				particle[i].pos_y = (particle[i].y_FP >> FP);

				if (PARTICLE_CheckParticleColission(particle[i]) == ENTITY_ID_BACKGROUND) {
					particle[i].x_FP -= particle[i].vx_FP;
					particle[i].pos_x = (particle[i].x_FP >> FP);
					particle[i].y_FP -= particle[i].vy_FP;
					particle[i].pos_y = (particle[i].y_FP >> FP);
				}

				// end of travel
				particle[i].current_step++;
				if (particle[i].current_step >= particle[i].steps) {
					particle[i].on_target = true;
					GFX_SetDefaultAnimation(particle[i].sprite_num, false, false, particle[i].speed);

					if (gfx_sprite_stack[particle[i].sprite_num].entity_id == ENTITY_ID_EXPLOSION) {
						// Set damage to all characters arround and set damage to 0.
						PARTICLE_SetExplosionDamage(particle[i]);
					}
				}
			}

			// Update particle screen position
			GFX_SetSpritePosition(particle[i].sprite_num, particle[i].pos_x - camera->pos_x, particle[i].pos_y - camera->pos_y);

			// Wait for animation ending
			if (particle[i].on_target) {
				if (GFX_IsSpriteAnimationEnded(particle[i].sprite_num, 0)) {
					PARTICLE_UnloadParticle(i);
				}
			}
		}
	}
}
