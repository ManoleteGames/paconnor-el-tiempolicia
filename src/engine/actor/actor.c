#include "../engine.h"
#include "actor.h"

Actor *actor;

void ACTOR_Init(void) {
	actor = MM_PushChunk(sizeof(Actor), CT_ENGINE);

	actor->gun = MM_PushChunk(sizeof(Gun), CT_ENGINE);

	actor->feet_animation = MM_PushChunk(sizeof(SpriteAnimation) * 160, CT_ENGINE);
	actor->body_animation = MM_PushChunk(sizeof(SpriteAnimation) * 160, CT_ENGINE);
	actor->head_animation = MM_PushChunk(sizeof(SpriteAnimation) * 160, CT_ENGINE);
	actor->left_arm_animation = MM_PushChunk(sizeof(SpriteAnimation) * 160, CT_ENGINE);
	actor->right_arm_animation = MM_PushChunk(sizeof(SpriteAnimation) * 160, CT_ENGINE);
}

/** ACTOR :: Set current equiped gun
 *  - List of available guns
 *    * Bare hands. Equiped as a gun but max distance limited to 1 and transparent bullet
 *    * Pistol
 *    * Shootgun
 *    * Uzi
 *    * Snipper
 */
void ACTOR_SetGun(int actor_spr_num, int type) {

	// Set gun type
	actor->gun->type = type;

	// Set data by type
	switch (type) {
		case ACTOR_GUN_TYPE_BARE_HANDS:
			// Set graphics
			actor->gun->graphics_id = SPRITE_GRAPHICS_ID_GUN1;
			actor->gun->effect_graphics_id = SPRITE_GRAPHICS_ID_PUNCH_EFFECT;
			actor->gun->bullet_graphics_id = SPRITE_GRAPHICS_ID_PUNCH;
			GFX_SetSpriteGraphicsId(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, SPRITE_GRAPHICS_ID_ACTOR_RARM_BARE_HANDS);

			actor->gun->max_accuracy = 4;
			actor->gun->damage = 10;
			actor->gun->recoil_time = 3;
			actor->gun->max_distance = 6;
			actor->gun->accurate = false;
			actor->gun->shoots = 1;
			actor->gun->bullet_speed = 1;
			break;
		case ACTOR_GUN_TYPE_PISTOL:
			// Set graphics id
			actor->gun->graphics_id = SPRITE_GRAPHICS_ID_GUN2;
			actor->gun->effect_graphics_id = SPRITE_GRAPHICS_ID_PISTOL_EFFECT;
			actor->gun->bullet_graphics_id = SPRITE_GRAPHICS_ID_BULLET1;
			GFX_SetSpriteGraphicsId(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, SPRITE_GRAPHICS_ID_ACTOR_RARM_PISTOL);

			actor->gun->max_accuracy = 4;
			actor->gun->damage = 20;
			actor->gun->recoil_time = 10;
			actor->gun->max_distance = 150;
			actor->gun->accurate = false;
			actor->gun->shoots = 1;
			actor->gun->bullet_speed = 15;
			break;
		case ACTOR_GUN_TYPE_SHOTGUN:
			// Set graphics id
			actor->gun->graphics_id = SPRITE_GRAPHICS_ID_GUN3;
			actor->gun->effect_graphics_id = SPRITE_GRAPHICS_ID_SHOTGUN_EFFECT;
			actor->gun->bullet_graphics_id = SPRITE_GRAPHICS_ID_BULLET2;
			GFX_SetSpriteGraphicsId(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, SPRITE_GRAPHICS_ID_ACTOR_RARM_SHOTGUN);

			actor->gun->max_accuracy = 1;
			actor->gun->damage = 40;
			actor->gun->recoil_time = 10;
			actor->gun->max_distance = 100;
			actor->gun->accurate = false;
			actor->gun->shoots = 3;
			actor->gun->bullet_speed = 15;
			break;
		case ACTOR_GUN_TYPE_UZI:
			// Set graphics id
			actor->gun->graphics_id = SPRITE_GRAPHICS_ID_GUN4;
			actor->gun->effect_graphics_id = SPRITE_GRAPHICS_ID_PISTOL_EFFECT;
			actor->gun->bullet_graphics_id = SPRITE_GRAPHICS_ID_BULLET1;
			GFX_SetSpriteGraphicsId(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, SPRITE_GRAPHICS_ID_ACTOR_RARM_UZI);

			actor->gun->max_accuracy = 1;
			actor->gun->damage = 20;
			actor->gun->recoil_time = 2;
			actor->gun->max_distance = 150;
			actor->gun->accurate = false;
			actor->gun->shoots = 1;
			actor->gun->bullet_speed = 20;
			break;

		case ACTOR_GUN_TYPE_SNIPPER:
			// Set graphics id
			actor->gun->graphics_id = SPRITE_GRAPHICS_ID_GUN5;
			actor->gun->effect_graphics_id = SPRITE_GRAPHICS_ID_SNIPPER_EFFECT;
			actor->gun->bullet_graphics_id = SPRITE_GRAPHICS_ID_BULLET1;
			GFX_SetSpriteGraphicsId(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, SPRITE_GRAPHICS_ID_ACTOR_RARM_RIFLE);

			actor->gun->max_accuracy = 4;
			actor->gun->damage = 30;
			actor->gun->recoil_time = 10;
			actor->gun->max_distance = 200;
			actor->gun->accurate = true;
			actor->gun->shoots = 1;
			actor->gun->bullet_speed = 20;
			break;
		default:
			sprintf(engine.system_error_message1, "ACTOR_SetGun function error");
			sprintf(engine.system_error_message2, "Undefined gun type %u ", type);
			sprintf(engine.system_error_message3, " ");
			Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_SYSTEM);
			break;
	}
}

/** ACTOR :: Set loop animation values
 */
void ACTOR_SetLoopAnimation(int movement) {
	switch (movement) {
		case 1:// loop right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_LOOP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_LOOP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_LOOP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_LOOP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_LOOP_RIGHT], true);
			break;
		case 2:// loop left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_LOOP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_LOOP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_LOOP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_LOOP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_LOOP_LEFT], true);
			break;
		case 4:// loop down
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_LOOP_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_LOOP_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_LOOP_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_LOOP_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_LOOP_DOWN], true);
			break;
		case 5:// loop down-right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_LOOP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_LOOP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_LOOP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_LOOP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_LOOP_RIGHT], true);
			break;
		case 6:// loop down-left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_LOOP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_LOOP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_LOOP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_LOOP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_LOOP_LEFT], true);
			break;
		case 8:// loop up
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_LOOP_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_LOOP_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_LOOP_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_LOOP_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_LOOP_UP], true);
			break;
		case 9:// loop up-right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_LOOP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_LOOP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_LOOP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_LOOP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_LOOP_RIGHT], true);
			break;
		case 10:// loop up-left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_LOOP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_LOOP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_LOOP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_LOOP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_LOOP_LEFT], true);
			break;
	}
}

/** ACTOR :: Set full walk animation values
 */
void ACTOR_SetFullWalkAnimation(int facing) {
	switch (facing) {
		case 1:// facing right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_WALK_RIGHT], false);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_WALK_RIGHT], false);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_WALK_RIGHT], false);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_WALK_RIGHT], false);
			if (actor->gun->type == ACTOR_GUN_TYPE_BARE_HANDS) GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_WALK_RIGHT], false);
			else
				GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_WALK_GUN_RIGHT], false);

			GFX_SetSpriteGfxOrder(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, ACTOR_ANIM_LARM_INDEX, ACTOR_ANIM_BODY_INDEX, ACTOR_ANIM_HEAD_INDEX, ACTOR_ANIM_RARM_INDEX);
			break;
		case 2:// facing left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_WALK_LEFT], false);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_WALK_LEFT], false);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_WALK_LEFT], false);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_WALK_LEFT], false);
			if (actor->gun->type == ACTOR_GUN_TYPE_BARE_HANDS) GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_WALK_LEFT], false);
			else
				GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_WALK_GUN_LEFT], false);

			GFX_SetSpriteGfxOrder(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, ACTOR_ANIM_RARM_INDEX, ACTOR_ANIM_BODY_INDEX, ACTOR_ANIM_HEAD_INDEX, ACTOR_ANIM_LARM_INDEX);
			break;
		case 4:// facing down
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_WALK_DOWN], false);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_WALK_DOWN], false);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_WALK_DOWN], false);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_WALK_DOWN], false);
			if (actor->gun->type == ACTOR_GUN_TYPE_BARE_HANDS) GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_WALK_DOWN], false);
			else
				GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_WALK_GUN_DOWN], false);
			break;
		case 5:// facing down-right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_WALK_DOWN_RIGHT], false);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_WALK_DOWN_RIGHT], false);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_WALK_DOWN_RIGHT], false);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_WALK_DOWN_RIGHT], false);
			if (actor->gun->type == ACTOR_GUN_TYPE_BARE_HANDS) GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_WALK_DOWN_RIGHT], false);
			else
				GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_WALK_GUN_DOWN_RIGHT], false);

			GFX_SetSpriteGfxOrder(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, ACTOR_ANIM_LARM_INDEX, ACTOR_ANIM_BODY_INDEX, ACTOR_ANIM_HEAD_INDEX, ACTOR_ANIM_RARM_INDEX);
			break;
		case 6:// facing down-left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_WALK_DOWN_LEFT], false);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_WALK_DOWN_LEFT], false);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_WALK_DOWN_LEFT], false);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_WALK_DOWN_LEFT], false);
			if (actor->gun->type == ACTOR_GUN_TYPE_BARE_HANDS) GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_WALK_DOWN_LEFT], false);
			else
				GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_WALK_GUN_DOWN_LEFT], false);

			GFX_SetSpriteGfxOrder(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, ACTOR_ANIM_RARM_INDEX, ACTOR_ANIM_BODY_INDEX, ACTOR_ANIM_HEAD_INDEX, ACTOR_ANIM_LARM_INDEX);
			break;
		case 8:// facing up
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_WALK_UP], false);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_WALK_UP], false);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_WALK_UP], false);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_WALK_UP], false);
			if (actor->gun->type == ACTOR_GUN_TYPE_BARE_HANDS) GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_WALK_UP], false);
			else
				GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_WALK_GUN_UP], false);
			break;
		case 9:// facing up-right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_WALK_UP_RIGHT], false);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_WALK_UP_RIGHT], false);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_WALK_UP_RIGHT], false);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_WALK_UP_RIGHT], false);
			if (actor->gun->type == ACTOR_GUN_TYPE_BARE_HANDS) GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_WALK_UP_RIGHT], false);
			else
				GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_WALK_GUN_UP_RIGHT], false);

			GFX_SetSpriteGfxOrder(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, ACTOR_ANIM_LARM_INDEX, ACTOR_ANIM_BODY_INDEX, ACTOR_ANIM_HEAD_INDEX, ACTOR_ANIM_RARM_INDEX);
			break;
		case 10:// facing up-left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_WALK_UP_LEFT], false);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_WALK_UP_LEFT], false);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_WALK_UP_LEFT], false);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_WALK_UP_LEFT], false);
			if (actor->gun->type == ACTOR_GUN_TYPE_BARE_HANDS) GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_WALK_UP_LEFT], false);
			else
				GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_WALK_GUN_UP_LEFT], false);

			GFX_SetSpriteGfxOrder(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, ACTOR_ANIM_RARM_INDEX, ACTOR_ANIM_BODY_INDEX, ACTOR_ANIM_HEAD_INDEX, ACTOR_ANIM_LARM_INDEX);
			break;
		default:
			break;
	}
}

/** ACTOR :: Set feet walk animation values
 */
void ACTOR_SetFeetWalkAnimation(int facing) {
	switch (facing) {
		case 1:// facing right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_WALK_RIGHT], false);
			break;
		case 2:// facing left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_WALK_LEFT], false);
			break;
		case 4:// facing down
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_WALK_DOWN], false);
			break;
		case 5:// facing down-right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_WALK_DOWN_RIGHT], false);
			break;
		case 6:// facing down-left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_WALK_DOWN_LEFT], false);
			break;
		case 8:// facing up
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_WALK_UP], false);
			break;
		case 9:// facing up-right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_WALK_UP_RIGHT], false);
			break;
		case 10:// facing up-left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_WALK_UP_LEFT], false);
			break;
		default:
			break;
	}
}

/** ACTOR :: Set point animation values
 */
void ACTOR_SetPointAnimation(int facing) {
	switch (facing) {
		case 1:// facing right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_POINT_GUN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_POINT_GUN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_POINT_GUN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_POINT_GUN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_POINT_GUN_RIGHT], true);
			break;
		case 2:// facing left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_POINT_GUN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_POINT_GUN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_POINT_GUN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_POINT_GUN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_POINT_GUN_LEFT], true);
			break;
		case 4:// facing down
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_POINT_GUN_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_POINT_GUN_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_POINT_GUN_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_POINT_GUN_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_POINT_GUN_DOWN], true);
			break;
		case 5:// facing down-right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_POINT_GUN_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_POINT_GUN_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_POINT_GUN_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_POINT_GUN_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_POINT_GUN_DOWN_RIGHT], true);
			break;
		case 6:// facing down-left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_POINT_GUN_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_POINT_GUN_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_POINT_GUN_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_POINT_GUN_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_POINT_GUN_DOWN_LEFT], true);
			break;
		case 8:// facing up
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_POINT_GUN_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_POINT_GUN_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_POINT_GUN_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_POINT_GUN_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_POINT_GUN_UP], true);
			break;
		case 9:// facing up-right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_POINT_GUN_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_POINT_GUN_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_POINT_GUN_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_POINT_GUN_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_POINT_GUN_UP_RIGHT], true);
			break;
		case 10:// facing up-left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_POINT_GUN_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_POINT_GUN_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_POINT_GUN_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_POINT_GUN_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_POINT_GUN_UP_LEFT], true);
			break;
	}
}

/** ACTOR :: Set shoot animation values
 */
void ACTOR_SetShotAnimation(int facing) {
	switch (facing) {
		case 1:// facing right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_SHOT_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_SHOT_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_SHOT_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_SHOT_RIGHT], true);
			GFX_SetSpriteGfxOrder(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, ACTOR_ANIM_LARM_INDEX, ACTOR_ANIM_BODY_INDEX, ACTOR_ANIM_HEAD_INDEX, ACTOR_ANIM_RARM_INDEX);
			break;
		case 2:// facing left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_SHOT_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_SHOT_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_SHOT_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_SHOT_LEFT], true);
			GFX_SetSpriteGfxOrder(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, ACTOR_ANIM_RARM_INDEX, ACTOR_ANIM_BODY_INDEX, ACTOR_ANIM_HEAD_INDEX, ACTOR_ANIM_LARM_INDEX);
			break;
		case 4:// facing down
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_SHOT_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_SHOT_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_SHOT_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_SHOT_DOWN], true);
			GFX_SetSpriteGfxOrder(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, ACTOR_ANIM_LARM_INDEX, ACTOR_ANIM_BODY_INDEX, ACTOR_ANIM_HEAD_INDEX, ACTOR_ANIM_RARM_INDEX);
			break;
		case 5:// facing down-right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_SHOT_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_SHOT_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_SHOT_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_SHOT_DOWN_RIGHT], true);
			GFX_SetSpriteGfxOrder(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, ACTOR_ANIM_LARM_INDEX, ACTOR_ANIM_BODY_INDEX, ACTOR_ANIM_HEAD_INDEX, ACTOR_ANIM_RARM_INDEX);
			break;
		case 6:// facing down-left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_SHOT_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_SHOT_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_SHOT_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_SHOT_DOWN_LEFT], true);
			GFX_SetSpriteGfxOrder(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, ACTOR_ANIM_RARM_INDEX, ACTOR_ANIM_BODY_INDEX, ACTOR_ANIM_HEAD_INDEX, ACTOR_ANIM_LARM_INDEX);
			break;
		case 8:// facing up
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_SHOT_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_SHOT_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_SHOT_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_SHOT_UP], true);
			GFX_SetSpriteGfxOrder(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, ACTOR_ANIM_LARM_INDEX, ACTOR_ANIM_BODY_INDEX, ACTOR_ANIM_HEAD_INDEX, ACTOR_ANIM_RARM_INDEX);
			break;
		case 9:// facing up-right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_SHOT_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_SHOT_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_SHOT_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_SHOT_UP_RIGHT], true);
			GFX_SetSpriteGfxOrder(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, ACTOR_ANIM_LARM_INDEX, ACTOR_ANIM_BODY_INDEX, ACTOR_ANIM_HEAD_INDEX, ACTOR_ANIM_RARM_INDEX);
			break;
		case 10:// facing up-left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_SHOT_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_SHOT_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_SHOT_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_SHOT_UP_LEFT], true);
			GFX_SetSpriteGfxOrder(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, ACTOR_ANIM_RARM_INDEX, ACTOR_ANIM_BODY_INDEX, ACTOR_ANIM_HEAD_INDEX, ACTOR_ANIM_LARM_INDEX);
			break;
	}
}

/** ACTOR :: Set punch 1 animation values
 */
void ACTOR_SetPunch1Animation(int facing) {
	switch (facing) {
		case 1:// facing right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_PUNCH1_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_PUNCH1_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_PUNCH1_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_PUNCH1_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_PUNCH1_RIGHT], true);
			break;
		case 2:// facing left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_PUNCH1_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_PUNCH1_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_PUNCH1_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_PUNCH1_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_PUNCH1_LEFT], true);
			break;
		case 4:// facing down
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_PUNCH1_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_PUNCH1_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_PUNCH1_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_PUNCH1_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_PUNCH1_DOWN], true);
			break;
		case 5:// facing down-right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_PUNCH1_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_PUNCH1_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_PUNCH1_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_PUNCH1_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_PUNCH1_DOWN_RIGHT], true);
			break;
		case 6:// facing down-left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_PUNCH1_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_PUNCH1_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_PUNCH1_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_PUNCH1_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_PUNCH1_DOWN_LEFT], true);
			break;
		case 8:// facing up
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_PUNCH1_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_PUNCH1_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_PUNCH1_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_PUNCH1_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_PUNCH1_UP], true);
			break;
		case 9:// facing up-right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_PUNCH1_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_PUNCH1_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_PUNCH1_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_PUNCH1_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_PUNCH1_UP_RIGHT], true);
			break;
		case 10:// facing up-left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_PUNCH1_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_PUNCH1_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_PUNCH1_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_PUNCH1_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_PUNCH1_UP_LEFT], true);
			break;
	}
	AUDIO_PlaySound(AUDIO_PUNCH_EFFECT, 0);
}

/** ACTOR :: Set punch 2 animation values
 */
void ACTOR_SetPunch2Animation(int facing) {
	switch (facing) {
		case 1:// facing right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_PUNCH2_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_PUNCH2_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_PUNCH2_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_PUNCH2_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_PUNCH2_RIGHT], true);
			break;
		case 2:// facing left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_PUNCH2_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_PUNCH2_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_PUNCH2_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_PUNCH2_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_PUNCH2_LEFT], true);
			break;
		case 4:// facing down
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_PUNCH2_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_PUNCH2_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_PUNCH2_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_PUNCH2_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_PUNCH2_DOWN], true);
			break;
		case 5:// facing down-right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_PUNCH2_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_PUNCH2_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_PUNCH2_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_PUNCH2_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_PUNCH2_DOWN_RIGHT], true);
			break;
		case 6:// facing down-left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_PUNCH2_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_PUNCH2_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_PUNCH2_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_PUNCH2_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_PUNCH2_DOWN_LEFT], true);
			break;
		case 8:// facing up
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_PUNCH2_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_PUNCH2_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_PUNCH2_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_PUNCH2_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_PUNCH2_UP], true);
			break;
		case 9:// facing up-right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_PUNCH2_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_PUNCH2_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_PUNCH2_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_PUNCH2_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_PUNCH2_UP_RIGHT], true);
			break;
		case 10:// facing up-left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_PUNCH2_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_PUNCH2_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_PUNCH2_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_PUNCH2_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_PUNCH2_UP_LEFT], true);
			break;
	}
	AUDIO_PlaySound(AUDIO_PUNCH_EFFECT, 0);
}

/** ACTOR :: Set kick animation values
 */
void ACTOR_SetKickAnimation(int facing) {
	switch (facing) {
		case 1:// facing right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_KICK_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_KICK_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_KICK_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_KICK_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_KICK_RIGHT], true);
			break;
		case 2:// facing left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_KICK_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_KICK_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_KICK_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_KICK_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_KICK_LEFT], true);
			break;
		case 4:// facing down
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_KICK_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_KICK_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_KICK_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_KICK_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_KICK_DOWN], true);
			break;
		case 5:// facing down-right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_KICK_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_KICK_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_KICK_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_KICK_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_KICK_DOWN_RIGHT], true);
			break;
		case 6:// facing down-left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_KICK_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_KICK_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_KICK_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_KICK_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_KICK_DOWN_LEFT], true);
			break;
		case 8:// facing up
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_KICK_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_KICK_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_KICK_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_KICK_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_KICK_UP], true);
			break;
		case 9:// facing up-right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_KICK_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_KICK_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_KICK_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_KICK_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_KICK_UP_RIGHT], true);
			break;
		case 10:// facing up-left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_KICK_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_KICK_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_KICK_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_KICK_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_KICK_UP_LEFT], true);
			break;
	}
	AUDIO_PlaySound(AUDIO_KICK_EFFECT, 0);
}

/** ACTOR :: Set fight animation values
 */
void ACTOR_SetFightAnimation(int facing) {
	switch (facing) {
		case 1:// facing right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_FIGHT_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_FIGHT_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_FIGHT_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_FIGHT_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_FIGHT_RIGHT], true);
			break;
		case 2:// facing left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_FIGHT_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_FIGHT_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_FIGHT_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_FIGHT_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_FIGHT_LEFT], true);
			break;
		case 4:// moving down
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_FIGHT_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_FIGHT_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_FIGHT_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_FIGHT_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_FIGHT_DOWN], true);
			break;
		case 5:// moving down-right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_FIGHT_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_FIGHT_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_FIGHT_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_FIGHT_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_FIGHT_DOWN_RIGHT], true);
			break;
		case 6:// moving down-left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_FIGHT_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_FIGHT_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_FIGHT_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_FIGHT_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_FIGHT_DOWN_LEFT], true);
			break;
		case 8:// moving up
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_FIGHT_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_FIGHT_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_FIGHT_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_FIGHT_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_FIGHT_UP], true);
			break;
		case 9:// moving up-right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_FIGHT_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_FIGHT_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_FIGHT_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_FIGHT_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_FIGHT_UP_RIGHT], true);
			break;
		case 10:// moving up-left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_FIGHT_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_FIGHT_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_FIGHT_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_FIGHT_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_FIGHT_UP_LEFT], true);
			break;
		default:
			break;
	}
}

/** ACTOR :: Set stand animation values
 */
void ACTOR_SetFullStandAnimation(int facing) {
	switch (facing) {
		case 1:// facing right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_STAND_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_STAND_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_STAND_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_STAND_RIGHT], true);
			if (actor->gun->type == ACTOR_GUN_TYPE_BARE_HANDS) GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_STAND_RIGHT], true);
			else
				GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_STAND_GUN_RIGHT], true);
			GFX_SetSpriteGfxOrder(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, ACTOR_ANIM_LARM_INDEX, ACTOR_ANIM_BODY_INDEX, ACTOR_ANIM_HEAD_INDEX, ACTOR_ANIM_RARM_INDEX);
			break;
		case 2:// facing left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_STAND_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_STAND_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_STAND_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_STAND_LEFT], true);
			if (actor->gun->type == ACTOR_GUN_TYPE_BARE_HANDS) GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_STAND_LEFT], true);
			else
				GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_STAND_GUN_LEFT], true);
			GFX_SetSpriteGfxOrder(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, ACTOR_ANIM_RARM_INDEX, ACTOR_ANIM_BODY_INDEX, ACTOR_ANIM_HEAD_INDEX, ACTOR_ANIM_LARM_INDEX);
			break;
		case 4:// facing down
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_STAND_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_STAND_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_STAND_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_STAND_DOWN], true);
			if (actor->gun->type == ACTOR_GUN_TYPE_BARE_HANDS) GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_STAND_DOWN], true);
			else
				GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_STAND_GUN_DOWN], true);
			break;
		case 5:// facing down-right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[4], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[4], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[4], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[4], true);
			if (actor->gun->type == ACTOR_GUN_TYPE_BARE_HANDS) GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_STAND_DOWN_RIGHT], true);
			else
				GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_STAND_GUN_DOWN_RIGHT], true);
			GFX_SetSpriteGfxOrder(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, ACTOR_ANIM_LARM_INDEX, ACTOR_ANIM_BODY_INDEX, ACTOR_ANIM_HEAD_INDEX, ACTOR_ANIM_RARM_INDEX);
			break;
		case 6:// facing down-left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[6], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[6], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[6], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[6], true);
			if (actor->gun->type == ACTOR_GUN_TYPE_BARE_HANDS) GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_STAND_DOWN_LEFT], true);
			else
				GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_STAND_GUN_DOWN_LEFT], true);
			GFX_SetSpriteGfxOrder(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, ACTOR_ANIM_RARM_INDEX, ACTOR_ANIM_BODY_INDEX, ACTOR_ANIM_HEAD_INDEX, ACTOR_ANIM_LARM_INDEX);
			break;
		case 8:// facing up
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_STAND_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_STAND_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_STAND_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_STAND_UP], true);
			if (actor->gun->type == ACTOR_GUN_TYPE_BARE_HANDS) GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_STAND_UP], true);
			else
				GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_STAND_GUN_UP], true);
			break;
		case 9:// facing up-right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_STAND_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_STAND_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_STAND_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_STAND_UP_RIGHT], true);
			if (actor->gun->type == ACTOR_GUN_TYPE_BARE_HANDS) GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_STAND_UP_RIGHT], true);
			else
				GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_STAND_GUN_UP_RIGHT], true);
			GFX_SetSpriteGfxOrder(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, ACTOR_ANIM_LARM_INDEX, ACTOR_ANIM_BODY_INDEX, ACTOR_ANIM_HEAD_INDEX, ACTOR_ANIM_RARM_INDEX);
			break;
		case 10:// facing up-left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_STAND_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_STAND_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_STAND_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_STAND_UP_LEFT], true);
			if (actor->gun->type == ACTOR_GUN_TYPE_BARE_HANDS) GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_STAND_UP_LEFT], true);
			else
				GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_STAND_GUN_UP_LEFT], true);
			GFX_SetSpriteGfxOrder(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, ACTOR_ANIM_RARM_INDEX, ACTOR_ANIM_BODY_INDEX, ACTOR_ANIM_HEAD_INDEX, ACTOR_ANIM_LARM_INDEX);
			break;
		default:
			break;
	}
}

/** ACTOR :: Set stand animation values
 */
void ACTOR_SetFeetStandAnimation(int facing) {
	switch (facing) {
		case 1:// facing right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[3], true);
			break;
		case 2:// facing left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[7], true);
			break;
		case 4:// facing down
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[5], true);
			break;
		case 5:// facing down-right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[4], true);
			break;
		case 6:// facing down-left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[6], true);
			break;
		case 8:// facing up
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[1], true);
			break;
		case 9:// facing up-right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[2], true);
			break;
		case 10:// facing up-left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[8], true);
			break;
		default:
			break;
	}
}

/** ACTOR :: Set throw animation values
 */
void ACTOR_SetThrowAnimation(int facing) {
	switch (facing) {
		case 1:// facing right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_STAND_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_STAND_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_STAND_RIGHT], true);
			if (actor->gun->type == ACTOR_GUN_TYPE_BARE_HANDS) GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_STAND_RIGHT], true);
			else
				GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_STAND_GUN_RIGHT], true);
			GFX_SetSpriteGfxOrder(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, ACTOR_ANIM_LARM_INDEX, ACTOR_ANIM_BODY_INDEX, ACTOR_ANIM_HEAD_INDEX, ACTOR_ANIM_RARM_INDEX);
			break;
		case 2:// facing left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_STAND_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_STAND_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_STAND_LEFT], true);
			if (actor->gun->type == ACTOR_GUN_TYPE_BARE_HANDS) GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_STAND_LEFT], true);
			else
				GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_STAND_GUN_LEFT], true);
			GFX_SetSpriteGfxOrder(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, ACTOR_ANIM_RARM_INDEX, ACTOR_ANIM_BODY_INDEX, ACTOR_ANIM_HEAD_INDEX, ACTOR_ANIM_LARM_INDEX);
			break;
		case 4:// facing down
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_STAND_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_STAND_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_STAND_DOWN], true);
			if (actor->gun->type == ACTOR_GUN_TYPE_BARE_HANDS) GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_STAND_DOWN], true);
			else
				GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_STAND_GUN_DOWN], true);
			break;
		case 5:// facing down-right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[4], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[4], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[4], true);
			if (actor->gun->type == ACTOR_GUN_TYPE_BARE_HANDS) GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_STAND_DOWN_RIGHT], true);
			else
				GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_STAND_GUN_DOWN_RIGHT], true);
			GFX_SetSpriteGfxOrder(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, ACTOR_ANIM_LARM_INDEX, ACTOR_ANIM_BODY_INDEX, ACTOR_ANIM_HEAD_INDEX, ACTOR_ANIM_RARM_INDEX);
			break;
		case 6:// facing down-left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[6], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[6], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[6], true);
			if (actor->gun->type == ACTOR_GUN_TYPE_BARE_HANDS) GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_STAND_DOWN_LEFT], true);
			else
				GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_STAND_GUN_DOWN_LEFT], true);
			GFX_SetSpriteGfxOrder(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, ACTOR_ANIM_RARM_INDEX, ACTOR_ANIM_BODY_INDEX, ACTOR_ANIM_HEAD_INDEX, ACTOR_ANIM_LARM_INDEX);
			break;
		case 8:// facing up
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_STAND_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_STAND_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_STAND_UP], true);
			if (actor->gun->type == ACTOR_GUN_TYPE_BARE_HANDS) GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_STAND_UP], true);
			else
				GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_STAND_GUN_UP], true);
			break;
		case 9:// facing up-right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_STAND_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_STAND_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_STAND_UP_RIGHT], true);
			if (actor->gun->type == ACTOR_GUN_TYPE_BARE_HANDS) GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_STAND_UP_RIGHT], true);
			else
				GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_STAND_GUN_UP_RIGHT], true);
			GFX_SetSpriteGfxOrder(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, ACTOR_ANIM_LARM_INDEX, ACTOR_ANIM_BODY_INDEX, ACTOR_ANIM_HEAD_INDEX, ACTOR_ANIM_RARM_INDEX);
			break;
		case 10:// facing up-left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_STAND_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_STAND_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_STAND_UP_LEFT], true);
			if (actor->gun->type == ACTOR_GUN_TYPE_BARE_HANDS) GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_STAND_UP_LEFT], true);
			else
				GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_STAND_GUN_UP_LEFT], true);
			GFX_SetSpriteGfxOrder(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, ACTOR_ANIM_RARM_INDEX, ACTOR_ANIM_BODY_INDEX, ACTOR_ANIM_HEAD_INDEX, ACTOR_ANIM_LARM_INDEX);
			break;
		default:
			break;
	}
}

/** ACTOR :: Set hide gun animation values
 */
void ACTOR_SetHideGunAnimation(int facing) {
	GFX_SetSpriteGfxOrder(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, ACTOR_ANIM_LARM_INDEX, ACTOR_ANIM_BODY_INDEX, ACTOR_ANIM_HEAD_INDEX, ACTOR_ANIM_RARM_INDEX);
	switch (facing) {
		case 1:// facing right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_HIDE_GUN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_HIDE_GUN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_HIDE_GUN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_HIDE_GUN_RIGHT], true);
			break;
		case 2:// facing left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_HIDE_GUN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_HIDE_GUN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_HIDE_GUN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_HIDE_GUN_LEFT], true);
			break;
		case 4:// facing down
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_HIDE_GUN_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_HIDE_GUN_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_HIDE_GUN_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_HIDE_GUN_DOWN], true);
			break;
		case 5:// facing down-right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_HIDE_GUN_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_HIDE_GUN_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_HIDE_GUN_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_HIDE_GUN_DOWN_RIGHT], true);
			break;
		case 6:// facing down-left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_HIDE_GUN_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_HIDE_GUN_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_HIDE_GUN_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_HIDE_GUN_DOWN_LEFT], true);
			break;
		case 8:// facing up
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_HIDE_GUN_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_HIDE_GUN_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_HIDE_GUN_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_HIDE_GUN_UP], true);
			break;
		case 9:// facing up-right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_HIDE_GUN_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_HIDE_GUN_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_HIDE_GUN_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_HIDE_GUN_UP_RIGHT], true);
			break;
		case 10:// facing up-left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_HIDE_GUN_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_HIDE_GUN_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_HIDE_GUN_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_HIDE_GUN_UP_LEFT], true);
			break;
	}
}

/** ACTOR :: Set hide hands animation values
 */
void ACTOR_SetHideHandsAnimation(int facing) {
	GFX_SetSpriteGfxOrder(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, ACTOR_ANIM_LARM_INDEX, ACTOR_ANIM_BODY_INDEX, ACTOR_ANIM_HEAD_INDEX, ACTOR_ANIM_RARM_INDEX);
	switch (facing) {
		case 1:// facing right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_HIDE_HAND_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_HIDE_HAND_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_HIDE_HAND_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_HIDE_HAND_RIGHT], true);
			break;
		case 2:// facing left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_HIDE_HAND_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_HIDE_HAND_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_HIDE_HAND_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_HIDE_HAND_LEFT], true);
			break;
		case 4:// facing down
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_HIDE_HAND_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_HIDE_HAND_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_HIDE_HAND_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_HIDE_HAND_DOWN], true);
			break;
		case 5:// facing down-right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_HIDE_HAND_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_HIDE_HAND_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_HIDE_HAND_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_HIDE_HAND_DOWN_RIGHT], true);
			break;
		case 6:// facing down-left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_HIDE_HAND_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_HIDE_HAND_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_HIDE_HAND_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_HIDE_HAND_DOWN_LEFT], true);
			break;
		case 8:// facing up
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_HIDE_HAND_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_HIDE_HAND_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_HIDE_HAND_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_HIDE_HAND_UP], true);
			break;
		case 9:// facing up-right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_HIDE_HAND_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_HIDE_HAND_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_HIDE_HAND_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_HIDE_HAND_UP_RIGHT], true);
			break;
		case 10:// facing up-left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_HIDE_HAND_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_HIDE_HAND_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_HIDE_HAND_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_HIDE_HAND_UP_LEFT], true);
			break;
	}
}

/** ACTOR :: Set show gun animation values
 */
void ACTOR_SetShowGunAnimation(int facing) {
	GFX_SetSpriteGfxOrder(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, ACTOR_ANIM_LARM_INDEX, ACTOR_ANIM_BODY_INDEX, ACTOR_ANIM_HEAD_INDEX, ACTOR_ANIM_RARM_INDEX);
	switch (facing) {
		case 1:// facing right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_SHOW_GUN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_SHOW_GUN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_SHOW_GUN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_SHOW_GUN_RIGHT], true);
			break;
		case 2:// facing left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_SHOW_GUN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_SHOW_GUN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_SHOW_GUN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_SHOW_GUN_LEFT], true);
			break;
		case 4:// facing down
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_SHOW_GUN_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_SHOW_GUN_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_SHOW_GUN_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_SHOW_GUN_DOWN], true);
			break;
		case 5:// facing down-right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_SHOW_GUN_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_SHOW_GUN_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_SHOW_GUN_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_SHOW_GUN_DOWN_RIGHT], true);
			break;
		case 6:// facing down-left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_SHOW_GUN_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_SHOW_GUN_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_SHOW_GUN_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_SHOW_GUN_DOWN_LEFT], true);
			break;
		case 8:// facing up
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_SHOW_GUN_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_SHOW_GUN_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_SHOW_GUN_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_SHOW_GUN_UP], true);
			break;
		case 9:// facing up-right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_SHOW_GUN_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_SHOW_GUN_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_SHOW_GUN_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_SHOW_GUN_UP_RIGHT], true);
			break;
		case 10:// facing up-left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_SHOW_GUN_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_SHOW_GUN_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_SHOW_GUN_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_SHOW_GUN_UP_LEFT], true);
			break;
	}
}

/** ACTOR :: Set show hands animation values
 */
void ACTOR_SetShowHandsAnimation(int facing) {
	GFX_SetSpriteGfxOrder(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, ACTOR_ANIM_LARM_INDEX, ACTOR_ANIM_BODY_INDEX, ACTOR_ANIM_HEAD_INDEX, ACTOR_ANIM_RARM_INDEX);
	switch (facing) {
		case 1:// facing right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_SHOW_HAND_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_SHOW_HAND_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_SHOW_HAND_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_SHOW_HAND_RIGHT], true);
			break;
		case 2:// facing left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_SHOW_HAND_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_SHOW_HAND_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_SHOW_HAND_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_SHOW_HAND_LEFT], true);
			break;
		case 4:// facing down
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_SHOW_HAND_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_SHOW_HAND_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_SHOW_HAND_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_SHOW_HAND_DOWN], true);
			break;
		case 5:// facing down-right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_SHOW_HAND_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_SHOW_HAND_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_SHOW_HAND_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_SHOW_HAND_DOWN_RIGHT], true);
			break;
		case 6:// facing down-left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_SHOW_HAND_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_SHOW_HAND_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_SHOW_HAND_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_SHOW_HAND_DOWN_LEFT], true);
			break;
		case 8:// facing up
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_SHOW_HAND_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_SHOW_HAND_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_SHOW_HAND_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_SHOW_HAND_UP], true);
			break;
		case 9:// facing up-right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_SHOW_HAND_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_SHOW_HAND_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_SHOW_HAND_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_SHOW_HAND_UP_RIGHT], true);
			break;
		case 10:// facing up-left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_SHOW_HAND_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_SHOW_HAND_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_SHOW_HAND_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_SHOW_HAND_UP_LEFT], true);
			break;
	}
}

/** ACTOR :: Set hide gun animation values
 */
void ACTOR_SetReloadAnimation(int facing) {
	GFX_SetSpriteGfxOrder(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, ACTOR_ANIM_LARM_INDEX, ACTOR_ANIM_BODY_INDEX, ACTOR_ANIM_HEAD_INDEX, ACTOR_ANIM_RARM_INDEX);
	switch (facing) {
		case 1:// facing right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_RELOAD_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_RELOAD_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_RELOAD_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_RELOAD_RIGHT], true);
			break;
		case 2:// facing left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_RELOAD_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_RELOAD_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_RELOAD_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_RELOAD_LEFT], true);
			break;
		case 4:// facing down
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_RELOAD_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_RELOAD_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_RELOAD_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_RELOAD_DOWN], true);
			break;
		case 5:// facing down-right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_RELOAD_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_RELOAD_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_RELOAD_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_RELOAD_DOWN_RIGHT], true);
			break;
		case 6:// facing down-left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_RELOAD_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_RELOAD_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_RELOAD_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_RELOAD_DOWN_LEFT], true);
			break;
		case 8:// facing up
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_RELOAD_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_RELOAD_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_RELOAD_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_RELOAD_UP], true);
			break;
		case 9:// facing up-right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_RELOAD_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_RELOAD_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_RELOAD_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_RELOAD_UP_RIGHT], true);
			break;
		case 10:// facing up-left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_RELOAD_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_RELOAD_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_RELOAD_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_RELOAD_UP_LEFT], true);
			break;
	}
}

/** ACTOR :: Set hit animation values
 */
void ACTOR_SetHitAnimation(int facing) {
	switch (facing) {
		case 1:// facing right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_HIT_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_HIT_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_HIT_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_HIT_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_HIT_RIGHT], true);
			break;
		case 2:// facing left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_HIT_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_HIT_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_HIT_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_HIT_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_HIT_LEFT], true);
			break;
		case 4:// facing down
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_HIT_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_HIT_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_HIT_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_HIT_DOWN], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_HIT_DOWN], true);
			break;
		case 5:// facing down-right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_HIT_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_HIT_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_HIT_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_HIT_DOWN_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_HIT_DOWN_RIGHT], true);
			break;
		case 6:// facing down-left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_HIT_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_HIT_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_HIT_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_HIT_DOWN_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_HIT_DOWN_LEFT], true);
			break;
		case 8:// facing up
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_HIT_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_HIT_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_HIT_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_HIT_UP], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_HIT_UP], true);
			break;
		case 9:// facing up-right
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_HIT_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_HIT_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_HIT_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_HIT_UP_RIGHT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_HIT_UP_RIGHT], true);
			break;
		case 10:// facing up-left
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_HIT_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_HIT_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_HIT_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_HIT_UP_LEFT], true);
			GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_HIT_UP_LEFT], true);
			break;
	}
}

/** ACTOR :: Set hit animation values
 */
void ACTOR_SetDeadAnimation() {
	GFX_SetSpriteGraphicsId(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, SPRITE_GRAPHICS_ID_ACTOR_RARM_BARE_HANDS);
	GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, actor->feet_animation[ACTOR_ANIM_DEAD], true);
	GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, actor->body_animation[ACTOR_ANIM_DEAD], true);
	GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, actor->head_animation[ACTOR_ANIM_DEAD], true);
	GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, actor->left_arm_animation[ACTOR_ANIM_DEAD], true);
	GFX_SetSpriteAnimation(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, actor->right_arm_animation[ACTOR_ANIM_DEAD], true);
}

void ACTOR_SetBulletStatus(int pistol_bullets, int shotgun_bullets, int uzi_bullets, int snipper_bullets, int grenades) {
	actor->gun->total_pistol_bullets = pistol_bullets;
	actor->gun->total_shotgun_bullets = shotgun_bullets;
	actor->gun->total_uzi_bullets = uzi_bullets;
	actor->gun->total_snipper_bullets = snipper_bullets;
	actor->current_grenades = grenades;
}

void ACTOR_SetPosition(int x, int y, int facing) {
	actor->pos_x = x;
	actor->pos_y = y;
	actor->x_FP = x << FP;
	actor->y_FP = y << FP;
	actor->last_facing = facing;
}


void ACTOR_LoadActorAssets(const char *dat_name) {

	// Load graphics
	GFX_LoadSpriteGraphicsRLE("ACTOR.DAT", "FACE.PCX", SPRITE_GRAPHICS_ID_ACTORCHAT, 80, 82, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("ACTOR.DAT", "PORTAIT.PCX", SPRITE_GRAPHICS_ID_ACTOR_PORTAIT, 32, 32, 2, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("ACTOR.DAT", "HEAD.PCX", SPRITE_GRAPHICS_ID_ACTOR_HEAD, 12, 12, 30, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("ACTOR.DAT", "FEET.PCX", SPRITE_GRAPHICS_ID_ACTOR_FEET, 27, 16, 84, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("ACTOR.DAT", "BODY.PCX", SPRITE_GRAPHICS_ID_ACTOR_BODY, 20, 15, 100, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("ACTOR.DAT", "LARM1.PCX", SPRITE_GRAPHICS_ID_ACTOR_LARM, 16, 18, 90, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("ACTOR.DAT", "RARM1.PCX", SPRITE_GRAPHICS_ID_ACTOR_RARM_BARE_HANDS, 16, 18, 135, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("ACTOR.DAT", "RARM2.PCX", SPRITE_GRAPHICS_ID_ACTOR_RARM_PISTOL, 20, 20, 72, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("ACTOR.DAT", "RARM3.PCX", SPRITE_GRAPHICS_ID_ACTOR_RARM_SHOTGUN, 20, 20, 72, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("ACTOR.DAT", "RARM4.PCX", SPRITE_GRAPHICS_ID_ACTOR_RARM_UZI, 20, 20, 72, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("ACTOR.DAT", "RARM5.PCX", SPRITE_GRAPHICS_ID_ACTOR_RARM_RIFLE, 20, 20, 72, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	// Load animations
	GFX_LoadAnimations("ACTOR.DAT", "FEET.ANI", actor->feet_animation, 150);     // feet animations
	GFX_LoadAnimations("ACTOR.DAT", "BODY.ANI", actor->body_animation, 150);     // body animations
	GFX_LoadAnimations("ACTOR.DAT", "HEAD.ANI", actor->head_animation, 150);     // body animations
	GFX_LoadAnimations("ACTOR.DAT", "LARM.ANI", actor->left_arm_animation, 150); // left arm animations
	GFX_LoadAnimations("ACTOR.DAT", "RARM.ANI", actor->right_arm_animation, 150);// right arm animations
}

/** ACTOR :: Initializes actor data
 * - Sets default values for the actor structure 
 */
void ACTOR_Load(int x, int y, int feet_gfx_id, int body_gfx_id, int head_gfx_id, int larm_gfx_id, int rarm_gfx_id, int facing) {
	int sprite_slot;

	actor->pos_x = x;
	actor->pos_y = y;
	actor->x_FP = x << FP;
	actor->y_FP = y << FP;
	actor->vx_FP = 0;
	actor->vy_FP = 0;

	actor->speed_horizontal_FP = ACTOR_SPEED << FP;
	actor->speed_vertical_FP = (ACTOR_SPEED << FP) - ((ACTOR_SPEED << FP) >> 3);
	actor->speed_diagonalv_FP = (actor->speed_vertical_FP * 10) / 14;
	actor->speed_diagonalh_FP = (actor->speed_horizontal_FP * 10) / 14;

	actor->speed_horizontal_loop_FP = actor->speed_horizontal_FP + (actor->speed_horizontal_FP);
	actor->speed_vertical_loop_FP = actor->speed_vertical_FP + (actor->speed_vertical_FP);
	actor->speed_diagonalv_loop_FP = actor->speed_diagonalv_FP + (actor->speed_diagonalv_FP);
	actor->speed_diagonalh_loop_FP = actor->speed_diagonalh_FP + (actor->speed_diagonalh_FP);
	actor->speed = ACTOR_SPEED;
	actor->max_life = ACTOR_LIFE;
	actor->life = ACTOR_LIFE;
	actor->damage = 0;
	actor->is_hit = false;
	actor->grenades_max_distance = ACTOR_GRENADES_MAX_DISTANCE;
	actor->grenades_max_time = ACTOR_GRENADES_MAX_TIME;

	actor->shoot_accuracy_speed = ACTOR_ACCURACY_SPEED;
	actor->shoot_accuracy_counter = 0;

	actor->status_dead = false;

	actor->action_loop = false;
	actor->action_shoot = false;
	actor->action_punch = false;
	actor->action_stand = false;
	actor->action_walk = false;
	actor->action_dead = false;
	actor->action_dying = false;
	actor->action_change_gun = false;
	actor->action_punch_combo = false;
	actor->action_hit = false;
	actor->action_step = 0;
	actor->action_reload = 0;
	actor->action_throw_grenade = false;
	actor->last_facing = facing;
	actor->last_movement = 0;
	actor->last_action = ACTOR_ACTION_STAND;

	actor->key_entity_id = 0;
	actor->key_graphics_id = SPRITE_GRAPHICS_ID_EMPTY;

	actor->mode_combat = false;

	sprite_slot = GFX_FindEmptySpriteSlot();
	if (sprite_slot == -1) {
		sprintf(engine.system_error_message1, "ACTOR_Load function error");
		sprintf(engine.system_error_message2, "No empty sprite slot available");
		sprintf(engine.system_error_message3, " ");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
	} else {
		actor->sprite_num = sprite_slot;
		GFX_InitSprite(ENTITY_ID_ACTOR, 0, sprite_slot, 0, ACTOR_SPRITE_WIDTH, ACTOR_SPRITE_HEIGHT);
	}

	actor->width_px = ACTOR_SPRITE_WIDTH;
	actor->height_px = ACTOR_SPRITE_HEIGHT;

	// Set colission points
	//  ----------------
	// |                |
	// |                |
	// |                |
	// |  0           1 |
	// |                |
	// |                |
	// |                |
	// |  2           3 |
	//  ----------------
	// Point 0
	actor->colission_area.points[0][0] = actor->width_px >> 3;
	actor->colission_area.points[0][1] = (actor->height_px >> 1) + (actor->height_px >> 3);
	// Point 1
	actor->colission_area.points[1][0] = actor->width_px - (actor->width_px >> 2);
	actor->colission_area.points[1][1] = (actor->height_px >> 1) + (actor->height_px >> 3);
	// Point 2
	actor->colission_area.points[2][0] = actor->width_px >> 3;
	actor->colission_area.points[2][1] = actor->height_px;
	// Point 3
	actor->colission_area.points[3][0] = actor->width_px - (actor->width_px >> 2);
	actor->colission_area.points[3][1] = actor->height_px;

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
	actor->hit_area.points[0][0] = actor->width_px >> 3;
	actor->hit_area.points[0][1] = (actor->height_px >> 1) - (actor->height_px >> 3);
	// Point 2
	actor->hit_area.points[1][0] = actor->width_px - (actor->width_px >> 2);
	actor->hit_area.points[1][1] = (actor->height_px >> 1) - (actor->height_px >> 3);
	// Point 3
	actor->hit_area.points[2][0] = actor->width_px >> 3;
	actor->hit_area.points[2][1] = actor->height_px - (actor->height_px >> 3);
	// Point 4
	actor->hit_area.points[3][0] = actor->width_px - (actor->width_px >> 2);
	actor->hit_area.points[3][1] = actor->height_px - (actor->height_px >> 3);

	GFX_SetSpriteGraphic(actor->sprite_num, ACTOR_ANIM_FEET_INDEX, feet_gfx_id, 0, 0);// feet
	GFX_SetSpriteGraphic(actor->sprite_num, ACTOR_ANIM_BODY_INDEX, body_gfx_id, 0, 0);// body
	GFX_SetSpriteGraphic(actor->sprite_num, ACTOR_ANIM_HEAD_INDEX, head_gfx_id, 0, 0);// head
	GFX_SetSpriteGraphic(actor->sprite_num, ACTOR_ANIM_LARM_INDEX, larm_gfx_id, 0, 0);// left arm
	GFX_SetSpriteGraphic(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, rarm_gfx_id, 0, 0);// right arm

	ACTOR_SetFullStandAnimation(facing);
	actor->is_loaded = true;
}

/** ACTOR :: Draw colission pixels
 *  - Draws colission pixels just for debug
 */
void ACTOR_DrawColissionPixels(void) {
	//  Draw colission pixels
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[actor->sprite_num].screen_pos_x + actor->colission_area.points[0][0], gfx_sprite_stack[actor->sprite_num].screen_pos_y + actor->colission_area.points[0][1], 229);
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[actor->sprite_num].screen_pos_x + actor->colission_area.points[1][0], gfx_sprite_stack[actor->sprite_num].screen_pos_y + actor->colission_area.points[1][1], 229);
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[actor->sprite_num].screen_pos_x + actor->colission_area.points[2][0], gfx_sprite_stack[actor->sprite_num].screen_pos_y + actor->colission_area.points[2][1], 229);
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[actor->sprite_num].screen_pos_x + actor->colission_area.points[3][0], gfx_sprite_stack[actor->sprite_num].screen_pos_y + actor->colission_area.points[3][1], 229);
}

/** ACTOR :: Draw hit pixels
 *  - Draws hit pixels just for debug
 */
void ACTOR_DrawHitPixels(void) {
	//  Draw colission pixels
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[actor->sprite_num].screen_pos_x + actor->hit_area.points[0][0], gfx_sprite_stack[actor->sprite_num].screen_pos_y + actor->hit_area.points[0][1], 230);
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[actor->sprite_num].screen_pos_x + actor->hit_area.points[1][0], gfx_sprite_stack[actor->sprite_num].screen_pos_y + actor->hit_area.points[1][1], 230);
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[actor->sprite_num].screen_pos_x + actor->hit_area.points[2][0], gfx_sprite_stack[actor->sprite_num].screen_pos_y + actor->hit_area.points[2][1], 230);
	VIDEO_DrawPixelOnScreenBuffer(gfx_sprite_stack[actor->sprite_num].screen_pos_x + actor->hit_area.points[3][0], gfx_sprite_stack[actor->sprite_num].screen_pos_y + actor->hit_area.points[3][1], 230);
}

void ACTOR_CheckItemColission(void) {
	int i;
	bool item_found;
	int old_life;

	int point1_x, point1_y;
	int point2_x;
	int point3_y;

	point1_x = actor->pos_x + actor->colission_area.points[0][0];
	point1_y = actor->pos_y + actor->colission_area.points[0][1];
	point2_x = actor->pos_x + actor->colission_area.points[1][0];
	point3_y = actor->pos_y + actor->colission_area.points[2][1];

	/////// ITEM COLISSIONS ///////////
	for (i = 0; i < ITEM_MAX_ITEMS; i++) {
		item_found = false;
		if (item[i].is_loaded) {
			if (gfx_sprite_stack[item[i].num_sprite].shown) {

				item_found = true;
				if (point1_x > item[i].pos_x + item[i].colission_area.points[1][0]) item_found = false;
				if (point2_x < item[i].pos_x + item[i].colission_area.points[0][0]) item_found = false;
				if (point1_y > item[i].pos_y + item[i].colission_area.points[2][1]) item_found = false;
				if (point3_y < item[i].pos_y + item[i].colission_area.points[0][1]) item_found = false;

				// found
				if (item_found) {
					switch (item[i].type) {
						case ENTITY_ID_ITEM_MEDIKIT:
							old_life = actor->life;
							actor->life += 100;
							if (actor->life > actor->max_life) actor->life = actor->max_life;
							AUDIO_PlaySound(AUDIO_GET_ITEM_EFFECT, 1);
							ITEM_UnloadItem(i);
							GFX_UpdatePanel(gfx_actor_status_panel, actor->life, old_life, actor->max_life, 1);
							break;
						case ENTITY_ID_ITEM_AMMO1:
							actor->gun->total_pistol_bullets += 40;
							if (actor->gun->total_pistol_bullets > 999) actor->gun->total_pistol_bullets = 999;
							AUDIO_PlaySound(AUDIO_RELOAD_PISTOL_EFFECT, 1);
							ITEM_UnloadItem(i);
							break;
						case ENTITY_ID_ITEM_AMMO2:
							actor->gun->total_shotgun_bullets += 12;
							if (actor->gun->total_shotgun_bullets > 999) actor->gun->total_shotgun_bullets = 999;
							AUDIO_PlaySound(AUDIO_RELOAD_SHOOTGUN_EFFECT, 1);
							ITEM_UnloadItem(i);
							break;
						case ENTITY_ID_ITEM_AMMO3:
							actor->gun->total_uzi_bullets += 90;
							if (actor->gun->total_uzi_bullets > 999) actor->gun->total_uzi_bullets = 999;
							AUDIO_PlaySound(AUDIO_RELOAD_UZI_EFFECT, 1);
							ITEM_UnloadItem(i);
							break;
						case ENTITY_ID_ITEM_AMMO4:
							actor->gun->total_snipper_bullets += 12;
							if (actor->gun->total_snipper_bullets > 999) actor->gun->total_snipper_bullets = 999;
							AUDIO_PlaySound(AUDIO_RELOAD_SNIPPER_EFFECT, 1);
							ITEM_UnloadItem(i);
							break;
						case ENTITY_ID_ITEM_GRENADE:
							actor->current_grenades += 10;
							if (actor->current_grenades > 99) actor->current_grenades = 99;
							AUDIO_PlaySound(AUDIO_GET_ITEM_EFFECT, 1);
							ITEM_UnloadItem(i);
							break;
						case ENTITY_ID_ITEM_KEYRED:
							if (actor->key_entity_id == 0) {
								actor->key_entity_id = ENTITY_ID_ITEM_KEYRED;
								actor->key_graphics_id = SPRITE_GRAPHICS_ID_ITEM_RED_KEY;
								AUDIO_PlaySound(AUDIO_GET_ITEM_EFFECT, 1);
								ITEM_UnloadItem(i);
							}
							break;
						case ENTITY_ID_ITEM_KEYGREEN:
							if (actor->key_entity_id == 0) {
								actor->key_entity_id = ENTITY_ID_ITEM_KEYGREEN;
								actor->key_graphics_id = SPRITE_GRAPHICS_ID_ITEM_GREEN_KEY;
								AUDIO_PlaySound(AUDIO_GET_ITEM_EFFECT, 1);
								ITEM_UnloadItem(i);
							}
							break;
						case ENTITY_ID_ITEM_KEYYELLOW:
							if (actor->key_entity_id == 0) {
								actor->key_entity_id = ENTITY_ID_ITEM_KEYYELLOW;
								actor->key_graphics_id = SPRITE_GRAPHICS_ID_ITEM_YELLOW_KEY;
								AUDIO_PlaySound(AUDIO_GET_ITEM_EFFECT, 1);
								ITEM_UnloadItem(i);
							}
							break;
						case ENTITY_ID_ITEM_KEYBLUE:
							if (actor->key_entity_id == 0) {
								actor->key_entity_id = ENTITY_ID_ITEM_KEYBLUE;
								actor->key_graphics_id = SPRITE_GRAPHICS_ID_ITEM_BLUE_KEY;
								AUDIO_PlaySound(AUDIO_GET_ITEM_EFFECT, 1);
								ITEM_UnloadItem(i);
							}
							break;
						case ENTITY_ID_ITEM_DOCU:
							if (actor->key_entity_id == 0) {
								actor->key_entity_id = ENTITY_ID_ITEM_DOCU;
								actor->key_graphics_id = SPRITE_GRAPHICS_ID_EMPTY;
								AUDIO_PlaySound(AUDIO_GET_ITEM_EFFECT, 1);
								ITEM_UnloadItem(i);
							}
							break;
						default:
							break;
					}
				}
			}
		}
	}
}

/** ACTOR :: Check up side colissions
 */
bool ACTOR_CheckUpColission(void) {
	int i;
	int point1_x, point1_y;
	int point2_x, point2_y;

	point1_x = actor->pos_x + actor->colission_area.points[0][0] + 1;
	point1_y = actor->pos_y + actor->colission_area.points[0][1];
	point2_x = actor->pos_x + actor->colission_area.points[1][0] - 1;
	point2_y = actor->pos_y + actor->colission_area.points[1][1];

	/////// BACKGROUND COLISSIONS ///////////
	if (MAP_CheckColissionTile(point1_x, point1_y)) { return true; }// Check point 1
	if (MAP_CheckColissionTile(point2_x, point2_y)) { return true; }// Check point 2

	/////// OBJECTS COLISSIONS ///////////
	for (i = 0; i < OBJECT_MAX_OBJECTS; i++) {
		if (object[i].is_loaded) {
			if (gfx_sprite_stack[object[i].num_sprite].shown) {
				//Point 1
				if (point1_x < object[i].pos_x + object[i].colission_area.points[1][0]

					&& point1_x > object[i].pos_x + object[i].colission_area.points[0][0]

					&& point1_y > object[i].pos_y + object[i].colission_area.points[1][1]

					&& point1_y < object[i].pos_y + object[i].colission_area.points[3][1]) {
					return true;
				}

				//Point 2
				if (point2_x < object[i].pos_x + object[i].colission_area.points[1][0]

					&& point2_x > object[i].pos_x + object[i].colission_area.points[0][0]

					&& point2_y > object[i].pos_y + object[i].colission_area.points[1][1]

					&& point2_y < object[i].pos_y + object[i].colission_area.points[3][1]) {
					return true;
				}
			}
		}
	}

	/////// ENEMIES COLISSIONS ///////////
	for (i = 0; i < ENEMY_MAX_ENEMIES; i++) {
		if (enemy[i].is_loaded && !enemy[i].action_dead) {
			if (gfx_sprite_stack[enemy[i].sprite_num].shown) {
				//Point 1
				if (point1_x < enemy[i].pos_x + enemy[i].colission_area.points[1][0]

					&& point1_x > enemy[i].pos_x + enemy[i].colission_area.points[0][0]

					&& point1_y > enemy[i].pos_y + enemy[i].colission_area.points[1][1]

					&& point1_y < enemy[i].pos_y + enemy[i].colission_area.points[3][1]) {
					return true;
				}

				//Point 2
				if (point2_x < enemy[i].pos_x + enemy[i].colission_area.points[1][0]

					&& point2_x > enemy[i].pos_x + enemy[i].colission_area.points[0][0]

					&& point2_y > enemy[i].pos_y + enemy[i].colission_area.points[1][1]

					&& point2_y < enemy[i].pos_y + enemy[i].colission_area.points[3][1]) {
					return true;
				}
			}
		}
	}

	/////// NPC COLISSIONS ///////////
	for (i = 0; i < NPC_MAX_NPCS; i++) {
		if (npc[i].is_loaded && !npc[i].action_dead) {
			if (gfx_sprite_stack[npc[i].sprite_num].shown) {
				//Point 1
				if (point1_x < npc[i].pos_x + npc[i].colission_area.points[1][0]

					&& point1_x > npc[i].pos_x + npc[i].colission_area.points[0][0]

					&& point1_y > npc[i].pos_y + npc[i].colission_area.points[1][1]

					&& point1_y < npc[i].pos_y + npc[i].colission_area.points[3][1]) {
					return true;
				}

				//Point 2
				if (point2_x < npc[i].pos_x + npc[i].colission_area.points[1][0]

					&& point2_x > npc[i].pos_x + npc[i].colission_area.points[0][0]

					&& point2_y > npc[i].pos_y + npc[i].colission_area.points[1][1]

					&& point2_y < npc[i].pos_y + npc[i].colission_area.points[3][1]) {
					return true;
				}
			}
		}
	}

	/////// BOSS COLISSIONS ///////////
	if (boss->is_loaded) {
		if (gfx_sprite_stack[boss->sprite_num].shown) {
			//Point 1
			if (point1_x < boss->pos_x + boss->colission_area.points[1][0]

				&& point1_x > boss->pos_x + boss->colission_area.points[0][0]

				&& point1_y > boss->pos_y + boss->colission_area.points[1][1]

				&& point1_y < boss->pos_y + boss->colission_area.points[3][1]) {
				return true;
			}

			//Point 2
			if (point2_x < boss->pos_x + boss->colission_area.points[1][0]

				&& point2_x > boss->pos_x + boss->colission_area.points[0][0]

				&& point2_y > boss->pos_y + boss->colission_area.points[1][1]

				&& point2_y < boss->pos_y + boss->colission_area.points[3][1]) {
				return true;
			}
		}
	}

	return false;
}

/** ACTOR :: Check down side colissions
 */
bool ACTOR_CheckDownColission(void) {
	int i;

	int point1_x, point1_y;
	int point2_x, point2_y;

	point1_x = actor->pos_x + actor->colission_area.points[2][0] + 1;
	point1_y = actor->pos_y + actor->colission_area.points[2][1];
	point2_x = actor->pos_x + actor->colission_area.points[3][0] - 1;
	point2_y = actor->pos_y + actor->colission_area.points[3][1];

	if (MAP_CheckColissionTile(point1_x, point1_y)) { return true; }// Check point 1
	if (MAP_CheckColissionTile(point2_x, point2_y)) { return true; }// Check point 2

	/////// OBJECTS COLISSIONS ///////////
	for (i = 0; i < OBJECT_MAX_OBJECTS; i++) {
		if (object[i].is_loaded) {
			if (gfx_sprite_stack[object[i].num_sprite].shown) {
				//Point 1
				if (point1_x < object[i].pos_x + object[i].colission_area.points[1][0]

					&& point1_x > object[i].pos_x + object[i].colission_area.points[0][0]

					&& point1_y > object[i].pos_y + object[i].colission_area.points[1][1]

					&& point1_y < object[i].pos_y + object[i].colission_area.points[2][1]) {
					return true;
				}

				//Point 2
				if (point2_x < object[i].pos_x + object[i].colission_area.points[1][0]

					&& point2_x > object[i].pos_x + object[i].colission_area.points[0][0]

					&& point2_y > object[i].pos_y + object[i].colission_area.points[1][1]

					&& point2_y < object[i].pos_y + object[i].colission_area.points[2][1]) {
					return true;
				}
			}
		}
	}

	/////// ENEMIES COLISSIONS ///////////
	for (i = 0; i < ENEMY_MAX_ENEMIES; i++) {
		if (enemy[i].is_loaded && !enemy[i].action_dead) {
			if (gfx_sprite_stack[enemy[i].sprite_num].shown) {
				//Point 1
				if (point1_x < enemy[i].pos_x + enemy[i].colission_area.points[1][0]

					&& point1_x > enemy[i].pos_x + enemy[i].colission_area.points[0][0]

					&& point1_y > enemy[i].pos_y + enemy[i].colission_area.points[1][1]

					&& point1_y < enemy[i].pos_y + enemy[i].colission_area.points[2][1]) {
					return true;
				}

				//Point 2
				if (point2_x < enemy[i].pos_x + enemy[i].colission_area.points[1][0]

					&& point2_x > enemy[i].pos_x + enemy[i].colission_area.points[0][0]

					&& point2_y > enemy[i].pos_y + enemy[i].colission_area.points[1][1]

					&& point2_y < enemy[i].pos_y + enemy[i].colission_area.points[2][1]) {
					return true;
				}
			}
		}
	}

	/////// NPC COLISSIONS ///////////
	for (i = 0; i < NPC_MAX_NPCS; i++) {
		if (npc[i].is_loaded && !npc[i].action_dead) {
			if (gfx_sprite_stack[npc[i].sprite_num].shown) {
				//Point 1
				if (point1_x < npc[i].pos_x + npc[i].colission_area.points[1][0]

					&& point1_x > npc[i].pos_x + npc[i].colission_area.points[0][0]

					&& point1_y > npc[i].pos_y + npc[i].colission_area.points[1][1]

					&& point1_y < npc[i].pos_y + npc[i].colission_area.points[2][1]) {
					return true;
				}

				//Point 2
				if (point2_x < npc[i].pos_x + npc[i].colission_area.points[1][0]

					&& point2_x > npc[i].pos_x + npc[i].colission_area.points[0][0]

					&& point2_y > npc[i].pos_y + npc[i].colission_area.points[1][1]

					&& point2_y < npc[i].pos_y + npc[i].colission_area.points[2][1]) {
					return true;
				}
			}
		}
	}

	/////// BOSS COLISSIONS ///////////
	if (boss->is_loaded) {
		if (gfx_sprite_stack[boss->sprite_num].shown) {
			//Point 1
			if (point1_x < boss->pos_x + boss->colission_area.points[1][0]

				&& point1_x > boss->pos_x + boss->colission_area.points[0][0]

				&& point1_y > boss->pos_y + boss->colission_area.points[1][1]

				&& point1_y < boss->pos_y + boss->colission_area.points[2][1]) {
				return true;
			}

			//Point 2
			if (point2_x < boss->pos_x + boss->colission_area.points[1][0]

				&& point2_x > boss->pos_x + boss->colission_area.points[0][0]

				&& point2_y > boss->pos_y + boss->colission_area.points[1][1]

				&& point2_y < boss->pos_y + boss->colission_area.points[2][1]) {
				return true;
			}
		}
	}
	return false;
}

/** ACTOR :: Check left side colissions
 */
bool ACTOR_CheckLeftColission(void) {
	int i;

	int point1_x, point1_y;
	int point2_x, point2_y;

	point1_x = actor->pos_x + actor->colission_area.points[0][0];
	point1_y = actor->pos_y + actor->colission_area.points[0][1] + 1;
	point2_x = actor->pos_x + actor->colission_area.points[2][0];
	point2_y = actor->pos_y + actor->colission_area.points[2][1] - 1;

	if (MAP_CheckColissionTile(point1_x, point1_y)) { return true; }// Check point 1
	if (MAP_CheckColissionTile(point2_x, point2_y)) { return true; }// Check point 2

	/////// OBJECTS COLISSIONS ///////////
	for (i = 0; i < OBJECT_MAX_OBJECTS; i++) {
		if (object[i].is_loaded) {
			if (gfx_sprite_stack[object[i].num_sprite].shown) {
				//Point 1
				if (point1_x < object[i].pos_x + object[i].colission_area.points[1][0]

					&& point1_x > object[i].pos_x + object[i].colission_area.points[0][0]

					&& point1_y > object[i].pos_y + object[i].colission_area.points[0][1]

					&& point1_y < object[i].pos_y + object[i].colission_area.points[2][1]) {
					return true;
				}

				//Point 1
				if (point2_x < object[i].pos_x + object[i].colission_area.points[1][0]

					&& point2_x > object[i].pos_x + object[i].colission_area.points[0][0]

					&& point2_y > object[i].pos_y + object[i].colission_area.points[0][1]

					&& point2_y < object[i].pos_y + object[i].colission_area.points[2][1]) {
					return true;
				}
			}
		}
	}

	/////// ENEMIES COLISSIONS ///////////
	for (i = 0; i < ENEMY_MAX_ENEMIES; i++) {
		if (enemy[i].is_loaded && !enemy[i].action_dead) {
			if (gfx_sprite_stack[enemy[i].sprite_num].shown) {

				//Point 1
				if (point1_x < enemy[i].pos_x + enemy[i].colission_area.points[1][0]

					&& point1_x > enemy[i].pos_x + enemy[i].colission_area.points[0][0]

					&& point1_y > enemy[i].pos_y + enemy[i].colission_area.points[0][1]

					&& point1_y < enemy[i].pos_y + enemy[i].colission_area.points[2][1]) {
					return true;
				}

				//Point 2
				if (point2_x < enemy[i].pos_x + enemy[i].colission_area.points[1][0]

					&& point2_x > enemy[i].pos_x + enemy[i].colission_area.points[0][0]

					&& point2_y > enemy[i].pos_y + enemy[i].colission_area.points[0][1]

					&& point2_y < enemy[i].pos_y + enemy[i].colission_area.points[2][1]) {
					return true;
				}
			}
		}
	}

	/////// NPC COLISSIONS ///////////
	for (i = 0; i < NPC_MAX_NPCS; i++) {
		if (npc[i].is_loaded && !npc[i].action_dead) {
			if (gfx_sprite_stack[npc[i].sprite_num].shown) {

				//Point 1
				if (point1_x < npc[i].pos_x + npc[i].colission_area.points[1][0]

					&& point1_x > npc[i].pos_x + npc[i].colission_area.points[0][0]

					&& point1_y > npc[i].pos_y + npc[i].colission_area.points[0][1]

					&& point1_y < npc[i].pos_y + npc[i].colission_area.points[2][1]) {
					return true;
				}

				//Point 2
				if (point2_x < npc[i].pos_x + npc[i].colission_area.points[1][0]

					&& point2_x > npc[i].pos_x + npc[i].colission_area.points[0][0]

					&& point2_y > npc[i].pos_y + npc[i].colission_area.points[0][1]

					&& point2_y < npc[i].pos_y + npc[i].colission_area.points[2][1]) {
					return true;
				}
			}
		}
	}

	/////// BOSS COLISSIONS ///////////
	if (boss->is_loaded) {
		if (gfx_sprite_stack[boss->sprite_num].shown) {
			//Point 1
			if (point1_x < boss->pos_x + boss->colission_area.points[1][0]

				&& point1_x > boss->pos_x + boss->colission_area.points[0][0]

				&& point1_y > boss->pos_y + boss->colission_area.points[0][1]

				&& point1_y < boss->pos_y + boss->colission_area.points[2][1]) {
				return true;
			}

			//Point 2
			if (point2_x < boss->pos_x + boss->colission_area.points[1][0]

				&& point2_x > boss->pos_x + boss->colission_area.points[0][0]

				&& point2_y > boss->pos_y + boss->colission_area.points[0][1]

				&& point2_y < boss->pos_y + boss->colission_area.points[2][1]) {
				return true;
			}
		}
	}

	return false;
}

/** ACTOR :: Check right side colissions
 */
bool ACTOR_CheckRightColission(void) {
	int i;

	int point1_x, point1_y;
	int point2_x, point2_y;

	point1_x = actor->pos_x + actor->colission_area.points[1][0];
	point1_y = actor->pos_y + actor->colission_area.points[1][1] + 1;
	point2_x = actor->pos_x + actor->colission_area.points[3][0];
	point2_y = actor->pos_y + actor->colission_area.points[3][1] - 1;

	if (MAP_CheckColissionTile(point1_x, point1_y)) { return true; }// Check point 1
	if (MAP_CheckColissionTile(point2_x, point2_y)) { return true; }// Check point 2


	/////// OBJECTS COLISSIONS ///////////
	for (i = 0; i < OBJECT_MAX_OBJECTS; i++) {
		if (object[i].is_loaded) {
			if (gfx_sprite_stack[object[i].num_sprite].shown) {
				//Point 1
				if (point1_x < object[i].pos_x + object[i].colission_area.points[1][0]

					&& point1_x > object[i].pos_x + object[i].colission_area.points[0][0]

					&& point1_y > object[i].pos_y + object[i].colission_area.points[1][1]

					&& point1_y < object[i].pos_y + object[i].colission_area.points[3][1]) {
					return true;
				}

				//Point 1
				if (point2_x < object[i].pos_x + object[i].colission_area.points[1][0]

					&& point2_x > object[i].pos_x + object[i].colission_area.points[0][0]

					&& point2_y > object[i].pos_y + object[i].colission_area.points[1][1]

					&& point2_y < object[i].pos_y + object[i].colission_area.points[3][1]) {
					return true;
				}
			}
		}
	}

	/////// ENEMIES COLISSIONS ///////////
	for (i = 0; i < ENEMY_MAX_ENEMIES; i++) {
		if (enemy[i].is_loaded && !enemy[i].action_dead) {
			if (gfx_sprite_stack[enemy[i].sprite_num].shown) {
				//Point 1
				if (point1_x < enemy[i].pos_x + enemy[i].colission_area.points[1][0]

					&& point1_x > enemy[i].pos_x + enemy[i].colission_area.points[0][0]

					&& point1_y > enemy[i].pos_y + enemy[i].colission_area.points[1][1]

					&& point1_y < enemy[i].pos_y + enemy[i].colission_area.points[3][1]) {
					return true;
				}

				//Point 2
				if (point2_x < enemy[i].pos_x + enemy[i].colission_area.points[1][0]

					&& point2_x > enemy[i].pos_x + enemy[i].colission_area.points[0][0]

					&& point2_y > enemy[i].pos_y + enemy[i].colission_area.points[1][1]

					&& point2_y < enemy[i].pos_y + enemy[i].colission_area.points[3][1]) {
					return true;
				}
			}
		}
	}

	/////// NPC COLISSIONS ///////////
	for (i = 0; i < NPC_MAX_NPCS; i++) {
		if (npc[i].is_loaded && !npc[i].action_dead) {
			if (gfx_sprite_stack[npc[i].sprite_num].shown) {
				//Point 1
				if (point1_x < npc[i].pos_x + npc[i].colission_area.points[1][0]

					&& point1_x > npc[i].pos_x + npc[i].colission_area.points[0][0]

					&& point1_y > npc[i].pos_y + npc[i].colission_area.points[1][1]

					&& point1_y < npc[i].pos_y + npc[i].colission_area.points[3][1]) {
					return true;
				}

				//Point 2
				if (point2_x < npc[i].pos_x + npc[i].colission_area.points[1][0]

					&& point2_x > npc[i].pos_x + npc[i].colission_area.points[0][0]

					&& point2_y > npc[i].pos_y + npc[i].colission_area.points[1][1]

					&& point2_y < npc[i].pos_y + npc[i].colission_area.points[3][1]) {
					return true;
				}
			}
		}
	}

	/////// BOSS COLISSIONS ///////////
	if (boss->is_loaded) {
		if (gfx_sprite_stack[boss->sprite_num].shown) {
			//Point 1
			if (point1_x < boss->pos_x + boss->colission_area.points[1][0]

				&& point1_x > boss->pos_x + boss->colission_area.points[0][0]

				&& point1_y > boss->pos_y + boss->colission_area.points[0][1]

				&& point1_y < boss->pos_y + boss->colission_area.points[3][1]) {
				return true;
			}

			//Point 2
			if (point2_x < boss->pos_x + boss->colission_area.points[1][0]

				&& point2_x > boss->pos_x + boss->colission_area.points[0][0]

				&& point2_y > boss->pos_y + boss->colission_area.points[0][1]

				&& point2_y < boss->pos_y + boss->colission_area.points[3][1]) {
				return true;
			}
		}
	}

	return false;
}

/** ACTOR :: Set combat mode
 */
void ACTOR_SetCombatMode(bool mode) {
	actor->mode_combat = mode;
	if (mode) {
		GFX_ShowPanel(gfx_actor_status_panel, true, 0);
	} else {
		GFX_ShowPanel(gfx_actor_status_panel, false, 0);
	}
}

/** ACTOR :: Get combat mode
 */
bool ACTOR_IsInCombatMode(void) {
	return actor->mode_combat;
}

void ACTOR_SetFreeze(bool freeze) {
	actor->status_freeze = freeze;
}

/** ACTOR :: In game update
 */
void ACTOR_Update(void) {
	bool idle;
	byte movement;
	byte facing;

	int bullets;
	int dx, dy, angle;

	// DEBUG
	//ACTOR_DrawColissionPixels();
	//ACTOR_DrawHitPixels();

	// Calculate actor middle point
	actor->middle_x = GFX_GetSpriteScreenPosX(actor->sprite_num) + (actor->width_px >> 1);
	actor->middle_y = GFX_GetSpriteScreenPosY(actor->sprite_num) + (actor->height_px >> 1);

	// Calculate movement
	movement = 0;
	if (!actor->status_freeze) {
		movement = movement | (kbKeyState[kbBindingUp] << 3);
		movement = movement | (kbKeyState[kbBindingDown] << 2);
		movement = movement | (kbKeyState[kbBindingLeft] << 1);
		movement = movement | (kbKeyState[kbBindingRight]);
	}

	// Calculate facing direction with actor and mouse positions
	if (actor->mode_combat && !actor->action_dead) {
		facing = 0;
		dx = cursor.screen_middle_x_px - actor->middle_x;
		dy = cursor.screen_middle_y_px - actor->middle_y;
		angle = UTIL_GetAngle(dx, dy);
		if ((angle > 190) && (angle < 350)) facing = facing | (1 << 3);// facing up
		if ((angle > 10) && (angle < 170)) facing = facing | (1 << 2); // facing down
		if ((angle > 100) && (angle < 260)) facing = facing | (1 << 1);// facing left
		if ((angle > 280) || (angle < 80)) facing = facing | 1;        // facing right
	}

	// Calculate facing direction with actor movement
	if (!actor->mode_combat && !actor->action_dead) {
		facing = movement;
	}

	// Combat mode switching
	if (engine.ingame & kbKeyState[SCANCODE_C]) {
		if (actor->mode_combat) {
			ACTOR_SetCombatMode(false);
			ACTOR_SetFullStandAnimation(actor->last_facing);
		} else {
			ACTOR_SetCombatMode(true);
		}
		kbKeyState[SCANCODE_C] = false;
	}

	//// ---------------- ACTOR ACTIONS ----------------

	// Check idle status
	idle = !actor->action_dead & !actor->action_loop & !actor->action_shoot & !actor->action_punch & !actor->action_hit & !actor->action_reload & !actor->action_change_gun & !actor->action_throw_grenade;

	// loop
	if (idle && actor->mode_combat && (kbKeyState[kbBindingLoop] && (movement != 0))) {
		idle = false;
		actor->action_loop = true;
		actor->action_stand = false;
		actor->action_step = 0;
	}

	// punch
	if (idle && actor->mode_combat && (actor->gun->type == ACTOR_GUN_TYPE_BARE_HANDS) && (kbKeyState[kbBindingFire] || cursor.left_click)) {
		idle = false;
		actor->action_punch = true;
		actor->action_stand = false;
		actor->shoot_accuracy = 0;
		actor->action_step = 0;
	}

	// throw grenade
	if (idle && actor->mode_combat && (cursor.right_click || kbKeyState[kbBindingThrow])) {
		idle = false;
		actor->action_throw_grenade = true;
		actor->shoot_accuracy = 0;
		actor->action_step = 0;
	}

	// shoot
	if (idle && actor->mode_combat && (actor->gun->type != ACTOR_GUN_TYPE_BARE_HANDS) && (kbKeyState[kbBindingFire] || cursor.left_click)) {
		idle = false;
		actor->action_shoot = true;
		actor->shoot_accuracy = 0;
		actor->action_step = 0;
	}

	// change gun
	if (idle && actor->mode_combat && (kbKeyState[SCANCODE_1] || kbKeyState[SCANCODE_2] || kbKeyState[SCANCODE_3] || kbKeyState[SCANCODE_4] || kbKeyState[SCANCODE_5])) {
		idle = false;
		actor->action_change_gun = true;
		actor->shoot_accuracy = 0;
		actor->action_step = 0;

		// Select gun
		if (kbKeyState[SCANCODE_1]) {// Bare hands
			actor->new_gun = ACTOR_GUN_TYPE_BARE_HANDS;
			kbKeyState[SCANCODE_1] = false;
		}
		if (kbKeyState[SCANCODE_2]) {// Pistol
			actor->new_gun = ACTOR_GUN_TYPE_PISTOL;
			kbKeyState[SCANCODE_2] = false;
		}
		if (kbKeyState[SCANCODE_3]) {// Shotgun
			actor->new_gun = ACTOR_GUN_TYPE_SHOTGUN;
			kbKeyState[SCANCODE_3] = false;
		}
		if (kbKeyState[SCANCODE_4]) {// UZI
			actor->new_gun = ACTOR_GUN_TYPE_UZI;
			kbKeyState[SCANCODE_4] = false;
		}
		if (kbKeyState[SCANCODE_5]) {// snipper
			actor->new_gun = ACTOR_GUN_TYPE_SNIPPER;
			kbKeyState[SCANCODE_5] = false;
		}
	}

	// reload
	if (idle && actor->mode_combat && (actor->gun->type != ACTOR_GUN_TYPE_BARE_HANDS) && kbKeyState[SCANCODE_R]) {
		idle = false;
		actor->action_reload = true;
		actor->shoot_accuracy = 0;
		actor->action_step = 0;
	}

	// hit
	if (idle && actor->is_hit) {
		idle = false;
		actor->action_hit = true;
		actor->action_step = 0;
	}

	//// ---------------- ACTION ANIMATION ----------------

	// loop position calculation
	if (actor->action_loop) {
		switch (actor->action_step) {
			case 0:// Set no gun graphics on right arm
				actor->last_movement = movement;
				GFX_SetSpriteGraphicsId(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, SPRITE_GRAPHICS_ID_ACTOR_RARM_BARE_HANDS);
				ACTOR_SetLoopAnimation(movement);
				actor->action_step++;
				break;
			case 1:// movement and animation
				switch (actor->last_movement) {
					case 1:// loop right
						actor->x_FP += actor->speed_horizontal_loop_FP;
						actor->pos_x = actor->x_FP >> FP;
						if (ACTOR_CheckRightColission()) {
							actor->x_FP -= actor->speed_horizontal_loop_FP;
							actor->pos_x = actor->x_FP >> FP;
						}
						break;
					case 2:// loop left
						actor->x_FP -= actor->speed_horizontal_loop_FP;
						actor->pos_x = actor->x_FP >> FP;
						if (ACTOR_CheckLeftColission()) {
							actor->x_FP += actor->speed_horizontal_loop_FP;
							actor->pos_x = actor->x_FP >> FP;
						}
						break;
					case 4:// loop down
						actor->y_FP += actor->speed_vertical_loop_FP;
						actor->pos_y = actor->y_FP >> FP;
						if (ACTOR_CheckDownColission()) {
							actor->y_FP -= actor->speed_vertical_loop_FP;
							actor->pos_y = actor->y_FP >> FP;
						}
						break;
					case 5:// loop down-right
						actor->x_FP += actor->speed_diagonalh_loop_FP;
						actor->y_FP += actor->speed_diagonalv_loop_FP;
						actor->pos_x = actor->x_FP >> FP;
						actor->pos_y = actor->y_FP >> FP;
						if (ACTOR_CheckDownColission()) {
							actor->y_FP -= actor->speed_diagonalv_loop_FP;
							actor->pos_y = actor->y_FP >> FP;
						}
						if (ACTOR_CheckRightColission()) {
							actor->x_FP -= actor->speed_diagonalh_loop_FP;
							actor->pos_x = actor->x_FP >> FP;
						}
						break;
					case 6:// loop down-left
						actor->x_FP -= actor->speed_diagonalh_loop_FP;
						actor->y_FP += actor->speed_diagonalv_loop_FP;
						actor->pos_x = actor->x_FP >> FP;
						actor->pos_y = actor->y_FP >> FP;
						if (ACTOR_CheckDownColission()) {
							actor->y_FP -= actor->speed_diagonalv_loop_FP;
							actor->pos_y = actor->y_FP >> FP;
						}
						if (ACTOR_CheckLeftColission()) {
							actor->x_FP += actor->speed_diagonalh_loop_FP;
							actor->pos_x = actor->x_FP >> FP;
						}
						break;
					case 8:// loop up
						actor->y_FP -= actor->speed_vertical_loop_FP;
						actor->pos_y = actor->y_FP >> FP;
						if (ACTOR_CheckUpColission()) {
							actor->y_FP += actor->speed_vertical_loop_FP;
							actor->pos_y = actor->y_FP >> FP;
						}
						break;
					case 9:// loop up-right
						actor->x_FP += actor->speed_diagonalh_loop_FP;
						actor->y_FP -= actor->speed_diagonalv_loop_FP;
						actor->pos_x = actor->x_FP >> FP;
						actor->pos_y = actor->y_FP >> FP;
						if (ACTOR_CheckUpColission()) {
							actor->y_FP += actor->speed_diagonalv_loop_FP;
							actor->pos_y = actor->y_FP >> FP;
						}
						if (ACTOR_CheckRightColission()) {
							actor->x_FP -= actor->speed_diagonalh_loop_FP;
							actor->pos_x = actor->x_FP >> FP;
						}
						break;
					case 10:// loop up-left
						actor->x_FP -= actor->speed_diagonalh_loop_FP;
						actor->y_FP -= actor->speed_diagonalv_loop_FP;
						actor->pos_x = actor->x_FP >> FP;
						actor->pos_y = actor->y_FP >> FP;
						if (ACTOR_CheckUpColission()) {
							actor->y_FP += actor->speed_diagonalv_loop_FP;
							actor->pos_y = actor->y_FP >> FP;
						}
						if (ACTOR_CheckLeftColission()) {
							actor->x_FP += actor->speed_diagonalh_loop_FP;
							actor->pos_x = actor->x_FP >> FP;
						}
						break;
				}
				if (GFX_IsSpriteAnimationEnded(actor->sprite_num, 0)) actor->action_step++;
				break;
			case 2:// Set back right arm graphis
				switch (actor->gun->type) {
					case ACTOR_GUN_TYPE_BARE_HANDS:
						GFX_SetSpriteGraphicsId(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, SPRITE_GRAPHICS_ID_ACTOR_RARM_BARE_HANDS);
						break;
					case ACTOR_GUN_TYPE_PISTOL:
						GFX_SetSpriteGraphicsId(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, SPRITE_GRAPHICS_ID_ACTOR_RARM_PISTOL);
						break;
					case ACTOR_GUN_TYPE_SHOTGUN:
						GFX_SetSpriteGraphicsId(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, SPRITE_GRAPHICS_ID_ACTOR_RARM_SHOTGUN);
						break;
					case ACTOR_GUN_TYPE_UZI:
						GFX_SetSpriteGraphicsId(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, SPRITE_GRAPHICS_ID_ACTOR_RARM_UZI);
						break;
					case ACTOR_GUN_TYPE_SNIPPER:
						GFX_SetSpriteGraphicsId(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, SPRITE_GRAPHICS_ID_ACTOR_RARM_RIFLE);
						break;
					default:
						GFX_SetSpriteGraphicsId(actor->sprite_num, ACTOR_ANIM_RARM_INDEX, SPRITE_GRAPHICS_ID_ACTOR_RARM_BARE_HANDS);
						break;
				}
				ACTOR_SetFullStandAnimation(actor->last_movement);
				actor->action_loop = false;
				idle = true;
				break;
			default:
				break;
		}

		// Set last action
		actor->last_action = ACTOR_ACTION_LOOP;
	}

	// Punch and kick animation
	if (actor->action_punch) {
		actor->last_facing = facing;
		switch (actor->action_step) {
			case 0:
				if (actor->last_action == ACTOR_ACTION_PUNCH) actor->action_punch_combo++;
				if (actor->action_punch_combo > 2) actor->action_punch_combo = 0;
				if (actor->last_action != ACTOR_ACTION_PUNCH) actor->action_punch_combo = 0;
				actor->last_action = ACTOR_ACTION_PUNCH;

				if (actor->action_punch_combo == 0) actor->action_step = 1;
				if (actor->action_punch_combo == 1) actor->action_step = 2;
				if (actor->action_punch_combo == 2) actor->action_step = 3;
				break;
			case 1:// combo x1
				ACTOR_SetPunch1Animation(actor->last_facing);
				actor->action_step = 4;
				break;
			case 2:// combo x2
				ACTOR_SetPunch2Animation(actor->last_facing);
				actor->action_step = 4;
				break;
			case 3://combo x3
				ACTOR_SetKickAnimation(actor->last_facing);
				actor->action_step = 8;
				break;
			case 4:// Punch x1 and x2. First frame
				if (gfx_sprite_stack[actor->sprite_num].gfx[ACTOR_ANIM_FEET_INDEX].anim_current_frame == 1) {
					switch (actor->last_facing) {
						case 1:// facing right
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_PUNCH, 4, 4, actor->pos_x + 30, actor->pos_y + 11, actor->pos_x + 30, actor->pos_y + 11, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 2:// facing left
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_PUNCH, 4, 4, actor->pos_x - 9, actor->pos_y + 11, actor->pos_x - 9, actor->pos_y + 11, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 4:// facing down
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_PUNCH, 4, 4, actor->pos_x + 1, actor->pos_y + 31, actor->pos_x + 5, actor->pos_y + 31, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 5:// facing down-right
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_PUNCH, 4, 4, actor->pos_x + 19, actor->pos_y + 27, actor->pos_x + 19, actor->pos_y + 27, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 6:// facing down-left
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_PUNCH, 4, 4, actor->pos_x - 3, actor->pos_y + 23, actor->pos_x - 3, actor->pos_y + 23, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 8:// facing up
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_PUNCH, 4, 4, actor->pos_x + 7, actor->pos_y - 1, actor->pos_x + 7, actor->pos_y - 1, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 9:// facing up-right
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_PUNCH, 4, 4, actor->pos_x + 27, actor->pos_y - 1, actor->pos_x + 27, actor->pos_y - 1, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 10:// facing up-left
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_PUNCH, 4, 4, actor->pos_x - 7, actor->pos_y - 1, actor->pos_x - 7, actor->pos_y - 1, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
					}
					actor->action_step++;
				}
				if (GFX_IsSpriteAnimationEnded(actor->sprite_num, 0)) {
					ACTOR_SetFightAnimation(facing);
					actor->action_step = 0;
					actor->action_punch = false;
					idle = true;
				}
			case 5:// Punch x1 and x2. Second frame
				if (gfx_sprite_stack[actor->sprite_num].gfx[ACTOR_ANIM_FEET_INDEX].anim_current_frame == 2) {
					switch (actor->last_facing) {
						case 1:// facing right
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_PUNCH, 4, 4, actor->pos_x + 34, actor->pos_y + 15, actor->pos_x + 34, actor->pos_y + 15, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 2:// facing left
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_PUNCH, 4, 4, actor->pos_x - 9, actor->pos_y + 15, actor->pos_x - 9, actor->pos_y + 15, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 4:// facing down
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_PUNCH, 4, 4, actor->pos_x + 5, actor->pos_y + 31, actor->pos_x + 9, actor->pos_y + 31, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 5:// facing down-right
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_PUNCH, 4, 4, actor->pos_x + 23, actor->pos_y + 25, actor->pos_x + 23, actor->pos_y + 25, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 6:// facing down-left
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_PUNCH, 4, 4, actor->pos_x, actor->pos_y + 25, actor->pos_x, actor->pos_y + 25, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 8:// facing up
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_PUNCH, 4, 4, actor->pos_x + 5, actor->pos_y - 1, actor->pos_x + 5, actor->pos_y - 1, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 9:// facing up-right
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_PUNCH, 4, 4, actor->pos_x + 25, actor->pos_y - 2, actor->pos_x + 25, actor->pos_y - 2, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 10:// facing up-left
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_PUNCH, 4, 4, actor->pos_x - 8, actor->pos_y - 2, actor->pos_x - 8, actor->pos_y - 2, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
					}
					actor->action_step++;
				}
				if (GFX_IsSpriteAnimationEnded(actor->sprite_num, 0)) {
					ACTOR_SetFightAnimation(facing);
					actor->action_step = 0;
					actor->action_punch = false;
					idle = true;
				}
			case 6:// Punch x1 and x2. Third frame
				if (gfx_sprite_stack[actor->sprite_num].gfx[ACTOR_ANIM_FEET_INDEX].anim_current_frame == 3) {
					switch (actor->last_facing) {
						case 1:// facing right
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_PUNCH, 4, 4, actor->pos_x + 38, actor->pos_y + 19, actor->pos_x + 38, actor->pos_y + 19, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 2:// facing left
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_PUNCH, 4, 4, actor->pos_x - 9, actor->pos_y + 19, actor->pos_x - 9, actor->pos_y + 19, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 4:// facing down
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_PUNCH, 4, 4, actor->pos_x + 9, actor->pos_y + 31, actor->pos_x + 13, actor->pos_y + 31, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 5:// facing down-right
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_PUNCH, 4, 4, actor->pos_x + 24, actor->pos_y + 23, actor->pos_x + 24, actor->pos_y + 23, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 6:// facing down-left
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_PUNCH, 4, 4, actor->pos_x + 3, actor->pos_y + 27, actor->pos_x + 3, actor->pos_y + 27, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 8:// facing up
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_PUNCH, 4, 4, actor->pos_x + 3, actor->pos_y - 1, actor->pos_x + 3, actor->pos_y - 1, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 9:// facing up-right
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_PUNCH, 4, 4, actor->pos_x + 23, actor->pos_y - 1, actor->pos_x + 23, actor->pos_y - 1, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 10:// facing up-left
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_PUNCH, 4, 4, actor->pos_x - 9, actor->pos_y - 3, actor->pos_x - 9, actor->pos_y - 3, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
					}
					actor->action_step = 16;
				}
				if (GFX_IsSpriteAnimationEnded(actor->sprite_num, 0)) {
					ACTOR_SetFightAnimation(facing);
					actor->action_step = 0;
					actor->action_punch = false;
					idle = true;
				}
				break;
			case 8:// Kick. First frame
				if (gfx_sprite_stack[actor->sprite_num].gfx[ACTOR_ANIM_FEET_INDEX].anim_current_frame == 1) {
					switch (actor->last_facing) {
						case 1:// facing right
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_KICK, 4, 4, actor->pos_x + 30, actor->pos_y + 11, actor->pos_x + 30, actor->pos_y + 11, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 2:// facing left
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_KICK, 4, 4, actor->pos_x - 9, actor->pos_y + 11, actor->pos_x - 9, actor->pos_y + 11, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 4:// facing down
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_KICK, 4, 4, actor->pos_x + 1, actor->pos_y + 31, actor->pos_x + 5, actor->pos_y + 31, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 5:// facing down-right
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_KICK, 4, 4, actor->pos_x + 19, actor->pos_y + 27, actor->pos_x + 19, actor->pos_y + 27, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 6:// facing down-left
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_KICK, 4, 4, actor->pos_x - 3, actor->pos_y + 23, actor->pos_x - 3, actor->pos_y + 23, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 8:// facing up
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_KICK, 4, 4, actor->pos_x + 7, actor->pos_y - 1, actor->pos_x + 7, actor->pos_y - 1, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 9:// facing up-right
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_KICK, 4, 4, actor->pos_x + 27, actor->pos_y - 1, actor->pos_x + 27, actor->pos_y - 1, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 10:// facing up-left
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_KICK, 4, 4, actor->pos_x - 7, actor->pos_y - 1, actor->pos_x - 7, actor->pos_y - 1, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
					}
					actor->action_step++;
				}
				if (GFX_IsSpriteAnimationEnded(actor->sprite_num, 0)) {
					ACTOR_SetFightAnimation(facing);
					actor->action_punch = false;
					idle = true;
				}
				break;
			case 9:// Kick. Second frame
				if (gfx_sprite_stack[actor->sprite_num].gfx[ACTOR_ANIM_FEET_INDEX].anim_current_frame == 2) {
					switch (actor->last_facing) {
						case 1:// facing right
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_KICK, 4, 4, actor->pos_x + 34, actor->pos_y + 15, actor->pos_x + 34, actor->pos_y + 15, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 2:// facing left
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_KICK, 4, 4, actor->pos_x - 9, actor->pos_y + 15, actor->pos_x - 9, actor->pos_y + 15, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 4:// facing down
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_KICK, 4, 4, actor->pos_x + 5, actor->pos_y + 31, actor->pos_x + 9, actor->pos_y + 31, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 5:// facing down-right
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_KICK, 4, 4, actor->pos_x + 23, actor->pos_y + 25, actor->pos_x + 23, actor->pos_y + 25, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 6:// facing down-left
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_KICK, 4, 4, actor->pos_x, actor->pos_y + 25, actor->pos_x, actor->pos_y + 25, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 8:// facing up
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_KICK, 4, 4, actor->pos_x + 5, actor->pos_y - 1, actor->pos_x + 5, actor->pos_y - 1, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 9:// facing up-right
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_KICK, 4, 4, actor->pos_x + 25, actor->pos_y - 2, actor->pos_x + 25, actor->pos_y - 2, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 10:// facing up-left
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_KICK, 4, 4, actor->pos_x - 8, actor->pos_y - 2, actor->pos_x - 8, actor->pos_y - 2, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
					}
					actor->action_step++;
				}
				if (GFX_IsSpriteAnimationEnded(actor->sprite_num, 0)) {
					ACTOR_SetFightAnimation(facing);
					actor->action_punch = false;
					idle = true;
				}
				break;
			case 10:// Kick. Third frame
				if (gfx_sprite_stack[actor->sprite_num].gfx[ACTOR_ANIM_FEET_INDEX].anim_current_frame == 3) {
					switch (actor->last_facing) {
						case 1:// facing right
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_KICK, 4, 4, actor->pos_x + 38, actor->pos_y + 19, actor->pos_x + 38, actor->pos_y + 19, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 2:// facing left
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_KICK, 4, 4, actor->pos_x - 9, actor->pos_y + 19, actor->pos_x - 9, actor->pos_y + 19, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 4:// facing down
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_KICK, 4, 4, actor->pos_x + 9, actor->pos_y + 31, actor->pos_x + 13, actor->pos_y + 31, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 5:// facing down-right
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_KICK, 4, 4, actor->pos_x + 24, actor->pos_y + 23, actor->pos_x + 24, actor->pos_y + 23, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 6:// facing down-left
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_KICK, 4, 4, actor->pos_x + 3, actor->pos_y + 27, actor->pos_x + 3, actor->pos_y + 27, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 8:// facing up
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_KICK, 4, 4, actor->pos_x + 3, actor->pos_y - 1, actor->pos_x + 3, actor->pos_y - 1, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 9:// facing up-right
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_KICK, 4, 4, actor->pos_x + 23, actor->pos_y - 1, actor->pos_x + 23, actor->pos_y - 1, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
						case 10:// facing up-left
							BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_KICK, 4, 4, actor->pos_x - 9, actor->pos_y - 3, actor->pos_x - 9, actor->pos_y - 3, actor->gun->max_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
							break;
					}
					actor->action_step = 16;
				}
				if (GFX_IsSpriteAnimationEnded(actor->sprite_num, 0)) {
					ACTOR_SetFightAnimation(facing);
					actor->action_punch = false;
					idle = true;
				}
				break;
			case 16:// End
				if (GFX_IsSpriteAnimationEnded(actor->sprite_num, 0)) {
					ACTOR_SetFightAnimation(facing);
					actor->action_punch = false;
					idle = true;
				}
				break;
			default:
				ACTOR_SetFightAnimation(facing);
				actor->action_punch = false;
				idle = true;
				break;
		}
		// Set last action
		actor->last_action = ACTOR_ACTION_PUNCH;
	}

	// throw grenade and animation
	if (actor->action_throw_grenade) {
		switch (actor->action_step) {
			case 0:
				actor->throw_accuracy_counter = 0;
				actor->throw_accuracy = 0;
				actor->action_step++;
				break;
			case 1:// Check grenades
				if (actor->current_grenades <= 0) {
					AUDIO_PlaySound(AUDIO_SHOT_FAIL_EFFECT, 0);
					actor->action_step = 18;
				} else {
					switch (facing) {
						case ACTOR_FACING_UP:
							actor->grenade_index = GRENADE_InitGrenade(SPRITE_GRAPHICS_ID_GRENADE1, ENTITY_ID_ACTOR_GRENADE, actor->pos_x + 4, actor->pos_y + 16, actor->grenades_max_time, actor->grenades_max_distance);
							break;
						case ACTOR_FACING_UP_RIGHT:
							actor->grenade_index = GRENADE_InitGrenade(SPRITE_GRAPHICS_ID_GRENADE1, ENTITY_ID_ACTOR_GRENADE, actor->pos_x, actor->pos_y + 8, actor->grenades_max_time, actor->grenades_max_distance);
							break;
						case ACTOR_FACING_RIGHT:
							actor->grenade_index = GRENADE_InitGrenade(SPRITE_GRAPHICS_ID_GRENADE1, ENTITY_ID_ACTOR_GRENADE, actor->pos_x + 18, actor->pos_y + 10, actor->grenades_max_time, actor->grenades_max_distance);
							break;
						case ACTOR_FACING_DOWN_RIGHT:
							actor->grenade_index = GRENADE_InitGrenade(SPRITE_GRAPHICS_ID_GRENADE1, ENTITY_ID_ACTOR_GRENADE, actor->pos_x + 22, actor->pos_y + 10, actor->grenades_max_time, actor->grenades_max_distance);
							break;
						case ACTOR_FACING_DOWN:
							actor->grenade_index = GRENADE_InitGrenade(SPRITE_GRAPHICS_ID_GRENADE1, ENTITY_ID_ACTOR_GRENADE, actor->pos_x + 12, actor->pos_y + 28, actor->grenades_max_time, actor->grenades_max_distance);
							break;
						case ACTOR_FACING_DOWN_LEFT:
							actor->grenade_index = GRENADE_InitGrenade(SPRITE_GRAPHICS_ID_GRENADE1, ENTITY_ID_ACTOR_GRENADE, actor->pos_x + 32, actor->pos_y + 16, actor->grenades_max_time, actor->grenades_max_distance);
							break;
						case ACTOR_FACING_LEFT:
							actor->grenade_index = GRENADE_InitGrenade(SPRITE_GRAPHICS_ID_GRENADE1, ENTITY_ID_ACTOR_GRENADE, actor->pos_x + 8, actor->pos_y + 16, actor->grenades_max_time, actor->grenades_max_distance);
							break;
						case ACTOR_FACING_UP_LEFT:
							actor->grenade_index = GRENADE_InitGrenade(SPRITE_GRAPHICS_ID_GRENADE1, ENTITY_ID_ACTOR_GRENADE, actor->pos_x, actor->pos_y + 8, actor->grenades_max_time, actor->grenades_max_distance);
							break;
					}
					actor->current_grenades--;
					actor->action_step++;
				}
				break;
			case 2:// Wait until trigger release
				if ((kbKeyState[kbBindingThrow] | cursor.right_click)) {
					actor->throw_accuracy_counter++;
				}
				if (actor->throw_accuracy_counter >= ACTOR_ACCURACY_DELAY) {
					actor->throw_accuracy_counter = 0;
					if (actor->throw_accuracy < ACTOR_MAX_ACCURACY) actor->throw_accuracy++;
					MOUSE_SetCursorFrame(actor->throw_accuracy);
				}

				ACTOR_SetThrowAnimation(facing);
				if (!kbKeyState[kbBindingThrow] & !cursor.right_click) actor->action_step = 12;
				break;
			case 12:// Calculate target
				actor->shoot_x = camera->pos_x + cursor.pos_x;
				actor->shoot_y = camera->pos_y + cursor.pos_y;
				actor->action_step++;
				break;
			case 13:// Throw
				GRENADE_ThrowGrenade(actor->grenade_index, actor->shoot_x, actor->shoot_y, actor->shoot_accuracy);
				actor->action_step++;
				break;
			case 14:// Set pointing animation
				actor->throw_time = 5;
				actor->current_throw_time = 0;
				actor->action_step++;
				break;
			case 15:// Wait recoil time
				actor->current_throw_time++;
				if (actor->current_throw_time >= actor->throw_time) {
					actor->current_throw_time = 0;
					actor->action_step++;
				}
				break;
			case 16:
				MOUSE_SetCursorFrame(0);
				actor->action_step = 0;
				actor->action_throw_grenade = false;
				break;
			default:
				actor->action_step = 0;
				actor->action_throw_grenade = false;
				break;
		}
		// Set last action
		actor->last_action = ACTOR_ACTION_THROW;
	}

	// Shooting and animation
	if (actor->action_shoot) {
		switch (actor->action_step) {
			case 0:
				if (actor->last_action == ACTOR_ACTION_SHOOT) actor->action_step = 2;
				else {
					ACTOR_SetPointAnimation(facing);
					actor->action_step++;
				}
				break;
			case 1:
				if (gfx_sprite_stack[actor->sprite_num].gfx[ACTOR_ANIM_RARM_INDEX].anim_end) actor->action_step++;
				break;
			case 2:                                               // Jump to mode step
				if (actor->gun->accurate && !actor->action_walk) {// accurate shoot
					actor->action_step = 3;
				} else {// Bad accuracy shoot
					actor->shoot_accuracy = 0;
					actor->action_step = 4;
				}
				break;
			case 3:// Wait until trigger release
				if ((kbKeyState[kbBindingFire] | cursor.left_click)) actor->shoot_accuracy_counter++;
				if (actor->shoot_accuracy_counter >= actor->shoot_accuracy_speed) {
					actor->shoot_accuracy_counter = 0;
					if (actor->shoot_accuracy < actor->gun->max_accuracy) actor->shoot_accuracy++;

					MOUSE_SetCursorFrame(actor->shoot_accuracy);
				}

				if (!kbKeyState[kbBindingFire] & !cursor.left_click) actor->action_step = 5;
				break;
			case 4:// Check trigger release if snipper rifle
				switch (actor->gun->type) {
					case ACTOR_GUN_TYPE_SNIPPER:
						if (!kbKeyState[kbBindingFire] & !cursor.left_click) actor->action_step = 5;
						break;
					case ACTOR_GUN_TYPE_PISTOL:
					case ACTOR_GUN_TYPE_SHOTGUN:
					case ACTOR_GUN_TYPE_UZI:
						actor->action_step = 5;
						break;
				}
				break;
			case 5:// Check bullets
				if (actor->gun->current_bullets <= 0) {
					AUDIO_PlaySound(AUDIO_SHOT_FAIL_EFFECT, 0);
					actor->action_step = 20;
				} else {
					actor->action_step = 12;
				}

				break;
			case 12:// Set shoot animation

				ACTOR_SetShotAnimation(facing);

				// Calculate target
				actor->shoot_x = camera->pos_x + cursor.screen_middle_x_px;
				actor->shoot_y = camera->pos_y + cursor.screen_middle_y_px;

				// Shoot sound
				switch (actor->gun->type) {
					case ACTOR_GUN_TYPE_PISTOL:
						AUDIO_PlaySound(AUDIO_GUN_EFFECT, 1);
						actor->gun->current_pistol_bullets--;
						break;
					case ACTOR_GUN_TYPE_SHOTGUN:
						AUDIO_PlaySound(AUDIO_SHOTGUN_EFFECT, 1);
						actor->gun->current_shotgun_bullets--;
						break;
					case ACTOR_GUN_TYPE_UZI:
						AUDIO_PlaySound(AUDIO_UZI_EFFECT, 1);
						actor->gun->current_uzi_bullets--;
						break;
					case ACTOR_GUN_TYPE_SNIPPER:
						AUDIO_PlaySound(AUDIO_SNIPPER_EFFECT, 1);
						actor->gun->current_snipper_bullets--;
						break;
					default:
						break;
				}

				// Shot bullet
				for (bullets = 0; bullets < actor->gun->shoots; bullets++) {
					BULLET_LoadBullet(actor->gun->bullet_graphics_id, ENTITY_ID_ACTOR_BULLET, 4, 4, actor->pos_x + (GFX_GetSpriteWidth_px(actor->sprite_num) >> 1), actor->pos_y + (GFX_GetSpriteHeight_px(actor->sprite_num) >> 1), actor->shoot_x, actor->shoot_y, actor->shoot_accuracy, actor->gun->max_distance, actor->gun->bullet_speed, actor->gun->damage);
				}

				// Shot effect
				switch (facing) {
					case 1:// facing right
						EFFECT_LoadEffect(ENTITY_ID_EMPTY, actor->gun->effect_graphics_id, actor->pos_x + 24 + (rand() % 8), actor->pos_y + (rand() % 8), false, (rand() % 3), false, false, 3);
						break;
					case 2:// facing left
						EFFECT_LoadEffect(ENTITY_ID_EMPTY, actor->gun->effect_graphics_id, actor->pos_x - 24 - (rand() % 8), actor->pos_y + (rand() % 8), false, (rand() % 3), false, false, 3);
						break;
					case 4:// facing down
						EFFECT_LoadEffect(ENTITY_ID_EMPTY, actor->gun->effect_graphics_id, actor->pos_x + 12 - (rand() % 8), actor->pos_y + 24 + (rand() % 8), false, (rand() % 3), false, false, 3);
						break;
					case 5:// facing down-right
						EFFECT_LoadEffect(ENTITY_ID_EMPTY, actor->gun->effect_graphics_id, actor->pos_x + 24 - (rand() % 8), actor->pos_y + 24 + (rand() % 8), false, (rand() % 3), false, false, 3);
						break;
					case 6:// facing down-left
						EFFECT_LoadEffect(ENTITY_ID_EMPTY, actor->gun->effect_graphics_id, actor->pos_x - 24 - (rand() % 8), actor->pos_y + 24 + (rand() % 8), false, (rand() % 3), false, false, 3);
						break;
					case 8:// facing up
						EFFECT_LoadEffect(ENTITY_ID_EMPTY, actor->gun->effect_graphics_id, actor->pos_x + (rand() % 8), actor->pos_y - 16 - (rand() % 8), false, (rand() % 3), false, false, 3);
						break;
					case 9:// facing up-right
						EFFECT_LoadEffect(ENTITY_ID_EMPTY, actor->gun->effect_graphics_id, actor->pos_x + (rand() % 8), actor->pos_y - 16 - (rand() % 8), false, (rand() % 3), false, false, 3);
						break;
					case 10:// facing up-left
						EFFECT_LoadEffect(ENTITY_ID_EMPTY, actor->gun->effect_graphics_id, actor->pos_x - 24 + (rand() % 8), actor->pos_y - 16 - (rand() % 8), false, (rand() % 3), false, false, 3);
						break;
					default:
						break;
				}

				// Set pointing animation
				actor->gun->current_recoil = 0;

				actor->action_step = 18;
				break;
			case 18:// Wait recoil time
				actor->gun->current_recoil++;
				if (actor->gun->current_recoil >= actor->gun->recoil_time) {
					actor->gun->current_recoil = 0;
					actor->action_step++;
				}
				break;
			case 19:// End sequence
				//if (gfx_sprite_stack[actor->sprite_num].animation[4].end)
				actor->action_step++;
				break;
			case 20:
				MOUSE_SetCursorFrame(0);
				actor->action_step = 0;
				actor->action_shoot = false;
				switch (actor->gun->type) {
					case ACTOR_GUN_TYPE_PISTOL:
						if (actor->gun->current_pistol_bullets <= 0) actor->action_reload = true;
						break;
					case ACTOR_GUN_TYPE_SHOTGUN:
						if (actor->gun->current_shotgun_bullets <= 0) actor->action_reload = true;
						break;
					case ACTOR_GUN_TYPE_UZI:
						if (actor->gun->current_uzi_bullets <= 0) actor->action_reload = true;
						break;
					case ACTOR_GUN_TYPE_SNIPPER:
						if (actor->gun->current_snipper_bullets <= 0) actor->action_reload = true;
						break;
					default:
						break;
				}
				break;
			default:
				actor->action_step = 0;
				actor->action_shoot = false;
				break;
		}
		actor->last_action = ACTOR_ACTION_SHOOT;
	}

	// Change gun
	if (actor->action_change_gun) {
		switch (actor->action_step) {
			case 0:// Hide current gun
				switch (actor->gun->type) {
					case ACTOR_GUN_TYPE_BARE_HANDS:
						ACTOR_SetHideHandsAnimation(facing);
						break;
					case ACTOR_GUN_TYPE_PISTOL:
					case ACTOR_GUN_TYPE_SHOTGUN:
					case ACTOR_GUN_TYPE_UZI:
					case ACTOR_GUN_TYPE_SNIPPER:
						ACTOR_SetHideGunAnimation(facing);
						break;
					default:
						break;
				}
				actor->action_step++;
				break;
			case 1:
				if (gfx_sprite_stack[actor->sprite_num].gfx[ACTOR_ANIM_RARM_INDEX].anim_end) {
					ACTOR_SetGun(actor->sprite_num, actor->new_gun);
					switch (actor->new_gun) {
						case ACTOR_GUN_TYPE_BARE_HANDS:
							ACTOR_SetShowHandsAnimation(facing);
							break;
						case ACTOR_GUN_TYPE_PISTOL:
						case ACTOR_GUN_TYPE_SHOTGUN:
						case ACTOR_GUN_TYPE_UZI:
						case ACTOR_GUN_TYPE_SNIPPER:
							ACTOR_SetShowGunAnimation(facing);
							break;
						default:
							break;
					}
					actor->new_gun = 0;
					actor->action_step++;
				}
				break;
			case 2:
				if (gfx_sprite_stack[actor->sprite_num].gfx[ACTOR_ANIM_RARM_INDEX].anim_end) actor->action_step++;
				break;
			case 3:
				actor->action_step = 0;
				actor->action_change_gun = false;
				actor->last_action = ACTOR_ACTION_CHANGE_GUN;
				break;
			default:
				actor->action_step = 0;
				actor->action_change_gun = false;
				actor->last_action = ACTOR_ACTION_CHANGE_GUN;
				break;
		}
	}

	// Reload
	if (actor->action_reload) {
		bullets = 0;
		switch (actor->action_step) {
			case 0:// Set reload animation
				ACTOR_SetReloadAnimation(facing);
				switch (actor->gun->type) {
					case ACTOR_GUN_TYPE_PISTOL:
						if (actor->gun->total_pistol_bullets < 0) {
							AUDIO_PlaySound(AUDIO_SHOT_FAIL_EFFECT, 1);
							actor->action_step = 2;
						} else {
							AUDIO_PlaySound(AUDIO_RELOAD_PISTOL_EFFECT, 1);
							actor->action_step++;
						}
						break;
					case ACTOR_GUN_TYPE_SHOTGUN:
						if (actor->gun->total_shotgun_bullets <= 0) {
							AUDIO_PlaySound(AUDIO_SHOT_FAIL_EFFECT, 1);
							actor->action_step = 2;
						} else {
							AUDIO_PlaySound(AUDIO_RELOAD_SHOOTGUN_EFFECT, 1);
							actor->action_step++;
						}
						break;
					case ACTOR_GUN_TYPE_UZI:
						if (actor->gun->total_uzi_bullets < -0) {
							AUDIO_PlaySound(AUDIO_SHOT_FAIL_EFFECT, 1);
							actor->action_step = 2;
						} else {
							AUDIO_PlaySound(AUDIO_RELOAD_PISTOL_EFFECT, 1);
							actor->action_step++;
						}
						break;
					case ACTOR_GUN_TYPE_SNIPPER:
						if (actor->gun->total_snipper_bullets <= 0) {
							AUDIO_PlaySound(AUDIO_SHOT_FAIL_EFFECT, 1);
							actor->action_step = 2;
						} else {
							AUDIO_PlaySound(AUDIO_RELOAD_SNIPPER_EFFECT, 1);
							actor->action_step++;
						}
						break;
					default:
						actor->action_step++;
						break;
				}
				break;
			case 1://  set bullets
				switch (actor->gun->type) {
					case ACTOR_GUN_TYPE_PISTOL:
						bullets = 6 - actor->gun->current_pistol_bullets;
						if (actor->gun->total_pistol_bullets >= bullets) {
							actor->gun->current_pistol_bullets = 6;
							actor->gun->total_pistol_bullets -= bullets;
						} else {
							actor->gun->current_pistol_bullets += actor->gun->total_pistol_bullets;
							actor->gun->total_pistol_bullets = 0;
						}
						break;
					case ACTOR_GUN_TYPE_SHOTGUN:
						if (actor->gun->total_shotgun_bullets >= 1) {
							actor->gun->total_shotgun_bullets = actor->gun->total_shotgun_bullets - (1 - actor->gun->current_shotgun_bullets);
							actor->gun->current_shotgun_bullets = 1;
						}
						break;
					case ACTOR_GUN_TYPE_UZI:
						bullets = 15 - actor->gun->current_uzi_bullets;
						if (actor->gun->total_uzi_bullets >= bullets) {
							actor->gun->current_uzi_bullets = 15;
							actor->gun->total_uzi_bullets -= bullets;
						} else {
							actor->gun->current_uzi_bullets += actor->gun->total_uzi_bullets;
							actor->gun->total_uzi_bullets = 0;
						}
						break;
					case ACTOR_GUN_TYPE_SNIPPER:
						if (actor->gun->total_snipper_bullets >= 1) {
							actor->gun->total_snipper_bullets = actor->gun->total_snipper_bullets - (1 - actor->gun->current_snipper_bullets);
							actor->gun->current_snipper_bullets = 1;
						}
						break;
					default:
						break;
				}
				actor->action_step++;
				break;
			case 2:// Wait
				if (gfx_sprite_stack[actor->sprite_num].gfx[4].anim_end) actor->action_step++;
				break;
			case 3:// Set next frame
				actor->action_step = 0;
				actor->action_reload = false;
				actor->last_action = ACTOR_ACTION_RELOAD;
				break;
			default:
				actor->action_step = 0;
				actor->action_reload = false;
				actor->last_action = ACTOR_ACTION_RELOAD;
				break;
		}
	}

	// Hit animation
	if (actor->action_hit) {
		switch (actor->action_step) {
			case 0:
				ACTOR_SetHitAnimation(actor->last_facing);
				actor->action_step++;
				break;
			case 1:
				if (GFX_IsSpriteAnimationEnded(actor->sprite_num, 0)) actor->action_step++;
				break;
			case 2:
				actor->action_step = 0;
				actor->action_hit = false;
				break;
			default:
				actor->action_step = 0;
				actor->action_hit = false;
				break;
		}
	}

	// Check if is dead
	if (actor->action_dead) {
		switch (actor->action_step) {
			case 0:
				actor->last_movement = 0;
				actor->shoot_accuracy = 0;
				actor->last_action = 0;
				ACTOR_SetDeadAnimation();
				AUDIO_PlaySound(AUDIO_ACTOR_DEAD, 8);
				GFX_SetPanelPortait(gfx_actor_status_panel, 2);
				actor->action_step++;
				break;
			case 1:
				if (GFX_IsSpriteAnimationEnded(actor->sprite_num, 0)) actor->action_step++;
				break;
			case 2:
				SetDelayTime(4000);
				actor->action_step++;
				break;
			case 3:
				if (AwaitDelayTime()) actor->action_step++;
				break;
			case 4:
				actor->action_step++;
				break;
			case 5:
				actor->status_dead = true;
				break;
			default:
				actor->status_dead = true;
				break;
		}
	}

	//// ---------------- SIMULTANEOUS ACTIONS ----------------

	// walk
	if (!actor->action_dead && !actor->action_loop && !actor->action_punch && !actor->action_hit && !actor->is_hit && (movement != 0)) {
		if (idle) ACTOR_SetFullWalkAnimation(facing);
		else
			ACTOR_SetFeetWalkAnimation(facing);

		actor->last_facing = facing;
		actor->last_movement = movement;

		switch (movement) {
			case 1:// moving right
				actor->x_FP += actor->speed_horizontal_FP;
				actor->pos_x = actor->x_FP >> FP;
				if (ACTOR_CheckRightColission()) {
					actor->x_FP -= actor->speed_horizontal_FP;
					actor->pos_x = actor->x_FP >> FP;
				}
				break;
			case 2:// moving left
				actor->x_FP -= actor->speed_horizontal_FP;
				actor->pos_x = actor->x_FP >> FP;
				if (ACTOR_CheckLeftColission()) {
					actor->x_FP += actor->speed_horizontal_FP;
					actor->pos_x = actor->x_FP >> FP;
				}
				break;
			case 4:// moving down
				actor->y_FP += actor->speed_vertical_FP;
				actor->pos_y = actor->y_FP >> FP;
				if (ACTOR_CheckDownColission()) {
					actor->y_FP -= actor->speed_vertical_FP;
					actor->pos_y = actor->y_FP >> FP;
				}
				break;
			case 5:// moving down-right
				actor->x_FP += actor->speed_diagonalh_FP;
				actor->y_FP += actor->speed_diagonalv_FP;
				actor->pos_x = actor->x_FP >> FP;
				if (ACTOR_CheckRightColission()) {
					actor->x_FP -= actor->speed_diagonalh_FP;
					actor->pos_x = actor->x_FP >> FP;
				}
				actor->pos_y = actor->y_FP >> FP;
				if (ACTOR_CheckDownColission()) {
					actor->y_FP -= actor->speed_diagonalv_FP;
					actor->pos_y = actor->y_FP >> FP;
				}
				break;
			case 6:// moving down-left
				actor->x_FP -= actor->speed_diagonalh_FP;
				actor->y_FP += actor->speed_diagonalv_FP;
				actor->pos_x = actor->x_FP >> FP;
				if (ACTOR_CheckLeftColission()) {
					actor->x_FP += actor->speed_diagonalh_FP;
					actor->pos_x = actor->x_FP >> FP;
				}
				actor->pos_y = actor->y_FP >> FP;
				if (ACTOR_CheckDownColission()) {
					actor->y_FP -= actor->speed_diagonalv_FP;
					actor->pos_y = actor->y_FP >> FP;
				}
				break;
			case 8:// moving up
				actor->y_FP -= actor->speed_vertical_FP;
				actor->pos_y = actor->y_FP >> FP;
				if (ACTOR_CheckUpColission()) {
					actor->y_FP += actor->speed_vertical_FP;
					actor->pos_y = actor->y_FP >> FP;
				}
				break;
			case 9:// moving up-right
				actor->x_FP += actor->speed_diagonalh_FP;
				actor->y_FP -= actor->speed_diagonalv_FP;
				actor->pos_x = actor->x_FP >> FP;
				if (ACTOR_CheckRightColission()) {
					actor->x_FP -= actor->speed_diagonalh_FP;
					actor->pos_x = actor->x_FP >> FP;
				}
				actor->pos_y = actor->y_FP >> FP;
				if (ACTOR_CheckUpColission()) {
					actor->y_FP += actor->speed_diagonalv_FP;
					actor->pos_y = actor->y_FP >> FP;
				}
				break;
			case 10:// moving up-left
				actor->x_FP -= actor->speed_diagonalh_FP;
				actor->y_FP -= actor->speed_diagonalv_FP;
				actor->pos_x = actor->x_FP >> FP;
				if (ACTOR_CheckLeftColission()) {
					actor->x_FP += actor->speed_diagonalh_FP;
					actor->pos_x = actor->x_FP >> FP;
				}
				actor->pos_y = actor->y_FP >> FP;
				if (ACTOR_CheckUpColission()) {
					actor->y_FP += actor->speed_diagonalv_FP;
					actor->pos_y = actor->y_FP >> FP;
				}
				break;
			default:
				break;
		}

		// Set last action
		if (idle) actor->last_action = ACTOR_ACTION_WALK;
	}

	//// ---------------- MISC ACTIONS ----------------
	// no action
	if (idle && (movement == 0)) {
		// If last action was walking, just set full stand animation
		if (actor->last_action == ACTOR_ACTION_WALK) {
			actor->last_action = ACTOR_ACTION_STAND;
		}

		// If last action was walking, just set full stand animation
		if (actor->last_action == ACTOR_ACTION_CHANGE_GUN) {
			actor->last_action = ACTOR_ACTION_STAND;
		}

		// If last action was loop, just set full stand animation
		if (actor->last_action == ACTOR_ACTION_LOOP) {
			actor->last_action = ACTOR_ACTION_STAND;
		}

		// If last action was throw, just set full stand animation
		if (actor->last_action == ACTOR_ACTION_THROW) {
			actor->last_action = ACTOR_ACTION_STAND;
		}

		// If last action was reload, just set full stand animation
		if (actor->last_action == ACTOR_ACTION_RELOAD) {
			ACTOR_SetPointAnimation(actor->last_facing);
			actor->last_action = ACTOR_ACTION_SHOOT;
		}

		// If last action was punch, set fight animation
		if (actor->last_action == ACTOR_ACTION_PUNCH) {
			ACTOR_SetFightAnimation(actor->last_facing);
		}

		// If last action was shot, set point animation
		if (actor->last_action == ACTOR_ACTION_SHOOT) {
			ACTOR_SetPointAnimation(actor->last_facing);
		}

		if (actor->last_action == ACTOR_ACTION_STAND) {
			ACTOR_SetFullStandAnimation(actor->last_facing);
		}

		if ((actor->last_action == ACTOR_ACTION_PUNCH) || (actor->last_action == ACTOR_ACTION_SHOOT)) {
			actor->idle_counter++;
			if (actor->idle_counter > ACTOR_IDLE_TICKS) {
				actor->action_stand = true;
				actor->last_action = ACTOR_ACTION_STAND;
				actor->idle_counter = 0;

				ACTOR_SetFullStandAnimation(actor->last_facing);
			}
		}

		/*if (!actor->action_stand) {
			actor->idle_counter++;
			if (actor->idle_counter > ACTOR_IDLE_TICKS) {
				actor->action_stand = true;
				actor->last_action = 0;
				actor->idle_counter = 0;
			}
		}

		if (idle) ACTOR_SetFullStandAnimation(facing);
		else
			ACTOR_SetFeetStandAnimation(facing);*/

		/*if (actor->last_action == ACTOR_ACTION_SHOOT) ACTOR_SetShotAnimation(facing);
		else if (actor->last_action == ACTOR_ACTION_PUNCH)
			ACTOR_SetFightAnimation(facing);
		else {
			if (idle) ACTOR_SetFullStandAnimation(facing);
			else
				
		}*/
	}

	// check if is hit
	if (actor->is_hit && !actor->action_dead) {
		actor->is_hit = false;
		// set blinking
		GFX_SetSpriteBlinkingProperties(actor->sprite_num, true, 1, SPRITE_HIT_COLOR);
		EFFECT_LoadEffect(ENTITY_ID_BLOOD, SPRITE_GRAPHICS_ID_BLOOD, actor->pos_x + (rand() % 32), actor->pos_y + 24 + (rand() % 8), true, 0, false, false, 5);
		EFFECT_LoadEffect(ENTITY_ID_BLOOD, SPRITE_GRAPHICS_ID_BLOOD, actor->pos_x + (rand() % 32), actor->pos_y + 24 + (rand() % 8), true, 0, false, false, 5);

		// avoid damage if actor is looping
		if ((actor->hit_by == ENTITY_ID_ENEMY_BULLET) && actor->action_loop) actor->damage = 0;

		if (actor->damage > actor->life) actor->damage = actor->life;
		if (!actor->life_shown) {
			actor->life -= actor->damage;
			GFX_UpdatePanel(gfx_actor_status_panel, actor->life, actor->life + actor->damage, actor->max_life, 1);
			actor->life_shown = true;
		}
		actor->damage = 0;

		// dead
		if (actor->life <= 0) {
			actor->life = 0;
			actor->action_step = 0;
			actor->action_dead = true;
			actor->action_hit = false;
			actor->action_shoot = false;
			actor->action_loop = false;
			actor->action_walk = false;
			actor->action_stand = false;
			actor->action_reload = false;
		}
	}

	// Life shown
	if (actor->life_shown) {
		//VIDEO_DrawLifeBarOnScreenBuffer(gfx_sprite_stack[actor->sprite_num].screen_pos_x, gfx_sprite_stack[actor->sprite_num].screen_pos_y, 16, ((actor->life << 4) / actor->max_life));
		actor->life_shown_counter++;
		if (actor->life_shown_counter > 10) { actor->life_shown = false; }
	}

	// Update total and current bullets
	switch (actor->gun->type) {
		case ACTOR_GUN_TYPE_BARE_HANDS:
			actor->gun->total_bullets = 0;
			actor->gun->current_bullets = 0;
		case ACTOR_GUN_TYPE_PISTOL:
			actor->gun->current_bullets = actor->gun->current_pistol_bullets;
			actor->gun->total_bullets = actor->gun->total_pistol_bullets;
			break;
		case ACTOR_GUN_TYPE_SHOTGUN:
			actor->gun->current_bullets = actor->gun->current_shotgun_bullets;
			actor->gun->total_bullets = actor->gun->total_shotgun_bullets;
			break;
		case ACTOR_GUN_TYPE_UZI:
			actor->gun->current_bullets = actor->gun->current_uzi_bullets;
			actor->gun->total_bullets = actor->gun->total_uzi_bullets;
			break;
		case ACTOR_GUN_TYPE_SNIPPER:
			actor->gun->current_bullets = actor->gun->current_snipper_bullets;
			actor->gun->total_bullets = actor->gun->total_snipper_bullets;
			break;
		default:
			break;
	}

	// Blink timeout
	if (GFX_IsBlinkActive(actor->sprite_num)) {
		actor->blink_counter++;
		if (actor->blink_counter > 5) {
			actor->blink_counter = 0;
			GFX_SetSpriteBlinkingProperties(actor->sprite_num, false, 0, SPRITE_HIT_COLOR);
		}
	}

	actor->status_idle = idle;

	ACTOR_CheckItemColission();

	// Update sprite position
	gfx_sprite_stack[actor->sprite_num].screen_pos_x = actor->pos_x - camera->pos_x;
	gfx_sprite_stack[actor->sprite_num].screen_pos_y = actor->pos_y - camera->pos_y;
}

void ACTOR_UnloadActor(void) {
	GFX_UnloadSprite(actor->sprite_num);
	actor->is_loaded = false;
	actor->sprite_num = -1;
}

void ACTOR_LeaveKey(void) {
	actor->key_entity_id = 0;
	actor->key_graphics_id = SPRITE_GRAPHICS_ID_EMPTY;
}

void ACTOR_Reload(void) {
	actor->action_reload = true;
	actor->action_shoot = false;
	actor->action_loop = false;
}
