#include "../engine.h"
#include <string.h>
#include "ui.h"

bool ui_show_panel;
byte *ui_panel;
UI *ui;
Button *ui_button;

void UI_Init(void) {
	ui = MM_PushChunk(sizeof(UI), CT_ENGINE);

	ui->txt_file[UI_TXT_INTRO] = MM_PushChunk(sizeof(TextFile), CT_ENGINE);
	ui->txt_file[UI_TXT_GLOBAL] = MM_PushChunk(sizeof(TextFile), CT_ENGINE);
	ui->txt_file[UI_TXT_CREDITS] = MM_PushChunk(sizeof(TextFile), CT_ENGINE);
	ui->txt_file[UI_TXT_NAMES] = MM_PushChunk(sizeof(TextFile), CT_ENGINE);
	ui->txt_file[UI_TXT_SCN1D] = MM_PushChunk(sizeof(TextFile), CT_ENGINE);
	ui->txt_file[UI_TXT_SCN1H] = MM_PushChunk(sizeof(TextFile), CT_ENGINE);
	ui->txt_file[UI_TXT_SCN1I] = MM_PushChunk(sizeof(TextFile), CT_ENGINE);
	ui->txt_file[UI_TXT_SCN2D] = MM_PushChunk(sizeof(TextFile), CT_ENGINE);
	ui->txt_file[UI_TXT_SCN2H] = MM_PushChunk(sizeof(TextFile), CT_ENGINE);
	ui->txt_file[UI_TXT_SCN2I] = MM_PushChunk(sizeof(TextFile), CT_ENGINE);
	ui->txt_file[UI_TXT_SCN3D] = MM_PushChunk(sizeof(TextFile), CT_ENGINE);
	ui->txt_file[UI_TXT_SCN3H] = MM_PushChunk(sizeof(TextFile), CT_ENGINE);
	ui->txt_file[UI_TXT_SCN3I] = MM_PushChunk(sizeof(TextFile), CT_ENGINE);
	ui->txt_file[UI_TXT_SCN4D] = MM_PushChunk(sizeof(TextFile), CT_ENGINE);
	ui->txt_file[UI_TXT_SCN4H] = MM_PushChunk(sizeof(TextFile), CT_ENGINE);
	ui->txt_file[UI_TXT_SCN4I] = MM_PushChunk(sizeof(TextFile), CT_ENGINE);
	ui->txt_file[UI_TXT_SCN5D] = MM_PushChunk(sizeof(TextFile), CT_ENGINE);
	ui->txt_file[UI_TXT_SCN5H] = MM_PushChunk(sizeof(TextFile), CT_ENGINE);
	ui->txt_file[UI_TXT_SCN5I] = MM_PushChunk(sizeof(TextFile), CT_ENGINE);
	ui->txt_file[UI_TXT_SCN6D] = MM_PushChunk(sizeof(TextFile), CT_ENGINE);
	ui->txt_file[UI_TXT_SCN6H] = MM_PushChunk(sizeof(TextFile), CT_ENGINE);
	ui->txt_file[UI_TXT_SCN6I] = MM_PushChunk(sizeof(TextFile), CT_ENGINE);
	ui->txt_file[UI_TXT_END] = MM_PushChunk(sizeof(TextFile), CT_ENGINE);

	// Buttons
	ui_button = MM_PushChunk(sizeof(Button) * UI_MAX_BUTTONS, CT_ENGINE);
}

void UI_SetStatusPanels(void) {
	// Set status panel graphics
	GFX_SetPanelGraphics(gfx_actor_status_panel, SPRITE_GRAPHICS_ID_ACTOR_PORTAIT, SPRITE_GRAPHICS_ID_LIFEBAR, actor->gun->graphics_id, actor->key_graphics_id);
	GFX_SetPanelPosition(gfx_actor_status_panel, 0, 0, 0, 0, 32, 0, 32, 8, 64, 8, 84, 8);
	GFX_UpdatePanel(gfx_actor_status_panel, actor->life, actor->life, actor->max_life, 1);
	GFX_UpdatePanelGun(gfx_actor_status_panel, actor->gun->graphics_id, actor->gun->type, actor->gun->total_bullets, actor->gun->current_bullets, SPRITE_GRAPHICS_ID_GRENADE, actor->current_grenades, actor->key_entity_id, actor->key_graphics_id);
	GFX_ShowPanel(gfx_actor_status_panel, actor->mode_combat, 0);

	GFX_SetPanelGraphics(gfx_enemy_status_panel, SPRITE_GRAPHICS_ID_OBJECT1_PORTAIT, SPRITE_GRAPHICS_ID_LIFEBAR, SPRITE_GRAPHICS_ID_GUN0, SPRITE_GRAPHICS_ID_EMPTY);
	GFX_SetPanelPosition(gfx_enemy_status_panel, 236, 0, 48, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	GFX_ShowPanel(gfx_enemy_status_panel, false, 200);
}

void UI_SetDefaultLanguage(void) {
	ui->language = 0;// set ES as default
}

void UI_ButtonOver(int button_number) {
	int sprite_num;
	if (ui_button[button_number].is_loaded) {

		sprite_num = ui_button[button_number].num_sprite;
		switch (ui_button[button_number].type) {
			case ENTITY_ID_BTN:
				// Update current screen position
				GFX_SetSpriteFrame(sprite_num, 0, UI_BUTTON_OVER_FRAME_NUM);
				break;
		}
	}
}

void UI_ButtonClick(int button_number) {
	int sprite_num;
	if (ui_button[button_number].is_loaded) {
		sprite_num = ui_button[button_number].num_sprite;

		switch (ui_button[button_number].type) {
			case ENTITY_ID_BTN:
				GFX_SetSpriteFrame(sprite_num, 0, UI_BUTTON_CLICK_FRAME_NUM);
				ui->button_pressed = button_number;
				break;
		}
	}
}

/** UI :: Update buttons
 */
void UI_UpdateButtons(void) {
	int i;
	int sprite_num;

	// Reset pressed button
	ui->button_pressed = 0;

	// Calculate screen pos and evaluate visibility
	for (i = 0; i < UI_MAX_BUTTONS; i++) {
		if (ui_button[i].is_loaded) {

			sprite_num = ui_button[i].num_sprite;
			switch (ui_button[i].type) {
				case ENTITY_ID_BTN:
					// Clear status
					gfx_sprite_stack[sprite_num].blink.active = false;
					GFX_SetSpriteFrame(sprite_num, 0, UI_BUTTON_IDLE_FRAME_NUM);
					break;
			}
		}
	}

	// Check mouse pointing
	switch (cursor.point_on & 0xFF) {
		case ENTITY_ID_BTN:
			UI_ButtonOver((cursor.point_on >> 8) & 0xFF);
			if (cursor.left_click) UI_ButtonClick((cursor.point_on >> 8) & 0xFF);
			break;
		default:
			break;
	}
}

void UI_HideLoadingScreen(void) {
	VIDEO_FadeOut(1);
	engine.loading = false;
	VIDEO_ClearScreen();
	VIDEO_ClearPalette();
}

void UI_ShowLoadingScreen(void) {
	VIDEO_ClearScreenBuffer();
	engine.loading = true;
	GFX_LoadPalette("PALETTES.DAT", "LOADING.PCX", 256);
	VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 128, 48, 96, 72, 1);
	VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 122, 42, 99, 75, 2);
	VIDEO_StringToScreenBuffer(115, 90, ui->txt_file[UI_TXT_GLOBAL]->line[6], FONT_SLIM_BLACK);
	VIDEO_VSync();
	VIDEO_ScreenBufferToVRAM();
	VIDEO_FadeIn(1);
}

void UI_UpdateUI(bool combat_mode) {
	int x, y;
	bool speech_finished;

	UI_UpdateButtons();

	// Pause
	if (kbKeyState[SCANCODE_P] && engine.ingame && !ui->exit_request && !ui->show_speech) {
		if (ui->pause) {
			ui->pause = false;
			ui->freeze = false;
			ACTOR_SetFreeze(false);
			VIDEO_SetPalette(gfx->palette_loaded);
		} else {
			ui->pause = true;
			ui->freeze = true;
			ACTOR_SetFreeze(true);
			MOUSE_SetCursorGraphics(SPRITE_GRAPHICS_ID_CURSOR);
			VIDEO_FadeOutPause(4);
		}
		kbKeyState[SCANCODE_P] = false;
	}

	// Exit ingame request
	if (kbKeyState[SCANCODE_ESC] && engine.ingame && !ui->pause && !ui->show_speech) {
		if (!ui->exit_request) {
			ui->exit_request = true;
			ui->freeze = true;
			ACTOR_SetFreeze(true);
			MOUSE_SetCursorGraphics(SPRITE_GRAPHICS_ID_CURSOR);
			VIDEO_FadeOutPause(4);
		}
		kbKeyState[SCANCODE_ESC] = false;
	}

	// Exit ingame
	if (engine.ingame & ui->exit_request & kbKeyState[SCANCODE_Y]) {

		// Exit from current level
		engine.ingame = false;

		// Set menu
		engine.exit_menu = true;
		engine.scene = 0;
		engine.room = 1;

		ui->freeze = false;
		ACTOR_SetFreeze(false);
		kbKeyState[SCANCODE_Y] = false;
		ui->exit_request = false;
		VIDEO_SetPalette(gfx->palette_loaded);
	}

	// NOT Exit ingame
	if (engine.ingame & ui->exit_request & kbKeyState[SCANCODE_N]) {
		engine.ingame = true;
		ui->freeze = false;
		ACTOR_SetFreeze(false);
		kbKeyState[SCANCODE_N] = false;
		ui->exit_request = false;
		VIDEO_SetPalette(gfx->palette_loaded);
	}

	// Show exit request
	if (ui->exit_request) {
		VIDEO_StringToScreenBuffer(110, 90, ui->txt_file[UI_TXT_GLOBAL]->line[8], FONT_BIG_WHITE);
	}

	// Show pause
	if (ui->pause) {
		VIDEO_StringToScreenBuffer(110, 90, ui->txt_file[UI_TXT_GLOBAL]->line[7], FONT_BIG_WHITE);
	}

	// Show description
	if (ui->show_description) {
		ui->show_description = false;
		x = cursor.pos_x - ((ui->name_length << 3) >> 1);
		y = cursor.pos_y - 16;
		if (x < 0) x = 0;
		if (x + (ui->name_length << 3) > camera->visible_width) x = camera->visible_width - (ui->name_length << 3);
		if (y < 0) y = 0;
		VIDEO_StringToScreenBuffer(x, y, ui->object_name, FONT_SLIM_WHITE);
	}

	if (ui->show_speech) {
		ui->actor_was_on_combat_mode = actor->mode_combat;
		ACTOR_SetCombatMode(false);
		speech_finished = VIDEO_ChatToScreenBuffer(gfx_chat_panel, cursor.right_click_FN);
		ui->speech_time++;

		// Cancel speech by right click
		if (speech_finished && cursor.right_click_FN) {
			MOUSE_MaskRightClick();
			ui->speech_time = ui->speech_timeout;
		}

		if (ui->speech_time >= ui->speech_timeout) {
			ui->show_speech = false;
			ui->freeze = false;
			ACTOR_SetCombatMode(ui->actor_was_on_combat_mode);
			ACTOR_SetFreeze(false);
		}
	}

	if (combat_mode) {
		GFX_UpdatePanelGun(gfx_actor_status_panel, actor->gun->graphics_id, actor->gun->type, actor->gun->total_bullets, actor->gun->current_bullets, SPRITE_GRAPHICS_ID_GRENADE, actor->current_grenades, actor->key_entity_id, actor->key_graphics_id);
		VIDEO_PanelToScreenBuffer(gfx_actor_status_panel);
		VIDEO_PanelToScreenBuffer(gfx_enemy_status_panel);
	}
}

bool UI_IsSpeechFinished(void) {
	return !ui->show_speech;
}

void UI_ShowSpeech(ChatPanel *pannel, int portait_graphics_id, int box_graphics_id, int text_file, int line1, int line2, int line3, bool inverted, int timeout) {

	if (!gfx_sprite_graphics_stack[portait_graphics_id].loaded) {
		sprintf(engine.system_error_message1, "UI_ShowSpeech function error");
		sprintf(engine.system_error_message2, "Portait graphics id %u not loaded", portait_graphics_id);
		sprintf(engine.system_error_message3, "...");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
	}

	if (!gfx_sprite_graphics_stack[box_graphics_id].loaded) {
		sprintf(engine.system_error_message1, "UI_ShowSpeech function error");
		sprintf(engine.system_error_message2, "Box graphics id %u not loaded", box_graphics_id);
		sprintf(engine.system_error_message3, "...");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
	}

	if (inverted) {
		GFX_SetChatGraphics(pannel, portait_graphics_id, box_graphics_id);
		GFX_SetChatPosition(pannel, 10, 0, 200, 10, inverted, 0, 10, inverted);
		GFX_SetLineChatPanel(gfx_chat_panel, ui->txt_file[text_file]->line[line1], 0);
		GFX_SetLineChatPanel(gfx_chat_panel, ui->txt_file[text_file]->line[line2], 1);
		GFX_SetLineChatPanel(gfx_chat_panel, ui->txt_file[text_file]->line[line3], 2);
	} else {
		GFX_SetChatGraphics(pannel, portait_graphics_id, box_graphics_id);
		GFX_SetChatPosition(pannel, 10, 0, 10, 10, inverted, 80, 10, inverted);
		GFX_SetLineChatPanel(gfx_chat_panel, ui->txt_file[text_file]->line[line1], 0);
		GFX_SetLineChatPanel(gfx_chat_panel, ui->txt_file[text_file]->line[line2], 1);
		GFX_SetLineChatPanel(gfx_chat_panel, ui->txt_file[text_file]->line[line3], 2);
	}
	ui->freeze = true;
	ui->show_speech = true;
	ui->speech_time = 0;
	ui->speech_timeout = timeout;
	ACTOR_SetFreeze(true);
}

void UI_ShowDescription(int text_file, int line) {
	sprintf(ui->object_name, ui->txt_file[text_file]->line[line]);
	ui->name_length = strlen(ui->object_name);
	ui->show_description = true;
}

int UI_GetButtonXPosition(byte number) {
	return ui_button[number].pos_x;
}

int UI_GetButtonYPosition(byte number) {
	return ui_button[number].pos_y;
}

void UI_SetButtonPosition(byte number, int x, int y) {
	if (number >= UI_MAX_BUTTONS) {
		sprintf(engine.system_error_message1, "UI_SetButtonPosition function error");
		sprintf(engine.system_error_message2, "Button index %u, out of range", number);
		sprintf(engine.system_error_message3, " ");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
	}

	if (!ui_button[number].is_loaded) {
		sprintf(engine.system_error_message1, "UI_SetButtonPosition function error");
		sprintf(engine.system_error_message2, "Button index %u, not loaded", number);
		sprintf(engine.system_error_message3, " ");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
	}

	ui_button[number].pos_x = x;
	ui_button[number].pos_y = y;
	gfx_sprite_stack[ui_button[number].num_sprite].screen_pos_x = x;
	gfx_sprite_stack[ui_button[number].num_sprite].screen_pos_y = y;
}

/** UI :: Load button
 */
void UI_LoadButton(byte number, byte entity_id, byte graphics_id, int pos_x, int pos_y) {

	int sprite_slot;

	if (number >= UI_MAX_BUTTONS) {
		sprintf(engine.system_error_message1, "UI_LoadButton function error");
		sprintf(engine.system_error_message2, "Max number of buttons exceded");
		sprintf(engine.system_error_message3, " ");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
	}

	ui_button[number].type = entity_id;
	ui_button[number].is_loaded = true;
	ui_button[number].pos_x = pos_x;
	ui_button[number].pos_y = pos_y;
	ui_button[number].graphic_index = 0;

	// Check if graphics id is already loaded
	if (!gfx_sprite_graphics_stack[graphics_id].loaded) {
		sprintf(engine.system_error_message1, "UI_LoadButton function error");
		sprintf(engine.system_error_message2, "Graphics id %u not loaded", graphics_id);
		sprintf(engine.system_error_message3, " ");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
	}

	sprite_slot = GFX_FindEmptySpriteSlot();
	if (sprite_slot == -1) {
		sprintf(engine.system_error_message1, "UI_LoadButton function error");
		sprintf(engine.system_error_message2, "No empty sprite slot available");
		sprintf(engine.system_error_message3, " ");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
	} else {
		ui_button[number].num_sprite = sprite_slot;
		GFX_InitSprite(entity_id, number, sprite_slot, 15, gfx_sprite_graphics_stack[graphics_id].width_px, gfx_sprite_graphics_stack[graphics_id].height_px);
	}
	// Set cursor graphics
	GFX_SetSpriteGraphic(ui_button[number].num_sprite, 0, graphics_id, 0, 0);
	GFX_SetDefaultAnimation(ui_button[number].num_sprite, false, false, 5);

	// Set hit box
	//  ----------------
	// |  1          2  |
	// |                |
	// |                |
	// |                |
	// |                |
	// |  3          4  |
	//  ----------------
	// Point 1
	ui_button[number]
			.hit_area.points[0][0] = 0;
	ui_button[number].hit_area.points[0][1] = 0;
	// Point 2
	ui_button[number].hit_area.points[1][0] = gfx_sprite_stack[ui_button[number].num_sprite].width_px;
	ui_button[number].hit_area.points[1][1] = 0;
	// Point 3
	ui_button[number].hit_area.points[2][0] = 0;
	ui_button[number].hit_area.points[2][1] = gfx_sprite_stack[ui_button[number].num_sprite].height_px;
	// Point 4
	ui_button[number].hit_area.points[3][0] = gfx_sprite_stack[ui_button[number].num_sprite].width_px;
	ui_button[number].hit_area.points[3][1] = gfx_sprite_stack[ui_button[number].num_sprite].height_px;


	// Set initial screen position
	gfx_sprite_stack[ui_button[number].num_sprite].screen_pos_x = pos_x - camera->pos_x;
	gfx_sprite_stack[ui_button[number].num_sprite].screen_pos_y = pos_y - camera->pos_y;
}


/** UI :: Uload buttons
 */
void UI_UnloadButtons(void) {
	int i;
	for (i = 0; i < UI_MAX_BUTTONS; i++) {
		if (ui_button[i].is_loaded) {
			GFX_UnloadSprite(ui_button[i].num_sprite);
			ui_button[i].is_loaded = false;
			ui_button[i].num_sprite = -1;
		}
	}
}