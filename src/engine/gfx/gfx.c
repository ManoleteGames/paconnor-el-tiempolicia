#include "../engine.h"
#include "gfx.h"
#include "../video/vga.h"
#include "string.h"

Graphics *gfx;

StatusPanel *gfx_actor_status_panel;
StatusPanel *gfx_enemy_status_panel;
ChatPanel *gfx_chat_panel;

static int gfx_sprites_priority_index;
static byte gfx_sprites_priority_stack[SPRITE_MAX_STACK];

Graphic *gfx_sprite_graphics_stack;

Sprite *gfx_sprite_stack;
Sprite *gfx_sprite_cursor;

int gfx_sprite_counter;

/** GFX :: Initialize graphics
 */
void GFX_Init(void) {
	int i, j;

	gfx = MM_PushChunk(sizeof(Graphics), CT_ENGINE);
	gfx_sprite_graphics_stack = MM_PushChunk(sizeof(Graphic) * SPRITE_MAX_GRAPHICS, CT_ENGINE);
	gfx_sprite_stack = MM_PushChunk(sizeof(Sprite) * SPRITE_MAX_STACK, CT_ENGINE);
	for (i = 0; i < SPRITE_MAX_STACK; i++) {
		gfx_sprite_stack[i].gfx = MM_PushChunk(sizeof(SpriteGfx) * 6, CT_ENGINE);
	}

	gfx_sprite_cursor = MM_PushChunk(sizeof(Sprite), CT_ENGINE);
	gfx_sprite_cursor->gfx = MM_PushChunk(sizeof(SpriteGfx) * 5, CT_ENGINE);

	// Reserve memory for palette
	gfx->palette_shown = MM_PushChunk(256 * 3, CT_ENGINE);
	gfx->palette_loaded = MM_PushChunk(256 * 3, CT_ENGINE);
	// Reserve memory for image buffer 1
	gfx->image_buffer1 = MM_PushChunk(360 * 240, CT_ENGINE);
	gfx->image_buffer1_width = 0;
	gfx->image_buffer1_height = 0;
	// Reserve memory for image buffer 2
	gfx->image_buffer2 = MM_PushChunk(360 * 240, CT_ENGINE);
	gfx->image_buffer2_width = 0;
	gfx->image_buffer2_height = 0;
	// Reserve memory for image buffer 3
	gfx->image_buffer3 = MM_PushChunk(360 * 240, CT_ENGINE);
	gfx->image_buffer3_width = 0;
	gfx->image_buffer3_height = 0;

	gfx_actor_status_panel = MM_PushChunk(sizeof(StatusPanel), CT_ENGINE);
	gfx_enemy_status_panel = MM_PushChunk(sizeof(StatusPanel), CT_ENGINE);
	gfx_chat_panel = MM_PushChunk(sizeof(ChatPanel), CT_ENGINE);

	// Reserve memory for font
	gfx->font[0].data = MM_PushChunk(FONT_SLIM_GFX_WIDTH * FONT_SLIM_GFX_HEIGHT, CT_ENGINE);
	gfx->font[1].data = MM_PushChunk(FONT_SLIM_GFX_WIDTH * FONT_SLIM_GFX_HEIGHT, CT_ENGINE);
	gfx->font[2].data = MM_PushChunk(FONT_BIG_GFX_WIDTH * FONT_BIG_GFX_HEIGHT, CT_ENGINE);
	gfx->font[3].data = MM_PushChunk(FONT_BIG_GFX_WIDTH * FONT_BIG_GFX_HEIGHT, CT_ENGINE);
	gfx->font[4].data = MM_PushChunk(FONT_SLIM_GFX_WIDTH * FONT_SLIM_GFX_HEIGHT, CT_ENGINE);

	// Load default fonts
	GFX_LoadFont("FONTS.DAT", "SLIMB.PCX", 128 * 32, 8, 8, FONT_SLIM_BLACK);
	GFX_LoadFont("FONTS.DAT", "SLIMW.PCX", 128 * 32, 8, 8, FONT_SLIM_WHITE);
	GFX_LoadFont("FONTS.DAT", "BIGW.PCX", 256 * 64, 16, 16, FONT_BIG_WHITE);
	GFX_LoadFont("FONTS.DAT", "BIGB.PCX", 256 * 64, 16, 16, FONT_BIG_BLACK);
	GFX_LoadFont("FONTS.DAT", "SLIMY.PCX", 128 * 32, 8, 8, FONT_SLIM_YELLOW);

	// Reset sprite graphics stack
	for (i = 0; i < SPRITE_MAX_GRAPHICS; i++) {
		gfx_sprite_graphics_stack[i].loaded = false;
	}

	// Reset sprite stack
	for (i = 0; i < SPRITE_MAX_STACK; i++) {
		gfx_sprite_stack[i].loaded = false;

		for (j = 0; j < SPRITE_SUBSPRITES_MAX; j++) {
			gfx_sprite_stack[i].gfx[j].loaded = false;
			gfx_sprite_stack[i].gfx[j].graphics_id = -1;
		}
	}

	// Reset cursor sprite
	gfx_sprite_cursor->loaded = false;
	gfx_sprite_cursor->gfx[0].loaded = false;
	gfx_sprite_cursor->gfx[0].graphics_id = -1;
	gfx_sprite_cursor->gfx[1].loaded = false;
	gfx_sprite_cursor->gfx[1].graphics_id = -1;
	gfx_sprite_cursor->gfx[2].loaded = false;
	gfx_sprite_cursor->gfx[2].graphics_id = -1;
	gfx_sprite_cursor->gfx[3].loaded = false;
	gfx_sprite_cursor->gfx[3].graphics_id = -1;
	gfx_sprite_cursor->gfx[4].loaded = false;
	gfx_sprite_cursor->gfx[4].graphics_id = -1;
	gfx_sprite_cursor->gfx[5].loaded = false;
	gfx_sprite_cursor->gfx[5].graphics_id = -1;
}

/** GFX :: Load sprite graphics on the stack
 */
void GFX_LoadSpriteGraphicsRLE(const char *dat_name, const char *asset_name, int id, int width_px, int height_px, int num_frames, int transparent_color, int hit_color, int mem_type) {

	word image_width_px, image_height_px;
	int rows, columns;
	register int src_index;
	register int dst_index;
	register int i, j, k, l;
	register int tranp_color_counter;
	register int num_frame;
	register int row_index;
	register byte color;
	byte *s, *d;
	byte *dataLoaded;

	// Check validity of sprite number
	if ((id < 0) || (id >= SPRITE_MAX_GRAPHICS)) {
		sprintf(engine.system_error_message1, "GFX_LoadSpriteGraphicsRLE function error");
		sprintf(engine.system_error_message2, "Invalid sprite stack graphic id number %u", id);
		sprintf(engine.system_error_message3, "DAT filename: %s, Asset filename: %s", dat_name, asset_name);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
	}

	// Check if sprite is already loaded
	if (gfx_sprite_graphics_stack[id].loaded) {
		sprintf(engine.system_error_message1, "GFX_LoadSpriteGraphics function error");
		sprintf(engine.system_error_message2, "Sprite stack graphic id number %u already in use", id);
		sprintf(engine.system_error_message3, "DAT filename: %s, Asset filename: %s", dat_name, asset_name);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
	}

	gfx_sprite_graphics_stack[id].buffer = MM_PushChunk((width_px * height_px * num_frames) + 100, CT_SPRITE);// keep extra space because RLE compression
	gfx_sprite_graphics_stack[id].frame_offset = MM_PushChunk(num_frames << 1, CT_SPRITE);                    // *2 because are INT values
	gfx_sprite_graphics_stack[id].row_offset = MM_PushChunk((num_frames * height_px) << 1, CT_SPRITE);        // *2 because ase INT values

	// Initialize data
	memset(gfx_sprite_graphics_stack[id].buffer, 0, width_px * height_px * num_frames);
	memset(gfx_sprite_graphics_stack[id].frame_offset, 0, num_frames << 2);
	memset(gfx_sprite_graphics_stack[id].row_offset, 0, (num_frames * height_px) << 2);

	dataLoaded = MM_PushChunk(width_px * height_px * num_frames, CT_TEMPORARY);

	// Load graphics file
	FILE_LoadPCXSprite(dat_name, asset_name, dataLoaded, width_px * height_px * num_frames, &image_width_px, &image_height_px, SPRITE_TRANSP_COLOR);

	gfx_sprite_graphics_stack[id].num_frames = num_frames;
	gfx_sprite_graphics_stack[id].width_px = width_px;
	gfx_sprite_graphics_stack[id].height_px = height_px;
	gfx_sprite_graphics_stack[id].transparent_color = transparent_color;
	gfx_sprite_graphics_stack[id].hit_color = hit_color;

	rows = (image_height_px / height_px);
	columns = (image_width_px / width_px);

	// Transfer data to sprite raw graphics
	// Set destination
	dst_index = 0;
	num_frame = 0;
	row_index = 0;

	for (i = 0; i < rows; i++) {
		for (j = 0; j < columns; j++) {
			tranp_color_counter = 0;

			// Set source
			src_index = ((i * height_px) * image_width_px) + (j * width_px);  // raw source data index
			gfx_sprite_graphics_stack[id].frame_offset[num_frame] = dst_index;// indicates the begining of each frame

			s = &dataLoaded[src_index];
			d = &gfx_sprite_graphics_stack[id].buffer[dst_index];

			for (k = 0; k < height_px; k++) {

				gfx_sprite_graphics_stack[id].row_offset[row_index] = dst_index;// indicates the beginith of each row

				for (l = 0; l < width_px; l++) {
					color = *s++;
					switch (color) {
						case SPRITE_TRANSP_COLOR:
							//if (tranp_color_counter == 0) {// first transparent bunch
							//	*d++ = 1;
							//	dst_index++;
							//}
							tranp_color_counter++;
							break;
						default:                           // just a color
							if (tranp_color_counter == 1) {// overwrite transparent bunch code by color number
								*d++ = SPRITE_TRANSP_COLOR;
								dst_index++;
								tranp_color_counter = 0;
							}
							if (tranp_color_counter > 1) {// end of transparent bunch
								*d++ = 1;
								dst_index++;
								*d++ = tranp_color_counter;
								dst_index++;
								tranp_color_counter = 0;
							}
							*d++ = color;
							dst_index++;
							break;
					}
				}
				if (tranp_color_counter == 1) {
					*d++ = SPRITE_TRANSP_COLOR;
					dst_index++;
					tranp_color_counter = 0;
				}
				if (tranp_color_counter > 1) {
					*d++ = 1;
					dst_index++;
					*d++ = tranp_color_counter;
					dst_index++;
					tranp_color_counter = 0;
				}
				*d++ = 0;// Set end of scanline
				dst_index++;
				s += image_width_px - width_px;
				row_index++;

				if (dst_index >= ((width_px * height_px * num_frames) + 100)) {
					sprintf(engine.system_error_message1, "GFX_LoadSpriteGraphicsRLE function error");
					sprintf(engine.system_error_message2, "Loading graphic id %u", id);
					sprintf(engine.system_error_message3, "Max. length %u. Current index %u", width_px * height_px * num_frames, dst_index);
					Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
				}
			}
			num_frame++;
		}
	}
	// Set loaded flag
	gfx_sprite_graphics_stack[id].loaded = true;

	MM_PopChunks(CT_TEMPORARY);
}

void GFX_SetSpriteGraphic(int spr_num, int index, int graphics_id, int offset_x, int offset_y) {

	if (!gfx_sprite_graphics_stack[graphics_id].loaded) {
		sprintf(engine.system_error_message1, "GFX_SetSpriteGraphics function error");
		sprintf(engine.system_error_message2, "Graphic id %u not loaded", graphics_id);
		sprintf(engine.system_error_message3, "Sprite number: %u, Sprite index: %u", spr_num, index);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
	}
	gfx_sprite_stack[spr_num].gfx[index].graphics_id = graphics_id;
	gfx_sprite_stack[spr_num].gfx[index].width_px = gfx_sprite_graphics_stack[graphics_id].width_px;
	gfx_sprite_stack[spr_num].gfx[index].heigth_px = gfx_sprite_graphics_stack[graphics_id].height_px;
	gfx_sprite_stack[spr_num].gfx[index].invisible = false;
	gfx_sprite_stack[spr_num].gfx[index].offset_x = offset_x;
	gfx_sprite_stack[spr_num].gfx[index].offset_y = offset_y;
	gfx_sprite_stack[spr_num].gfx[index].screen_pos_x = gfx_sprite_stack[spr_num].screen_pos_x + offset_x;
	gfx_sprite_stack[spr_num].gfx[index].screen_pos_y = gfx_sprite_stack[spr_num].screen_pos_y + offset_y;
	gfx_sprite_stack[spr_num].gfx[index].unmasked = false;
	gfx_sprite_stack[spr_num].gfx[index].loaded = true;
}

void GFX_SetSpriteCursorGraphic(int graphics_id, int offset_x, int offset_y) {

	if (!gfx_sprite_graphics_stack[graphics_id].loaded) {
		sprintf(engine.system_error_message1, "GFX_SetSpriteCursorGraphic function error");
		sprintf(engine.system_error_message2, "Graphic id %u not loaded", graphics_id);
		sprintf(engine.system_error_message3, "...");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
	}
	gfx_sprite_cursor->width_px = gfx_sprite_graphics_stack[graphics_id].width_px;
	gfx_sprite_cursor->height_px = gfx_sprite_graphics_stack[graphics_id].height_px;
	gfx_sprite_cursor->gfx[0].graphics_id = graphics_id;
	gfx_sprite_cursor->gfx[0].width_px = gfx_sprite_graphics_stack[graphics_id].width_px;
	gfx_sprite_cursor->gfx[0].heigth_px = gfx_sprite_graphics_stack[graphics_id].height_px;
	gfx_sprite_cursor->gfx[0].invisible = false;
	gfx_sprite_cursor->gfx[0].offset_x = offset_x;
	gfx_sprite_cursor->gfx[0].offset_y = offset_y;
	gfx_sprite_cursor->gfx[0].screen_pos_x = gfx_sprite_cursor->screen_pos_x + offset_x;
	gfx_sprite_cursor->gfx[0].screen_pos_y = gfx_sprite_cursor->screen_pos_y + offset_y;
	gfx_sprite_cursor->gfx[0].loaded = true;
	gfx_sprite_cursor->gfx[0].unmasked = true;
}

void GFX_SetPanelGraphics(StatusPanel *panel, int portait_graphics_id, int lifebar_graphics_id, int gun_graphics_id, int key_graphics_id) {
	panel->portait_graphics_id = portait_graphics_id;
	panel->lifebar_graphics_id = lifebar_graphics_id;
	panel->portait_frame = 0;
	panel->gun_graphics_id = gun_graphics_id;
	panel->key_graphics_id = key_graphics_id;
}

void GFX_SetPanelPosition(StatusPanel *panel, int x, int y, int portait_x, int portait_y, int lifebar_x, int lifebar_y, int gun_x, int gun_y, int grenade_x, int grenade_y, int key_x, int key_y) {

	panel->pos_x = x;
	panel->pos_y = y;

	panel->portait_x = portait_x;
	panel->portait_y = portait_y;
	panel->lifebar_x = lifebar_x;
	panel->lifebar_y = lifebar_y;
	panel->gun_x = gun_x;
	panel->gun_y = gun_y;
	panel->grenade_x = grenade_x;
	panel->grenade_y = grenade_y;
	panel->key_x = key_x;
	panel->key_y = key_y;
}

void GFX_ShowPanel(StatusPanel *panel, bool show, int timeout) {

	panel->life_speed = 1;
	panel->life_speed_counter = 0;
	panel->shown = show;
	panel->timeout = timeout;
}

void GFX_UpdatePanel(StatusPanel *panel, int life, int old_life, int max_life, int portait_frame) {
	panel->life_speed_counter = 0;
	panel->old_life = (old_life * 45) / max_life;
	panel->current_life = (life * 45) / max_life;
	panel->portait_frame = portait_frame - 1;
}

void GFX_UpdatePanelGun(StatusPanel *panel, int gun_graphics_id, int gun_type, int total_bullets, int bullets_number, int grenade_graphics_id, int grenades_number, int key_type, int key_graphics_id) {
	panel->gun_type = gun_type;
	panel->gun_graphics_id = gun_graphics_id;
	panel->bullets_number = bullets_number;
	panel->total_bullets = total_bullets;
	panel->grenade_graphics_id = grenade_graphics_id;
	panel->grenades_number = grenades_number;
	panel->key_type = key_type;
	panel->key_graphics_id = key_graphics_id;
}

void GFX_SetPanelPortait(StatusPanel *panel, int portait_frame) {
	panel->portait_frame = portait_frame - 1;
}

void GFX_SetChatGraphics(ChatPanel *panel, int portait_graphics_id, int chat_graphics_id) {
	panel->portait_graphics_id = portait_graphics_id;
	panel->chat_graphics_id = chat_graphics_id;
	panel->portait_frame = 0;
}

void GFX_SetChatPosition(ChatPanel *panel, int x, int y, int portait_x, int portait_y, bool portait_inverted, int chat_x, int chat_y, bool chat_inverted) {

	panel->pos_x = x;
	panel->pos_y = y;

	panel->portait_x = portait_x;
	panel->portait_y = portait_y;
	panel->portait_inverted = portait_inverted;

	panel->chat_x = chat_x;
	panel->chat_y = chat_y;
	panel->chat_inverted = chat_inverted;
}

void GFX_SetLineChatPanel(ChatPanel *panel, const char c[40], int line) {
	strcpy(panel->line[line], c);
	panel->line_length[line] = strlen(c);
	panel->line_count[line] = 0;
}

/** Unload sprite graphic
 *
 */
void GFX_UnloadSpriteGraphic(int graphics_id) {
	gfx_sprite_graphics_stack[graphics_id].loaded = false;
	gfx_sprite_graphics_stack[graphics_id].num_frames = 0;
	gfx_sprite_graphics_stack[graphics_id].height_px = 0;
	gfx_sprite_graphics_stack[graphics_id].width_px = 0;
}

/** Unload all sprite graphics
 *
 */
void GFX_UnloadSpriteGraphics(void) {
	int i, j;
	for (i = 0; i < SPRITE_MAX_STACK; i++) {
		for (j = 0; j < SPRITE_SUBSPRITES_MAX; j++) {
			if (gfx_sprite_stack[i].gfx[j].graphics_id != -1) {
				sprintf(engine.system_error_message1, "GFX_UnloadSpriteGraphics function error");
				sprintf(engine.system_error_message2, "Graphics assigned to sprites");
				sprintf(engine.system_error_message3, "Impossible unload all graphics");
				Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
			}
		}
	}
	for (i = 0; i < SPRITE_MAX_GRAPHICS; i++) {
		gfx_sprite_graphics_stack[i].loaded = false;
	}
}

int GFX_FindEmptySpriteSlot(void) {
	int i;
	for (i = 0; i < SPRITE_MAX_STACK; i++) {
		if (!gfx_sprite_stack[i].loaded) return i;
	}
	return -1;
}

/** GFX :: Initialize sprite on the stack
 */
void GFX_InitSprite(byte entity_id, byte entity_num, int number, byte priority, int width_px, int height_px) {
	int i, j;

	if (gfx_sprite_stack[number].loaded) {
		sprintf(engine.system_error_message1, "GFX_InitSprite function error");
		sprintf(engine.system_error_message2, "Sprite number %u already in use", number);
		sprintf(engine.system_error_message3, "Entity id: %u, Entity num: %u", entity_id, entity_num);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
	}

	gfx_sprite_stack[number].loaded = true;
	gfx_sprite_stack[number].entity_id = entity_id;
	gfx_sprite_stack[number].id = entity_num << 8 | entity_id;
	gfx_sprite_stack[number].priority = priority;

	gfx_sprite_stack[number].screen_pos_x = -99;
	gfx_sprite_stack[number].screen_pos_y = -99;
	gfx_sprite_stack[number].width_px = width_px;
	gfx_sprite_stack[number].height_px = height_px;

	gfx_sprite_stack[number].unmasked = false;
	gfx_sprite_stack[number].shown = false;

	gfx_sprite_stack[number].blink.active = false;
	gfx_sprite_stack[number].blink.blinked = false;
	gfx_sprite_stack[number].blink.counter = 0;
	gfx_sprite_stack[number].blink.time = 5;

	for (i = 0; i < SPRITE_SUBSPRITES_MAX; i++) {
		gfx_sprite_stack[number].gfx[i].loaded = false;
		gfx_sprite_stack[number].gfx[i].inverted = false;
		gfx_sprite_stack[number].gfx[i].blink = false;
		gfx_sprite_stack[number].gfx[i].invisible = false;
		gfx_sprite_stack[number].gfx[i].unmasked = true;
		gfx_sprite_stack[number].gfx[i].graphics_id = -1;
		gfx_sprite_stack[number].gfx[i].screen_pos_x = -99;
		gfx_sprite_stack[number].gfx[i].screen_pos_y = -99;
		gfx_sprite_stack[number].gfx[i].offset_x = 0;
		gfx_sprite_stack[number].gfx[i].offset_y = 0;

		gfx_sprite_stack[number].gfx[i].anim_current_frame = 0;
		gfx_sprite_stack[number].gfx[i].anim_counter = 0;
		gfx_sprite_stack[number].gfx[i].anim_frames = 0;
		gfx_sprite_stack[number].gfx[i].anim_end = false;
		gfx_sprite_stack[number].gfx[i].anim_loop = false;
		gfx_sprite_stack[number].gfx[i].anim_speed = 5;

		for (j = 0; j < SPRITE_ANIMATION_MAX_FRAMES; j++) {
			gfx_sprite_stack[number].gfx[i].anim_data[j].frame = 0;
			gfx_sprite_stack[number].gfx[i].anim_data[j].offset_x = 0;
			gfx_sprite_stack[number].gfx[i].anim_data[j].offset_y = 0;
		}

		gfx_sprite_stack[number].gfx_order[i] = i;
	}
}

/** GFX :: Initialize cursor sprite
 */
void GFX_InitCursorSprite(byte entity_id, byte entity_num, int width_px, int height_px) {
	int i;

	if (gfx_sprite_cursor->loaded) {
		sprintf(engine.system_error_message1, "GFX_InitCursorSprite function error");
		sprintf(engine.system_error_message2, "Cursor sprite already loaded");
		sprintf(engine.system_error_message3, "...");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
	}

	gfx_sprite_cursor->loaded = true;
	gfx_sprite_cursor->entity_id = entity_id;
	gfx_sprite_cursor->id = entity_num << 8 | entity_id;
	gfx_sprite_cursor->priority = 0;

	gfx_sprite_cursor->screen_pos_x = -99;
	gfx_sprite_cursor->screen_pos_y = -99;
	gfx_sprite_cursor->width_px = width_px;
	gfx_sprite_cursor->height_px = height_px;

	gfx_sprite_cursor->unmasked = false;
	gfx_sprite_cursor->shown = false;

	gfx_sprite_cursor->blink.active = false;
	gfx_sprite_cursor->blink.blinked = false;
	gfx_sprite_cursor->blink.counter = 0;
	gfx_sprite_cursor->blink.time = 5;

	for (i = 0; i < SPRITE_SUBSPRITES_MAX; i++) {
		gfx_sprite_cursor->gfx[i].loaded = false;
		gfx_sprite_cursor->gfx[i].graphics_id = -1;
		gfx_sprite_cursor->gfx_order[i] = i;
	}
}

/** GFX :: Unload sprite 
 */
void GFX_UnloadSprite(int number) {
	int i;

	if (number > SPRITE_MAX_STACK) {
		sprintf(engine.system_error_message1, "GFX_UnloadSprite function error");
		sprintf(engine.system_error_message2, "Impossible unloading sprite number %u", number);
		sprintf(engine.system_error_message3, "...");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
	}

	if (number != -1) {
		for (i = 0; i < SPRITE_SUBSPRITES_MAX; i++) {
			gfx_sprite_stack[number].gfx[i].loaded = false;
			gfx_sprite_stack[number].gfx[i].graphics_id = -1;
		}
		gfx_sprite_stack[number].loaded = false;
		gfx_sprite_stack[number].shown = false;
	}
}

/** GFX :: Unload all sprites 
 */
void GFX_UnloadSprites(void) {
	int i, j;
	for (i = 0; i < SPRITE_MAX_STACK; i++) {
		for (j = 0; j < SPRITE_SUBSPRITES_MAX; j++) {
			gfx_sprite_stack[i].gfx[j].loaded = false;
			gfx_sprite_stack[i].gfx[j].graphics_id = -1;
		}
		gfx_sprite_stack[i].loaded = false;
		gfx_sprite_stack[i].shown = false;
	}
}

void GFX_SetSpriteGfxOrder(int sprite_num, int one, int two, int three, int four, int five) {
	gfx_sprite_stack[sprite_num].gfx_order[0] = one;
	gfx_sprite_stack[sprite_num].gfx_order[1] = two;
	gfx_sprite_stack[sprite_num].gfx_order[2] = three;
	gfx_sprite_stack[sprite_num].gfx_order[3] = four;
	gfx_sprite_stack[sprite_num].gfx_order[4] = five;
}

bool GFX_IsSpriteOnScreen(int sprite_num) {
	return gfx_sprite_stack[sprite_num].shown;
}

bool GFX_IsBlinkActive(int sprite_num) {
	return gfx_sprite_stack[sprite_num].blink.active;
}

/** GFX :: Update sprites
 */
void GFX_UpdateSprites(void) {
	byte aux;
	int i, j;
	int index;

	gfx_sprite_counter = 0;

	// Reset stack
	gfx_sprites_priority_index = 0;
	memset(gfx_sprites_priority_stack, 0, SPRITE_MAX_STACK);

	// Calculate screen pos and evaluate visibility
	for (i = 0; i < SPRITE_MAX_STACK; i++) {
		if (gfx_sprite_stack[i].loaded) {// Sprite loaded

			gfx_sprite_counter++;

			if ((gfx_sprite_stack[i].screen_pos_x + gfx_sprite_stack[i].width_px > 0)

				&& (gfx_sprite_stack[i].screen_pos_x < camera->visible_width)

				&& ((gfx_sprite_stack[i].screen_pos_y + gfx_sprite_stack[i].height_px) > 0)

				&& (gfx_sprite_stack[i].screen_pos_y < camera->visible_height)) {

				/*if ((gfx_sprite_stack[i].screen_pos_x > 0)

				&& (gfx_sprite_stack[i].screen_pos_x + gfx_sprite_stack[i].width_px < camera->visible_width)

				&& ((gfx_sprite_stack[i].screen_pos_y > 0)

					&& (gfx_sprite_stack[i].screen_pos_y + gfx_sprite_stack[i].height_px) < camera->visible_height)) {*/


				gfx_sprite_stack[i].shown = true;

				// Add to stack
				gfx_sprites_priority_stack[gfx_sprites_priority_index] = i;
				gfx_sprites_priority_index++;

			} else {
				gfx_sprite_stack[i].shown = false;

				// Add to stack
				gfx_sprites_priority_stack[gfx_sprites_priority_index] = i;
				gfx_sprites_priority_index++;
			}
		} else {
			gfx_sprite_stack[i].shown = false;
		}
	}

	// Set sprite priority stack by depth
	for (i = 0; i < gfx_sprites_priority_index; i++) {
		for (j = 0; j < gfx_sprites_priority_index - 1; j++) {
			if ((gfx_sprite_stack[gfx_sprites_priority_stack[j]].screen_pos_y + gfx_sprite_stack[gfx_sprites_priority_stack[j]].height_px) > (gfx_sprite_stack[gfx_sprites_priority_stack[j + 1]].screen_pos_y) + gfx_sprite_stack[gfx_sprites_priority_stack[j + 1]].height_px) {

				aux = gfx_sprites_priority_stack[j + 1];
				gfx_sprites_priority_stack[j + 1] = gfx_sprites_priority_stack[j];
				gfx_sprites_priority_stack[j] = aux;
			}
		}
	}

	// Set sprite priority stack by priority value
	for (i = 0; i < gfx_sprites_priority_index; i++) {
		for (j = 0; j < gfx_sprites_priority_index - 1; j++) {
			if (gfx_sprite_stack[gfx_sprites_priority_stack[j]].priority > gfx_sprite_stack[gfx_sprites_priority_stack[j + 1]].priority) {
				aux = gfx_sprites_priority_stack[j + 1];
				gfx_sprites_priority_stack[j + 1] = gfx_sprites_priority_stack[j];
				gfx_sprites_priority_stack[j] = aux;
			}
		}
	}

	// Update animation frames
	for (i = 0; i < gfx_sprites_priority_index; i++) {
		index = gfx_sprites_priority_stack[i];

		//Check if it is blinking
		// blink
		if (gfx_sprite_stack[index].blink.active) {
			gfx_sprite_stack[index].blink.counter++;
			if (gfx_sprite_stack[index].blink.counter > gfx_sprite_stack[index].blink.time) {
				gfx_sprite_stack[index].blink.counter = 0;
				if (gfx_sprite_stack[index].blink.blinked) gfx_sprite_stack[index].blink.blinked = false;
				else
					gfx_sprite_stack[index].blink.blinked = true;
			}
		} else {
			gfx_sprite_stack[index].blink.blinked = false;
		}

		// Update graphics
		for (j = 0; j < SPRITE_SUBSPRITES_MAX; j++) {
			if (gfx_sprite_stack[index].gfx[j].loaded) {

				// Update animation
				if (!gfx_sprite_stack[index].gfx[j].anim_end) {
					gfx_sprite_stack[index].gfx[j].anim_counter++;
					if (gfx_sprite_stack[index].gfx[j].anim_counter >= gfx_sprite_stack[index].gfx[j].anim_speed) {
						gfx_sprite_stack[index].gfx[j].anim_counter = 0;
						gfx_sprite_stack[index].gfx[j].anim_current_frame++;
						if (gfx_sprite_stack[index].gfx[j].anim_current_frame >= gfx_sprite_stack[index].gfx[j].anim_frames) {
							if (gfx_sprite_stack[index].gfx[j].anim_loop) {
								gfx_sprite_stack[index].gfx[j].anim_current_frame = 0;
							} else {
								gfx_sprite_stack[index].gfx[j].anim_end = true;
								gfx_sprite_stack[index].gfx[j].anim_current_frame--;
							}
						}
					}
				}

				// Set graphics frame data
				gfx_sprite_stack[index].gfx[j].frame = gfx_sprite_stack[index].gfx[j].anim_data[gfx_sprite_stack[index].gfx[j].anim_current_frame].frame;
				gfx_sprite_stack[index].gfx[j].offset_x = gfx_sprite_stack[index].gfx[j].anim_data[gfx_sprite_stack[index].gfx[j].anim_current_frame].offset_x;
				gfx_sprite_stack[index].gfx[j].offset_y = gfx_sprite_stack[index].gfx[j].anim_data[gfx_sprite_stack[index].gfx[j].anim_current_frame].offset_y;
				gfx_sprite_stack[index].gfx[j].inverted = gfx_sprite_stack[index].gfx[j].inverted;
				gfx_sprite_stack[index].gfx[j].blink = gfx_sprite_stack[index].blink.blinked;

				// update screen position
				gfx_sprite_stack[index].gfx[j].screen_pos_x = gfx_sprite_stack[index].screen_pos_x + gfx_sprite_stack[index].gfx[j].offset_x;
				gfx_sprite_stack[index].gfx[j].screen_pos_y = gfx_sprite_stack[index].screen_pos_y + gfx_sprite_stack[index].gfx[j].offset_y;
			}
		}
	}
}

/** GFX :: Update sprites
 */
void GFX_UpdateCursorSprite(void) {
	int j;

	gfx_sprite_counter = 0;

	if (gfx_sprite_cursor->loaded) {// Sprite loaded

		if ((gfx_sprite_cursor->screen_pos_x + gfx_sprite_cursor->width_px > 0)

			&& (gfx_sprite_cursor->screen_pos_x < camera->visible_width)

			&& ((gfx_sprite_cursor->screen_pos_y + gfx_sprite_cursor->height_px) > 0)

			&& (gfx_sprite_cursor->screen_pos_y < camera->visible_height)) {

			gfx_sprite_cursor->shown = true;
		} else {
			gfx_sprite_cursor->shown = false;
		}
	} else {
		gfx_sprite_cursor->shown = false;
	}

	//Check if it is blinking
	// blink
	if (gfx_sprite_cursor->blink.active) {
		gfx_sprite_cursor->blink.counter++;
		if (gfx_sprite_cursor->blink.counter > gfx_sprite_cursor->blink.time) {
			gfx_sprite_cursor->blink.counter = 0;
			if (gfx_sprite_cursor->blink.blinked) gfx_sprite_cursor->blink.blinked = false;
			else
				gfx_sprite_cursor->blink.blinked = true;
		}
	} else {
		gfx_sprite_cursor->blink.blinked = false;
	}

	// Update graphics
	for (j = 0; j < SPRITE_SUBSPRITES_MAX; j++) {
		if (gfx_sprite_cursor->gfx[j].loaded) {

			// Update animation
			if (!gfx_sprite_cursor->gfx[j].anim_end) {
				gfx_sprite_cursor->gfx[j].anim_counter++;
				if (gfx_sprite_cursor->gfx[j].anim_counter >= gfx_sprite_cursor->gfx[j].anim_speed) {
					gfx_sprite_cursor->gfx[j].anim_counter = 0;
					gfx_sprite_cursor->gfx[j].anim_current_frame++;

					if (gfx_sprite_cursor->gfx[j].anim_current_frame >= gfx_sprite_cursor->gfx[j].anim_frames) {

						if (gfx_sprite_cursor->gfx[j].anim_loop) {
							gfx_sprite_cursor->gfx[j].anim_current_frame = 0;
						} else {
							gfx_sprite_cursor->gfx[j].anim_end = true;
							gfx_sprite_cursor->gfx[j].anim_current_frame = gfx_sprite_cursor->gfx[j].anim_frames;
							gfx_sprite_cursor->gfx[j].anim_current_frame = 0;
						}
					}
				}
			}

			// Set graphics frame data
			gfx_sprite_cursor->gfx[j].frame = gfx_sprite_cursor->gfx[j].anim_data[gfx_sprite_cursor->gfx[j].anim_current_frame].frame;
			gfx_sprite_cursor->gfx[j].offset_x = gfx_sprite_cursor->gfx[j].anim_data[gfx_sprite_cursor->gfx[j].anim_current_frame].offset_x;
			gfx_sprite_cursor->gfx[j].offset_y = gfx_sprite_cursor->gfx[j].anim_data[gfx_sprite_cursor->gfx[j].anim_current_frame].offset_y;
			gfx_sprite_cursor->gfx[j].inverted = gfx_sprite_cursor->gfx[j].inverted;
			gfx_sprite_cursor->gfx[j].blink = gfx_sprite_cursor->blink.blinked;

			// update screen position
			gfx_sprite_cursor->gfx[j].screen_pos_x = gfx_sprite_cursor->screen_pos_x + gfx_sprite_cursor->gfx[j].offset_x;
			gfx_sprite_cursor->gfx[j].screen_pos_y = gfx_sprite_cursor->screen_pos_y + gfx_sprite_cursor->gfx[j].offset_y;
		}
	}
}

/** GFX :: Draw sprites 
 */
void GFX_DrawSprites(void) {
	int i, j, sprite_num, gfx;
	for (i = 0; i < gfx_sprites_priority_index; i++) {
		sprite_num = gfx_sprites_priority_stack[i];
		if (gfx_sprite_stack[sprite_num].shown) {
			for (j = 0; j < SPRITE_SUBSPRITES_MAX; j++) {
				gfx = gfx_sprite_stack[gfx_sprites_priority_stack[i]].gfx_order[j];
				if (gfx_sprite_stack[gfx_sprites_priority_stack[i]].gfx[gfx].loaded) {
					VIDEO_DrawSpriteToScreenBufferRLE(&gfx_sprite_graphics_stack[gfx_sprite_stack[sprite_num].gfx[gfx].graphics_id], &gfx_sprite_stack[sprite_num].gfx[gfx]);
				}
			}
		}
	}
}

/** GFX :: Draw cursor sprite
 */
void GFX_DrawCursorSprite(void) {
	if (gfx_sprite_cursor->shown) {
		VIDEO_DrawSpriteToScreenBufferRLE(&gfx_sprite_graphics_stack[gfx_sprite_cursor->gfx[0].graphics_id], &gfx_sprite_cursor->gfx[0]);
	}
}

/** GFX :: Load image
 *  - PCX file inside a DAT file required
 *  - Loads image into RAM video buffer   
 */
void GFX_LoadImage(const char *filename, const char *subfile, int size) {
	int i;
	int src_index, dst_index;
	word width, height;

	byte *dataLoaded = MM_PushChunk(size, CT_TEMPORARY);
	FILE_LoadPCXImage(filename, subfile, dataLoaded, size, &width, &height);
	// Transfer data to video buffer
	src_index = 0;
	dst_index = 0;
	for (i = 0; i < height; i++) {
		memcpy(&video->map_buffer[MAP_BACKGROUND_LAYER][dst_index], &dataLoaded[src_index], width);
		src_index += width;
		dst_index += video->map_buffer_width;
	}

	MM_PopChunks(CT_TEMPORARY);
}

/** GFX :: Load font
 *  - PCX file inside a DAT file required
 *  - Loads image into RAM video buffer   
 */
void GFX_LoadFont(const char *filename, const char *subfile, int size, word char_w, word char_h, int font_number) {

	word width;
	word height;
	register int src_index;
	register int dst_index;
	register int c, r, j, k;
	register int num_char_columns, num_char_rows;

	// Check font number is lower than limit
	if (font_number > 4) {
		sprintf(engine.system_error_message1, "GFX_LoadFont function error");
		sprintf(engine.system_error_message2, "Font number %u not allowed", font_number);
		sprintf(engine.system_error_message3, " ");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_VIDEO);
	}

	byte *dataLoaded = MM_PushChunk(size, CT_TEMPORARY);
	FILE_LoadPCXImage(filename, subfile, dataLoaded, size, &width, &height);

	gfx->font[font_number].char_width = char_w;
	gfx->font[font_number].char_height = char_h;

	num_char_columns = width / char_w;
	num_char_rows = height / char_h;

	src_index = 0;
	dst_index = 0;

	// regular mode
	for (r = 0; r < num_char_rows; r++) {
		for (c = 0; c < num_char_columns; c++) {
			for (j = 0; j < char_h; j++) {
				for (k = 0; k < char_w; k++) {
					src_index = ((width * char_h) * r) + (c * char_w) + (j * width) + k;
					gfx->font[font_number].data[dst_index] = dataLoaded[src_index] + FONT_TRANSPARENT_COLOR;// Apply offset transparent color
					dst_index++;
				}
			}
		}
	}

	MM_PopChunks(CT_TEMPORARY);
}

/** GFX :: Load palette
 */
void GFX_LoadPalette(const char *dat_file, const char *asset_file, int size) {
	FILE_LoadPCXPalette(dat_file, asset_file, gfx->palette_loaded, size);
}

/** GFX :: PCX Image to image buffer
 *  - Loads a pcx image file and print it to a memory buffer
 */
/** Load and display specified text-mode screen */
void GFX_PCXImageToBuffer(const char *dat_name, const char *asset_name, int size, byte *buffer, word *width, word *height) {
	FILE_LoadPCXImage(dat_name, asset_name, buffer, size, width, height);

	if (size < *width * *height) {
		sprintf(engine.system_error_message1, "GFX_PCXImageToBuffer function error");
		sprintf(engine.system_error_message2, "Image size is bigger than memory size set");
		sprintf(engine.system_error_message3, " ");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_VIDEO);
	}
}

/** GFX :: Clear buffer
 */
void GFX_ClearBuffer(byte *buffer, int width, int heigth, byte clear_byte) {
	memset(buffer, clear_byte, width * heigth);
}

void GFX_SetSpritePosition(int sprite_num, int screen_pos_x, int screen_pos_y) {
	int i;
	gfx_sprite_stack[sprite_num].screen_pos_x = screen_pos_x;
	gfx_sprite_stack[sprite_num].screen_pos_y = screen_pos_y;
	for (i = 0; i < SPRITE_SUBSPRITES_MAX; i++) {
		if (gfx_sprite_stack[sprite_num].gfx[i].loaded) {
			gfx_sprite_stack[sprite_num].gfx[i].screen_pos_x = screen_pos_x + gfx_sprite_stack[sprite_num].gfx[i].offset_x;
			gfx_sprite_stack[sprite_num].gfx[i].screen_pos_y = screen_pos_y + gfx_sprite_stack[sprite_num].gfx[i].offset_y;
		}
	}
}

void GFX_SetCursorSpritePosition(int screen_pos_x, int screen_pos_y) {
	gfx_sprite_cursor->screen_pos_x = screen_pos_x;
	gfx_sprite_cursor->screen_pos_y = screen_pos_y;
	gfx_sprite_cursor->gfx[0].screen_pos_x = screen_pos_x;
	gfx_sprite_cursor->gfx[0].screen_pos_y = screen_pos_y;
}

/** GFX :: Set sprite frame
 *  - Set a single frame for a sprite animation, with no offset and default speed, loop and inverted values
 */
void GFX_SetSpriteFrame(int sprite_num, int anim_number, int frame) {

	if (anim_number < 0 || anim_number >= SPRITE_SUBSPRITES_MAX) {
		sprintf(engine.system_error_message1, "GFX_SetSpriteFrame function error");
		sprintf(engine.system_error_message2, "Invalid animation number %u", anim_number);
		sprintf(engine.system_error_message3, "Sprite number: %u", sprite_num);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
	}

	gfx_sprite_stack[sprite_num].gfx[anim_number].anim_frames = 0;
	gfx_sprite_stack[sprite_num].gfx[anim_number].anim_current_frame = 0;
	gfx_sprite_stack[sprite_num].gfx[anim_number].anim_data[0].frame = frame;
	gfx_sprite_stack[sprite_num].gfx[anim_number].anim_data[0].offset_x = 0;
	gfx_sprite_stack[sprite_num].gfx[anim_number].anim_data[0].offset_y = 0;
}

/** GFX :: Set cursor sprite frame
 *  - Set a single frame for a sprite animation, with no offset and default speed, loop and inverted values
 */
void GFX_SetCursorSpriteFrame(int anim_number, int frame) {

	if (anim_number < 0 || anim_number >= SPRITE_SUBSPRITES_MAX) {
		sprintf(engine.system_error_message1, "GFX_SetCursorSpriteFrame function error");
		sprintf(engine.system_error_message2, "Invalid animation number %u", anim_number);
		sprintf(engine.system_error_message3, "...");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
	}

	if (anim_number > gfx_sprite_graphics_stack[gfx_sprite_cursor->gfx[anim_number].graphics_id].num_frames) {
		sprintf(engine.system_error_message1, "GFX_SetCursorSpriteFrame function error");
		sprintf(engine.system_error_message2, "Frame number %u out of limits ", frame);
		sprintf(engine.system_error_message3, "Max frame number %u", gfx_sprite_graphics_stack[gfx_sprite_cursor->gfx[anim_number].graphics_id].num_frames);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
	}

	gfx_sprite_cursor->gfx[anim_number].anim_frames = 0;
	gfx_sprite_cursor->gfx[anim_number].anim_current_frame = 0;
	gfx_sprite_cursor->gfx[anim_number].anim_data[anim_number].frame = frame;
	gfx_sprite_cursor->gfx[anim_number].anim_data[anim_number].offset_x = 0;
	gfx_sprite_cursor->gfx[anim_number].anim_data[anim_number].offset_y = 0;
}

/** GFX :: Set default animation for a sprite
 *  - Default animation is set according to the number of frames in the graphics assigned to the sprite.
 *    Each frame in the graphics will be a frame in the animation, with no offset and default
 */
void GFX_SetDefaultAnimation(int sprite_num, bool inverted, bool loop, byte speed) {
	int i, j;
	for (i = 0; i < SPRITE_SUBSPRITES_MAX; i++) {
		if (gfx_sprite_stack[sprite_num].gfx[i].loaded) {

			if (gfx_sprite_stack[sprite_num].gfx[i].graphics_id < 0 || gfx_sprite_stack[sprite_num].gfx[i].graphics_id >= SPRITE_MAX_GRAPHICS) {
				sprintf(engine.system_error_message1, "GFX_SetDefaultAnimation function error");
				sprintf(engine.system_error_message2, "Graphics id %u out of bounds", gfx_sprite_stack[sprite_num].gfx[i].graphics_id);
				sprintf(engine.system_error_message3, "Sprite number: %u, Sprite index: %u", sprite_num, i);
				Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
			}

			if (!gfx_sprite_graphics_stack[gfx_sprite_stack[sprite_num].gfx[i].graphics_id].loaded) {
				sprintf(engine.system_error_message1, "GFX_SetDefaultAnimation function error");
				sprintf(engine.system_error_message2, "Graphics id %u not loaded", gfx_sprite_stack[sprite_num].gfx[i].graphics_id);
				sprintf(engine.system_error_message3, "Sprite number: %u, Sprite index: %u", sprite_num, i);
				Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
			}

			if (gfx_sprite_graphics_stack[gfx_sprite_stack[sprite_num].gfx[i].graphics_id].num_frames > SPRITE_ANIMATION_MAX_FRAMES) {
				sprintf(engine.system_error_message1, "GFX_SetDefaultAnimation function error");
				sprintf(engine.system_error_message2, "Number of frames in graphics id %u exceeds maximum animation frames", gfx_sprite_stack[sprite_num].gfx[i].graphics_id);
				sprintf(engine.system_error_message3, "Sprite number: %u, Sprite index: %u", sprite_num, i);
				Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
			}

			gfx_sprite_stack[sprite_num].gfx[i].anim_current_frame = 0;
			gfx_sprite_stack[sprite_num].gfx[i].anim_frames = gfx_sprite_graphics_stack[gfx_sprite_stack[sprite_num].gfx[i].graphics_id].num_frames;

			// Set frames according to graphics frames
			for (j = 0; j < gfx_sprite_stack[sprite_num].gfx[i].anim_frames; j++) {
				gfx_sprite_stack[sprite_num].gfx[i].anim_data[j].frame = j;
				gfx_sprite_stack[sprite_num].gfx[i].anim_data[j].offset_x = 0;
				gfx_sprite_stack[sprite_num].gfx[i].anim_data[j].offset_y = 0;
			}
			gfx_sprite_stack[sprite_num].gfx[i].anim_counter = 0;
			gfx_sprite_stack[sprite_num].gfx[i].anim_end = false;
			gfx_sprite_stack[sprite_num].gfx[i].inverted = inverted;
			gfx_sprite_stack[sprite_num].gfx[i].anim_loop = loop;
			gfx_sprite_stack[sprite_num].gfx[i].anim_speed = speed;
		}
	}
}

/** GFX :: Set default cursor animation for a sprite
 *  - Default animation is set according to the number of frames in the graphics assigned to the sprite.
 *    Each frame in the graphics will be a frame in the animation, with no offset and default
 */
void GFX_SetDefaultCursorAnimation(bool inverted, bool loop, byte speed) {
	int i, j;
	for (i = 0; i < SPRITE_SUBSPRITES_MAX; i++) {
		if (gfx_sprite_cursor->gfx[i].loaded) {

			if (gfx_sprite_cursor->gfx[i].graphics_id < 0 || gfx_sprite_cursor->gfx[i].graphics_id >= SPRITE_MAX_GRAPHICS) {
				sprintf(engine.system_error_message1, "GFX_SetDefaultCursorAnimation function error");
				sprintf(engine.system_error_message2, "Graphics id %u out of bounds", gfx_sprite_cursor->gfx[i].graphics_id);
				sprintf(engine.system_error_message3, "...");
				Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
			}

			if (!gfx_sprite_graphics_stack[gfx_sprite_cursor->gfx[i].graphics_id].loaded) {
				sprintf(engine.system_error_message1, "GFX_SetDefaultCursorAnimation function error");
				sprintf(engine.system_error_message2, "Graphics id %u not loaded", gfx_sprite_cursor->gfx[i].graphics_id);
				sprintf(engine.system_error_message3, "...");
				Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
			}

			if (gfx_sprite_graphics_stack[gfx_sprite_cursor->gfx[i].graphics_id].num_frames > SPRITE_ANIMATION_MAX_FRAMES) {
				sprintf(engine.system_error_message1, "GFX_SetDefaultCursorAnimation function error");
				sprintf(engine.system_error_message2, "Number of frames in graphics id %u exceeds maximum animation frames", gfx_sprite_cursor->gfx[i].graphics_id);
				sprintf(engine.system_error_message3, "...");
				Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
			}

			gfx_sprite_cursor->gfx[i].anim_current_frame = 0;
			gfx_sprite_cursor->gfx[i].anim_frames = gfx_sprite_graphics_stack[gfx_sprite_cursor->gfx[i].graphics_id].num_frames;

			// Set frames according to graphics frames
			for (j = 0; j < gfx_sprite_cursor->gfx[i].anim_frames; j++) {
				gfx_sprite_cursor->gfx[i].anim_data[j].frame = j;
				gfx_sprite_cursor->gfx[i].anim_data[j].offset_x = 0;
				gfx_sprite_cursor->gfx[i].anim_data[j].offset_y = 0;
			}
			gfx_sprite_cursor->gfx[i].anim_counter = 0;
			gfx_sprite_cursor->gfx[i].anim_end = false;
			gfx_sprite_cursor->gfx[i].inverted = inverted;
			gfx_sprite_cursor->gfx[i].anim_loop = loop;
			gfx_sprite_cursor->gfx[i].anim_speed = speed;
		}
	}
}

/** GFX :: Set single frame animation for a sprite
 *  - Set a single frame for all sprite graphics, with no offset and default speed, loop and inverted values
 */
void GFX_SetSingleFrameAnimation(int sprite_num, int frame) {
	int i, j;

	if (gfx_sprite_stack[sprite_num].gfx[0].graphics_id < 0 || gfx_sprite_stack[sprite_num].gfx[0].graphics_id >= SPRITE_MAX_GRAPHICS) {
		sprintf(engine.system_error_message1, "GFX_SetSingleFrameAnimation function error");
		sprintf(engine.system_error_message2, "Graphics id %u out of bounds", gfx_sprite_stack[sprite_num].gfx[0].graphics_id);
		sprintf(engine.system_error_message3, "Sprite number: %u, Sprite index: 0", sprite_num);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
	}

	if (!gfx_sprite_stack[sprite_num].gfx[0].loaded) {
		sprintf(engine.system_error_message1, "GFX_SetSingleFrameAnimation function error");
		sprintf(engine.system_error_message2, "Graphic number 0 is not loaded, graphics id %u", gfx_sprite_stack[sprite_num].gfx[0].graphics_id);
		sprintf(engine.system_error_message3, "Sprite number: %u", sprite_num);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
	}

	if (frame > gfx_sprite_graphics_stack[gfx_sprite_stack[sprite_num].gfx[0].graphics_id].num_frames) {
		sprintf(engine.system_error_message1, "GFX_SetSingleFrameAnimation function error");
		sprintf(engine.system_error_message2, "Frame number %u exceeds number of frames in graphics id %u", frame, gfx_sprite_stack[sprite_num].gfx[0].graphics_id);
		sprintf(engine.system_error_message3, "Sprite number: %u", sprite_num);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
	}

	/// Clear all existing frames
	for (i = 0; i < SPRITE_SUBSPRITES_MAX; i++) {
		gfx_sprite_stack[sprite_num].gfx[i].anim_current_frame = 0;
		gfx_sprite_stack[sprite_num].gfx[i].anim_frames = 0;
		gfx_sprite_stack[sprite_num].gfx[i].anim_counter = 0;
		gfx_sprite_stack[sprite_num].gfx[i].anim_end = false;
		gfx_sprite_stack[sprite_num].gfx[i].inverted = false;
		gfx_sprite_stack[sprite_num].gfx[i].anim_loop = false;
		gfx_sprite_stack[sprite_num].gfx[i].anim_speed = 3;

		for (j = 0; j < SPRITE_ANIMATION_MAX_FRAMES; j++) {
			gfx_sprite_stack[sprite_num].gfx[i].anim_data[j].frame = 0;
			gfx_sprite_stack[sprite_num].gfx[i].anim_data[j].offset_x = 0;
			gfx_sprite_stack[sprite_num].gfx[i].anim_data[j].offset_y = 0;
		}
	}

	gfx_sprite_stack[sprite_num].gfx[0].anim_current_frame = 0;
	gfx_sprite_stack[sprite_num].gfx[0].anim_frames = 1;
	gfx_sprite_stack[sprite_num].gfx[0].anim_counter = 0;
	gfx_sprite_stack[sprite_num].gfx[0].anim_end = false;
	gfx_sprite_stack[sprite_num].gfx[0].inverted = false;
	gfx_sprite_stack[sprite_num].gfx[0].anim_loop = false;
	gfx_sprite_stack[sprite_num].gfx[0].anim_speed = 5;

	gfx_sprite_stack[sprite_num].gfx[0].anim_data[0].frame = frame;
	gfx_sprite_stack[sprite_num].gfx[0].anim_data[0].offset_x = 0;
	gfx_sprite_stack[sprite_num].gfx[0].anim_data[0].offset_y = 0;
}

void GFX_SetSpriteAnimation(int sprite_num, int anim_num, SpriteAnimation anim_data, bool reset_current_frame) {

	gfx_sprite_stack[sprite_num].gfx[anim_num].anim_end = false;

	gfx_sprite_stack[sprite_num].gfx[anim_num].anim_frames = anim_data.frames;
	gfx_sprite_stack[sprite_num].gfx[anim_num].anim_speed = anim_data.speed;
	gfx_sprite_stack[sprite_num].gfx[anim_num].anim_loop = anim_data.loop;
	gfx_sprite_stack[sprite_num].gfx[anim_num].inverted = anim_data.inverted;

	// Update data
	gfx_sprite_stack[sprite_num].gfx[anim_num].anim_data[0] = anim_data.anim_data[0];
	gfx_sprite_stack[sprite_num].gfx[anim_num].anim_data[1] = anim_data.anim_data[1];
	gfx_sprite_stack[sprite_num].gfx[anim_num].anim_data[2] = anim_data.anim_data[2];
	gfx_sprite_stack[sprite_num].gfx[anim_num].anim_data[3] = anim_data.anim_data[3];
	gfx_sprite_stack[sprite_num].gfx[anim_num].anim_data[4] = anim_data.anim_data[4];
	gfx_sprite_stack[sprite_num].gfx[anim_num].anim_data[5] = anim_data.anim_data[5];
	gfx_sprite_stack[sprite_num].gfx[anim_num].anim_data[6] = anim_data.anim_data[6];
	gfx_sprite_stack[sprite_num].gfx[anim_num].anim_data[7] = anim_data.anim_data[7];
	gfx_sprite_stack[sprite_num].gfx[anim_num].anim_data[8] = anim_data.anim_data[8];
	gfx_sprite_stack[sprite_num].gfx[anim_num].anim_data[9] = anim_data.anim_data[9];

	if (reset_current_frame) {
		gfx_sprite_stack[sprite_num].gfx[anim_num].anim_current_frame = 0;
		gfx_sprite_stack[sprite_num].gfx[anim_num].anim_counter = 0;
	}
}

void GFX_UpdateSpriteAnimation(int sprite_num, int anim_num, SpriteAnimation anim_data) {
	gfx_sprite_stack[sprite_num].gfx[anim_num].anim_counter = anim_data.counter;
	gfx_sprite_stack[sprite_num].gfx[anim_num].anim_current_frame = anim_data.current_frame;
	gfx_sprite_stack[sprite_num].gfx[anim_num].anim_frames = anim_data.frames;
	gfx_sprite_stack[sprite_num].gfx[anim_num].anim_end = anim_data.end;
	gfx_sprite_stack[sprite_num].gfx[anim_num].anim_loop = anim_data.loop;
	gfx_sprite_stack[sprite_num].gfx[anim_num].anim_speed = anim_data.speed;
	gfx_sprite_stack[sprite_num].gfx[anim_num].anim_data[0] = anim_data.anim_data[0];
	gfx_sprite_stack[sprite_num].gfx[anim_num].anim_data[1] = anim_data.anim_data[1];
	gfx_sprite_stack[sprite_num].gfx[anim_num].anim_data[2] = anim_data.anim_data[2];
	gfx_sprite_stack[sprite_num].gfx[anim_num].anim_data[3] = anim_data.anim_data[3];
	gfx_sprite_stack[sprite_num].gfx[anim_num].anim_data[4] = anim_data.anim_data[4];
	gfx_sprite_stack[sprite_num].gfx[anim_num].anim_data[5] = anim_data.anim_data[5];
	gfx_sprite_stack[sprite_num].gfx[anim_num].anim_data[6] = anim_data.anim_data[6];
	gfx_sprite_stack[sprite_num].gfx[anim_num].anim_data[7] = anim_data.anim_data[7];
	gfx_sprite_stack[sprite_num].gfx[anim_num].anim_data[8] = anim_data.anim_data[8];
	gfx_sprite_stack[sprite_num].gfx[anim_num].anim_data[9] = anim_data.anim_data[9];
}

void GFX_SetSpriteUnmasked(int spr_num, bool value) {
	gfx_sprite_stack[spr_num].unmasked = value;
}

int GFX_GetSpriteWidth_px(int spr_num) {
	return gfx_sprite_stack[spr_num].width_px;
}

int GFX_GetSpriteHeight_px(int spr_num) {
	return gfx_sprite_stack[spr_num].height_px;
}

int GFX_GetSpriteGraphicsWidth_px(int graphics_id) {
	return gfx_sprite_graphics_stack[graphics_id].width_px;
}

int GFX_GetSpriteGraphicsHeight_px(int graphics_id) {
	return gfx_sprite_graphics_stack[graphics_id].height_px;
}

int GFX_GetSpriteScreenPosX(int spr_num) {
	return gfx_sprite_stack[spr_num].screen_pos_x;
}

bool GFX_IsSpriteAnimationEnded(int spr_num, int anim_num) {
	if (gfx_sprite_stack[spr_num].gfx[anim_num].anim_end) return true;
	if (!gfx_sprite_stack[spr_num].gfx[anim_num].loaded) return true;
	return false;
}

int GFX_GetSpriteScreenPosY(int spr_num) {
	return gfx_sprite_stack[spr_num].screen_pos_y;
}

int GFX_GetSpriteGraphicsId(int spr_num, int graphic_index) {
	return gfx_sprite_stack[spr_num].gfx[graphic_index].graphics_id;
}

void GFX_SetSpriteGraphicsId(int spr_num, int graphic_index, int graphics_id) {
	gfx_sprite_stack[spr_num].gfx[graphic_index].graphics_id = graphics_id;
}

int GFX_GetSpriteAnimationFrames(int spr_num, int anim_num) {
	return gfx_sprite_stack[spr_num].gfx[anim_num].anim_frames;
}

int GFX_GetSpriteGraphicsFrames(int gfx_id) {
	return gfx_sprite_graphics_stack[gfx_id].num_frames;
}

void GFX_SetSpritePriority(int sprite_num, byte priority) {
	gfx_sprite_stack[sprite_num].priority = priority;
}

void GFX_HideCursorSprite(void) {
	int i;
	gfx_sprite_cursor->invisible = true;
	for (i = 0; i < SPRITE_SUBSPRITES_MAX; i++)
		gfx_sprite_cursor->gfx[i].invisible = true;
}

void GFX_ShowCursorSprite(void) {
	int i;
	gfx_sprite_cursor->invisible = false;
	for (i = 0; i < SPRITE_SUBSPRITES_MAX; i++)
		gfx_sprite_cursor->gfx[i].invisible = false;
}

void GFX_ShowSprite(int sprite_num) {
	int i;
	gfx_sprite_stack[sprite_num].invisible = false;
	for (i = 0; i < SPRITE_SUBSPRITES_MAX; i++)
		gfx_sprite_stack[sprite_num].gfx[i].invisible = false;
}

void GFX_HideSprite(int sprite_num) {
	int i;
	gfx_sprite_stack[sprite_num].invisible = true;
	for (i = 0; i < SPRITE_SUBSPRITES_MAX; i++)
		gfx_sprite_stack[sprite_num].gfx[i].invisible = true;
}

void GFX_SetSpriteBlinkingProperties(int spr_num, bool blink, byte time, byte color) {
	gfx_sprite_stack[spr_num].blink.color = color;
	gfx_sprite_stack[spr_num].blink.active = blink;
	gfx_sprite_stack[spr_num].blink.time = time;
	gfx_sprite_stack[spr_num].blink.counter = 0;
	gfx_sprite_stack[spr_num].blink.blinked = false;
}

void GFX_LoadAnimations(const char *dat_file, const char *asset_file, SpriteAnimation *anim, int total_anims) {
	if (total_anims > 150) {
		sprintf(engine.system_error_message1, "GFX_LoadAnimations function error");
		sprintf(engine.system_error_message2, "Excessive animations requested in %s ", dat_file);
		sprintf(engine.system_error_message3, "Asset file %s", asset_file);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}

	FILE_LoadAnimationFile(dat_file, asset_file, anim, total_anims);
}