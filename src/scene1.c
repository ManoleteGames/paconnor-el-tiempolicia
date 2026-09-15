#include "engine/engine.h"

/** SCENARIO 1 :: Mission 1 :: The travel
 */
void Scene1_GameOver(void) {

	AUDIO_StopSong();
	VIDEO_FadeOutPause(4);
	ENEMY_UnloadEnemies();
	OBJECT_UnloadObjects();
	EFFECT_UnloadEffects();
	BULLET_UnloadBullets();
	PARTICLE_UnloadParticles();
	GRENADE_UnloadGrenades();
	NPC_UnloadNpcs();
	Update(false);

	VIDEO_StringToScreenBuffer(100, 90, ui->txt_file[UI_TXT_GLOBAL]->line[25], FONT_BIG_BLACK);

	Update(false);

	SetDelayTime(6000);
	while (!AwaitDelayTime()) {
		// Just wait
	}

	engine.ingame = false;

	engine.room = 1;
	engine.scene = 0;
}
void Scene1_Intro(void) {
	int step;
	int dialog_step, dialog_delay_counter;
	int char_number;
	char aux_string[40];
	int i, horizontal_scroll, max_scroll, scanline_scroll, src_index, dst_index, src_img_index;
	bool scroll_end;
	int chat_spr_num;

	engine.sequence = true;
	step = 0;
	while (engine.sequence) {
		switch (step) {
			case 0:// Draw loading screen
				MOUSE_HideCursor();
				UI_ShowLoadingScreen();
				step++;
				break;
			case 1:// Load first intro scene
				AUDIO_LoadSong(AUDIO_SONG_3);
				GFX_PCXImageToBuffer("ISCN1.DAT", "INTRO0.PCX", 564 * 177, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				step++;
				break;
			case 2:// Hide loading screen
				UI_HideLoadingScreen();
				step++;
				break;
			case 3:/// Scene 1 :: City with horizontal scroll
				VIDEO_ClearScreenBuffer();
				GFX_LoadPalette("PALETTES.DAT", "ISCN1.PCX", 256);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 292, 183, 13, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, 286, gfx->image_buffer1_height, 16, 14);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				VIDEO_FadeIn(4);

				TIMER_UpdateTimerTime(TIMER_AUDIO_NUMBER, 50);
				AUDIO_PlaySong(true);
				dialog_step = 0;

				src_index = 0;
				horizontal_scroll = 0;
				max_scroll = gfx->image_buffer1_width - 280;
				scroll_end = false;
				dialog_step = 0;
				while (!scroll_end & engine.sequence) {
					src_index = horizontal_scroll;
					dst_index = CAM_VISIBLE_WIDTH * 14 + 16;

					// Draw image with scroll
					for (i = 0; i < gfx->image_buffer1_height; i++) {
						memcpy(&video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index], &gfx->image_buffer1[src_index], CAM_VISIBLE_WIDTH - 35);
						src_index += gfx->image_buffer1_width;
						dst_index += CAM_VISIBLE_WIDTH;
					}
					if (horizontal_scroll < max_scroll) horizontal_scroll++;
					else
						scroll_end = true;

					// Draw text background
					VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 128, 46, 25, 21, 202);
					VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 120, 40, 29, 24, 208);

					// Draw text
					switch (dialog_step) {
						case 0:
							char_number = 0;
							dialog_delay_counter = 0;
							dialog_step++;
							break;
						case 1:
							aux_string[char_number] = ui->txt_file[UI_TXT_SCN1I]->line[1][char_number];
							aux_string[char_number + 1] = '\0';
							VIDEO_StringToScreenBuffer(38, 28, aux_string, FONT_SLIM_BLACK);
							dialog_delay_counter++;
							if (dialog_delay_counter > 4) {
								char_number++;
								dialog_delay_counter = 0;
							}
							if (char_number >= strlen(ui->txt_file[UI_TXT_SCN1I]->line[1])) {
								dialog_step++;
							}
							break;
						case 2:
							VIDEO_StringToScreenBuffer(38, 28, ui->txt_file[UI_TXT_SCN1I]->line[1], FONT_SLIM_BLACK);
							char_number = 0;
							dialog_step++;
							break;
						case 3:
							VIDEO_StringToScreenBuffer(38, 28, ui->txt_file[UI_TXT_SCN1I]->line[1], FONT_SLIM_BLACK);
							aux_string[char_number] = ui->txt_file[UI_TXT_SCN1I]->line[2][char_number];
							aux_string[char_number + 1] = '\0';
							VIDEO_StringToScreenBuffer(38, 40, aux_string, FONT_SLIM_BLACK);
							dialog_delay_counter++;
							if (dialog_delay_counter > 4) {
								char_number++;
								dialog_delay_counter = 0;
							}
							if (char_number >= strlen(ui->txt_file[UI_TXT_SCN1I]->line[2])) {
								dialog_step++;
							}
							break;
						case 4:
							VIDEO_StringToScreenBuffer(38, 28, ui->txt_file[UI_TXT_SCN1I]->line[1], FONT_SLIM_BLACK);
							VIDEO_StringToScreenBuffer(38, 40, ui->txt_file[UI_TXT_SCN1I]->line[2], FONT_SLIM_BLACK);
							char_number = 0;
							dialog_step++;
							break;
						case 5:
							VIDEO_StringToScreenBuffer(38, 28, ui->txt_file[UI_TXT_SCN1I]->line[1], FONT_SLIM_BLACK);
							VIDEO_StringToScreenBuffer(38, 40, ui->txt_file[UI_TXT_SCN1I]->line[2], FONT_SLIM_BLACK);

							aux_string[char_number] = ui->txt_file[UI_TXT_SCN1I]->line[3][char_number];
							aux_string[char_number + 1] = '\0';
							VIDEO_StringToScreenBuffer(38, 52, aux_string, FONT_SLIM_BLACK);
							dialog_delay_counter++;
							if (dialog_delay_counter > 4) {
								char_number++;
								dialog_delay_counter = 0;
							}
							if (char_number >= strlen(ui->txt_file[UI_TXT_SCN1I]->line[3])) {
								dialog_step++;
							}
							break;
						case 6:
							VIDEO_StringToScreenBuffer(38, 28, ui->txt_file[UI_TXT_SCN1I]->line[1], FONT_SLIM_BLACK);
							VIDEO_StringToScreenBuffer(38, 40, ui->txt_file[UI_TXT_SCN1I]->line[2], FONT_SLIM_BLACK);
							VIDEO_StringToScreenBuffer(38, 52, ui->txt_file[UI_TXT_SCN1I]->line[3], FONT_SLIM_BLACK);
							break;
						default:
							break;
					}

					SetDelayTime(20);
					while (!AwaitDelayTime()) {
						// Just wait
					}

					VIDEO_VSync();
					VIDEO_ScreenBufferToVRAM();
				}
				step = 5;
				break;
			case 5:// Coach horizontal scroll and text
				GFX_PCXImageToBuffer("ISCN1.DAT", "INTRO1H.PCX", 251 * 135, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("ISCN1.DAT", "INTRO1L.PCX", 205 * 135, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				VIDEO_FadeOut(4);
				horizontal_scroll = 36;
				scroll_end = false;

				VIDEO_ClearScreenBuffer();
				VIDEO_ScreenBufferToVRAM();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 211, 140, 11, 3, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, 205, gfx->image_buffer1_height, 14, 6);
				VIDEO_FadeIn(4);
				while (!scroll_end & engine.sequence) {
					src_index = horizontal_scroll;
					dst_index = CAM_VISIBLE_WIDTH * 6 + 14;

					// Draw image with scroll
					for (i = 0; i < gfx->image_buffer1_height; i++) {
						memcpy(&video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index], &gfx->image_buffer1[src_index], 205);
						src_index += gfx->image_buffer1_width;
						dst_index += CAM_VISIBLE_WIDTH;
					}
					if (horizontal_scroll > 0) horizontal_scroll--;
					else
						scroll_end = true;


					SetDelayTime(30);
					while (!AwaitDelayTime()) {
						// Just wait
					}

					VIDEO_VSync();
					VIDEO_ScreenBufferToVRAM();
				}
				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step = 6;
				break;
			case 6:// Clock (high)
				GFX_PCXImageToBuffer("ISCN1.DAT", "INTRO2H.PCX", 105 * 56, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 111, 62, 67, 117, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 70, 120);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(200);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step = 7;
				break;
			case 7:// Fade coach
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 14, 6);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 111, 62, 67, 117, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 70, 120);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();

				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step = 8;
				break;
			case 8:// Eye
				GFX_PCXImageToBuffer("ISCN1.DAT", "INTRO2L.PCX", 105 * 56, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				GFX_PCXImageToBuffer("ISCN1.DAT", "INTRO3H.PCX", 111 * 55, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 94, 61, 190, 100, 222);
				VIDEO_BufferToScreenBuffer(&gfx->image_buffer1[23], gfx->image_buffer1_width, gfx->image_buffer1_height, 88, gfx->image_buffer1_height, 193, 103);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(200);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step = 9;
				break;
			case 9:// Fade clock
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 70, 120);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 94, 61, 190, 100, 222);
				VIDEO_BufferToScreenBuffer(&gfx->image_buffer1[23], gfx->image_buffer1_width, gfx->image_buffer1_height, 88, gfx->image_buffer1_height, 193, 103);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				step = 10;
				break;
			case 10:// Scroll eye
				horizontal_scroll = 23;
				scroll_end = false;
				while (!scroll_end & engine.sequence) {
					src_index = horizontal_scroll;
					dst_index = CAM_VISIBLE_WIDTH * 103 + 193;

					// Draw image with scroll
					for (i = 0; i < gfx->image_buffer1_height; i++) {
						memcpy(&video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index], &gfx->image_buffer1[src_index], 88);
						src_index += gfx->image_buffer1_width;
						dst_index += CAM_VISIBLE_WIDTH;
					}
					if (horizontal_scroll > 0) horizontal_scroll--;
					else
						scroll_end = true;


					SetDelayTime(30);
					while (!AwaitDelayTime()) {
						// Just wait
					}

					VIDEO_VSync();
					VIDEO_ScreenBufferToVRAM();
				}
				step = 11;
				break;
			case 11:// Eye chat
				chat_spr_num = GFX_FindEmptySpriteSlot();
				if (chat_spr_num == -1) {
					sprintf(engine.system_error_message1, "Scene 1 Intro function error");
					sprintf(engine.system_error_message2, "No empty sprite slot available");
					sprintf(engine.system_error_message3, " ");
					Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
				} else {
					GFX_InitSprite(ENTITY_ID_EMPTY, 0, chat_spr_num, 0, 55, 30);
				}
				GFX_SetSpriteGraphic(chat_spr_num, 0, SPRITE_GRAPHICS_ID_CHAT_SMALL, 0, 0);
				GFX_SetDefaultAnimation(chat_spr_num, false, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 250, 125);
				SetDelayTime(200);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				GFX_UpdateSprites();
				GFX_DrawSprites();
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				char_number = 0;
				dialog_delay_counter = 0;
				while ((char_number < strlen(ui->txt_file[UI_TXT_SCN1I]->line[4])) & engine.sequence) {
					aux_string[char_number] = ui->txt_file[UI_TXT_SCN1I]->line[4][char_number];
					aux_string[char_number + 1] = '\0';
					VIDEO_StringToScreenBuffer(256, 134, aux_string, FONT_SLIM_BLACK);
					dialog_delay_counter++;
					if (dialog_delay_counter > 4) {
						char_number++;
						dialog_delay_counter = 0;
					}

					VIDEO_VSync();
					VIDEO_ScreenBufferToVRAM();
				}
				step = 12;
				break;
			case 12:// Fuck scene
				GFX_PCXImageToBuffer("ISCN1.DAT", "INTRO4H.PCX", 250 * 180, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("ISCN1.DAT", "INTRO4L.PCX", 194 * 180, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				VIDEO_FadeOut(4);
				GFX_UnloadSprite(chat_spr_num);

				horizontal_scroll = 50;
				scroll_end = false;

				VIDEO_ClearScreenBuffer();
				VIDEO_ScreenBufferToVRAM();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 200, 186, 110, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, 194, gfx->image_buffer1_height, 113, 13);
				VIDEO_FadeIn(4);
				while (!scroll_end & engine.sequence) {
					src_index = horizontal_scroll;
					dst_index = CAM_VISIBLE_WIDTH * 13 + 113;

					// Draw image with scroll
					for (i = 0; i < gfx->image_buffer1_height; i++) {
						memcpy(&video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index], &gfx->image_buffer1[src_index], 194);
						src_index += gfx->image_buffer1_width;
						dst_index += CAM_VISIBLE_WIDTH;
					}
					if (horizontal_scroll > 0) horizontal_scroll--;
					else
						scroll_end = true;


					SetDelayTime(30);
					while (!AwaitDelayTime()) {
						// Just wait
					}

					VIDEO_VSync();
					VIDEO_ScreenBufferToVRAM();
				}
				step = 13;
				break;
			case 13:// chat
				chat_spr_num = GFX_FindEmptySpriteSlot();
				if (chat_spr_num == -1) {
					sprintf(engine.system_error_message1, "Scene 1 Intro function error");
					sprintf(engine.system_error_message2, "No empty sprite slot available");
					sprintf(engine.system_error_message3, " ");
					Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
				} else {
					GFX_InitSprite(ENTITY_ID_EMPTY, 0, chat_spr_num, 0, 158, 81);
				}
				GFX_SetSpriteGraphic(chat_spr_num, 0, SPRITE_GRAPHICS_ID_FUCK, 0, 0);
				GFX_SetDefaultAnimation(chat_spr_num, false, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 120, 25);
				SetDelayTime(200);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				GFX_UpdateSprites();
				GFX_DrawSprites();
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();

				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step = 14;
				break;
			case 14:// Unload sprite
				GFX_UnloadSprite(chat_spr_num);
				GFX_UpdateSprites();
				GFX_DrawSprites();
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();

				SetDelayTime(300);
				while (!AwaitDelayTime()) {
					// Just wait
				}

				step = 15;
				break;
			case 15:// Washing face (high)
				GFX_PCXImageToBuffer("ISCN1.DAT", "INTRO5H.PCX", 130 * 60, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 136, 66, 10, 30, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 13, 33);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();

				SetDelayTime(200);
				while (!AwaitDelayTime()) {
					// Just wait
				}

				step = 16;
				break;
			case 16:// Fade fuck image
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 113, 13);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 136, 66, 10, 30, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 13, 33);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();

				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step = 17;
				break;
			case 17:// Dead image
				GFX_PCXImageToBuffer("ISCN1.DAT", "INTRO6H.PCX", 130 * 60, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("ISCN1.DAT", "INTRO5L.PCX", 130 * 60, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 136, 66, 10, 120, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 13, 123);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();

				SetDelayTime(200);
				while (!AwaitDelayTime()) {
					// Just wait
				}

				step = 18;
				break;
			case 18:// Fade washing face image
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 13, 33);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 136, 66, 10, 120, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 13, 123);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();

				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}

				step = 19;
				break;
			case 19:// Fade out and show image
				GFX_PCXImageToBuffer("ISCN1.DAT", "INTRO7H.PCX", 285 * 97, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("ISCN1.DAT", "INTRO8H.PCX", 82 * 100, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				VIDEO_FadeOut(4);

				VIDEO_ClearScreenBuffer();
				VIDEO_ScreenBufferToVRAM();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 291, 103, 15, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 18, 13);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();

				VIDEO_FadeIn(4);
				SetDelayTime(200);
				while (!AwaitDelayTime()) {
					// Just wait
				}

				step = 20;
				break;
			case 20:// chat
				char_number = 0;
				dialog_delay_counter = 0;

				while ((char_number < strlen(ui->txt_file[UI_TXT_SCN1I]->line[6])) & engine.sequence) {
					aux_string[char_number] = ui->txt_file[UI_TXT_SCN1I]->line[6][char_number];
					aux_string[char_number + 1] = '\0';
					VIDEO_StringToScreenBuffer(215, 34, aux_string, FONT_SLIM_BLACK);
					dialog_delay_counter++;
					if (dialog_delay_counter > 4) {
						char_number++;
						dialog_delay_counter = 0;
					}

					VIDEO_VSync();
					VIDEO_ScreenBufferToVRAM();
				}

				char_number = 0;
				dialog_delay_counter = 0;

				while ((char_number < strlen(ui->txt_file[UI_TXT_SCN1I]->line[7])) & engine.sequence) {
					aux_string[char_number] = ui->txt_file[UI_TXT_SCN1I]->line[7][char_number];
					aux_string[char_number + 1] = '\0';
					VIDEO_StringToScreenBuffer(210, 46, aux_string, FONT_SLIM_BLACK);
					dialog_delay_counter++;
					if (dialog_delay_counter > 4) {
						char_number++;
						dialog_delay_counter = 0;
					}

					VIDEO_VSync();
					VIDEO_ScreenBufferToVRAM();
				}

				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}

				step = 21;
				break;
			case 21:// Getting things
				GFX_PCXImageToBuffer("ISCN1.DAT", "INTRO7L.PCX", 285 * 97, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 88, 106, 20, 80, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 23, 83);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();

				SetDelayTime(200);
				while (!AwaitDelayTime()) {
					// Just wait
				}

				step = 22;
				break;
			case 22:// Fade chat
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 18, 13);
				VIDEO_StringToScreenBuffer(215, 34, ui->txt_file[UI_TXT_SCN1I]->line[6], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(210, 46, ui->txt_file[UI_TXT_SCN1I]->line[7], FONT_SLIM_BLACK);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 88, 106, 20, 80, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 23, 83);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();

				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}

				step = 23;
				break;
			case 23:// Show gun
				GFX_PCXImageToBuffer("ISCN1.DAT", "INTRO9H.PCX", 82 * 100, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("ISCN1.DAT", "INTRO8L.PCX", 82 * 100, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 88, 106, 118, 80, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 121, 83);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();

				SetDelayTime(200);
				while (!AwaitDelayTime()) {
					// Just wait
				}

				step = 24;
				break;
			case 24:// Fade getting things
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 23, 83);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();

				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}

				step = 25;
				break;
			case 25:// Show running
				GFX_PCXImageToBuffer("ISCN1.DAT", "INTRO10H.PCX", 194 * 180, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("ISCN1.DAT", "INTRO9L.PCX", 194 * 180, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 88, 106, 213, 80, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 216, 83);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();

				SetDelayTime(200);
				while (!AwaitDelayTime()) {
					// Just wait
				}

				step = 26;
				break;
			case 26:// Fade gun
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 121, 83);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();

				SetDelayTime(200);
				while (!AwaitDelayTime()) {
					// Just wait
				}

				step = 27;
				break;
			case 27:// chat
				char_number = 0;
				dialog_delay_counter = 0;

				while ((char_number < strlen(ui->txt_file[UI_TXT_SCN1I]->line[8])) & engine.sequence) {
					aux_string[char_number] = ui->txt_file[UI_TXT_SCN1I]->line[8][char_number];
					aux_string[char_number + 1] = '\0';
					VIDEO_StringToScreenBuffer(220, 86, aux_string, FONT_SLIM_BLACK);
					dialog_delay_counter++;
					if (dialog_delay_counter > 4) {
						char_number++;
						dialog_delay_counter = 0;
					}

					VIDEO_VSync();
					VIDEO_ScreenBufferToVRAM();
				}
				step = 28;
				break;
			case 28:// wait
				SetDelayTime(5000);
				while (!AwaitDelayTime()) {
					// Just wait
				}

				step = 30;
				break;
			case 30://End sequence
				engine.sequence = false;
				break;
		}
	}
	VIDEO_FadeOut(1);
	ITEM_UnloadItems();
	EFFECT_UnloadEffects();
	PARTICLE_UnloadParticles();
	ENEMY_UnloadEnemies();
	GFX_UnloadSprites();
	AUDIO_StopSong();
	AUDIO_UnloadSong();
	MM_PopChunks(CT_TEMPORARY_SPRITE);
}
void Scene1_LoadAssets(void) {
	ACTOR_Load(1 << 4, 22 << 4, SPRITE_GRAPHICS_ID_ACTOR_FEET, SPRITE_GRAPHICS_ID_ACTOR_BODY, SPRITE_GRAPHICS_ID_ACTOR_HEAD, SPRITE_GRAPHICS_ID_ACTOR_LARM, SPRITE_GRAPHICS_ID_ACTOR_RARM_BARE_HANDS, ACTOR_FACING_RIGHT);
	ACTOR_SetGun(actor->sprite_num, ACTOR_GUN_TYPE_BARE_HANDS);
	ACTOR_SetBulletStatus(99, 0, 0, 0, 0);

	// Load enemies and unload them just to preload graphics
	ENEMY_Load("ENEMY1.DAT", 0, 0, 0, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_SLEEP, 50);
	ENEMY_Load("ENEMY2.DAT", 1, 0, 0, SPRITE_GRAPHICS_ID_ENEMY2_CHAT, SPRITE_GRAPHICS_ID_ENEMY2_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY2_FEET, SPRITE_GRAPHICS_ID_ENEMY2_BODY, SPRITE_GRAPHICS_ID_ENEMY2_HEAD, SPRITE_GRAPHICS_ID_ENEMY2_LARM, SPRITE_GRAPHICS_ID_ENEMY2_RARM, ENEMY_FACING_RIGHT, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_SLEEP, 100);
	ENEMY_Load("ENEMY3.DAT", 2, 0, 0, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_SLEEP, 100);
	ENEMY_Load("ENEMY4.DAT", 3, 0, 0, SPRITE_GRAPHICS_ID_ENEMY4_CHAT, SPRITE_GRAPHICS_ID_ENEMY4_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY4_FEET, SPRITE_GRAPHICS_ID_ENEMY4_BODY, SPRITE_GRAPHICS_ID_ENEMY4_HEAD, SPRITE_GRAPHICS_ID_ENEMY4_LARM, SPRITE_GRAPHICS_ID_ENEMY4_RARM, ENEMY_FACING_RIGHT, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_SLEEP, 100);
	ENEMY_Load("ENEMY5.DAT", 4, 0, 0, SPRITE_GRAPHICS_ID_ENEMY5_CHAT, SPRITE_GRAPHICS_ID_ENEMY5_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY5_FEET, SPRITE_GRAPHICS_ID_ENEMY5_BODY, SPRITE_GRAPHICS_ID_ENEMY5_HEAD, SPRITE_GRAPHICS_ID_ENEMY5_LARM, SPRITE_GRAPHICS_ID_ENEMY5_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_SLEEP, 100);
	ENEMY_Load("ENEMY12.DAT", 5, 0, 0, SPRITE_GRAPHICS_ID_ENEMY6_CHAT, SPRITE_GRAPHICS_ID_ENEMY6_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY6_FEET, SPRITE_GRAPHICS_ID_ENEMY6_BODY, SPRITE_GRAPHICS_ID_ENEMY6_HEAD, SPRITE_GRAPHICS_ID_ENEMY6_LARM, SPRITE_GRAPHICS_ID_ENEMY6_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_SLEEP, 100);
	ENEMY_UnloadEnemies();

	// Load npcs and unload them just to preload graphics
	NPC_Load("NPC1.DAT", 0, 0, 0, SPRITE_GRAPHICS_ID_NPC_AGENT1_CHAT, SPRITE_GRAPHICS_ID_NPC_AGENT1, NPC_FACING_DOWN, NPC_STATUS_IDLE, 200);
	NPC_Load("NPC2.DAT", 1, 0, 0, SPRITE_GRAPHICS_ID_NPC_AGENT2_CHAT, SPRITE_GRAPHICS_ID_NPC_AGENT2, NPC_FACING_DOWN, NPC_STATUS_IDLE, 200);
	NPC_Load("NPC3.DAT", 2, 0, 0, SPRITE_GRAPHICS_ID_NPC_BOSS_CHAT, SPRITE_GRAPHICS_ID_NPC_BOSS, NPC_FACING_DOWN, NPC_STATUS_IDLE, 20);
	NPC_Load("NPC5.DAT", 3, 0, 0, SPRITE_GRAPHICS_ID_NPC_DROID1_CHAT, SPRITE_GRAPHICS_ID_NPC_DROID1, NPC_FACING_DOWN, NPC_STATUS_IDLE, 200);
	NPC_Load("NPC6.DAT", 4, 0, 0, SPRITE_GRAPHICS_ID_NPC_DROID2_CHAT, SPRITE_GRAPHICS_ID_NPC_DROID2, NPC_FACING_DOWN, NPC_STATUS_IDLE, 200);
	NPC_Load("NPC7.DAT", 5, 0, 0, SPRITE_GRAPHICS_ID_NPC_DROID3_CHAT, SPRITE_GRAPHICS_ID_NPC_DROID3, NPC_FACING_DOWN, NPC_STATUS_IDLE, 400);
	NPC_Load("NPC8.DAT", 6, 0, 0, SPRITE_GRAPHICS_ID_NPC_SARA_CHAT, SPRITE_GRAPHICS_ID_NPC_SARA, NPC_FACING_DOWN, NPC_STATUS_IDLE, 100);
	NPC_UnloadNpcs();

	// Load objects and unload them just to preload graphics
	OBJECT_LoadObject("OBJECT2.DAT", 0, ENTITY_ID_BARREL, SPRITE_GRAPHICS_ID_OBJECT1, SPRITE_GRAPHICS_ID_OBJECT1_PORTAIT, 0, 0);
	OBJECT_LoadObject("OBJECT3.DAT", 1, ENTITY_ID_TARGET, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 0, 0);
	OBJECT_UnloadObjects();

	UI_SetStatusPanels();// Set status panels
}
void Scene1_UnloadAssets(void) {
	// Enemy 1
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_CHAT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_FEET);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_BODY);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_HEAD);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_RARM);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_LARM);

	// Enemy 2
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY2_CHAT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY2_PORTAIT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY2_FEET);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY2_BODY);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY2_HEAD);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY2_RARM);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY2_LARM);

	// Enemy 1
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY3_CHAT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY3_FEET);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY3_BODY);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY3_HEAD);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY3_RARM);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY3_LARM);

	// Enemy 1
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY4_CHAT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY4_PORTAIT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY4_FEET);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY4_BODY);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY4_HEAD);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY4_RARM);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY4_LARM);

	// Enemy 1
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY5_CHAT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY5_PORTAIT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY5_FEET);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY5_BODY);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY5_HEAD);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY5_RARM);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY5_LARM);

	// NPCs
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_NPC_SARA_CHAT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_NPC_SARA);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_NPC_DROID1);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_NPC_DROID1_CHAT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_NPC_AGENT1);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_NPC_AGENT1_CHAT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_NPC_AGENT2);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_NPC_AGENT2_CHAT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_NPC_BOSS);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_NPC_BOSS_CHAT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_NPC_DROID2);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_NPC_DROID2_CHAT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_NPC_DROID3);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_NPC_DROID3_CHAT);

	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_OBJECT1);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_OBJECT1_PORTAIT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_OBJECT2);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT);

	MM_PopChunks(CT_TEMPORARY_SPRITE);// free level temporary mem
}
void Scene1_LoadRoom1(void) {
	ENEMY_Load("ENEMY1.DAT", 0, 6 << 4, 17 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_STATIC_NPC, 50);
	ENEMY_Load("ENEMY1.DAT", 1, 10 << 4, 17 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_STATIC_NPC, 50);
	ENEMY_Load("ENEMY1.DAT", 2, 29 << 4, 17 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_STATIC_NPC, 50);
	ENEMY_Load("ENEMY1.DAT", 3, 21 << 4, 23 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_WALKING_RANDOM_NPC, 50);
	ENEMY_Load("ENEMY1.DAT", 4, 53 << 4, 22 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_WALKING_RANDOM_NPC, 50);
	ENEMY_Load("ENEMY1.DAT", 5, 70 << 4, 12 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_WALKING_RANDOM_NPC, 50);

	MAP_LoadMap("MAPSCN11.DAT", 80, 30, "TSCN11.DAT", "SCN1_1_BACK.PCX", "SCN1_1_FORE.PCX", "SCN1_1_MASK.PCX", 320 * 416, 128 * 128, 128 * 128);

	AUDIO_LoadSong(AUDIO_SONG_5);// Load song
}
void Scene1_LoadRoom2(void) {

	// Enemies on jail
	ENEMY_Load("ENEMY1.DAT", 2, 44 << 4, 27 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_WALKING_RANDOM_NPC, 50);
	ENEMY_Load("ENEMY1.DAT", 3, 44 << 4, 37 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_WALKING_RANDOM_NPC, 50);
	// Enemy on ring
	ENEMY_Load("ENEMY12.DAT", 4, 38 << 4, 10 << 4, SPRITE_GRAPHICS_ID_ENEMY6_CHAT, SPRITE_GRAPHICS_ID_ENEMY6_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY6_FEET, SPRITE_GRAPHICS_ID_ENEMY6_BODY, SPRITE_GRAPHICS_ID_ENEMY6_HEAD, SPRITE_GRAPHICS_ID_ENEMY6_LARM, SPRITE_GRAPHICS_ID_ENEMY6_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_STATIC_NPC, 5000);
	// Sara
	NPC_Load("NPC8.DAT", 1, 39 << 4, 48 << 4, SPRITE_GRAPHICS_ID_NPC_SARA_CHAT, SPRITE_GRAPHICS_ID_NPC_SARA, NPC_FACING_DOWN, NPC_STATUS_IDLE, 100);
	// Desk droid
	NPC_Load("NPC5.DAT", 2, 37 << 4, 44 << 4, SPRITE_GRAPHICS_ID_NPC_DROID1_CHAT, SPRITE_GRAPHICS_ID_NPC_DROID1, NPC_FACING_DOWN, NPC_STATUS_IDLE, 200);
	// Agent 1
	NPC_Load("NPC1.DAT", 3, 9 << 4, 25 << 4, SPRITE_GRAPHICS_ID_NPC_AGENT1_CHAT, SPRITE_GRAPHICS_ID_NPC_AGENT1, NPC_FACING_DOWN, NPC_STATUS_IDLE, 200);
	// Agent 2
	NPC_Load("NPC2.DAT", 4, 14 << 4, 25 << 4, SPRITE_GRAPHICS_ID_NPC_AGENT2_CHAT, SPRITE_GRAPHICS_ID_NPC_AGENT2, NPC_FACING_DOWN, NPC_STATUS_IDLE, 200);
	// Boss
	NPC_Load("NPC3.DAT", 5, (11 << 4) + 4, (26 << 4) + 4, SPRITE_GRAPHICS_ID_NPC_BOSS_CHAT, SPRITE_GRAPHICS_ID_NPC_BOSS, NPC_FACING_DOWN, NPC_STATUS_IDLE, 20);
	// Inside Bathroom droid
	NPC_Load("NPC5.DAT", 6, 6 << 4, 56 << 4, SPRITE_GRAPHICS_ID_NPC_DROID1_CHAT, SPRITE_GRAPHICS_ID_NPC_DROID1, NPC_FACING_DOWN, NPC_STATUS_IDLE, 200);
	// Jail droid
	NPC_Load("NPC7.DAT", 7, 28 << 4, 30 << 4, SPRITE_GRAPHICS_ID_NPC_DROID3_CHAT, SPRITE_GRAPHICS_ID_NPC_DROID3, NPC_FACING_DOWN, NPC_STATUS_IDLE, 400);
	// Training droid
	NPC_Load("NPC6.DAT", 8, 30 << 4, 18 << 4, SPRITE_GRAPHICS_ID_NPC_DROID2_CHAT, SPRITE_GRAPHICS_ID_NPC_DROID2, NPC_FACING_DOWN, NPC_STATUS_IDLE, 800);
	NPC_SetRange(8, 64);
	// Outside bathroom droid
	NPC_Load("NPC5.DAT", 9, 7 << 4, 47 << 4, SPRITE_GRAPHICS_ID_NPC_DROID1_CHAT, SPRITE_GRAPHICS_ID_NPC_DROID1, NPC_FACING_DOWN, NPC_STATUS_IDLE, 200);
	// Target pistol
	OBJECT_LoadObject("OBJECT3.DAT", 1, ENTITY_ID_TARGET, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, (4 << 4) + 8, 7 << 4);
	// Target shotgun
	OBJECT_LoadObject("OBJECT3.DAT", 2, ENTITY_ID_TARGET, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, (9 << 4) + 8, 9 << 4);
	// Target uzi
	OBJECT_LoadObject("OBJECT3.DAT", 3, ENTITY_ID_TARGET, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, (14 << 4) + 8, 7 << 4);
	// Target snipper
	OBJECT_LoadObject("OBJECT3.DAT", 4, ENTITY_ID_TARGET, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, (19 << 4) + 8, 6 << 4);

	MAP_LoadMap("MAPSCN12.DAT", 50, 60, "TSCN12.DAT", "SCN1_2_BACK.PCX", "SCN1_2_FORE.PCX", "SCN1_2_MASK.PCX", 320 * 416, 128 * 128, 128 * 128);

	AUDIO_LoadSong(AUDIO_SONG_5);// Load song
}
void Scene1_SetHotspotsAndEvents(void) {

	// Initialize hotspots and events
	// Update hotspots
	map->hotspot_enabling_room1[0] = false;
	map->hotspot_enabling_room1[1] = true;
	map->hotspot_enabling_room1[2] = true;
	map->hotspot_enabling_room1[3] = true;
	map->hotspot_enabling_room1[4] = true;
	map->hotspot_enabling_room1[5] = true;
	map->hotspot_enabling_room1[6] = true;
	map->hotspot_enabling_room1[7] = true;
	map->hotspot_enabling_room1[8] = true;
	map->hotspot_enabling_room1[9] = true;
	map->hotspot_enabling_room1[10] = true;
	map->hotspot_enabling_room1[11] = true;
	map->hotspot_enabling_room1[12] = false;
	map->hotspot_enabling_room1[13] = false;
	map->hotspot_enabling_room1[14] = false;
	map->hotspot_enabling_room1[15] = false;
	map->hotspot_enabling_room1[16] = false;
	map->hotspot_enabling_room1[17] = false;
	map->hotspot_enabling_room1[18] = false;
	map->hotspot_enabling_room1[19] = false;
	map->hotspot_enabling_room1[20] = false;
	map->hotspot_enabling_room1[21] = false;
	map->hotspot_enabling_room1[22] = false;
	map->hotspot_enabling_room1[23] = false;
	map->hotspot_enabling_room1[24] = false;
	map->hotspot_enabling_room1[25] = false;
	map->hotspot_enabling_room1[26] = false;
	map->hotspot_enabling_room1[27] = false;
	map->hotspot_enabling_room1[28] = false;
	map->hotspot_enabling_room1[29] = false;
	map->hotspot_enabling_room1[30] = false;
	map->hotspot_enabling_room1[31] = false;

	// Update events
	map->event_enabling_room1[0] = false;
	map->event_enabling_room1[1] = true;
	map->event_enabling_room1[2] = true;
	map->event_enabling_room1[3] = true;
	map->event_enabling_room1[4] = true;
	map->event_enabling_room1[5] = true;
	map->event_enabling_room1[6] = true;
	map->event_enabling_room1[7] = false;
	map->event_enabling_room1[8] = false;
	map->event_enabling_room1[9] = false;
	map->event_enabling_room1[10] = false;
	map->event_enabling_room1[11] = false;
	map->event_enabling_room1[12] = false;
	map->event_enabling_room1[13] = false;
	map->event_enabling_room1[14] = false;
	map->event_enabling_room1[15] = false;
	map->event_enabling_room1[16] = false;
	map->event_enabling_room1[17] = false;
	map->event_enabling_room1[18] = false;
	map->event_enabling_room1[19] = false;
	map->event_enabling_room1[20] = false;
	map->event_enabling_room1[21] = false;
	map->event_enabling_room1[22] = false;
	map->event_enabling_room1[23] = false;
	map->event_enabling_room1[24] = false;
	map->event_enabling_room1[25] = false;
	map->event_enabling_room1[26] = false;
	map->event_enabling_room1[27] = false;
	map->event_enabling_room1[28] = false;
	map->event_enabling_room1[29] = false;
	map->event_enabling_room1[30] = false;
	map->event_enabling_room1[31] = false;

	// Update hotspots
	map->hotspot_enabling_room2[0] = false;
	map->hotspot_enabling_room2[1] = true;
	map->hotspot_enabling_room2[2] = true;
	map->hotspot_enabling_room2[3] = true;
	map->hotspot_enabling_room2[4] = true;
	map->hotspot_enabling_room2[5] = true;
	map->hotspot_enabling_room2[6] = true;
	map->hotspot_enabling_room2[7] = true;
	map->hotspot_enabling_room2[8] = true;
	map->hotspot_enabling_room2[9] = false;
	map->hotspot_enabling_room2[10] = false;
	map->hotspot_enabling_room2[11] = false;
	map->hotspot_enabling_room2[12] = false;
	map->hotspot_enabling_room2[13] = false;
	map->hotspot_enabling_room2[14] = false;
	map->hotspot_enabling_room2[15] = false;
	map->hotspot_enabling_room2[16] = false;
	map->hotspot_enabling_room2[17] = false;
	map->hotspot_enabling_room2[18] = false;
	map->hotspot_enabling_room2[19] = false;
	map->hotspot_enabling_room2[20] = false;
	map->hotspot_enabling_room2[21] = false;
	map->hotspot_enabling_room2[22] = false;
	map->hotspot_enabling_room2[23] = false;
	map->hotspot_enabling_room2[24] = false;
	map->hotspot_enabling_room2[25] = false;
	map->hotspot_enabling_room2[26] = false;
	map->hotspot_enabling_room2[27] = false;
	map->hotspot_enabling_room2[28] = false;
	map->hotspot_enabling_room2[29] = false;
	map->hotspot_enabling_room2[30] = false;
	map->hotspot_enabling_room2[31] = false;

	// Update events
	map->event_enabling_room2[0] = false;
	map->event_enabling_room2[1] = true;// Go back to room 1
	map->event_enabling_room2[2] = true;
	map->event_enabling_room2[3] = true;
	map->event_enabling_room2[4] = true;
	map->event_enabling_room2[5] = true;
	map->event_enabling_room2[6] = true;
	map->event_enabling_room2[7] = true;
	map->event_enabling_room2[8] = true;
	map->event_enabling_room2[9] = true;
	map->event_enabling_room2[10] = true;
	map->event_enabling_room2[11] = true;// Sara conversation 1
	map->event_enabling_room2[12] = true;
	map->event_enabling_room2[13] = true;// Desktop droid
	map->event_enabling_room2[14] = true;
	map->event_enabling_room2[15] = true;
	map->event_enabling_room2[16] = true;
	map->event_enabling_room2[17] = false;// Sara conversation 2
	map->event_enabling_room2[18] = true;
	map->event_enabling_room2[19] = true;
	map->event_enabling_room2[20] = true;// Entering speach
	map->event_enabling_room2[21] = false;
	map->event_enabling_room2[22] = false;
	map->event_enabling_room2[23] = false;
	map->event_enabling_room2[24] = false;
	map->event_enabling_room2[25] = false;
	map->event_enabling_room2[26] = false;
	map->event_enabling_room2[27] = false;
	map->event_enabling_room2[28] = false;
	map->event_enabling_room2[29] = false;
	map->event_enabling_room2[30] = false;
	map->event_enabling_room2[31] = false;
}
void Scene1_Loop(void) {
	bool end_sequence;
	bool tutorial;
	int actor_tile_num;
	int sequence_step;
	int scene_step;

	VIDEO_ClearScreenBuffer();
	GFX_LoadPalette("PALETTES.DAT", "SCN11.PCX", 256);

	VIDEO_StringToScreenBuffer(50, 60, ui->txt_file[UI_TXT_SCN1I]->line[10], FONT_BIG_BLACK);
	VIDEO_StringToScreenBuffer(90, 85, ui->txt_file[UI_TXT_SCN1I]->line[11], FONT_BIG_BLACK);
	VIDEO_StringToScreenBuffer(90, 135, ui->txt_file[UI_TXT_SCN1I]->line[12], FONT_SLIM_WHITE);
	VIDEO_StringToScreenBuffer(160, 135, ui->txt_file[UI_TXT_SCN1I]->line[13], FONT_SLIM_WHITE);

	VIDEO_VSync();
	VIDEO_ScreenBufferToVRAM();
	VIDEO_FadeIn(4);

	SetDelayTime(6000);

	Scene1_LoadAssets();
	Scene1_SetHotspotsAndEvents();

	// Initialize the Scene room
	switch (engine.room) {
		case 1:// Room 1. Outside doscity
			Scene1_LoadRoom1();
			ACTOR_SetPosition(1 << 4, 22 << 4, ACTOR_FACING_RIGHT);
			ACTOR_SetCombatMode(false);
			CAM_Init(map->width_px, map->height_px, 0 << 4, 18 << 4);// Initialize camera
			MAP_DrawMapToMapVideoBuffer();
			TIMER_UpdateTimerTime(TIMER_AUDIO_NUMBER, 55);
			break;
		case 2:// Room 2. Inside police office
			Scene1_LoadRoom2();
			ACTOR_SetPosition(46 << 4, 49 << 4, ACTOR_FACING_LEFT);
			ACTOR_SetCombatMode(false);
			CAM_Init(map->width_px, map->height_px, 30 << 4, 42 << 4);// Initialize camera
			MAP_DrawMapToMapVideoBuffer();
			TIMER_UpdateTimerTime(TIMER_AUDIO_NUMBER, 55);
			break;
		default:
			sprintf(engine.system_error_message1, "Scene1_Loop function error");
			sprintf(engine.system_error_message2, "Undefined room");
			sprintf(engine.system_error_message3, "Selected room: %u", engine.room);
			Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_SYSTEM);
			break;
	}

	VIDEO_FadeOut(4);
	VIDEO_ClearScreenBuffer();

	// Set palette
	switch (engine.room) {
		case 1:// Room 1. Outside doscity
			GFX_LoadPalette("PALETTES.DAT", "SCN11.PCX", 256);
			break;
		case 2:// Room 2. Inside police office
			GFX_LoadPalette("PALETTES.DAT", "SCN12.PCX", 256);
			break;
		default:
			sprintf(engine.system_error_message1, "Scene1_Loop function error");
			sprintf(engine.system_error_message2, "Undefined room");
			sprintf(engine.system_error_message3, "Selected room: %u", engine.room);
			Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_SYSTEM);
			break;
	}

	Update(true);
	Update(true);

	tutorial = false;
	scene_step = 0;
	sequence_step = 0;
	end_sequence = false;
	engine.ingame = true;

	while (!AwaitDelayTime()) {
		// Just wait
	}

	AUDIO_PlaySong(true);
	VIDEO_FadeIn(1);
	MOUSE_ShowCursor();

	// Loop until the game is over
	while (engine.ingame) {
		switch (engine.room) {
			case 1:// Outside
				if (!ui->freeze) {
					// Hotspots
					if (!actor->mode_combat) {
						// Check point on object or enemy
						switch (cursor.point_on & 0xFF) {
							case ENTITY_ID_HSPOT:// Hotspot
								switch ((cursor.point_on >> 8) & 0xFF) {
									case 0:// No hotspot, WTF??
										break;
									case 1:// Paint
										if (map->hotspot_enabling_room1[1]) {
											if (cursor.left_click) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1H, 101, 102, 103, false, 100);

											} else {
												UI_ShowDescription(UI_TXT_SCN1H, 1);
											}
										}
										break;
									case 2:// Cult door
										if (map->hotspot_enabling_room1[2]) {
											if (cursor.left_click) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1H, 104, 105, 106, false, 100);
												while (!UI_IsSpeechFinished()) {
													Update(false);
												}
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1H, 107, 108, 109, false, 100);
												while (!UI_IsSpeechFinished()) {
													Update(false);
												}
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1H, 110, 111, 112, false, 100);

											} else {
												UI_ShowDescription(UI_TXT_SCN1H, 2);
											}
										}
										break;
									case 3:// Container
										if (map->hotspot_enabling_room1[3]) {
											if (cursor.left_click) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1H, 113, 114, 115, false, 100);

											} else {
												UI_ShowDescription(UI_TXT_SCN1H, 3);
											}
										}
										break;
									case 4:// Bed
										if (map->hotspot_enabling_room1[4]) {
											if (cursor.left_click) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1H, 116, 117, 118, false, 100);
											} else {
												UI_ShowDescription(UI_TXT_SCN1H, 4);
											}
										}
										break;
									case 5:// Garbage
										if (map->hotspot_enabling_room1[5]) {
											if (cursor.left_click) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1H, 119, 120, 121, false, 100);
											} else {
												UI_ShowDescription(UI_TXT_SCN1H, 5);
											}
										}
										break;
									case 6:// Police office
										if (map->hotspot_enabling_room1[6]) {
											if (cursor.left_click) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1H, 122, 123, 124, false, 100);
											} else {
												UI_ShowDescription(UI_TXT_SCN1H, 6);
											}
										}
										break;
									case 7:// Accident
										if (map->hotspot_enabling_room1[7]) {
											if (cursor.left_click) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1H, 125, 126, 127, false, 100);
												while (!UI_IsSpeechFinished()) {
													Update(false);
												}
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1H, 128, 129, 130, false, 100);
											} else {
												UI_ShowDescription(UI_TXT_SCN1H, 7);
											}
										}
										break;
									case 8:// Paint
										if (map->hotspot_enabling_room1[8]) {
											if (cursor.left_click) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1H, 131, 132, 133, false, 100);
											} else {
												UI_ShowDescription(UI_TXT_SCN1H, 8);
											}
										}
										break;
									case 9:// Paint
										if (map->hotspot_enabling_room1[9]) {
											if (cursor.left_click) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1H, 134, 135, 136, false, 100);
												while (!UI_IsSpeechFinished()) {
													Update(false);
												}
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1H, 137, 138, 139, false, 100);
											} else {
												UI_ShowDescription(UI_TXT_SCN1H, 9);
											}
										}
										break;
									case 10:// Paint
										if (map->hotspot_enabling_room1[10]) {
											if (cursor.left_click) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1H, 140, 141, 142, false, 100);
											} else {
												UI_ShowDescription(UI_TXT_SCN1H, 10);
											}
										}
										break;
									case 11:// XXX
										if (map->hotspot_enabling_room1[11]) {
											if (cursor.left_click) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1H, 143, 144, 145, false, 100);
											} else {
												UI_ShowDescription(UI_TXT_SCN1H, 11);
											}
										}
										break;
									case 12:// Spare
										if (map->hotspot_enabling_room1[12]) {
										}
										break;
									case 13:// Spare
										if (map->hotspot_enabling_room1[13]) {
										}
										break;
									case 14:// Spare
										if (map->hotspot_enabling_room1[14]) {
										}
										break;
									case 15:// Spare
										if (map->hotspot_enabling_room1[15]) {
										}
										break;
									case 16:// Spare
										if (map->hotspot_enabling_room1[16]) {
										}
										break;
									default:
										break;
								}
							default:
								break;
						}
					}

					// Events
					actor_tile_num = MAP_GetTileNumber(actor->pos_x + (gfx_sprite_stack[actor->sprite_num].width_px >> 1), actor->pos_y + (gfx_sprite_stack[actor->sprite_num].height_px >> 1));
					switch ((map->layer[MAP_EVENT_HSPOT_LAYER][actor_tile_num] >> 8) & 0xFF) {
						case 1:// Event 1. Enter door. Go to room 2
							if (map->event_enabling_room1[1]) {
								VIDEO_FadeOut(4);

								AUDIO_StopSong();
								AUDIO_UnloadSong();
								NPC_UnloadNpcs();
								ENEMY_UnloadEnemies();
								OBJECT_UnloadObjects();
								EFFECT_UnloadEffects();
								BULLET_UnloadBullets();
								PARTICLE_UnloadParticles();
								GRENADE_UnloadGrenades();
								MAP_UnloadMap();

								Scene1_LoadRoom2();
								GFX_LoadPalette("PALETTES.DAT", "SCN12.PCX", 256);

								AUDIO_LoadSong(AUDIO_SONG_5);

								ACTOR_SetPosition(46 << 4, 49 << 4, ACTOR_FACING_LEFT);
								CAM_Init(map->width_px, map->height_px, 30 << 4, 42 << 4);// Initialize camera
								MAP_DrawMapToMapVideoBuffer();

								Update(true);
								Update(true);

								engine.room = 2;

								VIDEO_FadeIn(1);

								TIMER_UpdateTimerTime(TIMER_AUDIO_NUMBER, 55);
								AUDIO_PlaySong(true);

								ACTOR_SetGun(actor->sprite_num, ACTOR_GUN_TYPE_BARE_HANDS);
								ACTOR_SetCombatMode(false);
								UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 61, 62, 63, false, 100);
							}
							break;
						case 2:// Event 2. Thugs
							if (map->event_enabling_room1[2]) {
								map->event_enabling_room1[2] = false;// Clear event 2
								sequence_step = 0;

								ACTOR_SetFreeze(true);

								while (!end_sequence) {
									switch (sequence_step) {
										case 0:// Show enemy 6
											ENEMY_Load("ENEMY2.DAT", 6, 40 << 4, 5 << 4, SPRITE_GRAPHICS_ID_ENEMY2_CHAT, SPRITE_GRAPHICS_ID_ENEMY2_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY2_FEET, SPRITE_GRAPHICS_ID_ENEMY2_BODY, SPRITE_GRAPHICS_ID_ENEMY2_HEAD, SPRITE_GRAPHICS_ID_ENEMY2_LARM, SPRITE_GRAPHICS_ID_ENEMY2_RARM, ENEMY_FACING_RIGHT, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_STATIC_NPC, 100);
											SetDelayTime(500);
											sequence_step++;
											break;
										case 1:// Wait
											if (AwaitDelayTime()) sequence_step++;
											break;
										case 2:// Show enemy 7
											SetDelayTime(500);
											ENEMY_Load("ENEMY3.DAT", 7, 47 << 4, 5 << 4, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_STATIC_NPC, 100);
											sequence_step++;
											break;
										case 3:// Wait
											if (AwaitDelayTime()) sequence_step++;
											break;
										case 4:// Show enemy 8
											SetDelayTime(500);
											ENEMY_Load("ENEMY4.DAT", 8, 40 << 4, 4 << 4, SPRITE_GRAPHICS_ID_ENEMY4_CHAT, SPRITE_GRAPHICS_ID_ENEMY4_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY4_FEET, SPRITE_GRAPHICS_ID_ENEMY4_BODY, SPRITE_GRAPHICS_ID_ENEMY4_HEAD, SPRITE_GRAPHICS_ID_ENEMY4_LARM, SPRITE_GRAPHICS_ID_ENEMY4_RARM, ENEMY_FACING_RIGHT, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_STATIC_NPC, 100);
											sequence_step++;
											break;
										case 5:// Wait
											if (AwaitDelayTime()) sequence_step++;
											break;
										case 6:// Show enemy 9
											SetDelayTime(500);
											ENEMY_Load("ENEMY5.DAT", 9, 47 << 4, 4 << 4, SPRITE_GRAPHICS_ID_ENEMY5_CHAT, SPRITE_GRAPHICS_ID_ENEMY5_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY5_FEET, SPRITE_GRAPHICS_ID_ENEMY5_BODY, SPRITE_GRAPHICS_ID_ENEMY5_HEAD, SPRITE_GRAPHICS_ID_ENEMY5_LARM, SPRITE_GRAPHICS_ID_ENEMY5_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_STATIC_NPC, 100);
											sequence_step++;
											break;
										case 7:// Wait
											if (AwaitDelayTime()) sequence_step++;
											break;
										case 8://
											SetDelayTime(1000);
											sequence_step++;
										case 9://
											if (AwaitDelayTime()) sequence_step++;
											break;
										case 10://
											UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 37, 38, 39, true, 100);
											sequence_step++;
											break;
										case 11://
											if (UI_IsSpeechFinished()) sequence_step++;
											break;
										case 12://
											UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ENEMY2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 40, 41, 42, true, 60);
											sequence_step++;
											break;
										case 13://
											if (UI_IsSpeechFinished()) sequence_step++;
											break;
										case 14://
											UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 43, 44, 45, true, 100);
											sequence_step++;
											break;
										case 15://
											if (UI_IsSpeechFinished()) sequence_step++;
											break;
										case 16://
											UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 46, 47, 48, false, 100);
											sequence_step++;
											break;
										case 17://
											if (UI_IsSpeechFinished()) sequence_step++;
											break;
										case 18://
											UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 49, 50, 51, true, 100);
											sequence_step++;
											break;
										case 19://
											if (UI_IsSpeechFinished()) sequence_step++;
											break;
										case 20://
											UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ENEMY4_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 52, 53, 54, true, 100);
											sequence_step++;
											break;
										case 21://
											if (UI_IsSpeechFinished()) sequence_step++;
											break;
										case 22://
											UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ENEMY5_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 55, 56, 57, true, 100);
											sequence_step++;
											break;
										case 23://
											if (UI_IsSpeechFinished()) sequence_step++;
											break;
										case 24://
											UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 58, 59, 60, false, 100);
											sequence_step++;
											break;
										case 25://
											if (UI_IsSpeechFinished()) sequence_step++;
											break;
										case 26:
											ENEMY_SetBehavior(6, ENEMY_STATUS_ATTACK);
											ENEMY_SetBehavior(7, ENEMY_STATUS_ATTACK);
											ENEMY_SetBehavior(8, ENEMY_STATUS_ATTACK);
											ENEMY_SetBehavior(9, ENEMY_STATUS_ATTACK);
											ACTOR_SetCombatMode(true);
											ACTOR_SetFreeze(false);
											end_sequence = true;
											break;
										default:
											ENEMY_SetBehavior(6, ENEMY_STATUS_ATTACK);
											ENEMY_SetBehavior(7, ENEMY_STATUS_ATTACK);
											ENEMY_SetBehavior(8, ENEMY_STATUS_ATTACK);
											ENEMY_SetBehavior(9, ENEMY_STATUS_ATTACK);
											ACTOR_SetCombatMode(true);
											ACTOR_SetFreeze(false);
											end_sequence = true;
											break;
									}
									Update(false);
								}
								end_sequence = false;
								AUDIO_StopSong();
								AUDIO_UnloadSong();
								AUDIO_LoadSong(AUDIO_SONG_7);// Load song
								TIMER_UpdateTimerTime(TIMER_AUDIO_NUMBER, 50);
								AUDIO_PlaySong(true);
							}
							break;
						case 3:// Event 3. Blocked path
							if (map->event_enabling_room1[3]) {
								// Clear event 3
								map->event_enabling_room1[3] = false;
								UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 1, 2, 3, false, 100);
							}
							break;
						case 4:// Event 4. Cultists
							if (map->event_enabling_room1[4]) {
								// Clear event 4
								map->event_enabling_room1[4] = false;
								UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 4, 5, 6, true, 100);
								while (!UI_IsSpeechFinished()) {
									Update(false);
								}
								UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 7, 8, 9, false, 100);
								while (!UI_IsSpeechFinished()) {
									Update(false);
								}
								UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 10, 11, 12, true, 100);
								while (!UI_IsSpeechFinished()) {
									Update(false);
								}
								UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 13, 14, 15, true, 100);
								while (!UI_IsSpeechFinished()) {
									Update(false);
								}
								UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 16, 17, 18, false, 100);
								while (!UI_IsSpeechFinished()) {
									Update(false);
								}
								UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 19, 20, 21, false, 100);
								while (!UI_IsSpeechFinished()) {
									Update(false);
								}
								UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 22, 23, 24, false, 100);
							}
							break;
						case 5:// Event 5. XXX
							if (map->event_enabling_room1[5]) {
								// Clear event 5
								map->event_enabling_room1[5] = false;
								UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 25, 26, 27, true, 100);
								while (!UI_IsSpeechFinished()) {
									Update(false);
								}
								UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 28, 29, 30, false, 100);
								while (!UI_IsSpeechFinished()) {
									Update(false);
								}
								UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 31, 32, 33, false, 100);
								while (!UI_IsSpeechFinished()) {
									Update(false);
								}
								UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 34, 35, 36, true, 100);
							}
							break;
						case 6:// Event 6. Open door
							if (map->event_enabling_room1[6]) {
								map->event_enabling_room1[6] = false;

								MAP_HideSwapableTile(72 << 4, 1 << 4);
								MAP_HideSwapableTile(73 << 4, 1 << 4);
								MAP_HideSwapableTile(74 << 4, 1 << 4);
								MAP_HideSwapableTile(72 << 4, 2 << 4);
								MAP_HideSwapableTile(73 << 4, 2 << 4);
								MAP_HideSwapableTile(74 << 4, 2 << 4);
								MAP_HideSwapableTile(72 << 4, 3 << 4);
								MAP_HideSwapableTile(73 << 4, 3 << 4);
								MAP_HideSwapableTile(74 << 4, 3 << 4);
							}
							break;
						default:
							break;
					}

					// Disable event 4 if cultist are not at door
					if ((enemy[0].status_behavior != ENEMY_STATUS_STATIC_NPC) && (enemy[1].status_behavior != ENEMY_STATUS_STATIC_NPC)) {
						map->event_enabling_room1[4] = false;
					}

					// Disable event 5 if cultist are not at door
					if (enemy[2].status_behavior != ENEMY_STATUS_STATIC_NPC) {
						map->event_enabling_room1[5] = false;
					}

					// Thugs die
					if (enemy[6].status_behavior != ENEMY_STATUS_STATIC_NPC) {
						map->event_enabling_room1[5] = false;
					}
				}
				break;
			case 2:// Police office
				if (!ui->freeze) {
					// Hotspots
					if (!actor->mode_combat) {
						// Check point on object or enemy
						switch (cursor.point_on & 0xFF) {
							case ENTITY_ID_HSPOT:// Hotspot
								switch ((cursor.point_on >> 8) & 0xFF) {
									case 0:// No hotspot, WTF??
										break;
									case 1:// Camera
										if (map->hotspot_enabling_room2[1]) {
											if (cursor.left_click) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1H, 146, 147, 148, false, 100);

											} else {
												UI_ShowDescription(UI_TXT_SCN1H, 13);
											}
										}
										break;
									case 2:// Computer
										if (map->hotspot_enabling_room2[2]) {
											if (cursor.left_click) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1H, 149, 150, 151, false, 100);

											} else {
												UI_ShowDescription(UI_TXT_SCN1H, 14);
											}
										}
										break;
									case 3:// Label
										if (map->hotspot_enabling_room2[3]) {
											if (cursor.left_click) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1H, 152, 153, 154, false, 100);

											} else {
												UI_ShowDescription(UI_TXT_SCN1H, 15);
											}
										}
										break;
									case 4:// Gun
										if (map->hotspot_enabling_room2[4]) {
											if (cursor.left_click) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1H, 155, 156, 157, false, 100);

											} else {
												UI_ShowDescription(UI_TXT_SCN1H, 16);
											}
										}
										break;
									case 5:// Shootgun
										if (map->hotspot_enabling_room2[5]) {
											if (cursor.left_click) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1H, 158, 159, 160, false, 100);

											} else {
												UI_ShowDescription(UI_TXT_SCN1H, 17);
											}
										}
										break;
									case 6:// UZI
										if (map->hotspot_enabling_room2[6]) {
											if (cursor.left_click) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1H, 161, 162, 163, false, 100);

											} else {
												UI_ShowDescription(UI_TXT_SCN1H, 18);
											}
										}
										break;
									case 7:// Rifle
										if (map->hotspot_enabling_room2[7]) {
											if (cursor.left_click) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1H, 164, 165, 166, false, 100);

											} else {
												UI_ShowDescription(UI_TXT_SCN1H, 19);
											}
										}
										break;
									case 8:// Ring
										if (map->hotspot_enabling_room2[8]) {
											if (cursor.left_click) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1H, 167, 168, 169, false, 100);

											} else {
												UI_ShowDescription(UI_TXT_SCN1H, 20);
											}
										}
										break;
									case 9:// Spare
										if (map->hotspot_enabling_room2[9]) {
											if (cursor.left_click) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1H, 170, 171, 172, false, 100);

											} else {
												UI_ShowDescription(UI_TXT_SCN1H, 21);
											}
										}
										break;
									case 10:// Spare
										if (map->hotspot_enabling_room2[10]) {
										}
										break;
									case 11:// Spare
										if (map->hotspot_enabling_room2[11]) {
										}
										break;
									case 12:// Spare
										if (map->hotspot_enabling_room2[12]) {
										}
										break;
									case 13:// Spare
										if (map->hotspot_enabling_room2[13]) {
										}
										break;
									case 14:// Spare
										if (map->hotspot_enabling_room2[14]) {
										}
										break;
									case 15:// Spare
										if (map->hotspot_enabling_room2[15]) {
										}
										break;
									case 16:// Spare
										if (map->hotspot_enabling_room2[16]) {
										}
										break;
									default:
										break;
								}
								break;
							case ENTITY_ID_NPC:// NPC
								switch ((cursor.point_on >> 8) & 0xFF) {
									case 0:// No one
										break;
									case 1:// Sara
										if (cursor.left_click) {
											UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1H, 173, 174, 175, false, 100);

										} else {
											UI_ShowDescription(UI_TXT_SCN1H, 22);
										}
										break;
									case 2:// Office droid
										if (cursor.left_click) {
											UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1H, 190, 191, 192, false, 100);

										} else {
											UI_ShowDescription(UI_TXT_SCN1H, 23);
										}
										break;
									case 3:// Agent
									case 4:// Agent
										if (cursor.left_click) {
											if (scene_step == 0) UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1H, 178, 179, 355, false, 100);
											else
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1H, 194, 195, 196, false, 100);

										} else {
											if (scene_step == 0) UI_ShowDescription(UI_TXT_SCN1H, 25);
											else
												UI_ShowDescription(UI_TXT_SCN1H, 26);
										}
										break;
									case 5:// Boss
										if (cursor.left_click) {
											UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1H, 176, 177, 355, false, 100);

										} else {
											UI_ShowDescription(UI_TXT_SCN1H, 24);
										}
										break;
									case 6:// Droid
									case 7:// Droid
									case 8:// Droid
									case 9:// Droid
										if (cursor.left_click) {
											UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1H, 190, 191, 192, false, 100);

										} else {
											UI_ShowDescription(UI_TXT_SCN1H, 23);
										}
										break;
									default:
										break;
								}
								break;
							default:
								break;
						}
					}

					// Events
					actor_tile_num = MAP_GetTileNumber(actor->pos_x + (gfx_sprite_stack[actor->sprite_num].width_px >> 1), actor->pos_y + (gfx_sprite_stack[actor->sprite_num].height_px >> 1));
					switch ((map->layer[MAP_EVENT_HSPOT_LAYER][actor_tile_num] >> 8) & 0xFF) {
						case 1:// Event 1. Go from map 2 to map 1 or end of scene
							if (map->event_enabling_room2[1]) {
								VIDEO_FadeOut(4);

								NPC_UnloadNpcs();
								ENEMY_UnloadEnemies();
								OBJECT_UnloadObjects();
								EFFECT_UnloadEffects();
								BULLET_UnloadBullets();
								PARTICLE_UnloadParticles();
								GRENADE_UnloadGrenades();
								MAP_UnloadMap();

								switch (scene_step) {
									case 0:// go to map 1
										Scene1_LoadRoom1();
										ACTOR_SetPosition(73 << 4, 5 << 4, ACTOR_FACING_DOWN);
										CAM_Init(map->width_px, map->height_px, 60 << 4, 0 << 4);// Initialize camera
										MAP_DrawMapToMapVideoBuffer();

										Update(true);
										Update(true);

										engine.room = 1;

										VIDEO_FadeIn(1);
										break;
									case 1:// end of scene
										VIDEO_FadeOut(4);
										engine.ingame = false;
										engine.scene = 2;
										engine.room = 1;
										break;
									default:

										break;
								}
							}
							break;
						case 2:// Event 18. Bathroom door
							if (map->event_enabling_room2[2]) {
								map->event_enabling_room2[2] = false;

								MAP_HideSwapableTile(15 << 4, 51 << 4);
								MAP_HideSwapableTile(16 << 4, 51 << 4);
								MAP_HideSwapableTile(17 << 4, 51 << 4);

								MAP_HideSwapableTile(15 << 4, 52 << 4);
								MAP_HideSwapableTile(16 << 4, 52 << 4);
								MAP_HideSwapableTile(17 << 4, 52 << 4);

								MAP_HideSwapableTile(15 << 4, 53 << 4);
								MAP_HideSwapableTile(16 << 4, 53 << 4);
								MAP_HideSwapableTile(17 << 4, 53 << 4);
							}
							break;
						case 3:// Event 3. Private area door
							if (map->event_enabling_room2[3]) {
								map->event_enabling_room2[3] = false;

								MAP_HideSwapableTile(21 << 4, 41 << 4);
								MAP_HideSwapableTile(22 << 4, 41 << 4);
								MAP_HideSwapableTile(23 << 4, 41 << 4);

								MAP_HideSwapableTile(21 << 4, 42 << 4);
								MAP_HideSwapableTile(22 << 4, 42 << 4);
								MAP_HideSwapableTile(23 << 4, 42 << 4);

								MAP_HideSwapableTile(21 << 4, 43 << 4);
								MAP_HideSwapableTile(22 << 4, 43 << 4);
								MAP_HideSwapableTile(23 << 4, 43 << 4);
							}
							break;
						case 4:// Event 4. Boss room door
							if (map->event_enabling_room2[4]) {
								map->event_enabling_room2[4] = false;

								MAP_HideSwapableTile(7 << 4, 35 << 4);
								MAP_HideSwapableTile(8 << 4, 35 << 4);
								MAP_HideSwapableTile(9 << 4, 35 << 4);

								MAP_HideSwapableTile(7 << 4, 36 << 4);
								MAP_HideSwapableTile(8 << 4, 36 << 4);
								MAP_HideSwapableTile(9 << 4, 36 << 4);

								MAP_HideSwapableTile(7 << 4, 37 << 4);
								MAP_HideSwapableTile(8 << 4, 37 << 4);
								MAP_HideSwapableTile(9 << 4, 37 << 4);
							}
							break;
						case 5:// Event 5. Practice room door
							if (map->event_enabling_room2[5]) {
								map->event_enabling_room2[5] = false;

								if (tutorial) {
									tutorial = false;
									ENEMY_SetBehavior(7, ENEMY_STATUS_SLEEP);
								}

								MAP_HideSwapableTile(25 << 4, 21 << 4);
								MAP_HideSwapableTile(26 << 4, 21 << 4);
								MAP_HideSwapableTile(27 << 4, 21 << 4);

								MAP_HideSwapableTile(25 << 4, 22 << 4);
								MAP_HideSwapableTile(26 << 4, 22 << 4);
								MAP_HideSwapableTile(27 << 4, 22 << 4);

								MAP_HideSwapableTile(25 << 4, 23 << 4);
								MAP_HideSwapableTile(26 << 4, 23 << 4);
								MAP_HideSwapableTile(27 << 4, 23 << 4);
							}
							break;
						case 6:// Event 6. Practice room pistol
							if (map->event_enabling_room2[6]) {
								map->event_enabling_room2[6] = false;

								if (!OBJECT_IsLoaded(1)) OBJECT_LoadObject("OBJECT3.DAT", 1, ENTITY_ID_TARGET, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, (4 << 4) + 8, 7 << 4);

								sequence_step = 0;
								end_sequence = false;
								while (!end_sequence) {
									switch (sequence_step) {
										case 0://
											UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_DROID2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 252, 253, 254, true, 100);
											sequence_step++;
											break;
										case 1://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_DROID2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 255, 256, 257, true, 100);
												sequence_step++;
											}
											break;
										case 2://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_DROID2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 258, 259, 355, true, 100);
												sequence_step++;
											}
											break;
										case 3://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_DROID2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 263, 264, 265, true, 100);
												sequence_step++;
											}
											break;
										case 4://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 310, 355, 355, false, 100);
												sequence_step++;
											}
											break;
										case 5://
											if (UI_IsSpeechFinished()) {
												end_sequence = true;
											}
											break;
										default:
											end_sequence = true;
											break;
									}
									Update(false);
								}

								// Set pistol
								ACTOR_SetGun(actor->sprite_num, ACTOR_GUN_TYPE_PISTOL);
								ACTOR_SetCombatMode(true);
								ACTOR_SetBulletStatus(99, 0, 0, 0, 0);
								ACTOR_Reload();
							}
							break;
						case 7:// Event 7. Practice room shotgun
							if (map->event_enabling_room2[7]) {
								map->event_enabling_room2[7] = false;

								if (!OBJECT_IsLoaded(2)) OBJECT_LoadObject("OBJECT3.DAT", 2, ENTITY_ID_TARGET, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, (9 << 4) + 8, 9 << 4);

								sequence_step = 0;
								end_sequence = false;
								while (!end_sequence) {
									switch (sequence_step) {
										case 0://
											UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_DROID2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 266, 267, 268, true, 100);
											sequence_step++;
											break;
										case 1://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_DROID2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 269, 270, 355, true, 100);
												sequence_step++;
											}
											break;
										case 2://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_DROID2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 264, 271, 355, true, 100);
												sequence_step++;
											}
											break;
										case 3://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 309, 355, 355, false, 100);
												sequence_step++;
											}
											break;
										case 4://
											if (UI_IsSpeechFinished()) {
												end_sequence = true;
											}
											break;
										default:
											end_sequence = true;
											break;
									}
									Update(false);
								}


								// Set shotgun
								ACTOR_SetGun(actor->sprite_num, ACTOR_GUN_TYPE_SHOTGUN);
								ACTOR_SetCombatMode(true);
								ACTOR_SetBulletStatus(0, 99, 0, 0, 0);
								ACTOR_Reload();
							}
							break;
						case 8:// Event 8. Practice room uzi
							if (map->event_enabling_room2[8]) {
								map->event_enabling_room2[8] = false;

								if (!OBJECT_IsLoaded(3)) OBJECT_LoadObject("OBJECT3.DAT", 3, ENTITY_ID_TARGET, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, (14 << 4) + 8, 7 << 4);

								sequence_step = 0;
								end_sequence = false;
								while (!end_sequence) {
									switch (sequence_step) {
										case 0://
											UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_DROID2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 272, 273, 274, true, 100);
											sequence_step++;
											break;
										case 1://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_DROID2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 275, 276, 277, true, 100);
												sequence_step++;
											}
											break;
										case 2://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_DROID2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 264, 278, 355, true, 100);
												sequence_step++;
											}
											break;
										case 3://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 310, 355, 355, false, 100);
												sequence_step++;
											}
											break;
										case 4://
											if (UI_IsSpeechFinished()) {
												end_sequence = true;
											}
											break;
										default:
											end_sequence = true;
											break;
									}
									Update(false);
								}

								// Set uzi
								ACTOR_SetGun(actor->sprite_num, ACTOR_GUN_TYPE_UZI);
								ACTOR_SetCombatMode(true);
								ACTOR_SetBulletStatus(0, 0, 999, 0, 0);
								ACTOR_Reload();
							}
							break;
						case 9:// Event 9. Practice room rifle
							if (map->event_enabling_room2[9]) {
								map->event_enabling_room2[9] = false;

								if (!OBJECT_IsLoaded(4)) OBJECT_LoadObject("OBJECT3.DAT", 4, ENTITY_ID_TARGET, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, (19 << 4) + 8, 6 << 4);

								sequence_step = 0;
								end_sequence = false;
								while (!end_sequence) {
									switch (sequence_step) {
										case 0://
											UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_DROID2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 279, 280, 281, true, 100);
											sequence_step++;
											break;
										case 1://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_DROID2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 282, 283, 284, true, 100);
												sequence_step++;
											}
											break;
										case 2://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_DROID2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 285, 286, 287, true, 100);
												sequence_step++;
											}
											break;
										case 3://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_DROID2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 264, 288, 355, true, 100);
												sequence_step++;
											}
											break;
										case 4://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 309, 355, 355, false, 100);
												sequence_step++;
											}
											break;
										case 5://
											if (UI_IsSpeechFinished()) {
												end_sequence = true;
											}
											break;
										default:
											end_sequence = true;
											break;
									}
									Update(false);
								}

								// Set snipper rifle
								ACTOR_SetGun(actor->sprite_num, ACTOR_GUN_TYPE_SNIPPER);
								ACTOR_SetCombatMode(true);
								ACTOR_SetBulletStatus(0, 0, 0, 99, 0);
								ACTOR_Reload();
							}
							break;
						case 10:// Event 10. Practice room ring
							if (map->event_enabling_room2[10]) {
								map->event_enabling_room2[10] = false;

								sequence_step = 0;
								end_sequence = false;
								while (!end_sequence) {
									switch (sequence_step) {
										case 0://
											UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_DROID2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 289, 290, 291, true, 100);
											sequence_step++;
											break;
										case 1://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_DROID2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 292, 293, 355, true, 100);
												sequence_step++;
											}
											break;
										case 2://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_DROID2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 294, 295, 296, true, 100);
												sequence_step++;
											}
											break;
										case 3://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_DROID2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 297, 298, 355, true, 100);
												sequence_step++;
											}
											break;
										case 4://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_DROID2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 299, 300, 301, true, 100);
												sequence_step++;
											}
											break;
										case 5://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_DROID2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 302, 303, 355, true, 100);
												sequence_step++;
											}
											break;
										case 6://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_DROID2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 304, 305, 355, true, 100);
												sequence_step++;
											}
											break;
										case 7://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_DROID2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 306, 307, 308, true, 100);
												sequence_step++;
											}
											break;
										case 8://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_DROID2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 332, 333, 334, true, 100);
												sequence_step++;
											}
											break;
										case 9://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_DROID2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 335, 336, 337, true, 100);
												sequence_step++;
											}
											break;
										case 10://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 311, 355, 355, false, 100);
												sequence_step++;
											}
											break;
										case 11://
											if (UI_IsSpeechFinished()) {
												end_sequence = true;
											}
											break;
										default:
											end_sequence = true;
											break;
									}
									Update(false);
								}

								ENEMY_SetBehavior(4, ENEMY_STATUS_CHASE);

								// Set pistol
								ACTOR_SetGun(actor->sprite_num, ACTOR_GUN_TYPE_BARE_HANDS);
								ACTOR_SetCombatMode(true);
								ACTOR_SetBulletStatus(0, 0, 0, 0, 0);
							}
							break;
						case 11:// Event 11. Sara conversation 1
							if (map->event_enabling_room2[11]) {
								map->event_enabling_room2[11] = false;

								sequence_step = 0;
								end_sequence = false;
								while (!end_sequence) {
									switch (sequence_step) {
										case 0://
											UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_SARA_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 80, 355, 355, true, 100);
											sequence_step++;
											break;
										case 1://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 81, 355, 355, false, 100);
												sequence_step++;
											}
											break;
										case 2://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_SARA_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 82, 355, 355, true, 100);
												sequence_step++;
											}
											break;
										case 3://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 83, 355, 355, false, 100);
												sequence_step++;
											}
											break;
										case 4://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_SARA_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 84, 85, 86, true, 100);
												sequence_step++;
											}
											break;
										case 5://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 87, 88, 355, false, 100);
												sequence_step++;
											}
											break;
										case 6://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 89, 90, 91, false, 100);
												sequence_step++;
											}
											break;
										case 7://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_SARA_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 92, 93, 355, true, 100);
												sequence_step++;
											}
											break;
										case 8://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_SARA_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 94, 95, 96, true, 100);
												sequence_step++;
											}
											break;
										case 9://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_SARA_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 97, 98, 355, true, 100);
												sequence_step++;
											}
											break;
										case 10://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 99, 100, 101, false, 100);
												sequence_step++;
											}
											break;
										case 11://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_SARA_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 102, 103, 355, true, 100);
												sequence_step++;
											}
											break;
										case 12://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_SARA_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 104, 105, 355, true, 100);
												sequence_step++;
											}
											break;
										case 13://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_SARA_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 106, 107, 108, true, 100);
												sequence_step++;
											}
											break;
										case 14://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_SARA_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 109, 355, 355, true, 100);
												sequence_step++;
											}
											break;
										case 15://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 110, 111, 112, false, 100);
												sequence_step++;
											}
											break;
										case 16://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 113, 114, 115, false, 100);
												sequence_step++;
											}
											break;
										case 17://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_SARA_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 116, 117, 355, true, 100);
												sequence_step++;
											}
											break;
										case 18://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_SARA_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 118, 119, 120, true, 100);
												sequence_step++;
											}
											break;
										case 19://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_SARA_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 121, 122, 123, true, 100);
												sequence_step++;
											}
											break;
										case 20://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 124, 125, 126, false, 100);
												sequence_step++;
											}
											break;
										case 21://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 127, 128, 129, false, 100);
												sequence_step++;
											}
											break;
										case 22://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_SARA_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 130, 131, 355, true, 100);
												sequence_step++;
											}
											break;
										case 23://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_SARA_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 132, 133, 134, true, 100);
												sequence_step++;
											}
											break;
										case 24://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 135, 136, 137, false, 100);
												sequence_step++;
											}
											break;
										case 25://
											if (UI_IsSpeechFinished()) {
												end_sequence = true;
											}
											break;
										default:
											end_sequence = true;
											break;
									}
									Update(false);
								}
							}
							break;
						case 12:// Event 12. Boss room
							if (map->event_enabling_room2[12]) {
								map->event_enabling_room2[12] = false;

								sequence_step = 0;
								end_sequence = false;
								while (!end_sequence) {
									switch (sequence_step) {
										case 0://
											UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 140, 141, 142, false, 100);
											sequence_step++;
											break;
										case 1://
											if (UI_IsSpeechFinished()) {
												sequence_step++;
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 143, 144, 145, false, 100);
											}
											break;
										case 2://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_BOSS_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 146, 147, 148, true, 100);
												sequence_step++;
											}
											break;
										case 3://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 149, 355, 355, false, 100);
												sequence_step++;
											}
											break;
										case 4://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_BOSS_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 150, 151, 152, true, 100);
												sequence_step++;
											}
											break;
										case 5://
											if (UI_IsSpeechFinished()) {
												end_sequence = true;
											}
											break;
										default:
											end_sequence = true;
											break;
									}
									Update(false);
								}
								end_sequence = false;
							}
							break;
						case 13:// Event 13. Droid conversation
							if (map->event_enabling_room2[13]) {
								map->event_enabling_room2[13] = false;
								sequence_step = 0;
								end_sequence = false;
								while (!end_sequence) {
									switch (sequence_step) {
										case 0://
											UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_DROID1_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 65, 66, 67, true, 100);
											sequence_step++;
											break;
										case 1://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_DROID1_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 68, 355, 355, true, 100);
												sequence_step++;
											}
											break;
										case 2://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 69, 70, 71, false, 100);
												sequence_step++;
											}
											break;
										case 3://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_DROID1_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 72, 73, 74, true, 100);
												sequence_step++;
											}
											break;
										case 4://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_DROID1_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 75, 76, 355, true, 100);
												sequence_step++;
											}
											break;
										case 5://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 77, 355, 355, false, 100);
												sequence_step++;
											}
											break;
										case 6://
											if (UI_IsSpeechFinished()) {
												end_sequence = true;
											}
											break;
										default:
											end_sequence = true;
											break;
									}
									Update(false);
								}
								end_sequence = false;
							}
							break;
						case 14:// Event 14. Bathroom droid conversation
							if (map->event_enabling_room2[14]) {
								map->event_enabling_room2[14] = false;

								sequence_step = 0;
								end_sequence = false;
								while (!end_sequence) {
									switch (sequence_step) {
										case 0://
											UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_DROID1_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 312, 313, 314, true, 100);
											sequence_step++;
											break;
										case 1://
											if (UI_IsSpeechFinished()) {
												end_sequence = true;
											}
											break;
										default:
											end_sequence = true;
											break;
									}
									Update(false);
								}
								end_sequence = false;
							}
							break;
						case 15:// Event 15. Enemy in jail 1
							if (map->event_enabling_room2[15]) {
								map->event_enabling_room2[15] = false;

								sequence_step = 0;
								end_sequence = false;
								while (!end_sequence) {
									switch (sequence_step) {
										case 0://
											UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 315, 316, 355, true, 100);
											sequence_step++;
											break;
										case 1://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 317, 318, 319, true, 100);
												sequence_step++;
											}
											break;
										case 2://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 320, 321, 355, false, 100);
												sequence_step++;
											}
											break;
										case 3://
											if (UI_IsSpeechFinished()) {
												end_sequence = true;
											}
											break;
										default:
											end_sequence = true;
											break;
									}
									Update(false);
								}
								end_sequence = false;
							}
							break;
						case 16:// Event 16. Enemy in jail 2
							if (map->event_enabling_room2[16]) {
								map->event_enabling_room2[16] = false;

								sequence_step = 0;
								end_sequence = false;
								while (!end_sequence) {
									switch (sequence_step) {
										case 0://
											UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 322, 323, 324, true, 100);
											sequence_step++;
											break;
										case 1://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 325, 326, 327, true, 100);
												sequence_step++;
											}
											break;
										case 2://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 328, 329, 330, false, 100);
												sequence_step++;
											}
											break;
										case 3://
											if (UI_IsSpeechFinished()) {
												end_sequence = true;
											}
											break;
										default:
											end_sequence = true;
											break;
									}
									Update(false);
								}
								end_sequence = false;
							}
							break;
						case 17:// Event 17. Sara conversation 2
							if (map->event_enabling_room2[17]) {
								map->event_enabling_room2[17] = false;
							}
							break;
						case 18:// Event 18. Agent conversation
							if (map->event_enabling_room2[18]) {
								map->event_enabling_room2[18] = false;

								sequence_step = 0;
								end_sequence = false;
								while (!end_sequence) {
									switch (sequence_step) {
										case 0://
											UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_BOSS_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 153, 154, 355, true, 100);
											sequence_step++;
											break;
										case 1://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_BOSS_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 213, 214, 355, true, 100);
												sequence_step++;
											}
											break;
										case 2://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 156, 157, 158, false, 100);
												sequence_step++;
											}
											break;
										case 3://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_AGENT2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 159, 160, 161, true, 100);
												sequence_step++;
											}
											break;
										case 4://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 162, 163, 355, false, 100);
												sequence_step++;
											}
											break;
										case 5://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_AGENT1_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 164, 165, 355, true, 100);
												sequence_step++;
											}
											break;
										case 6://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_AGENT1_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 166, 167, 355, true, 100);
												sequence_step++;
											}
											break;
										case 7://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 168, 355, 355, false, 100);
												sequence_step++;
											}
											break;
										case 8://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_AGENT2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 169, 170, 355, true, 100);
												sequence_step++;
											}
											break;
										case 9://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_AGENT2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 171, 172, 173, true, 100);
												sequence_step++;
											}
											break;
										case 10://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_AGENT2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 174, 175, 355, true, 100);
												sequence_step++;
											}
											break;
										case 11://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 176, 177, 355, false, 100);
												sequence_step++;
											}
											break;
										case 12://
											if (UI_IsSpeechFinished()) {
												sequence_step++;
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_AGENT1_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 178, 179, 180, true, 100);
											}
											break;
										case 13://
											if (UI_IsSpeechFinished()) {
												sequence_step++;
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_AGENT1_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 181, 182, 355, true, 100);
											}
											break;
										case 14://
											if (UI_IsSpeechFinished()) {
												sequence_step++;
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_AGENT1_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 183, 184, 355, true, 100);
											}
											break;
										case 15://
											if (UI_IsSpeechFinished()) {
												sequence_step++;
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_AGENT1_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 185, 186, 187, true, 100);
											}
											break;
										case 16://
											if (UI_IsSpeechFinished()) {
												sequence_step++;
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 188, 189, 190, false, 100);
											}
											break;
										case 17://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_AGENT2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 191, 355, 355, true, 100);
												sequence_step++;
											}
											break;
										case 18://
											if (UI_IsSpeechFinished()) {
												sequence_step++;
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 192, 355, 355, false, 100);
											}
											break;
										case 19://
											if (UI_IsSpeechFinished()) {
												sequence_step++;
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_AGENT1_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 193, 194, 195, true, 100);
											}
											break;
										case 20://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 196, 197, 198, false, 100);
												sequence_step++;
											}
											break;
										case 21://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_AGENT1_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 199, 200, 201, true, 100);
												sequence_step++;
											}
											break;
										case 22://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_AGENT2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 202, 203, 204, true, 100);
												sequence_step++;
											}
											break;
										case 23://
											if (UI_IsSpeechFinished()) sequence_step++;
											break;
										case 24:// Fade out
											VIDEO_FadeOut(4);
											NPC_UnloadNpc(3);
											NPC_UnloadNpc(4);
											Update(false);
											Update(false);
											VIDEO_FadeIn(4);
											sequence_step++;
											break;
										case 25://
											UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 205, 206, 355, false, 100);
											sequence_step++;
											break;
										case 26://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_BOSS_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 207, 208, 209, true, 100);
												sequence_step++;
											}
											break;
										case 27://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_BOSS_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 210, 211, 212, true, 100);
												sequence_step++;
											}
											break;
										case 28://
											if (UI_IsSpeechFinished()) {
												end_sequence = true;
											}
											break;
										default:
											end_sequence = true;
											break;
									}
									Update(false);
								}
								end_sequence = false;

								scene_step = 1;

								NPC_UnloadNpc(1);
							}
							break;
						case 19:// Event 19. Tutorial start
							if (map->event_enabling_room2[19]) {
								map->event_enabling_room2[19] = false;

								tutorial = true;
								sequence_step = 0;
								end_sequence = false;
								while (!end_sequence) {
									switch (sequence_step) {
										case 0://
											UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_DROID2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 235, 236, 237, true, 100);
											sequence_step++;
											break;
										case 1://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_DROID2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 238, 239, 240, true, 100);
												sequence_step++;
											}
											break;
										case 2://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 241, 242, 243, false, 100);
												sequence_step++;
											}
											break;
										case 4://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_DROID2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 244, 355, 355, true, 100);
												sequence_step++;
											}
											break;
										case 5://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_DROID2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 245, 246, 247, true, 100);
												sequence_step++;
											}
											break;
										case 6://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_NPC_DROID2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 248, 249, 250, true, 100);
												sequence_step++;
											}
											break;
										case 7://
											if (UI_IsSpeechFinished()) {
												UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN1D, 251, 355, 355, false, 100);
												sequence_step++;
											}
											break;
										case 8://
											if (UI_IsSpeechFinished()) {
												end_sequence = true;
											}
											break;
										default:
											end_sequence = true;
											break;
									}
									Update(false);
								}

								NPC_SetBehavior(8, NPC_STATUS_CHASE);
							}
							break;
						case 20:// Event 20. Entering
							if (map->event_enabling_room2[20]) {
								map->event_enabling_room2[20] = false;
							}
							break;
						default:
							break;
					}
				}

				// Close bathroom door
				if (((map->layer[MAP_EVENT_HSPOT_LAYER][actor_tile_num] >> 8) & 0xFF) != 2) {
					if (!map->event_enabling_room2[2]) {
						map->event_enabling_room2[2] = true;

						MAP_ShowSwapableTile(15 << 4, 51 << 4);
						MAP_ShowSwapableTile(16 << 4, 51 << 4);
						MAP_ShowSwapableTile(17 << 4, 51 << 4);

						MAP_ShowSwapableTile(15 << 4, 52 << 4);
						MAP_ShowSwapableTile(16 << 4, 52 << 4);
						MAP_ShowSwapableTile(17 << 4, 52 << 4);

						MAP_ShowSwapableTile(15 << 4, 53 << 4);
						MAP_ShowSwapableTile(16 << 4, 53 << 4);
						MAP_ShowSwapableTile(17 << 4, 53 << 4);
					}
				}

				// Close private area door
				if (((map->layer[MAP_EVENT_HSPOT_LAYER][actor_tile_num] >> 8) & 0xFF) != 3) {
					if (!map->event_enabling_room2[3]) {
						map->event_enabling_room2[3] = true;

						MAP_ShowSwapableTile(21 << 4, 41 << 4);
						MAP_ShowSwapableTile(22 << 4, 41 << 4);
						MAP_ShowSwapableTile(23 << 4, 41 << 4);

						MAP_ShowSwapableTile(21 << 4, 42 << 4);
						MAP_ShowSwapableTile(22 << 4, 42 << 4);
						MAP_ShowSwapableTile(23 << 4, 42 << 4);

						MAP_ShowSwapableTile(21 << 4, 43 << 4);
						MAP_ShowSwapableTile(22 << 4, 43 << 4);
						MAP_ShowSwapableTile(23 << 4, 43 << 4);
					}
				}

				// Close boss room door
				if (((map->layer[MAP_EVENT_HSPOT_LAYER][actor_tile_num] >> 8) & 0xFF) != 4) {
					if (!map->event_enabling_room2[4]) {
						map->event_enabling_room2[4] = true;

						MAP_ShowSwapableTile(7 << 4, 35 << 4);
						MAP_ShowSwapableTile(8 << 4, 35 << 4);
						MAP_ShowSwapableTile(9 << 4, 35 << 4);

						MAP_ShowSwapableTile(7 << 4, 36 << 4);
						MAP_ShowSwapableTile(8 << 4, 36 << 4);
						MAP_ShowSwapableTile(9 << 4, 36 << 4);

						MAP_ShowSwapableTile(7 << 4, 37 << 4);
						MAP_ShowSwapableTile(8 << 4, 37 << 4);
						MAP_ShowSwapableTile(9 << 4, 37 << 4);
					}
				}

				// Close practice room door
				if (((map->layer[MAP_EVENT_HSPOT_LAYER][actor_tile_num] >> 8) & 0xFF) != 5) {
					if (!map->event_enabling_room2[5]) {
						map->event_enabling_room2[5] = true;

						MAP_ShowSwapableTile(25 << 4, 21 << 4);
						MAP_ShowSwapableTile(26 << 4, 21 << 4);
						MAP_ShowSwapableTile(27 << 4, 21 << 4);

						MAP_ShowSwapableTile(25 << 4, 22 << 4);
						MAP_ShowSwapableTile(26 << 4, 22 << 4);
						MAP_ShowSwapableTile(27 << 4, 22 << 4);

						MAP_ShowSwapableTile(25 << 4, 23 << 4);
						MAP_ShowSwapableTile(26 << 4, 23 << 4);
						MAP_ShowSwapableTile(27 << 4, 23 << 4);
					}
				}

				// Leave practice pistol area
				if (((map->layer[MAP_EVENT_HSPOT_LAYER][actor_tile_num] >> 8) & 0xFF) != 6) {
					if (!map->event_enabling_room2[6]) {
						map->event_enabling_room2[6] = true;

						OBJECT_UnloadObject(1);

						// Set bare hands
						ACTOR_SetGun(actor->sprite_num, ACTOR_GUN_TYPE_BARE_HANDS);
						ACTOR_SetCombatMode(false);
						ACTOR_SetBulletStatus(99, 0, 0, 0, 0);
					}
				}

				// Leave practice shotgun area
				if (((map->layer[MAP_EVENT_HSPOT_LAYER][actor_tile_num] >> 8) & 0xFF) != 7) {
					if (!map->event_enabling_room2[7]) {
						map->event_enabling_room2[7] = true;

						OBJECT_UnloadObject(2);

						// Set bare hands
						ACTOR_SetGun(actor->sprite_num, ACTOR_GUN_TYPE_BARE_HANDS);
						ACTOR_SetCombatMode(false);
						ACTOR_SetBulletStatus(99, 0, 0, 0, 0);
					}
				}

				// Leave practice uzi area
				if (((map->layer[MAP_EVENT_HSPOT_LAYER][actor_tile_num] >> 8) & 0xFF) != 8) {
					if (!map->event_enabling_room2[8]) {
						map->event_enabling_room2[8] = true;

						OBJECT_UnloadObject(3);

						// Set bare hands
						ACTOR_SetGun(actor->sprite_num, ACTOR_GUN_TYPE_BARE_HANDS);
						ACTOR_SetCombatMode(false);
						ACTOR_SetBulletStatus(99, 0, 0, 0, 0);
					}
				}

				// Leave practice rifle area
				if (((map->layer[MAP_EVENT_HSPOT_LAYER][actor_tile_num] >> 8) & 0xFF) != 9) {
					if (!map->event_enabling_room2[9]) {
						map->event_enabling_room2[9] = true;

						OBJECT_UnloadObject(4);

						// Set bare hands
						ACTOR_SetGun(actor->sprite_num, ACTOR_GUN_TYPE_BARE_HANDS);
						ACTOR_SetCombatMode(false);
						ACTOR_SetBulletStatus(99, 0, 0, 0, 0);
					}
				}

				// Leave practice fight area
				if (((map->layer[MAP_EVENT_HSPOT_LAYER][actor_tile_num] >> 8) & 0xFF) != 10) {
					if (!map->event_enabling_room2[10]) {
						map->event_enabling_room2[10] = true;

						ENEMY_SetBehavior(4, ENEMY_STATUS_STATIC_NPC);

						// Set bare hands
						ACTOR_SetGun(actor->sprite_num, ACTOR_GUN_TYPE_BARE_HANDS);
						ACTOR_SetCombatMode(false);
						ACTOR_SetBulletStatus(99, 0, 0, 0, 0);
					}
				}


				break;
			default:
				break;
		}

		Update(!actor->action_dead);

		// Finish all if actor is dead
		if (actor->status_dead) Scene1_GameOver();
	}

	VIDEO_FadeOut(4);
	VIDEO_ClearScreen();
	AUDIO_StopSong();
	ENEMY_UnloadEnemies();
	OBJECT_UnloadObjects();
	EFFECT_UnloadEffects();
	BULLET_UnloadBullets();
	PARTICLE_UnloadParticles();
	GRENADE_UnloadGrenades();
	ITEM_UnloadItems();
	NPC_UnloadNpcs();
	ACTOR_UnloadActor();
	MAP_UnloadMap();
	Scene1_UnloadAssets();
}
void Scene1_Outro(void) {
	int step;
	int i, horizontal_scroll, max_scroll, src_index, dst_index;
	int chat_spr_num;
	bool scroll_end;

	engine.sequence = true;
	step = 0;
	while (engine.sequence) {
		switch (step) {
			case 0:// Draw loading screen
				MOUSE_HideCursor();
				UI_ShowLoadingScreen();
				step++;
				break;
			case 1:// Load first outro scene
				AUDIO_LoadSong(AUDIO_SONG_3);
				GFX_PCXImageToBuffer("OSCN1.DAT", "OUTRO1H.PCX", 266 * 165, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("OSCN1.DAT", "OUTRO2H.PCX", 140 * 140, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				GFX_PCXImageToBuffer("OSCN1.DAT", "OUTRO3H.PCX", 140 * 100, gfx->image_buffer3, &gfx->image_buffer3_width, &gfx->image_buffer3_height);
				step++;
				break;
			case 2:// Hide loading screen
				UI_HideLoadingScreen();
				step++;
				break;
			case 3:/// Scene 1 :: Agents waiting outside
				VIDEO_ClearScreenBuffer();
				GFX_LoadPalette("PALETTES.DAT", "OSCN1.PCX", 256);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 272, 171, 13, 11, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 16, 14);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				VIDEO_FadeIn(4);

				TIMER_UpdateTimerTime(TIMER_AUDIO_NUMBER, 50);
				AUDIO_PlaySong(true);

				step++;
				break;
			case 4://Show chat case and spech
				chat_spr_num = GFX_FindEmptySpriteSlot();
				if (chat_spr_num == -1) {
					sprintf(engine.system_error_message1, "Scene 1 Intro function error");
					sprintf(engine.system_error_message2, "No empty sprite slot available");
					sprintf(engine.system_error_message3, " ");
					Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
				} else {
					GFX_InitSprite(ENTITY_ID_EMPTY, 0, chat_spr_num, 0, 65, 30);
				}
				GFX_SetSpriteGraphic(chat_spr_num, 0, SPRITE_GRAPHICS_ID_CHAT_BIG, 0, 0);
				GFX_SetDefaultAnimation(chat_spr_num, false, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 110, 50);
				step++;
				break;
			case 5://Sara speech
				VIDEO_StringToScreenBuffer(116, 60, ui->txt_file[UI_TXT_SCN1I]->line[35], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(116, 75, ui->txt_file[UI_TXT_SCN1I]->line[36], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				GFX_HideSprite(chat_spr_num);
				step++;
				break;
			case 6:// Just wait
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 272, 171, 13, 11, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 16, 14);
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 7:// actor Speech
				GFX_SetSpriteGraphic(chat_spr_num, 0, SPRITE_GRAPHICS_ID_CHAT_BIG, 0, 0);
				GFX_SetDefaultAnimation(chat_spr_num, true, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 30, 5);
				GFX_ShowSprite(chat_spr_num);
				step++;
				break;
			case 8://Actor speech
				VIDEO_StringToScreenBuffer(36, 15, ui->txt_file[UI_TXT_SCN1I]->line[37], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(36, 30, ui->txt_file[UI_TXT_SCN1I]->line[38], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				GFX_HideSprite(chat_spr_num);
				step++;
				break;
			case 9:// Just wait
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 272, 171, 13, 11, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 16, 14);

				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 10:// sara Speech
				GFX_SetSpriteGraphic(chat_spr_num, 0, SPRITE_GRAPHICS_ID_CHAT_BIG, 0, 0);
				GFX_SetDefaultAnimation(chat_spr_num, false, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 110, 50);
				GFX_ShowSprite(chat_spr_num);
				step++;
				break;
			case 11://Sara speech
				VIDEO_StringToScreenBuffer(116, 60, ui->txt_file[UI_TXT_SCN1I]->line[39], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(116, 75, ui->txt_file[UI_TXT_SCN1I]->line[40], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				GFX_HideSprite(chat_spr_num);
				step++;
				break;
			case 12:// Just wait
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 272, 171, 13, 11, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 16, 14);

				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 13:// actor Speech
				GFX_SetSpriteGraphic(chat_spr_num, 0, SPRITE_GRAPHICS_ID_CHAT_BIG, 0, 0);
				GFX_SetDefaultAnimation(chat_spr_num, true, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 30, 5);
				GFX_ShowSprite(chat_spr_num);
				step++;
				break;
			case 14://Actor speech
				VIDEO_StringToScreenBuffer(36, 25, ui->txt_file[UI_TXT_SCN1I]->line[41], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(3000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				GFX_HideSprite(chat_spr_num);
				step++;
				break;
			case 15:// Clear screen and show image 2
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 146, 30, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 13);
				step++;
				break;
			case 16:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 17:// Sara Speech
				GFX_SetSpriteGraphic(chat_spr_num, 0, SPRITE_GRAPHICS_ID_CHAT_BIG, 0, 0);
				GFX_SetDefaultAnimation(chat_spr_num, false, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 60, 50);
				GFX_ShowSprite(chat_spr_num);
				step++;
				break;
			case 18://Sara speech
				VIDEO_StringToScreenBuffer(66, 60, ui->txt_file[UI_TXT_SCN1I]->line[43], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(66, 75, ui->txt_file[UI_TXT_SCN1I]->line[44], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				GFX_HideSprite(chat_spr_num);
				step++;
				break;
			case 19:// Show images 2 and 3
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 146, 30, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 13);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 106, 160, 60, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 163, 63);
				step++;
				break;
			case 20:// Just wait
				SetDelayTime(2000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				GFX_ShowSprite(chat_spr_num);
				step++;
				break;
			case 21://Sara speech
				VIDEO_StringToScreenBuffer(66, 60, ui->txt_file[UI_TXT_SCN1I]->line[45], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(66, 75, ui->txt_file[UI_TXT_SCN1I]->line[46], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 22:// Just wait
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 146, 30, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 13);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 106, 160, 60, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 163, 63);

				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 23://Sara speech
				VIDEO_StringToScreenBuffer(66, 60, ui->txt_file[UI_TXT_SCN1I]->line[47], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(66, 75, ui->txt_file[UI_TXT_SCN1I]->line[48], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				GFX_HideSprite(chat_spr_num);
				step++;
				break;
			case 24:// Just wait
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 146, 30, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 13);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 106, 160, 60, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 163, 63);

				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 25:// actor Speech
				GFX_SetSpriteGraphic(chat_spr_num, 0, SPRITE_GRAPHICS_ID_CHAT_BIG, 0, 0);
				GFX_SetDefaultAnimation(chat_spr_num, false, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 150, 10);
				GFX_ShowSprite(chat_spr_num);
				step++;
				break;
			case 26://Actor speech
				VIDEO_StringToScreenBuffer(156, 20, ui->txt_file[UI_TXT_SCN1I]->line[50], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(156, 35, ui->txt_file[UI_TXT_SCN1I]->line[51], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 27:// Just wait
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 146, 30, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 13);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 106, 160, 60, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 163, 63);

				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 28://Actor speech
				VIDEO_StringToScreenBuffer(156, 20, ui->txt_file[UI_TXT_SCN1I]->line[52], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(156, 35, ui->txt_file[UI_TXT_SCN1I]->line[53], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 29:// Just wait
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 146, 30, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 13);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 106, 160, 60, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 163, 63);

				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 30://Actor speech
				VIDEO_StringToScreenBuffer(156, 30, ui->txt_file[UI_TXT_SCN1I]->line[54], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(3000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				GFX_HideSprite(chat_spr_num);
				step++;
				break;
			case 31:// Just wait
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 146, 30, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 13);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 106, 160, 60, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 163, 63);

				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 32://Sara Speech
				GFX_SetSpriteGraphic(chat_spr_num, 0, SPRITE_GRAPHICS_ID_CHAT_BIG, 0, 0);
				GFX_SetDefaultAnimation(chat_spr_num, false, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 60, 50);
				GFX_ShowSprite(chat_spr_num);
				step++;
				break;
			case 33://Sara speech
				VIDEO_StringToScreenBuffer(66, 70, ui->txt_file[UI_TXT_SCN1I]->line[56], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(3000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				GFX_HideSprite(chat_spr_num);
				step++;
				break;
			case 34:// Just wait
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 146, 30, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 13);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 106, 160, 60, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 163, 63);
				step++;
				break;
			case 35:// actor Speech
				GFX_SetSpriteGraphic(chat_spr_num, 0, SPRITE_GRAPHICS_ID_CHAT_BIG, 0, 0);
				GFX_SetDefaultAnimation(chat_spr_num, false, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 150, 10);
				GFX_ShowSprite(chat_spr_num);
				step++;
				break;
			case 36://Actor speech
				VIDEO_StringToScreenBuffer(156, 30, ui->txt_file[UI_TXT_SCN1I]->line[58], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(3000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				GFX_HideSprite(chat_spr_num);
				step++;
				break;
			case 37:// Fade out
				VIDEO_FadeOut(4);
				step++;
				break;
			case 38:// Just wait
				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 39:// Load image 4
				VIDEO_ClearScreenBuffer();
				GFX_PCXImageToBuffer("OSCN1.DAT", "OUTRO4H.PCX", 266 * 165, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				step++;
				break;
			case 40:// Just wait
				VIDEO_FadeIn(4);
				step++;
				break;
			case 41://Show chat case
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 272, 171, 20, 8, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 23, 11);

				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 30, 141, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 32, 143, 208);
				step++;
				break;
			case 42:// Speech
				VIDEO_StringToScreenBuffer(35, 148, ui->txt_file[UI_TXT_SCN1I]->line[15], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(35, 158, ui->txt_file[UI_TXT_SCN1I]->line[16], FONT_SLIM_BLACK);
				step++;
				break;
			case 43:// Just wait
				GFX_PCXImageToBuffer("OSCN1.DAT", "OUTRO5H.PCX", 300 * 80, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("OSCN1.DAT", "OUTRO5L.PCX", 300 * 80, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				GFX_PCXImageToBuffer("OSCN1.DAT", "OUTRO6H.PCX", 100 * 100, gfx->image_buffer3, &gfx->image_buffer3_width, &gfx->image_buffer3_height);
				SetDelayTime(2000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				VIDEO_FadeOut(4);
				step++;
				break;
			case 44:// On the way
				VIDEO_ClearScreen();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 304, 84, 8, 8, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 10, 10);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				VIDEO_FadeIn(4);
				step++;
				break;
			case 45:// Just wait
				SetDelayTime(2000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 46:// Show chat case
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 60, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 62, 208);
				step++;
				break;
			case 47:// Speech
				VIDEO_StringToScreenBuffer(28, 64, ui->txt_file[UI_TXT_SCN1I]->line[17], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(28, 74, ui->txt_file[UI_TXT_SCN1I]->line[18], FONT_SLIM_BLACK);
				step++;
				break;
			case 48:// Just wait
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 49:// Hide chat box
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 10, 10);
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 50:// Show second image
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 104, 104, 23, 60, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 25, 62);
				step++;
				break;
			case 51:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 52:// Hide first image and show again second
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 10, 10);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 104, 104, 23, 60, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 25, 62);
				step++;
				break;
			case 53:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 54:// Show chat case
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 168, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 170, 208);
				step++;
				break;
			case 55:// Speech
				VIDEO_StringToScreenBuffer(28, 172, ui->txt_file[UI_TXT_SCN1I]->line[19], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(28, 182, ui->txt_file[UI_TXT_SCN1I]->line[20], FONT_SLIM_BLACK);
				step++;
				break;
			case 56:// Just wait
				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 57:
				GFX_PCXImageToBuffer("OSCN1.DAT", "OUTRO6L.PCX", 100 * 100, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("OSCN1.DAT", "OUTRO7H.PCX", 100 * 100, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 58:// Show third image image
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 104, 104, 163, 60, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 165, 62);
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 59:// Hide second image
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 25, 62);
				step++;
				break;
			case 60:// Show chat case
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 168, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 170, 208);
				step++;
				break;
			case 61:// Speech
				VIDEO_StringToScreenBuffer(28, 172, ui->txt_file[UI_TXT_SCN1I]->line[21], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(28, 182, ui->txt_file[UI_TXT_SCN1I]->line[22], FONT_SLIM_BLACK);
				step++;
				break;
			case 62:// Wait
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 63:// Show chat case
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 168, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 170, 208);
				step++;
				break;
			case 64:// Speech
				VIDEO_StringToScreenBuffer(28, 172, ui->txt_file[UI_TXT_SCN1I]->line[23], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(28, 182, ui->txt_file[UI_TXT_SCN1I]->line[24], FONT_SLIM_BLACK);
				step++;
				break;
			case 65:// Wait
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 66:// Show chat case
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 168, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 170, 208);
				step++;
				break;
			case 67:// Speech
				VIDEO_StringToScreenBuffer(28, 172, ui->txt_file[UI_TXT_SCN1I]->line[25], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(28, 182, ui->txt_file[UI_TXT_SCN1I]->line[26], FONT_SLIM_BLACK);
				step++;
				break;
			case 68:// Wait
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 69:
				VIDEO_FadeOut(4);
				step++;
				break;
			case 70:
				VIDEO_ClearScreen();
				step++;
				break;
			case 71:/// Draw square
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 85, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 87, 208);
				VIDEO_FadeIn(4);
				step++;
				break;
			case 72:
				VIDEO_StringToScreenBuffer(28, 95, ui->txt_file[UI_TXT_SCN1I]->line[28], FONT_SLIM_BLACK);
				step++;
				break;
			case 73:// Wait
				GFX_PCXImageToBuffer("OSCN1.DAT", "OUTRO8H.PCX", 320 * 140, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("OSCN1.DAT", "OUTRO9H.PCX", 120 * 140, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				GFX_PCXImageToBuffer("OSCN1.DAT", "OUTRO8L.PCX", 120 * 140, gfx->image_buffer3, &gfx->image_buffer3_width, &gfx->image_buffer3_height);
				step++;
				break;
			case 74:// Load hangar
				VIDEO_ClearScreen();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 124, 144, 10, 50, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, 120, 140, 12, 52);
				horizontal_scroll = 0;
				max_scroll = 200;
				step++;
				break;
			case 75:// Chat
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 10, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 12, 208);
				step++;
				break;
			case 76:// Scroll hangar
				src_index = horizontal_scroll;
				dst_index = 52 * CAM_VISIBLE_WIDTH + 12;
				for (i = 0; i < gfx->image_buffer1_height; i++) {
					memcpy(&video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index], &gfx->image_buffer1[src_index], 120);
					src_index += gfx->image_buffer1_width;
					dst_index += CAM_VISIBLE_WIDTH;
				}
				horizontal_scroll += 1;
				SetDelayTime(20);
				while (!AwaitDelayTime()) {
					// Just wait
				}

				if (horizontal_scroll == 40) {
					VIDEO_StringToScreenBuffer(28, 16, ui->txt_file[UI_TXT_SCN1I]->line[30], FONT_SLIM_BLACK);
					VIDEO_StringToScreenBuffer(28, 26, ui->txt_file[UI_TXT_SCN1I]->line[31], FONT_SLIM_BLACK);
				}

				if (horizontal_scroll >= max_scroll) scroll_end = true;
				if (scroll_end) step++;
				break;
			case 77:// Wait
				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 78:// Show lavoratory
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 124, 144, 160, 50, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, 120, 140, 162, 52);
				step++;
				break;
			case 79:// Chat
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 10, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 12, 208);
				step++;
				break;
			case 80:// Hide picture 1
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, 120, 140, 12, 52);
				step++;
				break;
			case 81:// Chat
				VIDEO_StringToScreenBuffer(28, 16, ui->txt_file[UI_TXT_SCN1I]->line[32], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(28, 26, ui->txt_file[UI_TXT_SCN1I]->line[33], FONT_SLIM_BLACK);
				step++;
				break;
			case 82://
				GFX_PCXImageToBuffer("OSCN1.DAT", "OUTRO10H.PCX", 270 * 170, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("OSCN1.DAT", "OUTRO11H.PCX", 270 * 170, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				SetDelayTime(2000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 83:// Stop music
				VIDEO_FadeOut(4);
				VIDEO_ClearScreen();
				AUDIO_StopSong();
				AUDIO_LoadSong(AUDIO_SONG_4);
				step++;
				break;
			case 84:// Draw last picture
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 264, 164, 16, 16, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, 260, 160, 18, 18);
				step++;
				break;
			case 85:
				VIDEO_FadeIn(4);
				step++;
				break;
			case 86:
				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				TIMER_UpdateTimerTime(TIMER_AUDIO_NUMBER, 10);
				AUDIO_PlaySong(false);
				step++;
				break;
			case 87:
				i = (rand() % 2 * gfx->image_buffer1_width) + rand() % 2;
				VIDEO_BufferToScreenBuffer(&gfx->image_buffer1[i], gfx->image_buffer1_width, gfx->image_buffer1_height, 260, 160, 18, 18);
				if (AUDIO_GetCurrentLine() == 200) step++;
				break;
			case 88:
				i = (rand() % 4 * gfx->image_buffer1_width) + rand() % 4;
				VIDEO_BufferToScreenBuffer(&gfx->image_buffer1[i], gfx->image_buffer1_width, gfx->image_buffer1_height, 260, 160, 18, 18);
				if (AUDIO_GetCurrentLine() == 550) step++;
				break;
			case 89:
				i = (rand() % 2 * gfx->image_buffer1_width) + rand() % 2;
				VIDEO_BufferToScreenBuffer(&gfx->image_buffer1[i], gfx->image_buffer1_width, gfx->image_buffer1_height, 260, 160, 18, 18);
				SetDelayTime(10);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				if (AUDIO_GetCurrentLine() == 750) step++;
				break;
			case 90:
				i = (rand() % 5 * gfx->image_buffer2_width) + rand() % 5;
				VIDEO_BufferToScreenBuffer(&gfx->image_buffer2[i], gfx->image_buffer2_width, gfx->image_buffer2_height, 260, 160, 18, 18);
				if (AUDIO_GetCurrentLine() == 1100) step++;
				break;
			case 91://
				VIDEO_FadeOutToWhite(4);
				step++;
				break;
			case 92:
				if (AUDIO_IsSongEnded()) step++;
				break;
			case 93:
				SetDelayTime(2000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 94:
				VIDEO_ClearScreen();
				VIDEO_FadeIn(4);
				step++;
				break;
			case 95://End sequence
				engine.sequence = false;
				break;
			default:
				engine.sequence = false;
				break;
		}
		EFFECT_UpdateEffects();
		GFX_UpdateSprites();
		GFX_DrawSprites();
		VIDEO_VSync();
		VIDEO_ScreenBufferToVRAM();
	}
	VIDEO_FadeOut(1);
	ENEMY_UnloadEnemies();
	PARTICLE_UnloadParticles();
	EFFECT_UnloadEffects();
	OBJECT_UnloadObjects();
	GFX_UnloadSprites();
	AUDIO_StopSong();
	AUDIO_UnloadSong();
}
