#include "../engine.h"
#include "mouse.h"
#include <string.h>

_go32_dpmi_registers mouse_regs;
int old_mouse_handler_mask;
int old_mouse_handler_seg;
int old_mouse_handler_off;

MouseData cursor;
//SpriteAnimation click_anim;

/** MOUSE :: Interrupt handler
 */
void MOUSE_handler(void) {
	disable();
	cursor.pos_x = mouse_regs.x.cx >> 2;
	cursor.pos_y = mouse_regs.x.dx >> 2;

	if (cursor.pos_x > 310) cursor.pos_x = 310;
	if (cursor.pos_x < 0) cursor.pos_x = 0;
	if (cursor.pos_y > 184) cursor.pos_y = 184;
	if (cursor.pos_y < 0) cursor.pos_y = 0;

	cursor.left_click = mouse_regs.x.bx & 0x01;
	cursor.right_click = mouse_regs.x.bx & 0x02;
	enable();
}
END_OF_FUNCTION(MOUSE_handler);

/** MOUSE :: Restrict movement zone 
 */
void MOUSE_RestrictZone(int x1, int x2, int y1, int y2) {
	__dpmi_regs regs;
	regs.x.ax = 7;
	regs.x.cx = x1;
	regs.x.dx = x2;
	__dpmi_int(0x33, &regs);

	regs.x.ax = 8;
	regs.x.cx = y1;
	regs.x.dx = y2;
	__dpmi_int(0x33, &regs);
}

/** MOUSE :: Initialize interrupt and memory reservation 
 */
void MOUSE_Init(void) {
	__dpmi_regs r;
	_go32_dpmi_seginfo mouse_seginfo;

	if (!engine.mouse_initialized) {
		LOCK_VARIABLE(cursor.pos_x);
		LOCK_VARIABLE(cursor.pos_y);
		LOCK_VARIABLE(cursor.left_click);
		LOCK_VARIABLE(cursor.right_click);
		LOCK_FUNCTION(MOUSE_handler);

		mouse_seginfo.pm_offset = (int) MOUSE_handler;

		_go32_dpmi_allocate_real_mode_callback_retf(&mouse_seginfo,
													&mouse_regs);
		r.x.ax = 0x14;
		r.x.cx = 0x1F;
		r.x.dx = mouse_seginfo.rm_offset;
		r.x.es = mouse_seginfo.rm_segment;

		__dpmi_int(0x33, &r);

		old_mouse_handler_mask = r.x.cx;
		old_mouse_handler_off = r.x.dx;
		old_mouse_handler_seg = r.x.es;
	}

	// Set cursor initial values
	cursor.pos_x = 160;
	cursor.pos_y = 100;
	cursor.left_click = false;
	cursor.right_click = false;
	cursor.right_clicK_delay_counter = 0;
	cursor.right_clicK_delay_time = MOUSE_RIGHT_CLICK_DELAY;
	cursor.right_click_mask = false;
	cursor.sprite_num = -1;

	engine.mouse_initialized = true;
}

/** MOUSE :: Shutdown interrupt and free graphics 
 */
void MOUSE_Shutdown(void) {
	__dpmi_regs r;
	if (engine.mouse_initialized) {
		r.x.ax = 0xC;
		r.x.cx = (short) old_mouse_handler_mask;
		r.x.dx = (short) old_mouse_handler_off;
		r.x.es = (short) old_mouse_handler_seg;
		__dpmi_int(0x33, &r);
	}

	engine.mouse_initialized = false;
}

/** MOUSE :: Enable cursor 
 */
void MOUSE_EnableCursor(void) {
	__dpmi_regs regs;
	regs.x.ax = 1;
	__dpmi_int(0x33, &regs);
}

/** MOUSE :: Check if availablity
 *  - Returns true if mouse is available
 */
bool MOUSE_CheckIfAvailable(void) {
	__dpmi_regs regs;
	regs.x.ax = 0;
	__dpmi_int(0x33, &regs);

	if (regs.x.ax == 0xFFFF) {
		return true;
	} else {
		return false;
	}
}

void MOUSE_InitCursorSprite(byte graphics_id) {
	switch (graphics_id) {
		case SPRITE_GRAPHICS_ID_CURSOR:
			GFX_InitCursorSprite(ENTITY_ID_CURSOR, 0, 7, 12);
			cursor.width_px = 7;
			cursor.height_px = 12;
			break;
		case SPRITE_GRAPHICS_ID_AIM:
			GFX_InitCursorSprite(ENTITY_ID_CURSOR, 0, 15, 16);
			cursor.width_px = 15;
			cursor.height_px = 16;
			break;
		default:
			sprintf(engine.system_error_message1, "MOUSE_InitCursorSprite function error");
			sprintf(engine.system_error_message2, "Graphic id %u not loaded", graphics_id);
			sprintf(engine.system_error_message3, "");
			Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
			break;
	}

	GFX_SetSpriteCursorGraphic(graphics_id, 0, 0);
	GFX_SetDefaultCursorAnimation(false, false, 5);
}

void MOUSE_SetCursorGraphics(int graphics_id) {
	if (graphics_id != cursor.current_cursor_id) {
		GFX_SetSpriteCursorGraphic(graphics_id, 0, 0);
		cursor.current_cursor_id = graphics_id;
	}
}

/** MOUSE :: Set cursor graphic frame
 */
void MOUSE_SetCursorFrame(int frame_number) {

	GFX_SetCursorSpriteFrame(0, frame_number);
}

void MOUSE_HideCursor(void) {
	GFX_HideCursorSprite();
}

void MOUSE_ShowCursor(void) {
	GFX_ShowCursorSprite();
}

/** MOUSE :: Check cursor colission
 */
int MOUSE_CheckCursorColission(void) {
	int i, hspot;
	int point1_x, point1_y;

	point1_x = cursor.pos_x;
	point1_y = cursor.pos_y;

	/////// BUTTONS COLISSIONS ///////////
	for (i = 0; i < UI_MAX_BUTTONS; i++) {
		if (ui_button[i].is_loaded) {
			if (gfx_sprite_stack[ui_button[i].num_sprite].shown) {

				//Point 1
				if (point1_x < ui_button[i].pos_x + ui_button[i].hit_area.points[1][0]

					&& point1_x > ui_button[i].pos_x + ui_button[i].hit_area.points[0][0]

					&& point1_y > ui_button[i].pos_y + ui_button[i].hit_area.points[0][1]

					&& point1_y < ui_button[i].pos_y + ui_button[i].hit_area.points[2][1]) {
					return gfx_sprite_stack[ui_button[i].num_sprite].id;
				}
			}
		}
	}

	/////// OBJECTS COLISSIONS ///////////
	for (i = 0; i < OBJECT_MAX_OBJECTS; i++) {
		if (object[i].is_loaded) {
			if (gfx_sprite_stack[object[i].num_sprite].shown) {
				//Point 1
				if (point1_x < gfx_sprite_stack[object[i].num_sprite].screen_pos_x + object[i].hit_area.points[1][0]

					&& point1_x > gfx_sprite_stack[object[i].num_sprite].screen_pos_x + object[i].hit_area.points[0][0]

					&& point1_y > gfx_sprite_stack[object[i].num_sprite].screen_pos_y + object[i].hit_area.points[0][1]

					&& point1_y < gfx_sprite_stack[object[i].num_sprite].screen_pos_y + object[i].hit_area.points[2][1]) {
					return ((i << 8) | object[i].type);
				}
			}
		}
	}

	/////// ENEMIES COLISSIONS ///////////
	for (i = 0; i < ENEMY_MAX_ENEMIES; i++) {
		if (enemy[i].is_loaded) {
			if (gfx_sprite_stack[enemy[i].sprite_num].shown) {
				//Point 1
				if (point1_x < gfx_sprite_stack[enemy[i].sprite_num].screen_pos_x + enemy[i].hit_area.points[1][0]

					&& point1_x > gfx_sprite_stack[enemy[i].sprite_num].screen_pos_x + enemy[i].hit_area.points[0][0]

					&& point1_y > gfx_sprite_stack[enemy[i].sprite_num].screen_pos_y + enemy[i].hit_area.points[0][1]

					&& point1_y < gfx_sprite_stack[enemy[i].sprite_num].screen_pos_y + enemy[i].hit_area.points[1][1]) {
					return gfx_sprite_stack[enemy[i].sprite_num].id;
				}
			}
		}
	}

	/////// NPCS COLISSIONS ///////////
	for (i = 0; i < NPC_MAX_NPCS; i++) {
		if (npc[i].is_loaded) {
			if (gfx_sprite_stack[npc[i].sprite_num].shown) {
				//Point 1
				if (point1_x < gfx_sprite_stack[npc[i].sprite_num].screen_pos_x + npc[i].hit_area.points[1][0]

					&& point1_x > gfx_sprite_stack[npc[i].sprite_num].screen_pos_x + npc[i].hit_area.points[0][0]

					&& point1_y > gfx_sprite_stack[npc[i].sprite_num].screen_pos_y + npc[i].hit_area.points[0][1]

					&& point1_y < gfx_sprite_stack[npc[i].sprite_num].screen_pos_y + npc[i].hit_area.points[2][1]) {
					return gfx_sprite_stack[npc[i].sprite_num].id;
				}
			}
		}
	}

	/////// HOTSPOT COLISSIONS ///////////
	hspot = 0;
	if (map->loaded) {
		hspot = MAP_CheckHotspotTile(camera->pos_x + cursor.pos_x, camera->pos_y + cursor.pos_y);
		if (hspot != 255) return ((hspot) << 8) | ENTITY_ID_HSPOT;
		else
			return 0;
	}

	return 0;
}

void MOUSE_MaskRightClick(void) {
	cursor.right_click_mask = true;
}

/** MOUSE :: Update
 */
void MOUSE_Update(bool combat_mode, bool freeze) {
	if (engine.mouse_initialized) {
		// Check FP
		cursor.left_click_FP = false;
		cursor.right_click_FP = false;
		if (cursor.left_click && !cursor.left_click_status_old) cursor.left_click_FP = true;
		if (cursor.right_click && !cursor.right_click_status_old) cursor.right_click_FP = true;

		// Check FN
		cursor.left_click_FN = false;
		cursor.right_click_FN = false;
		if (!cursor.left_click && cursor.left_click_status_old) cursor.left_click_FN = true;
		if (!cursor.right_click && cursor.right_click_status_old) cursor.right_click_FN = true;

		GFX_SetCursorSpritePosition(cursor.pos_x, cursor.pos_y);

		// Calculate middle position
		cursor.screen_middle_x_px = cursor.pos_x + (cursor.width_px >> 1);
		cursor.screen_middle_y_px = cursor.pos_y + (cursor.height_px >> 1);

		if (!combat_mode) {
			MOUSE_SetCursorGraphics(SPRITE_GRAPHICS_ID_CURSOR);
			cursor.point_on = MOUSE_CheckCursorColission();
			// Left click
			if (cursor.left_click_FP && !freeze) EFFECT_LoadEffect(ENTITY_ID_EMPTY, SPRITE_GRAPHICS_ID_CLICK, cursor.pos_x - 2 + camera->pos_x, cursor.pos_y - 2 + camera->pos_y, true, 0, false, false, 2);

		} else {
			MOUSE_SetCursorGraphics(SPRITE_GRAPHICS_ID_AIM);
		}

		// Update right click delay
		if (cursor.right_click_mask) {
			cursor.right_clicK_delay_counter++;
			if (cursor.right_clicK_delay_counter > cursor.right_clicK_delay_time) cursor.right_click_mask = false;
		}

		// save old status
		cursor.left_click_status_old = cursor.left_click;
		cursor.right_click_status_old = cursor.right_click;

		GFX_UpdateCursorSprite();
	}
}
