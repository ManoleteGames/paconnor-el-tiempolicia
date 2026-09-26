#include "engine/actor/actor.h"
#include "engine/engine.h"

/** SCENARIO 3 :: Mission 3 :: Behind enemy lines
 */
void Scene3_GameOver(void) {

	AUDIO_StopSong();
	VIDEO_FadeOutPause(4);
	ENEMY_UnloadEnemies();
	OBJECT_UnloadObjects();
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
void Scene3_Intro(void) {
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
				AUDIO_PlayScenesSong(true);

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
void Scene3_LoadAssets(void) {

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

	UI_SetStatusPanels();// Set status panels
}
void Scene3_UnloadAssets(void) {
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

	// NPCs
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_NPC_AGENT1);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_NPC_AGENT1_CHAT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_NPC_AGENT2);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_NPC_AGENT2_CHAT);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_NPC_CHEF);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_NPC_CHEF_CHAT);

	// Object
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_OBJECT2);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT);

	MM_PopChunks(CT_TEMPORARY_SPRITE);
}
void Scene3_LoadRoom1(void) {

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

	// Room 6
	// Enemy 18
	ENEMY_Load("ENEMY9.DAT", 18, 7 << 4, 26 << 4, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_UP, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_NPC, 100);
	// Enemy 19
	ENEMY_Load("ENEMY9.DAT", 19, 14 << 4, 27 << 4, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY3_FEET, SPRITE_GRAPHICS_ID_ENEMY3_BODY, SPRITE_GRAPHICS_ID_ENEMY3_HEAD, SPRITE_GRAPHICS_ID_ENEMY3_LARM, SPRITE_GRAPHICS_ID_ENEMY3_RARM, ENEMY_FACING_UP, ENEMY_GUN_PISTOL, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_WALKING_ARROUND_NPC, 100);
	// Enemy 20
	ENEMY_Load("ENEMY1.DAT", 20, 8 << 4, 36 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_UP, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_GUN1, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_HIDE, 10);


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
	ITEM_LoadItem(7, ENTITY_ID_ITEM_AMMO2, SPRITE_GRAPHICS_ID_ITEM_AMMO2, 36 << 4, 7 << 4);
	ITEM_LoadItem(8, ENTITY_ID_ITEM_AMMO2, SPRITE_GRAPHICS_ID_ITEM_AMMO2, 5 << 4, 3 << 4);

	MAP_LoadMap("MAPSCN31.DAT", 70, 50, "TSCN31.DAT", "SCN3_1_BACK.PCX", "SCN3_1_FORE.PCX", "SCN3_1_MASK.PCX", 320 * 416, 128 * 256, 128 * 128);

	AUDIO_LoadSong(AUDIO_SONG_7);// Load song
}
void Scene3_LoadRoom2(void) {

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
	ITEM_LoadItem(7, ENTITY_ID_ITEM_AMMO3, SPRITE_GRAPHICS_ID_ITEM_AMMO3, 21 << 4, 33 << 4);
	ITEM_LoadItem(8, ENTITY_ID_ITEM_DOCU, SPRITE_GRAPHICS_ID_ITEM_DOCU, 2 << 4, 12 << 4);
	ITEM_LoadItem(9, ENTITY_ID_ITEM_AMMO4, SPRITE_GRAPHICS_ID_ITEM_AMMO4, 6 << 4, 6 << 4);


	MAP_LoadMap("MAPSCN32.DAT", 57, 48, "TSCN32.DAT", "SCN3_2_BACK.PCX", "SCN3_2_FORE.PCX", "SCN3_2_MASK.PCX", 320 * 416, 128 * 256, 128 * 128);

	AUDIO_LoadSong(AUDIO_SONG_7);// Load song
}
void Scene3_LoadRoom3(void) {

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

	AUDIO_LoadSong(AUDIO_SONG_7);// Load song
}
void Scene3_SetHotspotsAndEvents(void) {

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
void Scene3_Loop(void) {
	bool end_sequence;
	int actor_tile_num;
	int sequence_step;
	int scene_step;

	VIDEO_ClearScreenBuffer();
	GFX_LoadPalette("PALETTES.DAT", "SCN31.PCX", 256);

	VIDEO_StringToScreenBuffer(50, 60, ui->txt_file[UI_TXT_SCN3I]->line[40], FONT_BIG_BLACK);
	VIDEO_StringToScreenBuffer(50, 85, ui->txt_file[UI_TXT_SCN3I]->line[41], FONT_BIG_BLACK);
	VIDEO_StringToScreenBuffer(90, 135, ui->txt_file[UI_TXT_SCN3I]->line[42], FONT_SLIM_WHITE);
	VIDEO_StringToScreenBuffer(160, 135, ui->txt_file[UI_TXT_SCN3I]->line[43], FONT_SLIM_WHITE);

	VIDEO_VSync();
	VIDEO_ScreenBufferToVRAM();
	VIDEO_FadeIn(4);

	SetDelayTime(6000);

	Scene3_LoadAssets();
	Scene3_SetHotspotsAndEvents();

	// Initialize the Scene room
	switch (engine.room) {
		case 1:// Room 1. Lower floor
			Scene3_LoadRoom1();
			ACTOR_SetPosition(65 << 4, 8 << 4, ACTOR_FACING_LEFT);// Set actor position
			ACTOR_SetGun(actor->sprite_num, ACTOR_GUN_TYPE_PISTOL);
			ACTOR_SetCombatMode(true);
			CAM_Init(map->width_px, map->height_px, 46 << 4, 0 << 4);// Initialize camera
			MAP_DrawMapToMapVideoBuffer();
			TIMER_UpdateTimerTime(TIMER_AUDIO_NUMBER, 55);
			break;
		case 2:// Room 2. Upper floor
			Scene3_LoadRoom2();
			ACTOR_SetPosition(27 << 4, 23 << 4, ACTOR_FACING_UP);// Set actor position
			ACTOR_SetCombatMode(true);
			CAM_Init(map->width_px, map->height_px, 17 << 4, 14 << 4);// Initialize camera
			MAP_DrawMapToMapVideoBuffer();
			TIMER_UpdateTimerTime(TIMER_AUDIO_NUMBER, 55);
			break;
		default:
			Error("Scene3_Loop function error", "Undefined room", "", ERROR_SYSTEM);
			break;
	}

	VIDEO_FadeOut(4);
	VIDEO_ClearScreenBuffer();

	// Set palette
	switch (engine.room) {
		case 1:// Room 1. First floor
			GFX_LoadPalette("PALETTES.DAT", "SCN31.PCX", 256);
			break;
		case 2:// Room 2. Second floor
			GFX_LoadPalette("PALETTES.DAT", "SCN32.PCX", 256);
			break;
		case 3:// Room 3. Back to First floor
			GFX_LoadPalette("PALETTES.DAT", "SCN31.PCX", 256);
			break;
		default:
			sprintf(engine.system_error_message1, "Scene3_Loop function error");
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

							ACTOR_SetGun(actor->sprite_num, ACTOR_GUN_TYPE_BARE_HANDS);
							ACTOR_SetCombatMode(false);

							AUDIO_StopSong();
							AUDIO_UnloadSong();
							AUDIO_LoadSong(AUDIO_SONG_5);// Load song
							TIMER_UpdateTimerTime(TIMER_AUDIO_NUMBER, 50);
							AUDIO_PlayIngameSong(true);

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
											ENEMY_SetBehavior(18, ENEMY_STATUS_STATIC_SHOOTER);
											ENEMY_SetBehavior(19, ENEMY_STATUS_CHASE);
											ENEMY_SetBehavior(20, ENEMY_STATUS_STATIC_SHOOTER);

											end_sequence = true;
										}
										break;
									default:
										end_sequence = true;
										break;
								}
								Update(false);
							}

							AUDIO_StopSong();
							AUDIO_UnloadSong();
							AUDIO_LoadSong(AUDIO_SONG_7);// Load song
							TIMER_UpdateTimerTime(TIMER_AUDIO_NUMBER, 55);
							AUDIO_PlayIngameSong(true);

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
							GFX_LoadPalette("PALETTES.DAT", "SCN32.PCX", 256);
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
								ACTOR_SetCombatMode(true);
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
								GFX_LoadPalette("PALETTES.DAT", "SCN31.PCX", 256);
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
											UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ENEMY3_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN3D, 31, 32, 355, true, 100);
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
		if (actor->status_dead) Scene3_GameOver();
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
	BOSS_Unload();
	ITEM_UnloadItems();
	ACTOR_UnloadActor();
	MAP_UnloadMap();
	Scene3_UnloadAssets();
}
void Scene3_Outro(void) {
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
				AUDIO_PlayScenesSong(true);

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
			case 46:// Load new images
				VIDEO_ClearScreenBuffer();
				GFX_PCXImageToBuffer("OSCN3.DAT", "OUTRO5H.PCX", 150 * 130, gfx->image_buffer1, &gfx->image_buffer1_width, &gfx->image_buffer1_height);
				GFX_PCXImageToBuffer("OSCN3.DAT", "OUTRO6H.PCX", 150 * 130, gfx->image_buffer2, &gfx->image_buffer2_width, &gfx->image_buffer2_height);
				step++;
				break;
			case 47:// Show images 5 and 6
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 156, 136, 2, 40, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 5, 43);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 156, 136, 160, 40, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer2, gfx->image_buffer2_width, gfx->image_buffer2_height, gfx->image_buffer2_width, gfx->image_buffer2_height, 163, 43);
				step++;
				break;
			case 48:// Just wait
				SetDelayTime(2000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 49:// Clear image 6 and show just 5
				VIDEO_ClearScreenBuffer();
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 156, 136, 2, 40, 222);
				VIDEO_BufferToScreenBuffer(gfx->image_buffer1, gfx->image_buffer1_width, gfx->image_buffer1_height, gfx->image_buffer1_width, gfx->image_buffer1_height, 5, 43);
				step++;
				break;
			case 50:// Show speech
				// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 239);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 208);

				VIDEO_StringToScreenBuffer(28, 13, ui->txt_file[UI_TXT_SCN3I]->line[68], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(28, 25, ui->txt_file[UI_TXT_SCN3I]->line[69], FONT_SLIM_BLACK);
				step++;
				break;
			case 51:// Just wait
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 52:// Clear speech
				// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 239);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 208);
				step++;
				break;
			case 53:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 54:// Show speech
				// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 239);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 208);

				VIDEO_StringToScreenBuffer(28, 13, ui->txt_file[UI_TXT_SCN3I]->line[72], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(28, 25, ui->txt_file[UI_TXT_SCN3I]->line[73], FONT_SLIM_BLACK);
				step++;
				break;
			case 55:// Just wait
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 56:// Clear speech
				// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 239);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 208);
				step++;
				break;
			case 57:// Just wait
				SetDelayTime(500);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 58:// Show speech
				// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 239);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 208);

				VIDEO_StringToScreenBuffer(28, 13, ui->txt_file[UI_TXT_SCN3I]->line[74], FONT_SLIM_BLACK);
				VIDEO_StringToScreenBuffer(28, 25, ui->txt_file[UI_TXT_SCN3I]->line[75], FONT_SLIM_BLACK);
				step++;
				break;
			case 59:// Just wait
				SetDelayTime(4000);
				while (!AwaitDelayTime()) {
					// Just wait
				}
				step++;
				break;
			case 60:// Clear speech
				// Draw text background
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 252, 30, 23, 7, 239);
				VIDEO_DrawSquareToScreenBuffer(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], video->screen_width, video->screen_height, 248, 26, 25, 9, 208);
				step++;
				break;
			case 61://End sequence
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