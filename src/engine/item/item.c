#include "../engine.h"
#include "item.h"

Item item[ITEM_MAX_ITEMS];
static byte item_update_counter;
byte item_counter;

/** ITEM :: Load item
 */
void ITEM_LoadItem(byte number, byte entity_id, byte graphics_id, int pos_x, int pos_y) {
	int i;

	// Check if item is already loaded
	if (item[number].is_loaded) {
		sprintf(engine.system_error_message1, "ITEM_LoadItem function error");
		sprintf(engine.system_error_message2, "Item id %u already loaded", number);
		sprintf(engine.system_error_message3, "Current scene %u, room %u", engine.scene, engine.room);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
	}

	item[number].type = entity_id;
	item[number].is_loaded = true;
	item[number].pos_x = pos_x;
	item[number].pos_y = pos_y;

	// Check if graphics id is already loaded
	if (!gfx_sprite_graphics_stack[graphics_id].loaded) {
		sprintf(engine.system_error_message1, "ITEM_LoadItem function error");
		sprintf(engine.system_error_message2, "Graphics id %u not loaded", graphics_id);
		sprintf(engine.system_error_message3, "...");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
	}

	item[number].num_sprite = GFX_FindEmptySpriteSlot();
	if (item[number].num_sprite == -1) {
		sprintf(engine.system_error_message1, "ITEM_LoadItem function error");
		sprintf(engine.system_error_message2, "No empty sprite slot available");
		sprintf(engine.system_error_message3, " ");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
	} else {
		GFX_InitSprite(entity_id, number, item[number].num_sprite, 0, gfx_sprite_graphics_stack[graphics_id].width_px, gfx_sprite_graphics_stack[graphics_id].height_px);
	}

	// Set colission points
	//  ----------------
	// |  0           1 |
	// |                |
	// |                |
	// |                |
	// |  2           3 |
	//  ----------------
	// Point 0
	item[number].colission_area.points[0][0] = 0;
	item[number].colission_area.points[0][1] = 0;
	// Point 1
	item[number].colission_area.points[1][0] = gfx_sprite_stack[item[number].num_sprite].width_px;
	item[number].colission_area.points[1][1] = 0;

	// Point 2
	item[number].colission_area.points[2][0] = 0;
	item[number].colission_area.points[2][1] = gfx_sprite_stack[item[number].num_sprite].height_px;
	// Point 3
	item[number].colission_area.points[3][0] = gfx_sprite_stack[item[number].num_sprite].width_px;
	item[number].colission_area.points[3][1] = gfx_sprite_stack[item[number].num_sprite].height_px;

	GFX_SetSpriteGraphic(item[number].num_sprite, 0, graphics_id, 0, 0);// feet
	for (i = 1; i < 5; i++) {
		gfx_sprite_stack[item[number].num_sprite].gfx[i].graphics_id = -1;
		gfx_sprite_stack[item[number].num_sprite].gfx[i].offset_x = 0;
		gfx_sprite_stack[item[number].num_sprite].gfx[i].offset_y = 0;
		gfx_sprite_stack[item[number].num_sprite].gfx[i].screen_pos_x = -1;
		gfx_sprite_stack[item[number].num_sprite].gfx[i].screen_pos_y = -1;
	}

	// Set initial screen position
	gfx_sprite_stack[item[number].num_sprite].screen_pos_x = pos_x - camera->pos_x;
	gfx_sprite_stack[item[number].num_sprite].screen_pos_y = pos_y - camera->pos_y;

	GFX_SetDefaultAnimation(item[number].num_sprite, false, true, 20);
}

/** ITEM :: Update items
 */
void ITEM_UpdateItems(void) {
	int i;

	item_counter = 0;
	// Calculate screen pos and evaluate visibility
	for (i = 0; i < ITEM_MAX_ITEMS; i++) {
		if (item[i].is_loaded) {
			item_counter++;

			// Update enemy position on screen
			GFX_SetSpritePosition(item[i].num_sprite, item[i].pos_x - camera->pos_x, item[i].pos_y - camera->pos_y);

			// On screen
			item[i].on_screen = GFX_IsSpriteOnScreen(item[i].num_sprite);

			// DEBUG: Draw collision and hit pixels
			//if (item[i].on_screen) ITEM_DrawColissionPixels(item[i]);
		}
	}
}

void ITEM_UnloadItems(void) {
	int i;
	for (i = 0; i < ITEM_MAX_ITEMS; i++) {
		if (item[i].is_loaded) {
			GFX_UnloadSprite(item[i].num_sprite);
			item[i].is_loaded = false;
			item[i].num_sprite = -1;
		}
	}
}

void ITEM_UnloadItem(int number) {

	if (item[number].is_loaded) {
		GFX_UnloadSprite(item[number].num_sprite);
		item[number].is_loaded = false;
		item[number].num_sprite = -1;
	}
}

bool ITEM_IsLoaded(int number) {
	return item[number].is_loaded;
}

bool ITEM_IsOnScreen(int number) {
	if (item[number].is_loaded) {
		return item[number].on_screen;
	} else {
		return false;
	}
}

void ITEM_SetPosition(int number, int pos_x, int pos_y) {
	if (item[number].is_loaded) {
		item[number].pos_x = pos_x;
		item[number].pos_y = pos_y;
	}
}