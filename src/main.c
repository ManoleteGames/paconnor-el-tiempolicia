/**********************************************
* main.c
* Entry point
* (c) 2025-2026 by J.Martin
***********************************************/

#include "engine/audio/audio.h"
#include "engine/engine.h"
#include "engine/file/file.h"
#include "engine/gfx/gfx.h"
#include "engine/keyb/keyb.h"
#include "engine/types/types.h"
#include "engine/ui/ui.h"
#include "scene1.h"
#include "scene2.h"
#include "scene3.h"
#include "scene4.h"
#include "scene5.h"
#include "scene6.h"

#define GDB_IMPLEMENTATION
#include "gdbstub.h"

/** Load language text files
 */
static void LoadTexts(void) {
	char language[13];
	// Load game texts
	switch (ui->language) {
		case 0:// ES
			sprintf(language, "DIALOGES.DAT");
			break;
		case 1:// EN
			sprintf(language, "DIALOGEN.DAT");
			break;
		case 2:// FR
			sprintf(language, "DIALOGFR.DAT");
			break;
		case 3:// GR
			sprintf(language, "DIALOGGR.DAT");
			break;
		default:
			sprintf(engine.system_error_message1, "LoadTexts function error");
			sprintf(engine.system_error_message2, "Unknown language %u", ui->language);
			sprintf(engine.system_error_message3, " ");
			Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
			break;
	}

	// Clear dialogs buffers
	FILE_LoadTextFile(language, "INTRO.TXT", ui->txt_file[UI_TXT_INTRO]);
	FILE_LoadTextFile(language, "GLOBAL.TXT", ui->txt_file[UI_TXT_GLOBAL]);
	FILE_LoadTextFile(language, "CREDITS.TXT", ui->txt_file[UI_TXT_CREDITS]);
	FILE_LoadTextFile(language, "SCN1D.TXT", ui->txt_file[UI_TXT_SCN1D]);
	FILE_LoadTextFile(language, "SCN1H.TXT", ui->txt_file[UI_TXT_SCN1H]);
	FILE_LoadTextFile(language, "SCN1I.TXT", ui->txt_file[UI_TXT_SCN1I]);
	FILE_LoadTextFile(language, "SCN2D.TXT", ui->txt_file[UI_TXT_SCN2D]);
	FILE_LoadTextFile(language, "SCN2H.TXT", ui->txt_file[UI_TXT_SCN2H]);
	FILE_LoadTextFile(language, "SCN2I.TXT", ui->txt_file[UI_TXT_SCN2I]);
	FILE_LoadTextFile(language, "SCN3D.TXT", ui->txt_file[UI_TXT_SCN3D]);
	FILE_LoadTextFile(language, "SCN3H.TXT", ui->txt_file[UI_TXT_SCN3H]);
	FILE_LoadTextFile(language, "SCN3I.TXT", ui->txt_file[UI_TXT_SCN3I]);
	FILE_LoadTextFile(language, "SCN4D.TXT", ui->txt_file[UI_TXT_SCN4D]);
	FILE_LoadTextFile(language, "SCN4H.TXT", ui->txt_file[UI_TXT_SCN4H]);
	FILE_LoadTextFile(language, "SCN4I.TXT", ui->txt_file[UI_TXT_SCN4I]);
	FILE_LoadTextFile(language, "SCN5D.TXT", ui->txt_file[UI_TXT_SCN5D]);
	FILE_LoadTextFile(language, "SCN5H.TXT", ui->txt_file[UI_TXT_SCN5H]);
	FILE_LoadTextFile(language, "SCN5I.TXT", ui->txt_file[UI_TXT_SCN5I]);
	FILE_LoadTextFile(language, "SCN6D.TXT", ui->txt_file[UI_TXT_SCN6D]);
	FILE_LoadTextFile(language, "SCN6H.TXT", ui->txt_file[UI_TXT_SCN6H]);
	FILE_LoadTextFile(language, "SCN6I.TXT", ui->txt_file[UI_TXT_SCN6I]);
	FILE_LoadTextFile(language, "END.TXT", ui->txt_file[UI_TXT_END]);
}
/** Main logo sequence
 */
static void Logo(void) {
	bool spark_end;
	int logo_step;
	int spr_num;
	int spark_counter;
	int sprite_slot;
	int char_counter, aux_counter;

	logo_step = 0;

	// shares logo sequence status is running
	engine.logo = true;

	while (engine.logo) {
		switch (logo_step) {
			case 0:// Logo MS-DOS Club
				VIDEO_ClearScreen();
				VIDEO_ClearPalette();
				GFX_LoadPalette("PALETTES.DAT", "LDOSCLUB.PCX", 256);
				AUDIO_LoadSong(AUDIO_SONG_6);// Load song
				GFX_PCXImageToBuffer("ILOGO.DAT", "LDOSCLUB.PCX", 180 * 135, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 70, 32);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				VIDEO_FadeIn_Async(1);
				VIDEO_RotatePaletteStart_Async(48, 79, 98);

				logo_step = 11;
				break;
			case 11:// Set delay time
				TIMER_UpdateTimerTime(TIMER_AUDIO_NUMBER, 10);
				AUDIO_PlaySong(false);
				logo_step = 12;
				break;
			case 12:// Load some assets
				GFX_LoadSpriteGraphicsRLE("MISC.DAT", "IWORMS.PCX", SPRITE_GRAPHICS_ID_WORMS, 192, 24, 14, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_TEMPORARY_SPRITE);
				logo_step = 20;
				break;
			case 20:// Load new image on second buffer
				GFX_PCXImageToBuffer("ILOGO.DAT", "LCONTEST.PCX", 320 * 100, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				sprite_slot = GFX_FindEmptySpriteSlot();
				if (sprite_slot == -1) {
					sprintf(engine.system_error_message1, "Logo function error");
					sprintf(engine.system_error_message2, "No empty sprite slot available");
					sprintf(engine.system_error_message3, " ");
					Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
				} else {
					spr_num = sprite_slot;
					GFX_InitSprite(ENTITY_ID_EMPTY, 0, spr_num, 0, 192, 24);
				}

				GFX_SetSpriteGraphic(spr_num, 0, SPRITE_GRAPHICS_ID_WORMS, 0, 0);
				GFX_SetSpritePosition(spr_num, 63, 108);
				GFX_SetDefaultAnimation(spr_num, false, false, 8);
				logo_step = 21;
				break;
			case 21:
				logo_step = 22;
				break;
			case 22:// wait song ended
				if (AUDIO_IsSongEnded()) {
					logo_step = 23;
					VIDEO_RotatePaletteEnd_Async();
					VIDEO_FadeOut(1);
				}
				break;
			case 23:// Print new logo
				spr_num = 0;
				VIDEO_ClearScreen();
				VIDEO_ClearPalette();
				GFX_LoadPalette("PALETTES.DAT", "LCONTEST.PCX", 256);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 0, 50);
				VIDEO_FadeIn(1);
				logo_step = 24;
				break;
			case 24:// Draw logo and effects until delay is finished
				while (!GFX_IsSpriteAnimationEnded(spr_num, 0)) {
					VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 0, 50);
					GFX_UpdateSprites();
					if (gfx_sprite_stack[spr_num].gfx[0].anim_current_frame == 2) AUDIO_PlaySound(AUDIO_JUMP_EFFECT, 1);
					if (gfx_sprite_stack[spr_num].gfx[0].anim_current_frame == 12) AUDIO_PlaySound(AUDIO_CRASH_EFFECT, 1);
					GFX_DrawSprites();
					VIDEO_VSync();
					VIDEO_ScreenBufferToVRAM();
				}
				logo_step = 25;
				break;
			case 25:
				logo_step = 26;
				break;
			case 26:// Unload sprites
				GFX_UnloadSprites();
				logo_step = 30;
				break;
			case 30:// Load new image and fade out
				GFX_PCXImageToBuffer("ILOGO.DAT", "LMANOL.PCX", 220 * 165, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				VIDEO_FadeOut(1);
				logo_step = 31;
				break;
			case 31:// Load sprites
				VIDEO_ClearScreen();
				VIDEO_ClearPalette();
				GFX_LoadPalette("PALETTES.DAT", "LMANOL.PCX", 256);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 50, 18);
				VIDEO_FadeIn(1);
				logo_step = 32;
				break;
			case 32:// Draw image and effects
				spark_counter = 0;
				spark_end = false;
				while (!spark_end) {
					VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 50, 18);
					switch (spark_counter) {
						case 0:
							spark_counter++;
							break;
						case 1:
							EFFECT_LoadEffect(ENTITY_ID_EMPTY, SPRITE_GRAPHICS_ID_SPARK_EFFECT, 65, 130, true, 0, false, false, 7);
							AUDIO_PlaySound(AUDIO_TICK_EFFECT, 1);
							spark_counter++;
							break;
						case 2:
							if (EFFECT_NoEffectsActive()) spark_counter++;
							break;
						case 3:
							spark_counter++;
							break;
						case 4:
							EFFECT_LoadEffect(ENTITY_ID_EMPTY, SPRITE_GRAPHICS_ID_SPARK_EFFECT, 100, 128, true, 0, false, false, 7);
							AUDIO_PlaySound(AUDIO_TICK_EFFECT, 1);
							spark_counter++;
							break;
						case 5:
							if (EFFECT_NoEffectsActive()) spark_counter++;
							break;
						case 6:
							spark_counter++;
							break;
						case 7:
							EFFECT_LoadEffect(ENTITY_ID_EMPTY, SPRITE_GRAPHICS_ID_SPARK_EFFECT, 128, 130, true, 0, false, false, 7);
							AUDIO_PlaySound(AUDIO_TICK_EFFECT, 1);
							spark_counter++;
							break;
						case 8:
							if (EFFECT_NoEffectsActive()) spark_counter++;
							break;
						case 9:
							spark_counter++;
							break;
						case 10:
							EFFECT_LoadEffect(ENTITY_ID_EMPTY, SPRITE_GRAPHICS_ID_SPARK_EFFECT, 175, 130, true, 0, false, false, 7);
							AUDIO_PlaySound(AUDIO_TICK_EFFECT, 1);
							spark_counter++;
							break;
						case 11:
							if (EFFECT_NoEffectsActive()) spark_counter++;
							break;
						case 12:
							spark_counter++;
							break;
						case 13:
							EFFECT_LoadEffect(ENTITY_ID_EMPTY, SPRITE_GRAPHICS_ID_SPARK_EFFECT, 210, 128, true, 0, false, false, 7);
							AUDIO_PlaySound(AUDIO_TICK_EFFECT, 1);
							spark_counter++;
							break;
						case 14:
							if (EFFECT_NoEffectsActive()) spark_counter++;
							break;
						case 15:
							aux_counter = 0;
							spark_counter++;
							break;
						case 16:
							SetDelayTime(40);
							while (!AwaitDelayTime()) {
								// Just wait
							}
							VIDEO_ClearScreenBuffer();
							VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 50, 18);
							if (VIDEO_StringToScreenBufferSteps(82, 175, ui->txt_file[UI_TXT_INTRO]->line[1], FONT_BIG_WHITE, &aux_counter)) {
								spark_counter++;
							}
							AUDIO_PlaySound(AUDIO_TICK_EFFECT, 1);
							break;
						case 17:
							VIDEO_StringToScreenBuffer(82, 175, ui->txt_file[UI_TXT_INTRO]->line[1], FONT_BIG_WHITE);
							AUDIO_PlaySound(AUDIO_CRASH_EFFECT, 1);
							spark_end = true;
							break;
						default:
							Error("Logo function error", "Unknown logo step", "", ERROR_SYSTEM);
							break;
					}

					EFFECT_UpdateEffects();
					GFX_UpdateSprites();
					GFX_DrawSprites();
					VIDEO_VSync();
					VIDEO_ScreenBufferToVRAM();
				}
				logo_step = 40;
				break;
			case 40:
				GFX_PCXImageToBuffer("ILOGO.DAT", "JOY1.PCX", 120 * 160, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("ILOGO.DAT", "JOY2.PCX", 120 * 160, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				GFX_PCXImageToBuffer("ILOGO.DAT", "JOY3.PCX", 120 * 160, gfx->image_buffer3, &gfx->image_buffer3_width, &gfx->image_buffer3_height);
				VIDEO_FadeOut(1);
				logo_step = 41;
				break;
			case 41:
				VIDEO_ClearScreen();
				VIDEO_ClearPalette();
				GFX_LoadPalette("PALETTES.DAT", "LMANOL.PCX", 256);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 100, 50);
				VIDEO_FadeIn(1);
				SetDelayTime(1000);
				logo_step = 42;
				break;
			case 42:
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 100, 50);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				if (AwaitDelayTime()) {
					logo_step = 43;
				}
				break;
			case 43:
				SetDelayTime(500);
				logo_step = 44;
				break;
			case 44:
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 100, 50);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				if (AwaitDelayTime()) {
					logo_step = 45;
				}
				break;
			case 45:
				SetDelayTime(500);
				logo_step = 46;
				break;
			case 46:
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 100, 50);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				if (AwaitDelayTime()) {
					logo_step = 47;
				}
				break;
			case 47:
				SetDelayTime(500);
				logo_step = 48;
				break;
			case 48:
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 100, 50);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				if (AwaitDelayTime()) {
					logo_step = 49;
				}
				break;
			case 49:
				SetDelayTime(500);
				logo_step = 50;
				break;
			case 50:
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 100, 50);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				if (AwaitDelayTime()) {
					logo_step = 51;
				}
				break;
			case 51:
				SetDelayTime(500);
				logo_step = 52;
				break;
			case 52:
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 100, 50);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				char_counter = 0;
				aux_counter = 0;
				if (AwaitDelayTime()) {
					logo_step = 53;
				}
				break;
			case 53:
				SetDelayTime(500);
				logo_step = 54;
				break;
			case 54:
				VIDEO_ClearScreenBuffer();
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 100, 50);

				if (VIDEO_StringToScreenBufferRand(40, 15, ui->txt_file[UI_TXT_INTRO]->line[3], FONT_BIG_WHITE, &char_counter, &aux_counter)) {
					aux_counter = 0;
					logo_step = 55;
				}
				AUDIO_PlaySound(AUDIO_TICK_EFFECT, 1);

				SetDelayTime(40);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				break;
			case 55:
				SetDelayTime(80);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				VIDEO_ClearScreenBuffer();
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 100, 50);
				VIDEO_StringToScreenBuffer(40, 15, ui->txt_file[UI_TXT_INTRO]->line[3], FONT_BIG_WHITE);
				if (VIDEO_StringToScreenBufferSteps(80, 30, ui->txt_file[UI_TXT_INTRO]->line[4], FONT_BIG_WHITE, &aux_counter)) {
					aux_counter = 0;
					logo_step = 56;
				}
				AUDIO_PlaySound(AUDIO_TICK_EFFECT, 1);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				break;
			case 56:
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 100, 50);
				VIDEO_StringToScreenBuffer(40, 15, ui->txt_file[UI_TXT_INTRO]->line[3], FONT_BIG_WHITE);
				VIDEO_StringToScreenBuffer(80, 30, ui->txt_file[UI_TXT_INTRO]->line[4], FONT_BIG_WHITE);
				AUDIO_PlaySound(AUDIO_CRASH_EFFECT, 1);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(2000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				logo_step = 57;
				break;
			case 57:
				engine.logo = false;
				break;
			default:
				engine.logo = false;
				break;
		}
	}

	AUDIO_StopSong();
	VIDEO_RotatePaletteEnd_Async();
	SetDelayTime(0);
	VIDEO_FadeOut(1);
	GFX_UnloadSprites();
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_WORMS);
	MM_PopChunks(CT_TEMPORARY_SPRITE);

	// shares logo sequence is ended
	engine.logo = false;
}
static void Intro(void) {
	int step;
	int i, scroll_counter;
	int horizontal_scroll2, max_scroll2, src_index2, dst_index2;
	int horizontal_scroll3, max_scroll3, src_index3, dst_index3;
	int actor_spr_num;
	int speech_step;
	bool scroll_end;

	engine.sequence = true;
	step = 0;
	while (engine.sequence) {
		switch (step) {
			case 0:// Show screen and play music

				AUDIO_LoadSong(AUDIO_SONG_1);
				GFX_PCXImageToBuffer("INTRO.DAT", "INTRO1.PCX", 640 * 32, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("INTRO.DAT", "INTRO2.PCX", 640 * 96, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);

				VIDEO_ClearScreenBuffer();
				TIMER_UpdateTimerTime(TIMER_AUDIO_NUMBER, 50);
				AUDIO_PlaySong(true);
				GFX_LoadPalette("PALETTES.DAT", "INTRO.PCX", 256);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 230, 168, 45, 12, 222);

				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 224, 32, 48, 16, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 218, 26, 51, 19, 208);

				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, 224, gfx->image_buffer1_height, 48, 48);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, 224, gfx->image_buffer2_height, 48, 80);
				VIDEO_StringToScreenBuffer(40, 185, ui->txt_file[UI_TXT_INTRO]->line[25], FONT_SLIM_WHITE);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				VIDEO_FadeIn(4);
				step++;
				break;
			case 1:// load small actor
				actor_spr_num = GFX_FindEmptySpriteSlot();
				if (actor_spr_num == -1) {
					sprintf(engine.system_error_message1, "Intro function error");
					sprintf(engine.system_error_message2, "No empty sprite slot available");
					sprintf(engine.system_error_message3, " ");
					Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
				} else {
					GFX_InitSprite(ENTITY_ID_EMPTY, 0, actor_spr_num, 0, 20, 33);
				}
				GFX_SetSpriteGraphic(actor_spr_num, 0, SPRITE_GRAPHICS_ID_ACTOR_INTRO, 0, 0);
				GFX_SetDefaultAnimation(actor_spr_num, false, true, 10);
				GFX_SetSpritePosition(actor_spr_num, 48, 142);
				speech_step = 0;
				scroll_counter = 0;
				step++;
				break;
			case 2:// actor walk to middle screen
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 224, 32, 48, 16, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 218, 26, 51, 19, 208);

				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, 224, gfx->image_buffer1_height, 48, 48);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, 224, gfx->image_buffer2_height, 48, 80);
				scroll_counter++;
				if (scroll_counter >= 5) {
					GFX_SetSpritePosition(actor_spr_num, GFX_GetSpriteScreenPosX(actor_spr_num) + 1, GFX_GetSpriteScreenPosY(actor_spr_num));
					scroll_counter = 0;
				}

				// Rain effect
				if (effects_counter < 20) EFFECT_LoadEffect(ENTITY_ID_EMPTY, SPRITE_GRAPHICS_ID_RAIN_EFFECT, 48 + rand() % 219, 50 + rand() % 110, true, 0, false, false, 1);

				// next speech
				if (GFX_GetSpriteScreenPosX(actor_spr_num) == 52) {
					speech_step = 1;
				}

				// next speech
				if (GFX_GetSpriteScreenPosX(actor_spr_num) == 110) {
					speech_step = 2;
				}

				// next speech
				if (GFX_GetSpriteScreenPosX(actor_spr_num) == 115) {
					speech_step = 3;
				}

				// end of step
				if (GFX_GetSpriteScreenPosX(actor_spr_num) == 160) {
					src_index2 = 0;
					horizontal_scroll2 = 0;
					max_scroll2 = gfx->image_buffer1_width - 224;
					src_index3 = 0;
					horizontal_scroll3 = 0;
					max_scroll3 = gfx->image_buffer2_width - 224;
					scroll_end = false;
					scroll_counter = 0;
					step++;
				}
				break;
			case 3:// Start scrolling
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 224, 32, 48, 16, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 218, 26, 51, 19, 208);

				if (scroll_counter % 32 == 0) {
					// Draw buffer 2. slow scroll
					src_index2 = horizontal_scroll2;
					dst_index2 = 48 * CAM_VISIBLE_WIDTH + 48;
					for (i = 0; i < gfx->image_buffer1_height; i++) {
						memcpy(&video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index2], &gfx->image_buffer1[src_index2], 224);
						src_index2 += gfx->image_buffer1_width;
						dst_index2 += CAM_VISIBLE_WIDTH;
					}
					horizontal_scroll2 += 1;
				}
				if (horizontal_scroll2 >= max_scroll2) scroll_end = true;


				if (scroll_counter % 8 == 0) {

					// Draw buffer 3. fast scroll
					src_index3 = horizontal_scroll3;
					dst_index3 = 80 * CAM_VISIBLE_WIDTH + 48;
					for (i = 0; i < gfx->image_buffer2_height; i++) {
						memcpy(&video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index3], &gfx->image_buffer2[src_index3], 224);
						src_index3 += gfx->image_buffer2_width;
						dst_index3 += CAM_VISIBLE_WIDTH;
					}
					horizontal_scroll3 += 1;
				}
				if (horizontal_scroll3 >= max_scroll3) scroll_end = true;

				if (horizontal_scroll3 == 1) {
					speech_step = 4;
				}

				if (horizontal_scroll3 == 6) {
					speech_step = 5;
				}

				if (horizontal_scroll3 == 65) {
					speech_step = 6;
				}

				if (horizontal_scroll3 == 70) {
					speech_step = 7;
				}

				if (horizontal_scroll3 == 120) {
					speech_step = 8;
				}

				if (horizontal_scroll3 == 125) {
					speech_step = 9;
				}

				if (horizontal_scroll3 == 185) {
					speech_step = 10;
				}

				if (horizontal_scroll3 == 190) {
					speech_step = 11;
				}

				if (horizontal_scroll3 == 260) {
					speech_step = 12;
				}

				if (horizontal_scroll3 == 265) {
					speech_step = 13;
				}

				if (horizontal_scroll3 == 325) {
					speech_step = 14;
				}

				if (horizontal_scroll3 == 330) {
					speech_step = 15;
				}

				if (horizontal_scroll3 == 400) {
					speech_step = 16;
				}

				if (horizontal_scroll3 == 405) {
					speech_step = 17;
				}

				// increase scroll counter
				scroll_counter++;

				if (effects_counter < 20) EFFECT_LoadEffect(ENTITY_ID_EMPTY, SPRITE_GRAPHICS_ID_RAIN_EFFECT, 48 + rand() % 219, 50 + rand() % 110, true, 0, false, false, 1);

				if (scroll_end) step++;
				break;
			case 4:
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 224, 32, 48, 16, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 218, 26, 51, 19, 208);

				src_index2 = horizontal_scroll2;
				dst_index2 = 48 * CAM_VISIBLE_WIDTH + 48;
				for (i = 0; i < gfx->image_buffer1_height; i++) {
					memcpy(&video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index2], &gfx->image_buffer1[src_index2], 224);
					src_index2 += gfx->image_buffer1_width;
					dst_index2 += CAM_VISIBLE_WIDTH;
				}
				src_index3 = horizontal_scroll3;
				dst_index3 = 80 * CAM_VISIBLE_WIDTH + 48;
				for (i = 0; i < gfx->image_buffer2_height; i++) {
					memcpy(&video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index3], &gfx->image_buffer2[src_index3], 224);
					src_index3 += gfx->image_buffer2_width;
					dst_index3 += CAM_VISIBLE_WIDTH;
				}
				scroll_counter++;
				if (scroll_counter >= 5) {
					GFX_SetSpritePosition(actor_spr_num, GFX_GetSpriteScreenPosX(actor_spr_num) + 1, GFX_GetSpriteScreenPosY(actor_spr_num));
					scroll_counter = 0;
				}

				// Rain effect
				if (effects_counter < 20) EFFECT_LoadEffect(ENTITY_ID_EMPTY, SPRITE_GRAPHICS_ID_RAIN_EFFECT, 48 + rand() % 219, 50 + rand() % 110, true, 0, false, false, 1);

				// end of step
				if (GFX_GetSpriteScreenPosX(actor_spr_num) == 250) {
					step++;
				}

				break;
			case 5:
				engine.sequence = false;
				break;
			default:
				engine.sequence = false;
				break;
		}

		switch (speech_step) {
			case 0:// no speech
				break;
			case 1:// speech 1
				VIDEO_StringToScreenBuffer(52, 22, ui->txt_file[UI_TXT_INTRO]->line[5], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(52, 30, ui->txt_file[UI_TXT_INTRO]->line[6], FONT_SLIM_BLACK);
				break;
			case 2:// silence
				break;
			case 3:// speech 1
				VIDEO_StringToScreenBuffer(52, 22, ui->txt_file[UI_TXT_INTRO]->line[7], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(52, 30, ui->txt_file[UI_TXT_INTRO]->line[8], FONT_SLIM_BLACK);
				break;
			case 4:// silence
				break;
			case 5:// speech 1
				VIDEO_StringToScreenBuffer(52, 22, ui->txt_file[UI_TXT_INTRO]->line[9], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(52, 30, ui->txt_file[UI_TXT_INTRO]->line[10], FONT_SLIM_BLACK);
				break;
			case 6:// silence
				break;
			case 7:// speech 1
				VIDEO_StringToScreenBuffer(52, 22, ui->txt_file[UI_TXT_INTRO]->line[11], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(52, 30, ui->txt_file[UI_TXT_INTRO]->line[12], FONT_SLIM_BLACK);
				break;
			case 8:// silence
				break;
			case 9:// speech 1
				VIDEO_StringToScreenBuffer(52, 22, ui->txt_file[UI_TXT_INTRO]->line[13], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(52, 30, ui->txt_file[UI_TXT_INTRO]->line[14], FONT_SLIM_BLACK);
				break;
			case 10:// silence
				break;
			case 11:// speech 1
				VIDEO_StringToScreenBuffer(52, 22, ui->txt_file[UI_TXT_INTRO]->line[15], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(52, 30, ui->txt_file[UI_TXT_INTRO]->line[16], FONT_SLIM_BLACK);
				break;
			case 12:// silence
				break;
			case 13:// speech 1
				VIDEO_StringToScreenBuffer(52, 22, ui->txt_file[UI_TXT_INTRO]->line[17], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(52, 30, ui->txt_file[UI_TXT_INTRO]->line[18], FONT_SLIM_BLACK);
				break;
			case 14:// silence
				break;
			case 15:// speech 1
				VIDEO_StringToScreenBuffer(52, 22, ui->txt_file[UI_TXT_INTRO]->line[19], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(52, 30, ui->txt_file[UI_TXT_INTRO]->line[20], FONT_SLIM_BLACK);
				break;
			case 16:
				break;
			case 17:// speech 1
				VIDEO_StringToScreenBuffer(52, 22, ui->txt_file[UI_TXT_INTRO]->line[21], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(52, 30, ui->txt_file[UI_TXT_INTRO]->line[22], FONT_SLIM_BLACK);
				break;
			case 18:
				break;
			default:
				break;
		}

		EFFECT_UpdateEffects();
		GFX_UpdateSprites();
		GFX_DrawSprites();
		VIDEO_VSync();
		VIDEO_ScreenBufferToVRAM();
	}

	VIDEO_FadeOut(4);
	AUDIO_StopSong();

	AUDIO_UnloadSong();
	EFFECT_UnloadEffects();
	GFX_UnloadSprites();
}
/** Main menu
 */
static void Menu(void) {

	bool exit_menu;
	bool show_main_menu, hide_main_menu, main_menu_shown, main_menu_hidden;
	bool show_options_menu, hide_options_menu, options_menu_shown, options_menu_hidden;
	bool show_password_menu, hide_password_menu, password_menu_shown, password_menu_hidden;
	bool show_credits, hide_credits, credits_shown, credits_hidden;
	bool redefine_keys;
	int credits_step;
	int redefine_step;
	int i;
	int title_spr_num1, title_spr_num2, opt_pnl_spr_num, pass_pnl_spr_num, chat_pnl_spr_num;
	unsigned char sound_volume[5];
	unsigned char music_volume[5];

	int idle_counter;

	char password[13];
	byte password_step;
	byte key;

	exit_menu = false;

	show_main_menu = false;
	hide_main_menu = false;
	main_menu_hidden = false;
	main_menu_shown = true;

	show_options_menu = false;
	hide_options_menu = false;
	options_menu_hidden = true;
	options_menu_shown = false;

	show_password_menu = false;
	hide_password_menu = false;
	password_menu_hidden = true;
	password_menu_shown = false;

	show_credits = false;
	hide_credits = false;
	credits_shown = false;
	credits_hidden = true;
	credits_step = 0;
	redefine_step = 0;

	int char_number = 0;
	char aux_string[40];

	VIDEO_ClearScreen();

	GFX_LoadPalette("PALETTES.DAT", "MENU.PCX", 256);
	CAM_SetPosition(0, 0);

	AUDIO_LoadSong(AUDIO_SONG_2);

	TIMER_UpdateTimerTime(TIMER_AUDIO_NUMBER, 50);
	AUDIO_PlaySong(true);

	VIDEO_FadeIn(4);

	// ---- Title scroll in the middle screen
	title_spr_num1 = GFX_FindEmptySpriteSlot();
	GFX_InitSprite(0, 0, title_spr_num1, 1, gfx_sprite_graphics_stack[SPRITE_GRAPHICS_ID_TITLE].width_px, gfx_sprite_graphics_stack[SPRITE_GRAPHICS_ID_TITLE].height_px);
	GFX_SetSpriteGraphic(title_spr_num1, 0, SPRITE_GRAPHICS_ID_TITLE, 0, 0);
	GFX_SetSpritePosition(title_spr_num1, 0 - gfx_sprite_graphics_stack[SPRITE_GRAPHICS_ID_TITLE].width_px, 100);

	title_spr_num2 = GFX_FindEmptySpriteSlot();
	GFX_InitSprite(0, 0, title_spr_num2, 1, gfx_sprite_graphics_stack[SPRITE_GRAPHICS_ID_TITLE].width_px, gfx_sprite_graphics_stack[SPRITE_GRAPHICS_ID_TITLE].height_px);
	GFX_SetSpriteGraphic(title_spr_num2, 0, SPRITE_GRAPHICS_ID_TITLE, 0, 0);
	GFX_SetSpritePosition(title_spr_num2, video->screen_width, 100);

	for (i = 0; i < 53; i++) {
		VIDEO_ClearScreenBuffer();
		if (i < 40) {
			GFX_SetSpritePosition(title_spr_num1, gfx_sprite_stack[title_spr_num1].screen_pos_x + 6, 100);
			GFX_SetSpritePosition(title_spr_num2, gfx_sprite_stack[title_spr_num2].screen_pos_x - 6, 100);
		} else if (i < 48) {
			GFX_SetSpritePosition(title_spr_num1, gfx_sprite_stack[title_spr_num1].screen_pos_x + 4, 100);
			GFX_SetSpritePosition(title_spr_num2, gfx_sprite_stack[title_spr_num2].screen_pos_x - 4, 100);
		} else if (i < 52) {
			GFX_SetSpritePosition(title_spr_num1, gfx_sprite_stack[title_spr_num1].screen_pos_x + 2, 100);
			GFX_SetSpritePosition(title_spr_num2, gfx_sprite_stack[title_spr_num2].screen_pos_x - 2, 100);
		} else {
			GFX_SetSpritePosition(title_spr_num1, gfx_sprite_stack[title_spr_num1].screen_pos_x + 1, 100);
			GFX_SetSpritePosition(title_spr_num2, gfx_sprite_stack[title_spr_num2].screen_pos_x - 1, 100);
		}

		GFX_UpdateSprites();

		VIDEO_DrawSpriteInterlacedToScreenBufferRLE(&gfx_sprite_graphics_stack[gfx_sprite_stack[title_spr_num1].gfx[0].graphics_id], &gfx_sprite_stack[title_spr_num1].gfx[0], 0);
		VIDEO_DrawSpriteInterlacedToScreenBufferRLE(&gfx_sprite_graphics_stack[gfx_sprite_stack[title_spr_num2].gfx[0].graphics_id], &gfx_sprite_stack[title_spr_num2].gfx[0], 1);

		VIDEO_VSync();
		VIDEO_ScreenBufferToVRAM();
	}
	GFX_UnloadSprite(title_spr_num2);
	GFX_UnloadSprite(title_spr_num2);
	GFX_UpdateSprites();
	GFX_DrawSprites();
	VIDEO_VSync();
	VIDEO_ScreenBufferToVRAM();
	/// ------------------

	GFX_PCXImageToBuffer("IMENU.DAT", "MENUBACK.PCX", 360 * 200, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);

	GFX_UnloadSprite(title_spr_num2);
	GFX_UpdateSprites();
	GFX_DrawSprites();
	VIDEO_VSync();
	VIDEO_ScreenBufferToVRAM();

	// move title down
	while (GFX_GetSpriteScreenPosY(title_spr_num1) < 140) {
		VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 0, 0);
		GFX_SetSpritePosition(title_spr_num1, GFX_GetSpriteScreenPosX(title_spr_num1), GFX_GetSpriteScreenPosY(title_spr_num1) + 1);
		GFX_UpdateSprites();
		GFX_DrawSprites();
		VIDEO_VSync();
		VIDEO_ScreenBufferToVRAM();
	}

	while (char_number < strlen(ui->txt_file[UI_TXT_INTRO]->line[2])) {
		aux_string[char_number] = ui->txt_file[UI_TXT_INTRO]->line[2][char_number];
		aux_string[char_number + 1] = '\0';
		VIDEO_StringToScreenBuffer(40, 180, aux_string, FONT_BIG_BLACK);
		char_number++;
		VIDEO_VSync();
		VIDEO_ScreenBufferToVRAM();
	}

	// Main menu buttons
	UI_LoadButton(1, ENTITY_ID_BTN, SPRITE_GRAPHICS_ID_BTN_NEW, 320, 16);
	UI_LoadButton(2, ENTITY_ID_BTN, SPRITE_GRAPHICS_ID_BTN_PASS, 340, 40);
	UI_LoadButton(3, ENTITY_ID_BTN, SPRITE_GRAPHICS_ID_BTN_OPT, 360, 64);
	UI_LoadButton(4, ENTITY_ID_BTN, SPRITE_GRAPHICS_ID_BTN_CREDITS, 380, 88);
	UI_LoadButton(5, ENTITY_ID_BTN, SPRITE_GRAPHICS_ID_BTN_EXIT, 400, 112);

	// Options assets
	opt_pnl_spr_num = GFX_FindEmptySpriteSlot();
	if (opt_pnl_spr_num == -1) {
		sprintf(engine.system_error_message1, "Loading options panel function error");
		sprintf(engine.system_error_message2, "No empty sprite slot available");
		sprintf(engine.system_error_message3, " ");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
	}
	GFX_InitSprite(0, 0, opt_pnl_spr_num, 0, gfx_sprite_graphics_stack[SPRITE_GRAPHICS_ID_PNL_OPTIONS].width_px, gfx_sprite_graphics_stack[SPRITE_GRAPHICS_ID_PNL_OPTIONS].height_px);
	GFX_SetSpriteGraphic(opt_pnl_spr_num, 0, SPRITE_GRAPHICS_ID_PNL_OPTIONS, 0, 0);
	GFX_SetSpritePosition(opt_pnl_spr_num, 320, 5);

	UI_LoadButton(6, ENTITY_ID_BTN, SPRITE_GRAPHICS_ID_BTN_BACK, 409, 98);         // Back
	UI_LoadButton(7, ENTITY_ID_BTN, SPRITE_GRAPHICS_ID_BTN_SOUND, 324, 9);         // Sound
	UI_LoadButton(8, ENTITY_ID_BTN, SPRITE_GRAPHICS_ID_BTN_LEFT, 324, 32);         // Sound volume left
	UI_LoadButton(9, ENTITY_ID_BTN, SPRITE_GRAPHICS_ID_BTN_RIGHT, 365, 32);        // Sound volume right
	UI_LoadButton(10, ENTITY_ID_BTN, SPRITE_GRAPHICS_ID_BTN_MUSIC_SCENES, 385, 9); // Scenes music
	UI_LoadButton(11, ENTITY_ID_BTN, SPRITE_GRAPHICS_ID_BTN_MUSIC_INGAME, 385, 31);// Ingame music
	UI_LoadButton(12, ENTITY_ID_BTN, SPRITE_GRAPHICS_ID_BTN_LEFT, 385, 53);        // Music volume left
	UI_LoadButton(13, ENTITY_ID_BTN, SPRITE_GRAPHICS_ID_BTN_RIGHT, 426, 53);       // Music volume right
	UI_LoadButton(14, ENTITY_ID_BTN, SPRITE_GRAPHICS_ID_BTN_LANGUAGE, 324, 48);    // Language
	UI_LoadButton(15, ENTITY_ID_BTN, SPRITE_GRAPHICS_ID_BTN_KEYS, 324, 70);        // Redefine keys
	UI_LoadButton(18, ENTITY_ID_BTN, SPRITE_GRAPHICS_ID_BTN_DIFICULTY, 385, 70);   // Dificulty keys

	// Password assets
	pass_pnl_spr_num = GFX_FindEmptySpriteSlot();
	if (pass_pnl_spr_num == -1) {
		sprintf(engine.system_error_message1, "Loading password panel function error");
		sprintf(engine.system_error_message2, "No empty sprite slot available");
		sprintf(engine.system_error_message3, " ");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
	}
	GFX_InitSprite(0, 0, pass_pnl_spr_num, 0, gfx_sprite_graphics_stack[SPRITE_GRAPHICS_ID_PNL_PASS].width_px, gfx_sprite_graphics_stack[SPRITE_GRAPHICS_ID_PNL_OPTIONS].height_px);
	GFX_SetSpriteGraphic(pass_pnl_spr_num, 0, SPRITE_GRAPHICS_ID_PNL_PASS, 0, 0);
	GFX_SetSpritePosition(pass_pnl_spr_num, 320, 10);
	UI_LoadButton(16, ENTITY_ID_BTN, SPRITE_GRAPHICS_ID_BTN_OK, 321, 55);  // OK
	UI_LoadButton(17, ENTITY_ID_BTN, SPRITE_GRAPHICS_ID_BTN_BACK, 410, 55);// Back

	// Credits assets
	chat_pnl_spr_num = GFX_FindEmptySpriteSlot();
	if (chat_pnl_spr_num == -1) {
		sprintf(engine.system_error_message1, "Loading chat panel function error");
		sprintf(engine.system_error_message2, "No empty sprite slot available");
		sprintf(engine.system_error_message3, " ");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
	}
	GFX_InitSprite(0, 0, chat_pnl_spr_num, 0, gfx_sprite_graphics_stack[SPRITE_GRAPHICS_ID_MCHAT].width_px, gfx_sprite_graphics_stack[SPRITE_GRAPHICS_ID_MCHAT].height_px);
	GFX_SetSpriteGraphic(chat_pnl_spr_num, 0, SPRITE_GRAPHICS_ID_MCHAT, 0, 0);
	GFX_SetSpritePosition(chat_pnl_spr_num, 320, 20);

	for (i = 0; i < 20; i++) {
		UI_UpdateButtons();
		if (UI_GetButtonXPosition(1) > 300) UI_SetButtonPosition(1, UI_GetButtonXPosition(1) - 4, UI_GetButtonYPosition(1));
		if (UI_GetButtonXPosition(1) > 255) UI_SetButtonPosition(1, UI_GetButtonXPosition(1) - 2, UI_GetButtonYPosition(1));
		if (UI_GetButtonXPosition(1) > 250) UI_SetButtonPosition(1, UI_GetButtonXPosition(1) - 1, UI_GetButtonYPosition(1));
		if (UI_GetButtonXPosition(2) > 300) UI_SetButtonPosition(2, UI_GetButtonXPosition(2) - 4, UI_GetButtonYPosition(2));
		if (UI_GetButtonXPosition(2) > 255) UI_SetButtonPosition(2, UI_GetButtonXPosition(2) - 2, UI_GetButtonYPosition(2));
		if (UI_GetButtonXPosition(2) > 250) UI_SetButtonPosition(2, UI_GetButtonXPosition(2) - 1, UI_GetButtonYPosition(2));
		if (UI_GetButtonXPosition(3) > 300) UI_SetButtonPosition(3, UI_GetButtonXPosition(3) - 4, UI_GetButtonYPosition(3));
		if (UI_GetButtonXPosition(3) > 255) UI_SetButtonPosition(3, UI_GetButtonXPosition(3) - 2, UI_GetButtonYPosition(3));
		if (UI_GetButtonXPosition(3) > 250) UI_SetButtonPosition(3, UI_GetButtonXPosition(3) - 1, UI_GetButtonYPosition(3));
		if (UI_GetButtonXPosition(4) > 300) UI_SetButtonPosition(4, UI_GetButtonXPosition(4) - 4, UI_GetButtonYPosition(4));
		if (UI_GetButtonXPosition(4) > 255) UI_SetButtonPosition(4, UI_GetButtonXPosition(4) - 2, UI_GetButtonYPosition(4));
		if (UI_GetButtonXPosition(4) > 250) UI_SetButtonPosition(4, UI_GetButtonXPosition(4) - 1, UI_GetButtonYPosition(4));
		if (UI_GetButtonXPosition(5) > 300) UI_SetButtonPosition(5, UI_GetButtonXPosition(5) - 4, UI_GetButtonYPosition(5));
		if (UI_GetButtonXPosition(5) > 255) UI_SetButtonPosition(5, UI_GetButtonXPosition(5) - 2, UI_GetButtonYPosition(5));
		if (UI_GetButtonXPosition(5) > 250) UI_SetButtonPosition(5, UI_GetButtonXPosition(5) - 1, UI_GetButtonYPosition(5));
		VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 0, 0);
		GFX_UpdateSprites();
		VIDEO_StringToScreenBuffer(40, 180, ui->txt_file[UI_TXT_INTRO]->line[2], FONT_BIG_BLACK);
		GFX_DrawSprites();
		VIDEO_VSync();
		VIDEO_ScreenBufferToVRAM();
	}

	idle_counter = 0;
	exit_menu = false;
	show_main_menu = true;
	redefine_keys = false;

	while (!exit_menu) {

		// Load intro if nothing happends
		if (main_menu_shown) idle_counter++;
		if (idle_counter > 1000) {
			idle_counter = 0;
			exit_menu = true;
		}

		// reset idle counter on mouse move
		if (cursor.left_click || cursor.right_click) {
			idle_counter = 0;
		}

		UI_UpdateUI(false);
		MOUSE_Update(false, false);
		EFFECT_UpdateEffects();

		GFX_UpdateSprites();
		VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 0, 0);
		VIDEO_StringToScreenBuffer(40, 180, ui->txt_file[UI_TXT_INTRO]->line[2], FONT_BIG_BLACK);
		GFX_DrawSprites();
		GFX_DrawCursorSprite();

		// show main menu
		if (show_main_menu && options_menu_hidden && credits_hidden && password_menu_hidden) {
			if (UI_GetButtonXPosition(1) > 250) UI_SetButtonPosition(1, UI_GetButtonXPosition(1) - 4, UI_GetButtonYPosition(1));
			if (UI_GetButtonXPosition(2) > 250) UI_SetButtonPosition(2, UI_GetButtonXPosition(2) - 4, UI_GetButtonYPosition(2));
			if (UI_GetButtonXPosition(3) > 250) UI_SetButtonPosition(3, UI_GetButtonXPosition(3) - 4, UI_GetButtonYPosition(3));
			if (UI_GetButtonXPosition(4) > 250) UI_SetButtonPosition(4, UI_GetButtonXPosition(4) - 4, UI_GetButtonYPosition(4));
			if (UI_GetButtonXPosition(5) > 250) UI_SetButtonPosition(5, UI_GetButtonXPosition(5) - 4, UI_GetButtonYPosition(5));

			if (UI_GetButtonXPosition(5) <= 250) {
				show_main_menu = false;
				hide_main_menu = false;
				main_menu_shown = true;
				main_menu_hidden = false;
			}
		}

		// hide main menu
		if (hide_main_menu) {
			UI_SetButtonPosition(1, UI_GetButtonXPosition(1) + 4, UI_GetButtonYPosition(1));
			UI_SetButtonPosition(2, UI_GetButtonXPosition(2) + 4, UI_GetButtonYPosition(2));
			UI_SetButtonPosition(3, UI_GetButtonXPosition(3) + 4, UI_GetButtonYPosition(3));
			UI_SetButtonPosition(4, UI_GetButtonXPosition(4) + 4, UI_GetButtonYPosition(4));
			UI_SetButtonPosition(5, UI_GetButtonXPosition(5) + 4, UI_GetButtonYPosition(5));

			if (UI_GetButtonXPosition(5) >= 320) {
				show_main_menu = false;
				hide_main_menu = false;
				main_menu_hidden = true;
				main_menu_shown = false;
			}
		}

		// Show options menu
		if (show_options_menu && main_menu_hidden && credits_hidden && password_menu_hidden) {
			GFX_SetSpritePosition(opt_pnl_spr_num, gfx_sprite_stack[opt_pnl_spr_num].screen_pos_x - 4, gfx_sprite_stack[opt_pnl_spr_num].screen_pos_y);
			UI_SetButtonPosition(6, UI_GetButtonXPosition(6) - 4, UI_GetButtonYPosition(6));
			UI_SetButtonPosition(7, UI_GetButtonXPosition(7) - 4, UI_GetButtonYPosition(7));
			UI_SetButtonPosition(8, UI_GetButtonXPosition(8) - 4, UI_GetButtonYPosition(8));
			UI_SetButtonPosition(9, UI_GetButtonXPosition(9) - 4, UI_GetButtonYPosition(9));
			UI_SetButtonPosition(10, UI_GetButtonXPosition(10) - 4, UI_GetButtonYPosition(10));
			UI_SetButtonPosition(11, UI_GetButtonXPosition(11) - 4, UI_GetButtonYPosition(11));
			UI_SetButtonPosition(12, UI_GetButtonXPosition(12) - 4, UI_GetButtonYPosition(12));
			UI_SetButtonPosition(13, UI_GetButtonXPosition(13) - 4, UI_GetButtonYPosition(13));
			UI_SetButtonPosition(14, UI_GetButtonXPosition(14) - 4, UI_GetButtonYPosition(14));
			UI_SetButtonPosition(15, UI_GetButtonXPosition(15) - 4, UI_GetButtonYPosition(15));
			UI_SetButtonPosition(18, UI_GetButtonXPosition(18) - 4, UI_GetButtonYPosition(18));

			if (gfx_sprite_stack[opt_pnl_spr_num].screen_pos_x <= 188) {
				show_options_menu = false;
				hide_options_menu = false;
				options_menu_hidden = false;
				options_menu_shown = true;
			}
		}

		// Options menu shown
		if (options_menu_shown) {
			// Sound option
			switch (settings.sound_device) {
				case 0:// no sound
					VIDEO_StringToScreenBuffer(206, 18, ui->txt_file[UI_TXT_GLOBAL]->line[13], FONT_SLIM_WHITE);
					break;
				case 1:// speaker
					VIDEO_StringToScreenBuffer(206, 18, ui->txt_file[UI_TXT_GLOBAL]->line[14], FONT_SLIM_WHITE);
					break;
				case 2:// adlib
					VIDEO_StringToScreenBuffer(206, 18, ui->txt_file[UI_TXT_GLOBAL]->line[15], FONT_SLIM_WHITE);
					break;
				case 3:// sound blaster
					VIDEO_StringToScreenBuffer(206, 18, ui->txt_file[UI_TXT_GLOBAL]->line[16], FONT_SLIM_WHITE);
					break;
				default:
					break;
			}

			// Sound volume
			sprintf(sound_volume, "%03d\n", settings.sound_volume);
			VIDEO_StringToScreenBuffer(208, 35, sound_volume, FONT_SLIM_WHITE);

			// Scene Music option
			switch (settings.scenes_music) {
				case 0:// music OFF
					VIDEO_StringToScreenBuffer(268, 18, ui->txt_file[UI_TXT_GLOBAL]->line[13], FONT_SLIM_WHITE);
					break;
				case 1:// music ON
					VIDEO_StringToScreenBuffer(268, 18, ui->txt_file[UI_TXT_GLOBAL]->line[17], FONT_SLIM_WHITE);
					break;
				default:
					break;
			}

			// Ingame Music option
			switch (settings.ingame_music) {
				case 0:// music OFF
					VIDEO_StringToScreenBuffer(268, 40, ui->txt_file[UI_TXT_GLOBAL]->line[13], FONT_SLIM_WHITE);
					break;
				case 1:// music ON
					VIDEO_StringToScreenBuffer(268, 40, ui->txt_file[UI_TXT_GLOBAL]->line[17], FONT_SLIM_WHITE);
					break;
				default:
					break;
			}

			// Music volume
			sprintf(music_volume, "%03d\n", settings.music_volume);
			VIDEO_StringToScreenBuffer(269, 56, music_volume, FONT_SLIM_WHITE);

			// Language option
			switch (settings.language) {
				case 0:// SP
					VIDEO_StringToScreenBuffer(210, 57, ui->txt_file[UI_TXT_GLOBAL]->line[18], FONT_SLIM_WHITE);
					break;
				case 1:// EN
					VIDEO_StringToScreenBuffer(210, 57, ui->txt_file[UI_TXT_GLOBAL]->line[19], FONT_SLIM_WHITE);
					break;
				case 2:// FR
					VIDEO_StringToScreenBuffer(210, 57, ui->txt_file[UI_TXT_GLOBAL]->line[20], FONT_SLIM_WHITE);
					break;
				case 3:// GR
					VIDEO_StringToScreenBuffer(210, 57, ui->txt_file[UI_TXT_GLOBAL]->line[21], FONT_SLIM_WHITE);
					break;
				default:
					break;
			}

			// Dificulty option
			switch (settings.dificulty) {
				case 0:// Easy
					VIDEO_StringToScreenBuffer(272, 78, ui->txt_file[UI_TXT_GLOBAL]->line[22], FONT_SLIM_WHITE);
					break;
				case 1:// Medium
					VIDEO_StringToScreenBuffer(272, 78, ui->txt_file[UI_TXT_GLOBAL]->line[23], FONT_SLIM_WHITE);
					break;
				case 2:// Hard
					VIDEO_StringToScreenBuffer(272, 78, ui->txt_file[UI_TXT_GLOBAL]->line[24], FONT_SLIM_WHITE);
					break;
				default:
					break;
			}

			if (redefine_keys) {
				switch (redefine_step) {
					case 0:// Up key
						VIDEO_StringToScreenBuffer(60, 55, ui->txt_file[UI_TXT_GLOBAL]->line[30], FONT_BIG_BLACK);
						if (KEYB_IsAnyKeyPressed()) {
							kbBindingUp = KEYB_GetLastKeyPressed_CODE();
							redefine_step++;
						}
						break;
					case 1:
						if (!KEYB_IsAnyKeyPressed()) redefine_step++;
						break;
					case 2:// Down key
						VIDEO_StringToScreenBuffer(60, 55, ui->txt_file[UI_TXT_GLOBAL]->line[31], FONT_BIG_BLACK);
						if (KEYB_IsAnyKeyPressed()) {
							kbBindingDown = KEYB_GetLastKeyPressed_CODE();
							redefine_step++;
						}
						break;
					case 3:
						if (!KEYB_IsAnyKeyPressed()) redefine_step++;
						break;
					case 4:// Left key
						VIDEO_StringToScreenBuffer(60, 55, ui->txt_file[UI_TXT_GLOBAL]->line[32], FONT_BIG_BLACK);
						if (KEYB_IsAnyKeyPressed()) {
							kbBindingLeft = KEYB_GetLastKeyPressed_CODE();
							redefine_step++;
						}
						break;
					case 5:
						if (!KEYB_IsAnyKeyPressed()) redefine_step++;
						break;
					case 6:// Right key
						VIDEO_StringToScreenBuffer(60, 55, ui->txt_file[UI_TXT_GLOBAL]->line[33], FONT_BIG_BLACK);
						if (KEYB_IsAnyKeyPressed()) {
							kbBindingRight = KEYB_GetLastKeyPressed_CODE();
							redefine_step++;
						}
						break;
					case 7:
						if (!KEYB_IsAnyKeyPressed()) redefine_step++;
						break;
					case 8:// Shoot key
						VIDEO_StringToScreenBuffer(60, 55, ui->txt_file[UI_TXT_GLOBAL]->line[34], FONT_BIG_BLACK);
						if (KEYB_IsAnyKeyPressed()) {
							kbBindingFire = KEYB_GetLastKeyPressed_CODE();
							redefine_step++;
						}
						break;
					case 9:
						if (!KEYB_IsAnyKeyPressed()) redefine_step++;
						break;
					case 10:// Thrown key
						VIDEO_StringToScreenBuffer(60, 55, ui->txt_file[UI_TXT_GLOBAL]->line[35], FONT_BIG_BLACK);
						if (KEYB_IsAnyKeyPressed()) {
							kbBindingThrow = KEYB_GetLastKeyPressed_CODE();
							redefine_step++;
						}
						break;
					case 11:
						if (!KEYB_IsAnyKeyPressed()) redefine_step++;
						break;
					case 12:// Jump key
						VIDEO_StringToScreenBuffer(60, 55, ui->txt_file[UI_TXT_GLOBAL]->line[36], FONT_BIG_BLACK);
						if (KEYB_IsAnyKeyPressed()) {
							kbBindingLoop = KEYB_GetLastKeyPressed_CODE();
							redefine_step++;
						}
						break;
					case 13:
						if (!KEYB_IsAnyKeyPressed()) redefine_step++;
						break;
					case 14:
						redefine_keys = false;
						break;
					default:
						redefine_keys = false;
						break;
				}
			}
		}

		// Hide options menu
		if (hide_options_menu) {
			options_menu_shown = false;
			GFX_SetSpritePosition(opt_pnl_spr_num, gfx_sprite_stack[opt_pnl_spr_num].screen_pos_x + 4, gfx_sprite_stack[opt_pnl_spr_num].screen_pos_y);
			UI_SetButtonPosition(6, UI_GetButtonXPosition(6) + 4, UI_GetButtonYPosition(6));
			UI_SetButtonPosition(7, UI_GetButtonXPosition(7) + 4, UI_GetButtonYPosition(7));
			UI_SetButtonPosition(8, UI_GetButtonXPosition(8) + 4, UI_GetButtonYPosition(8));
			UI_SetButtonPosition(9, UI_GetButtonXPosition(9) + 4, UI_GetButtonYPosition(9));
			UI_SetButtonPosition(10, UI_GetButtonXPosition(10) + 4, UI_GetButtonYPosition(10));
			UI_SetButtonPosition(11, UI_GetButtonXPosition(11) + 4, UI_GetButtonYPosition(11));
			UI_SetButtonPosition(12, UI_GetButtonXPosition(12) + 4, UI_GetButtonYPosition(12));
			UI_SetButtonPosition(13, UI_GetButtonXPosition(13) + 4, UI_GetButtonYPosition(13));
			UI_SetButtonPosition(14, UI_GetButtonXPosition(14) + 4, UI_GetButtonYPosition(14));
			UI_SetButtonPosition(15, UI_GetButtonXPosition(15) + 4, UI_GetButtonYPosition(15));
			UI_SetButtonPosition(18, UI_GetButtonXPosition(18) + 4, UI_GetButtonYPosition(18));
			if (gfx_sprite_stack[opt_pnl_spr_num].screen_pos_x >= 320) {
				show_options_menu = false;
				hide_options_menu = false;
				options_menu_hidden = true;
			}
		}

		// Show password menu
		if (show_password_menu && main_menu_hidden && credits_hidden && options_menu_hidden) {
			GFX_SetSpritePosition(pass_pnl_spr_num, gfx_sprite_stack[pass_pnl_spr_num].screen_pos_x - 4, gfx_sprite_stack[pass_pnl_spr_num].screen_pos_y);
			UI_SetButtonPosition(16, UI_GetButtonXPosition(16) - 4, UI_GetButtonYPosition(16));
			UI_SetButtonPosition(17, UI_GetButtonXPosition(17) - 4, UI_GetButtonYPosition(17));

			if (gfx_sprite_stack[pass_pnl_spr_num].screen_pos_x <= 190) {
				show_password_menu = false;
				hide_password_menu = false;
				password_menu_hidden = false;
				password_menu_shown = true;

				// Reset last password
				password_step = 0;
				password[0] = '*';
				password[1] = ' ';
				password[2] = '*';
				password[3] = ' ';
				password[4] = '*';
				password[5] = ' ';
				password[6] = '*';
				password[7] = ' ';
				password[8] = '*';
				password[9] = ' ';
				password[10] = '*';
				password[11] = ' ';
				password[12] = 0;
			}
		}

		// Password menu shown
		if (password_menu_shown) {
			VIDEO_StringToScreenBuffer(206, 35, password, FONT_SLIM_WHITE);
			switch (password_step) {
				case 0:// wait
					if (AwaitDelayTime()) password_step++;
					break;
				case 1:// 1st char
					if (KEYB_IsAnyKeyPressed()) {
						key = KEYB_GetLastKeyPressed_ASCII();
						if (key != 0) {
							password[0] = key;
							SetDelayTime(300);
							password_step++;
						} else {
							AUDIO_PlaySound(AUDIO_SHOT_FAIL_EFFECT, 0);
						}
					}
					break;
				case 2:// wait
					if (!KEYB_IsAnyKeyPressed()) password_step++;
					break;
				case 3:// 2nd char
					if (KEYB_IsAnyKeyPressed()) {
						if (kbKeyState[SCANCODE_BACKSPACE]) {
							password[0] = '*';
							password_step = 0;
							SetDelayTime(300);
						} else {
							key = KEYB_GetLastKeyPressed_ASCII();
							if (key != 0) {
								password[2] = key;
								SetDelayTime(300);
								password_step++;
							} else {
								AUDIO_PlaySound(AUDIO_SHOT_FAIL_EFFECT, 0);
							}
						}
					}
					break;
				case 4:// wait
					if (!KEYB_IsAnyKeyPressed()) password_step++;
					break;
				case 5:// 3rd char
					if (KEYB_IsAnyKeyPressed()) {
						if (kbKeyState[SCANCODE_BACKSPACE]) {
							password[2] = '*';
							password_step = 2;
							SetDelayTime(300);
						} else {
							key = KEYB_GetLastKeyPressed_ASCII();
							if (key != 0) {
								password[4] = key;
								SetDelayTime(300);
								password_step++;
							} else {
								AUDIO_PlaySound(AUDIO_SHOT_FAIL_EFFECT, 0);
							}
						}
					}
					break;
				case 6:// wait
					if (!KEYB_IsAnyKeyPressed()) password_step++;
					break;
				case 7:// 4th char
					if (KEYB_IsAnyKeyPressed()) {
						if (kbKeyState[SCANCODE_BACKSPACE]) {
							password[4] = '*';
							password_step = 4;
							SetDelayTime(300);
						} else {
							key = KEYB_GetLastKeyPressed_ASCII();
							if (key != 0) {
								password[6] = key;
								SetDelayTime(300);
								password_step++;
							} else {
								AUDIO_PlaySound(AUDIO_SHOT_FAIL_EFFECT, 0);
							}
						}
					}
					break;
				case 8:// wait
					if (!KEYB_IsAnyKeyPressed()) password_step++;
					break;
				case 9:// 5th char
					if (KEYB_IsAnyKeyPressed()) {
						if (kbKeyState[SCANCODE_BACKSPACE]) {
							password[6] = '*';
							password_step = 6;
							SetDelayTime(300);
						} else {
							key = KEYB_GetLastKeyPressed_ASCII();
							if (key != 0) {
								password[8] = key;
								SetDelayTime(300);
								password_step++;
							} else {
								AUDIO_PlaySound(AUDIO_SHOT_FAIL_EFFECT, 0);
							}
						}
					}
					break;
				case 10:// wait
					if (!KEYB_IsAnyKeyPressed()) password_step++;
					break;
				case 11:// 6th char
					if (KEYB_IsAnyKeyPressed()) {
						if (kbKeyState[SCANCODE_BACKSPACE]) {
							password[8] = '*';
							password_step = 8;
							SetDelayTime(300);
						} else {
							key = KEYB_GetLastKeyPressed_ASCII();
							if (key != 0) {
								password[10] = key;
								SetDelayTime(300);
								password_step++;
							} else {
								AUDIO_PlaySound(AUDIO_SHOT_FAIL_EFFECT, 0);
							}
						}
					}
					break;
				case 12:// wait
					if (!KEYB_IsAnyKeyPressed()) password_step++;
					break;
				case 13:// All chars done
					if (kbKeyState[SCANCODE_BACKSPACE]) {
						password[10] = '*';
						password_step = 10;
					}
					break;
				default:
					break;
			}
		}

		// Hide password menu
		if (hide_password_menu) {
			password_menu_shown = false;
			GFX_SetSpritePosition(pass_pnl_spr_num, gfx_sprite_stack[pass_pnl_spr_num].screen_pos_x + 4, gfx_sprite_stack[pass_pnl_spr_num].screen_pos_y);
			UI_SetButtonPosition(16, UI_GetButtonXPosition(16) + 4, UI_GetButtonYPosition(16));
			UI_SetButtonPosition(17, UI_GetButtonXPosition(17) + 4, UI_GetButtonYPosition(17));
			if (gfx_sprite_stack[pass_pnl_spr_num].screen_pos_x >= 320) {
				show_password_menu = false;
				hide_password_menu = false;
				password_menu_hidden = true;
			}
		}

		// Show credits
		if (show_credits && main_menu_hidden && options_menu_hidden && password_menu_hidden) {
			GFX_SetSpritePosition(chat_pnl_spr_num, 160, 10);
			SetDelayTime(2000);
			hide_credits = false;
			show_credits = false;
			credits_hidden = false;
			credits_shown = true;
		}

		// Hide credits
		if (hide_credits) {
			GFX_SetSpritePosition(chat_pnl_spr_num, 320, 10);
			hide_credits = false;
			show_credits = false;
			credits_shown = false;
			credits_hidden = true;
			credits_step = 0;
			show_main_menu = true;
		}

		// Credits being shown
		if (credits_shown) {
			switch (credits_step) {
				case 0:
					VIDEO_StringToScreenBuffer(180, 40, ui->txt_file[UI_TXT_CREDITS]->line[1], FONT_SLIM_BLACK);
					VIDEO_StringToScreenBuffer(180, 60, ui->txt_file[UI_TXT_CREDITS]->line[2], FONT_SLIM_BLACK);
					VIDEO_StringToScreenBuffer(180, 80, ui->txt_file[UI_TXT_CREDITS]->line[3], FONT_SLIM_BLACK);
					if (AwaitDelayTime()) {
						credits_step++;
					}
					break;
				case 1:
					SetDelayTime(2000);
					credits_step++;
					break;
				case 2:
					VIDEO_StringToScreenBuffer(180, 40, ui->txt_file[UI_TXT_CREDITS]->line[4], FONT_SLIM_BLACK);
					VIDEO_StringToScreenBuffer(180, 60, ui->txt_file[UI_TXT_CREDITS]->line[5], FONT_SLIM_BLACK);
					VIDEO_StringToScreenBuffer(180, 80, ui->txt_file[UI_TXT_CREDITS]->line[6], FONT_SLIM_BLACK);
					if (AwaitDelayTime()) {
						credits_step++;
					}
					break;
				case 3:
					SetDelayTime(2000);
					credits_step++;
					break;
				case 4:
					VIDEO_StringToScreenBuffer(180, 40, ui->txt_file[UI_TXT_CREDITS]->line[7], FONT_SLIM_BLACK);
					VIDEO_StringToScreenBuffer(180, 60, ui->txt_file[UI_TXT_CREDITS]->line[8], FONT_SLIM_BLACK);
					VIDEO_StringToScreenBuffer(180, 80, ui->txt_file[UI_TXT_CREDITS]->line[9], FONT_SLIM_BLACK);
					if (AwaitDelayTime()) {
						credits_step++;
					}
					break;
				case 5:
					SetDelayTime(2000);
					credits_step++;
					break;
				case 6:
					VIDEO_StringToScreenBuffer(180, 40, ui->txt_file[UI_TXT_CREDITS]->line[10], FONT_SLIM_BLACK);
					VIDEO_StringToScreenBuffer(180, 60, ui->txt_file[UI_TXT_CREDITS]->line[11], FONT_SLIM_BLACK);
					VIDEO_StringToScreenBuffer(180, 80, ui->txt_file[UI_TXT_CREDITS]->line[12], FONT_SLIM_BLACK);
					if (AwaitDelayTime()) {
						credits_step++;
					}
					break;
				case 7:
					SetDelayTime(3000);
					credits_step++;
					break;
				case 8:
					VIDEO_StringToScreenBuffer(170, 30, ui->txt_file[UI_TXT_CREDITS]->line[13], FONT_SLIM_BLACK);
					VIDEO_StringToScreenBuffer(170, 50, ui->txt_file[UI_TXT_CREDITS]->line[14], FONT_SLIM_BLACK);
					VIDEO_StringToScreenBuffer(170, 70, ui->txt_file[UI_TXT_CREDITS]->line[15], FONT_SLIM_BLACK);
					if (AwaitDelayTime()) {
						credits_step++;
					}
					break;
				case 9:
					SetDelayTime(3000);
					credits_step++;
					break;
				case 10:
					VIDEO_StringToScreenBuffer(170, 30, ui->txt_file[UI_TXT_CREDITS]->line[16], FONT_SLIM_BLACK);
					VIDEO_StringToScreenBuffer(170, 50, ui->txt_file[UI_TXT_CREDITS]->line[17], FONT_SLIM_BLACK);
					VIDEO_StringToScreenBuffer(170, 70, ui->txt_file[UI_TXT_CREDITS]->line[18], FONT_SLIM_BLACK);
					if (AwaitDelayTime()) {
						credits_step++;
					}
					break;
				case 11:
					hide_credits = true;
					break;
				default:
					break;
			}
		}

		// Option selected
		switch (ui->button_pressed) {
			case 0:// No button pressed
				break;
			case 1:// New game
				exit_menu = true;
				engine.scene = 1;
				engine.room = 1;
				break;
			case 2:// Password
				hide_main_menu = true;
				show_password_menu = true;
				break;
			case 3:// options
				hide_main_menu = true;
				show_options_menu = true;
				break;
			case 4:// credits
				hide_main_menu = true;
				show_credits = true;
				break;
			case 5:// exit game
				exit_menu = true;
				engine.exit_game = true;
				break;
			case 6:// hide options menu
				redefine_keys = false;
				hide_options_menu = true;
				show_main_menu = true;
				FILE_SaveSettingsFile("SETTINGS.CFG");
				break;
			case 7:// Sound setting
				settings.sound_device++;
				if (settings.sound_device == 1) AUDIO_StopSong();
				if (settings.sound_device > 1) settings.sound_device = 0;
				if (settings.sound_device == 0) AUDIO_StopSong();
				if (settings.sound_device == 1) {
					AUDIO_LoadSong(AUDIO_SONG_2);
					AUDIO_PlaySong(true);
				}

				SetDelayTime(300);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				break;
			case 8:// decrease sound volume
				if (settings.sound_volume > 0) settings.sound_volume--;
				break;
			case 9:// increase sound volume
				if (settings.sound_volume < 100) settings.sound_volume++;
				break;
			case 10:// Scenes-music setting
				settings.scenes_music++;
				if (settings.scenes_music > 1) settings.scenes_music = 0;
				SetDelayTime(300);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				break;
			case 11:// Ingame-music setting
				settings.ingame_music++;
				if (settings.ingame_music > 1) settings.ingame_music = 0;
				SetDelayTime(300);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				break;
			case 12:// decrease music volume
				if (settings.music_volume > 0) settings.music_volume--;
				break;
			case 13:// increase music volume
				if (settings.music_volume < 100) settings.music_volume++;
				break;

			case 14:// change language
				settings.language++;
				if (settings.language > 3) settings.language = 0;
				UI_SetLanguage(settings.language);
				LoadTexts();
				SetDelayTime(300);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				break;
			case 15:// Redefine keys
				redefine_keys = true;
				redefine_step = 0;
				break;
			case 16:// Password confirmation
				// Chapter 1: Room 2: 'The travel' >> TRAVEL
				if ((password[0] == 'T') && (password[2] == 'R') && (password[4] == 'A') && (password[6] == 'V') && (password[8] == 'E') && (password[10] == 'L')) {
					exit_menu = true;
					engine.scene = 1;
					engine.room = 2;
				}

				// Chapter 2: Room 1: 'Down to the hell' >> GODOWN
				if ((password[0] == 'G') && (password[2] == 'O') && (password[4] == 'D') && (password[6] == 'O') && (password[8] == 'W') && (password[10] == 'N')) {
					exit_menu = true;
					engine.scene = 2;
					engine.room = 1;
				}

				// Chapter 3: Room 1 'Behind enemy lines' >> INSIDE
				if ((password[0] == 'I') && (password[2] == 'N') && (password[4] == 'S') && (password[6] == 'I') && (password[8] == 'D') && (password[10] == 'E')) {
					exit_menu = true;
					engine.scene = 3;
					engine.room = 1;
				}

				// Chapter 4: Room 1: 'Praise for mercy' >> PRAISE
				if ((password[0] == 'P') && (password[2] == 'R') && (password[4] == 'A') && (password[6] == 'I') && (password[8] == 'S') && (password[10] == 'E')) {
					exit_menu = true;
					engine.scene = 4;
					engine.room = 1;
				}

				// Chapter 5: Room 1: 'Hit and run' >> HITRUN
				if ((password[0] == 'H') && (password[2] == 'I') && (password[4] == 'T') && (password[6] == 'R') && (password[8] == 'U') && (password[10] == 'N')) {
					exit_menu = true;
					engine.scene = 5;
					engine.room = 1;
				}
				// Chapter 6: Room 1: 'Bullet hell' >> BULLET
				if ((password[0] == 'B') && (password[2] == 'U') && (password[4] == 'L') && (password[6] == 'L') && (password[8] == 'E') && (password[10] == 'T')) {
					exit_menu = true;
					engine.scene = 6;
					engine.room = 1;
				}

				if (engine.scene == 0) {
					AUDIO_PlaySound(AUDIO_SHOT_FAIL_EFFECT, 0);
				}

				break;
			case 17:// Hide password menu
				hide_password_menu = true;
				show_main_menu = true;
				break;
			case 18:// Dificulty setting
				settings.dificulty++;
				if (settings.dificulty > 2) settings.dificulty = 0;
				SetDelayTime(300);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				break;
			default:
				Error("Unknown button pressed", "", "", ERROR_SYSTEM);
				break;
		}

		VIDEO_VSync();
		VIDEO_ScreenBufferToVRAM();
	}

	VIDEO_FadeOut(1);
	GFX_UnloadSprites();
	UI_UnloadButtons();
	EFFECT_UnloadEffects();
	AUDIO_StopSong();
	AUDIO_UnloadSong();
}
void LoadGlobalAssets(void) {
	ScreenSetCursor(21, 15);
	printf("       ...loading effects graphics...       ");
	GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "SPARK.PCX", SPRITE_GRAPHICS_ID_SPARK_EFFECT, 16, 16, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "PUNCHL.PCX", SPRITE_GRAPHICS_ID_PUNCH_EFFECT, 24, 16, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "PISTOLL.PCX", SPRITE_GRAPHICS_ID_PISTOL_EFFECT, 24, 16, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "SHOTGUNL.PCX", SPRITE_GRAPHICS_ID_SHOTGUN_EFFECT, 24, 16, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "UZIL.PCX", SPRITE_GRAPHICS_ID_UZI_EFFECT, 24, 16, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "SNIPPERL.PCX", SPRITE_GRAPHICS_ID_SNIPPER_EFFECT, 24, 16, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "BLOOD.PCX", SPRITE_GRAPHICS_ID_BLOOD, 4, 4, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "FIRE1.PCX", SPRITE_GRAPHICS_ID_FIRE1, 8, 8, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "ELECTRIC1.PCX", SPRITE_GRAPHICS_ID_ELECTRIC1, 8, 8, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "WATER1.PCX", SPRITE_GRAPHICS_ID_WATER1, 8, 8, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);

	GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "EXPLOSION.PCX", SPRITE_GRAPHICS_ID_EXPLOSION1, 32, 32, 10, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "TIMER.PCX", SPRITE_GRAPHICS_ID_TIMER, 8, 8, 10, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "CLICK.PCX", SPRITE_GRAPHICS_ID_CLICK, 5, 5, 5, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "RAIN.PCX", SPRITE_GRAPHICS_ID_RAIN_EFFECT, 5, 5, 5, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);

	ScreenSetCursor(21, 15);
	printf("       ...loading cursor graphics...       ");
	GFX_LoadSpriteGraphicsRLE("CURSOR.DAT", "POINTER.PCX", SPRITE_GRAPHICS_ID_CURSOR, 7, 12, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("CURSOR.DAT", "AIM.PCX", SPRITE_GRAPHICS_ID_AIM, 15, 15, 5, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);

	ScreenSetCursor(21, 15);
	printf("       ...loading bullets graphics...      ");
	GFX_LoadSpriteGraphicsRLE("BULLETS.DAT", "BULLET1.PCX", SPRITE_GRAPHICS_ID_BULLET1, 8, 8, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("BULLETS.DAT", "BULLET2.PCX", SPRITE_GRAPHICS_ID_BULLET2, 8, 8, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("BULLETS.DAT", "BULLET3.PCX", SPRITE_GRAPHICS_ID_BULLET3, 8, 8, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("BULLETS.DAT", "WEBS.PCX", SPRITE_GRAPHICS_ID_WEBS, 8, 8, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("BULLETS.DAT", "GRENADE.PCX", SPRITE_GRAPHICS_ID_GRENADE1, 8, 8, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("BULLETS.DAT", "ROCKET.PCX", SPRITE_GRAPHICS_ID_MISILE1, 16, 16, 8, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("BULLETS.DAT", "SHADOW.PCX", SPRITE_GRAPHICS_ID_BULLET_SHADOW, 8, 8, 2, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("BULLETS.DAT", "PUNCH1.PCX", SPRITE_GRAPHICS_ID_PUNCH, 12, 12, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("BULLETS.DAT", "PUKE.PCX", SPRITE_GRAPHICS_ID_PUKE, 16, 16, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("BULLETS.DAT", "RAY.PCX", SPRITE_GRAPHICS_ID_RAY, 16, 16, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("BULLETS.DAT", "WEB.PCX", SPRITE_GRAPHICS_ID_WEB, 16, 16, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);

	ScreenSetCursor(21, 15);
	printf("       ...loading items graphics...        ");
	GFX_LoadSpriteGraphicsRLE("ITEMS.DAT", "MEDIKIT.PCX", SPRITE_GRAPHICS_ID_ITEM_MEDIKIT, 12, 14, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("ITEMS.DAT", "DOCU.PCX", SPRITE_GRAPHICS_ID_ITEM_DOCU, 12, 14, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("ITEMS.DAT", "AMMO1.PCX", SPRITE_GRAPHICS_ID_ITEM_AMMO1, 15, 13, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("ITEMS.DAT", "AMMO2.PCX", SPRITE_GRAPHICS_ID_ITEM_AMMO2, 15, 13, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("ITEMS.DAT", "AMMO3.PCX", SPRITE_GRAPHICS_ID_ITEM_AMMO3, 15, 13, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("ITEMS.DAT", "AMMO4.PCX", SPRITE_GRAPHICS_ID_ITEM_AMMO4, 15, 13, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("ITEMS.DAT", "GRENADE.PCX", SPRITE_GRAPHICS_ID_ITEM_GRENADE, 15, 13, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("ITEMS.DAT", "KEYRED.PCX", SPRITE_GRAPHICS_ID_ITEM_RED_KEY, 12, 14, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("ITEMS.DAT", "KEYBLUE.PCX", SPRITE_GRAPHICS_ID_ITEM_BLUE_KEY, 12, 14, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("ITEMS.DAT", "KEYYELLOW.PCX", SPRITE_GRAPHICS_ID_ITEM_YELLOW_KEY, 12, 14, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("ITEMS.DAT", "KEYGREEN.PCX", SPRITE_GRAPHICS_ID_ITEM_GREEN_KEY, 12, 14, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);

	ScreenSetCursor(21, 15);
	printf("        ...loading panel graphics...       ");
	GFX_LoadSpriteGraphicsRLE("PANEL.DAT", "GUN00.PCX", SPRITE_GRAPHICS_ID_GUN0, 32, 16, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("PANEL.DAT", "GUN01.PCX", SPRITE_GRAPHICS_ID_GUN1, 32, 16, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("PANEL.DAT", "GUN02.PCX", SPRITE_GRAPHICS_ID_GUN2, 32, 16, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("PANEL.DAT", "GUN03.PCX", SPRITE_GRAPHICS_ID_GUN3, 32, 16, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("PANEL.DAT", "LIFEBAR.PCX", SPRITE_GRAPHICS_ID_LIFEBAR, 48, 6, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("PANEL.DAT", "GUN04.PCX", SPRITE_GRAPHICS_ID_GUN4, 32, 16, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("PANEL.DAT", "GUN05.PCX", SPRITE_GRAPHICS_ID_GUN5, 32, 16, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("PANEL.DAT", "GRENADE.PCX", SPRITE_GRAPHICS_ID_GRENADE, 16, 16, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("PANEL.DAT", "CHAT.PCX", SPRITE_GRAPHICS_ID_CHAT, 220, 70, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);

	ScreenSetCursor(21, 15);
	printf("        ...loading actor graphics...       ");
	ACTOR_LoadActorAssets("ACTOR.DAT");

	ScreenSetCursor(21, 15);
	printf("       ...loading misc graphics...        ");
	GFX_LoadSpriteGraphicsRLE("ISPR.DAT", "ACTOR.PCX", SPRITE_GRAPHICS_ID_ACTOR_INTRO, 20, 33, 12, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("MISC.DAT", "EMPTY.PCX", SPRITE_GRAPHICS_ID_EMPTY, 1, 1, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("MISC.DAT", "CHAT.PCX", SPRITE_GRAPHICS_ID_CHAT_SMALL, 55, 30, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("MISC.DAT", "CHAT_BIG.PCX", SPRITE_GRAPHICS_ID_CHAT_BIG, 140, 50, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("MISC.DAT", "FUCK.PCX", SPRITE_GRAPHICS_ID_FUCK, 158, 81, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);

	ScreenSetCursor(21, 15);
	printf("     ...loading main menu graphics...       ");
	GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "BTNNEW.PCX", SPRITE_GRAPHICS_ID_BTN_NEW, 63, 14, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "BTNOPT.PCX", SPRITE_GRAPHICS_ID_BTN_OPT, 63, 14, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "BTNPASS.PCX", SPRITE_GRAPHICS_ID_BTN_PASS, 63, 14, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "BTNCRED.PCX", SPRITE_GRAPHICS_ID_BTN_CREDITS, 63, 14, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "BTNEXIT.PCX", SPRITE_GRAPHICS_ID_BTN_EXIT, 63, 14, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "BTNBACK.PCX", SPRITE_GRAPHICS_ID_BTN_BACK, 31, 15, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "BTNOK.PCX", SPRITE_GRAPHICS_ID_BTN_OK, 31, 15, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "BTNRIGHT.PCX", SPRITE_GRAPHICS_ID_BTN_RIGHT, 15, 15, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "BTNLEFT.PCX", SPRITE_GRAPHICS_ID_BTN_LEFT, 15, 15, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);

	GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "BTNSOUND.PCX", SPRITE_GRAPHICS_ID_BTN_SOUND, 55, 21, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "BTNSMUS.PCX", SPRITE_GRAPHICS_ID_BTN_MUSIC_SCENES, 55, 21, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "BTNIMUS.PCX", SPRITE_GRAPHICS_ID_BTN_MUSIC_INGAME, 55, 21, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "BTNLANG.PCX", SPRITE_GRAPHICS_ID_BTN_LANGUAGE, 55, 21, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "BTNKEYS.PCX", SPRITE_GRAPHICS_ID_BTN_KEYS, 55, 21, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "BTNDIF.PCX", SPRITE_GRAPHICS_ID_BTN_DIFICULTY, 55, 21, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);

	GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "POPTIONS.PCX", SPRITE_GRAPHICS_ID_PNL_OPTIONS, 124, 90, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "PPASS.PCX", SPRITE_GRAPHICS_ID_PNL_PASS, 120, 41, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "MTITLE.PCX", SPRITE_GRAPHICS_ID_TITLE, 243, 41, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "MCHAT.PCX", SPRITE_GRAPHICS_ID_MCHAT, 136, 88, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);

	ScreenSetCursor(21, 15);
	printf("     ...loading text and dialogs ...       ");
	UI_SetLanguage(settings.language);
	LoadTexts();
}
/** End credits
 */
static void EndCredits(void) {

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
				GFX_PCXImageToBuffer("END.DAT", "END1H.PCX", 256 * 160, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				AUDIO_LoadSong(AUDIO_SONG_3);
				step++;
				break;
			case 2:// Hide loading screen
				UI_HideLoadingScreen();
				step++;
				break;
			case 3:///
				VIDEO_ClearScreenBuffer();
				GFX_LoadPalette("PALETTES.DAT", "END.PCX", 256);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				VIDEO_FadeIn(4);

				TIMER_UpdateTimerTime(TIMER_AUDIO_NUMBER, 50);
				AUDIO_PlaySong(true);

				step++;
				break;
			case 4:// First image
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 262, 166, 40, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 43, 23);
				step++;
				break;
			case 5:// Just wait
				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 6:// Show chat
				chat_spr_num = GFX_FindEmptySpriteSlot();
				if (chat_spr_num == -1) {
					sprintf(engine.system_error_message1, "End scene function error");
					sprintf(engine.system_error_message2, "No empty sprite slot available");
					sprintf(engine.system_error_message3, " ");
					Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
				} else {
					GFX_InitSprite(ENTITY_ID_EMPTY, 0, chat_spr_num, 0, 65, 30);
				}
				GFX_SetSpriteGraphic(chat_spr_num, 0, SPRITE_GRAPHICS_ID_CHAT_BIG, 0, 0);
				GFX_SetDefaultAnimation(chat_spr_num, false, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 120, 100);
				char_number = 0;
				step++;
				break;
			case 7:// Show chat
				VIDEO_StringToScreenBuffer(126, 105, ui->txt_file[UI_TXT_END]->line[1], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(126, 120, ui->txt_file[UI_TXT_END]->line[2], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(126, 135, ui->txt_file[UI_TXT_END]->line[3], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				GFX_HideSprite(chat_spr_num);
				step++;
				break;
			case 8:// Load second scene
				GFX_PCXImageToBuffer("END.DAT", "END2H.PCX", 100 * 140, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("END.DAT", "END2L.PCX", 100 * 140, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				GFX_PCXImageToBuffer("END.DAT", "END3H.PCX", 140 * 95, gfx->image_buffer3, &gfx->image_buffer3_width, &gfx->image_buffer3_height);
				step++;
				break;
			case 9:// First image
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 106, 146, 10, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 13, 23);
				step++;
				break;
			case 10:// Show chat
				GFX_SetSpriteGraphic(chat_spr_num, 0, SPRITE_GRAPHICS_ID_CHAT_BIG, 0, 0);
				GFX_SetDefaultAnimation(chat_spr_num, false, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 100, 10);
				GFX_ShowSprite(chat_spr_num);
				char_number = 0;
				step++;
				break;
			case 11:// Show chat
				VIDEO_StringToScreenBuffer(106, 15, ui->txt_file[UI_TXT_END]->line[5], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 30, ui->txt_file[UI_TXT_END]->line[6], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 45, ui->txt_file[UI_TXT_END]->line[7], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(6000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				GFX_HideSprite(chat_spr_num);
				step++;
				break;
			case 12:// second and third image
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 106, 146, 10, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 13, 23);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 101, 150, 80, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 153, 83);
				step++;
				break;
			case 13:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 14:// second and third image
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 106, 146, 10, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 13, 23);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 101, 150, 80, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 153, 83);
				step++;
				break;
			case 15:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 16:// second and third image
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 106, 146, 10, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 13, 23);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 101, 150, 80, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 153, 85);
				step++;
				break;
			case 17:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 18:// second and third image
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 106, 146, 10, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 13, 23);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 101, 150, 80, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 153, 81);
				step++;
				break;
			case 19:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 20:// second and third image
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 106, 146, 10, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 13, 23);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 101, 150, 80, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 153, 85);
				step++;
				break;
			case 21:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 22:// second and third image
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 106, 146, 10, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 13, 23);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 101, 150, 80, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 153, 81);
				step++;
				break;
			case 23:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 24:// second and third image
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 106, 146, 10, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 13, 23);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 101, 150, 80, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 153, 83);
				step++;
				break;
			case 25:// Load fourth scene
				GFX_PCXImageToBuffer("END.DAT", "END4H.PCX", 140 * 140, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("END.DAT", "END4L.PCX", 140 * 140, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				GFX_PCXImageToBuffer("END.DAT", "END5H.PCX", 120 * 80, gfx->image_buffer3, &gfx->image_buffer3_width, &gfx->image_buffer3_height);
				step++;
				break;
			case 26:// First image
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 146, 10, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 13, 23);
				step++;
				break;
			case 27:// Show chat
				GFX_SetDefaultAnimation(chat_spr_num, false, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 100, 20);
				GFX_ShowSprite(chat_spr_num);
				char_number = 0;
				step++;
				break;
			case 28:// Show chat
				VIDEO_StringToScreenBuffer(106, 25, ui->txt_file[UI_TXT_END]->line[9], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 40, ui->txt_file[UI_TXT_END]->line[10], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 55, ui->txt_file[UI_TXT_END]->line[11], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(6000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				GFX_HideSprite(chat_spr_num);
				break;
			case 29:// Show chat
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 146, 10, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 13, 23);

				GFX_SetDefaultAnimation(chat_spr_num, false, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 130, 100);
				GFX_ShowSprite(chat_spr_num);
				char_number = 0;
				step++;
				break;
			case 30:// Show chat
				VIDEO_StringToScreenBuffer(136, 105, ui->txt_file[UI_TXT_END]->line[13], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(136, 120, ui->txt_file[UI_TXT_END]->line[14], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(136, 135, ui->txt_file[UI_TXT_END]->line[15], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(6000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				GFX_HideSprite(chat_spr_num);
				break;
			case 31://  image
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 146, 10, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 13, 23);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 126, 86, 180, 40, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 183, 43);
				step++;
				break;
			case 32:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 33://  image
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 146, 10, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 13, 23);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 126, 86, 180, 40, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 183, 43);
				step++;
				break;
			case 34:// Just wait
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 35:// Load fourth scene
				GFX_PCXImageToBuffer("END.DAT", "END6H.PCX", 140 * 140, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("END.DAT", "END7H.PCX", 100 * 100, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				GFX_PCXImageToBuffer("END.DAT", "END8H.PCX", 100 * 100, gfx->image_buffer3, &gfx->image_buffer3_width, &gfx->image_buffer3_height);
				step++;
				break;
			case 36://  image
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 146, 140, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 143, 23);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 106, 106, 60, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 63, 13);
				step++;
				break;
			case 37:// Just wait
				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 38://  image
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 146, 140, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 143, 23);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 106, 106, 60, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 63, 13);
				step++;
				break;
			case 39:// Just wait
				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 40://  image
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 146, 140, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 143, 23);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 106, 106, 60, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 63, 13);
				step++;
				break;
			case 41:// Just wait
				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 42://  image
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 146, 140, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 143, 23);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 106, 106, 60, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 63, 13);
				step++;
				break;
			case 43:// Load fourth scene
				GFX_PCXImageToBuffer("END.DAT", "END9H.PCX", 100 * 100, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				step++;
				break;
			case 44://  image
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 146, 140, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 143, 23);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 106, 106, 60, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 63, 13);
				step++;
				break;
			case 45:// Just wait
				SetDelayTime(8000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 46:// Fade out
				VIDEO_FadeOut(1);
				step++;
				break;
			case 47:
				VIDEO_ClearScreenBuffer();
				VIDEO_StringToScreenBuffer(100, 90, ui->txt_file[UI_TXT_END]->line[20], FONT_BIG_BLACK);

				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				VIDEO_FadeIn(4);
				SetDelayTime(8000);
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
	BOSS_Unload();
	ITEM_UnloadItems();
	PARTICLE_UnloadParticles();
	EFFECT_UnloadEffects();
	OBJECT_UnloadObjects();
	GFX_UnloadSprites();
	AUDIO_StopSong();
	AUDIO_UnloadSong();

	engine.scene = 0;
	engine.room = 1;
}
/** MAIN FUNCTION*******
 */
int main(int argc, char **argv) {
	//gdb_start();

	engine.good_mode = false;
	engine.debug_mode = false;

	// Check for debug mode argument
	switch (argc) {
		case 3:
			// Two arguments
			if (strcmp(argv[2], "-god") == 0) {
				engine.good_mode = true;
				printf("God mode enabled\n");
				sleep(1);
			}

			if (strcmp(argv[2], "-debug") == 0) {
				engine.debug_mode = true;
				printf("Debug mode enabled\n");
				sleep(1);
			}
		case 2:
			// One argument
			if (strcmp(argv[1], "-god") == 0) {
				engine.good_mode = true;
				printf("God mode enabled\n");
				sleep(1);
			}

			if (strcmp(argv[1], "-debug") == 0) {
				engine.debug_mode = true;
				printf("Debug mode enabled\n");
				sleep(1);
			}
		case 1:
			// Normal start, no arguments
			break;
		default:
			break;
	}

	// Initialize the game engine
	InitEngine();

	/****************************
	*    GAME STARTS HERE!!
	*
	****************************/

	Logo();

	engine.exit_game = false;

	engine.scene = 0;
	engine.room = 1;

	MOUSE_InitCursorSprite(SPRITE_GRAPHICS_ID_CURSOR);// Initialize mouse cursor

	while (!engine.exit_game) {

		//if (ui->pause) gdb_checkpoint();

		switch (engine.scene) {
			case 0:// main menu
				engine.exit_menu = false;
				Intro();
				Menu();
				break;
			case 1:// scene 1 :: Mission 1 :: The travel. Pass: TRAVEL
				Scene1_Intro();
				Scene1_Loop();
				if (!actor->status_dead && !engine.exit_menu) Scene1_Outro();
				break;
			case 2:// scene 2 :: Mission 2 :: Down to the hell. Pass: GODOWN
				Scene2_Intro();
				Scene2_Loop();
				if (!actor->status_dead && !engine.exit_menu) Scene2_Outro();
				break;
			case 3:// scene 3 :: Mission 3 :: Behind enemy lines. Pass: INSIDE
				Scene3_Intro();
				Scene3_Loop();
				if (!actor->status_dead && !engine.exit_menu) Scene3_Outro();
				break;
			case 4:// scene 4 :: Mission 4 :: Praise for mercy. Pass: PRAISE
				Scene4_Intro();
				Scene4_Loop();
				if (!actor->status_dead && !engine.exit_menu) Scene4_Outro();
				break;
			case 5:// scene 5 :: Mission 5 :: Hit and run. Pass: HITRUN
				Scene5_Intro();
				Scene5_Loop();
				if (!actor->status_dead && !engine.exit_menu) Scene5_Outro();
				break;
			case 6:// scene 6 :: Mission 6 :: Bullet hell. Pass: BULLET
				Scene6_Intro();
				Scene6_Loop();
				if (!actor->status_dead && !engine.exit_menu) Scene6_Outro();
				break;
			case 7:// scene 7 :: End scene
				EndCredits();
				break;
			default:
				// Unknown level
				engine.exit_game = true;
				break;
		}
	}

	ExitDos();
	return 0;
}