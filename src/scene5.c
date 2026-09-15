#include "engine/engine.h"
#include "engine/gfx/gfx.h"
#include "engine/types/types.h"


/** SCENARIO 5 :: Mission 5 :: Hit and run
 */
void Scene5_GameOver(void) {
	AUDIO_StopSong();
	VIDEO_FadeOutPause(4);
	ENEMY_UnloadEnemies();
	OBJECT_UnloadObjects();
	BOSS_Unload();
	EFFECT_UnloadEffects();
	ITEM_UnloadItems();
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
void Scene5_Intro(void) {
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
			case 1:// Load first intro scene
				GFX_PCXImageToBuffer("ISCN5.DAT", "INTRO1H.PCX", 200 * 120, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("ISCN5.DAT", "INTRO2H.PCX", 140 * 140, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				GFX_PCXImageToBuffer("ISCN5.DAT", "INTRO3H.PCX", 110 * 160, gfx->image_buffer3, &gfx->image_buffer3_width, &gfx->image_buffer3_height);

				AUDIO_LoadSong(AUDIO_SONG_3);
				step++;
				break;
			case 2:// Hide loading screen
				UI_HideLoadingScreen();
				step++;
				break;
			case 3:/// Scene 1 :: Entering kitchen
				VIDEO_ClearScreenBuffer();
				GFX_LoadPalette("PALETTES.DAT", "ISCN5.PCX", 256);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				VIDEO_FadeIn(4);

				TIMER_UpdateTimerTime(TIMER_AUDIO_NUMBER, 50);
				AUDIO_PlaySong(true);

				step++;
				break;
			case 4:// Draw frame and image 1
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 206, 126, 30, 50, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 33, 53);
				step++;
				break;
			case 5:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 6:// Show text
				   // Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 46, 5, 1, 233);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 293, 40, 9, 4, 208);
				VIDEO_StringToScreenBuffer(12, 12, ui->txt_file[UI_TXT_SCN5I]->line[1], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(12, 22, ui->txt_file[UI_TXT_SCN5I]->line[2], FONT_SLIM_BLACK);
				step++;
				break;
			case 7:// Just wait
				SetDelayTime(6000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 8:// Hide text
				   // Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 46, 5, 1, 233);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 293, 40, 9, 4, 208);
				step++;
				break;
			case 9:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 10:// Show text
					// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 46, 5, 1, 233);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 293, 40, 9, 4, 208);
				VIDEO_StringToScreenBuffer(12, 12, ui->txt_file[UI_TXT_SCN5I]->line[3], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(12, 22, ui->txt_file[UI_TXT_SCN5I]->line[4], FONT_SLIM_BLACK);
				step++;
				break;
			case 11:// Just wait
				SetDelayTime(6000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 12:// Hide text
					// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 46, 5, 1, 233);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 293, 40, 9, 4, 208);
				step++;
				break;
			case 13:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 14:// Show text
					// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 46, 5, 1, 233);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 293, 40, 9, 4, 208);
				VIDEO_StringToScreenBuffer(12, 12, ui->txt_file[UI_TXT_SCN5I]->line[5], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(12, 22, ui->txt_file[UI_TXT_SCN5I]->line[6], FONT_SLIM_BLACK);
				step++;
				break;
			case 15:// Just wait
				SetDelayTime(6000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 16://  Clear back and Show text
				VIDEO_ClearScreenBuffer();
				// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 46, 5, 1, 233);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 293, 40, 9, 4, 208);
				VIDEO_StringToScreenBuffer(12, 12, ui->txt_file[UI_TXT_SCN5I]->line[7], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(12, 22, ui->txt_file[UI_TXT_SCN5I]->line[8], FONT_SLIM_BLACK);
				step++;
				break;
			case 17:// Just wait
				SetDelayTime(6000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 18://Clear back and show image 2
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 146, 20, 30, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 23, 33);
				step++;
				break;
			case 19:// Just wait
				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 20:// Show images 2 and 3
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 146, 20, 30, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 23, 33);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 116, 166, 180, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 183, 23);
				step++;
				break;
			case 21:// Just wait
				SetDelayTime(6000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 22:// Load images 4 and 5
				GFX_PCXImageToBuffer("ISCN5.DAT", "INTRO4H.PCX", 120 * 180, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("ISCN5.DAT", "INTRO4L.PCX", 120 * 180, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				GFX_PCXImageToBuffer("ISCN5.DAT", "INTRO5H.PCX", 130 * 130, gfx->image_buffer3, &gfx->image_buffer3_width, &gfx->image_buffer3_height);
				step++;
				break;
			case 23:// Hide text box and Show image 4
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 126, 186, 20, 5, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 23, 8);
				step++;
				break;
			case 24:
				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 25:// Show image 4 and 5
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 126, 186, 20, 5, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 23, 8);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 136, 136, 160, 40, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 163, 43);
				step++;
				break;
			case 26:
				SetDelayTime(100);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 27:// Fade image 4 and show 5
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 126, 186, 20, 5, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 23, 8);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 136, 136, 160, 40, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 163, 43);
				step++;
				break;
			case 28:// Load images 6 and 7
				GFX_PCXImageToBuffer("ISCN5.DAT", "INTRO6H.PCX", 100 * 100, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("ISCN5.DAT", "INTRO6L.PCX", 100 * 100, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				GFX_PCXImageToBuffer("ISCN5.DAT", "INTRO7H.PCX", 100 * 100, gfx->image_buffer3, &gfx->image_buffer3_width, &gfx->image_buffer3_height);
				step++;
				break;
			case 29:// clear backgound and Show image 6
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 106, 106, 20, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 23, 13);
				step++;
				break;
			case 30:
				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 31:// Show image 6 and 7
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 106, 106, 20, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 23, 13);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 106, 106, 120, 90, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 123, 93);
				step++;
				break;
			case 32:
				SetDelayTime(100);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 33:// Fade image 6 and show 7
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 106, 106, 20, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 23, 13);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 106, 106, 120, 90, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 123, 93);
				step++;
				break;
			case 34:// Load image 8
				GFX_PCXImageToBuffer("ISCN5.DAT", "INTRO8H.PCX", 100 * 100, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("ISCN5.DAT", "INTRO7L.PCX", 100 * 100, gfx->image_buffer3, &gfx->image_buffer3_width, &gfx->image_buffer3_height);
				step++;
				break;
			case 35:// Fade image 6 and 7 and show 8
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 106, 106, 20, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 23, 13);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 106, 106, 120, 90, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 123, 93);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 106, 106, 200, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 203, 13);
				step++;
				break;
			case 36:// Load images 9
				GFX_PCXImageToBuffer("ISCN5.DAT", "INTRO9H.PCX", 260 * 140, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				step++;
				break;
			case 37:// Show text box and image 9
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 46, 5, 1, 233);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 293, 40, 9, 4, 208);

				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 266, 146, 20, 50, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 23, 53);
				step++;
				break;
			case 38:
				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 39:// Show text
					// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 46, 5, 1, 233);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 293, 40, 9, 4, 208);
				VIDEO_StringToScreenBuffer(12, 12, ui->txt_file[UI_TXT_SCN5I]->line[10], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(12, 22, ui->txt_file[UI_TXT_SCN5I]->line[11], FONT_SLIM_BLACK);
				step++;
				break;
			case 40:// Just wait
				SetDelayTime(6000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 41:// Hide text
					// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 46, 5, 1, 233);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 293, 40, 9, 4, 208);

				step++;
				break;
			case 42:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 43:// Show text
					// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 46, 5, 1, 233);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 293, 40, 9, 4, 208);
				VIDEO_StringToScreenBuffer(12, 12, ui->txt_file[UI_TXT_SCN5I]->line[12], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(12, 22, ui->txt_file[UI_TXT_SCN5I]->line[13], FONT_SLIM_BLACK);
				step++;
				break;
			case 44:// Just wait
				SetDelayTime(6000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 45:// Hide text
					// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 46, 5, 1, 233);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 293, 40, 9, 4, 208);

				step++;
				break;
			case 46:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 47:// Show text
					// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 46, 5, 1, 233);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 293, 40, 9, 4, 208);
				VIDEO_StringToScreenBuffer(12, 12, ui->txt_file[UI_TXT_SCN5I]->line[14], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(12, 22, ui->txt_file[UI_TXT_SCN5I]->line[15], FONT_SLIM_BLACK);
				step++;
				break;
			case 48:// Just wait
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 49:// Load images 10 and 11
				GFX_PCXImageToBuffer("ISCN5.DAT", "INTRO10H.PCX", 260 * 70, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("ISCN5.DAT", "INTRO10L.PCX", 260 * 70, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				GFX_PCXImageToBuffer("ISCN5.DAT", "INTRO11H.PCX", 260 * 70, gfx->image_buffer3, &gfx->image_buffer3_width, &gfx->image_buffer3_height);
				step++;
				break;
			case 50:// Hide text box and Show image 10
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 266, 76, 30, 5, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 33, 8);
				step++;
				break;
			case 51:
				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 52:// Show image 10 and 11
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 266, 76, 30, 5, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 33, 8);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 266, 76, 30, 100, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 33, 103);
				step++;
				break;
			case 53:
				SetDelayTime(100);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 54:// Fade image 10 and show 11
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 266, 76, 30, 5, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 8);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 266, 76, 30, 100, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 33, 103);
				step++;
				break;
			case 55:
				SetDelayTime(6000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 56:
				VIDEO_FadeOutToWhite(4);
				step++;
				break;
			case 57://End sequence
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
void Scene5_UnloadAssets(void) {
	// Enemy 1
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_CHAT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_FEET);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_BODY);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_HEAD);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_RARM);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_LARM);

	// Object
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_OBJECT1);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_OBJECT1_PORTAIT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_OBJECT2);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_OBJECT3);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT);

	MM_PopChunks(CT_TEMPORARY_SPRITE);
}
void Scene5_LoadAssets(void) {

	ENEMY_Load("ENEMY9.DAT", 0, 0, 0, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_GUN0, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_NPC, 1);
	ENEMY_UnloadEnemies();

	// Load objects and unload them just to get graphics
	OBJECT_LoadObject("OBJECT1.DAT", 0, ENTITY_ID_BARREL, SPRITE_GRAPHICS_ID_OBJECT1, SPRITE_GRAPHICS_ID_OBJECT1_PORTAIT, 0, 0);
	OBJECT_LoadObject("OBJECT4.DAT", 1, ENTITY_ID_BARREL, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 0, 0);
	OBJECT_LoadObject("OBJECT5.DAT", 2, ENTITY_ID_BARREL, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 0, 0);
	OBJECT_UnloadObjects();

	ACTOR_Load(9 << 4, 47 << 4, SPRITE_GRAPHICS_ID_ACTOR_FEET, SPRITE_GRAPHICS_ID_ACTOR_BODY, SPRITE_GRAPHICS_ID_ACTOR_HEAD, SPRITE_GRAPHICS_ID_ACTOR_LARM, SPRITE_GRAPHICS_ID_ACTOR_RARM_BARE_HANDS, ACTOR_FACING_RIGHT);
	ACTOR_SetGun(actor->sprite_num, ACTOR_GUN_TYPE_BARE_HANDS);
	ACTOR_SetBulletStatus(99, 19, 199, 9, 9);

	NPC_Init();

	UI_SetStatusPanels();// Set status panels

	AUDIO_LoadSong(AUDIO_SONG_3);// Load song
}
void Scene5_LoadRoom1(void) {

	// Items
	ITEM_LoadItem(0, ENTITY_ID_ITEM_AMMO4, SPRITE_GRAPHICS_ID_ITEM_AMMO4, 21 << 4, 6 << 4);
	ITEM_LoadItem(1, ENTITY_ID_ITEM_AMMO2, SPRITE_GRAPHICS_ID_ITEM_AMMO2, 20 << 4, 6 << 4);
	ITEM_LoadItem(2, ENTITY_ID_ITEM_GRENADE, SPRITE_GRAPHICS_ID_ITEM_GRENADE, 21 << 4, 11 << 4);

	MAP_LoadMap("MAPSCN51.DAT", 25, 20, "TSCN51.DAT", "SCN5_1_BACK.PCX", "SCN5_1_FORE.PCX", "SCN5_1_MASK.PCX", 320 * 416, 128 * 128, 128 * 128);
	GFX_LoadPalette("PALETTES.DAT", "SCN51.PCX", 256);

	AUDIO_LoadSong(AUDIO_SONG_7);// Load song
}
void Scene5_LoadRoom2(void) {

	// Load enemies. Main corridor
	ENEMY_Load("ENEMY9.DAT", 0, 8 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 1, 22 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 2, 34 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 1
	ENEMY_Load("ENEMY9.DAT", 3, 17 << 4, 7 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 2
	ENEMY_Load("ENEMY9.DAT", 4, 25 << 4, 7 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 3
	ENEMY_Load("ENEMY9.DAT", 5, 10 << 4, 20 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 4
	ENEMY_Load("ENEMY9.DAT", 6, 21 << 4, 17 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 7, 32 << 4, 21 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 5
	ENEMY_Load("ENEMY9.DAT", 8, 38 << 4, 19 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 9, 40 << 4, 14 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Kitchen
	ENEMY_Load("ENEMY9.DAT", 10, 37 << 4, 5 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Stairs
	ENEMY_Load("ENEMY9.DAT", 11, 48 << 4, 12 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Lock upstairs left
	//OBJECT_LoadObject("OBJECT4.DAT", 1, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 2 << 4, 9 << 4);
	//OBJECT_LoadObject("OBJECT5.DAT", 2, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 3 << 4, 10 << 4);
	//OBJECT_LoadObject("OBJECT5.DAT", 3, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 2 << 4, 11 << 4);

	// Lock downstairs left
	OBJECT_LoadObject("OBJECT4.DAT", 1, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 2 << 4, 14 << 4);
	OBJECT_LoadObject("OBJECT5.DAT", 2, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 3 << 4, 15 << 4);
	OBJECT_LoadObject("OBJECT5.DAT", 3, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 2 << 4, 16 << 4);

	// Lock main path 1
	OBJECT_LoadObject("OBJECT4.DAT", 4, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 19 << 4, 11 << 4);
	OBJECT_LoadObject("OBJECT5.DAT", 5, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 20 << 4, 12 << 4);
	OBJECT_LoadObject("OBJECT4.DAT", 6, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 19 << 4, 13 << 4);

	// Lock main path 2
	OBJECT_LoadObject("OBJECT4.DAT", 7, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 35 << 4, 8 << 4);
	OBJECT_LoadObject("OBJECT5.DAT", 8, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 36 << 4, 9 << 4);

	// Lock upstairs right
	OBJECT_LoadObject("OBJECT4.DAT", 9, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 44 << 4, 11 << 4);

	// Lock downstairs right
	//OBJECT_LoadObject("OBJECT4.DAT", 9, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 44 << 4, 15 << 4);

	// Lock room 1
	//OBJECT_LoadObject("OBJECT4.DAT", 10, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 15 << 4, 8 << 4);
	//OBJECT_LoadObject("OBJECT5.DAT", 11, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 17 << 4, 8 << 4);

	// Lock room 2
	OBJECT_LoadObject("OBJECT4.DAT", 12, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 23 << 4, 8 << 4);
	OBJECT_LoadObject("OBJECT5.DAT", 13, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 25 << 4, 8 << 4);

	// Lock room 3
	//OBJECT_LoadObject("OBJECT4.DAT", 14, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 9 << 4, 18 << 4);
	//OBJECT_LoadObject("OBJECT5.DAT", 15, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 11 << 4, 18 << 4);

	// Lock room 4
	//OBJECT_LoadObject("OBJECT4.DAT", 16, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 38 << 4, 12 << 4);
	//OBJECT_LoadObject("OBJECT5.DAT", 17, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 40 << 4, 13 << 4);

	// Lock room 5
	//OBJECT_LoadObject("OBJECT4.DAT", 18, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 38 << 4, 12 << 4);
	//OBJECT_LoadObject("OBJECT5.DAT", 19, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 40 << 4, 13 << 4);


	MAP_LoadMap("MAPSCN52.DAT", 52, 29, "TSCN52.DAT", "SCN5_2_BACK.PCX", "SCN5_2_FORE.PCX", "SCN5_2_MASK.PCX", 320 * 416, 128 * 128, 128 * 128);
	GFX_LoadPalette("PALETTES.DAT", "SCN52.PCX", 256);

	AUDIO_LoadSong(AUDIO_SONG_7);// Load song
}
void Scene5_Update_Floor1(void) {
	OBJECT_UnloadObjects();
	ENEMY_UnloadEnemies();

	// Load enemies. Main corridor
	ENEMY_Load("ENEMY9.DAT", 0, 8 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 1, 22 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 2, 34 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 1
	ENEMY_Load("ENEMY9.DAT", 3, 17 << 4, 7 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 2
	ENEMY_Load("ENEMY9.DAT", 4, 25 << 4, 7 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 3
	ENEMY_Load("ENEMY9.DAT", 5, 10 << 4, 20 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 4
	ENEMY_Load("ENEMY9.DAT", 6, 21 << 4, 17 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 7, 32 << 4, 21 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 5
	ENEMY_Load("ENEMY9.DAT", 8, 38 << 4, 19 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 9, 40 << 4, 14 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Kitchen
	ENEMY_Load("ENEMY9.DAT", 10, 37 << 4, 5 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Stairs
	ENEMY_Load("ENEMY9.DAT", 11, 48 << 4, 12 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);


	// Lock upstairs left
	OBJECT_LoadObject("OBJECT4.DAT", 1, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 2 << 4, 9 << 4);
	OBJECT_LoadObject("OBJECT5.DAT", 2, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 3 << 4, 10 << 4);
	OBJECT_LoadObject("OBJECT5.DAT", 3, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 2 << 4, 11 << 4);

	// Lock downstairs left
	//OBJECT_LoadObject("OBJECT4.DAT", 1, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 2 << 4, 14 << 4);
	//OBJECT_LoadObject("OBJECT5.DAT", 2, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 3 << 4, 15 << 4);
	//OBJECT_LoadObject("OBJECT5.DAT", 3, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 2 << 4, 16 << 4);

	// Lock main path 1
	OBJECT_LoadObject("OBJECT4.DAT", 4, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 19 << 4, 11 << 4);
	OBJECT_LoadObject("OBJECT5.DAT", 5, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 20 << 4, 12 << 4);
	OBJECT_LoadObject("OBJECT4.DAT", 6, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 19 << 4, 13 << 4);

	// Lock main path 2
	//OBJECT_LoadObject("OBJECT4.DAT", 7, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 35 << 4, 8 << 4);
	//OBJECT_LoadObject("OBJECT5.DAT", 8, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 36 << 4, 9 << 4);

	// Lock upstairs right
	//OBJECT_LoadObject("OBJECT4.DAT", 9, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 44 << 4, 11 << 4);

	// Lock downstairs right
	OBJECT_LoadObject("OBJECT4.DAT", 9, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 44 << 4, 15 << 4);

	// Lock room 1
	OBJECT_LoadObject("OBJECT4.DAT", 10, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 15 << 4, 8 << 4);
	OBJECT_LoadObject("OBJECT5.DAT", 11, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 17 << 4, 8 << 4);

	// Lock room 2
	OBJECT_LoadObject("OBJECT4.DAT", 12, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 23 << 4, 8 << 4);
	OBJECT_LoadObject("OBJECT5.DAT", 13, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 25 << 4, 8 << 4);

	// Lock room 3
	//OBJECT_LoadObject("OBJECT4.DAT", 14, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 9 << 4, 18 << 4);
	//OBJECT_LoadObject("OBJECT5.DAT", 15, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 11 << 4, 18 << 4);

	// Lock room 4
	//OBJECT_LoadObject("OBJECT4.DAT", 16, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 38 << 4, 12 << 4);
	//OBJECT_LoadObject("OBJECT5.DAT", 17, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 40 << 4, 13 << 4);

	// Lock room 5
	OBJECT_LoadObject("OBJECT4.DAT", 18, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 38 << 4, 12 << 4);
	OBJECT_LoadObject("OBJECT5.DAT", 19, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 40 << 4, 13 << 4);
}
void Scene5_Update_Floor2(void) {
	OBJECT_UnloadObjects();
	ENEMY_UnloadEnemies();

	// Load enemies. Main corridor
	ENEMY_Load("ENEMY9.DAT", 0, 8 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 1, 22 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 2, 34 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 1
	ENEMY_Load("ENEMY9.DAT", 3, 17 << 4, 7 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 2
	ENEMY_Load("ENEMY9.DAT", 4, 25 << 4, 7 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 3
	ENEMY_Load("ENEMY9.DAT", 5, 10 << 4, 20 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 4
	ENEMY_Load("ENEMY9.DAT", 6, 21 << 4, 17 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 7, 32 << 4, 21 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 5
	ENEMY_Load("ENEMY9.DAT", 8, 38 << 4, 19 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 9, 40 << 4, 14 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Kitchen
	ENEMY_Load("ENEMY9.DAT", 10, 37 << 4, 5 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Stairs
	ENEMY_Load("ENEMY9.DAT", 11, 48 << 4, 12 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);


	// Lock upstairs left
	//OBJECT_LoadObject("OBJECT4.DAT", 1, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 2 << 4, 9 << 4);
	//OBJECT_LoadObject("OBJECT5.DAT", 2, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 3 << 4, 10 << 4);
	//OBJECT_LoadObject("OBJECT5.DAT", 3, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 2 << 4, 11 << 4);

	// Lock downstairs left
	OBJECT_LoadObject("OBJECT4.DAT", 1, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 2 << 4, 14 << 4);
	OBJECT_LoadObject("OBJECT5.DAT", 2, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 3 << 4, 15 << 4);
	OBJECT_LoadObject("OBJECT5.DAT", 3, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 2 << 4, 16 << 4);

	// Lock main path 1
	OBJECT_LoadObject("OBJECT4.DAT", 4, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 19 << 4, 11 << 4);
	OBJECT_LoadObject("OBJECT5.DAT", 5, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 20 << 4, 12 << 4);
	OBJECT_LoadObject("OBJECT4.DAT", 6, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 19 << 4, 13 << 4);

	// Lock main path 2
	OBJECT_LoadObject("OBJECT4.DAT", 7, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 35 << 4, 8 << 4);
	OBJECT_LoadObject("OBJECT5.DAT", 8, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 36 << 4, 9 << 4);

	// Lock upstairs right
	OBJECT_LoadObject("OBJECT4.DAT", 9, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 44 << 4, 11 << 4);

	// Lock downstairs right
	//OBJECT_LoadObject("OBJECT4.DAT", 9, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 44 << 4, 15 << 4);

	// Lock room 1
	//OBJECT_LoadObject("OBJECT4.DAT", 10, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 15 << 4, 8 << 4);
	//OBJECT_LoadObject("OBJECT5.DAT", 11, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 17 << 4, 8 << 4);

	// Lock room 2
	//OBJECT_LoadObject("OBJECT4.DAT", 12, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 23 << 4, 8 << 4);
	//OBJECT_LoadObject("OBJECT5.DAT", 13, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 25 << 4, 8 << 4);

	// Lock room 3
	OBJECT_LoadObject("OBJECT4.DAT", 14, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 9 << 4, 18 << 4);
	OBJECT_LoadObject("OBJECT5.DAT", 15, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 11 << 4, 18 << 4);

	// Lock room 4
	//OBJECT_LoadObject("OBJECT4.DAT", 16, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 38 << 4, 12 << 4);
	//OBJECT_LoadObject("OBJECT5.DAT", 17, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 40 << 4, 13 << 4);

	// Lock room 5
	//OBJECT_LoadObject("OBJECT4.DAT", 18, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 38 << 4, 12 << 4);
	//OBJECT_LoadObject("OBJECT5.DAT", 19, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 40 << 4, 13 << 4);
}
void Scene5_Update_Floor3(void) {
	OBJECT_UnloadObjects();
	ENEMY_UnloadEnemies();

	// Load enemies. Main corridor
	ENEMY_Load("ENEMY9.DAT", 0, 8 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 1, 22 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 2, 34 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 1
	ENEMY_Load("ENEMY9.DAT", 3, 17 << 4, 7 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 2
	ENEMY_Load("ENEMY9.DAT", 4, 25 << 4, 7 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 3
	ENEMY_Load("ENEMY9.DAT", 5, 10 << 4, 20 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 4
	ENEMY_Load("ENEMY9.DAT", 6, 21 << 4, 17 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 7, 32 << 4, 21 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 5
	ENEMY_Load("ENEMY9.DAT", 8, 38 << 4, 19 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 9, 40 << 4, 14 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Kitchen
	ENEMY_Load("ENEMY9.DAT", 10, 37 << 4, 5 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Stairs
	ENEMY_Load("ENEMY9.DAT", 11, 48 << 4, 12 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Lock upstairs left
	OBJECT_LoadObject("OBJECT4.DAT", 1, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 2 << 4, 9 << 4);
	OBJECT_LoadObject("OBJECT5.DAT", 2, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 3 << 4, 10 << 4);
	OBJECT_LoadObject("OBJECT5.DAT", 3, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 2 << 4, 11 << 4);

	// Lock downstairs left
	//OBJECT_LoadObject("OBJECT4.DAT", 1, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 2 << 4, 14 << 4);
	//OBJECT_LoadObject("OBJECT5.DAT", 2, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 3 << 4, 15 << 4);
	//OBJECT_LoadObject("OBJECT5.DAT", 3, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 2 << 4, 16 << 4);

	// Lock main path 1
	//OBJECT_LoadObject("OBJECT4.DAT", 4, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 19 << 4, 11 << 4);
	//OBJECT_LoadObject("OBJECT5.DAT", 5, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 20 << 4, 12 << 4);
	//OBJECT_LoadObject("OBJECT4.DAT", 6, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 19 << 4, 13 << 4);

	// Lock main path 2
	OBJECT_LoadObject("OBJECT4.DAT", 7, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 35 << 4, 8 << 4);
	OBJECT_LoadObject("OBJECT5.DAT", 8, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 36 << 4, 9 << 4);

	// Lock upstairs right
	//OBJECT_LoadObject("OBJECT4.DAT", 9, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 44 << 4, 11 << 4);

	// Lock downstairs right
	OBJECT_LoadObject("OBJECT4.DAT", 9, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 44 << 4, 15 << 4);

	// Lock room 1
	OBJECT_LoadObject("OBJECT4.DAT", 10, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 15 << 4, 8 << 4);
	OBJECT_LoadObject("OBJECT5.DAT", 11, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 17 << 4, 8 << 4);

	// Lock room 2
	OBJECT_LoadObject("OBJECT4.DAT", 12, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 23 << 4, 8 << 4);
	OBJECT_LoadObject("OBJECT5.DAT", 13, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 25 << 4, 8 << 4);

	// Lock room 3
	OBJECT_LoadObject("OBJECT4.DAT", 14, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 9 << 4, 18 << 4);
	OBJECT_LoadObject("OBJECT5.DAT", 15, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 11 << 4, 18 << 4);

	// Lock room 4
	//OBJECT_LoadObject("OBJECT4.DAT", 16, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 38 << 4, 12 << 4);
	//OBJECT_LoadObject("OBJECT5.DAT", 17, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 40 << 4, 13 << 4);

	// Lock room 5
	//OBJECT_LoadObject("OBJECT4.DAT", 18, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 38 << 4, 12 << 4);
	//OBJECT_LoadObject("OBJECT5.DAT", 19, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 40 << 4, 13 << 4);
}
void Scene5_Update_Floor4(void) {
	OBJECT_UnloadObjects();
	ENEMY_UnloadEnemies();

	// Load enemies. Main corridor
	ENEMY_Load("ENEMY9.DAT", 0, 8 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 1, 22 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 2, 34 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 1
	ENEMY_Load("ENEMY9.DAT", 3, 17 << 4, 7 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 2
	ENEMY_Load("ENEMY9.DAT", 4, 25 << 4, 7 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 3
	ENEMY_Load("ENEMY9.DAT", 5, 10 << 4, 20 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 4
	ENEMY_Load("ENEMY9.DAT", 6, 21 << 4, 17 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 7, 32 << 4, 21 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 5
	ENEMY_Load("ENEMY9.DAT", 8, 38 << 4, 19 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 9, 40 << 4, 14 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Kitchen
	ENEMY_Load("ENEMY9.DAT", 10, 37 << 4, 5 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Stairs
	ENEMY_Load("ENEMY9.DAT", 11, 48 << 4, 12 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Lock upstairs left
	//OBJECT_LoadObject("OBJECT4.DAT", 1, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 2 << 4, 9 << 4);
	//OBJECT_LoadObject("OBJECT5.DAT", 2, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 3 << 4, 10 << 4);
	//OBJECT_LoadObject("OBJECT5.DAT", 3, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 2 << 4, 11 << 4);

	// Lock downstairs left
	OBJECT_LoadObject("OBJECT4.DAT", 1, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 2 << 4, 14 << 4);
	OBJECT_LoadObject("OBJECT5.DAT", 2, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 3 << 4, 15 << 4);
	OBJECT_LoadObject("OBJECT5.DAT", 3, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 2 << 4, 16 << 4);

	// Lock main path 1
	OBJECT_LoadObject("OBJECT4.DAT", 4, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 19 << 4, 11 << 4);
	OBJECT_LoadObject("OBJECT5.DAT", 5, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 20 << 4, 12 << 4);
	OBJECT_LoadObject("OBJECT4.DAT", 6, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 19 << 4, 13 << 4);

	// Lock main path 2
	//OBJECT_LoadObject("OBJECT4.DAT", 7, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 35 << 4, 8 << 4);
	//OBJECT_LoadObject("OBJECT5.DAT", 8, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 36 << 4, 9 << 4);

	// Lock upstairs right
	OBJECT_LoadObject("OBJECT4.DAT", 9, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 44 << 4, 11 << 4);

	// Lock downstairs right
	//OBJECT_LoadObject("OBJECT4.DAT", 9, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 44 << 4, 15 << 4);

	// Lock room 1
	//OBJECT_LoadObject("OBJECT4.DAT", 10, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 15 << 4, 8 << 4);
	//OBJECT_LoadObject("OBJECT5.DAT", 11, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 17 << 4, 8 << 4);

	// Lock room 2
	OBJECT_LoadObject("OBJECT4.DAT", 12, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 23 << 4, 8 << 4);
	OBJECT_LoadObject("OBJECT5.DAT", 13, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 25 << 4, 8 << 4);

	// Lock room 3
	//OBJECT_LoadObject("OBJECT4.DAT", 14, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 9 << 4, 18 << 4);
	//OBJECT_LoadObject("OBJECT5.DAT", 15, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 11 << 4, 18 << 4);

	// Lock room 4
	//OBJECT_LoadObject("OBJECT4.DAT", 16, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 38 << 4, 12 << 4);
	//OBJECT_LoadObject("OBJECT5.DAT", 17, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 40 << 4, 13 << 4);

	// Lock room 5
	OBJECT_LoadObject("OBJECT4.DAT", 18, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 38 << 4, 12 << 4);
	OBJECT_LoadObject("OBJECT5.DAT", 19, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 40 << 4, 13 << 4);
}
void Scene5_Update_Floor5(void) {
	OBJECT_UnloadObjects();
	ENEMY_UnloadEnemies();

	// Load enemies. Main corridor
	ENEMY_Load("ENEMY9.DAT", 0, 8 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 1, 22 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 2, 34 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 1
	ENEMY_Load("ENEMY9.DAT", 3, 17 << 4, 7 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 2
	ENEMY_Load("ENEMY9.DAT", 4, 25 << 4, 7 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 3
	ENEMY_Load("ENEMY9.DAT", 5, 10 << 4, 20 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 4
	ENEMY_Load("ENEMY9.DAT", 6, 21 << 4, 17 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 7, 32 << 4, 21 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 5
	ENEMY_Load("ENEMY9.DAT", 8, 38 << 4, 19 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 9, 40 << 4, 14 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Kitchen
	ENEMY_Load("ENEMY9.DAT", 10, 37 << 4, 5 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Stairs
	ENEMY_Load("ENEMY9.DAT", 11, 48 << 4, 12 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Lock downstairs left
	//OBJECT_LoadObject("OBJECT4.DAT", 1, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 2 << 4, 14 << 4);
	//OBJECT_LoadObject("OBJECT5.DAT", 2, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 3 << 4, 15 << 4);
	//OBJECT_LoadObject("OBJECT5.DAT", 3, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 2 << 4, 16 << 4);

	// Lock downstairs left
	OBJECT_LoadObject("OBJECT4.DAT", 1, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 2 << 4, 9 << 4);
	OBJECT_LoadObject("OBJECT5.DAT", 2, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 3 << 4, 10 << 4);
	OBJECT_LoadObject("OBJECT5.DAT", 3, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 2 << 4, 11 << 4);

	// Lock main path 1
	OBJECT_LoadObject("OBJECT4.DAT", 4, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 19 << 4, 11 << 4);
	OBJECT_LoadObject("OBJECT5.DAT", 5, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 20 << 4, 12 << 4);
	OBJECT_LoadObject("OBJECT4.DAT", 6, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 19 << 4, 13 << 4);

	// Lock main path 2
	OBJECT_LoadObject("OBJECT4.DAT", 7, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 35 << 4, 8 << 4);
	OBJECT_LoadObject("OBJECT5.DAT", 8, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 36 << 4, 9 << 4);

	// Lock upstairs right
	//OBJECT_LoadObject("OBJECT4.DAT", 9, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 44 << 4, 11 << 4);

	// Lock downstairs right
	OBJECT_LoadObject("OBJECT4.DAT", 9, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 44 << 4, 15 << 4);

	// Lock room 1
	//OBJECT_LoadObject("OBJECT4.DAT", 10, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 15 << 4, 8 << 4);
	//OBJECT_LoadObject("OBJECT5.DAT", 11, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 17 << 4, 8 << 4);

	// Lock room 2
	//OBJECT_LoadObject("OBJECT4.DAT", 12, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 23 << 4, 8 << 4);
	//OBJECT_LoadObject("OBJECT5.DAT", 13, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 25 << 4, 8 << 4);

	// Lock room 3
	OBJECT_LoadObject("OBJECT4.DAT", 14, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 9 << 4, 18 << 4);
	OBJECT_LoadObject("OBJECT5.DAT", 15, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 11 << 4, 18 << 4);

	// Lock room 4
	//OBJECT_LoadObject("OBJECT4.DAT", 16, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 38 << 4, 12 << 4);
	//OBJECT_LoadObject("OBJECT5.DAT", 17, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 40 << 4, 13 << 4);

	// Lock room 5
	//OBJECT_LoadObject("OBJECT4.DAT", 18, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 38 << 4, 12 << 4);
	//OBJECT_LoadObject("OBJECT5.DAT", 19, ENTITY_ID_NO_BREAK_OBJ, SPRITE_GRAPHICS_ID_OBJECT3, SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT, 40 << 4, 13 << 4);
}
void Scene5_SetHotspotsAndEvents(void) {
	// Initialize hotspots and events
	// Update hotspots
	map->hotspot_enabling_room1[0] = false;
	map->hotspot_enabling_room1[1] = true;
	map->hotspot_enabling_room1[2] = true;
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
	map->event_enabling_room1[3] = false;
	map->event_enabling_room1[4] = false;
	map->event_enabling_room1[5] = false;
	map->event_enabling_room1[6] = false;
	map->event_enabling_room1[7] = false;
	map->event_enabling_room1[8] = false;
	map->event_enabling_room1[9] = false;
	map->event_enabling_room1[10] = false;
	map->event_enabling_room1[11] = false;
	map->event_enabling_room1[12] = false;
	map->event_enabling_room1[13] = false;
	map->event_enabling_room1[14] = false;
	map->event_enabling_room1[15] = false;
	map->event_enabling_room1[16] = true;
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
	map->hotspot_enabling_room2[1] = false;
	map->hotspot_enabling_room2[2] = false;
	map->hotspot_enabling_room2[3] = false;
	map->hotspot_enabling_room2[4] = false;
	map->hotspot_enabling_room2[5] = false;
	map->hotspot_enabling_room2[6] = false;
	map->hotspot_enabling_room2[7] = false;
	map->hotspot_enabling_room2[8] = false;
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
	map->event_enabling_room2[1] = false;
	map->event_enabling_room2[2] = false;
	map->event_enabling_room2[3] = false;
	map->event_enabling_room2[4] = false;
	map->event_enabling_room2[5] = false;
	map->event_enabling_room2[6] = false;
	map->event_enabling_room2[7] = false;
	map->event_enabling_room2[8] = false;
	map->event_enabling_room2[9] = false;
	map->event_enabling_room2[10] = false;
	map->event_enabling_room2[11] = false;
	map->event_enabling_room2[12] = false;
	map->event_enabling_room2[13] = false;
	map->event_enabling_room2[14] = false;
	map->event_enabling_room2[15] = true;
	map->event_enabling_room2[16] = false;
	map->event_enabling_room2[17] = false;
	map->event_enabling_room2[18] = false;
	map->event_enabling_room2[19] = false;
	map->event_enabling_room2[20] = false;
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
void Scene5_Loop(void) {
	bool end_sequence;
	int actor_tile_num;
	int sequence_step;
	int scene_step;

	VIDEO_ClearScreenBuffer();
	GFX_LoadPalette("PALETTES.DAT", "SCN51.PCX", 256);

	VIDEO_StringToScreenBuffer(50, 60, ui->txt_file[UI_TXT_SCN5I]->line[40], FONT_BIG_BLACK);
	VIDEO_StringToScreenBuffer(60, 85, ui->txt_file[UI_TXT_SCN5I]->line[41], FONT_BIG_BLACK);
	VIDEO_StringToScreenBuffer(90, 135, ui->txt_file[UI_TXT_SCN5I]->line[42], FONT_SLIM_WHITE);
	VIDEO_StringToScreenBuffer(160, 135, ui->txt_file[UI_TXT_SCN5I]->line[43], FONT_SLIM_WHITE);

	VIDEO_VSync();
	VIDEO_ScreenBufferToVRAM();
	VIDEO_FadeIn(4);

	SetDelayTime(6000);

	Scene5_LoadAssets();
	Scene5_SetHotspotsAndEvents();

	while (!AwaitDelayTime()) {
		// Just wait
	}

	VIDEO_FadeOut(4);

	scene_step = 0;
	sequence_step = 0;
	end_sequence = false;
	engine.ingame = true;

	MOUSE_ShowCursor();

	// Initialize the Scene room
	switch (engine.room) {
		case 1:// Room 1. ??
			Scene5_LoadRoom1();

			ACTOR_SetPosition(21 << 4, 8 << 4, ACTOR_FACING_RIGHT);// Set actor position
			ACTOR_SetCombatMode(true);

			CAM_Init(map->width_px, map->height_px, 5 << 4, 0 << 4);// Initialize camera
			MAP_DrawMapToMapVideoBuffer();

			Update(true);
			Update(true);
			VIDEO_FadeIn(1);

			TIMER_UpdateTimerTime(TIMER_AUDIO_NUMBER, 55);
			AUDIO_PlaySong(true);

			ACTOR_SetCombatMode(true);
			ACTOR_SetGun(actor->sprite_num, ACTOR_GUN_TYPE_SNIPPER);
			ACTOR_Reload();

			break;
		case 2:// Room 2. ??
			Scene5_LoadRoom2();

			ACTOR_SetPosition(2 << 4, 10 << 4, ACTOR_FACING_RIGHT); // Set actor position
			CAM_Init(map->width_px, map->height_px, 0 << 4, 0 << 4);// Initialize camera
			MAP_DrawMapToMapVideoBuffer();

			Update(true);
			Update(true);
			VIDEO_FadeIn(1);

			TIMER_UpdateTimerTime(TIMER_AUDIO_NUMBER, 55);
			AUDIO_PlaySong(true);

			break;
		default:
			Error("Scene5_Loop function error", "Undefined room", "", ERROR_SYSTEM);
			break;
	}

	// Loop until the game is over
	while (engine.ingame) {
		switch (engine.room) {
			case 1:// Room 1. Roof
				// Hotspots
				if (!actor->mode_combat) {
					// Check point on object or enemy
					switch (cursor.point_on & 0xFF) {
						case ENTITY_ID_HSPOT:// Hotspot
							switch ((cursor.point_on >> 8) & 0xFF) {
								case 0:// No hotspot, WTF??
									break;
								case 1:// Spare
									if (cursor.left_click) {
										//UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN5H, 120, 121, 122, false, 100);

									} else {
										//UI_ShowDescription(UI_TXT_SCN5H, 6);
									}
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
					case 2:// Event 2. Spare
						if (map->event_enabling_room1[2]) {
							map->event_enabling_room1[2] = false;
						}
						break;
					case 15:// Event 15. Spare
						if (map->event_enabling_room1[15]) {
							map->event_enabling_room1[15] = false;
						}
						break;
					case 16:// Event 16. ??
						if (map->event_enabling_room1[16]) {
							map->event_enabling_room1[16] = false;

							VIDEO_FadeOut(4);
							ENEMY_UnloadEnemies();
							ITEM_UnloadItems();
							OBJECT_UnloadObjects();
							EFFECT_UnloadEffects();
							BULLET_UnloadBullets();
							PARTICLE_UnloadParticles();
							GRENADE_UnloadGrenades();
							NPC_UnloadNpcs();
							MAP_UnloadMap();

							Scene5_LoadRoom2();
							ACTOR_SetPosition(2 << 4, 10 << 4, ACTOR_FACING_RIGHT); // Set actor position
							CAM_Init(map->width_px, map->height_px, 0 << 4, 6 << 4);// Initialize camera
							MAP_DrawMapToMapVideoBuffer();

							Update(true);
							Update(true);

							engine.room = 2;

							VIDEO_FadeIn(1);
						}
						break;
					default:
						break;
				}

				break;
			case 2:// Room 2. ??
				   // Hotspots
				if (!actor->mode_combat) {
					// Check point on object or enemy
					switch (cursor.point_on & 0xFF) {
						case ENTITY_ID_HSPOT:// Hotspot
							switch ((cursor.point_on >> 8) & 0xFF) {
								case 0:// No hotspot
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
					case 1:// Event 1.
						if (map->event_enabling_room2[1]) {
							map->event_enabling_room2[1] = false;
						}
						break;
					case 2:// Event 2.
						if (map->event_enabling_room2[2]) {
							map->event_enabling_room2[2] = false;
						}
						break;
					case 3:// Event 3.
						if (map->event_enabling_room2[3]) {
							map->event_enabling_room2[3] = false;
						}
						break;
					case 4:// Event 4.
						if (map->event_enabling_room2[4]) {
							map->event_enabling_room2[4] = false;
						}
						break;
					case 5:// Event 5.
						if (map->event_enabling_room2[5]) {
						}
						break;
					case 6:// Event 6.
						if (map->event_enabling_room2[6]) {
							map->event_enabling_room2[6] = false;
						}
						break;
					case 15:// Event 15. Go down
						if (map->event_enabling_room2[15]) {
							map->event_enabling_room2[15] = false;
							switch (scene_step) {
								case 0:// 6th floor to 5th floor
									VIDEO_FadeOut(4);
									map->event_enabling_room2[16] = true;
									scene_step++;
									Scene5_Update_Floor5();
									ACTOR_SetPosition(44 << 4, 11 << 4, ACTOR_FACING_UP);    // Set actor position
									CAM_Init(map->width_px, map->height_px, 30 << 4, 0 << 4);// Initialize camera
									MAP_DrawMapToMapVideoBuffer();

									Update(true);
									Update(true);
									VIDEO_FadeIn(1);
									break;
								case 2:// 4th floor to 3th floor
									VIDEO_FadeOut(4);
									map->event_enabling_room2[16] = true;
									scene_step++;
									Scene5_Update_Floor3();
									ACTOR_SetPosition(44 << 4, 11 << 4, ACTOR_FACING_UP);    // Set actor position
									CAM_Init(map->width_px, map->height_px, 30 << 4, 0 << 4);// Initialize camera
									MAP_DrawMapToMapVideoBuffer();

									Update(true);
									Update(true);
									VIDEO_FadeIn(1);
									break;
								case 4:// 2th floor to 1th floor
									VIDEO_FadeOut(4);
									map->event_enabling_room2[16] = true;
									scene_step++;
									Scene5_Update_Floor1();
									ACTOR_SetPosition(44 << 4, 11 << 4, ACTOR_FACING_UP);    // Set actor position
									CAM_Init(map->width_px, map->height_px, 30 << 4, 0 << 4);// Initialize camera
									MAP_DrawMapToMapVideoBuffer();

									Update(true);
									Update(true);
									VIDEO_FadeIn(1);
									break;
								default:
									break;
							}
						}
						break;
					case 16:// Event 16. Go down
						if (map->event_enabling_room2[16]) {
							map->event_enabling_room2[16] = false;
							switch (scene_step) {
								case 1:// 5th floor to 4th floor
									VIDEO_FadeOut(4);
									map->event_enabling_room2[15] = true;
									scene_step++;
									Scene5_Update_Floor4();
									ACTOR_SetPosition(2 << 4, 10 << 4, ACTOR_FACING_RIGHT); // Set actor position
									CAM_Init(map->width_px, map->height_px, 0 << 4, 6 << 4);// Initialize camera
									MAP_DrawMapToMapVideoBuffer();

									Update(true);
									Update(true);
									VIDEO_FadeIn(1);
									break;
								case 3:// 3th floor to 2th floor
									VIDEO_FadeOut(4);
									map->event_enabling_room2[15] = true;
									scene_step++;
									Scene5_Update_Floor2();
									ACTOR_SetPosition(2 << 4, 10 << 4, ACTOR_FACING_RIGHT); // Set actor position
									CAM_Init(map->width_px, map->height_px, 0 << 4, 6 << 4);// Initialize camera
									MAP_DrawMapToMapVideoBuffer();

									Update(true);
									Update(true);
									VIDEO_FadeIn(1);
									break;
								case 5:// 1th floor to street. End of scene
									VIDEO_FadeOut(4);
									engine.ingame = false;
									engine.scene = 6;
									engine.room = 1;
									break;
								default:
									break;
							}
						}
						break;
					default:
						break;
				}
				break;
		}

		Update(true);

		// Finish all if actor is dead
		if (actor->status_dead) Scene5_GameOver();
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
	ACTOR_UnloadActor();
	MAP_UnloadMap();
	Scene5_UnloadAssets();
}
void Scene5_Outro(void) {
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
			case 1:// Load first scene
				GFX_PCXImageToBuffer("OSCN5.DAT", "OUTRO1H.PCX", 104 * 140, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("OSCN5.DAT", "OUTRO1L.PCX", 104 * 140, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				GFX_PCXImageToBuffer("OSCN5.DAT", "OUTRO2H.PCX", 104 * 140, gfx->image_buffer3, &gfx->image_buffer3_width, &gfx->image_buffer3_height);
				AUDIO_LoadSong(AUDIO_SONG_3);
				step++;
				break;
			case 2:// Hide loading screen
				UI_HideLoadingScreen();
				step++;
				break;
			case 3:///
				VIDEO_ClearScreenBuffer();
				GFX_LoadPalette("PALETTES.DAT", "OSCN5.PCX", 256);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				VIDEO_FadeIn(4);

				TIMER_UpdateTimerTime(TIMER_AUDIO_NUMBER, 50);
				AUDIO_PlaySong(true);

				step++;
				break;
			case 4:// First image
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 110, 146, 30, 50, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 33, 53);
				step++;
				break;
			case 5:// Just wait
				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 6://Speech
				   // Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 296, 46, 5, 1, 239);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 40, 8, 4, 208);
				VIDEO_StringToScreenBuffer(12, 12, ui->txt_file[UI_TXT_SCN5I]->line[20], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(12, 22, ui->txt_file[UI_TXT_SCN5I]->line[21], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(6000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 7:// Hide speech
				   // Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 296, 46, 5, 1, 239);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 40, 8, 4, 208);
				step++;
				break;
			case 8:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 9://Speech
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 296, 46, 5, 1, 239);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 40, 8, 4, 208);
				VIDEO_StringToScreenBuffer(12, 12, ui->txt_file[UI_TXT_SCN5I]->line[22], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(12, 22, ui->txt_file[UI_TXT_SCN5I]->line[23], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(6000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 10:// Show first and Second image image
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 110, 146, 30, 50, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 33, 53);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 110, 146, 180, 50, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 183, 53);
				step++;
				break;
			case 11:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 12:// Clear chat Fade first and show Second image
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 296, 46, 5, 1, 239);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 40, 8, 4, 208);

				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 110, 146, 30, 50, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 53);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 110, 146, 180, 50, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 183, 53);
				step++;
				break;
			case 13:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 14://Speech
				// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 296, 46, 5, 1, 239);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 40, 8, 4, 208);
				VIDEO_StringToScreenBuffer(12, 12, ui->txt_file[UI_TXT_SCN5I]->line[24], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(12, 22, ui->txt_file[UI_TXT_SCN5I]->line[25], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(6000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 15://Fade out
				VIDEO_FadeOut(4);
				AUDIO_StopSong();
				step++;
				break;
			case 16:// Load images 3 and 4
				GFX_PCXImageToBuffer("OSCN5.DAT", "OUTRO3H.PCX", 250 * 135, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("OSCN5.DAT", "OUTRO4H.PCX", 251 * 140, gfx->image_buffer3, &gfx->image_buffer3_width, &gfx->image_buffer3_height);
				step++;
				break;
			case 17:/// Image 3 :: back home
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 211, 141, 10, 40, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, 205, gfx->image_buffer1_height, 13, 43);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				VIDEO_FadeIn(4);

				TIMER_UpdateTimerTime(TIMER_AUDIO_NUMBER, 50);
				AUDIO_PlaySong(true);

				step++;
				break;
			case 18:// image 3 scroll
				horizontal_scroll = 0;
				dialog_step = 0;
				scroll_end = false;

				while (!scroll_end & engine.sequence) {
					src_index = horizontal_scroll;
					dst_index = CAM_VISIBLE_WIDTH * 43 + 13;

					// Draw image with scroll
					for (i = 0; i < gfx->image_buffer1_height; i++) {
						memcpy(&video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index], &gfx->image_buffer1[src_index], 205);
						src_index += gfx->image_buffer1_width;
						dst_index += CAM_VISIBLE_WIDTH;
					}
					if (horizontal_scroll < 45) horizontal_scroll++;
					else
						scroll_end = true;

					// Draw text background
					VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 242);
					VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 250);

					VIDEO_StringToScreenBuffer(28, 13, ui->txt_file[UI_TXT_SCN5I]->line[30], FONT_SLIM_BLACK);
					VIDEO_StringToScreenBuffer(28, 25, ui->txt_file[UI_TXT_SCN5I]->line[31], FONT_SLIM_BLACK);

					SetDelayTime(50);
					while (!AwaitDelayTime()) {
						// Just wait
					}

					VIDEO_VSync();
					VIDEO_ScreenBufferToVRAM();
				}

				step++;
				break;
			case 19:// Just wait
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 20://Image 2

				// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 242);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 250);

				VIDEO_StringToScreenBuffer(28, 13, ui->txt_file[UI_TXT_SCN5I]->line[32], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(28, 25, ui->txt_file[UI_TXT_SCN5I]->line[33], FONT_SLIM_BLACK);

				step++;
				break;
			case 21:// Just wait
				SetDelayTime(6000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 22:// Show image 3
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 257, 146, 20, 45, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 23, 48);
				step++;
				break;
			case 23:
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 24://Speech
				// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 242);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 250);

				VIDEO_StringToScreenBuffer(28, 13, ui->txt_file[UI_TXT_SCN5I]->line[34], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(28, 25, ui->txt_file[UI_TXT_SCN5I]->line[35], FONT_SLIM_BLACK);

				step++;
				break;
			case 25:// Just wait
				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 26:// Load images 5 and 6
				GFX_PCXImageToBuffer("OSCN5.DAT", "OUTRO5H.PCX", 150 * 160, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("OSCN5.DAT", "OUTRO5L.PCX", 150 * 160, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				GFX_PCXImageToBuffer("OSCN5.DAT", "OUTRO6H.PCX", 140 * 130, gfx->image_buffer3, &gfx->image_buffer3_width, &gfx->image_buffer3_height);
				step++;
				break;
			case 27:// show image 5
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 156, 166, 10, 30, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 13, 33);
				step++;
				break;
			case 28:// Just wait
				SetDelayTime(3000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 29:// show image 6
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 156, 166, 10, 30, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 13, 33);

				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 136, 150, 50, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 153, 53);
				step++;
				break;
			case 30:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 31:// fade image 5
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 156, 166, 10, 30, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 13, 33);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 136, 150, 50, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 153, 53);

				step++;
				break;
			case 32:// Just wait
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 33:// Load images 5 and 6
				GFX_PCXImageToBuffer("OSCN5.DAT", "OUTRO7H.PCX", 110 * 150, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("OSCN5.DAT", "OUTRO7L.PCX", 110 * 150, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				GFX_PCXImageToBuffer("OSCN5.DAT", "OUTRO8H.PCX", 110 * 140, gfx->image_buffer3, &gfx->image_buffer3_width, &gfx->image_buffer3_height);
				step++;
				break;
			case 34:// show image 7
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 116, 156, 10, 30, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 13, 33);
				step++;
				break;
			case 35:// Just wait
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 36:// show image 8
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 116, 156, 10, 30, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 13, 33);

				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 116, 146, 160, 35, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 163, 38);
				step++;
				break;
			case 37:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 38:// fade image 7 and show image 8
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 116, 156, 10, 30, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 13, 33);

				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 116, 146, 160, 35, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 163, 38);
				step++;
				break;
			case 39:// Just wait
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 40:// Load image 9
				GFX_PCXImageToBuffer("OSCN5.DAT", "OUTRO9H.PCX", 110 * 140, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				step++;
				break;
			case 41:// fade image 8 and show image 9
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 116, 156, 10, 30, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 13, 33);

				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 116, 146, 160, 35, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 163, 38);
				step++;
				break;
			case 42:// Just wait
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 43:// Load image 10 and 11
				GFX_PCXImageToBuffer("OSCN5.DAT", "OUTRO10H.PCX", 220 * 90, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("OSCN5.DAT", "OUTRO11H.PCX", 220 * 90, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				step++;
				break;
			case 44:// show image 10
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 226, 96, 30, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 33, 13);
				step++;
				break;
			case 45:// Just wait
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 46:// show image 10 and 11
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 226, 96, 30, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 33, 13);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 226, 96, 50, 80, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 53, 83);
				step++;
				break;
			case 47:// Just wait
				SetDelayTime(6000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 48://End sequence
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
