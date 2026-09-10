#include "../engine.h"
#include "effect.h"

Effect *effect;
int effects_counter;

void EFFECT_InitEffects(void) {
	int i;
	effect = MM_PushChunk(sizeof(Effect) * EFFECT_MAX_EFFECTS, CT_ENGINE);

	for (i = 0; i < EFFECT_MAX_EFFECTS; i++) {
		effect[i].is_loaded = false;
		effect[i].num_sprite = -1;
	}
}

/** EFFECT :: Load effect
 */
void EFFECT_LoadEffect(byte entity_id, byte graphics_id, int pos_x, int pos_y, bool full_animation, int frame, bool inverted, bool loop, int speed) {
	int i, number, sprite_slot;

	// Search for a slot
	number = 999;
	for (i = 0; i < EFFECT_MAX_EFFECTS; i++) {
		if (!effect[i].is_loaded) {
			number = i;
			break;
		}
	}

	// No space for more effects. Just do not load it
	if (number == 999) return;

	effect[number].is_loaded = true;
	effect[number].type = entity_id;
	effect[number].pos_x = pos_x;
	effect[number].pos_y = pos_y;

	// Check if graphics id is already loaded
	if (!gfx_sprite_graphics_stack[graphics_id].loaded) {
		sprintf(engine.system_error_message1, "EFFECT_LoadEffect function error");
		sprintf(engine.system_error_message2, "Graphics id %u not loaded", graphics_id);
		sprintf(engine.system_error_message3, "");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
	}

	// Search for an empty sprite slot and initialize it
	sprite_slot = GFX_FindEmptySpriteSlot();
	if (gfx_sprite_stack[sprite_slot].loaded || (sprite_slot == -1)) {
		sprintf(engine.system_error_message1, "EFFECT_LoadEffect function error");
		sprintf(engine.system_error_message2, "No empty sprite slot available");
		sprintf(engine.system_error_message3, "");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
	} else {
		// Initialize effect sprite
		GFX_InitSprite(entity_id, number, sprite_slot, 1, gfx_sprite_graphics_stack[graphics_id].width_px, gfx_sprite_graphics_stack[graphics_id].height_px);
	}
	effect[number].num_sprite = sprite_slot;

	// Set effect graphics
	GFX_SetSpriteGraphic(sprite_slot, 0, graphics_id, 0, 0);
	// Set initial screen position
	GFX_SetSpritePosition(sprite_slot, pos_x - camera->pos_x, pos_y - camera->pos_y);
	if (full_animation) GFX_SetDefaultAnimation(effect[number].num_sprite, inverted, loop, speed);
	else
		GFX_SetSingleFrameAnimation(sprite_slot, frame);
}

void EFFECT_UnloadEffect(int number) {
	if (effect[number].is_loaded) {
		GFX_UnloadSprite(effect[number].num_sprite);
		effect[number].is_loaded = false;
		effect[number].num_sprite = -1;
	}
}


/** EFFECT :: Update effects
 */
void EFFECT_UpdateEffects(void) {
	int i;
	effects_counter = 0;
	// Calculate screen pos and evaluate visibility
	for (i = 0; i < EFFECT_MAX_EFFECTS; i++) {
		if (effect[i].is_loaded) {
			effects_counter++;
			// Update current screen position
			GFX_SetSpritePosition(effect[i].num_sprite, effect[i].pos_x - camera->pos_x, effect[i].pos_y - camera->pos_y);
			if (GFX_IsSpriteAnimationEnded(effect[i].num_sprite, 0)) EFFECT_UnloadEffect(i);
		}
	}
}

/** EFFECT :: Unload effects
 */
void EFFECT_UnloadEffects(void) {
	int i;
	// Calculate screen pos and evaluate visibility
	for (i = 0; i < EFFECT_MAX_EFFECTS; i++) {
		if (effect[i].is_loaded) {
			GFX_UnloadSprite(effect[i].num_sprite);
			effect[i].is_loaded = false;
			effect[i].num_sprite = -1;
		}
	}
}

bool EFFECT_NoEffectsActive(void) {
	int i;
	for (i = 0; i < EFFECT_MAX_EFFECTS; i++) {
		if (effect[i].is_loaded) {
			return false;
		}
	}
	return true;
}