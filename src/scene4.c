#include "engine/boss/boss.h"
#include "engine/engine.h"

/** SCENARIO 4 :: Mission 4 :: Praise for mercy
 */
void Scene4_GameOver(void) {
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
void Scene4_Intro(void) {
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
				AUDIO_PlayScenesSong(true);

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
void Scene4_LoadAssets(void) {

	// Load enemies and unload them just to get graphics
	ENEMY_Load("ENEMY10.DAT", 0, 0, 0, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_GUN0, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_NPC, 1);
	ENEMY_UnloadEnemies();

	// Load boss and unload it just to preload graphics
	BOSS_Load("BOSS3.DAT", 0, 0, BOSS_TYPE_PRIEST, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_RAY, BOSS_STATUS_STATIC, BOSS_LIFE);
	BOSS_Unload();

	ACTOR_Load(9 << 4, 47 << 4, SPRITE_GRAPHICS_ID_ACTOR_FEET, SPRITE_GRAPHICS_ID_ACTOR_BODY, SPRITE_GRAPHICS_ID_ACTOR_HEAD, SPRITE_GRAPHICS_ID_ACTOR_LARM, SPRITE_GRAPHICS_ID_ACTOR_RARM_BARE_HANDS, ACTOR_FACING_RIGHT);
	ACTOR_SetGun(actor->sprite_num, ACTOR_GUN_TYPE_BARE_HANDS);
	ACTOR_SetBulletStatus(99, 19, 199, 9, 9);

	UI_SetStatusPanels();// Set status panels
}
void Scene4_UnloadAssets(void) {
	// Enemy 1
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_CHAT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_FEET);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_BODY);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_HEAD);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_RARM);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY1_LARM);

	// Boss
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY3_CHAT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY3_FEET);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY3_BODY);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY3_HEAD);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY3_RARM);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_ENEMY3_LARM);

	MM_PopChunks(CT_TEMPORARY_SPRITE);
}
void Scene4_LoadRoom1(void) {

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
	ENEMY_Load("ENEMY10.DAT", 9, 11 << 4, 22 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 50);
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
	AUDIO_LoadSong(AUDIO_SONG_7);// Load sonG
}
void Scene4_LoadRoom2(void) {

	ENEMY_Load("ENEMY10.DAT", 0, 15 << 4, 11 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 50);
	ENEMY_Load("ENEMY10.DAT", 1, 18 << 4, 15 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 50);
	ENEMY_Load("ENEMY10.DAT", 2, 29 << 4, 15 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 50);
	ENEMY_Load("ENEMY10.DAT", 3, 31 << 4, 12 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 50);

	BOSS_Load("BOSS3.DAT", 21 << 4, 8 << 4, BOSS_TYPE_PRIEST, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_PUNCH, SPRITE_GRAPHICS_ID_RAY, BOSS_STATUS_STATIC, BOSS_LIFE);

	// Items
	ITEM_LoadItem(0, ENTITY_ID_ITEM_MEDIKIT, SPRITE_GRAPHICS_ID_ITEM_MEDIKIT, 18 << 4, 9 << 4);
	ITEM_LoadItem(1, ENTITY_ID_ITEM_MEDIKIT, SPRITE_GRAPHICS_ID_ITEM_MEDIKIT, 45 << 4, 26 << 4);
	ITEM_LoadItem(2, ENTITY_ID_ITEM_AMMO2, SPRITE_GRAPHICS_ID_ITEM_AMMO2, 1 << 4, 9 << 4);
	ITEM_LoadItem(3, ENTITY_ID_ITEM_AMMO3, SPRITE_GRAPHICS_ID_ITEM_AMMO3, 1 << 4, 26 << 4);
	ITEM_LoadItem(4, ENTITY_ID_ITEM_GRENADE, SPRITE_GRAPHICS_ID_ITEM_GRENADE, 45 << 4, 9 << 4);

	MAP_LoadMap("MAPSCN42.DAT", 47, 32, "TSCN42.DAT", "SCN4_2_BACK.PCX", "SCN4_2_FORE.PCX", "SCN4_2_MASK.PCX", 320 * 416, 128 * 128, 128 * 128);
	AUDIO_LoadSong(AUDIO_SONG_7);// Load sonG
}
void Scene4_SetHotspotsAndEvents(void) {
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
void Scene4_Loop(void) {
	bool end_sequence;
	int actor_tile_num;
	int sequence_step;
	int scene_step;
	int scene_counter;

	VIDEO_ClearScreenBuffer();
	GFX_LoadPalette("PALETTES.DAT", "SCN41.PCX", 256);

	VIDEO_StringToScreenBuffer(50, 60, ui->txt_file[UI_TXT_SCN4I]->line[40], FONT_BIG_BLACK);
	VIDEO_StringToScreenBuffer(50, 85, ui->txt_file[UI_TXT_SCN4I]->line[41], FONT_BIG_BLACK);
	VIDEO_StringToScreenBuffer(90, 135, ui->txt_file[UI_TXT_SCN4I]->line[42], FONT_SLIM_WHITE);
	VIDEO_StringToScreenBuffer(160, 135, ui->txt_file[UI_TXT_SCN4I]->line[43], FONT_SLIM_WHITE);

	VIDEO_VSync();
	VIDEO_ScreenBufferToVRAM();
	VIDEO_FadeIn(4);

	SetDelayTime(6000);

	Scene4_LoadAssets();
	Scene4_SetHotspotsAndEvents();

	// Initialize the Scene room
	switch (engine.room) {
		case 1:// Room 1. Garden
			Scene4_LoadRoom1();
			ACTOR_SetPosition(9 << 4, 47 << 4, ACTOR_FACING_UP);// Set actor position
			ACTOR_SetGun(actor->sprite_num, ACTOR_GUN_TYPE_PISTOL);
			ACTOR_SetCombatMode(true);
			ACTOR_Reload();
			CAM_Init(map->width_px, map->height_px, 0 << 4, 37 << 4);// Initialize camera
			MAP_DrawMapToMapVideoBuffer();
			TIMER_UpdateTimerTime(TIMER_AUDIO_NUMBER, 55);
			break;
		case 2:// Room 2. Church
			Scene4_LoadRoom2();
			ACTOR_SetPosition(23 << 4, 29 << 4, ACTOR_FACING_UP);// Set actor position
			ACTOR_SetCombatMode(true);
			ACTOR_Reload();
			CAM_Init(map->width_px, map->height_px, 13 << 4, 20 << 4);// Initialize camera
			MAP_DrawMapToMapVideoBuffer();
			TIMER_UpdateTimerTime(TIMER_AUDIO_NUMBER, 55);
			break;
		default:
			Error("Scene4_Loop function error", "Undefined room", "", ERROR_SYSTEM);
			break;
	}

	VIDEO_FadeOut(4);
	VIDEO_ClearScreenBuffer();

	// Set palette
	switch (engine.room) {
		case 1:// Room 1. First floor
			GFX_LoadPalette("PALETTES.DAT", "SCN41.PCX", 256);
			break;
		case 2:// Room 2. Second floor
			GFX_LoadPalette("PALETTES.DAT", "SCN42.PCX", 256);
			break;
		default:
			sprintf(engine.system_error_message1, "Scene4_Loop function error");
			sprintf(engine.system_error_message2, "Undefined room");
			sprintf(engine.system_error_message3, "Selected room: %u", engine.room);
			Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_SYSTEM);
			break;
	}

	Update(true);
	Update(true);

	scene_step = 0;
	sequence_step = 0;
	end_sequence = false;
	engine.ingame = true;

	while (!AwaitDelayTime()) {
		// Just wait
	}

	AUDIO_PlayIngameSong(true);
	VIDEO_FadeIn(1);
	MOUSE_ShowCursor();

	// Pre-loop
	switch (engine.room) {
		case 1:
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

			ACTOR_SetGun(actor->sprite_num, ACTOR_GUN_TYPE_PISTOL);
			ACTOR_SetCombatMode(true);
			ACTOR_Reload();

			break;
		case 2:
			break;
		default:
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
							BOSS_Unload();
							BULLET_UnloadBullets();
							ITEM_UnloadItems();
							PARTICLE_UnloadParticles();
							GRENADE_UnloadGrenades();
							NPC_UnloadNpcs();
							MAP_UnloadMap();

							Scene4_LoadRoom2();
							GFX_LoadPalette("PALETTES.DAT", "SCN42.PCX", 256);
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
				scene_counter++;
				if (scene_counter > 64) scene_counter = 0;

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

							scene_counter = 0;
							scene_step = 1;

							sequence_step = 0;
							end_sequence = false;
							while (!end_sequence) {
								switch (sequence_step) {
									case 0://
										UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN4D, 5, 6, 7, true, 100);
										sequence_step++;
										break;
									case 1://
										if (UI_IsSpeechFinished()) {
											sequence_step++;
										}
										break;
									case 2://
										UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN4D, 10, 11, 12, false, 100);
										sequence_step++;
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
						switch (scene_counter) {
							case 0:
								if (!enemy[0].is_loaded) ENEMY_Load("ENEMY10.DAT", 0, 1 << 4, 17 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_RAMPAGE, 50);
								break;
							case 16:
								if (!enemy[1].is_loaded) ENEMY_Load("ENEMY10.DAT", 1, 1 << 4, 18 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_RAMPAGE, 50);
								break;
							case 32:
								if (!enemy[2].is_loaded) ENEMY_Load("ENEMY10.DAT", 2, 45 << 4, 17 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_RAMPAGE, 50);
								break;
							case 48:
								if (!enemy[3].is_loaded) ENEMY_Load("ENEMY10.DAT", 3, 45 << 4, 18 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_RAMPAGE, 50);
								break;
							default:
								break;
						}

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
		if (actor->status_dead) Scene4_GameOver();
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
}
void Scene4_Outro(void) {
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
				AUDIO_PlayScenesSong(true);

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
				AUDIO_PlayScenesSong(true);

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
					VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 202);
					VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 208);

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
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 208);
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
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 208);

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
			case 49:// End chat
				GFX_HideSprite(chat_spr_num);
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 257, 166, 30, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 23);
				step++;
				break;
			case 50:// Just wait
				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 51://Girl's chat
				GFX_SetSpriteGraphic(chat_spr_num, 0, SPRITE_GRAPHICS_ID_CHAT_BIG, 0, 0);
				GFX_SetDefaultAnimation(chat_spr_num, true, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 70, 50);
				char_number = 0;
				step++;
				break;
			case 52://Girl's chat
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
			case 53:// End chat
				GFX_HideSprite(chat_spr_num);
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 257, 166, 30, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 23);
				step++;
				break;
			case 54:// Just wait
				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 55:// Actors chat
				GFX_SetDefaultAnimation(chat_spr_num, false, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 100, 20);
				GFX_ShowSprite(chat_spr_num);
				char_number = 0;
				step++;
				break;
			case 56:// Actors chat
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
			case 57:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 58:// Actors chat
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
			case 59:// End chat
				GFX_HideSprite(chat_spr_num);
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 257, 166, 30, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 23);
				step++;
				break;
			case 60:// Just wait
				SetDelayTime(1000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 61://Girl's chat
				GFX_SetSpriteGraphic(chat_spr_num, 0, SPRITE_GRAPHICS_ID_CHAT_BIG, 0, 0);
				GFX_SetDefaultAnimation(chat_spr_num, true, false, 10);
				GFX_SetSpritePosition(chat_spr_num, 70, 50);
				char_number = 0;
				step++;
				break;
			case 62://Girl's chat
				VIDEO_StringToScreenBuffer(76, 70, ui->txt_file[UI_TXT_SCN4I]->line[70], FONT_SLIM_BLACK);
				VIDEO_VSync();
				VIDEO_ScreenBufferToVRAM();
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 63:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 64://Girl's chat
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
			case 65:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 66://Girl's chat
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
			case 67:// End chat
				GFX_HideSprite(chat_spr_num);
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 257, 166, 30, 20, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 33, 23);
				step++;
				break;
			case 68:// Load new images
				VIDEO_ClearScreenBuffer();
				GFX_PCXImageToBuffer("OSCN4.DAT", "OUTRO9H.PCX", 150 * 130, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("OSCN4.DAT", "OUTRO10H.PCX", 150 * 130, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				step++;
				break;
			case 69:// Show images 9 and 10
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 156, 136, 2, 40, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 5, 43);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 156, 136, 160, 40, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 163, 43);
				step++;
				break;
			case 70:// Just wait
				SetDelayTime(2000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 71:// Clear image 10 and show just 9
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 156, 136, 2, 40, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 5, 43);
				step++;
				break;
			case 72:// Show speech
				// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 208);

				VIDEO_StringToScreenBuffer(28, 13, ui->txt_file[UI_TXT_SCN4I]->line[105], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(28, 25, ui->txt_file[UI_TXT_SCN4I]->line[106], FONT_SLIM_BLACK);
				step++;
				break;
			case 73:// Just wait
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 74:// Clear speech
				// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 208);
				step++;
				break;
			case 75:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 76:// Show speech
				// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 208);

				VIDEO_StringToScreenBuffer(28, 13, ui->txt_file[UI_TXT_SCN4I]->line[107], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(28, 25, ui->txt_file[UI_TXT_SCN4I]->line[108], FONT_SLIM_BLACK);
				step++;
				break;
			case 77:// Just wait
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 78:// Clear speech
				// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 208);
				step++;
				break;
			case 79:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 80:// Show speech
				// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 202);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 208);

				VIDEO_StringToScreenBuffer(28, 13, ui->txt_file[UI_TXT_SCN4I]->line[109], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(28, 25, ui->txt_file[UI_TXT_SCN4I]->line[110], FONT_SLIM_BLACK);
				step++;
				break;
			case 81:// Just wait
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