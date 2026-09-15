#include "engine/engine.h"
#include "engine/types/types.h"

/** SCENARIO 2 :: Mission 2 :: Down to the hell 
*/
void Scene2_GameOver(void) {

	AUDIO_StopSong();
	VIDEO_FadeOutPause(4);
	ENEMY_UnloadEnemies();
	ITEM_UnloadItems();
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
void Scene2_Intro(void) {
	int step;
	int chat_spr_num;
	int i, horizontal_scroll, max_scroll, scanline_scroll, src_index, dst_index, src_img_index;
	bool scroll_end;
	int dialog_step, dialog_delay_counter;
	int char_number;

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
				GFX_PCXImageToBuffer("ISCN2.DAT", "INTRO1H.PCX", 90 * 180, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("ISCN2.DAT", "INTRO3H.PCX", 280 * 180, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				GFX_PCXImageToBuffer("ISCN2.DAT", "INTRO2H.PCX", 90 * 180, gfx->image_buffer3, &gfx->image_buffer3_width, &gfx->image_buffer3_height);
				AUDIO_LoadSong(AUDIO_SONG_3);
				step++;
				break;
			case 2:// Hide loading screen
				UI_HideLoadingScreen();
				step++;
				break;
			case 3:/// Scene 1 :: Agents waiting outside
				VIDEO_ClearScreenBuffer();
				GFX_LoadPalette("PALETTES.DAT", "ISCN2.PCX", 256);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				VIDEO_FadeIn(4);

				TIMER_UpdateTimerTime(TIMER_AUDIO_NUMBER, 50);
				AUDIO_PlaySong(true);

				scanline_scroll = 0;
				step++;
				break;
			case 4:// Opening eyes 1
				scanline_scroll++;
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 320, scanline_scroll, 0, 100 - scanline_scroll, 222);
				SetDelayTime(100);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				if (scanline_scroll >= 10) {
					step++;
				}
				break;
			case 5:// Opening eyes 2
				scanline_scroll--;
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 320, scanline_scroll, 0, 100 - scanline_scroll, 222);
				SetDelayTime(100);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				if (scanline_scroll <= 0) {
					step++;
				}
				break;
			case 6:// Opening eyes 3
				scanline_scroll++;
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 320, scanline_scroll, 0, 100 - scanline_scroll, 222);
				SetDelayTime(100);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				if (scanline_scroll >= 20) {
					step++;
				}
				break;
			case 7:// Opening eyes 4
				scanline_scroll--;
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 320, scanline_scroll, 0, 100 - scanline_scroll, 222);
				SetDelayTime(100);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				if (scanline_scroll <= 0) {
					step++;
				}
				break;
			case 8:// Clear screen
				VIDEO_ClearScreenBuffer();
				SetDelayTime(100);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				scanline_scroll = 1;
				step++;
				break;
			case 9:// Show Jenny progressive
				i = ((gfx->image_buffer1_height / 2) - scanline_scroll) * gfx->image_buffer1_width;
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 94, scanline_scroll * 2, 10, (90 - scanline_scroll + 12), 222);
				VIDEO_BufferToScreenBuffer(&gfx->image_buffer1[i], gfx->image_buffer1_width, gfx->image_buffer1_height, 90, scanline_scroll * 2, 12, (90 - scanline_scroll + 12));
				scanline_scroll++;
				SetDelayTime(20);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				if (scanline_scroll >= 90) step++;
				break;
			case 10:
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 94, 184, 10, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, 90, 180, 12, 12);
				step++;
				break;
			case 11:
				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 12:
				chat_spr_num = GFX_FindEmptySpriteSlot();
				if (chat_spr_num == -1) {
					sprintf(engine.system_error_message1, "Scene 2 Intro function error");
					sprintf(engine.system_error_message2, "No empty sprite slot available");
					sprintf(engine.system_error_message3, " ");
					Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
				} else {
					GFX_InitSprite(ENTITY_ID_EMPTY, 0, chat_spr_num, 0, 65, 30);
				}
				GFX_SetSpriteGraphic(chat_spr_num, 0, SPRITE_GRAPHICS_ID_CHAT_BIG, 0, 0);
				GFX_SetDefaultAnimation(chat_spr_num, false, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 100, 25);
				char_number = 0;
				step++;
				break;
			case 13://Sara's chat
				VIDEO_StringToScreenBuffer(106, 30, ui->txt_file[UI_TXT_SCN2I]->line[1], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 45, ui->txt_file[UI_TXT_SCN2I]->line[2], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 60, ui->txt_file[UI_TXT_SCN2I]->line[3], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 14:// End chat
				GFX_HideSprite(chat_spr_num);
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 94, 184, 10, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, 90, 180, 12, 12);
				SetDelayTime(200);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 15:// Show actor
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 94, 184, 200, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, 90, 180, 202, 12);
				step++;
				break;
			case 16:
				SetDelayTime(2000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 17:// set chat
				GFX_SetDefaultAnimation(chat_spr_num, true, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 85, 45);
				GFX_ShowSprite(chat_spr_num);
				step++;
				break;
			case 18:
				VIDEO_StringToScreenBuffer(88, 50, ui->txt_file[UI_TXT_SCN2I]->line[4], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(88, 65, ui->txt_file[UI_TXT_SCN2I]->line[5], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(88, 80, ui->txt_file[UI_TXT_SCN2I]->line[6], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 19:// End chat
				GFX_HideSprite(chat_spr_num);
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 94, 184, 10, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, 90, 180, 12, 12);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 94, 184, 200, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, 90, 180, 202, 12);
				step++;
				break;
			case 20:
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 21:// Show chat box
				GFX_SetDefaultAnimation(chat_spr_num, false, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 100, 25);
				GFX_ShowSprite(chat_spr_num);
				step++;
				break;
			case 22:// chat
				VIDEO_StringToScreenBuffer(104, 30, ui->txt_file[UI_TXT_SCN2I]->line[7], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(104, 45, ui->txt_file[UI_TXT_SCN2I]->line[8], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(104, 60, ui->txt_file[UI_TXT_SCN2I]->line[9], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 23:
				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 24:
				VIDEO_StringToScreenBuffer(104, 30, ui->txt_file[UI_TXT_SCN2I]->line[10], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(104, 45, ui->txt_file[UI_TXT_SCN2I]->line[11], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 25:
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 26:
				VIDEO_StringToScreenBuffer(104, 30, ui->txt_file[UI_TXT_SCN2I]->line[12], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(104, 45, ui->txt_file[UI_TXT_SCN2I]->line[13], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 27:
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 28:
				VIDEO_StringToScreenBuffer(104, 30, ui->txt_file[UI_TXT_SCN2I]->line[14], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(104, 45, ui->txt_file[UI_TXT_SCN2I]->line[15], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 29:// Show bed
				GFX_HideSprite(chat_spr_num);
				VIDEO_ClearScreen();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 94, 184, 10, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, 90, 180, 12, 12);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 94, 184, 200, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, 90, 180, 202, 12);
				step++;
				break;
			case 30:// set chat
				GFX_SetDefaultAnimation(chat_spr_num, true, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 85, 45);
				GFX_ShowSprite(chat_spr_num);
				step++;
				break;
			case 31:
				VIDEO_StringToScreenBuffer(88, 50, ui->txt_file[UI_TXT_SCN2I]->line[16], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(88, 65, ui->txt_file[UI_TXT_SCN2I]->line[17], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(88, 80, ui->txt_file[UI_TXT_SCN2I]->line[18], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(3000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 32:// Show bed
				GFX_HideSprite(chat_spr_num);
				VIDEO_ClearScreen();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 94, 184, 10, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, 90, 180, 12, 12);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 94, 184, 200, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, 90, 180, 202, 12);
				step++;
				break;
			case 33:
				GFX_SetDefaultAnimation(chat_spr_num, false, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 100, 25);
				GFX_ShowSprite(chat_spr_num);
				step++;
				break;
			case 34:
				VIDEO_StringToScreenBuffer(104, 30, ui->txt_file[UI_TXT_SCN2I]->line[19], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(104, 45, ui->txt_file[UI_TXT_SCN2I]->line[20], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(3000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 35:
				VIDEO_StringToScreenBuffer(104, 30, ui->txt_file[UI_TXT_SCN2I]->line[21], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(104, 45, ui->txt_file[UI_TXT_SCN2I]->line[22], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(104, 60, ui->txt_file[UI_TXT_SCN2I]->line[23], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(3000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 36:
				VIDEO_StringToScreenBuffer(104, 30, ui->txt_file[UI_TXT_SCN2I]->line[24], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(104, 45, ui->txt_file[UI_TXT_SCN2I]->line[25], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(104, 60, ui->txt_file[UI_TXT_SCN2I]->line[26], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 37:
				GFX_HideSprite(chat_spr_num);
				VIDEO_ClearScreen();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 94, 184, 200, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, 90, 180, 202, 12);
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				VIDEO_FadeOut(4);
				step++;
				break;
			case 38:
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 284, 184, 10, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, 280, 180, 12, 12);
				step++;
				break;
			case 39:
				VIDEO_FadeIn(4);
				step++;
				break;
			case 40://Show chat case
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 141, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 143, 208);
				step++;
				break;
			case 41:
				VIDEO_StringToScreenBuffer(28, 148, ui->txt_file[UI_TXT_SCN2I]->line[27], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(28, 158, ui->txt_file[UI_TXT_SCN2I]->line[28], FONT_SLIM_BLACK);
				step++;
				break;
			case 42:// wait
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 43://Show chat case
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 141, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 143, 208);
				step++;
				break;
			case 44:
				VIDEO_StringToScreenBuffer(28, 148, ui->txt_file[UI_TXT_SCN2I]->line[29], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(28, 158, ui->txt_file[UI_TXT_SCN2I]->line[30], FONT_SLIM_BLACK);
				step++;
				break;
			case 45:// wait
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 46://Show chat case
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 141, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 143, 208);
				step++;
				break;
			case 47:
				VIDEO_StringToScreenBuffer(28, 148, ui->txt_file[UI_TXT_SCN2I]->line[31], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(28, 158, ui->txt_file[UI_TXT_SCN2I]->line[32], FONT_SLIM_BLACK);
				step++;
				break;
			case 48:// Load more images
				GFX_PCXImageToBuffer("ISCN2.DAT", "INTRO4H.PCX", 280 * 140, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("ISCN2.DAT", "INTRO4L.PCX", 140 * 140, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				GFX_PCXImageToBuffer("ISCN2.DAT", "INTRO5H.PCX", 140 * 140, gfx->image_buffer3, &gfx->image_buffer3_width, &gfx->image_buffer3_height);
				step++;
				break;
			case 49:
				VIDEO_FadeOut(4);
				step++;
				break;
			case 50:
				horizontal_scroll = 134;
				scroll_end = false;
				dialog_step = 0;

				VIDEO_ClearScreenBuffer();
				VIDEO_ScreenBufferToVRAM();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 150, 146, 5, 50, 222);
				VIDEO_FadeIn(4);
				while (!scroll_end & engine.sequence) {
					src_index = horizontal_scroll;
					dst_index = CAM_VISIBLE_WIDTH * 53 + 7;

					// Draw image with scroll
					for (i = 0; i < gfx->image_buffer1_height; i++) {
						memcpy(&video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index], &gfx->image_buffer1[src_index], 146);
						src_index += gfx->image_buffer1_width;
						dst_index += CAM_VISIBLE_WIDTH;
					}
					if (horizontal_scroll > 0) horizontal_scroll--;

					// Draw text background
					VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 296, 46, 5, 1, 202);
					VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 40, 8, 4, 208);

					// Draw text
					switch (dialog_step) {
						case 0:
							char_number = 0;
							dialog_delay_counter = 0;
							dialog_step++;
							break;
						case 1:
							VIDEO_StringToScreenBuffer(12, 12, ui->txt_file[UI_TXT_SCN2I]->line[33], FONT_SLIM_BLACK);
							VIDEO_StringToScreenBuffer(12, 22, ui->txt_file[UI_TXT_SCN2I]->line[34], FONT_SLIM_BLACK);
							dialog_delay_counter++;
							if (dialog_delay_counter > 100) {
								dialog_step++;
								dialog_delay_counter = 0;
							}
							break;
						case 2:
							dialog_delay_counter++;
							if (dialog_delay_counter > 20) {
								dialog_step++;
								dialog_delay_counter = 0;
							}
							break;
						case 3:
							VIDEO_StringToScreenBuffer(12, 12, ui->txt_file[UI_TXT_SCN2I]->line[35], FONT_SLIM_BLACK);
							VIDEO_StringToScreenBuffer(12, 22, ui->txt_file[UI_TXT_SCN2I]->line[36], FONT_SLIM_BLACK);
							dialog_delay_counter++;
							if (dialog_delay_counter > 100) {
								dialog_step++;
								dialog_delay_counter = 0;
							}
							break;
						case 4:
							dialog_delay_counter++;
							if (dialog_delay_counter > 20) {
								dialog_step++;
								dialog_delay_counter = 0;
							}
							break;
						case 5:
							VIDEO_StringToScreenBuffer(12, 12, ui->txt_file[UI_TXT_SCN2I]->line[37], FONT_SLIM_BLACK);
							VIDEO_StringToScreenBuffer(12, 22, ui->txt_file[UI_TXT_SCN2I]->line[38], FONT_SLIM_BLACK);
							dialog_delay_counter++;
							if (dialog_delay_counter > 100) {
								dialog_step++;
								dialog_delay_counter = 0;
							}
							break;
						case 6:
							dialog_delay_counter++;
							if (dialog_delay_counter > 20) {
								dialog_step++;
								dialog_delay_counter = 0;
							}
							break;
						case 7:
							scroll_end = true;
							break;
						default:
							break;
					}

					SetDelayTime(30);
					while (!AwaitDelayTime()) {
						// Just wait
					}

					VIDEO_VSync();
					VIDEO_ScreenBufferToVRAM();
				}
				step++;
				break;
			case 51:
				// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 296, 46, 5, 1, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 293, 40, 8, 4, 208);
				step++;
				break;
			case 52:
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 150, 146, 165, 50, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, 146, gfx->image_buffer3_height, 168, 53);
				SetDelayTime(100);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 53:
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, 146, gfx->image_buffer2_height, 8, 53);
				step++;
				break;
			case 54:
				// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 296, 46, 5, 1, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 293, 40, 8, 4, 208);

				VIDEO_StringToScreenBuffer(12, 12, ui->txt_file[UI_TXT_SCN2I]->line[43], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(12, 22, ui->txt_file[UI_TXT_SCN2I]->line[44], FONT_SLIM_BLACK);
				dialog_delay_counter++;
				if (dialog_delay_counter > 100) {
					step++;
					dialog_delay_counter = 0;
				}
				break;
			case 55:
				dialog_delay_counter++;
				if (dialog_delay_counter > 20) {
					step++;
					dialog_delay_counter = 0;
				}
				break;
			case 56:
				// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 296, 46, 5, 1, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 293, 40, 8, 4, 208);
				VIDEO_StringToScreenBuffer(12, 12, ui->txt_file[UI_TXT_SCN2I]->line[45], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(12, 22, ui->txt_file[UI_TXT_SCN2I]->line[46], FONT_SLIM_BLACK);
				dialog_delay_counter++;
				if (dialog_delay_counter > 100) {
					step++;
					dialog_delay_counter = 0;
				}
				break;
			case 57:
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 58://End sequence
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
void Scene2_LoadAssets(void) {

	// Load and unload enemies just to get graphic resources loaded
	ENEMY_Load("ENEMY6.DAT", 0, 0 << 4, 0 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_CHASE, 1);
	ENEMY_UnloadEnemies();
	ENEMY_Load("ENEMY7.DAT", 0, 0 << 4, 0 << 4, SPRITE_GRAPHICS_ID_ENEMY2_CHAT, SPRITE_GRAPHICS_ID_ENEMY2_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY2_FEET, SPRITE_GRAPHICS_ID_ENEMY2_BODY, SPRITE_GRAPHICS_ID_ENEMY2_HEAD, SPRITE_GRAPHICS_ID_ENEMY2_LARM, SPRITE_GRAPHICS_ID_ENEMY2_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 10);
	ENEMY_UnloadEnemies();

	// Load boss and unload it just to preload graphics
	BOSS_Load("BOSS1.DAT", 0, 0, BOSS_TYPE_RAT, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_BULLET3, BOSS_STATUS_CHASE, 2000);
	BOSS_Unload();
	BOSS_Load("BOSS2.DAT", 0, 0, BOSS_TYPE_SPIDER, SPRITE_GRAPHICS_ID_ENEMY4_CHAT, SPRITE_GRAPHICS_ID_ENEMY4_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY4_FEET, SPRITE_GRAPHICS_ID_ENEMY4_BODY, SPRITE_GRAPHICS_ID_ENEMY4_HEAD, SPRITE_GRAPHICS_ID_ENEMY4_LARM, SPRITE_GRAPHICS_ID_ENEMY4_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_BULLET3, BOSS_STATUS_CHASE, 2000);
	BOSS_Unload();

	ACTOR_Load(78 << 4, 3 << 4, SPRITE_GRAPHICS_ID_ACTOR_FEET, SPRITE_GRAPHICS_ID_ACTOR_BODY, SPRITE_GRAPHICS_ID_ACTOR_HEAD, SPRITE_GRAPHICS_ID_ACTOR_LARM, SPRITE_GRAPHICS_ID_ACTOR_RARM_BARE_HANDS, ACTOR_FACING_LEFT);
	ACTOR_SetGun(actor->sprite_num, ACTOR_GUN_TYPE_BARE_HANDS);
	ACTOR_SetBulletStatus(99, 19, 199, 9, 9);

	UI_SetStatusPanels();// Set status panels
}
void Scene2_UnloadAssets(void) {
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

	// Enemy 3
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY3_CHAT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY3_FEET);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY3_BODY);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY3_HEAD);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY3_RARM);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY3_LARM);

	// Enemy 4
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY4_CHAT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY4_PORTAIT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY4_FEET);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY4_BODY);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY4_HEAD);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY4_RARM);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY4_LARM);

	MM_PopChunks(CT_TEMPORARY_SPRITE);
}
void Scene2_LoadRoom1(void) {
	ENEMY_Load("ENEMY6.DAT", 0, 57 << 4, 2 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_SLEEP, 1);
	ENEMY_Load("ENEMY6.DAT", 1, 58 << 4, 2 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_SLEEP, 1);
	ENEMY_Load("ENEMY6.DAT", 2, 67 << 4, 8 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_SLEEP, 1);
	ENEMY_Load("ENEMY6.DAT", 3, 70 << 4, 15 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_SLEEP, 1);
	ENEMY_Load("ENEMY6.DAT", 4, 73 << 4, 21 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_SLEEP, 1);
	ENEMY_Load("ENEMY6.DAT", 5, 72 << 4, 33 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_SLEEP, 1);
	ENEMY_Load("ENEMY6.DAT", 6, 73 << 4, 33 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_SLEEP, 1);
	ENEMY_Load("ENEMY6.DAT", 7, 62 << 4, 44 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_SLEEP, 1);
	ENEMY_Load("ENEMY6.DAT", 8, 63 << 4, 44 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_SLEEP, 1);

	ITEM_LoadItem(0, ENTITY_ID_ITEM_MEDIKIT, SPRITE_GRAPHICS_ID_ITEM_MEDIKIT, 11 << 4, 45 << 4);

	ITEM_LoadItem(1, ENTITY_ID_ITEM_KEYRED, SPRITE_GRAPHICS_ID_ITEM_RED_KEY, 74 << 4, 16 << 4);
	ITEM_LoadItem(2, ENTITY_ID_ITEM_KEYGREEN, SPRITE_GRAPHICS_ID_ITEM_GREEN_KEY, 51 << 4, 42 << 4);
	ITEM_LoadItem(3, ENTITY_ID_ITEM_KEYBLUE, SPRITE_GRAPHICS_ID_ITEM_BLUE_KEY, 11 << 4, 13 << 4);
	ITEM_LoadItem(4, ENTITY_ID_ITEM_KEYYELLOW, SPRITE_GRAPHICS_ID_ITEM_YELLOW_KEY, 18 << 4, 21 << 4);

	ITEM_LoadItem(5, ENTITY_ID_ITEM_AMMO1, SPRITE_GRAPHICS_ID_ITEM_AMMO1, 56 << 4, 42 << 4);
	ITEM_LoadItem(6, ENTITY_ID_ITEM_AMMO3, SPRITE_GRAPHICS_ID_ITEM_AMMO3, 78 << 4, 29 << 4);
	ITEM_LoadItem(7, ENTITY_ID_ITEM_MEDIKIT, SPRITE_GRAPHICS_ID_ITEM_MEDIKIT, 29 << 4, 39 << 4);


	ITEM_LoadItem(8, ENTITY_ID_ITEM_MEDIKIT, SPRITE_GRAPHICS_ID_ITEM_MEDIKIT, 41 << 4, 18 << 4);
	ITEM_LoadItem(9, ENTITY_ID_ITEM_AMMO3, SPRITE_GRAPHICS_ID_ITEM_AMMO3, 46 << 4, 4 << 4);

	ITEM_LoadItem(10, ENTITY_ID_ITEM_GRENADE, SPRITE_GRAPHICS_ID_ITEM_GRENADE, 60 << 4, 18 << 4);

	ITEM_LoadItem(11, ENTITY_ID_ITEM_AMMO2, SPRITE_GRAPHICS_ID_ITEM_AMMO2, 20 << 4, 17 << 4);
	ITEM_LoadItem(12, ENTITY_ID_ITEM_AMMO3, SPRITE_GRAPHICS_ID_ITEM_AMMO3, 27 << 4, 39 << 4);

	MAP_LoadMap("MAPSCN21.DAT", 80, 57, "TSCN21.DAT", "SCN2_1_BACK.PCX", "SCN2_1_FORE.PCX", "SCN2_1_MASK.PCX", 320 * 416, 128 * 128, 128 * 128);

	AUDIO_LoadSong(AUDIO_SONG_5);// Load song
}
void Scene2_SetHotspotsAndEvents(void) {

	// Update hotspots
	map->hotspot_enabling_room1[0] = false;
	map->hotspot_enabling_room1[1] = false;
	map->hotspot_enabling_room1[2] = false;
	map->hotspot_enabling_room1[3] = false;
	map->hotspot_enabling_room1[4] = false;
	map->hotspot_enabling_room1[5] = false;
	map->hotspot_enabling_room1[6] = false;
	map->hotspot_enabling_room1[7] = false;
	map->hotspot_enabling_room1[8] = false;
	map->hotspot_enabling_room1[9] = false;
	map->hotspot_enabling_room1[10] = false;
	map->hotspot_enabling_room1[11] = false;
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
	map->event_enabling_room1[7] = true;
	map->event_enabling_room1[8] = true;
	map->event_enabling_room1[9] = true;
	map->event_enabling_room1[10] = true;
	map->event_enabling_room1[11] = true;
	map->event_enabling_room1[12] = true;
	map->event_enabling_room1[13] = true;
	map->event_enabling_room1[14] = true;
	map->event_enabling_room1[15] = true;
	map->event_enabling_room1[16] = true;
	map->event_enabling_room1[17] = true;
	map->event_enabling_room1[18] = true;
	map->event_enabling_room1[19] = true;
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
}
void Scene2_Loop(void) {
	bool end_sequence;
	int actor_tile_num;
	int sequence_step;
	int scene_step;
	int scene_counter;

	VIDEO_ClearScreenBuffer();
	GFX_LoadPalette("PALETTES.DAT", "SCN21.PCX", 256);

	VIDEO_StringToScreenBuffer(50, 60, ui->txt_file[UI_TXT_SCN2I]->line[40], FONT_BIG_BLACK);
	VIDEO_StringToScreenBuffer(90, 85, ui->txt_file[UI_TXT_SCN2I]->line[41], FONT_BIG_BLACK);
	VIDEO_StringToScreenBuffer(90, 135, ui->txt_file[UI_TXT_SCN2I]->line[48], FONT_SLIM_WHITE);
	VIDEO_StringToScreenBuffer(160, 135, ui->txt_file[UI_TXT_SCN2I]->line[49], FONT_SLIM_WHITE);

	VIDEO_VSync();
	VIDEO_ScreenBufferToVRAM();
	VIDEO_FadeIn(4);

	SetDelayTime(6000);

	Scene2_LoadAssets();
	Scene2_SetHotspotsAndEvents();

	// Initialize the Scene room
	switch (engine.room) {
		case 1:// Room 1. Sewer
			Scene2_LoadRoom1();

			ACTOR_SetPosition(78 << 4, 3 << 4, ACTOR_FACING_LEFT);   // Set actor position
			CAM_Init(map->width_px, map->height_px, 60 << 4, 0 << 4);// Initialize camera
			MAP_DrawMapToMapVideoBuffer();

			Update(true);
			Update(true);
			VIDEO_FadeIn(1);

			TIMER_UpdateTimerTime(TIMER_AUDIO_NUMBER, 55);
			AUDIO_PlaySong(true);
			break;
		default:
			Error("Scene2_Loop function error", "Undefined room", "", ERROR_SYSTEM);
			break;
	}

	VIDEO_FadeOut(4);
	VIDEO_ClearScreenBuffer();

	// Set palette
	switch (engine.room) {
		case 1:// Room 1. Outside doscity
			GFX_LoadPalette("PALETTES.DAT", "SCN21.PCX", 256);
			break;
		default:
			sprintf(engine.system_error_message1, "Scene2_Loop function error");
			sprintf(engine.system_error_message2, "Undefined room");
			sprintf(engine.system_error_message3, "Selected room: %u", engine.room);
			Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_SYSTEM);
			break;
	}

	Update(true);
	Update(true);

	scene_step = 0;
	sequence_step = 0;
	scene_counter = 0;
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
			case 1:// Room 1. Sewer
				// Scene steps
				// - Event 3: Starts the party! >> Step = 1 - Load rats
				// - Event 4: Open red door >> Step = 2
				// - Event 6: Get green key >> Step = 3 - Load more rats
				// - Event 13: Open green door. Access to boss 1 >> Step = 4 - Clear all rats
				// - Event 7: Load boss 1 >> Step = 5
				// - Boss 1 dead >> Step = 6 - Open boss door
				// - Event 8: Out of boss room >> Step 7
				// - Event 9: Entering spiders zone >> Step 8 - Show spiders
				// - Event 18: Open blue door >> Step 9 -
				// - Event 10: Entering water spiders zone >> Step 10 -
				// - Event 19: Open yellow door >> Step 11 - Unload all spiders
				// - Event 11: Enter boss room >> Step 12 - Load boss 2
				// - Boss 2 dead >> Step 13 - Unlock exit door
				// - Event 12: End level >> Step 14

				// Scene counter
				scene_counter++;
				if (scene_counter > 64) scene_counter = 0;

				//scene_counter = 0;

				// Hotspots
				if (!actor->mode_combat) {
					// Check point on object or enemy
					switch (cursor.point_on & 0xFF) {
						case ENTITY_ID_HSPOT:// Hotspot
							switch ((cursor.point_on >> 8) & 0xFF) {
								case 0:// No hotspot, WTF??
									break;
								case 1:// Spare

									break;
								case 2:// Spare

									break;
								case 3:// Spare

									break;
								case 4:// Spare

									break;
								case 5:// Spare

									break;
								case 6:// Spare

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
					case 1:// Event 1. Spare
						if (map->event_enabling_room1[1]) {
							map->event_enabling_room1[1] = false;
						}
						break;
					case 2:// Event 2. Getting in
						if (map->event_enabling_room1[2]) {
							map->event_enabling_room1[2] = false;

							sequence_step = 0;
							end_sequence = false;
							while (!end_sequence) {
								switch (sequence_step) {
									case 0://
										UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN2D, 30, 31, 355, false, 100);
										sequence_step++;
										break;
									case 1://
										if (UI_IsSpeechFinished()) {
											ACTOR_SetCombatMode(true);
											ACTOR_SetGun(actor->sprite_num, ACTOR_GUN_TYPE_PISTOL);
											ACTOR_Reload();
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
							AUDIO_StopSong();
							AUDIO_UnloadSong();
							AUDIO_LoadSong(AUDIO_SONG_7);// Load song
							TIMER_UpdateTimerTime(TIMER_AUDIO_NUMBER, 50);
							AUDIO_PlaySong(true);

							ENEMY_SetBehavior(0, ENEMY_STATUS_CHASE);
							ENEMY_SetBehavior(1, ENEMY_STATUS_CHASE);
						}
						break;
					case 3:// Event 3. Starts the rat party!
						if (map->event_enabling_room1[3]) {
							map->event_enabling_room1[3] = false;


							ENEMY_SetBehavior(2, ENEMY_STATUS_CHASE);
							ENEMY_SetBehavior(3, ENEMY_STATUS_CHASE);
							ENEMY_SetBehavior(4, ENEMY_STATUS_CHASE);
							ENEMY_SetBehavior(5, ENEMY_STATUS_CHASE);
							ENEMY_SetBehavior(6, ENEMY_STATUS_CHASE);
							ENEMY_SetBehavior(7, ENEMY_STATUS_CHASE);
							ENEMY_SetBehavior(8, ENEMY_STATUS_CHASE);

							scene_step = 1;
						}
						break;
					case 4:// Event 4. Red Door, access to water
						if (map->event_enabling_room1[4]) {
							map->event_enabling_room1[4] = false;

							if (actor->key_entity_id != ENTITY_ID_ITEM_KEYRED) {
								AUDIO_PlaySound(AUDIO_SHOT_FAIL_EFFECT, 1);
							} else {

								scene_step = 2;
								ACTOR_LeaveKey();
								AUDIO_PlaySound(AUDIO_OPEN_DOOR_EFFECT, 1);

								MAP_HideSwapableTile(69 << 4, 40 << 4);
								MAP_HideSwapableTile(70 << 4, 40 << 4);
								MAP_HideSwapableTile(71 << 4, 40 << 4);
								MAP_HideSwapableTile(69 << 4, 41 << 4);
								MAP_HideSwapableTile(70 << 4, 41 << 4);
								MAP_HideSwapableTile(71 << 4, 41 << 4);
							}
						}
						break;
					case 5:// Event 5. Water 2
						if (map->event_enabling_room1[5]) {
							map->event_enabling_room1[5] = false;
						}
						break;
					case 6:// Event 6. Get green key
						if (map->event_enabling_room1[6]) {
							map->event_enabling_room1[6] = false;

							scene_step = 3;
						}
						break;
					case 7:// Event 7. BIG Surprise
						if (map->event_enabling_room1[7]) {
							map->event_enabling_room1[7] = false;
							scene_step = 5;

							MAP_ShowSwapableTile(45 << 4, 40 << 4);
							MAP_ShowSwapableTile(46 << 4, 40 << 4);
							MAP_ShowSwapableTile(47 << 4, 40 << 4);
							MAP_ShowSwapableTile(45 << 4, 41 << 4);
							MAP_ShowSwapableTile(46 << 4, 41 << 4);
							MAP_ShowSwapableTile(47 << 4, 41 << 4);

							BOSS_Load("BOSS1.DAT", 47 << 4, 27 << 4, BOSS_TYPE_RAT, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUKE, BOSS_STATUS_STATIC, 2000);

							sequence_step = 0;
							end_sequence = false;
							while (!end_sequence) {
								switch (sequence_step) {
									case 0://
										UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN2D, 1, 355, 355, true, 100);
										sequence_step++;
										break;
									case 1://
										if (UI_IsSpeechFinished()) {
											UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN2D, 3, 4, 355, false, 100);
											sequence_step++;
										}
										break;
									case 2://
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

							BOSS_SetBehavior(BOSS_STATUS_CHASE);
							ACTOR_SetCombatMode(true);
						}
						break;
					case 8:// Event 8. Bye bye rats
						if (map->event_enabling_room1[8]) {
							map->event_enabling_room1[8] = false;

							scene_step = 7;
						}
						break;
					case 9:// Event 9. Spiders everywhere!
						if (map->event_enabling_room1[9]) {
							map->event_enabling_room1[9] = false;

							if (!enemy[0].is_loaded) {
								ENEMY_Load("ENEMY7.DAT", 0, 41 << 4, 7 << 4, SPRITE_GRAPHICS_ID_ENEMY2_CHAT, SPRITE_GRAPHICS_ID_ENEMY2_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY2_FEET, SPRITE_GRAPHICS_ID_ENEMY2_BODY, SPRITE_GRAPHICS_ID_ENEMY2_HEAD, SPRITE_GRAPHICS_ID_ENEMY2_LARM, SPRITE_GRAPHICS_ID_ENEMY2_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_WEBS, ENEMY_STATUS_STATIC_SHOOTER, 10);
							} else if (!enemy[1].is_loaded) {
								ENEMY_Load("ENEMY7.DAT", 1, 27 << 4, 9 << 4, SPRITE_GRAPHICS_ID_ENEMY2_CHAT, SPRITE_GRAPHICS_ID_ENEMY2_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY2_FEET, SPRITE_GRAPHICS_ID_ENEMY2_BODY, SPRITE_GRAPHICS_ID_ENEMY2_HEAD, SPRITE_GRAPHICS_ID_ENEMY2_LARM, SPRITE_GRAPHICS_ID_ENEMY2_RARM, ENEMY_FACING_RIGHT, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_WEBS, ENEMY_STATUS_STATIC_SHOOTER, 10);
							} else if (!enemy[2].is_loaded) {
								ENEMY_Load("ENEMY7.DAT", 2, 35 << 4, 13 << 4, SPRITE_GRAPHICS_ID_ENEMY2_CHAT, SPRITE_GRAPHICS_ID_ENEMY2_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY2_FEET, SPRITE_GRAPHICS_ID_ENEMY2_BODY, SPRITE_GRAPHICS_ID_ENEMY2_HEAD, SPRITE_GRAPHICS_ID_ENEMY2_LARM, SPRITE_GRAPHICS_ID_ENEMY2_RARM, ENEMY_FACING_UP, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_WEBS, ENEMY_STATUS_STATIC_SHOOTER, 10);
							} else if (!enemy[3].is_loaded) {
								ENEMY_Load("ENEMY7.DAT", 3, 32 << 4, 35 << 4, SPRITE_GRAPHICS_ID_ENEMY2_CHAT, SPRITE_GRAPHICS_ID_ENEMY2_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY2_FEET, SPRITE_GRAPHICS_ID_ENEMY2_BODY, SPRITE_GRAPHICS_ID_ENEMY2_HEAD, SPRITE_GRAPHICS_ID_ENEMY2_LARM, SPRITE_GRAPHICS_ID_ENEMY2_RARM, ENEMY_FACING_RIGHT, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_WEBS, ENEMY_STATUS_STATIC_SHOOTER, 10);
							}

							scene_step = 8;
						}
						break;
					case 10:// Event 10. End spiders!
						if (map->event_enabling_room1[10]) {
							map->event_enabling_room1[10] = false;
							scene_step = 10;
						}
						break;
					case 11:// Event 11. The big one!
						if (map->event_enabling_room1[11]) {
							map->event_enabling_room1[11] = false;
							scene_step = 12;

							MAP_ShowSwapableTile(9 << 4, 41 << 4);
							MAP_ShowSwapableTile(10 << 4, 41 << 4);
							MAP_ShowSwapableTile(11 << 4, 41 << 4);
							MAP_ShowSwapableTile(9 << 4, 42 << 4);
							MAP_ShowSwapableTile(10 << 4, 42 << 4);
							MAP_ShowSwapableTile(11 << 4, 42 << 4);

							BOSS_Load("BOSS2.DAT", 5 << 4, 28 << 4, BOSS_TYPE_SPIDER, SPRITE_GRAPHICS_ID_ENEMY4_CHAT, SPRITE_GRAPHICS_ID_ENEMY4_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY4_FEET, SPRITE_GRAPHICS_ID_ENEMY4_BODY, SPRITE_GRAPHICS_ID_ENEMY4_HEAD, SPRITE_GRAPHICS_ID_ENEMY4_LARM, SPRITE_GRAPHICS_ID_ENEMY4_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_WEB, BOSS_STATUS_STATIC, 2000);//

							sequence_step = 0;
							end_sequence = false;
							while (!end_sequence) {
								switch (sequence_step) {
									case 0://
										UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ENEMY4_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN2D, 7, 355, 355, true, 100);
										sequence_step++;
										break;
									case 1://
										if (UI_IsSpeechFinished()) {
											UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN2D, 9, 10, 355, false, 100);
											sequence_step++;
										}
										break;
									case 2://
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

							BOSS_SetBehavior(BOSS_STATUS_CHASE);
							ACTOR_SetCombatMode(true);
						}
						break;
					case 12:// Event 12: End of scene
						if (map->event_enabling_room1[12]) {
							map->event_enabling_room1[12] = false;
							scene_step = 15;
						}
						break;
					case 13:// Event 13. Green Door to boss 1
						if (map->event_enabling_room1[13]) {
							map->event_enabling_room1[13] = false;

							if (actor->key_entity_id != ENTITY_ID_ITEM_KEYGREEN) {
								AUDIO_PlaySound(AUDIO_SHOT_FAIL_EFFECT, 1);
							} else {
								ENEMY_UnloadEnemies();

								ACTOR_LeaveKey();
								scene_step = 4;

								AUDIO_PlaySound(AUDIO_OPEN_DOOR_EFFECT, 1);

								MAP_HideSwapableTile(45 << 4, 40 << 4);
								MAP_HideSwapableTile(46 << 4, 40 << 4);
								MAP_HideSwapableTile(47 << 4, 40 << 4);
								MAP_HideSwapableTile(45 << 4, 41 << 4);
								MAP_HideSwapableTile(46 << 4, 41 << 4);
								MAP_HideSwapableTile(47 << 4, 41 << 4);
							}
						}
						break;
					case 16:// Event 16. Enter building
						if (map->event_enabling_room1[16]) {
							map->event_enabling_room1[16] = false;

							VIDEO_FadeOut(4);
							engine.ingame = false;
							engine.scene = 3;
							engine.room = 1;
						}
						break;
					case 18:// Event 18. Blue door
						if (map->event_enabling_room1[18]) {
							map->event_enabling_room1[18] = false;

							if (actor->key_entity_id != ENTITY_ID_ITEM_KEYBLUE) {
								AUDIO_PlaySound(AUDIO_SHOT_FAIL_EFFECT, 1);
							} else {
								ACTOR_LeaveKey();
								scene_step = 9;

								AUDIO_PlaySound(AUDIO_OPEN_DOOR_EFFECT, 1);

								MAP_HideSwapableTile(34 << 4, 31 << 4);
								MAP_HideSwapableTile(35 << 4, 31 << 4);
								MAP_HideSwapableTile(36 << 4, 31 << 4);
								MAP_HideSwapableTile(34 << 4, 32 << 4);
								MAP_HideSwapableTile(35 << 4, 32 << 4);
								MAP_HideSwapableTile(36 << 4, 32 << 4);
							}
						}
						break;
					case 19:// Event 19. Yellow door
						if (map->event_enabling_room1[19]) {
							map->event_enabling_room1[19] = false;

							if (actor->key_entity_id != ENTITY_ID_ITEM_KEYYELLOW) {
								AUDIO_PlaySound(AUDIO_SHOT_FAIL_EFFECT, 1);
							} else {
								ACTOR_LeaveKey();
								scene_step = 11;

								AUDIO_PlaySound(AUDIO_OPEN_DOOR_EFFECT, 1);

								MAP_HideSwapableTile(9 << 4, 41 << 4);
								MAP_HideSwapableTile(10 << 4, 41 << 4);
								MAP_HideSwapableTile(11 << 4, 41 << 4);
								MAP_HideSwapableTile(9 << 4, 42 << 4);
								MAP_HideSwapableTile(10 << 4, 42 << 4);
								MAP_HideSwapableTile(11 << 4, 42 << 4);
							}
						}
						break;
					default:
						// Reenable red door event if actor did not have key
						if (!map->event_enabling_room1[4] && (scene_step < 2)) {
							map->event_enabling_room1[4] = true;
						}

						// Reenable green door event if actor did not have key
						if (!map->event_enabling_room1[13] && (scene_step < 4)) {
							map->event_enabling_room1[13] = true;
						}

						// Reenable blue door event if actor did not have key
						if (!map->event_enabling_room1[18] && (scene_step < 9)) {
							map->event_enabling_room1[18] = true;
						}

						// Reenable yellow door event if actor did not have key
						if (!map->event_enabling_room1[19] && (scene_step < 11)) {
							map->event_enabling_room1[19] = true;
						}
						break;
				}

				switch (scene_step) {
					case 1:// Rats!!!!
						switch (scene_counter) {
							case 0:
								if (!enemy[0].is_loaded) {
									ENEMY_Load("ENEMY6.DAT", 0, 57 << 4, 2 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_CHASE, 1);
								}
								break;
							case 12:
								if (!enemy[1].is_loaded) {
									ENEMY_Load("ENEMY6.DAT", 1, 58 << 4, 2 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_CHASE, 1);
								}
								break;
							case 24:
								if (!enemy[2].is_loaded) {
									ENEMY_Load("ENEMY6.DAT", 2, 67 << 4, 8 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_CHASE, 1);
								}
								break;
							case 36:
								if (!enemy[3].is_loaded) {
									ENEMY_Load("ENEMY6.DAT", 3, 70 << 4, 15 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_CHASE, 1);
								}
								break;
							case 47:
								if (!enemy[4].is_loaded) {
									ENEMY_Load("ENEMY6.DAT", 4, 73 << 4, 21 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_CHASE, 1);
								}
								break;
							case 58:
								if (!enemy[5].is_loaded) {
									ENEMY_Load("ENEMY6.DAT", 5, 72 << 4, 33 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_CHASE, 1);
								}
								break;
							case 64:
								if (!enemy[6].is_loaded) {
									ENEMY_Load("ENEMY6.DAT", 6, 73 << 4, 33 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_CHASE, 1);
								}
								break;
							default:
								break;
						}
						break;
					case 2:
						break;
					case 3:// more rats!!
						switch (scene_counter) {
							case 16:
								if (!enemy[7].is_loaded) {
									ENEMY_Load("ENEMY6.DAT", 7, 62 << 4, 44 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_CHASE, 1);
								}
								break;
							case 32:
								if (!enemy[8].is_loaded) {
									ENEMY_Load("ENEMY6.DAT", 8, 63 << 4, 44 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_CHASE, 1);
								}
								break;
							default:
								break;
						}
						break;
					case 4:// kill all rats (on event)
						break;
					case 5:
						if (boss->is_loaded) {
							switch (scene_counter) {
								case 16:
									if (!enemy[0].is_loaded) {
										ENEMY_Load("ENEMY6.DAT", 0, 50 << 4, 26 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_CHASE, 1);
									}
									break;
								case 32:
									if (!enemy[1].is_loaded) {
										ENEMY_Load("ENEMY6.DAT", 1, 51 << 4, 26 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_CHASE, 1);
									}
									break;
								default:
									break;
							}
						}

						if (!boss->is_loaded && !enemy[0].is_loaded && !enemy[1].is_loaded) {
							MAP_HideSwapableTile(56 << 4, 24 << 4);
							MAP_HideSwapableTile(57 << 4, 24 << 4);
							MAP_HideSwapableTile(56 << 4, 25 << 4);
							MAP_HideSwapableTile(57 << 4, 25 << 4);

							scene_step = 6;
						}
						break;
					case 6:
						ENEMY_UnloadEnemies();
						break;
					case 7:
						break;
					case 8:// more spiders
						switch (scene_counter) {
							case 8:
								if (!enemy[4].is_loaded) {
									ENEMY_Load("ENEMY7.DAT", 4, 47 << 4, 4 << 4, SPRITE_GRAPHICS_ID_ENEMY2_CHAT, SPRITE_GRAPHICS_ID_ENEMY2_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY2_FEET, SPRITE_GRAPHICS_ID_ENEMY2_BODY, SPRITE_GRAPHICS_ID_ENEMY2_HEAD, SPRITE_GRAPHICS_ID_ENEMY2_LARM, SPRITE_GRAPHICS_ID_ENEMY2_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_WEBS, ENEMY_STATUS_CHASE, 10);
								}
								break;
							case 16:
								if (!enemy[5].is_loaded) {
									ENEMY_Load("ENEMY7.DAT", 5, 14 << 4, 4 << 4, SPRITE_GRAPHICS_ID_ENEMY2_CHAT, SPRITE_GRAPHICS_ID_ENEMY2_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY2_FEET, SPRITE_GRAPHICS_ID_ENEMY2_BODY, SPRITE_GRAPHICS_ID_ENEMY2_HEAD, SPRITE_GRAPHICS_ID_ENEMY2_LARM, SPRITE_GRAPHICS_ID_ENEMY2_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_WEBS, ENEMY_STATUS_CHASE, 10);
								}
								break;
							case 24:
								if (!enemy[6].is_loaded) {
									ENEMY_Load("ENEMY7.DAT", 6, 11 << 4, 13 << 4, SPRITE_GRAPHICS_ID_ENEMY2_CHAT, SPRITE_GRAPHICS_ID_ENEMY2_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY2_FEET, SPRITE_GRAPHICS_ID_ENEMY2_BODY, SPRITE_GRAPHICS_ID_ENEMY2_HEAD, SPRITE_GRAPHICS_ID_ENEMY2_LARM, SPRITE_GRAPHICS_ID_ENEMY2_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_WEBS, ENEMY_STATUS_CHASE, 10);
								}
								break;
							case 32:
								if (!enemy[7].is_loaded) {
									ENEMY_Load("ENEMY7.DAT", 7, 19 << 4, 17 << 4, SPRITE_GRAPHICS_ID_ENEMY2_CHAT, SPRITE_GRAPHICS_ID_ENEMY2_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY2_FEET, SPRITE_GRAPHICS_ID_ENEMY2_BODY, SPRITE_GRAPHICS_ID_ENEMY2_HEAD, SPRITE_GRAPHICS_ID_ENEMY2_LARM, SPRITE_GRAPHICS_ID_ENEMY2_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_WEBS, ENEMY_STATUS_CHASE, 10);
								}
								break;
							case 48:
								if (!enemy[8].is_loaded) {
									ENEMY_Load("ENEMY7.DAT", 8, 27 << 4, 28 << 4, SPRITE_GRAPHICS_ID_ENEMY2_CHAT, SPRITE_GRAPHICS_ID_ENEMY2_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY2_FEET, SPRITE_GRAPHICS_ID_ENEMY2_BODY, SPRITE_GRAPHICS_ID_ENEMY2_HEAD, SPRITE_GRAPHICS_ID_ENEMY2_LARM, SPRITE_GRAPHICS_ID_ENEMY2_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_WEBS, ENEMY_STATUS_CHASE, 10);
								}
								break;
							default:
								break;
						}
						break;
					case 9:
						break;
					case 10:
						break;
					case 11:// kill all spiders
						ENEMY_UnloadEnemies();
						break;
					case 12:
						if (boss->is_loaded) {
							switch (scene_counter) {
								case 16:
									if (!enemy[9].is_loaded) {
										ENEMY_Load("ENEMY7.DAT", 9, 3 << 4, 29 << 4, SPRITE_GRAPHICS_ID_ENEMY2_CHAT, SPRITE_GRAPHICS_ID_ENEMY2_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY2_FEET, SPRITE_GRAPHICS_ID_ENEMY2_BODY, SPRITE_GRAPHICS_ID_ENEMY2_HEAD, SPRITE_GRAPHICS_ID_ENEMY2_LARM, SPRITE_GRAPHICS_ID_ENEMY2_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_WEBS, ENEMY_STATUS_CHASE, 10);
									}
									break;
								case 32:
									if (!enemy[10].is_loaded) {
										ENEMY_Load("ENEMY7.DAT", 10, 4 << 4, 29 << 4, SPRITE_GRAPHICS_ID_ENEMY2_CHAT, SPRITE_GRAPHICS_ID_ENEMY2_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY2_FEET, SPRITE_GRAPHICS_ID_ENEMY2_BODY, SPRITE_GRAPHICS_ID_ENEMY2_HEAD, SPRITE_GRAPHICS_ID_ENEMY2_LARM, SPRITE_GRAPHICS_ID_ENEMY2_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_WEBS, ENEMY_STATUS_CHASE, 10);
									}
									break;
								default:
									break;
							}
						}

						if (!boss->is_loaded && !enemy[9].is_loaded && !enemy[10].is_loaded) {
							MAP_HideSwapableTile(9 << 4, 27 << 4);
							MAP_HideSwapableTile(10 << 4, 27 << 4);
							MAP_HideSwapableTile(9 << 4, 28 << 4);
							MAP_HideSwapableTile(10 << 4, 28 << 4);

							scene_step = 13;
						}
						break;
					case 13:
						break;
					case 14:// End of level
						break;
					default:
						break;
				}
		}

		Update(true);

		// Finish all if actor is dead
		if (actor->status_dead) Scene2_GameOver();
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
	BOSS_Unload();
	NPC_UnloadNpcs();
	ITEM_UnloadItems();
	ACTOR_UnloadActor();
	MAP_UnloadMap();
	Scene2_UnloadAssets();
}
void Scene2_Outro(void) {
	int step;

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
				GFX_PCXImageToBuffer("OSCN2.DAT", "OUTRO1H.PCX", 140 * 140, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("OSCN2.DAT", "OUTRO1L.PCX", 140 * 140, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				GFX_PCXImageToBuffer("OSCN2.DAT", "OUTRO2H.PCX", 160 * 160, gfx->image_buffer3, &gfx->image_buffer3_width, &gfx->image_buffer3_height);

				step++;
				break;
			case 2:// Hide loading screen
				UI_HideLoadingScreen();
				step++;
				break;
			case 3:/// Scene 1 :: Agents waiting outside
				VIDEO_ClearScreenBuffer();
				GFX_LoadPalette("PALETTES.DAT", "OSCN2.PCX", 256);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 146, 10, 40, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 13, 43);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				VIDEO_FadeIn(4);

				TIMER_UpdateTimerTime(TIMER_AUDIO_NUMBER, 50);
				AUDIO_PlaySong(true);

				step++;
				break;
			case 4:// Just wait
				SetDelayTime(2000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 5://Show chat case
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 208);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				step++;
				break;
			case 6:// Speech
				VIDEO_StringToScreenBuffer(28, 13, ui->txt_file[UI_TXT_SCN2I]->line[60], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(28, 25, ui->txt_file[UI_TXT_SCN2I]->line[61], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				step++;
				break;
			case 7:// Just wait
				SetDelayTime(2000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 8:// Second frame
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 166, 166, 150, 30, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 153, 33);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 208);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				step++;
				break;
			case 9:// Just wait
				SetDelayTime(2000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 10:// Hide first frame
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 146, 10, 40, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 13, 43);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 166, 166, 150, 30, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 153, 33);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 208);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				step++;
				break;
			case 11://Show chat case
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 208);
				step++;
				break;
			case 12:// Speech
				VIDEO_StringToScreenBuffer(28, 13, ui->txt_file[UI_TXT_SCN2I]->line[62], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(28, 25, ui->txt_file[UI_TXT_SCN2I]->line[63], FONT_SLIM_BLACK);
				step++;
				break;
			case 13:// Just wait
				SetDelayTime(8000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 14://End sequence
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
