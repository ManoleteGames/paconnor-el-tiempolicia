/**********************************************
* main.c
* Entry point
* (c) 2025-2026 by J.Martin
***********************************************/

#include "engine/actor/actor.h"
#include "engine/audio/audio.h"
#include "engine/boss/boss.h"
#include "engine/camera/camera.h"
#include "engine/effect/effect.h"
#include "engine/enemy/enemy.h"
#include "engine/engine.h"
#include "engine/file/file.h"
#include "engine/gfx/gfx.h"
#include "engine/grenade/grenade.h"
#include "engine/item/item.h"
#include "engine/keyb/keyb.h"
#include "engine/map/map.h"
#include "engine/mem/mem.h"
#include "engine/mouse/mouse.h"
#include "engine/npc/npc.h"
#include "engine/object/object.h"
#include "engine/particle/particle.h"
#include "engine/types/types.h"
#include "engine/ui/ui.h"
#include "engine/video/video.h"
#include "pc.h"
#include "stdbool.h"
#include <stdio.h>

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
	}

	// Clear dialogs buffers
	FILE_LoadTextFile(language, "INTRO.TXT", ui->txt_file[UI_TXT_INTRO]);
	FILE_LoadTextFile(language, "GLOBAL.TXT", ui->txt_file[UI_TXT_GLOBAL]);
	FILE_LoadTextFile(language, "CREDITS.TXT", ui->txt_file[UI_TXT_CREDITS]);
	FILE_LoadTextFile(language, "NAMES.TXT", ui->txt_file[UI_TXT_NAMES]);
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
				GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "SPARK.PCX", SPRITE_GRAPHICS_ID_SPARK_EFFECT, 16, 16, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
				GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "PUNCHL.PCX", SPRITE_GRAPHICS_ID_PUNCH_EFFECT, 24, 16, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
				GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "PISTOLL.PCX", SPRITE_GRAPHICS_ID_PISTOL_EFFECT, 24, 16, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
				GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "SHOTGUNL.PCX", SPRITE_GRAPHICS_ID_SHOTGUN_EFFECT, 24, 16, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
				GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "UZIL.PCX", SPRITE_GRAPHICS_ID_UZI_EFFECT, 24, 16, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
				GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "BTNNEW.PCX", SPRITE_GRAPHICS_ID_BTN_NEW, 63, 14, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
				GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "BTNOPT.PCX", SPRITE_GRAPHICS_ID_BTN_OPT, 63, 14, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
				GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "BTNPASS.PCX", SPRITE_GRAPHICS_ID_BTN_PASS, 63, 14, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
				GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "BTNCRED.PCX", SPRITE_GRAPHICS_ID_BTN_CREDITS, 63, 14, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
				GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "BTNEXIT.PCX", SPRITE_GRAPHICS_ID_BTN_EXIT, 63, 14, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
				GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "BTNBACK.PCX", SPRITE_GRAPHICS_ID_BTN_BACK, 31, 15, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
				GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "BTNOK.PCX", SPRITE_GRAPHICS_ID_BTN_OK, 31, 15, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
				GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "BTNRIGHT.PCX", SPRITE_GRAPHICS_ID_BTN_RIGHT, 15, 15, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
				GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "BTNLEFT.PCX", SPRITE_GRAPHICS_ID_BTN_LEFT, 15, 15, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);

				logo_step = 20;
				break;
			case 20:// Load new image on second buffer
				GFX_PCXImageToBuffer("ILOGO.DAT", "LCONTEST.PCX", 320 * 100, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				GFX_LoadSpriteGraphicsRLE("MISC.DAT", "IWORMS.PCX", SPRITE_GRAPHICS_ID_WORMS, 192, 24, 14, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
				GFX_LoadSpriteGraphicsRLE("MISC.DAT", "EMPTY.PCX", SPRITE_GRAPHICS_ID_EMPTY, 1, 1, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
				GFX_LoadSpriteGraphicsRLE("MISC.DAT", "FUCK.PCX", SPRITE_GRAPHICS_ID_FUCK, 158, 81, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_TEMPORARY_SPRITE);

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
			case 21:// Load some assets by the way

				GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "MTITLE.PCX", SPRITE_GRAPHICS_ID_TITLE, 243, 41, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
				GFX_LoadSpriteGraphicsRLE("MISC.DAT", "CHAT.PCX", SPRITE_GRAPHICS_ID_CHAT_SMALL, 55, 30, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
				GFX_LoadSpriteGraphicsRLE("MISC.DAT", "CHAT_BIG.PCX", SPRITE_GRAPHICS_ID_CHAT_BIG, 140, 50, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
				GFX_LoadSpriteGraphicsRLE("PANEL.DAT", "GUN00.PCX", SPRITE_GRAPHICS_ID_GUN0, 32, 16, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
				GFX_LoadSpriteGraphicsRLE("PANEL.DAT", "GUN01.PCX", SPRITE_GRAPHICS_ID_GUN1, 32, 16, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
				GFX_LoadSpriteGraphicsRLE("PANEL.DAT", "GUN02.PCX", SPRITE_GRAPHICS_ID_GUN2, 32, 16, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
				GFX_LoadSpriteGraphicsRLE("PANEL.DAT", "GUN03.PCX", SPRITE_GRAPHICS_ID_GUN3, 32, 16, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
				GFX_LoadSpriteGraphicsRLE("PANEL.DAT", "LIFEBAR.PCX", SPRITE_GRAPHICS_ID_LIFEBAR, 48, 6, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
				GFX_LoadSpriteGraphicsRLE("PANEL.DAT", "GUN04.PCX", SPRITE_GRAPHICS_ID_GUN4, 32, 16, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
				GFX_LoadSpriteGraphicsRLE("PANEL.DAT", "GUN05.PCX", SPRITE_GRAPHICS_ID_GUN5, 32, 16, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
				GFX_LoadSpriteGraphicsRLE("PANEL.DAT", "GRENADE.PCX", SPRITE_GRAPHICS_ID_GRENADE, 16, 16, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);

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
			case 25:// Load some more assets
				ACTOR_LoadActorAssets("ACTOR.DAT");
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
						case 0:// Load some assets
							GFX_LoadSpriteGraphicsRLE("CURSOR.DAT", "POINTER.PCX", SPRITE_GRAPHICS_ID_CURSOR, 7, 12, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
							GFX_LoadSpriteGraphicsRLE("CURSOR.DAT", "AIM.PCX", SPRITE_GRAPHICS_ID_AIM, 15, 15, 5, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
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
						case 3:// Load some assets
							GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "POPTIONS.PCX", SPRITE_GRAPHICS_ID_PNL_OPTIONS, 62, 130, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
							GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "PPASS.PCX", SPRITE_GRAPHICS_ID_PNL_PASS, 120, 41, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
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
						case 6:// Load some assets
							GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "MCHAT.PCX", SPRITE_GRAPHICS_ID_MCHAT, 136, 88, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
							GFX_LoadSpriteGraphicsRLE("PANEL.DAT", "CHAT.PCX", SPRITE_GRAPHICS_ID_CHAT, 220, 70, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
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
						case 9:// Load some assets
							GFX_LoadSpriteGraphicsRLE("BULLETS.DAT", "BULLET1.PCX", SPRITE_GRAPHICS_ID_BULLET1, 8, 8, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
							GFX_LoadSpriteGraphicsRLE("BULLETS.DAT", "BULLET2.PCX", SPRITE_GRAPHICS_ID_BULLET2, 8, 8, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
							GFX_LoadSpriteGraphicsRLE("BULLETS.DAT", "BULLET3.PCX", SPRITE_GRAPHICS_ID_BULLET3, 8, 8, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
							GFX_LoadSpriteGraphicsRLE("BULLETS.DAT", "PUKE.PCX", SPRITE_GRAPHICS_ID_PUKE, 16, 16, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
							GFX_LoadSpriteGraphicsRLE("BULLETS.DAT", "RAY.PCX", SPRITE_GRAPHICS_ID_RAY, 16, 16, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
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
						case 12:// Load some assets
							GFX_LoadSpriteGraphicsRLE("BULLETS.DAT", "GRENADE1.PCX", SPRITE_GRAPHICS_ID_GRENADE1, 8, 8, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
							GFX_LoadSpriteGraphicsRLE("BULLETS.DAT", "GRENADES.PCX", SPRITE_GRAPHICS_ID_GRENADE_SHADOW, 8, 8, 2, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
							GFX_LoadSpriteGraphicsRLE("BULLETS.DAT", "PUNCH1.PCX", SPRITE_GRAPHICS_ID_PUNCH, 12, 12, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
							GFX_LoadSpriteGraphicsRLE("BULLETS.DAT", "WEB.PCX", SPRITE_GRAPHICS_ID_WEB, 16, 16, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
							GFX_LoadSpriteGraphicsRLE("BULLETS.DAT", "WEBS.PCX", SPRITE_GRAPHICS_ID_WEBS, 8, 8, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
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
							// Load some more assets
							GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "SNIPPERL.PCX", SPRITE_GRAPHICS_ID_SNIPPER_EFFECT, 24, 16, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
							GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "BLOOD.PCX", SPRITE_GRAPHICS_ID_BLOOD, 4, 4, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
							GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "FIRE1.PCX", SPRITE_GRAPHICS_ID_FIRE1, 8, 8, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
							GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "ELECTRIC1.PCX", SPRITE_GRAPHICS_ID_ELECTRIC1, 8, 8, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
							GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "WATER1.PCX", SPRITE_GRAPHICS_ID_WATER1, 8, 8, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
							GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "EXPLOSION.PCX", SPRITE_GRAPHICS_ID_EXPLOSION1, 32, 32, 10, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
							GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "TIMER.PCX", SPRITE_GRAPHICS_ID_TIMER, 8, 8, 10, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
							GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "CLICK.PCX", SPRITE_GRAPHICS_ID_CLICK, 5, 5, 5, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
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
			case 41:// Load sprites
				VIDEO_ClearScreen();
				VIDEO_ClearPalette();
				GFX_LoadPalette("PALETTES.DAT", "LMANOL.PCX", 256);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 100, 50);
				VIDEO_FadeIn(1);
				logo_step = 42;
				break;
			case 42:
				// Load some more assets

				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 100, 50);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				logo_step = 43;
				break;
			case 43:
				// Load some more assets

				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 100, 50);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				logo_step = 44;
				break;
			case 44:
				// Load some more assets


				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 100, 50);

				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				logo_step = 45;
			case 45:
				// Load some more assets

				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 100, 50);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				logo_step = 46;
				break;
			case 46:
				// Load some more assets
				GFX_LoadSpriteGraphicsRLE("ISPR.DAT", "ACTOR.PCX", SPRITE_GRAPHICS_ID_ACTOR_INTRO, 20, 33, 12, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
				GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "RAIN.PCX", SPRITE_GRAPHICS_ID_RAIN_EFFECT, 5, 5, 5, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);

				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 100, 50);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				logo_step = 47;
				break;
			case 47:
				// Load some more assets
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

				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 100, 50);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				char_counter = 0;
				aux_counter = 0;
				logo_step = 48;
				break;
			case 48:
				VIDEO_ClearScreenBuffer();
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 100, 50);

				if (VIDEO_StringToScreenBufferRand(40, 15, ui->txt_file[UI_TXT_INTRO]->line[3], FONT_BIG_WHITE, &char_counter, &aux_counter)) {
					aux_counter = 0;
					logo_step = 49;
				}
				AUDIO_PlaySound(AUDIO_TICK_EFFECT, 1);

				SetDelayTime(40);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				break;
			case 49:
				SetDelayTime(80);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				VIDEO_ClearScreenBuffer();
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 100, 50);
				VIDEO_StringToScreenBuffer(40, 15, ui->txt_file[UI_TXT_INTRO]->line[3], FONT_BIG_WHITE);
				if (VIDEO_StringToScreenBufferSteps(80, 30, ui->txt_file[UI_TXT_INTRO]->line[4], FONT_BIG_WHITE, &aux_counter)) {
					aux_counter = 0;
					logo_step = 50;
				}
				AUDIO_PlaySound(AUDIO_TICK_EFFECT, 1);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				break;
			case 50:
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 100, 50);
				VIDEO_StringToScreenBuffer(40, 15, ui->txt_file[UI_TXT_INTRO]->line[3], FONT_BIG_WHITE);
				VIDEO_StringToScreenBuffer(80, 30, ui->txt_file[UI_TXT_INTRO]->line[4], FONT_BIG_WHITE);
				AUDIO_PlaySound(AUDIO_CRASH_EFFECT, 1);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				logo_step = 51;
				break;
			case 51:
				AUDIO_LoadSong(AUDIO_SONG_1);
				GFX_PCXImageToBuffer("INTRO.DAT", "INTRO1.PCX", 640 * 32, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("INTRO.DAT", "INTRO2.PCX", 640 * 96, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				logo_step = 52;
				break;
			case 52:
				engine.logo = false;
				break;
			default:
				engine.logo = false;
				break;
		}
	}

	VIDEO_FadeOut(1);
	SetDelayTime(0);
	GFX_UnloadSprites();

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
	int credits_step;
	int i;
	int title_spr_num1, title_spr_num2, opt_pnl_spr_num, pass_pnl_spr_num, chat_pnl_spr_num;
	unsigned char sound_volume[5];
	unsigned char music_volume[5];

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

	UI_LoadButton(6, ENTITY_ID_BTN, SPRITE_GRAPHICS_ID_BTN_BACK, 345, 135);  // Back
	UI_LoadButton(7, ENTITY_ID_BTN, SPRITE_GRAPHICS_ID_BTN_LEFT, 324, 35);   // Sound volume left
	UI_LoadButton(8, ENTITY_ID_BTN, SPRITE_GRAPHICS_ID_BTN_RIGHT, 365, 35);  // Sound volume right
	UI_LoadButton(9, ENTITY_ID_BTN, SPRITE_GRAPHICS_ID_BTN_LEFT, 324, 75);   // Music volume left
	UI_LoadButton(10, ENTITY_ID_BTN, SPRITE_GRAPHICS_ID_BTN_RIGHT, 365, 75); // Music volume right
	UI_LoadButton(11, ENTITY_ID_BTN, SPRITE_GRAPHICS_ID_BTN_LEFT, 324, 115); // Language left
	UI_LoadButton(12, ENTITY_ID_BTN, SPRITE_GRAPHICS_ID_BTN_RIGHT, 365, 116);// Language right

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
	UI_LoadButton(13, ENTITY_ID_BTN, SPRITE_GRAPHICS_ID_BTN_OK, 321, 55);  // OK
	UI_LoadButton(14, ENTITY_ID_BTN, SPRITE_GRAPHICS_ID_BTN_BACK, 410, 55);// Back

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

	exit_menu = false;
	show_main_menu = true;

	GFX_InitSprite(ENTITY_ID_EMPTY, 0, 20, 1, 7, 12);
	GFX_SetSpriteGraphic(20, 0, SPRITE_GRAPHICS_ID_CURSOR, 0, 0);
	GFX_SetSpritePosition(20, 20, 20);

	while (!exit_menu) {

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

			if (gfx_sprite_stack[opt_pnl_spr_num].screen_pos_x <= 250) {
				show_options_menu = false;
				hide_options_menu = false;
				options_menu_hidden = false;
				options_menu_shown = true;
			}
		}

		// Options menu shown
		if (options_menu_shown) {
			// Sound volume
			sprintf(sound_volume, "%03d\n", settings.sound_volume);
			VIDEO_StringToScreenBuffer(268, 40, sound_volume, FONT_SLIM_WHITE);

			// Music volume
			sprintf(music_volume, "%03d\n", settings.music_volume);
			VIDEO_StringToScreenBuffer(268, 80, music_volume, FONT_SLIM_WHITE);

			// Language
			switch (settings.language) {
				case UI_LANGUAGE_ES:// spanish
					VIDEO_StringToScreenBuffer(266, 120, ui->txt_file[UI_TXT_GLOBAL]->line[18], FONT_SLIM_WHITE);
					break;
				case UI_LANGUAGE_EN:// english
					VIDEO_StringToScreenBuffer(266, 120, ui->txt_file[UI_TXT_GLOBAL]->line[19], FONT_SLIM_WHITE);
					break;
				case UI_LANGUAGE_FR:// French
					VIDEO_StringToScreenBuffer(266, 120, ui->txt_file[UI_TXT_GLOBAL]->line[20], FONT_SLIM_WHITE);
					break;
				default:
					break;
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
			if (gfx_sprite_stack[opt_pnl_spr_num].screen_pos_x >= 320) {
				show_options_menu = false;
				hide_options_menu = false;
				options_menu_hidden = true;
			}
		}

		// Show password menu
		if (show_password_menu && main_menu_hidden && credits_hidden && options_menu_hidden) {
			GFX_SetSpritePosition(pass_pnl_spr_num, gfx_sprite_stack[pass_pnl_spr_num].screen_pos_x - 4, gfx_sprite_stack[pass_pnl_spr_num].screen_pos_y);
			UI_SetButtonPosition(13, UI_GetButtonXPosition(13) - 4, UI_GetButtonYPosition(13));
			UI_SetButtonPosition(14, UI_GetButtonXPosition(14) - 4, UI_GetButtonYPosition(14));

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
							SetDelayTime(200);
							password_step++;
						} else {
							AUDIO_PlaySound(AUDIO_SHOT_FAIL_EFFECT, 0);
						}
					}
					break;
				case 2:// wait
					if (AwaitDelayTime()) password_step++;
					break;
				case 3:// 2nd char
					if (KEYB_IsAnyKeyPressed()) {
						if (kbKeyState[SCANCODE_BACKSPACE]) {
							password[0] = '*';
							password_step = 0;
							SetDelayTime(200);
						} else {
							key = KEYB_GetLastKeyPressed_ASCII();
							if (key != 0) {
								password[2] = key;
								SetDelayTime(200);
								password_step++;
							} else {
								AUDIO_PlaySound(AUDIO_SHOT_FAIL_EFFECT, 0);
							}
						}
					}
					break;
				case 4:// wait
					if (AwaitDelayTime()) password_step++;
					break;
				case 5:// 3rd char
					if (KEYB_IsAnyKeyPressed()) {
						if (kbKeyState[SCANCODE_BACKSPACE]) {
							password[2] = '*';
							password_step = 2;
							SetDelayTime(200);
						} else {
							key = KEYB_GetLastKeyPressed_ASCII();
							if (key != 0) {
								password[4] = key;
								SetDelayTime(200);
								password_step++;
							} else {
								AUDIO_PlaySound(AUDIO_SHOT_FAIL_EFFECT, 0);
							}
						}
					}
					break;
				case 6:// wait
					if (AwaitDelayTime()) password_step++;
					break;
				case 7:// 4th char
					if (KEYB_IsAnyKeyPressed()) {
						if (kbKeyState[SCANCODE_BACKSPACE]) {
							password[4] = '*';
							password_step = 4;
							SetDelayTime(200);
						} else {
							key = KEYB_GetLastKeyPressed_ASCII();
							if (key != 0) {
								password[6] = key;
								SetDelayTime(200);
								password_step++;
							} else {
								AUDIO_PlaySound(AUDIO_SHOT_FAIL_EFFECT, 0);
							}
						}
					}
					break;
				case 8:// wait
					if (AwaitDelayTime()) password_step++;
					break;
				case 9:// 5th char
					if (KEYB_IsAnyKeyPressed()) {
						if (kbKeyState[SCANCODE_BACKSPACE]) {
							password[6] = '*';
							password_step = 6;
							SetDelayTime(200);
						} else {
							key = KEYB_GetLastKeyPressed_ASCII();
							if (key != 0) {
								password[8] = key;
								SetDelayTime(200);
								password_step++;
							} else {
								AUDIO_PlaySound(AUDIO_SHOT_FAIL_EFFECT, 0);
							}
						}
					}
					break;
				case 10:// wait
					if (AwaitDelayTime()) password_step++;
					break;
				case 11:// 6th char
					if (KEYB_IsAnyKeyPressed()) {
						if (kbKeyState[SCANCODE_BACKSPACE]) {
							password[8] = '*';
							password_step = 8;
							SetDelayTime(200);
						} else {
							key = KEYB_GetLastKeyPressed_ASCII();
							if (key != 0) {
								password[10] = key;
								SetDelayTime(200);
								password_step++;
							} else {
								AUDIO_PlaySound(AUDIO_SHOT_FAIL_EFFECT, 0);
							}
						}
					}
					break;
				case 12:// wait
					if (AwaitDelayTime()) password_step++;
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
			UI_SetButtonPosition(13, UI_GetButtonXPosition(13) + 4, UI_GetButtonYPosition(13));
			UI_SetButtonPosition(14, UI_GetButtonXPosition(14) + 4, UI_GetButtonYPosition(14));
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
				hide_options_menu = true;
				show_main_menu = true;
				break;
			case 7:// decrease sound volume
				if (settings.sound_volume > 0) settings.sound_volume--;
				break;
			case 8:// increase sound volume
				if (settings.sound_volume < 100) settings.sound_volume++;
				break;
			case 9:// decrease music volume
				if (settings.music_volume > 0) settings.music_volume--;
				break;
			case 10:// increase music volume
				if (settings.music_volume < 100) settings.music_volume++;
				break;
			case 11:// change language
				settings.language++;
				if (settings.language > 2) settings.language = 2;
				break;
			case 12:// change language
				settings.language--;
				if (settings.language < 0) settings.language = 0;
				break;
			case 13:// Password confirmation
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
			case 14:// Hide password menu
				hide_password_menu = true;
				show_main_menu = true;
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

static void GameOver(void) {

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

	VIDEO_StringToScreenBuffer(80, 90, ui->txt_file[UI_TXT_GLOBAL]->line[25], FONT_BIG_WHITE);

	Update(false);

	SetDelayTime(6000);
	while (!AwaitDelayTime()) {
		// Just wait
	}

	engine.ingame = false;

	engine.room = 1;
	engine.scene = 0;
}

/** SCENARIO 1 :: Mission 1 :: The travel
 */

static void Scene1_Intro(void) {
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
static void Scene1_LoadAssets(void) {
	ACTOR_Load(1 << 4, 22 << 4, SPRITE_GRAPHICS_ID_ACTOR_FEET, SPRITE_GRAPHICS_ID_ACTOR_BODY, SPRITE_GRAPHICS_ID_ACTOR_HEAD, SPRITE_GRAPHICS_ID_ACTOR_LARM, SPRITE_GRAPHICS_ID_ACTOR_RARM_BARE_HANDS, ACTOR_FACING_RIGHT);
	ACTOR_SetGun(actor->sprite_num, ACTOR_GUN_TYPE_BARE_HANDS);
	ACTOR_SetBulletStatus(99, 0, 0, 0, 0);

	// Load enemies and unload them just to preload graphics
	ENEMY_Load("ENEMY1.DAT", 0, 0, 0, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_SLEEP, 50);
	ENEMY_Load("ENEMY2.DAT", 1, 0, 0, SPRITE_GRAPHICS_ID_ENEMY2_CHAT, SPRITE_GRAPHICS_ID_ENEMY2_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY2_FEET, SPRITE_GRAPHICS_ID_ENEMY2_BODY, SPRITE_GRAPHICS_ID_ENEMY2_HEAD, SPRITE_GRAPHICS_ID_ENEMY2_LARM, SPRITE_GRAPHICS_ID_ENEMY2_RARM, ENEMY_FACING_RIGHT, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_SLEEP, 100);
	ENEMY_Load("ENEMY3.DAT", 2, 0, 0, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_SLEEP, 100);
	ENEMY_Load("ENEMY4.DAT", 3, 0, 0, SPRITE_GRAPHICS_ID_ENEMY4_CHAT, SPRITE_GRAPHICS_ID_ENEMY4_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY4_FEET, SPRITE_GRAPHICS_ID_ENEMY4_BODY, SPRITE_GRAPHICS_ID_ENEMY4_HEAD, SPRITE_GRAPHICS_ID_ENEMY4_LARM, SPRITE_GRAPHICS_ID_ENEMY4_RARM, ENEMY_FACING_RIGHT, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_SLEEP, 100);
	ENEMY_Load("ENEMY5.DAT", 4, 0, 0, SPRITE_GRAPHICS_ID_ENEMY5_CHAT, SPRITE_GRAPHICS_ID_ENEMY5_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY5_FEET, SPRITE_GRAPHICS_ID_ENEMY5_BODY, SPRITE_GRAPHICS_ID_ENEMY5_HEAD, SPRITE_GRAPHICS_ID_ENEMY5_LARM, SPRITE_GRAPHICS_ID_ENEMY5_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_SLEEP, 100);
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

	AUDIO_LoadSong(AUDIO_SONG_3);// Load song
}
static void Scene1_UnloadAssets(void) {
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
}
static void Scene1_LoadRoom1(void) {
	ENEMY_Load("ENEMY1.DAT", 0, 6 << 4, 17 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_SLEEP, 50);
	ENEMY_Load("ENEMY1.DAT", 1, 10 << 4, 17 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_SLEEP, 50);
	ENEMY_Load("ENEMY1.DAT", 2, 29 << 4, 17 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_SLEEP, 50);
	ENEMY_Load("ENEMY1.DAT", 3, 21 << 4, 23 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_WALKING_RANDOM_NPC, 50);
	ENEMY_Load("ENEMY1.DAT", 4, 53 << 4, 22 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_WALKING_RANDOM_NPC, 50);
	ENEMY_Load("ENEMY1.DAT", 5, 70 << 4, 12 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_WALKING_RANDOM_NPC, 50);

	MAP_LoadMap("MAPSCN11.DAT", 80, 30, "TSCN11.DAT", "SCN1_1_BACK.PCX", "SCN1_1_FORE.PCX", "SCN1_1_MASK.PCX", 320 * 416, 128 * 128, 128 * 128);

	GFX_LoadPalette("PALETTES.DAT", "SCN11.PCX", 256);
}
static void Scene1_LoadRoom2(void) {

	// Enemies on jail
	ENEMY_Load("ENEMY1.DAT", 2, 44 << 4, 27 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_WALKING_RANDOM_NPC, 50);
	ENEMY_Load("ENEMY1.DAT", 3, 44 << 4, 37 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_WALKING_RANDOM_NPC, 50);
	// Enemy on ring
	ENEMY_Load("ENEMY1.DAT", 4, 38 << 4, 10 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_STATIC_NPC, 5000);
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

	GFX_LoadPalette("PALETTES.DAT", "SCN12.PCX", 256);
}
static void Scene1_SetHotspotsAndEvents(void) {

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
static void Scene1_Loop(void) {
	bool end_sequence;
	bool tutorial;
	int actor_tile_num;
	int sequence_step;
	int scene_step;

	VIDEO_ClearScreenBuffer();
	GFX_LoadPalette("PALETTES.DAT", "ISCN1.PCX", 256);

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

	while (!AwaitDelayTime()) {
		// Just wait
	}

	VIDEO_FadeOut(4);
	VIDEO_ClearScreenBuffer();

	tutorial = false;
	scene_step = 0;
	sequence_step = 0;
	end_sequence = false;
	engine.ingame = true;

	MOUSE_ShowCursor();

	// Initialize the Scene room
	switch (engine.room) {
		case 1:// Room 1. Outside doscity
			Scene1_LoadRoom1();
			ACTOR_SetPosition(1 << 4, 22 << 4, ACTOR_FACING_RIGHT);
			CAM_Init(map->width_px, map->height_px, 0 << 4, 18 << 4);// Initialize camera
			MAP_DrawMapToMapVideoBuffer();
			Update(true);
			Update(true);
			VIDEO_FadeIn(1);
			break;
		case 2:// Room 2. Inside police office
			Scene1_LoadRoom2();
			ACTOR_SetPosition(46 << 4, 49 << 4, ACTOR_FACING_LEFT);
			CAM_Init(map->width_px, map->height_px, 30 << 4, 42 << 4);// Initialize camera
			MAP_DrawMapToMapVideoBuffer();
			Update(true);
			Update(true);
			VIDEO_FadeIn(1);
			break;
		default:
			sprintf(engine.system_error_message1, "Scene1_Loop function error");
			sprintf(engine.system_error_message2, "Undefined room");
			sprintf(engine.system_error_message3, "Selected room: %u", engine.room);
			Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_SYSTEM);
			break;
	}

	// Loop until the game is over
	while (engine.ingame) {

		if (ui->pause) gdb_checkpoint();

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

								NPC_UnloadNpcs();
								ENEMY_UnloadEnemies();
								OBJECT_UnloadObjects();
								EFFECT_UnloadEffects();
								BULLET_UnloadBullets();
								PARTICLE_UnloadParticles();
								GRENADE_UnloadGrenades();
								MAP_UnloadMap();

								Scene1_LoadRoom2();

								ACTOR_SetPosition(46 << 4, 49 << 4, ACTOR_FACING_LEFT);
								CAM_Init(map->width_px, map->height_px, 30 << 4, 42 << 4);// Initialize camera
								MAP_DrawMapToMapVideoBuffer();

								Update(true);
								Update(true);

								engine.room = 2;

								VIDEO_FadeIn(1);

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
											ENEMY_Load("ENEMY2.DAT", 6, 40 << 4, 5 << 4, SPRITE_GRAPHICS_ID_ENEMY2_CHAT, SPRITE_GRAPHICS_ID_ENEMY2_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY2_FEET, SPRITE_GRAPHICS_ID_ENEMY2_BODY, SPRITE_GRAPHICS_ID_ENEMY2_HEAD, SPRITE_GRAPHICS_ID_ENEMY2_LARM, SPRITE_GRAPHICS_ID_ENEMY2_RARM, ENEMY_FACING_RIGHT, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_SLEEP, 100);
											SetDelayTime(500);
											sequence_step++;
											break;
										case 1:// Wait
											if (AwaitDelayTime()) sequence_step++;
											break;
										case 2:// Show enemy 7
											SetDelayTime(500);
											ENEMY_Load("ENEMY3.DAT", 7, 47 << 4, 5 << 4, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_SLEEP, 100);
											sequence_step++;
											break;
										case 3:// Wait
											if (AwaitDelayTime()) sequence_step++;
											break;
										case 4:// Show enemy 8
											SetDelayTime(500);
											ENEMY_Load("ENEMY4.DAT", 8, 40 << 4, 4 << 4, SPRITE_GRAPHICS_ID_ENEMY4_CHAT, SPRITE_GRAPHICS_ID_ENEMY4_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY4_FEET, SPRITE_GRAPHICS_ID_ENEMY4_BODY, SPRITE_GRAPHICS_ID_ENEMY4_HEAD, SPRITE_GRAPHICS_ID_ENEMY4_LARM, SPRITE_GRAPHICS_ID_ENEMY4_RARM, ENEMY_FACING_RIGHT, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_SLEEP, 100);
											sequence_step++;
											break;
										case 5:// Wait
											if (AwaitDelayTime()) sequence_step++;
											break;
										case 6:// Show enemy 9
											SetDelayTime(500);
											ENEMY_Load("ENEMY5.DAT", 9, 47 << 4, 4 << 4, SPRITE_GRAPHICS_ID_ENEMY5_CHAT, SPRITE_GRAPHICS_ID_ENEMY5_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY5_FEET, SPRITE_GRAPHICS_ID_ENEMY5_BODY, SPRITE_GRAPHICS_ID_ENEMY5_HEAD, SPRITE_GRAPHICS_ID_ENEMY5_LARM, SPRITE_GRAPHICS_ID_ENEMY5_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_SLEEP, 100);
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
					if ((enemy[0].status_behavior != ENEMY_STATUS_SLEEP) && (enemy[1].status_behavior != ENEMY_STATUS_SLEEP)) {
						map->event_enabling_room1[4] = false;
					}

					// Disable event 5 if cultist are not at door
					if (enemy[2].status_behavior != ENEMY_STATUS_SLEEP) {
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
		if (actor->status_dead) GameOver();
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
	MM_PopChunks(CT_TEMPORARY_SPRITE);// free level temporary mem
}
static void Scene1_Outro(void) {
	int step;
	int i, horizontal_scroll, max_scroll, src_index, dst_index;
	int chat_spr_num, char_number;
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
				char_number = 0;
				step++;
				break;
			case 5://Sara speech
				VIDEO_StringToScreenBuffer(116, 55, ui->txt_file[UI_TXT_SCN1I]->line[35], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(116, 70, ui->txt_file[UI_TXT_SCN1I]->line[36], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(6000);
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
				VIDEO_StringToScreenBuffer(36, 10, ui->txt_file[UI_TXT_SCN1I]->line[37], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(36, 25, ui->txt_file[UI_TXT_SCN1I]->line[38], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(6000);
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
				VIDEO_StringToScreenBuffer(116, 55, ui->txt_file[UI_TXT_SCN1I]->line[39], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(116, 70, ui->txt_file[UI_TXT_SCN1I]->line[40], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(6000);
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
				SetDelayTime(6000);
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
				VIDEO_StringToScreenBuffer(66, 55, ui->txt_file[UI_TXT_SCN1I]->line[43], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(66, 70, ui->txt_file[UI_TXT_SCN1I]->line[44], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(6000);
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
				VIDEO_StringToScreenBuffer(66, 55, ui->txt_file[UI_TXT_SCN1I]->line[45], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(66, 70, ui->txt_file[UI_TXT_SCN1I]->line[46], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(6000);
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
				VIDEO_StringToScreenBuffer(66, 55, ui->txt_file[UI_TXT_SCN1I]->line[47], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(66, 70, ui->txt_file[UI_TXT_SCN1I]->line[48], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(6000);
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
				VIDEO_StringToScreenBuffer(156, 15, ui->txt_file[UI_TXT_SCN1I]->line[50], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(156, 30, ui->txt_file[UI_TXT_SCN1I]->line[51], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(6000);
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
				VIDEO_StringToScreenBuffer(156, 15, ui->txt_file[UI_TXT_SCN1I]->line[52], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(156, 30, ui->txt_file[UI_TXT_SCN1I]->line[53], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(6000);
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
				SetDelayTime(6000);
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
				SetDelayTime(6000);
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
				SetDelayTime(6000);
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

				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 141, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 143, 208);
				step++;
				break;
			case 42:// Speech
				VIDEO_StringToScreenBuffer(28, 148, ui->txt_file[UI_TXT_SCN1I]->line[15], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(28, 158, ui->txt_file[UI_TXT_SCN1I]->line[16], FONT_SLIM_BLACK);
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
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 74:// Load hangar
				VIDEO_ClearScreen();
				GFX_PCXImageToBuffer("OSCN1.DAT", "OUTRO8H.PCX", 320 * 140, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("OSCN1.DAT", "OUTRO9H.PCX", 120 * 140, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				GFX_PCXImageToBuffer("OSCN1.DAT", "OUTRO8L.PCX", 120 * 140, gfx->image_buffer3, &gfx->image_buffer3_width, &gfx->image_buffer3_height);
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

/** SCENARIO 2 :: Mission 2 :: Down to the hell 
 */
static void Scene2_Intro(void) {
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
static void Scene2_LoadAssets(void) {

	// Load and unload enemies just to get graphic resources loaded
	ENEMY_Load("ENEMY6.DAT", 0, 0 << 4, 0 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_CHASE, 1);
	ENEMY_UnloadEnemies();
	ENEMY_Load("ENEMY7.DAT", 0, 0 << 4, 0 << 4, SPRITE_GRAPHICS_ID_ENEMY2_CHAT, SPRITE_GRAPHICS_ID_ENEMY2_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY2_FEET, SPRITE_GRAPHICS_ID_ENEMY2_BODY, SPRITE_GRAPHICS_ID_ENEMY2_HEAD, SPRITE_GRAPHICS_ID_ENEMY2_LARM, SPRITE_GRAPHICS_ID_ENEMY2_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 10);
	ENEMY_UnloadEnemies();

	// Load boss and unload it just to preload graphics
	BOSS_Load("BOSS1.DAT", 0, 0, BOSS_TYPE_FAST_SPEED, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_BULLET3, BOSS_STATUS_CHASE, BOSS_LIFE);
	BOSS_Unload();
	BOSS_Load("BOSS2.DAT", 0, 0, BOSS_TYPE_MEDIUM_SPEED, SPRITE_GRAPHICS_ID_ENEMY4_CHAT, SPRITE_GRAPHICS_ID_ENEMY4_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY4_FEET, SPRITE_GRAPHICS_ID_ENEMY4_BODY, SPRITE_GRAPHICS_ID_ENEMY4_HEAD, SPRITE_GRAPHICS_ID_ENEMY4_LARM, SPRITE_GRAPHICS_ID_ENEMY4_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_BULLET3, BOSS_STATUS_CHASE, BOSS_LIFE);
	BOSS_Unload();

	ACTOR_Load(78 << 4, 3 << 4, SPRITE_GRAPHICS_ID_ACTOR_FEET, SPRITE_GRAPHICS_ID_ACTOR_BODY, SPRITE_GRAPHICS_ID_ACTOR_HEAD, SPRITE_GRAPHICS_ID_ACTOR_LARM, SPRITE_GRAPHICS_ID_ACTOR_RARM_BARE_HANDS, ACTOR_FACING_LEFT);
	ACTOR_SetGun(actor->sprite_num, ACTOR_GUN_TYPE_BARE_HANDS);
	ACTOR_SetBulletStatus(99, 19, 199, 9, 9);

	UI_SetStatusPanels();// Set status panels

	AUDIO_LoadSong(AUDIO_SONG_3);// Load song
}
static void Scene2_UnloadAssets(void) {
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
}
static void Scene2_LoadRoom1(void) {

	MAP_LoadMap("MAPSCN21.DAT", 80, 57, "TSCN21.DAT", "SCN2_1_BACK.PCX", "SCN2_1_FORE.PCX", "SCN2_1_MASK.PCX", 320 * 416, 128 * 128, 128 * 128);

	GFX_LoadPalette("PALETTES.DAT", "SCN21.PCX", 256);

	ITEM_LoadItem(0, ENTITY_ID_ITEM_MEDIKIT, SPRITE_GRAPHICS_ID_ITEM_MEDIKIT, 11 << 4, 45 << 4);

	ITEM_LoadItem(1, ENTITY_ID_ITEM_KEYRED, SPRITE_GRAPHICS_ID_ITEM_RED_KEY, 74 << 4, 16 << 4);
	ITEM_LoadItem(2, ENTITY_ID_ITEM_KEYGREEN, SPRITE_GRAPHICS_ID_ITEM_GREEN_KEY, 51 << 4, 41 << 4);
	ITEM_LoadItem(3, ENTITY_ID_ITEM_KEYBLUE, SPRITE_GRAPHICS_ID_ITEM_BLUE_KEY, 11 << 4, 13 << 4);
	ITEM_LoadItem(4, ENTITY_ID_ITEM_KEYYELLOW, SPRITE_GRAPHICS_ID_ITEM_YELLOW_KEY, 18 << 4, 21 << 4);

	ITEM_LoadItem(5, ENTITY_ID_ITEM_AMMO1, SPRITE_GRAPHICS_ID_ITEM_AMMO1, 56 << 4, 41 << 4);
	ITEM_LoadItem(6, ENTITY_ID_ITEM_AMMO3, SPRITE_GRAPHICS_ID_ITEM_AMMO3, 78 << 4, 29 << 4);
	ITEM_LoadItem(7, ENTITY_ID_ITEM_MEDIKIT, SPRITE_GRAPHICS_ID_ITEM_MEDIKIT, 29 << 4, 39 << 4);


	ITEM_LoadItem(8, ENTITY_ID_ITEM_MEDIKIT, SPRITE_GRAPHICS_ID_ITEM_MEDIKIT, 41 << 4, 18 << 4);
	ITEM_LoadItem(9, ENTITY_ID_ITEM_AMMO3, SPRITE_GRAPHICS_ID_ITEM_AMMO3, 46 << 4, 4 << 4);

	ITEM_LoadItem(10, ENTITY_ID_ITEM_GRENADE, SPRITE_GRAPHICS_ID_ITEM_GRENADE, 60 << 4, 18 << 4);

	ITEM_LoadItem(11, ENTITY_ID_ITEM_AMMO2, SPRITE_GRAPHICS_ID_ITEM_AMMO2, 20 << 4, 17 << 4);
	ITEM_LoadItem(12, ENTITY_ID_ITEM_AMMO3, SPRITE_GRAPHICS_ID_ITEM_AMMO3, 27 << 4, 39 << 4);
}
static void Scene2_SetHotspotsAndEvents(void) {

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
static void Scene2_Loop(void) {
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

	while (!AwaitDelayTime()) {
		// Just wait
	}

	VIDEO_FadeOut(4);

	scene_step = 0;
	sequence_step = 0;
	scene_counter = 0;
	end_sequence = false;
	engine.ingame = true;

	MOUSE_ShowCursor();

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
			break;
		default:
			Error("Scene2_Loop function error", "Undefined room", "", ERROR_SYSTEM);
			break;
	}

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
						}
						break;
					case 3:// Event 3. Starts the rat party!
						if (map->event_enabling_room1[3]) {
							map->event_enabling_room1[3] = false;
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

							BOSS_Load("BOSS1.DAT", 47 << 4, 27 << 4, BOSS_TYPE_FAST_SPEED, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUKE, BOSS_STATUS_STATIC, BOSS_LIFE);

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

							BOSS_Load("BOSS2.DAT", 5 << 4, 28 << 4, BOSS_TYPE_MEDIUM_SPEED, SPRITE_GRAPHICS_ID_ENEMY4_CHAT, SPRITE_GRAPHICS_ID_ENEMY4_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY4_FEET, SPRITE_GRAPHICS_ID_ENEMY4_BODY, SPRITE_GRAPHICS_ID_ENEMY4_HEAD, SPRITE_GRAPHICS_ID_ENEMY4_LARM, SPRITE_GRAPHICS_ID_ENEMY4_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_WEB, BOSS_STATUS_STATIC, BOSS_LIFE);//

							sequence_step = 0;
							end_sequence = false;
							while (!end_sequence) {
								switch (sequence_step) {
									case 0://
										UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN2D, 7, 355, 355, true, 100);
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
									ENEMY_Load("ENEMY6.DAT", 7, 62 << 4, 43 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_CHASE, 1);
								}
								break;
							case 32:
								if (!enemy[8].is_loaded) {
									ENEMY_Load("ENEMY6.DAT", 8, 63 << 4, 43 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_PUNCH, ENEMY_STATUS_CHASE, 1);
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
							MAP_HideSwapableTile(56 << 4, 23 << 4);
							MAP_HideSwapableTile(57 << 4, 23 << 4);
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
							MAP_HideSwapableTile(9 << 4, 26 << 4);
							MAP_HideSwapableTile(10 << 4, 26 << 4);
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

				break;
		}

		Update(true);

		// Finish all if actor is dead
		if (actor->status_dead) GameOver();
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
	MM_PopChunks(CT_TEMPORARY_SPRITE);// free level temporary mem
}
static void Scene2_Outro(void) {
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
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 242);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 250);
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
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 242);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 250);
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
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 242);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 250);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				step++;
				break;
			case 11://Show chat case
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 242);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 250);
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

/** SCENARIO 3 :: Mission 3 :: Behind enemy lines
 */
static void Scene3_Intro(void) {
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
				GFX_PCXImageToBuffer("ISCN3.DAT", "INTRO1H.PCX", 260 * 70, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("ISCN3.DAT", "INTRO1L.PCX", 200 * 70, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				GFX_PCXImageToBuffer("ISCN3.DAT", "INTRO2H.PCX", 260 * 70, gfx->image_buffer3, &gfx->image_buffer3_width, &gfx->image_buffer3_height);
				AUDIO_LoadSong(AUDIO_SONG_3);
				step++;
				break;
			case 2:// Hide loading screen
				UI_HideLoadingScreen();
				step++;
				break;
			case 3:/// Scene 1 :: Entering kitchen
				VIDEO_ClearScreenBuffer();
				GFX_LoadPalette("PALETTES.DAT", "ISCN3.PCX", 256);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				VIDEO_FadeIn(4);

				TIMER_UpdateTimerTime(TIMER_AUDIO_NUMBER, 50);
				AUDIO_PlaySong(true);

				step++;
				break;
			case 4:// Draw frame
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 206, 76, 30, 50, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, 200, gfx->image_buffer1_height, 33, 53);

				step++;
				break;
			case 5:// First image scroll
				horizontal_scroll = 0;
				dialog_step = 0;
				scroll_end = false;

				while (!scroll_end & engine.sequence) {
					src_index = horizontal_scroll;
					dst_index = CAM_VISIBLE_WIDTH * 53 + 33;

					// Draw image with scroll
					for (i = 0; i < gfx->image_buffer1_height; i++) {
						memcpy(&video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index], &gfx->image_buffer1[src_index], 200);
						src_index += gfx->image_buffer1_width;
						dst_index += CAM_VISIBLE_WIDTH;
					}
					if (horizontal_scroll < 59) horizontal_scroll++;

					// Draw text background
					VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 296, 46, 5, 1, 239);
					VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 40, 8, 4, 208);

					// Draw text
					switch (dialog_step) {
						case 0:
							char_number = 0;
							dialog_delay_counter = 0;
							dialog_step++;
							break;
						case 1:
							VIDEO_StringToScreenBuffer(12, 12, ui->txt_file[UI_TXT_SCN3I]->line[80], FONT_SLIM_BLACK);
							VIDEO_StringToScreenBuffer(12, 22, ui->txt_file[UI_TXT_SCN3I]->line[81], FONT_SLIM_BLACK);
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
							VIDEO_StringToScreenBuffer(12, 12, ui->txt_file[UI_TXT_SCN3I]->line[82], FONT_SLIM_BLACK);
							VIDEO_StringToScreenBuffer(12, 22, ui->txt_file[UI_TXT_SCN3I]->line[83], FONT_SLIM_BLACK);
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
							scroll_end = true;
							break;
						default:
							break;
					}

					SetDelayTime(50);
					while (!AwaitDelayTime()) {
						// Just wait
					}

					VIDEO_VSync();
					VIDEO_ScreenBufferToVRAM();
				}

				step++;
				break;
			case 6:
				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 7:
				horizontal_scroll = 60;
				dialog_step = 0;
				scroll_end = false;

				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 206, 76, 60, 120, 222);

				while (!scroll_end & engine.sequence) {
					src_index = horizontal_scroll;
					dst_index = CAM_VISIBLE_WIDTH * 123 + 63;

					// Draw image with scroll
					for (i = 0; i < gfx->image_buffer3_height; i++) {
						memcpy(&video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index], &gfx->image_buffer3[src_index], 200);
						src_index += gfx->image_buffer3_width;
						dst_index += CAM_VISIBLE_WIDTH;
					}
					if (horizontal_scroll > 0) horizontal_scroll--;

					// Draw text background
					VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 296, 46, 5, 1, 239);
					VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 40, 8, 4, 208);
					// Draw text
					switch (dialog_step) {
						case 0:
							char_number = 0;
							dialog_delay_counter = 0;
							dialog_step++;
							break;
						case 1:
							VIDEO_StringToScreenBuffer(12, 12, ui->txt_file[UI_TXT_SCN3I]->line[84], FONT_SLIM_BLACK);
							VIDEO_StringToScreenBuffer(12, 22, ui->txt_file[UI_TXT_SCN3I]->line[85], FONT_SLIM_BLACK);
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
							VIDEO_StringToScreenBuffer(12, 12, ui->txt_file[UI_TXT_SCN3I]->line[86], FONT_SLIM_BLACK);
							VIDEO_StringToScreenBuffer(12, 22, ui->txt_file[UI_TXT_SCN3I]->line[87], FONT_SLIM_BLACK);
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
							scroll_end = true;
							break;
						default:
							break;
					}

					SetDelayTime(50);
					while (!AwaitDelayTime()) {
						// Just wait
					}

					VIDEO_VSync();
					VIDEO_ScreenBufferToVRAM();
				}

				step++;
				break;
			case 8:
				SetDelayTime(2000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 9://End sequence
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
static void Scene3_LoadAssets(void) {

	// Load enemies and unload them just to get graphics
	ENEMY_Load("ENEMY1.DAT", 0, 0, 0, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_GUN0, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_NPC, 1);
	ENEMY_Load("ENEMY8.DAT", 1, 0, 0, SPRITE_GRAPHICS_ID_ENEMY2_CHAT, SPRITE_GRAPHICS_ID_ENEMY2_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY2_FEET, SPRITE_GRAPHICS_ID_ENEMY2_BODY, SPRITE_GRAPHICS_ID_ENEMY2_HEAD, SPRITE_GRAPHICS_ID_ENEMY2_LARM, SPRITE_GRAPHICS_ID_ENEMY2_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_GUN0, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_NPC, 1);
	ENEMY_Load("ENEMY9.DAT", 2, 0, 0, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_GUN0, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_NPC, 1);
	ENEMY_UnloadEnemies();

	// Load objects and unload them just to get graphics
	OBJECT_LoadObject("OBJECT2.DAT", 0, ENTITY_ID_BARREL, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 0, 0);
	OBJECT_UnloadObjects();

	ACTOR_Load(1 << 4, 22 << 4, SPRITE_GRAPHICS_ID_ACTOR_FEET, SPRITE_GRAPHICS_ID_ACTOR_BODY, SPRITE_GRAPHICS_ID_ACTOR_HEAD, SPRITE_GRAPHICS_ID_ACTOR_LARM, SPRITE_GRAPHICS_ID_ACTOR_RARM_BARE_HANDS, ACTOR_FACING_RIGHT);
	ACTOR_SetGun(actor->sprite_num, ACTOR_GUN_TYPE_BARE_HANDS);
	ACTOR_SetBulletStatus(99, 19, 199, 9, 9);

	NPC_Init();

	UI_SetStatusPanels();// Set status panels

	AUDIO_LoadSong(AUDIO_SONG_3);// Load song
}
static void Scene3_UnloadAssets(void) {
	// Enemy 1
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_CHAT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_FEET);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_BODY);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_HEAD);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_RARM);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_LARM);

	// NPCs
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_NPC_AGENT1);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_NPC_AGENT1_CHAT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_NPC_AGENT2);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_NPC_AGENT2_CHAT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_NPC_CHEF);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_NPC_CHEF_CHAT);
}
static void Scene3_LoadRoom1(void) {

	// Kitchen
	// - Chef
	ENEMY_Load("ENEMY8.DAT", 0, 49 << 4, 4 << 4, SPRITE_GRAPHICS_ID_ENEMY2_CHAT, SPRITE_GRAPHICS_ID_ENEMY2_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY2_FEET, SPRITE_GRAPHICS_ID_ENEMY2_BODY, SPRITE_GRAPHICS_ID_ENEMY2_HEAD, SPRITE_GRAPHICS_ID_ENEMY2_LARM, SPRITE_GRAPHICS_ID_ENEMY2_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_GUN0, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_NPC, 20);

	// Room 1
	// - Enemy 1
	ENEMY_Load("ENEMY9.DAT", 1, 49 << 4, 13 << 4, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_UP_LEFT, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_NPC, 100);
	// - Enemy 2
	ENEMY_Load("ENEMY9.DAT", 2, 44 << 4, 13 << 4, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_UP_RIGHT, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_NPC, 100);
	// - Enemy 3
	ENEMY_Load("ENEMY9.DAT", 3, 33 << 4, 13 << 4, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_RIGHT, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_NPC, 100);
	// - Enemy 4
	ENEMY_Load("ENEMY9.DAT", 4, 33 << 4, 18 << 4, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_UP_RIGHT, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_NPC, 100);
	// - Enemy 13
	ENEMY_Load("ENEMY1.DAT", 13, 41 << 4, 16 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN_RIGHT, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_HIDE, 10);

	// Bathroom
	// Enemy 5
	ENEMY_Load("ENEMY9.DAT", 5, 22 << 4, 14 << 4, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_DOWN_RIGHT, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_NPC, 100);
	// Enemy 14
	ENEMY_Load("ENEMY1.DAT", 14, 27 << 4, 14 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN_RIGHT, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_HIDE, 10);

	// Room 2
	// Enemy 6
	ENEMY_Load("ENEMY9.DAT", 6, 37 << 4, 23 << 4, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_UP_RIGHT, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_NPC, 100);
	// Enemy 7
	ENEMY_Load("ENEMY9.DAT", 7, 47 << 4, 24 << 4, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_WALKING_ARROUND_NPC, 100);
	// Enemy 15
	ENEMY_Load("ENEMY1.DAT", 15, 43 << 4, 27 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN_RIGHT, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_HIDE, 10);

	// Room 3
	// Enemy 6
	ENEMY_Load("ENEMY9.DAT", 8, 12 << 4, 20 << 4, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_UP_RIGHT, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_NPC, 100);
	// Enemy 7
	ENEMY_Load("ENEMY9.DAT", 9, 4 << 4, 15 << 4, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_WALKING_ARROUND_NPC, 100);
	// Enemy 16
	ENEMY_Load("ENEMY1.DAT", 16, 11 << 4, 17 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN_RIGHT, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_HIDE, 10);

	// Garden
	// Enemy 8
	ENEMY_Load("ENEMY9.DAT", 10, 4 << 4, 7 << 4, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_DOWN_RIGHT, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_NPC, 100);
	// Enemy 9
	ENEMY_Load("ENEMY9.DAT", 11, 10 << 4, 7 << 4, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_NPC, 100);
	// Enemy 10
	ENEMY_Load("ENEMY9.DAT", 12, 29 << 4, 2 << 4, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_WALKING_ARROUND_NPC, 100);
	// Enemy 17
	ENEMY_Load("ENEMY1.DAT", 17, 35 << 4, 4 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN_RIGHT, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_HIDE, 10);

	// Objects
	// - Gas bottle on kitchen
	OBJECT_LoadObject("OBJECT2.DAT", 0, ENTITY_ID_BARREL, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 49 << 4, 3 << 4);
	// - Gas bottle on warehouse
	OBJECT_LoadObject("OBJECT2.DAT", 1, ENTITY_ID_BARREL, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 41 << 4, 3 << 4);
	OBJECT_LoadObject("OBJECT2.DAT", 2, ENTITY_ID_BARREL, SPRITE_GRAPHICS_ID_OBJECT2, SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT, 42 << 4, 3 << 4);

	// Items
	ITEM_LoadItem(0, ENTITY_ID_ITEM_KEYRED, SPRITE_GRAPHICS_ID_ITEM_RED_KEY, 44 << 4, 5 << 4);
	ITEM_LoadItem(1, ENTITY_ID_ITEM_KEYBLUE, SPRITE_GRAPHICS_ID_ITEM_BLUE_KEY, 39 << 4, 4 << 4);
	ITEM_LoadItem(2, ENTITY_ID_ITEM_KEYYELLOW, SPRITE_GRAPHICS_ID_ITEM_YELLOW_KEY, 36 << 4, 4 << 4);
	ITEM_LoadItem(3, ENTITY_ID_ITEM_KEYGREEN, SPRITE_GRAPHICS_ID_ITEM_GREEN_KEY, 2 << 4, 40 << 4);
	ITEM_LoadItem(4, ENTITY_ID_ITEM_MEDIKIT, SPRITE_GRAPHICS_ID_ITEM_MEDIKIT, 36 << 4, 7 << 4);
	ITEM_LoadItem(5, ENTITY_ID_ITEM_MEDIKIT, SPRITE_GRAPHICS_ID_ITEM_MEDIKIT, 51 << 4, 37 << 4);
	ITEM_LoadItem(6, ENTITY_ID_ITEM_AMMO1, SPRITE_GRAPHICS_ID_ITEM_AMMO1, 42 << 4, 5 << 4);
	ITEM_LoadItem(7, ENTITY_ID_ITEM_AMMO1, SPRITE_GRAPHICS_ID_ITEM_AMMO1, 36 << 4, 7 << 4);

	MAP_LoadMap("MAPSCN31.DAT", 70, 50, "TSCN31.DAT", "SCN3_1_BACK.PCX", "SCN3_1_FORE.PCX", "SCN3_1_MASK.PCX", 320 * 416, 128 * 256, 128 * 128);
	GFX_LoadPalette("PALETTES.DAT", "SCN31.PCX", 256);
}
static void Scene3_LoadRoom2(void) {

	// Entry
	// - Enemy 0
	ENEMY_Load("ENEMY9.DAT", 0, 24 << 4, 20 << 4, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_NPC, 100);
	// - Enemy 1
	ENEMY_Load("ENEMY9.DAT", 1, 31 << 4, 20 << 4, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_UP_LEFT, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_NPC, 100);

	// Room 1
	// - Enemy 2
	ENEMY_Load("ENEMY9.DAT", 2, 8 << 4, 7 << 4, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_UP_RIGHT, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_NPC, 100);
	// - Enemy 3
	ENEMY_Load("ENEMY9.DAT", 3, 21 << 4, 6 << 4, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_RIGHT, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_NPC, 100);

	// Room 2
	// - Enemy 4
	ENEMY_Load("ENEMY9.DAT", 4, 33 << 4, 6 << 4, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_UP_RIGHT, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_NPC, 100);

	// Room 3
	// Enemy 5
	ENEMY_Load("ENEMY9.DAT", 5, 45 << 4, 5 << 4, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_DOWN_RIGHT, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_NPC, 100);
	// Enemy 6
	ENEMY_Load("ENEMY9.DAT", 6, 53 << 4, 6 << 4, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_UP_RIGHT, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_NPC, 100);

	// Room 4
	// Enemy 7
	ENEMY_Load("ENEMY9.DAT", 7, 5 << 4, 13 << 4, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_WALKING_ARROUND_NPC, 100);
	// Enemy 8
	ENEMY_Load("ENEMY9.DAT", 8, 8 << 4, 15 << 4, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_UP_RIGHT, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_NPC, 100);

	// Room 5
	// Enemy 9
	ENEMY_Load("ENEMY9.DAT", 9, 14 << 4, 15 << 4, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_WALKING_ARROUND_NPC, 100);
	// Enemy 10
	ENEMY_Load("ENEMY9.DAT", 10, 22 << 4, 13 << 4, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_DOWN_RIGHT, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_NPC, 100);

	// Room 6

	// Room 7
	// Enemy 11
	ENEMY_Load("ENEMY9.DAT", 11, 46 << 4, 14 << 4, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_NPC, 100);

	// Room 8
	// Enemy 12
	ENEMY_Load("ENEMY9.DAT", 12, 8 << 4, 25 << 4, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_WALKING_ARROUND_NPC, 100);


	// Enemy 13
	ENEMY_Load("ENEMY1.DAT", 13, 14 << 4, 8 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN_RIGHT, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_HIDE, 10);
	// Enemy 14
	ENEMY_Load("ENEMY1.DAT", 14, 35 << 4, 6 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN_RIGHT, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_HIDE, 10);
	// Enemy 15
	ENEMY_Load("ENEMY1.DAT", 15, 47 << 4, 26 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN_RIGHT, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_HIDE, 10);
	// Enemy 16
	ENEMY_Load("ENEMY1.DAT", 16, 8 << 4, 15 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN_RIGHT, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_HIDE, 10);
	// Enemy 17
	ENEMY_Load("ENEMY1.DAT", 17, 35 << 4, 4 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN_RIGHT, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_HIDE, 10);


	// Objects

	// Items
	ITEM_LoadItem(0, ENTITY_ID_ITEM_KEYRED, SPRITE_GRAPHICS_ID_ITEM_RED_KEY, 23 << 4, 33 << 4);
	ITEM_LoadItem(1, ENTITY_ID_ITEM_KEYBLUE, SPRITE_GRAPHICS_ID_ITEM_BLUE_KEY, 12 << 4, 15 << 4);
	ITEM_LoadItem(2, ENTITY_ID_ITEM_KEYYELLOW, SPRITE_GRAPHICS_ID_ITEM_YELLOW_KEY, 52 << 4, 4 << 4);
	ITEM_LoadItem(3, ENTITY_ID_ITEM_KEYGREEN, SPRITE_GRAPHICS_ID_ITEM_GREEN_KEY, 44 << 4, 34 << 4);

	ITEM_LoadItem(4, ENTITY_ID_ITEM_MEDIKIT, SPRITE_GRAPHICS_ID_ITEM_MEDIKIT, 54 << 4, 32 << 4);
	ITEM_LoadItem(5, ENTITY_ID_ITEM_MEDIKIT, SPRITE_GRAPHICS_ID_ITEM_MEDIKIT, 5 << 4, 3 << 4);

	ITEM_LoadItem(6, ENTITY_ID_ITEM_AMMO3, SPRITE_GRAPHICS_ID_ITEM_AMMO3, 44 << 4, 3 << 4);
	ITEM_LoadItem(7, ENTITY_ID_ITEM_AMMO3, SPRITE_GRAPHICS_ID_ITEM_AMMO3, 21 << 4, 32 << 4);
	ITEM_LoadItem(8, ENTITY_ID_ITEM_DOCU, SPRITE_GRAPHICS_ID_ITEM_DOCU, 2 << 4, 12 << 4);
	ITEM_LoadItem(9, ENTITY_ID_ITEM_AMMO4, SPRITE_GRAPHICS_ID_ITEM_AMMO4, 6 << 4, 6 << 4);


	MAP_LoadMap("MAPSCN32.DAT", 57, 48, "TSCN32.DAT", "SCN3_2_BACK.PCX", "SCN3_2_FORE.PCX", "SCN3_2_MASK.PCX", 320 * 416, 128 * 256, 128 * 128);
	GFX_LoadPalette("PALETTES.DAT", "SCN32.PCX", 256);
}
static void Scene3_LoadRoom3(void) {

	// Hall
	// - Enemy 0
	ENEMY_Load("ENEMY9.DAT", 0, 21 << 4, 38 << 4, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_UP_LEFT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_RAMPAGE, 100);
	// - Enemy 1
	ENEMY_Load("ENEMY9.DAT", 1, 24 << 4, 39 << 4, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_UP_RIGHT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_RAMPAGE, 100);
	// - Enemy 2
	ENEMY_Load("ENEMY9.DAT", 2, 27 << 4, 39 << 4, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_RIGHT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_RAMPAGE, 100);
	// - Enemy 3
	ENEMY_Load("ENEMY9.DAT", 3, 33 << 4, 39 << 4, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_UP_RIGHT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_RAMPAGE, 100);
	// - Enemy 4
	ENEMY_Load("ENEMY9.DAT", 5, 35 << 4, 38 << 4, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_DOWN_RIGHT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_RAMPAGE, 100);
	// - Enemy 5
	ENEMY_Load("ENEMY9.DAT", 6, 28 << 4, 37 << 4, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_UP_RIGHT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_RAMPAGE, 100);

	MAP_LoadMap("MAPSCN31.DAT", 70, 50, "TSCN31.DAT", "SCN3_1_BACK.PCX", "SCN3_1_FORE.PCX", "SCN3_1_MASK.PCX", 320 * 416, 128 * 256, 128 * 128);
	GFX_LoadPalette("PALETTES.DAT", "SCN31.PCX", 256);
}
static void Scene3_SetHotspotsAndEvents(void) {

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
	map->event_enabling_room1[15] = true;
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
	map->event_enabling_room2[1] = true;
	map->event_enabling_room2[2] = true;
	map->event_enabling_room2[3] = true;
	map->event_enabling_room2[4] = true;
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
	map->event_enabling_room2[15] = false;
	map->event_enabling_room2[16] = true;
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
static void Scene3_Loop(void) {
	bool end_sequence;
	int actor_tile_num;
	int sequence_step;
	int scene_step;

	VIDEO_ClearScreenBuffer();
	GFX_LoadPalette("PALETTES.DAT", "SCN31.PCX", 256);

	VIDEO_StringToScreenBuffer(50, 60, ui->txt_file[UI_TXT_SCN3I]->line[40], FONT_BIG_WHITE);
	VIDEO_StringToScreenBuffer(80, 85, ui->txt_file[UI_TXT_SCN3I]->line[41], FONT_BIG_WHITE);
	VIDEO_StringToScreenBuffer(90, 135, ui->txt_file[UI_TXT_SCN3I]->line[42], FONT_SLIM_WHITE);
	VIDEO_StringToScreenBuffer(160, 135, ui->txt_file[UI_TXT_SCN3I]->line[43], FONT_SLIM_WHITE);

	VIDEO_VSync();
	VIDEO_ScreenBufferToVRAM();
	VIDEO_FadeIn(4);

	SetDelayTime(6000);

	Scene3_LoadAssets();
	Scene3_SetHotspotsAndEvents();

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
		case 1:// Room 1. Lower floor
			Scene3_LoadRoom1();

			ACTOR_SetPosition(65 << 4, 8 << 4, ACTOR_FACING_LEFT);// Set actor position
			ACTOR_SetCombatMode(true);

			CAM_Init(map->width_px, map->height_px, 46 << 4, 0 << 4);// Initialize camera
			MAP_DrawMapToMapVideoBuffer();

			Update(true);
			Update(true);
			VIDEO_FadeIn(1);
			break;

			break;
		case 2:// Room 2. Upper floor
			Scene3_LoadRoom2();

			ACTOR_SetPosition(27 << 4, 23 << 4, ACTOR_FACING_UP);// Set actor position
			ACTOR_SetCombatMode(true);

			CAM_Init(map->width_px, map->height_px, 17 << 4, 14 << 4);// Initialize camera
			MAP_DrawMapToMapVideoBuffer();

			Update(true);
			Update(true);
			VIDEO_FadeIn(1);
			break;
		default:
			Error("Scene3_Loop function error", "Undefined room", "", ERROR_SYSTEM);
			break;
	}

	// Loop until the game is over
	while (engine.ingame) {

		switch (engine.room) {
			case 1:// Room 1. Lower floor
				   // Scene steps
				// - Event 1: Starts the party! >> Step = 1 - Keep guns
				// - Event 2: Chef spot >> Step = 2 - Starts the party!
				// - Event 3: Open red door >> Step = 3
				// - Event 4: Open blue door >> Step = 4
				// - Event 5: Open yellow door >> Step = 5
				// - Event 6: Get green key >> Step = 6
				// - Event 16: Go to the second floor >> Step = 16
				// - Event 15: Exit building

				// Hotspots
				if (!actor->mode_combat) {
					// Check point on object or enemy
					switch (cursor.point_on & 0xFF) {
						case ENTITY_ID_HSPOT:// Hotspot
							switch ((cursor.point_on >> 8) & 0xFF) {
								case 0:// No hotspot, WTF??
									break;
								case 1:// Garbage bin
									if (cursor.left_click) {
										UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN3H, 101, 102, 103, false, 100);

									} else {
										UI_ShowDescription(UI_TXT_SCN3H, 1);
									}
									break;
								case 2:// Garbage
									if (cursor.left_click) {
										UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN3H, 101, 102, 103, false, 100);

									} else {
										UI_ShowDescription(UI_TXT_SCN3H, 2);
									}
									break;
								case 3:// Sewer
									if (cursor.left_click) {
										UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN3H, 104, 105, 106, false, 100);

									} else {
										UI_ShowDescription(UI_TXT_SCN3H, 3);
									}
									break;
								case 4:// Chef
									if (cursor.left_click) {
										UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN3H, 107, 108, 109, false, 100);

									} else {
										UI_ShowDescription(UI_TXT_SCN3H, 4);
									}
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
					case 1:// Event 1. Kitchen entry
						if (map->event_enabling_room1[1]) {
							map->event_enabling_room1[1] = false;

							sequence_step = 0;
							end_sequence = false;
							while (!end_sequence) {
								switch (sequence_step) {
									case 0://
										UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN3D, 1, 2, 355, false, 100);
										sequence_step++;
										break;
									case 1://
										if (UI_IsSpeechFinished()) {
											UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN3D, 3, 4, 355, false, 100);
											sequence_step++;
										}
										break;
									case 2://
										if (UI_IsSpeechFinished()) {
											UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN3D, 5, 6, 355, false, 100);
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
							scene_step = 1;
							end_sequence = false;
						}
						break;
					case 2:// Event 2. Chef
						if (map->event_enabling_room1[2]) {
							map->event_enabling_room1[2] = false;
							sequence_step = 0;
							end_sequence = false;
							while (!end_sequence) {
								switch (sequence_step) {
									case 0://
										UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ENEMY2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN3D, 12, 13, 14, true, 100);
										sequence_step++;
										break;
									case 1://
										if (UI_IsSpeechFinished()) {
											UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN3D, 15, 16, 17, false, 100);
											sequence_step++;
										}
										break;
									case 2://
										if (UI_IsSpeechFinished()) {
											UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ENEMY2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN3D, 18, 19, 20, true, 100);
											sequence_step++;
										}
										break;
									case 3://
										if (UI_IsSpeechFinished()) {
											UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN3D, 21, 355, 355, false, 100);
											sequence_step++;
										}
										break;
									case 4://
										if (UI_IsSpeechFinished()) {
											UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ENEMY2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN3D, 22, 355, 355, true, 100);
											sequence_step++;
										}
										break;
									case 5://
										if (UI_IsSpeechFinished()) {

											ACTOR_SetCombatMode(true);
											ACTOR_SetGun(actor->sprite_num, ACTOR_GUN_TYPE_PISTOL);
											ACTOR_Reload();

											ENEMY_SetBehavior(0, ENEMY_STATUS_CHASE);
											ENEMY_SetBehavior(1, ENEMY_STATUS_STATIC_SHOOTER);
											ENEMY_SetBehavior(2, ENEMY_STATUS_CHASE);
											ENEMY_SetBehavior(3, ENEMY_STATUS_STATIC_SHOOTER);
											ENEMY_SetBehavior(4, ENEMY_STATUS_STATIC_SHOOTER);
											ENEMY_SetBehavior(5, ENEMY_STATUS_STATIC_SHOOTER);
											ENEMY_SetBehavior(6, ENEMY_STATUS_STATIC_SHOOTER);
											ENEMY_SetBehavior(7, ENEMY_STATUS_STATIC_SHOOTER);
											ENEMY_SetBehavior(8, ENEMY_STATUS_STATIC_SHOOTER);
											ENEMY_SetBehavior(9, ENEMY_STATUS_STATIC_SHOOTER);
											ENEMY_SetBehavior(10, ENEMY_STATUS_STATIC_SHOOTER);
											ENEMY_SetBehavior(11, ENEMY_STATUS_STATIC_SHOOTER);
											ENEMY_SetBehavior(12, ENEMY_STATUS_STATIC_SHOOTER);
											end_sequence = true;
										}
										break;
									default:
										end_sequence = true;
										break;
								}
								Update(false);
							}
							scene_step = 2;
							end_sequence = false;
						}
						break;
					case 3:// Event 3. Entry storeroom. Red key
						if (map->event_enabling_room1[3]) {
							map->event_enabling_room1[3] = false;

							if (actor->key_entity_id != ENTITY_ID_ITEM_KEYRED) {
								AUDIO_PlaySound(AUDIO_SHOT_FAIL_EFFECT, 1);
							} else {

								scene_step = 3;
								ACTOR_LeaveKey();
								AUDIO_PlaySound(AUDIO_OPEN_DOOR_EFFECT, 1);

								MAP_HideSwapableTile(40 << 4, 10 << 4);
								MAP_HideSwapableTile(41 << 4, 10 << 4);
								MAP_HideSwapableTile(40 << 4, 11 << 4);
								MAP_HideSwapableTile(41 << 4, 11 << 4);
							}
						}
						break;
					case 4:// Event 4. Entry blue door. Blue key
						if (map->event_enabling_room1[4]) {
							map->event_enabling_room1[4] = false;

							if (actor->key_entity_id != ENTITY_ID_ITEM_KEYBLUE) {
								AUDIO_PlaySound(AUDIO_SHOT_FAIL_EFFECT, 1);
							} else {

								scene_step = 4;
								ACTOR_LeaveKey();
								AUDIO_PlaySound(AUDIO_OPEN_DOOR_EFFECT, 1);

								MAP_HideSwapableTile(18 << 4, 19 << 4);
								MAP_HideSwapableTile(19 << 4, 19 << 4);
								MAP_HideSwapableTile(18 << 4, 20 << 4);
								MAP_HideSwapableTile(19 << 4, 20 << 4);
							}
						}
						break;
					case 5:// Event 5. Entry yellow door. Yellow key
						if (map->event_enabling_room1[5]) {
							map->event_enabling_room1[5] = false;

							if (actor->key_entity_id != ENTITY_ID_ITEM_KEYYELLOW) {
								AUDIO_PlaySound(AUDIO_SHOT_FAIL_EFFECT, 1);
							} else {

								scene_step = 5;
								ACTOR_LeaveKey();
								AUDIO_PlaySound(AUDIO_OPEN_DOOR_EFFECT, 1);

								MAP_HideSwapableTile(10 << 4, 23 << 4);
								MAP_HideSwapableTile(11 << 4, 23 << 4);
								MAP_HideSwapableTile(10 << 4, 24 << 4);
								MAP_HideSwapableTile(11 << 4, 24 << 4);
							}
						}
						break;
					case 6:// Event 6. Entry green door. Green key
						if (map->event_enabling_room1[6]) {
							map->event_enabling_room1[6] = false;

							if (actor->key_entity_id != ENTITY_ID_ITEM_KEYGREEN) {
								AUDIO_PlaySound(AUDIO_SHOT_FAIL_EFFECT, 1);
							} else {

								scene_step = 6;
								ACTOR_LeaveKey();
								AUDIO_PlaySound(AUDIO_OPEN_DOOR_EFFECT, 1);

								MAP_HideSwapableTile(47 << 4, 30 << 4);
								MAP_HideSwapableTile(48 << 4, 30 << 4);
								MAP_HideSwapableTile(47 << 4, 31 << 4);
								MAP_HideSwapableTile(48 << 4, 31 << 4);
							}
						}
						break;
					case 15:// Event 15. Exit building
						if (map->event_enabling_room1[15]) {
							map->event_enabling_room1[15] = false;

							if (scene_step < 32) {

								sequence_step = 0;
								end_sequence = false;
								while (!end_sequence) {
									switch (sequence_step) {
										case 0://
											UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN3D, 24, 25, 355, false, 100);
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
							} else {
								engine.ingame = false;

								engine.scene = 4;
								engine.room = 1;
							}
						}
						break;
					case 16:// Event 16. Go to second floor
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

							Scene3_LoadRoom2();
							ACTOR_SetPosition(27 << 4, 24 << 4, ACTOR_FACING_UP);     // Set actor position
							CAM_Init(map->width_px, map->height_px, 17 << 4, 12 << 4);// Initialize camera
							MAP_DrawMapToMapVideoBuffer();

							Update(false);
							Update(false);

							engine.room = 2;
							scene_step = 16;

							VIDEO_FadeIn(1);

							Update(false);

							sequence_step = 0;
							end_sequence = false;
							while (!end_sequence) {
								switch (sequence_step) {
									case 0://
										UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN3D, 34, 35, 36, false, 100);
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

							ENEMY_SetBehavior(0, ENEMY_STATUS_STATIC_SHOOTER);
							ENEMY_SetBehavior(1, ENEMY_STATUS_STATIC_SHOOTER);

							ENEMY_SetBehavior(2, ENEMY_STATUS_STATIC_SHOOTER);
							ENEMY_SetBehavior(3, ENEMY_STATUS_CHASE);

							ENEMY_SetBehavior(4, ENEMY_STATUS_STATIC_SHOOTER);

							ENEMY_SetBehavior(5, ENEMY_STATUS_STATIC_SHOOTER);
							ENEMY_SetBehavior(6, ENEMY_STATUS_CHASE);

							ENEMY_SetBehavior(7, ENEMY_STATUS_STATIC_SHOOTER);
							ENEMY_SetBehavior(8, ENEMY_STATUS_CHASE);

							ENEMY_SetBehavior(9, ENEMY_STATUS_STATIC_SHOOTER);
							ENEMY_SetBehavior(10, ENEMY_STATUS_CHASE);

							ENEMY_SetBehavior(11, ENEMY_STATUS_STATIC_SHOOTER);

							ENEMY_SetBehavior(12, ENEMY_STATUS_STATIC_SHOOTER);

							ACTOR_SetCombatMode(true);
						}
						break;
					default:
						// Reenable red door event if actor did not have key
						if (!map->event_enabling_room1[3] && (scene_step < 3)) {
							map->event_enabling_room1[3] = true;
						}

						// Reenable blue door event if actor did not have key
						if (!map->event_enabling_room1[4] && (scene_step < 4)) {
							map->event_enabling_room1[4] = true;
						}

						// Reenable yellow door event if actor did not have key
						if (!map->event_enabling_room1[5] && (scene_step < 5)) {
							map->event_enabling_room1[5] = true;
						}

						// Reenable green door event if actor did not have key
						if (!map->event_enabling_room1[6] && (scene_step < 6)) {
							map->event_enabling_room1[6] = true;
						}

						// Reenable end of level event if actor leave event
						if (!map->event_enabling_room1[15] && (scene_step != 32)) {
							map->event_enabling_room1[15] = true;
						}

						// Reenable go to second floor event if actor leave event
						if (!map->event_enabling_room1[16] && (scene_step == 32)) {
							map->event_enabling_room1[16] = true;
						}
						break;
				}
				break;

			case 2:// Room 2. Upper floor
				// Scene steps
				// - Event 1: Open red door >> Step = 17
				// - Event 4: Open green door >> Step = 18
				// - Event 5: Open yellow door >> Step = 19
				// - Event 6: Get blue key >> Step = 20
				// - Take documents >> Step = 31

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
					case 1:// Event 1. Red door
						if (map->event_enabling_room2[1]) {
							map->event_enabling_room2[1] = false;

							if (actor->key_entity_id != ENTITY_ID_ITEM_KEYRED) {
								AUDIO_PlaySound(AUDIO_SHOT_FAIL_EFFECT, 1);
							} else {

								scene_step = 17;
								ACTOR_LeaveKey();
								AUDIO_PlaySound(AUDIO_OPEN_DOOR_EFFECT, 1);

								MAP_HideSwapableTile(21 << 4, 16 << 4);
								MAP_HideSwapableTile(22 << 4, 16 << 4);
								MAP_HideSwapableTile(21 << 4, 17 << 4);
								MAP_HideSwapableTile(22 << 4, 17 << 4);
							}
						}

						break;
					case 2:// Event 2. Green door
						if (map->event_enabling_room2[2]) {
							map->event_enabling_room2[2] = false;

							if (actor->key_entity_id != ENTITY_ID_ITEM_KEYGREEN) {
								AUDIO_PlaySound(AUDIO_SHOT_FAIL_EFFECT, 1);
							} else {

								scene_step = 18;
								ACTOR_LeaveKey();
								AUDIO_PlaySound(AUDIO_OPEN_DOOR_EFFECT, 1);

								MAP_HideSwapableTile(46 << 4, 9 << 4);
								MAP_HideSwapableTile(47 << 4, 9 << 4);
								MAP_HideSwapableTile(46 << 4, 10 << 4);
								MAP_HideSwapableTile(47 << 4, 10 << 4);
							}
						}
						break;
					case 3:// Event 3. Yellow door
						if (map->event_enabling_room2[3]) {
							map->event_enabling_room2[3] = false;

							if (actor->key_entity_id != ENTITY_ID_ITEM_KEYYELLOW) {
								AUDIO_PlaySound(AUDIO_SHOT_FAIL_EFFECT, 1);
							} else {

								scene_step = 19;
								ACTOR_LeaveKey();
								AUDIO_PlaySound(AUDIO_OPEN_DOOR_EFFECT, 1);

								MAP_HideSwapableTile(5 << 4, 18 << 4);
								MAP_HideSwapableTile(6 << 4, 18 << 4);
								MAP_HideSwapableTile(5 << 4, 19 << 4);
								MAP_HideSwapableTile(6 << 4, 19 << 4);
							}
						}
						break;
					case 4:// Event 4. Blue door
						if (map->event_enabling_room2[4]) {
							map->event_enabling_room2[4] = false;

							if (actor->key_entity_id != ENTITY_ID_ITEM_KEYBLUE) {
								AUDIO_PlaySound(AUDIO_SHOT_FAIL_EFFECT, 1);
							} else {

								scene_step = 20;
								ACTOR_LeaveKey();
								AUDIO_PlaySound(AUDIO_OPEN_DOOR_EFFECT, 1);

								MAP_HideSwapableTile(51 << 4, 29 << 4);
								MAP_HideSwapableTile(52 << 4, 29 << 4);
								MAP_HideSwapableTile(51 << 4, 30 << 4);
								MAP_HideSwapableTile(52 << 4, 30 << 4);
							}
						}
						break;
					case 16:// Event 16. Go to first floor
						if (map->event_enabling_room2[16]) {
							map->event_enabling_room2[16] = false;

							if (scene_step < 31) {
								sequence_step = 0;
								end_sequence = false;
								while (!end_sequence) {
									switch (sequence_step) {
										case 0://
											UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN3D, 24, 25, 355, false, 100);
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
							} else {// Go back to floor 1

								VIDEO_FadeOut(4);
								ENEMY_UnloadEnemies();
								OBJECT_UnloadObjects();
								EFFECT_UnloadEffects();
								BULLET_UnloadBullets();
								PARTICLE_UnloadParticles();
								GRENADE_UnloadGrenades();
								NPC_UnloadNpcs();
								MAP_UnloadMap();

								Scene3_LoadRoom3();
								ACTOR_SetPosition(27 << 4, 32 << 4, ACTOR_FACING_DOWN);   // Set actor position
								CAM_Init(map->width_px, map->height_px, 17 << 4, 28 << 4);// Initialize camera
								MAP_DrawMapToMapVideoBuffer();

								Update(false);
								Update(false);

								engine.room = 1;
								scene_step = 32;

								VIDEO_FadeIn(1);

								// Enemies chat
								sequence_step = 0;
								end_sequence = false;
								while (!end_sequence) {
									switch (sequence_step) {
										case 0://
											UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ENEMY2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN3D, 31, 32, 355, true, 100);
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

								ACTOR_SetCombatMode(true);
							}
						}
						break;
					default:
						// Reenable red door event if actor did not have key
						if (!map->event_enabling_room2[1] && (scene_step < 17)) {
							map->event_enabling_room2[1] = true;
						}

						// Reenable green door event if actor did not have key
						if (!map->event_enabling_room2[2] && (scene_step < 18)) {
							map->event_enabling_room2[2] = true;
						}

						// Reenable yellow door event if actor did not have key
						if (!map->event_enabling_room2[3] && (scene_step < 19)) {
							map->event_enabling_room2[3] = true;
						}

						// Reenable blue door event if actor did not have key
						if (!map->event_enabling_room2[4] && (scene_step < 20)) {
							map->event_enabling_room2[4] = true;
						}

						// Reenable go to back to first floor event if actor leave event
						if (!map->event_enabling_room2[16] && (scene_step < 31)) {
							map->event_enabling_room2[16] = true;
						}
						break;
				}

				// If actor find documents. End of level
				if ((actor->key_entity_id == ENTITY_ID_ITEM_DOCU) && (scene_step < 31)) {
					scene_step = 31;

					sequence_step = 0;
					end_sequence = false;
					while (!end_sequence) {
						switch (sequence_step) {
							case 0://
								UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN3D, 26, 27, 355, false, 100);
								sequence_step++;
								break;
							case 1://
								if (UI_IsSpeechFinished()) {
									UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN3D, 28, 29, 355, false, 100);
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

					ACTOR_SetCombatMode(true);
				}
				break;
		}

		Update(true);

		// Finish all if actor is dead
		if (actor->status_dead) GameOver();
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
	NPC_UnloadNpcs();
	ITEM_UnloadItems();
	ACTOR_UnloadActor();
	MAP_UnloadMap();
	Scene3_UnloadAssets();
	MM_PopChunks(CT_TEMPORARY_SPRITE);
}
static void Scene3_Outro(void) {
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
				AUDIO_LoadSong(AUDIO_SONG_3);
				GFX_PCXImageToBuffer("OSCN3.DAT", "OUTRO1H.PCX", 250 * 135, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("OSCN3.DAT", "OUTRO1L.PCX", 205 * 135, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				GFX_PCXImageToBuffer("OSCN3.DAT", "OUTRO2H.PCX", 150 * 130, gfx->image_buffer3, &gfx->image_buffer3_width, &gfx->image_buffer3_height);

				step++;
				break;
			case 2:// Hide loading screen
				UI_HideLoadingScreen();
				step++;
				break;
			case 3:/// Image 1 :: back home
				VIDEO_ClearScreenBuffer();
				GFX_LoadPalette("PALETTES.DAT", "OSCN3.PCX", 256);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 211, 141, 10, 40, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, 205, gfx->image_buffer1_height, 13, 43);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				VIDEO_FadeIn(4);

				TIMER_UpdateTimerTime(TIMER_AUDIO_NUMBER, 50);
				AUDIO_PlaySong(true);

				step++;
				break;
			case 4:// First image scroll
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
					VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 239);
					VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 208);

					VIDEO_StringToScreenBuffer(28, 13, ui->txt_file[UI_TXT_SCN3I]->line[1], FONT_SLIM_BLACK);
					VIDEO_StringToScreenBuffer(28, 25, ui->txt_file[UI_TXT_SCN3I]->line[2], FONT_SLIM_BLACK);

					SetDelayTime(50);
					while (!AwaitDelayTime()) {
						// Just wait
					}

					VIDEO_VSync();
					VIDEO_ScreenBufferToVRAM();
				}

				step++;
				break;
			case 5:// Just wait
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 6://Image 2
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 156, 136, 150, 60, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 153, 63);

				// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 239);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 208);
				step++;
				break;
			case 7:// Just wait
				SetDelayTime(200);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 8:// Fade first image and show second again
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 211, 141, 10, 40, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 13, 43);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 156, 136, 150, 60, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 153, 63);

				// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 239);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 208);

				VIDEO_StringToScreenBuffer(28, 13, ui->txt_file[UI_TXT_SCN3I]->line[3], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(28, 25, ui->txt_file[UI_TXT_SCN3I]->line[4], FONT_SLIM_BLACK);
				step++;
				break;
			case 9:// Load new images
				VIDEO_ClearScreenBuffer();
				GFX_PCXImageToBuffer("OSCN3.DAT", "OUTRO3H.PCX", 150 * 130, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("OSCN3.DAT", "OUTRO4H.PCX", 251 * 160, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				step++;
				break;
			case 10:// Show image 3
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 156, 136, 160, 40, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 163, 43);
				step++;
				break;
			case 11:// Just wait
				SetDelayTime(2000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 12://Girl's chat
				chat_spr_num = GFX_FindEmptySpriteSlot();
				if (chat_spr_num == -1) {
					sprintf(engine.system_error_message1, "Scene 3 Outtro function error");
					sprintf(engine.system_error_message2, "No empty sprite slot available");
					sprintf(engine.system_error_message3, " ");
					Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
				} else {
					GFX_InitSprite(ENTITY_ID_EMPTY, 0, chat_spr_num, 0, 65, 30);
				}
				GFX_SetSpriteGraphic(chat_spr_num, 0, SPRITE_GRAPHICS_ID_CHAT_BIG, 0, 0);
				GFX_SetDefaultAnimation(chat_spr_num, true, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 50, 50);
				char_number = 0;
				step++;
				break;
			case 13://Girl's chat
				VIDEO_StringToScreenBuffer(56, 55, ui->txt_file[UI_TXT_SCN3I]->line[6], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(56, 70, ui->txt_file[UI_TXT_SCN3I]->line[7], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(56, 85, ui->txt_file[UI_TXT_SCN3I]->line[8], FONT_SLIM_BLACK);
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
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 156, 136, 160, 40, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 163, 43);
				step++;
				break;
			case 15:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 16:// Show image 4
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 257, 166, 30, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 23);
				step++;
				break;
			case 17:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 18:// Actors chat
				GFX_SetDefaultAnimation(chat_spr_num, false, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 100, 20);
				GFX_ShowSprite(chat_spr_num);
				char_number = 0;
				step++;
				break;
			case 19:// Actors chat
				VIDEO_StringToScreenBuffer(106, 25, ui->txt_file[UI_TXT_SCN3I]->line[9], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 40, ui->txt_file[UI_TXT_SCN3I]->line[10], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 55, ui->txt_file[UI_TXT_SCN3I]->line[11], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 20:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 21:// Actors chat
				VIDEO_StringToScreenBuffer(106, 25, ui->txt_file[UI_TXT_SCN3I]->line[13], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 40, ui->txt_file[UI_TXT_SCN3I]->line[14], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 55, ui->txt_file[UI_TXT_SCN3I]->line[15], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 22:// End chat
				GFX_HideSprite(chat_spr_num);
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 257, 166, 30, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 23);
				step++;
				break;
			case 23:// Just wait
				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 24://Girl's chat
				GFX_SetSpriteGraphic(chat_spr_num, 0, SPRITE_GRAPHICS_ID_CHAT_BIG, 0, 0);
				GFX_SetDefaultAnimation(chat_spr_num, true, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 70, 50);
				char_number = 0;
				step++;
				break;
			case 25://Girl's chat
				VIDEO_StringToScreenBuffer(76, 55, ui->txt_file[UI_TXT_SCN3I]->line[16], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(76, 70, ui->txt_file[UI_TXT_SCN3I]->line[17], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(76, 85, ui->txt_file[UI_TXT_SCN3I]->line[18], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 26:// End chat
				GFX_HideSprite(chat_spr_num);
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 257, 166, 30, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 23);
				step++;
				break;
			case 27:// Just wait
				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 28:// Actors chat
				GFX_SetDefaultAnimation(chat_spr_num, false, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 100, 20);
				GFX_ShowSprite(chat_spr_num);
				char_number = 0;
				step++;
				break;
			case 29:// Actors chat
				VIDEO_StringToScreenBuffer(106, 25, ui->txt_file[UI_TXT_SCN3I]->line[19], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 40, ui->txt_file[UI_TXT_SCN3I]->line[20], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 55, ui->txt_file[UI_TXT_SCN3I]->line[21], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 30:// End chat
				GFX_HideSprite(chat_spr_num);
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 257, 166, 30, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 23);
				step++;
				break;
			case 31:// Just wait
				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 32://Girl's chat
				GFX_SetSpriteGraphic(chat_spr_num, 0, SPRITE_GRAPHICS_ID_CHAT_BIG, 0, 0);
				GFX_SetDefaultAnimation(chat_spr_num, true, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 70, 50);
				char_number = 0;
				step++;
				break;
			case 33://Girl's chat
				VIDEO_StringToScreenBuffer(76, 55, ui->txt_file[UI_TXT_SCN3I]->line[22], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(76, 70, ui->txt_file[UI_TXT_SCN3I]->line[23], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(76, 85, ui->txt_file[UI_TXT_SCN3I]->line[24], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 34:// End chat
				GFX_HideSprite(chat_spr_num);
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 257, 166, 30, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 23);
				step++;
				break;
			case 35:// Just wait
				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 36:// Actors chat
				GFX_SetDefaultAnimation(chat_spr_num, false, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 100, 20);
				GFX_ShowSprite(chat_spr_num);
				char_number = 0;
				step++;
				break;
			case 37:// Actors chat
				VIDEO_StringToScreenBuffer(106, 25, ui->txt_file[UI_TXT_SCN3I]->line[25], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 40, ui->txt_file[UI_TXT_SCN3I]->line[26], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 55, ui->txt_file[UI_TXT_SCN3I]->line[27], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 38:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 39:// Actors chat
				VIDEO_StringToScreenBuffer(106, 25, ui->txt_file[UI_TXT_SCN3I]->line[28], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 40, ui->txt_file[UI_TXT_SCN3I]->line[29], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 55, ui->txt_file[UI_TXT_SCN3I]->line[30], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 40:// End chat
				GFX_HideSprite(chat_spr_num);
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 257, 166, 30, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 23);
				SetDelayTime(200);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 41://Girl's chat
				GFX_SetSpriteGraphic(chat_spr_num, 0, SPRITE_GRAPHICS_ID_CHAT_BIG, 0, 0);
				GFX_SetDefaultAnimation(chat_spr_num, true, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 70, 50);
				char_number = 0;
				step++;
				break;
			case 42://Girl's chat
				VIDEO_StringToScreenBuffer(76, 55, ui->txt_file[UI_TXT_SCN3I]->line[31], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(76, 70, ui->txt_file[UI_TXT_SCN3I]->line[32], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(76, 85, ui->txt_file[UI_TXT_SCN3I]->line[33], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 43:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 44://Girl's chat
				VIDEO_StringToScreenBuffer(76, 55, ui->txt_file[UI_TXT_SCN3I]->line[34], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(76, 70, ui->txt_file[UI_TXT_SCN3I]->line[35], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(76, 85, ui->txt_file[UI_TXT_SCN3I]->line[36], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 45:// End chat
				GFX_HideSprite(chat_spr_num);
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 257, 166, 30, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 23);
				SetDelayTime(200);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 46:// Actors chat
				GFX_SetDefaultAnimation(chat_spr_num, false, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 100, 20);
				GFX_ShowSprite(chat_spr_num);
				char_number = 0;
				step++;
				break;
			case 47:// Actors chat
				VIDEO_StringToScreenBuffer(106, 25, ui->txt_file[UI_TXT_SCN3I]->line[44], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 40, ui->txt_file[UI_TXT_SCN3I]->line[45], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 55, ui->txt_file[UI_TXT_SCN3I]->line[46], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 48:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 49:// Actors chat
				VIDEO_StringToScreenBuffer(106, 25, ui->txt_file[UI_TXT_SCN3I]->line[47], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 40, ui->txt_file[UI_TXT_SCN3I]->line[48], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 55, ui->txt_file[UI_TXT_SCN3I]->line[49], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 50:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 51:// End chat
				GFX_HideSprite(chat_spr_num);
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 257, 166, 30, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 23);
				SetDelayTime(200);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 52://Girl's chat
				GFX_SetSpriteGraphic(chat_spr_num, 0, SPRITE_GRAPHICS_ID_CHAT_BIG, 0, 0);
				GFX_SetDefaultAnimation(chat_spr_num, true, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 70, 50);
				char_number = 0;
				step++;
				break;
			case 53://Girl's chat
				VIDEO_StringToScreenBuffer(76, 55, ui->txt_file[UI_TXT_SCN3I]->line[50], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(76, 70, ui->txt_file[UI_TXT_SCN3I]->line[51], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(76, 85, ui->txt_file[UI_TXT_SCN3I]->line[52], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 54:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 55://Girl's chat
				VIDEO_StringToScreenBuffer(76, 55, ui->txt_file[UI_TXT_SCN3I]->line[53], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(76, 70, ui->txt_file[UI_TXT_SCN3I]->line[54], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(76, 85, ui->txt_file[UI_TXT_SCN3I]->line[55], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 56:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 57://Girl's chat
				VIDEO_StringToScreenBuffer(76, 55, ui->txt_file[UI_TXT_SCN3I]->line[56], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(76, 70, ui->txt_file[UI_TXT_SCN3I]->line[57], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(76, 85, ui->txt_file[UI_TXT_SCN3I]->line[58], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 58:// End chat
				GFX_HideSprite(chat_spr_num);
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 257, 166, 30, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 23);
				SetDelayTime(200);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 59:// Just wait
				SetDelayTime(2000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 60:// Actors chat
				GFX_SetDefaultAnimation(chat_spr_num, false, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 100, 20);
				GFX_ShowSprite(chat_spr_num);
				char_number = 0;
				step++;
				break;
			case 61:// Actors chat
				VIDEO_StringToScreenBuffer(106, 25, ui->txt_file[UI_TXT_SCN3I]->line[59], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 40, ui->txt_file[UI_TXT_SCN3I]->line[60], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 55, ui->txt_file[UI_TXT_SCN3I]->line[61], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 62:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 63:// Actors chat
				VIDEO_StringToScreenBuffer(106, 25, ui->txt_file[UI_TXT_SCN3I]->line[62], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 40, ui->txt_file[UI_TXT_SCN3I]->line[63], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 55, ui->txt_file[UI_TXT_SCN3I]->line[64], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 64:// End chat
				GFX_HideSprite(chat_spr_num);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 257, 166, 30, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 23);
				SetDelayTime(200);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 65://Girl's chat
				GFX_SetSpriteGraphic(chat_spr_num, 0, SPRITE_GRAPHICS_ID_CHAT_BIG, 0, 0);
				GFX_SetDefaultAnimation(chat_spr_num, true, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 50, 50);
				char_number = 0;
				step++;
				break;
			case 66://Girl's chat
				VIDEO_StringToScreenBuffer(56, 55, ui->txt_file[UI_TXT_SCN3I]->line[65], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(56, 70, ui->txt_file[UI_TXT_SCN3I]->line[66], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(56, 85, ui->txt_file[UI_TXT_SCN3I]->line[67], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 67:// End chat
				GFX_HideSprite(chat_spr_num);
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 257, 166, 30, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 23);
				SetDelayTime(200);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 68:// Actors chat
				GFX_SetDefaultAnimation(chat_spr_num, false, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 100, 20);
				GFX_ShowSprite(chat_spr_num);
				char_number = 0;
				step++;
				break;
			case 69:// Actors chat
				VIDEO_StringToScreenBuffer(106, 25, ui->txt_file[UI_TXT_SCN3I]->line[66], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 70:// End chat
				GFX_HideSprite(chat_spr_num);
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 257, 166, 30, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 23);
				SetDelayTime(200);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 71:// Load new images
				VIDEO_ClearScreenBuffer();
				GFX_PCXImageToBuffer("OSCN3.DAT", "OUTRO5H.PCX", 150 * 130, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("OSCN3.DAT", "OUTRO6H.PCX", 150 * 130, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				step++;
				break;
			case 72:// Show images 5 and 6
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 156, 136, 2, 40, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 5, 43);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 156, 136, 160, 40, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 163, 43);
				step++;
				break;
			case 73:// Just wait
				SetDelayTime(2000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 74:// Clear image 6 and show just 5
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 156, 136, 2, 40, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 5, 43);
				step++;
				break;
			case 75:// Show speech
				// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 239);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 208);

				VIDEO_StringToScreenBuffer(28, 13, ui->txt_file[UI_TXT_SCN3I]->line[68], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(28, 25, ui->txt_file[UI_TXT_SCN3I]->line[69], FONT_SLIM_BLACK);
				step++;
				break;
			case 76:// Just wait
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 77:// Clear speech
				// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 239);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 208);
				step++;
				break;
			case 78:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 79:// Show speech
				// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 239);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 208);

				VIDEO_StringToScreenBuffer(28, 13, ui->txt_file[UI_TXT_SCN3I]->line[70], FONT_SLIM_BLACK);
				step++;
				break;
			case 80:// Just wait
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 81:// Clear speech
				// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 239);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 208);
				step++;
				break;
			case 82:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 83:// Show speech
				// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 239);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 208);

				VIDEO_StringToScreenBuffer(28, 13, ui->txt_file[UI_TXT_SCN3I]->line[72], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(28, 25, ui->txt_file[UI_TXT_SCN3I]->line[73], FONT_SLIM_BLACK);
				step++;
				break;
			case 84:// Just wait
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 85:// Clear speech
				// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 239);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 208);
				step++;
				break;
			case 86:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 87:// Show speech
				// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 239);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 208);

				VIDEO_StringToScreenBuffer(28, 13, ui->txt_file[UI_TXT_SCN3I]->line[74], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(28, 25, ui->txt_file[UI_TXT_SCN3I]->line[75], FONT_SLIM_BLACK);
				step++;
				break;
			case 88:// Just wait
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 89:// Clear speech
				// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 239);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 208);
				step++;
				break;
			case 90://End sequence
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

/** SCENARIO 4 :: Mission 4 :: Praise for mercy
 */
static void Scene4_Intro(void) {
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
				GFX_PCXImageToBuffer("ISCN4.DAT", "INTRO1H.PCX", 120 * 140, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("ISCN4.DAT", "INTRO1L.PCX", 120 * 140, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				GFX_PCXImageToBuffer("ISCN4.DAT", "INTRO2H.PCX", 120 * 140, gfx->image_buffer3, &gfx->image_buffer3_width, &gfx->image_buffer3_height);
				AUDIO_LoadSong(AUDIO_SONG_3);
				step++;
				break;
			case 2:// Hide loading screen
				UI_HideLoadingScreen();
				step++;
				break;
			case 3:/// Scene 1 :: Entering kitchen
				VIDEO_ClearScreenBuffer();
				GFX_LoadPalette("PALETTES.DAT", "ISCN4.PCX", 256);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				VIDEO_FadeIn(4);

				TIMER_UpdateTimerTime(TIMER_AUDIO_NUMBER, 50);
				AUDIO_PlaySong(true);

				step++;
				break;
			case 4:// Draw frame and image 1
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 126, 146, 20, 50, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 23, 53);
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
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 296, 46, 5, 1, 239);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 40, 8, 4, 208);
				VIDEO_StringToScreenBuffer(12, 12, ui->txt_file[UI_TXT_SCN4I]->line[1], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(12, 22, ui->txt_file[UI_TXT_SCN4I]->line[2], FONT_SLIM_BLACK);
				step++;
				break;
			case 7:// Just wait
				SetDelayTime(6000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 8:// Hide text Draw frame and image 2
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 296, 46, 5, 1, 239);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 40, 9, 4, 208);

				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 126, 146, 170, 50, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 173, 53);
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
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 296, 46, 5, 1, 239);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 40, 9, 4, 208);
				VIDEO_StringToScreenBuffer(12, 12, ui->txt_file[UI_TXT_SCN4I]->line[3], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(12, 22, ui->txt_file[UI_TXT_SCN4I]->line[4], FONT_SLIM_BLACK);
				step++;
				break;
			case 11:// Fade image 1 and show 2
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 126, 146, 20, 50, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 23, 53);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 126, 146, 170, 50, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 173, 53);

				step++;
				break;
			case 12:// Just wait
				SetDelayTime(6000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 13:// Hide text
					// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 296, 46, 5, 1, 239);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 40, 9, 4, 208);

				step++;
				break;
			case 14:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 15:// Show text
					// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 296, 46, 5, 1, 239);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 40, 9, 4, 208);
				VIDEO_StringToScreenBuffer(12, 12, ui->txt_file[UI_TXT_SCN4I]->line[5], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(12, 22, ui->txt_file[UI_TXT_SCN4I]->line[6], FONT_SLIM_BLACK);
				step++;
				break;
			case 16:
				SetDelayTime(6000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 17:// Just wait
				GFX_PCXImageToBuffer("ISCN4.DAT", "INTRO3H.PCX", 250 * 135, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				step++;
				break;
			case 18:// Hide text and Show image 3
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 296, 46, 5, 1, 239);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 40, 9, 4, 208);

				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 256, 141, 40, 50, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 43, 53);

				step++;
				break;
			case 19:
				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 20:// Show text
					// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 46, 5, 1, 239);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 293, 40, 9, 4, 208);
				VIDEO_StringToScreenBuffer(12, 12, ui->txt_file[UI_TXT_SCN4I]->line[10], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(12, 22, ui->txt_file[UI_TXT_SCN4I]->line[11], FONT_SLIM_BLACK);
				step++;
				break;
			case 21:// Just wait
				SetDelayTime(6000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 22:// Hide text
					// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 46, 5, 1, 239);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 293, 40, 9, 4, 208);

				step++;
				break;
			case 23:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 24:// Show text
					// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 46, 5, 1, 239);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 293, 40, 9, 4, 208);
				VIDEO_StringToScreenBuffer(12, 12, ui->txt_file[UI_TXT_SCN4I]->line[12], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(12, 22, ui->txt_file[UI_TXT_SCN4I]->line[13], FONT_SLIM_BLACK);
				step++;
				break;
			case 25:// Just wait
				SetDelayTime(6000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 26:// Hide text
					// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 46, 5, 1, 239);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 293, 40, 9, 4, 208);

				step++;
				break;
			case 27:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 28:// Show text
					// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 46, 5, 1, 239);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 293, 40, 9, 4, 208);
				VIDEO_StringToScreenBuffer(12, 12, ui->txt_file[UI_TXT_SCN4I]->line[15], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(12, 22, ui->txt_file[UI_TXT_SCN4I]->line[16], FONT_SLIM_BLACK);
				step++;
				break;
			case 29:// Just wait
				SetDelayTime(6000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 30:// Hide text
					// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 46, 5, 1, 239);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 293, 40, 9, 4, 208);

				step++;
				break;
			case 31:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 32:// Show text
					// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 46, 5, 1, 239);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 293, 40, 9, 4, 208);
				VIDEO_StringToScreenBuffer(12, 8, ui->txt_file[UI_TXT_SCN4I]->line[17], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(12, 18, ui->txt_file[UI_TXT_SCN4I]->line[18], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(12, 28, ui->txt_file[UI_TXT_SCN4I]->line[19], FONT_SLIM_BLACK);
				step++;
				break;
			case 33:// Just wait
				SetDelayTime(6000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 34://End sequence
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
static void Scene4_LoadAssets(void) {

	// Load enemies and unload them just to get graphics
	ENEMY_Load("ENEMY10.DAT", 0, 0, 0, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_GUN0, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_NPC, 1);
	ENEMY_UnloadEnemies();

	// Load boss and unload it just to preload graphics
	BOSS_Load("BOSS3.DAT", 0, 0, BOSS_TYPE_SLOW, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_BULLET3, BOSS_STATUS_STATIC, BOSS_LIFE);
	BOSS_Unload();

	ACTOR_Load(9 << 4, 47 << 4, SPRITE_GRAPHICS_ID_ACTOR_FEET, SPRITE_GRAPHICS_ID_ACTOR_BODY, SPRITE_GRAPHICS_ID_ACTOR_HEAD, SPRITE_GRAPHICS_ID_ACTOR_LARM, SPRITE_GRAPHICS_ID_ACTOR_RARM_BARE_HANDS, ACTOR_FACING_RIGHT);
	ACTOR_SetGun(actor->sprite_num, ACTOR_GUN_TYPE_BARE_HANDS);
	ACTOR_SetBulletStatus(99, 19, 199, 9, 9);

	UI_SetStatusPanels();// Set status panels

	AUDIO_LoadSong(AUDIO_SONG_3);// Load song
}
static void Scene4_UnloadAssets(void) {
	// Enemy 1
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_CHAT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_FEET);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_BODY);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_HEAD);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_RARM);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_LARM);
}
static void Scene4_LoadRoom1(void) {

	// First stairs
	ENEMY_Load("ENEMY10.DAT", 0, 24 << 4, 44 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 50);
	// Second stairs
	ENEMY_Load("ENEMY10.DAT", 1, 3 << 4, 39 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 50);
	ENEMY_Load("ENEMY10.DAT", 2, 10 << 4, 39 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 50);
	ENEMY_Load("ENEMY10.DAT", 3, 22 << 4, 39 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 50);

	// Garden 1
	ENEMY_Load("ENEMY10.DAT", 4, 10 << 4, 30 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 50);
	ENEMY_Load("ENEMY10.DAT", 5, 12 << 4, 26 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 50);
	ENEMY_Load("ENEMY10.DAT", 6, 19 << 4, 27 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 50);
	ENEMY_Load("ENEMY10.DAT", 7, 24 << 4, 28 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 50);
	ENEMY_Load("ENEMY10.DAT", 8, 19 << 4, 20 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 50);
	ENEMY_Load("ENEMY10.DAT", 9, 12 << 4, 23 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 50);
	ENEMY_Load("ENEMY10.DAT", 10, 4 << 4, 14 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 50);
	ENEMY_Load("ENEMY10.DAT", 11, 16 << 4, 15 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 50);
	ENEMY_Load("ENEMY10.DAT", 12, 24 << 4, 15 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 50);

	// Garden 2
	ENEMY_Load("ENEMY10.DAT", 13, 34 << 4, 14 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_CHASE, 50);
	ENEMY_Load("ENEMY10.DAT", 14, 39 << 4, 21 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_CHASE, 50);
	ENEMY_Load("ENEMY10.DAT", 15, 38 << 4, 34 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_CHASE, 50);

	// stairs 3
	ENEMY_Load("ENEMY10.DAT", 16, 35 << 4, 45 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 50);
	ENEMY_Load("ENEMY10.DAT", 17, 42 << 4, 45 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 50);

	// Upstairs
	ENEMY_Load("ENEMY10.DAT", 18, 47 << 4, 36 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 50);
	ENEMY_Load("ENEMY10.DAT", 19, 45 << 4, 24 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 50);

	// Door
	ENEMY_Load("ENEMY10.DAT", 20, 20 << 4, 5 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 50);
	ENEMY_Load("ENEMY10.DAT", 21, 29 << 4, 5 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 50);

	// Items
	ITEM_LoadItem(0, ENTITY_ID_ITEM_MEDIKIT, SPRITE_GRAPHICS_ID_ITEM_MEDIKIT, 34 << 4, 12 << 4);
	ITEM_LoadItem(1, ENTITY_ID_ITEM_MEDIKIT, SPRITE_GRAPHICS_ID_ITEM_MEDIKIT, 1 << 4, 5 << 4);
	ITEM_LoadItem(2, ENTITY_ID_ITEM_AMMO1, SPRITE_GRAPHICS_ID_ITEM_AMMO1, 13 << 4, 12 << 4);
	ITEM_LoadItem(3, ENTITY_ID_ITEM_AMMO3, SPRITE_GRAPHICS_ID_ITEM_AMMO3, 34 << 4, 38 << 4);
	ITEM_LoadItem(4, ENTITY_ID_ITEM_GRENADE, SPRITE_GRAPHICS_ID_ITEM_GRENADE, 1 << 4, 8 << 4);

	MAP_LoadMap("MAPSCN41.DAT", 50, 50, "TSCN41.DAT", "SCN4_1_BACK.PCX", "SCN4_1_FORE.PCX", "SCN4_1_MASK.PCX", 320 * 416, 128 * 128, 128 * 128);
	GFX_LoadPalette("PALETTES.DAT", "SCN41.PCX", 256);
}
static void Scene4_LoadRoom2(void) {

	ENEMY_Load("ENEMY10.DAT", 0, 15 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 50);
	ENEMY_Load("ENEMY10.DAT", 1, 18 << 4, 15 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 50);
	ENEMY_Load("ENEMY10.DAT", 2, 29 << 4, 15 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 50);
	ENEMY_Load("ENEMY10.DAT", 3, 31 << 4, 12 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 50);

	MAP_LoadMap("MAPSCN42.DAT", 47, 32, "TSCN42.DAT", "SCN4_2_BACK.PCX", "SCN4_2_FORE.PCX", "SCN4_2_MASK.PCX", 320 * 416, 128 * 128, 128 * 128);
	GFX_LoadPalette("PALETTES.DAT", "SCN42.PCX", 256);
}
static void Scene4_SetHotspotsAndEvents(void) {
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
	map->event_enabling_room2[1] = true;
	map->event_enabling_room2[2] = true;
	map->event_enabling_room2[3] = true;
	map->event_enabling_room2[4] = true;
	map->event_enabling_room2[5] = true;
	map->event_enabling_room2[6] = false;
	map->event_enabling_room2[7] = false;
	map->event_enabling_room2[8] = false;
	map->event_enabling_room2[9] = false;
	map->event_enabling_room2[10] = false;
	map->event_enabling_room2[11] = false;
	map->event_enabling_room2[12] = false;
	map->event_enabling_room2[13] = false;
	map->event_enabling_room2[14] = false;
	map->event_enabling_room2[15] = false;
	map->event_enabling_room2[16] = true;
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
static void Scene4_Loop(void) {
	bool end_sequence;
	int actor_tile_num;
	int sequence_step;
	int scene_step;

	VIDEO_ClearScreenBuffer();
	GFX_LoadPalette("PALETTES.DAT", "SCN41.PCX", 256);

	VIDEO_StringToScreenBuffer(50, 60, ui->txt_file[UI_TXT_SCN4I]->line[40], FONT_BIG_WHITE);
	VIDEO_StringToScreenBuffer(70, 85, ui->txt_file[UI_TXT_SCN4I]->line[41], FONT_BIG_WHITE);
	VIDEO_StringToScreenBuffer(90, 135, ui->txt_file[UI_TXT_SCN4I]->line[42], FONT_SLIM_WHITE);
	VIDEO_StringToScreenBuffer(160, 135, ui->txt_file[UI_TXT_SCN4I]->line[43], FONT_SLIM_WHITE);


	VIDEO_VSync();
	VIDEO_ScreenBufferToVRAM();
	VIDEO_FadeIn(4);

	SetDelayTime(6000);

	Scene4_LoadAssets();
	Scene4_SetHotspotsAndEvents();

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
		case 1:// Room 1. Garden
			Scene4_LoadRoom1();

			ACTOR_SetPosition(9 << 4, 47 << 4, ACTOR_FACING_UP);// Set actor position
			ACTOR_SetCombatMode(true);

			CAM_Init(map->width_px, map->height_px, 0 << 4, 37 << 4);// Initialize camera
			MAP_DrawMapToMapVideoBuffer();

			Update(true);
			Update(true);

			VIDEO_FadeIn(1);

			sequence_step = 0;
			end_sequence = false;
			while (!end_sequence) {
				switch (sequence_step) {
					case 0://
						UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN4D, 1, 355, 355, false, 100);
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

			ACTOR_SetCombatMode(true);
			ACTOR_SetGun(actor->sprite_num, ACTOR_GUN_TYPE_PISTOL);
			ACTOR_Reload();

			break;
		case 2:// Room 2. Church
			Scene4_LoadRoom2();

			ACTOR_SetPosition(23 << 4, 29 << 4, ACTOR_FACING_UP);     // Set actor position
			CAM_Init(map->width_px, map->height_px, 13 << 4, 20 << 4);// Initialize camera
			MAP_DrawMapToMapVideoBuffer();

			Update(true);
			Update(true);
			VIDEO_FadeIn(1);

			ACTOR_SetCombatMode(true);
			ACTOR_Reload();

			break;
		default:
			Error("Scene4_Loop function error", "Undefined room", "", ERROR_SYSTEM);
			break;
	}

	// Loop until the game is over
	while (engine.ingame) {
		switch (engine.room) {
			case 1:// Room 1. Garden
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
										//UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN4H, 120, 121, 122, false, 100);

									} else {
										//UI_ShowDescription(UI_TXT_SCN4H, 6);
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
					case 16:// Event 16. Go inside church
						if (map->event_enabling_room1[16]) {
							map->event_enabling_room1[16] = false;

							VIDEO_FadeOut(4);
							ENEMY_UnloadEnemies();
							OBJECT_UnloadObjects();
							EFFECT_UnloadEffects();
							BULLET_UnloadBullets();
							PARTICLE_UnloadParticles();
							GRENADE_UnloadGrenades();
							NPC_UnloadNpcs();
							MAP_UnloadMap();

							Scene4_LoadRoom2();
							ACTOR_SetPosition(23 << 4, 29 << 4, ACTOR_FACING_UP);     // Set actor position
							CAM_Init(map->width_px, map->height_px, 13 << 4, 20 << 4);// Initialize camera
							MAP_DrawMapToMapVideoBuffer();

							Update(true);
							Update(true);

							engine.room = 2;

							VIDEO_FadeIn(1);

							ACTOR_SetCombatMode(true);
						}
						break;
					default:
						break;
				}

				break;
			case 2:// Room 2. Inside church
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
					case 1:// Event 1. Entry
						if (map->event_enabling_room2[1]) {
							map->event_enabling_room2[1] = false;

							scene_step = 1;

							BOSS_Load("BOSS3.DAT", 23 << 4, 10 << 4, BOSS_TYPE_SLOW, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_RAY, BOSS_STATUS_STATIC, BOSS_LIFE);

							/*sequence_step = 0;
							end_sequence = false;
							while (!end_sequence) {
								switch (sequence_step) {
									case 0://
										UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN4D, 24, 25, 355, false, 100);
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
							end_sequence = false;*/

							BOSS_SetBehavior(BOSS_STATUS_CHASE);

							ENEMY_SetBehavior(0, ENEMY_STATUS_CHASE);
							ENEMY_SetBehavior(1, ENEMY_STATUS_CHASE);
							ENEMY_SetBehavior(2, ENEMY_STATUS_CHASE);
							ENEMY_SetBehavior(3, ENEMY_STATUS_CHASE);

							ACTOR_SetCombatMode(true);
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
					case 16:// Event 16.
						if (map->event_enabling_room2[16]) {
							map->event_enabling_room2[16] = false;
						}
						break;
					default:
						break;
				}

				// Scene events
				switch (scene_step) {
					case 1:// fight
						if (!enemy[0].is_loaded) ENEMY_Load("ENEMY10.DAT", 0, 1 << 4, 17 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_RAMPAGE, 50);
						if (!enemy[1].is_loaded) ENEMY_Load("ENEMY10.DAT", 1, 1 << 4, 18 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_RAMPAGE, 50);
						if (!enemy[2].is_loaded) ENEMY_Load("ENEMY10.DAT", 2, 45 << 4, 17 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_RAMPAGE, 50);
						if (!enemy[3].is_loaded) ENEMY_Load("ENEMY10.DAT", 3, 45 << 4, 18 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_RAMPAGE, 50);

						if (!boss->is_loaded) {
							ENEMY_UnloadEnemies();
							VIDEO_FadeOut(4);
							engine.ingame = false;
							engine.scene = 5;
							engine.room = 1;
						}
						break;
					default:
						break;
				}

				break;
		}

		Update(true);

		// Finish all if actor is dead
		if (actor->status_dead) GameOver();
	}

	VIDEO_FadeOut(4);
	VIDEO_ClearScreen();
	AUDIO_StopSong();
	ITEM_UnloadItems();
	BOSS_Unload();
	ENEMY_UnloadEnemies();
	OBJECT_UnloadObjects();
	EFFECT_UnloadEffects();
	BULLET_UnloadBullets();
	PARTICLE_UnloadParticles();
	GRENADE_UnloadGrenades();
	ACTOR_UnloadActor();
	MAP_UnloadMap();
	Scene4_UnloadAssets();
	MM_PopChunks(CT_TEMPORARY_SPRITE);
}
static void Scene4_Outro(void) {
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
				GFX_PCXImageToBuffer("OSCN4.DAT", "OUTRO1H.PCX", 260 * 100, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("OSCN4.DAT", "OUTRO1L.PCX", 260 * 100, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				GFX_PCXImageToBuffer("OSCN4.DAT", "OUTRO2H.PCX", 130 * 120, gfx->image_buffer3, &gfx->image_buffer3_width, &gfx->image_buffer3_height);
				AUDIO_LoadSong(AUDIO_SONG_3);
				step++;
				break;
			case 2:// Hide loading screen
				UI_HideLoadingScreen();
				step++;
				break;
			case 3:///
				VIDEO_ClearScreenBuffer();
				GFX_LoadPalette("PALETTES.DAT", "OSCN4.PCX", 256);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				VIDEO_FadeIn(4);

				TIMER_UpdateTimerTime(TIMER_AUDIO_NUMBER, 50);
				AUDIO_PlaySong(true);

				step++;
				break;
			case 4:// First image
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 266, 106, 30, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 33, 13);
				step++;
				break;
			case 5:// Just wait
				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 6:// Cult boss chat
				chat_spr_num = GFX_FindEmptySpriteSlot();
				if (chat_spr_num == -1) {
					sprintf(engine.system_error_message1, "Scene 4 Outtro function error");
					sprintf(engine.system_error_message2, "No empty sprite slot available");
					sprintf(engine.system_error_message3, " ");
					Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
				} else {
					GFX_InitSprite(ENTITY_ID_EMPTY, 0, chat_spr_num, 0, 65, 30);
				}
				GFX_SetSpriteGraphic(chat_spr_num, 0, SPRITE_GRAPHICS_ID_CHAT_BIG, 0, 0);
				GFX_SetDefaultAnimation(chat_spr_num, true, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 100, 10);
				char_number = 0;
				step++;
				break;
			case 7://Cult boss chat
				VIDEO_StringToScreenBuffer(106, 15, ui->txt_file[UI_TXT_SCN4I]->line[25], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 30, ui->txt_file[UI_TXT_SCN4I]->line[26], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(6000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 8:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 9://Cult boss chat
				VIDEO_StringToScreenBuffer(106, 15, ui->txt_file[UI_TXT_SCN4I]->line[27], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 30, ui->txt_file[UI_TXT_SCN4I]->line[28], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(6000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 10:// End chat
				GFX_HideSprite(chat_spr_num);
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 266, 106, 30, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 33, 13);
				step++;
				break;
			case 11:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 12:// Show image 2
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 266, 106, 30, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 33, 13);

				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 136, 126, 20, 70, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 23, 73);
				step++;
				break;
			case 13:
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 14:// Fade first image
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 266, 106, 30, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 13);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 136, 126, 20, 70, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 23, 73);
				step++;
				break;
			case 15:// Actors chat
				GFX_SetDefaultAnimation(chat_spr_num, false, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 100, 80);
				GFX_ShowSprite(chat_spr_num);
				char_number = 0;
				step++;
				break;
			case 16:// Actors chat
				VIDEO_StringToScreenBuffer(106, 85, ui->txt_file[UI_TXT_SCN4I]->line[29], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 100, ui->txt_file[UI_TXT_SCN4I]->line[30], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 17:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 18:// Actors chat
				VIDEO_StringToScreenBuffer(106, 85, ui->txt_file[UI_TXT_SCN4I]->line[31], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 100, ui->txt_file[UI_TXT_SCN4I]->line[32], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 115, ui->txt_file[UI_TXT_SCN4I]->line[33], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(6000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 19:// End chat
				GFX_PCXImageToBuffer("OSCN4.DAT", "OUTRO3H.PCX", 130 * 120, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);

				GFX_HideSprite(chat_spr_num);
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 266, 106, 30, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 13);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 136, 126, 20, 70, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 23, 73);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 136, 126, 170, 70, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 173, 73);
				step++;
				break;
			case 20:// Just wait
				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 21:// Cult boss chat
				GFX_SetSpriteGraphic(chat_spr_num, 0, SPRITE_GRAPHICS_ID_CHAT_BIG, 0, 0);
				GFX_SetDefaultAnimation(chat_spr_num, true, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 80, 80);
				char_number = 0;
				step++;
				break;
			case 22://Cult boss chat
				VIDEO_StringToScreenBuffer(86, 85, ui->txt_file[UI_TXT_SCN4I]->line[34], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(86, 100, ui->txt_file[UI_TXT_SCN4I]->line[35], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(86, 115, ui->txt_file[UI_TXT_SCN4I]->line[36], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(6000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 23:// End chat
				GFX_HideSprite(chat_spr_num);
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 266, 106, 30, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 13);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 136, 126, 20, 70, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 23, 73);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 136, 126, 170, 70, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 173, 73);
				step++;
				break;
			case 24:// Actors chat
				GFX_SetDefaultAnimation(chat_spr_num, false, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 100, 80);
				GFX_ShowSprite(chat_spr_num);
				char_number = 0;
				step++;
				break;
			case 25:// Actors chat
				VIDEO_StringToScreenBuffer(106, 85, ui->txt_file[UI_TXT_SCN4I]->line[45], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 100, ui->txt_file[UI_TXT_SCN4I]->line[46], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(6000);
				while (!AwaitDelayTime()) {
					// Just wait
				}

				GFX_PCXImageToBuffer("OSCN4.DAT", "OUTRO4H.PCX", 130 * 120, gfx->image_buffer3, &gfx->image_buffer3_width, &gfx->image_buffer3_height);

				step++;
				break;
			case 26:// End chat
				GFX_HideSprite(chat_spr_num);
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 266, 106, 30, 10, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 13);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 136, 126, 20, 70, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 23, 73);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 136, 126, 170, 70, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 173, 73);
				step++;
				break;
			case 27:// Just wait
				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 28://
				VIDEO_FadeOutToWhite(4);
				step++;
				break;
			case 29:
				SetDelayTime(2000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 30:
				VIDEO_ClearScreen();
				VIDEO_FadeIn(4);
				VIDEO_FadeOut(4);
				step++;
				break;
			case 31:// Load first outro scene
				AUDIO_LoadSong(AUDIO_SONG_3);
				GFX_PCXImageToBuffer("OSCN4.DAT", "OUTRO5H.PCX", 250 * 135, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("OSCN4.DAT", "OUTRO5L.PCX", 205 * 135, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				GFX_PCXImageToBuffer("OSCN4.DAT", "OUTRO6H.PCX", 150 * 130, gfx->image_buffer3, &gfx->image_buffer3_width, &gfx->image_buffer3_height);
				step++;
				break;
			case 32:/// Image 5 :: back home
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 211, 141, 10, 40, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, 205, gfx->image_buffer1_height, 13, 43);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				VIDEO_FadeIn(4);

				TIMER_UpdateTimerTime(TIMER_AUDIO_NUMBER, 50);
				AUDIO_PlaySong(true);

				step++;
				break;
			case 33:// First image scroll
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

					VIDEO_StringToScreenBuffer(28, 13, ui->txt_file[UI_TXT_SCN4I]->line[50], FONT_SLIM_BLACK);
					VIDEO_StringToScreenBuffer(28, 25, ui->txt_file[UI_TXT_SCN4I]->line[51], FONT_SLIM_BLACK);

					SetDelayTime(50);
					while (!AwaitDelayTime()) {
						// Just wait
					}

					VIDEO_VSync();
					VIDEO_ScreenBufferToVRAM();
				}

				step++;
				break;
			case 34:// Just wait
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 35://Image 2
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 156, 136, 150, 60, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 153, 63);

				// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 242);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 250);
				step++;
				break;
			case 36:// Just wait
				SetDelayTime(200);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 37:// Fade first image and show second again
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 211, 141, 10, 40, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 13, 43);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 156, 136, 150, 60, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 153, 63);

				// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 242);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 250);

				VIDEO_StringToScreenBuffer(28, 13, ui->txt_file[UI_TXT_SCN4I]->line[52], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(28, 25, ui->txt_file[UI_TXT_SCN4I]->line[53], FONT_SLIM_BLACK);
				step++;
				break;
			case 38:// Load new images
				VIDEO_ClearScreenBuffer();
				GFX_PCXImageToBuffer("OSCN4.DAT", "OUTRO7H.PCX", 150 * 130, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("OSCN4.DAT", "OUTRO8H.PCX", 251 * 160, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				step++;
				break;
			case 39:// Show image 7
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 156, 136, 160, 40, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 163, 43);
				step++;
				break;
			case 40:// Just wait
				SetDelayTime(2000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 41://Girl's chat
				chat_spr_num = GFX_FindEmptySpriteSlot();
				if (chat_spr_num == -1) {
					sprintf(engine.system_error_message1, "Scene 3 Outtro function error");
					sprintf(engine.system_error_message2, "No empty sprite slot available");
					sprintf(engine.system_error_message3, " ");
					Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
				} else {
					GFX_InitSprite(ENTITY_ID_EMPTY, 0, chat_spr_num, 0, 65, 30);
				}
				GFX_SetSpriteGraphic(chat_spr_num, 0, SPRITE_GRAPHICS_ID_CHAT_BIG, 0, 0);
				GFX_SetDefaultAnimation(chat_spr_num, true, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 50, 50);
				char_number = 0;
				step++;
				break;
			case 42://Girl's chat
				VIDEO_StringToScreenBuffer(56, 55, ui->txt_file[UI_TXT_SCN4I]->line[55], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(56, 70, ui->txt_file[UI_TXT_SCN4I]->line[56], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(56, 85, ui->txt_file[UI_TXT_SCN4I]->line[57], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 43:// End chat
				GFX_HideSprite(chat_spr_num);
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 156, 136, 160, 40, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 163, 43);
				step++;
				break;
			case 44:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 45:// Show image 8
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 257, 166, 30, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 23);
				step++;
				break;
			case 46:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 47:// Actors chat
				GFX_SetDefaultAnimation(chat_spr_num, false, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 100, 20);
				GFX_ShowSprite(chat_spr_num);
				char_number = 0;
				step++;
				break;
			case 48:// Actors chat
				VIDEO_StringToScreenBuffer(106, 25, ui->txt_file[UI_TXT_SCN4I]->line[58], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 49:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 50:// Actors chat
				VIDEO_StringToScreenBuffer(106, 25, ui->txt_file[UI_TXT_SCN4I]->line[59], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 40, ui->txt_file[UI_TXT_SCN4I]->line[60], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 55, ui->txt_file[UI_TXT_SCN4I]->line[61], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 51:// End chat
				GFX_HideSprite(chat_spr_num);
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 257, 166, 30, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 23);
				step++;
				break;
			case 52:// Just wait
				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 53://Girl's chat
				GFX_SetSpriteGraphic(chat_spr_num, 0, SPRITE_GRAPHICS_ID_CHAT_BIG, 0, 0);
				GFX_SetDefaultAnimation(chat_spr_num, true, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 70, 50);
				char_number = 0;
				step++;
				break;
			case 54://Girl's chat
				VIDEO_StringToScreenBuffer(76, 55, ui->txt_file[UI_TXT_SCN4I]->line[62], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(76, 70, ui->txt_file[UI_TXT_SCN4I]->line[63], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(76, 85, ui->txt_file[UI_TXT_SCN4I]->line[64], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 55:// End chat
				GFX_HideSprite(chat_spr_num);
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 257, 166, 30, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 23);
				step++;
				break;
			case 56:// Just wait
				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 57:// Actors chat
				GFX_SetDefaultAnimation(chat_spr_num, false, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 100, 20);
				GFX_ShowSprite(chat_spr_num);
				char_number = 0;
				step++;
				break;
			case 58:// Actors chat
				VIDEO_StringToScreenBuffer(106, 25, ui->txt_file[UI_TXT_SCN4I]->line[65], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 40, ui->txt_file[UI_TXT_SCN4I]->line[66], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 55, ui->txt_file[UI_TXT_SCN4I]->line[67], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 59:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 60:// Actors chat
				VIDEO_StringToScreenBuffer(106, 25, ui->txt_file[UI_TXT_SCN4I]->line[68], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 40, ui->txt_file[UI_TXT_SCN4I]->line[69], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 61:// End chat
				GFX_HideSprite(chat_spr_num);
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 257, 166, 30, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 23);
				step++;
				break;
			case 62:// Just wait
				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 63://Girl's chat
				GFX_SetSpriteGraphic(chat_spr_num, 0, SPRITE_GRAPHICS_ID_CHAT_BIG, 0, 0);
				GFX_SetDefaultAnimation(chat_spr_num, true, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 70, 50);
				char_number = 0;
				step++;
				break;
			case 64://Girl's chat
				VIDEO_StringToScreenBuffer(76, 70, ui->txt_file[UI_TXT_SCN4I]->line[70], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 65:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 66://Girl's chat
				VIDEO_StringToScreenBuffer(76, 55, ui->txt_file[UI_TXT_SCN4I]->line[71], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(76, 70, ui->txt_file[UI_TXT_SCN4I]->line[72], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(76, 85, ui->txt_file[UI_TXT_SCN4I]->line[73], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 67:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 68://Girl's chat
				VIDEO_StringToScreenBuffer(76, 55, ui->txt_file[UI_TXT_SCN4I]->line[74], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(76, 70, ui->txt_file[UI_TXT_SCN4I]->line[75], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 69:// End chat
				GFX_HideSprite(chat_spr_num);
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 257, 166, 30, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 23);
				step++;
				break;
			case 70:// Just wait
				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 71:// Actors chat
				GFX_SetDefaultAnimation(chat_spr_num, false, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 100, 20);
				GFX_ShowSprite(chat_spr_num);
				char_number = 0;
				step++;
				break;
			case 72:// Actors chat
				VIDEO_StringToScreenBuffer(106, 25, ui->txt_file[UI_TXT_SCN4I]->line[76], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 40, ui->txt_file[UI_TXT_SCN4I]->line[77], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 55, ui->txt_file[UI_TXT_SCN4I]->line[78], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 73:// End chat
				GFX_HideSprite(chat_spr_num);
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 257, 166, 30, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 23);
				SetDelayTime(200);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 74://Girl's chat
				GFX_SetSpriteGraphic(chat_spr_num, 0, SPRITE_GRAPHICS_ID_CHAT_BIG, 0, 0);
				GFX_SetDefaultAnimation(chat_spr_num, true, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 70, 50);
				char_number = 0;
				step++;
				break;
			case 75://Girl's chat
				VIDEO_StringToScreenBuffer(76, 55, ui->txt_file[UI_TXT_SCN4I]->line[79], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(76, 70, ui->txt_file[UI_TXT_SCN4I]->line[80], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(76, 85, ui->txt_file[UI_TXT_SCN4I]->line[81], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 76:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 77://Girl's chat
				VIDEO_StringToScreenBuffer(76, 55, ui->txt_file[UI_TXT_SCN4I]->line[82], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(76, 70, ui->txt_file[UI_TXT_SCN4I]->line[83], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(76, 85, ui->txt_file[UI_TXT_SCN4I]->line[84], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 78:// End chat
				GFX_HideSprite(chat_spr_num);
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 257, 166, 30, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 23);
				SetDelayTime(200);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 79:// Actors chat
				GFX_SetDefaultAnimation(chat_spr_num, false, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 100, 20);
				GFX_ShowSprite(chat_spr_num);
				char_number = 0;
				step++;
				break;
			case 80:// Actors chat
				VIDEO_StringToScreenBuffer(106, 25, ui->txt_file[UI_TXT_SCN4I]->line[85], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 40, ui->txt_file[UI_TXT_SCN4I]->line[86], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 55, ui->txt_file[UI_TXT_SCN4I]->line[87], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 81:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 82:// Actors chat
				VIDEO_StringToScreenBuffer(106, 25, ui->txt_file[UI_TXT_SCN4I]->line[89], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 40, ui->txt_file[UI_TXT_SCN4I]->line[90], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 55, ui->txt_file[UI_TXT_SCN4I]->line[91], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 83:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 84:// Actors chat
				VIDEO_StringToScreenBuffer(106, 25, ui->txt_file[UI_TXT_SCN4I]->line[92], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 40, ui->txt_file[UI_TXT_SCN4I]->line[93], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 85:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 86:// End chat
				GFX_HideSprite(chat_spr_num);
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 257, 166, 30, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 23);
				SetDelayTime(200);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 87://Girl's chat
				GFX_SetSpriteGraphic(chat_spr_num, 0, SPRITE_GRAPHICS_ID_CHAT_BIG, 0, 0);
				GFX_SetDefaultAnimation(chat_spr_num, true, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 70, 50);
				char_number = 0;
				step++;
				break;
			case 88://Girl's chat
				VIDEO_StringToScreenBuffer(76, 70, ui->txt_file[UI_TXT_SCN4I]->line[94], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 89:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 90://Girl's chat
				VIDEO_StringToScreenBuffer(76, 55, ui->txt_file[UI_TXT_SCN4I]->line[95], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(76, 70, ui->txt_file[UI_TXT_SCN4I]->line[96], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(76, 85, ui->txt_file[UI_TXT_SCN4I]->line[97], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 91:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 92://Girl's chat
				VIDEO_StringToScreenBuffer(76, 55, ui->txt_file[UI_TXT_SCN4I]->line[98], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(76, 70, ui->txt_file[UI_TXT_SCN4I]->line[99], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(76, 85, ui->txt_file[UI_TXT_SCN4I]->line[100], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 93:// End chat
				GFX_HideSprite(chat_spr_num);
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 257, 166, 30, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 23);
				SetDelayTime(200);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 94:// Just wait
				SetDelayTime(2000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 95:// Actors chat
				GFX_SetDefaultAnimation(chat_spr_num, false, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 100, 20);
				GFX_ShowSprite(chat_spr_num);
				char_number = 0;
				step++;
				break;
			case 96:// Actors chat
				VIDEO_StringToScreenBuffer(106, 25, ui->txt_file[UI_TXT_SCN4I]->line[101], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(106, 40, ui->txt_file[UI_TXT_SCN4I]->line[102], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 97:// End chat
				GFX_HideSprite(chat_spr_num);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 257, 166, 30, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 23);
				SetDelayTime(200);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 98:// Load new images
				VIDEO_ClearScreenBuffer();
				GFX_PCXImageToBuffer("OSCN4.DAT", "OUTRO9H.PCX", 150 * 130, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("OSCN4.DAT", "OUTRO10H.PCX", 150 * 130, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				step++;
				break;
			case 99:// Show images 9 and 10
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 156, 136, 2, 40, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 5, 43);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 156, 136, 160, 40, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 163, 43);
				step++;
				break;
			case 100:// Just wait
				SetDelayTime(2000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 101:// Clear image 10 and show just 9
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 156, 136, 2, 40, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 5, 43);
				step++;
				break;
			case 102:// Show speech
				// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 242);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 250);

				VIDEO_StringToScreenBuffer(28, 13, ui->txt_file[UI_TXT_SCN4I]->line[105], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(28, 25, ui->txt_file[UI_TXT_SCN4I]->line[106], FONT_SLIM_BLACK);
				step++;
				break;
			case 103:// Just wait
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 104:// Clear speech
				// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 242);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 250);
				step++;
				break;
			case 105:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 106:// Show speech
				// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 242);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 250);

				VIDEO_StringToScreenBuffer(28, 13, ui->txt_file[UI_TXT_SCN4I]->line[107], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(28, 25, ui->txt_file[UI_TXT_SCN4I]->line[108], FONT_SLIM_BLACK);
				step++;
				break;
			case 107:// Just wait
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 108:// Clear speech
				// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 242);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 250);
				step++;
				break;
			case 109:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 110:// Show speech
				// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 242);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 250);

				VIDEO_StringToScreenBuffer(28, 13, ui->txt_file[UI_TXT_SCN4I]->line[109], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(28, 25, ui->txt_file[UI_TXT_SCN4I]->line[110], FONT_SLIM_BLACK);
				step++;
				break;
			case 111:// Just wait
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 112://End sequence
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

/** SCENARIO 5 :: Mission 5 :: Hit and run
 */
static void Scene5_Intro(void) {
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
static void Scene5_UnloadAssets(void) {
	// Enemy 1
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_CHAT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_FEET);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_BODY);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_HEAD);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_RARM);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_LARM);
}
static void Scene5_LoadAssets(void) {

	ENEMY_Load("ENEMY9.DAT", 0, 0, 0, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_GUN0, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_NPC, 1);
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
static void Scene5_LoadRoom1(void) {

	// Items
	ITEM_LoadItem(0, ENTITY_ID_ITEM_AMMO4, SPRITE_GRAPHICS_ID_ITEM_AMMO4, 21 << 4, 6 << 4);
	ITEM_LoadItem(1, ENTITY_ID_ITEM_AMMO2, SPRITE_GRAPHICS_ID_ITEM_AMMO2, 20 << 4, 6 << 4);
	ITEM_LoadItem(2, ENTITY_ID_ITEM_GRENADE, SPRITE_GRAPHICS_ID_ITEM_GRENADE, 21 << 4, 11 << 4);

	MAP_LoadMap("MAPSCN51.DAT", 25, 20, "TSCN51.DAT", "SCN5_1_BACK.PCX", "SCN5_1_FORE.PCX", "SCN5_1_MASK.PCX", 320 * 416, 128 * 128, 128 * 128);
	GFX_LoadPalette("PALETTES.DAT", "SCN51.PCX", 256);
}
static void Scene5_LoadRoom2(void) {

	// Load enemies. Main corridor
	ENEMY_Load("ENEMY9.DAT", 0, 8 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 1, 22 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 2, 34 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 1
	ENEMY_Load("ENEMY9.DAT", 3, 17 << 4, 7 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 2
	ENEMY_Load("ENEMY9.DAT", 4, 25 << 4, 7 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 3
	ENEMY_Load("ENEMY9.DAT", 5, 10 << 4, 20 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 4
	ENEMY_Load("ENEMY9.DAT", 6, 21 << 4, 17 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 7, 32 << 4, 21 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 5
	ENEMY_Load("ENEMY9.DAT", 8, 38 << 4, 19 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 9, 40 << 4, 14 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Kitchen
	ENEMY_Load("ENEMY9.DAT", 10, 37 << 4, 5 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Stairs
	ENEMY_Load("ENEMY9.DAT", 11, 48 << 4, 12 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

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
}
static void Scene5_Update_Floor1(void) {
	OBJECT_UnloadObjects();
	ENEMY_UnloadEnemies();

	// Load enemies. Main corridor
	ENEMY_Load("ENEMY9.DAT", 0, 8 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 1, 22 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 2, 34 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 1
	ENEMY_Load("ENEMY9.DAT", 3, 17 << 4, 7 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 2
	ENEMY_Load("ENEMY9.DAT", 4, 25 << 4, 7 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 3
	ENEMY_Load("ENEMY9.DAT", 5, 10 << 4, 20 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 4
	ENEMY_Load("ENEMY9.DAT", 6, 21 << 4, 17 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 7, 32 << 4, 21 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 5
	ENEMY_Load("ENEMY9.DAT", 8, 38 << 4, 19 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 9, 40 << 4, 14 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Kitchen
	ENEMY_Load("ENEMY9.DAT", 10, 37 << 4, 5 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Stairs
	ENEMY_Load("ENEMY9.DAT", 11, 48 << 4, 12 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);


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
static void Scene5_Update_Floor2(void) {
	OBJECT_UnloadObjects();
	ENEMY_UnloadEnemies();

	// Load enemies. Main corridor
	ENEMY_Load("ENEMY9.DAT", 0, 8 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 1, 22 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 2, 34 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 1
	ENEMY_Load("ENEMY9.DAT", 3, 17 << 4, 7 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 2
	ENEMY_Load("ENEMY9.DAT", 4, 25 << 4, 7 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 3
	ENEMY_Load("ENEMY9.DAT", 5, 10 << 4, 20 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 4
	ENEMY_Load("ENEMY9.DAT", 6, 21 << 4, 17 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 7, 32 << 4, 21 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 5
	ENEMY_Load("ENEMY9.DAT", 8, 38 << 4, 19 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 9, 40 << 4, 14 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Kitchen
	ENEMY_Load("ENEMY9.DAT", 10, 37 << 4, 5 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Stairs
	ENEMY_Load("ENEMY9.DAT", 11, 48 << 4, 12 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);


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
static void Scene5_Update_Floor3(void) {
	OBJECT_UnloadObjects();
	ENEMY_UnloadEnemies();

	// Load enemies. Main corridor
	ENEMY_Load("ENEMY9.DAT", 0, 8 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 1, 22 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 2, 34 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 1
	ENEMY_Load("ENEMY9.DAT", 3, 17 << 4, 7 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 2
	ENEMY_Load("ENEMY9.DAT", 4, 25 << 4, 7 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 3
	ENEMY_Load("ENEMY9.DAT", 5, 10 << 4, 20 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 4
	ENEMY_Load("ENEMY9.DAT", 6, 21 << 4, 17 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 7, 32 << 4, 21 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 5
	ENEMY_Load("ENEMY9.DAT", 8, 38 << 4, 19 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 9, 40 << 4, 14 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Kitchen
	ENEMY_Load("ENEMY9.DAT", 10, 37 << 4, 5 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Stairs
	ENEMY_Load("ENEMY9.DAT", 11, 48 << 4, 12 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

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
static void Scene5_Update_Floor4(void) {
	OBJECT_UnloadObjects();
	ENEMY_UnloadEnemies();

	// Load enemies. Main corridor
	ENEMY_Load("ENEMY9.DAT", 0, 8 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 1, 22 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 2, 34 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 1
	ENEMY_Load("ENEMY9.DAT", 3, 17 << 4, 7 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 2
	ENEMY_Load("ENEMY9.DAT", 4, 25 << 4, 7 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 3
	ENEMY_Load("ENEMY9.DAT", 5, 10 << 4, 20 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 4
	ENEMY_Load("ENEMY9.DAT", 6, 21 << 4, 17 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 7, 32 << 4, 21 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 5
	ENEMY_Load("ENEMY9.DAT", 8, 38 << 4, 19 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 9, 40 << 4, 14 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Kitchen
	ENEMY_Load("ENEMY9.DAT", 10, 37 << 4, 5 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Stairs
	ENEMY_Load("ENEMY9.DAT", 11, 48 << 4, 12 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

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
static void Scene5_Update_Floor5(void) {
	OBJECT_UnloadObjects();
	ENEMY_UnloadEnemies();

	// Load enemies. Main corridor
	ENEMY_Load("ENEMY9.DAT", 0, 8 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 1, 22 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 2, 34 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 1
	ENEMY_Load("ENEMY9.DAT", 3, 17 << 4, 7 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 2
	ENEMY_Load("ENEMY9.DAT", 4, 25 << 4, 7 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 3
	ENEMY_Load("ENEMY9.DAT", 5, 10 << 4, 20 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 4
	ENEMY_Load("ENEMY9.DAT", 6, 21 << 4, 17 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 7, 32 << 4, 21 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Room 5
	ENEMY_Load("ENEMY9.DAT", 8, 38 << 4, 19 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY9.DAT", 9, 40 << 4, 14 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Kitchen
	ENEMY_Load("ENEMY9.DAT", 10, 37 << 4, 5 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

	// Load enemies. Stairs
	ENEMY_Load("ENEMY9.DAT", 11, 48 << 4, 12 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_SHOTGUN, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_SHOOTER, 100);

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
static void Scene5_SetHotspotsAndEvents(void) {
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
static void Scene5_Loop(void) {
	bool end_sequence;
	int actor_tile_num;
	int sequence_step;
	int scene_step;

	VIDEO_ClearScreenBuffer();
	GFX_LoadPalette("PALETTES.DAT", "SCN51.PCX", 256);

	VIDEO_StringToScreenBuffer(50, 60, ui->txt_file[UI_TXT_SCN5I]->line[40], FONT_BIG_WHITE);
	VIDEO_StringToScreenBuffer(60, 85, ui->txt_file[UI_TXT_SCN5I]->line[41], FONT_BIG_WHITE);
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
		if (actor->status_dead) GameOver();
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
	MM_PopChunks(CT_TEMPORARY_SPRITE);
}
static void Scene5_Outro(void) {
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

/** SCENARIO 6 :: Mission 6 :: Bullet hell
 */
static void Scene6_Intro(void) {
	int step;
	int chat_spr_num;
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
				GFX_PCXImageToBuffer("ISCN6.DAT", "INTRO1H.PCX", 256 * 180, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("ISCN6.DAT", "INTRO2H.PCX", 91 * 141, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);

				AUDIO_LoadSong(AUDIO_SONG_3);
				step++;
				break;
			case 2:// Hide loading screen
				UI_HideLoadingScreen();
				step++;
				break;
			case 3:/// Scene 1 :: Entering kitchen
				VIDEO_ClearScreenBuffer();
				GFX_LoadPalette("PALETTES.DAT", "ISCN6.PCX", 256);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				VIDEO_FadeIn(4);

				TIMER_UpdateTimerTime(TIMER_AUDIO_NUMBER, 50);
				AUDIO_PlaySong(true);

				step++;
				break;
			case 4:// Draw frame and image 1
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 262, 186, 50, 5, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 53, 8);
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
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 299, 46, 5, 1, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 293, 40, 8, 4, 208);
				VIDEO_StringToScreenBuffer(12, 12, ui->txt_file[UI_TXT_SCN6I]->line[1], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(12, 22, ui->txt_file[UI_TXT_SCN6I]->line[2], FONT_SLIM_BLACK);
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
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 299, 46, 5, 1, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 293, 40, 8, 4, 208);
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
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 299, 46, 5, 1, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 293, 40, 8, 4, 208);
				VIDEO_StringToScreenBuffer(12, 12, ui->txt_file[UI_TXT_SCN6I]->line[3], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(12, 22, ui->txt_file[UI_TXT_SCN6I]->line[4], FONT_SLIM_BLACK);
				step++;
				break;
			case 11:// Just wait
				SetDelayTime(6000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 12:// Show image 1 and 2
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 262, 186, 50, 5, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 53, 8);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 97, 147, 10, 50, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 13, 53);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 299, 46, 5, 1, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 293, 40, 8, 4, 208);
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
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 299, 46, 5, 1, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 293, 40, 8, 4, 208);
				VIDEO_StringToScreenBuffer(12, 12, ui->txt_file[UI_TXT_SCN6I]->line[5], FONT_SLIM_BLACK);
				step++;
				break;
			case 15:// Just wait
				SetDelayTime(6000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 16:// Show text
					// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 299, 46, 5, 1, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 293, 40, 8, 4, 208);
				step++;
				break;
			case 17:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 18:// Show text
					// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 299, 46, 5, 1, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 293, 40, 8, 4, 208);
				VIDEO_StringToScreenBuffer(12, 12, ui->txt_file[UI_TXT_SCN6I]->line[6], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(12, 22, ui->txt_file[UI_TXT_SCN6I]->line[7], FONT_SLIM_BLACK);
				step++;
				break;
			case 19:// Just wait
				SetDelayTime(6000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 20:// Show image 1 and 2
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 262, 186, 50, 5, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 53, 8);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 97, 147, 10, 50, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 13, 53);
				step++;
				break;
			case 21:// Show chat
				chat_spr_num = GFX_FindEmptySpriteSlot();
				if (chat_spr_num == -1) {
					sprintf(engine.system_error_message1, "Scene 6 intro function error");
					sprintf(engine.system_error_message2, "No empty sprite slot available");
					sprintf(engine.system_error_message3, " ");
					Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_GRAPHICS);
				} else {
					GFX_InitSprite(ENTITY_ID_EMPTY, 0, chat_spr_num, 0, 65, 30);
				}
				GFX_SetSpriteGraphic(chat_spr_num, 0, SPRITE_GRAPHICS_ID_CHAT_BIG, 0, 0);
				GFX_SetDefaultAnimation(chat_spr_num, false, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 100, 100);
				char_number = 0;
				step++;
				break;
			case 22:// Show chat
				VIDEO_StringToScreenBuffer(106, 105, ui->txt_file[UI_TXT_SCN6I]->line[9], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(6000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 23://End sequence
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
	ITEM_UnloadItems();
	BOSS_Unload();
	PARTICLE_UnloadParticles();
	EFFECT_UnloadEffects();
	OBJECT_UnloadObjects();
	GFX_UnloadSprites();
	AUDIO_StopSong();
	AUDIO_UnloadSong();
}
static void Scene6_UnloadAssets(void) {
	// Enemy 1
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_CHAT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_FEET);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_BODY);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_HEAD);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_RARM);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_LARM);
}
static void Scene6_LoadAssets(void) {

	// Load enemies and unload them just to get graphics
	ENEMY_Load("ENEMY11.DAT", 0, 0, 0, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_GUN0, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_NPC, 1);
	ENEMY_UnloadEnemies();

	// Load objects and unload them just to get graphics
	OBJECT_LoadObject("OBJECT1.DAT", 0, ENTITY_ID_BARREL, SPRITE_GRAPHICS_ID_OBJECT1, SPRITE_GRAPHICS_ID_OBJECT1_PORTAIT, 0, 0);
	OBJECT_UnloadObjects();

	// Load boss and unload it just to preload graphics
	BOSS_Load("BOSS4.DAT", 0, 0, BOSS_TYPE_STRONG, SPRITE_GRAPHICS_ID_ENEMY2_CHAT, SPRITE_GRAPHICS_ID_ENEMY2_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY2_FEET, SPRITE_GRAPHICS_ID_ENEMY2_BODY, SPRITE_GRAPHICS_ID_ENEMY2_HEAD, SPRITE_GRAPHICS_ID_ENEMY2_LARM, SPRITE_GRAPHICS_ID_ENEMY2_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN0, SPRITE_GRAPHICS_ID_BULLET1, BOSS_STATUS_STATIC, BOSS_LIFE);
	BOSS_Unload();

	ACTOR_Load(9 << 4, 47 << 4, SPRITE_GRAPHICS_ID_ACTOR_FEET, SPRITE_GRAPHICS_ID_ACTOR_BODY, SPRITE_GRAPHICS_ID_ACTOR_HEAD, SPRITE_GRAPHICS_ID_ACTOR_LARM, SPRITE_GRAPHICS_ID_ACTOR_RARM_BARE_HANDS, ACTOR_FACING_RIGHT);
	ACTOR_SetGun(actor->sprite_num, ACTOR_GUN_TYPE_BARE_HANDS);
	ACTOR_SetBulletStatus(99, 19, 199, 9, 9);

	NPC_Init();

	UI_SetStatusPanels();// Set status panels

	AUDIO_LoadSong(AUDIO_SONG_3);// Load song
}
static void Scene6_LoadRoom1(void) {

	// Items
	ITEM_LoadItem(0, ENTITY_ID_ITEM_KEYBLUE, SPRITE_GRAPHICS_ID_ITEM_BLUE_KEY, 32 << 4, 42 << 4);
	ITEM_LoadItem(1, ENTITY_ID_ITEM_KEYGREEN, SPRITE_GRAPHICS_ID_ITEM_GREEN_KEY, 8 << 4, 4 << 4);
	ITEM_LoadItem(2, ENTITY_ID_ITEM_KEYYELLOW, SPRITE_GRAPHICS_ID_ITEM_YELLOW_KEY, 35 << 4, 9 << 4);
	ITEM_LoadItem(3, ENTITY_ID_ITEM_KEYRED, SPRITE_GRAPHICS_ID_ITEM_RED_KEY, 7 << 4, 28 << 4);

	ITEM_LoadItem(4, ENTITY_ID_ITEM_MEDIKIT, SPRITE_GRAPHICS_ID_ITEM_MEDIKIT, 29 << 4, 40 << 4);
	ITEM_LoadItem(5, ENTITY_ID_ITEM_MEDIKIT, SPRITE_GRAPHICS_ID_ITEM_MEDIKIT, 23 << 4, 4 << 4);
	ITEM_LoadItem(6, ENTITY_ID_ITEM_MEDIKIT, SPRITE_GRAPHICS_ID_ITEM_MEDIKIT, 61 << 4, 9 << 4);

	ITEM_LoadItem(7, ENTITY_ID_ITEM_AMMO2, SPRITE_GRAPHICS_ID_ITEM_AMMO2, 9 << 4, 56 << 4);
	ITEM_LoadItem(8, ENTITY_ID_ITEM_AMMO3, SPRITE_GRAPHICS_ID_ITEM_AMMO3, 61 << 4, 17 << 4);
	ITEM_LoadItem(9, ENTITY_ID_ITEM_AMMO3, SPRITE_GRAPHICS_ID_ITEM_AMMO3, 24 << 4, 28 << 4);
	ITEM_LoadItem(10, ENTITY_ID_ITEM_AMMO4, SPRITE_GRAPHICS_ID_ITEM_AMMO4, 12 << 4, 17 << 4);
	ITEM_LoadItem(11, ENTITY_ID_ITEM_AMMO4, SPRITE_GRAPHICS_ID_ITEM_AMMO4, 15 << 4, 4 << 4);

	// Objects
	OBJECT_LoadObject("OBJECT1.DAT", 0, ENTITY_ID_BARREL, SPRITE_GRAPHICS_ID_OBJECT1, SPRITE_GRAPHICS_ID_OBJECT1_PORTAIT, 5 << 4, 46 << 4);
	OBJECT_LoadObject("OBJECT1.DAT", 1, ENTITY_ID_BARREL, SPRITE_GRAPHICS_ID_OBJECT1, SPRITE_GRAPHICS_ID_OBJECT1_PORTAIT, 7 << 4, 55 << 4);
	OBJECT_LoadObject("OBJECT1.DAT", 2, ENTITY_ID_BARREL, SPRITE_GRAPHICS_ID_OBJECT1, SPRITE_GRAPHICS_ID_OBJECT1_PORTAIT, 49 << 4, 57 << 4);

	// Enemies
	ENEMY_Load("ENEMY11.DAT", 0, 6 << 4, 58 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_RIGHT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY11.DAT", 1, 29 << 4, 59 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_UP, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY11.DAT", 2, 41 << 4, 53 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN_RIGHT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY11.DAT", 3, 51 << 4, 57 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY11.DAT", 4, 61 << 4, 55 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY11.DAT", 5, 58 << 4, 46 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_RIGHT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY11.DAT", 6, 43 << 4, 42 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY11.DAT", 7, 49 << 4, 42 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY11.DAT", 8, 38 << 4, 42 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	ENEMY_Load("ENEMY11.DAT", 9, 23 << 4, 49 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN_LEFT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY11.DAT", 10, 17 << 4, 49 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN_RIGHT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY11.DAT", 11, 24 << 4, 42 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY11.DAT", 12, 9 << 4, 46 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_RIGHT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY11.DAT", 13, 15 << 4, 43 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY11.DAT", 14, 9 << 4, 39 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_UP_LEFT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY11.DAT", 15, 4 << 4, 31 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY11.DAT", 16, 27 << 4, 31 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY11.DAT", 17, 7 << 4, 23 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN_LEFT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY11.DAT", 18, 25 << 4, 23 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN_RIGHT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY11.DAT", 19, 16 << 4, 16 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY11.DAT", 20, 12 << 4, 12 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY11.DAT", 21, 7 << 4, 6 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY11.DAT", 22, 24 << 4, 6 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY11.DAT", 23, 28 << 4, 8 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY11.DAT", 24, 48 << 4, 3 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY11.DAT", 25, 54 << 4, 3 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	ENEMY_Load("ENEMY11.DAT", 26, 38 << 4, 32 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY11.DAT", 27, 49 << 4, 35 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY11.DAT", 28, 44 << 4, 25 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY11.DAT", 29, 42 << 4, 22 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY11.DAT", 30, 59 << 4, 25 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY11.DAT", 31, 54 << 4, 20 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY11.DAT", 32, 52 << 4, 10 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY11.DAT", 33, 43 << 4, 13 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY11.DAT", 34, 39 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY11.DAT", 35, 39 << 4, 16 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
	ENEMY_Load("ENEMY11.DAT", 36, 35 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

	MAP_LoadMap("MAPSCN61.DAT", 66, 64, "TSCN61.DAT", "SCN6_1_BACK.PCX", "SCN6_1_FORE.PCX", "SCN6_1_MASK.PCX", 320 * 416, 128 * 256, 128 * 128);
	GFX_LoadPalette("PALETTES.DAT", "SCN61.PCX", 256);
}
static void Scene6_LoadRoom2(void) {

	// Items
	ITEM_LoadItem(0, ENTITY_ID_ITEM_MEDIKIT, SPRITE_GRAPHICS_ID_ITEM_MEDIKIT, 31 << 4, 26 << 4);
	ITEM_LoadItem(1, ENTITY_ID_ITEM_MEDIKIT, SPRITE_GRAPHICS_ID_ITEM_MEDIKIT, 2 << 4, 26 << 4);

	ITEM_LoadItem(2, ENTITY_ID_ITEM_GRENADE, SPRITE_GRAPHICS_ID_ITEM_GRENADE, 2 << 4, 27 << 4);
	ITEM_LoadItem(3, ENTITY_ID_ITEM_AMMO3, SPRITE_GRAPHICS_ID_ITEM_AMMO3, 28 << 4, 26 << 4);
	ITEM_LoadItem(4, ENTITY_ID_ITEM_AMMO3, SPRITE_GRAPHICS_ID_ITEM_AMMO3, 24 << 4, 28 << 4);
	ITEM_LoadItem(5, ENTITY_ID_ITEM_AMMO4, SPRITE_GRAPHICS_ID_ITEM_AMMO4, 20 << 4, 4 << 4);
	ITEM_LoadItem(6, ENTITY_ID_ITEM_GRENADE, SPRITE_GRAPHICS_ID_ITEM_GRENADE, 26 << 4, 26 << 4);

	MAP_LoadMap("MAPSCN62.DAT", 34, 32, "TSCN62.DAT", "SCN6_2_BACK.PCX", "SCN6_2_FORE.PCX", "SCN6_2_MASK.PCX", 320 * 416, 128 * 128, 128 * 128);
	GFX_LoadPalette("PALETTES.DAT", "SCN62.PCX", 256);
}
static void Scene6_SetHotspotsAndEvents(void) {
	// Initialize hotspots and events
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
	map->event_enabling_room2[1] = true;
	map->event_enabling_room2[2] = true;
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
	map->event_enabling_room2[15] = false;
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
static void Scene6_Loop(void) {
	bool end_sequence;
	int actor_tile_num;
	int sequence_step;
	int scene_step;
	int tile_data, tileset_index;

	VIDEO_ClearScreenBuffer();
	GFX_LoadPalette("PALETTES.DAT", "SCN61.PCX", 256);

	VIDEO_StringToScreenBuffer(50, 60, ui->txt_file[UI_TXT_SCN6I]->line[40], FONT_BIG_WHITE);
	VIDEO_StringToScreenBuffer(30, 85, ui->txt_file[UI_TXT_SCN6I]->line[41], FONT_BIG_WHITE);
	VIDEO_StringToScreenBuffer(90, 135, ui->txt_file[UI_TXT_SCN6I]->line[42], FONT_SLIM_WHITE);
	VIDEO_StringToScreenBuffer(160, 135, ui->txt_file[UI_TXT_SCN6I]->line[43], FONT_SLIM_WHITE);

	VIDEO_VSync();
	VIDEO_ScreenBufferToVRAM();
	VIDEO_FadeIn(4);

	SetDelayTime(6000);

	Scene6_LoadAssets();
	Scene6_SetHotspotsAndEvents();

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
		case 1:// Room 1. Outside base
			Scene6_LoadRoom1();

			ACTOR_SetPosition(19 << 4, 59 << 4, ACTOR_FACING_UP);// Set actor position
			ACTOR_SetCombatMode(true);

			CAM_Init(map->width_px, map->height_px, 14 << 4, 50 << 4);// Initialize camera
			MAP_DrawMapToMapVideoBuffer();

			Update(true);
			Update(true);
			VIDEO_FadeIn(1);

			ACTOR_SetGun(actor->sprite_num, ACTOR_GUN_TYPE_PISTOL);
			ACTOR_Reload();
			break;

			break;
		case 2:// Room 2. Inside base
			Scene6_LoadRoom2();

			ACTOR_SetPosition(17 << 4, 27 << 4, ACTOR_FACING_UP);     // Set actor position
			CAM_Init(map->width_px, map->height_px, 12 << 4, 20 << 4);// Initialize camera
			MAP_DrawMapToMapVideoBuffer();

			Update(true);
			Update(true);
			VIDEO_FadeIn(1);

			ACTOR_SetGun(actor->sprite_num, ACTOR_GUN_TYPE_PISTOL);
			ACTOR_Reload();

			break;
		default:
			Error("Scene5_Loop function error", "Undefined room", "", ERROR_SYSTEM);
			break;
	}

	// Loop until the game is over
	while (engine.ingame) {
		switch (engine.room) {
			case 1:// Room 1. Inside base
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
					case 1:// Event 1. Green door
						if (map->event_enabling_room1[1]) {
							map->event_enabling_room1[1] = false;

							if (actor->key_entity_id != ENTITY_ID_ITEM_KEYGREEN) {
								AUDIO_PlaySound(AUDIO_SHOT_FAIL_EFFECT, 1);
							} else {

								scene_step = 2;
								ACTOR_LeaveKey();
								AUDIO_PlaySound(AUDIO_OPEN_DOOR_EFFECT, 1);

								MAP_HideSwapableTile(45 << 4, 39 << 4);
								MAP_HideSwapableTile(46 << 4, 39 << 4);
								MAP_HideSwapableTile(47 << 4, 39 << 4);
								MAP_HideSwapableTile(45 << 4, 40 << 4);
								MAP_HideSwapableTile(46 << 4, 40 << 4);
								MAP_HideSwapableTile(47 << 4, 40 << 4);
								MAP_HideSwapableTile(45 << 4, 41 << 4);
								MAP_HideSwapableTile(46 << 4, 41 << 4);
								MAP_HideSwapableTile(47 << 4, 41 << 4);
							}
						}
						break;
					case 2:// Event 2. Blue door
						if (map->event_enabling_room1[2]) {
							map->event_enabling_room1[2] = false;

							if (actor->key_entity_id != ENTITY_ID_ITEM_KEYBLUE) {
								AUDIO_PlaySound(AUDIO_SHOT_FAIL_EFFECT, 1);
							} else {

								scene_step = 1;
								ACTOR_LeaveKey();
								AUDIO_PlaySound(AUDIO_OPEN_DOOR_EFFECT, 1);

								MAP_HideSwapableTile(19 << 4, 51 << 4);
								MAP_HideSwapableTile(20 << 4, 51 << 4);
								MAP_HideSwapableTile(21 << 4, 51 << 4);
								MAP_HideSwapableTile(19 << 4, 52 << 4);
								MAP_HideSwapableTile(20 << 4, 52 << 4);
								MAP_HideSwapableTile(21 << 4, 52 << 4);
								MAP_HideSwapableTile(19 << 4, 53 << 4);
								MAP_HideSwapableTile(20 << 4, 53 << 4);
								MAP_HideSwapableTile(21 << 4, 53 << 4);
							}
						}
						break;
					case 3:// Event 3. Yellow door
						if (map->event_enabling_room1[3]) {
							map->event_enabling_room1[3] = false;

							if (actor->key_entity_id != ENTITY_ID_ITEM_KEYYELLOW) {
								AUDIO_PlaySound(AUDIO_SHOT_FAIL_EFFECT, 1);
							} else {

								scene_step = 3;
								ACTOR_LeaveKey();
								AUDIO_PlaySound(AUDIO_OPEN_DOOR_EFFECT, 1);

								MAP_HideSwapableTile(17 << 4, 31 << 4);
								MAP_HideSwapableTile(18 << 4, 31 << 4);
								MAP_HideSwapableTile(19 << 4, 31 << 4);
								MAP_HideSwapableTile(17 << 4, 32 << 4);
								MAP_HideSwapableTile(18 << 4, 32 << 4);
								MAP_HideSwapableTile(19 << 4, 32 << 4);
								MAP_HideSwapableTile(17 << 4, 33 << 4);
								MAP_HideSwapableTile(18 << 4, 33 << 4);
								MAP_HideSwapableTile(19 << 4, 33 << 4);
							}
						}
						break;
					case 4:// Event 4. Red door
						if (map->event_enabling_room1[4]) {
							map->event_enabling_room1[4] = false;

							if (actor->key_entity_id != ENTITY_ID_ITEM_KEYRED) {
								AUDIO_PlaySound(AUDIO_SHOT_FAIL_EFFECT, 1);
							} else {

								scene_step = 3;
								ACTOR_LeaveKey();
								AUDIO_PlaySound(AUDIO_OPEN_DOOR_EFFECT, 1);

								MAP_HideSwapableTile(50 << 4, 1 << 4);
								MAP_HideSwapableTile(51 << 4, 1 << 4);
								MAP_HideSwapableTile(52 << 4, 1 << 4);
								MAP_HideSwapableTile(50 << 4, 2 << 4);
								MAP_HideSwapableTile(51 << 4, 2 << 4);
								MAP_HideSwapableTile(52 << 4, 2 << 4);
								MAP_HideSwapableTile(50 << 4, 3 << 4);
								MAP_HideSwapableTile(51 << 4, 3 << 4);
								MAP_HideSwapableTile(52 << 4, 3 << 4);
							}
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
							OBJECT_UnloadObjects();
							EFFECT_UnloadEffects();
							BULLET_UnloadBullets();
							PARTICLE_UnloadParticles();
							GRENADE_UnloadGrenades();
							NPC_UnloadNpcs();
							MAP_UnloadMap();

							Scene6_LoadRoom2();
							ACTOR_SetPosition(17 << 4, 27 << 4, ACTOR_FACING_UP);     // Set actor position
							CAM_Init(map->width_px, map->height_px, 12 << 4, 20 << 4);// Initialize camera
							MAP_DrawMapToMapVideoBuffer();

							Update(true);
							Update(true);

							engine.room = 2;

							VIDEO_FadeIn(1);
						}
						break;
					default:
						// Reenable green door event if actor did not have key
						if (!map->event_enabling_room1[1] && (scene_step < 2)) {
							map->event_enabling_room1[1] = true;
						}

						// Reenable blue door event if actor did not have key
						if (!map->event_enabling_room1[2] && (scene_step < 1)) {
							map->event_enabling_room1[2] = true;
						}

						// Reenable yellow door event if actor did not have key
						if (!map->event_enabling_room1[3] && (scene_step < 3)) {
							map->event_enabling_room1[3] = true;
						}

						// Reenable red door event if actor did not have key
						if (!map->event_enabling_room1[4] && (scene_step < 4)) {
							map->event_enabling_room1[4] = true;
						}


						break;
				}

				break;
			case 2:// Room 2. Final boss
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
					case 1:// Event 1. Near final boss
						if (map->event_enabling_room2[1]) {
							map->event_enabling_room2[1] = false;

							scene_step = 2;

							BOSS_SetBehavior(BOSS_STATUS_CHASE);

							ACTOR_SetCombatMode(true);
						}
						break;
					case 2:// Event 2. Entry
						if (map->event_enabling_room2[2]) {
							map->event_enabling_room2[2] = false;

							scene_step = 1;

							BOSS_Load("BOSS4.DAT", 14 << 4, 3 << 4, BOSS_TYPE_STRONG, SPRITE_GRAPHICS_ID_ENEMY2_CHAT, SPRITE_GRAPHICS_ID_ENEMY2_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY2_FEET, SPRITE_GRAPHICS_ID_ENEMY2_BODY, SPRITE_GRAPHICS_ID_ENEMY2_HEAD, SPRITE_GRAPHICS_ID_ENEMY2_LARM, SPRITE_GRAPHICS_ID_ENEMY2_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, BOSS_STATUS_STATIC, BOSS_LIFE);
						}
						break;
					default:
						break;
				}

				// Scene events
				switch (scene_step) {
					case 1:// before fight
						break;
					case 2:// fight

						if (!boss->is_loaded) {
							ENEMY_UnloadEnemies();
							VIDEO_FadeOut(4);
							engine.ingame = false;
							engine.scene = 7;
							engine.room = 1;
						}
						break;
					default:
						break;
				}
		}

		Update(true);

		// Finish all if actor is dead
		if (actor->status_dead) GameOver();
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
	Scene6_UnloadAssets();
	MM_PopChunks(CT_TEMPORARY_SPRITE);
}
static void Scene6_Outro(void) {
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
				GFX_PCXImageToBuffer("OSCN6.DAT", "OUTRO1H.PCX", 120 * 120, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("OSCN6.DAT", "OUTRO1L.PCX", 120 * 120, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				GFX_PCXImageToBuffer("OSCN6.DAT", "OUTRO2H.PCX", 140 * 120, gfx->image_buffer3, &gfx->image_buffer3_width, &gfx->image_buffer3_height);
				AUDIO_LoadSong(AUDIO_SONG_3);
				step++;
				break;
			case 2:// Hide loading screen
				UI_HideLoadingScreen();
				step++;
				break;
			case 3:///
				VIDEO_ClearScreenBuffer();
				GFX_LoadPalette("PALETTES.DAT", "OSCN6.PCX", 256);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				VIDEO_FadeIn(4);

				TIMER_UpdateTimerTime(TIMER_AUDIO_NUMBER, 50);
				AUDIO_PlaySong(true);

				step++;
				break;
			case 4:// First image
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 126, 126, 10, 50, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 13, 53);
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
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 296, 46, 5, 1, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 40, 8, 4, 208);
				VIDEO_StringToScreenBuffer(12, 12, ui->txt_file[UI_TXT_SCN6I]->line[15], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(12, 22, ui->txt_file[UI_TXT_SCN6I]->line[16], FONT_SLIM_BLACK);
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
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 296, 46, 5, 1, 202);
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
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 296, 46, 5, 1, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 40, 8, 4, 208);
				VIDEO_StringToScreenBuffer(12, 12, ui->txt_file[UI_TXT_SCN6I]->line[17], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(12, 22, ui->txt_file[UI_TXT_SCN6I]->line[18], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(6000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 10:// Show first and Second image image
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 126, 126, 10, 50, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 13, 53);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 126, 160, 50, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 163, 53);
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
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 296, 46, 5, 1, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 40, 8, 4, 208);

				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 126, 126, 10, 50, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 13, 53);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 126, 160, 50, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 163, 53);
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
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 296, 46, 5, 1, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 40, 8, 4, 208);
				VIDEO_StringToScreenBuffer(12, 12, ui->txt_file[UI_TXT_SCN6I]->line[19], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(12, 22, ui->txt_file[UI_TXT_SCN6I]->line[20], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(6000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 15:// Clear chat Fade first and show Second image
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 296, 46, 5, 1, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 40, 8, 4, 208);

				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 126, 126, 10, 50, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 13, 53);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 146, 126, 160, 50, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer3, gfx->image_buffer3_width, gfx->image_buffer3_height, gfx->image_buffer3_width, gfx->image_buffer3_height, 163, 53);
				step++;
				break;
			case 16:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 17://Speech
				// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 296, 46, 5, 1, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 290, 40, 8, 4, 208);
				VIDEO_StringToScreenBuffer(12, 12, ui->txt_file[UI_TXT_SCN6I]->line[21], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(12, 22, ui->txt_file[UI_TXT_SCN6I]->line[22], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(6000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 18://Fade out
				VIDEO_FadeOut(4);
				AUDIO_StopSong();
				step++;
				break;
			case 19:// Load image 3
				GFX_PCXImageToBuffer("OSCN6.DAT", "OUTRO3H.PCX", 220 * 140, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				step++;
				break;
			case 20:/// Image 3 :: OUT
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 226, 146, 10, 40, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 13, 43);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				VIDEO_FadeIn(4);
				step++;
				break;
			case 21://Image 2

				// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 208);
				VIDEO_StringToScreenBuffer(28, 13, ui->txt_file[UI_TXT_SCN6I]->line[23], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(28, 25, ui->txt_file[UI_TXT_SCN6I]->line[24], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(6000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 22:// Clear speech
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 208);
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
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 208);
				VIDEO_StringToScreenBuffer(28, 13, ui->txt_file[UI_TXT_SCN6I]->line[25], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(28, 25, ui->txt_file[UI_TXT_SCN6I]->line[26], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(6000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 25:
				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 26://Speech
				// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 208);
				VIDEO_StringToScreenBuffer(28, 13, ui->txt_file[UI_TXT_SCN6I]->line[27], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(28, 25, ui->txt_file[UI_TXT_SCN6I]->line[28], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(2000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 27://End sequence
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

/** SCENARIO 7 :: End credits
 */
static void Scene7_EndCredits(void) {

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
				VIDEO_StringToScreenBuffer(100, 90, ui->txt_file[UI_TXT_END]->line[20], FONT_BIG_WHITE);

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

	engine.exit_game = true;
}

static void Test(void) {

	GFX_LoadSpriteGraphicsRLE("ISPR.DAT", "ACTOR.PCX", SPRITE_GRAPHICS_ID_ACTOR_INTRO, 20, 33, 12, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);

	GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "SPARK.PCX", SPRITE_GRAPHICS_ID_SPARK_EFFECT, 16, 16, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "PUNCHL.PCX", SPRITE_GRAPHICS_ID_PUNCH_EFFECT, 24, 16, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "PISTOLL.PCX", SPRITE_GRAPHICS_ID_PISTOL_EFFECT, 24, 16, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "SHOTGUNL.PCX", SPRITE_GRAPHICS_ID_SHOTGUN_EFFECT, 24, 16, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "UZIL.PCX", SPRITE_GRAPHICS_ID_UZI_EFFECT, 24, 16, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);

	GFX_LoadSpriteGraphicsRLE("CURSOR.DAT", "POINTER.PCX", SPRITE_GRAPHICS_ID_CURSOR, 7, 12, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("CURSOR.DAT", "AIM.PCX", SPRITE_GRAPHICS_ID_AIM, 15, 15, 5, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);

	GFX_LoadSpriteGraphicsRLE("PANEL.DAT", "CHAT.PCX", SPRITE_GRAPHICS_ID_CHAT, 220, 70, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);

	GFX_LoadSpriteGraphicsRLE("BULLETS.DAT", "BULLET1.PCX", SPRITE_GRAPHICS_ID_BULLET1, 8, 8, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("BULLETS.DAT", "BULLET2.PCX", SPRITE_GRAPHICS_ID_BULLET2, 8, 8, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("BULLETS.DAT", "BULLET3.PCX", SPRITE_GRAPHICS_ID_BULLET3, 8, 8, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("BULLETS.DAT", "WEBS.PCX", SPRITE_GRAPHICS_ID_WEBS, 8, 8, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);

	GFX_LoadSpriteGraphicsRLE("BULLETS.DAT", "GRENADE1.PCX", SPRITE_GRAPHICS_ID_GRENADE1, 8, 8, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("BULLETS.DAT", "GRENADES.PCX", SPRITE_GRAPHICS_ID_GRENADE_SHADOW, 8, 8, 2, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("BULLETS.DAT", "PUNCH1.PCX", SPRITE_GRAPHICS_ID_PUNCH, 12, 12, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("BULLETS.DAT", "PUKE.PCX", SPRITE_GRAPHICS_ID_PUKE, 16, 16, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("BULLETS.DAT", "RAY.PCX", SPRITE_GRAPHICS_ID_RAY, 16, 16, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("BULLETS.DAT", "WEB.PCX", SPRITE_GRAPHICS_ID_WEB, 16, 16, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);

	GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "SNIPPERL.PCX", SPRITE_GRAPHICS_ID_SNIPPER_EFFECT, 24, 16, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "BLOOD.PCX", SPRITE_GRAPHICS_ID_BLOOD, 4, 4, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "FIRE1.PCX", SPRITE_GRAPHICS_ID_FIRE1, 8, 8, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "ELECTRIC1.PCX", SPRITE_GRAPHICS_ID_ELECTRIC1, 8, 8, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "WATER1.PCX", SPRITE_GRAPHICS_ID_WATER1, 8, 8, 4, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);

	GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "EXPLOSION.PCX", SPRITE_GRAPHICS_ID_EXPLOSION1, 32, 32, 10, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "TIMER.PCX", SPRITE_GRAPHICS_ID_TIMER, 8, 8, 10, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "CLICK.PCX", SPRITE_GRAPHICS_ID_CLICK, 5, 5, 5, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("EFFECTS.DAT", "RAIN.PCX", SPRITE_GRAPHICS_ID_RAIN_EFFECT, 5, 5, 5, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);

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

	GFX_LoadSpriteGraphicsRLE("PANEL.DAT", "GUN00.PCX", SPRITE_GRAPHICS_ID_GUN0, 32, 16, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("PANEL.DAT", "GUN01.PCX", SPRITE_GRAPHICS_ID_GUN1, 32, 16, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("PANEL.DAT", "GUN02.PCX", SPRITE_GRAPHICS_ID_GUN2, 32, 16, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("PANEL.DAT", "GUN03.PCX", SPRITE_GRAPHICS_ID_GUN3, 32, 16, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);

	GFX_LoadSpriteGraphicsRLE("PANEL.DAT", "LIFEBAR.PCX", SPRITE_GRAPHICS_ID_LIFEBAR, 48, 6, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("PANEL.DAT", "GUN04.PCX", SPRITE_GRAPHICS_ID_GUN4, 32, 16, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("PANEL.DAT", "GUN05.PCX", SPRITE_GRAPHICS_ID_GUN5, 32, 16, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("PANEL.DAT", "GRENADE.PCX", SPRITE_GRAPHICS_ID_GRENADE, 16, 16, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);

	ACTOR_LoadActorAssets("ACTOR.DAT");

	GFX_LoadSpriteGraphicsRLE("MISC.DAT", "EMPTY.PCX", SPRITE_GRAPHICS_ID_EMPTY, 1, 1, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("MISC.DAT", "FUCK.PCX", SPRITE_GRAPHICS_ID_FUCK, 158, 81, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_TEMPORARY_SPRITE);
	GFX_LoadSpriteGraphicsRLE("MISC.DAT", "CHAT.PCX", SPRITE_GRAPHICS_ID_CHAT_SMALL, 55, 30, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("MISC.DAT", "CHAT_BIG.PCX", SPRITE_GRAPHICS_ID_CHAT_BIG, 140, 50, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);

	GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "BTNNEW.PCX", SPRITE_GRAPHICS_ID_BTN_NEW, 63, 14, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "BTNOPT.PCX", SPRITE_GRAPHICS_ID_BTN_OPT, 63, 14, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "BTNPASS.PCX", SPRITE_GRAPHICS_ID_BTN_PASS, 63, 14, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "BTNCRED.PCX", SPRITE_GRAPHICS_ID_BTN_CREDITS, 63, 14, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "BTNEXIT.PCX", SPRITE_GRAPHICS_ID_BTN_EXIT, 63, 14, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "BTNBACK.PCX", SPRITE_GRAPHICS_ID_BTN_BACK, 31, 15, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "BTNOK.PCX", SPRITE_GRAPHICS_ID_BTN_OK, 31, 15, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "BTNRIGHT.PCX", SPRITE_GRAPHICS_ID_BTN_RIGHT, 15, 15, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "BTNLEFT.PCX", SPRITE_GRAPHICS_ID_BTN_LEFT, 15, 15, 3, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);

	GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "POPTIONS.PCX", SPRITE_GRAPHICS_ID_PNL_OPTIONS, 62, 130, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "PPASS.PCX", SPRITE_GRAPHICS_ID_PNL_PASS, 120, 41, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "MTITLE.PCX", SPRITE_GRAPHICS_ID_TITLE, 243, 41, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
	GFX_LoadSpriteGraphicsRLE("SMENU.DAT", "MCHAT.PCX", SPRITE_GRAPHICS_ID_MCHAT, 136, 88, 1, SPRITE_TRANSP_COLOR, SPRITE_HIT_COLOR, CT_SPRITE);
}

void set_video_mode(int mode) {
	__dpmi_regs regs = {0};
	regs.x.ax = mode;
	__dpmi_int(0x10, &regs);
}

/** MAIN FUNCTION*******
 */
int main(int argc, char **argv) {
	gdb_start();

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
	LoadTexts();

	Test();

	//Logo();

	//Intro();

	engine.exit_game = false;

	engine.scene = 2;
	engine.room = 1;

	MOUSE_InitCursorSprite(SPRITE_GRAPHICS_ID_CURSOR);// Initialize mouse cursor

	while (!engine.exit_game) {
		switch (engine.scene) {
			case 0:// main menu
				engine.exit_menu = false;
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
			case 4:// scene 4 :: Mission 4 :: Praise for mercy
				Scene4_Intro();
				Scene4_Loop();
				if (!actor->status_dead && !engine.exit_menu) Scene4_Outro();
				break;
			case 5:// scene 5 :: Mission 5 :: Hit and run
				Scene5_Intro();
				Scene5_Loop();
				if (!actor->status_dead && !engine.exit_menu) Scene5_Outro();
				break;
			case 6:// scene 6 :: Mission 6 :: Bullet hell
				Scene6_Intro();
				Scene6_Loop();
				if (!actor->status_dead && !engine.exit_menu) Scene6_Outro();
				break;
			case 7:// scene 7 :: End scene
				Scene7_EndCredits();
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