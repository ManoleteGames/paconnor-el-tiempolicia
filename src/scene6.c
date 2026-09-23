#include "engine/boss/boss.h"
#include "engine/engine.h"
#include "engine/misile/misile.h"

/** SCENARIO 6 :: Mission 6 :: Bullet hell
 */
void Scene6_GameOver(void) {
	AUDIO_StopSong();
	VIDEO_FadeOutPause(4);
	ENEMY_UnloadEnemies();
	OBJECT_UnloadObjects();
	EFFECT_UnloadEffects();
	BOSS_Unload();
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
void Scene6_Intro(void) {
	int step;
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
void Scene6_UnloadAssets(void) {
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

	// Object
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_OBJECT1);
	GFX_UnloadSpriteGraphic(SPRITE_GRAPHICS_ID_OBJECT1_PORTAIT);

	MM_PopChunks(CT_TEMPORARY_SPRITE);
}
void Scene6_LoadAssets(void) {

	// Load enemies and unload them just to get graphics
	ENEMY_Load("ENEMY11.DAT", 0, 0, 0, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_BARE_HANDS, SPRITE_GRAPHICS_ID_GUN0, SPRITE_GRAPHICS_ID_EMPTY, ENEMY_STATUS_STATIC_NPC, 1);
	ENEMY_UnloadEnemies();

	// Load objects and unload them just to get graphics
	OBJECT_LoadObject("OBJECT1.DAT", 0, ENTITY_ID_BARREL, SPRITE_GRAPHICS_ID_OBJECT1, SPRITE_GRAPHICS_ID_OBJECT1_PORTAIT, 0, 0);
	OBJECT_UnloadObjects();

	// Load boss and unload it just to preload graphics
	BOSS_Load("BOSS4.DAT", 0, 0, BOSS_TYPE_MECHA, SPRITE_GRAPHICS_ID_ENEMY2_CHAT, SPRITE_GRAPHICS_ID_ENEMY2_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY2_FEET, SPRITE_GRAPHICS_ID_ENEMY2_BODY, SPRITE_GRAPHICS_ID_ENEMY2_HEAD, SPRITE_GRAPHICS_ID_ENEMY2_LARM, SPRITE_GRAPHICS_ID_ENEMY2_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN0, SPRITE_GRAPHICS_ID_BULLET1, BOSS_STATUS_STATIC, BOSS_LIFE);
	BOSS_Unload();

	ACTOR_Load(9 << 4, 47 << 4, SPRITE_GRAPHICS_ID_ACTOR_FEET, SPRITE_GRAPHICS_ID_ACTOR_BODY, SPRITE_GRAPHICS_ID_ACTOR_HEAD, SPRITE_GRAPHICS_ID_ACTOR_LARM, SPRITE_GRAPHICS_ID_ACTOR_RARM_BARE_HANDS, ACTOR_FACING_RIGHT);
	ACTOR_SetGun(actor->sprite_num, ACTOR_GUN_TYPE_BARE_HANDS);
	ACTOR_SetBulletStatus(99, 19, 199, 9, 9);

	UI_SetStatusPanels();// Set status panels
}
void Scene6_LoadRoom1(void) {

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
	AUDIO_LoadSong(AUDIO_SONG_7);// Load song
}
void Scene6_LoadRoom2(void) {

	// Items
	ITEM_LoadItem(0, ENTITY_ID_ITEM_MEDIKIT, SPRITE_GRAPHICS_ID_ITEM_MEDIKIT, 26 << 4, 21 << 4);
	ITEM_LoadItem(1, ENTITY_ID_ITEM_MEDIKIT, SPRITE_GRAPHICS_ID_ITEM_MEDIKIT, 2 << 4, 21 << 4);

	ITEM_LoadItem(2, ENTITY_ID_ITEM_GRENADE, SPRITE_GRAPHICS_ID_ITEM_GRENADE, 26 << 4, 22 << 4);
	ITEM_LoadItem(3, ENTITY_ID_ITEM_AMMO3, SPRITE_GRAPHICS_ID_ITEM_AMMO3, 2 << 4, 22 << 4);
	ITEM_LoadItem(4, ENTITY_ID_ITEM_AMMO3, SPRITE_GRAPHICS_ID_ITEM_AMMO3, 2 << 4, 4 << 4);
	ITEM_LoadItem(5, ENTITY_ID_ITEM_AMMO4, SPRITE_GRAPHICS_ID_ITEM_AMMO4, 26 << 4, 4 << 4);
	ITEM_LoadItem(6, ENTITY_ID_ITEM_GRENADE, SPRITE_GRAPHICS_ID_ITEM_GRENADE, 26 << 4, 15 << 4);

	BOSS_Load("BOSS4.DAT", 11 << 4, 3 << 4, BOSS_TYPE_MECHA, SPRITE_GRAPHICS_ID_ENEMY2_CHAT, SPRITE_GRAPHICS_ID_ENEMY2_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY2_FEET, SPRITE_GRAPHICS_ID_ENEMY2_BODY, SPRITE_GRAPHICS_ID_ENEMY2_HEAD, SPRITE_GRAPHICS_ID_ENEMY2_LARM, SPRITE_GRAPHICS_ID_ENEMY2_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN2, SPRITE_GRAPHICS_ID_BULLET1, BOSS_STATUS_STATIC, 3000);

	MAP_LoadMap("MAPSCN62.DAT", 29, 27, "TSCN62.DAT", "SCN6_2_BACK.PCX", "SCN6_2_FORE.PCX", "SCN6_2_MASK.PCX", 320 * 416, 128 * 128, 128 * 128);
	AUDIO_LoadSong(AUDIO_SONG_7);// Load song
}
void Scene6_SetHotspotsAndEvents(void) {

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
void Scene6_Loop(void) {
	bool end_sequence;
	int actor_tile_num;
	int sequence_step;
	int scene_step;

	VIDEO_ClearScreenBuffer();
	GFX_LoadPalette("PALETTES.DAT", "SCN61.PCX", 256);

	VIDEO_StringToScreenBuffer(50, 60, ui->txt_file[UI_TXT_SCN6I]->line[40], FONT_BIG_BLACK);
	VIDEO_StringToScreenBuffer(50, 85, ui->txt_file[UI_TXT_SCN6I]->line[41], FONT_BIG_BLACK);
	VIDEO_StringToScreenBuffer(90, 135, ui->txt_file[UI_TXT_SCN6I]->line[42], FONT_SLIM_WHITE);
	VIDEO_StringToScreenBuffer(160, 135, ui->txt_file[UI_TXT_SCN6I]->line[43], FONT_SLIM_WHITE);

	VIDEO_VSync();
	VIDEO_ScreenBufferToVRAM();
	VIDEO_FadeIn(4);

	SetDelayTime(6000);

	Scene6_LoadAssets();
	Scene6_SetHotspotsAndEvents();

	// Initialize the Scene room
	switch (engine.room) {
		case 1:// Room 1. Outside base
			Scene6_LoadRoom1();
			ACTOR_SetPosition(19 << 4, 59 << 4, ACTOR_FACING_UP);// Set actor position
			ACTOR_SetGun(actor->sprite_num, ACTOR_GUN_TYPE_PISTOL);
			ACTOR_SetCombatMode(true);
			ACTOR_Reload();
			CAM_Init(map->width_px, map->height_px, 14 << 4, 50 << 4);// Initialize camera
			MAP_DrawMapToMapVideoBuffer();
			TIMER_UpdateTimerTime(TIMER_AUDIO_NUMBER, 55);
			break;
		case 2:// Room 2. Inside base
			Scene6_LoadRoom2();
			ACTOR_SetPosition(14 << 4, 21 << 4, ACTOR_FACING_UP);// Set actor position
			ACTOR_SetGun(actor->sprite_num, ACTOR_GUN_TYPE_PISTOL);
			ACTOR_SetCombatMode(true);
			ACTOR_Reload();
			CAM_Init(map->width_px, map->height_px, 9 << 4, 15 << 4);// Initialize camera
			MAP_DrawMapToMapVideoBuffer();
			TIMER_UpdateTimerTime(TIMER_AUDIO_NUMBER, 55);
			break;
		default:
			Error("Scene6_Loop function error", "Undefined room", "", ERROR_SYSTEM);
			break;
	}

	VIDEO_FadeOut(4);
	VIDEO_ClearScreenBuffer();

	// Set palette
	switch (engine.room) {
		case 1:// Room 1. Roof
			GFX_LoadPalette("PALETTES.DAT", "SCN61.PCX", 256);
			break;
		case 2:// Room 2. Top floor
			GFX_LoadPalette("PALETTES.DAT", "SCN62.PCX", 256);
			break;
		default:
			sprintf(engine.system_error_message1, "Scene6_Loop function error");
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

	AUDIO_PlaySong(true);
	VIDEO_FadeIn(1);
	MOUSE_ShowCursor();

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
					case 5:// Event 5. Get the green key and respawn enemies
						if (map->event_enabling_room1[5]) {
							map->event_enabling_room1[5] = false;

							// respawn enemies
							if (!enemy[0].is_loaded) ENEMY_Load("ENEMY11.DAT", 0, 6 << 4, 58 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_RIGHT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
							if (!enemy[1].is_loaded) ENEMY_Load("ENEMY11.DAT", 1, 29 << 4, 59 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_UP, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
							if (!enemy[2].is_loaded) ENEMY_Load("ENEMY11.DAT", 2, 41 << 4, 53 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN_RIGHT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
							if (!enemy[3].is_loaded) ENEMY_Load("ENEMY11.DAT", 3, 51 << 4, 57 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
							if (!enemy[4].is_loaded) ENEMY_Load("ENEMY11.DAT", 4, 61 << 4, 55 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
							if (!enemy[5].is_loaded) ENEMY_Load("ENEMY11.DAT", 5, 58 << 4, 46 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_RIGHT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
							if (!enemy[6].is_loaded) ENEMY_Load("ENEMY11.DAT", 6, 43 << 4, 42 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
							if (!enemy[7].is_loaded) ENEMY_Load("ENEMY11.DAT", 7, 49 << 4, 42 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
							if (!enemy[8].is_loaded) ENEMY_Load("ENEMY11.DAT", 8, 38 << 4, 42 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
						}
						break;
					case 6:// Event 6. Get yellow key and respawn enemies
						if (map->event_enabling_room1[6]) {
							map->event_enabling_room1[6] = false;

							// Enemies
							if (!enemy[0].is_loaded) ENEMY_Load("ENEMY11.DAT", 0, 6 << 4, 58 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_RIGHT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
							if (!enemy[1].is_loaded) ENEMY_Load("ENEMY11.DAT", 1, 29 << 4, 59 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_UP, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
							if (!enemy[2].is_loaded) ENEMY_Load("ENEMY11.DAT", 2, 41 << 4, 53 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN_RIGHT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
							if (!enemy[3].is_loaded) ENEMY_Load("ENEMY11.DAT", 3, 51 << 4, 57 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_LEFT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
							if (!enemy[4].is_loaded) ENEMY_Load("ENEMY11.DAT", 4, 61 << 4, 55 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
							if (!enemy[5].is_loaded) ENEMY_Load("ENEMY11.DAT", 5, 58 << 4, 46 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_RIGHT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
							if (!enemy[6].is_loaded) ENEMY_Load("ENEMY11.DAT", 6, 43 << 4, 42 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
							if (!enemy[7].is_loaded) ENEMY_Load("ENEMY11.DAT", 7, 49 << 4, 42 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
							if (!enemy[8].is_loaded) ENEMY_Load("ENEMY11.DAT", 8, 38 << 4, 42 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

							if (!enemy[9].is_loaded) ENEMY_Load("ENEMY11.DAT", 9, 23 << 4, 49 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN_LEFT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
							if (!enemy[10].is_loaded) ENEMY_Load("ENEMY11.DAT", 10, 17 << 4, 49 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN_RIGHT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
							if (!enemy[11].is_loaded) ENEMY_Load("ENEMY11.DAT", 11, 24 << 4, 42 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
							if (!enemy[12].is_loaded) ENEMY_Load("ENEMY11.DAT", 12, 9 << 4, 46 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_RIGHT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
							if (!enemy[13].is_loaded) ENEMY_Load("ENEMY11.DAT", 13, 15 << 4, 43 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
							if (!enemy[14].is_loaded) ENEMY_Load("ENEMY11.DAT", 14, 9 << 4, 39 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_UP_LEFT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
							if (!enemy[15].is_loaded) ENEMY_Load("ENEMY11.DAT", 15, 4 << 4, 31 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
							if (!enemy[16].is_loaded) ENEMY_Load("ENEMY11.DAT", 16, 27 << 4, 31 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
							if (!enemy[17].is_loaded) ENEMY_Load("ENEMY11.DAT", 17, 7 << 4, 23 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN_LEFT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
							if (!enemy[18].is_loaded) ENEMY_Load("ENEMY11.DAT", 18, 25 << 4, 23 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN_RIGHT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
							if (!enemy[19].is_loaded) ENEMY_Load("ENEMY11.DAT", 19, 16 << 4, 16 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
							if (!enemy[20].is_loaded) ENEMY_Load("ENEMY11.DAT", 20, 12 << 4, 12 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
							if (!enemy[21].is_loaded) ENEMY_Load("ENEMY11.DAT", 21, 7 << 4, 6 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
							if (!enemy[22].is_loaded) ENEMY_Load("ENEMY11.DAT", 22, 24 << 4, 6 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
							if (!enemy[23].is_loaded) ENEMY_Load("ENEMY11.DAT", 23, 28 << 4, 8 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
							if (!enemy[24].is_loaded) ENEMY_Load("ENEMY11.DAT", 24, 48 << 4, 3 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
							if (!enemy[25].is_loaded) ENEMY_Load("ENEMY11.DAT", 25, 54 << 4, 3 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);

							if (!enemy[26].is_loaded) ENEMY_Load("ENEMY11.DAT", 26, 38 << 4, 32 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
							if (!enemy[27].is_loaded) ENEMY_Load("ENEMY11.DAT", 27, 49 << 4, 35 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_DOWN, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_STATIC_SHOOTER, 100);
						}
						break;
					case 15:// Event 15. Spare
						if (map->event_enabling_room1[15]) {
							map->event_enabling_room1[15] = false;
						}
						break;
					case 16:// Event 16. Red door
						if (map->event_enabling_room1[16]) {
							map->event_enabling_room1[16] = false;

							VIDEO_FadeOut(4);
							ENEMY_UnloadEnemies();
							OBJECT_UnloadObjects();
							ITEM_UnloadItems();
							MISILE_UnloadMisiles();
							EFFECT_UnloadEffects();
							BULLET_UnloadBullets();
							PARTICLE_UnloadParticles();
							GRENADE_UnloadGrenades();
							NPC_UnloadNpcs();
							MAP_UnloadMap();

							Scene6_LoadRoom2();
							GFX_LoadPalette("PALETTES.DAT", "SCN62.PCX", 256);
							ACTOR_SetPosition(14 << 4, 21 << 4, ACTOR_FACING_UP);    // Set actor position
							CAM_Init(map->width_px, map->height_px, 9 << 4, 15 << 4);// Initialize camera
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

				// Emeny throw items
				if (enemy[0].action_dead && (enemy[0].action_step == 1)) {
					switch (rand() % 8) {
						case 0:// Nothing
							break;
						case 1:// Medikit
							if (!item[12].is_loaded) ITEM_LoadItem(12, ENTITY_ID_ITEM_MEDIKIT, SPRITE_GRAPHICS_ID_ITEM_MEDIKIT, enemy[0].pos_x, enemy[0].pos_y + 16);
							break;
						case 2:// Grenades
							if (!item[12].is_loaded) ITEM_LoadItem(12, ENTITY_ID_ITEM_GRENADE, SPRITE_GRAPHICS_ID_ITEM_GRENADE, enemy[0].pos_x, enemy[0].pos_y + 16);
							break;
						case 3:// Ammo 3
							if (!item[12].is_loaded) ITEM_LoadItem(12, ENTITY_ID_ITEM_AMMO3, SPRITE_GRAPHICS_ID_ITEM_AMMO3, enemy[0].pos_x, enemy[0].pos_y + 16);
							break;
						case 4:// Ammo 4
							if (!item[12].is_loaded) ITEM_LoadItem(12, ENTITY_ID_ITEM_AMMO4, SPRITE_GRAPHICS_ID_ITEM_AMMO4, enemy[0].pos_x, enemy[0].pos_y + 16);
							break;
						default:
							break;
					}
				}

				// Emeny throw items
				if (enemy[2].action_dead && (enemy[2].action_step == 1)) {
					switch (rand() % 8) {
						case 0:// Nothing
							break;
						case 1:// Medikit
							if (!item[13].is_loaded) ITEM_LoadItem(13, ENTITY_ID_ITEM_MEDIKIT, SPRITE_GRAPHICS_ID_ITEM_MEDIKIT, enemy[2].pos_x, enemy[2].pos_y + 16);
							break;
						case 2:// Grenades
							if (!item[13].is_loaded) ITEM_LoadItem(13, ENTITY_ID_ITEM_GRENADE, SPRITE_GRAPHICS_ID_ITEM_GRENADE, enemy[2].pos_x, enemy[2].pos_y + 16);
							break;
						case 3:// Ammo 3
							if (!item[13].is_loaded) ITEM_LoadItem(13, ENTITY_ID_ITEM_AMMO3, SPRITE_GRAPHICS_ID_ITEM_AMMO3, enemy[2].pos_x, enemy[2].pos_y + 16);
							break;
						case 4:// Ammo 4
							if (!item[13].is_loaded) ITEM_LoadItem(13, ENTITY_ID_ITEM_AMMO4, SPRITE_GRAPHICS_ID_ITEM_AMMO4, enemy[2].pos_x, enemy[2].pos_y + 16);
							break;
						default:
							break;
					}
				}

				// Emeny throw items
				if (enemy[5].action_dead && (enemy[5].action_step == 1)) {
					switch (rand() % 8) {
						case 0:// Nothing
							break;
						case 1:// Medikit
							if (!item[14].is_loaded) ITEM_LoadItem(14, ENTITY_ID_ITEM_MEDIKIT, SPRITE_GRAPHICS_ID_ITEM_MEDIKIT, enemy[5].pos_x, enemy[5].pos_y + 16);
							break;
						case 2:// Grenades
							if (!item[14].is_loaded) ITEM_LoadItem(14, ENTITY_ID_ITEM_GRENADE, SPRITE_GRAPHICS_ID_ITEM_GRENADE, enemy[5].pos_x, enemy[5].pos_y + 16);
							break;
						case 3:// Ammo 3
							if (!item[14].is_loaded) ITEM_LoadItem(14, ENTITY_ID_ITEM_AMMO3, SPRITE_GRAPHICS_ID_ITEM_AMMO3, enemy[5].pos_x, enemy[5].pos_y + 16);
							break;
						case 4:// Ammo 4
							if (!item[14].is_loaded) ITEM_LoadItem(14, ENTITY_ID_ITEM_AMMO4, SPRITE_GRAPHICS_ID_ITEM_AMMO4, enemy[5].pos_x, enemy[5].pos_y + 16);
							break;
						default:
							break;
					}
				}

				// Emeny throw items
				if (enemy[10].action_dead && (enemy[10].action_step == 1)) {
					switch (rand() % 8) {
						case 0:// Nothing
							break;
						case 1:// Medikit
							if (!item[15].is_loaded) ITEM_LoadItem(15, ENTITY_ID_ITEM_MEDIKIT, SPRITE_GRAPHICS_ID_ITEM_MEDIKIT, enemy[10].pos_x, enemy[10].pos_y + 16);
							break;
						case 2:// Grenades
							if (!item[15].is_loaded) ITEM_LoadItem(15, ENTITY_ID_ITEM_GRENADE, SPRITE_GRAPHICS_ID_ITEM_GRENADE, enemy[10].pos_x, enemy[10].pos_y + 16);
							break;
						case 3:// Ammo 3
							if (!item[15].is_loaded) ITEM_LoadItem(15, ENTITY_ID_ITEM_AMMO3, SPRITE_GRAPHICS_ID_ITEM_AMMO3, enemy[10].pos_x, enemy[10].pos_y + 16);
							break;
						case 4:// Ammo 4
							if (!item[15].is_loaded) ITEM_LoadItem(15, ENTITY_ID_ITEM_AMMO4, SPRITE_GRAPHICS_ID_ITEM_AMMO4, enemy[10].pos_x, enemy[10].pos_y + 16);
							break;
						default:
							break;
					}
				}

				// Emeny throw items
				if (enemy[15].action_dead && (enemy[15].action_step == 1)) {
					switch (rand() % 8) {
						case 0:// Nothing
							break;
						case 1:// Medikit
							if (!item[12].is_loaded) ITEM_LoadItem(12, ENTITY_ID_ITEM_MEDIKIT, SPRITE_GRAPHICS_ID_ITEM_MEDIKIT, enemy[15].pos_x, enemy[15].pos_y + 16);
							break;
						case 2:// Grenades
							if (!item[12].is_loaded) ITEM_LoadItem(12, ENTITY_ID_ITEM_GRENADE, SPRITE_GRAPHICS_ID_ITEM_GRENADE, enemy[15].pos_x, enemy[15].pos_y + 16);
							break;
						case 3:// Ammo 3
							if (!item[12].is_loaded) ITEM_LoadItem(12, ENTITY_ID_ITEM_AMMO3, SPRITE_GRAPHICS_ID_ITEM_AMMO3, enemy[15].pos_x, enemy[15].pos_y + 16);
							break;
						case 4:// Ammo 4
							if (!item[12].is_loaded) ITEM_LoadItem(12, ENTITY_ID_ITEM_AMMO4, SPRITE_GRAPHICS_ID_ITEM_AMMO4, enemy[15].pos_x, enemy[15].pos_y + 16);
							break;
						default:
							break;
					}
				}

				// Emeny throw items
				if (enemy[20].action_dead && (enemy[20].action_step == 1)) {
					switch (rand() % 8) {
						case 0:// Nothing
							break;
						case 1:// Medikit
							if (!item[13].is_loaded) ITEM_LoadItem(13, ENTITY_ID_ITEM_MEDIKIT, SPRITE_GRAPHICS_ID_ITEM_MEDIKIT, enemy[20].pos_x, enemy[20].pos_y + 16);
							break;
						case 2:// Grenades
							if (!item[13].is_loaded) ITEM_LoadItem(13, ENTITY_ID_ITEM_GRENADE, SPRITE_GRAPHICS_ID_ITEM_GRENADE, enemy[20].pos_x, enemy[20].pos_y + 16);
							break;
						case 3:// Ammo 3
							if (!item[13].is_loaded) ITEM_LoadItem(13, ENTITY_ID_ITEM_AMMO3, SPRITE_GRAPHICS_ID_ITEM_AMMO3, enemy[20].pos_x, enemy[20].pos_y + 16);
							break;
						case 4:// Ammo 4
							if (!item[13].is_loaded) ITEM_LoadItem(13, ENTITY_ID_ITEM_AMMO4, SPRITE_GRAPHICS_ID_ITEM_AMMO4, enemy[20].pos_x, enemy[20].pos_y + 16);
							break;
						default:
							break;
					}
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

							sequence_step = 0;
							end_sequence = false;
							while (!end_sequence) {
								switch (sequence_step) {
									case 0://
										UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ENEMY2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN6D, 1, 2, 3, true, 100);
										sequence_step++;
										break;
									case 1://
										if (UI_IsSpeechFinished()) {
											sequence_step++;
										}
										break;
									case 2://
										UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ENEMY2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN6D, 4, 5, 6, true, 100);
										sequence_step++;
										break;
									case 3://
										if (UI_IsSpeechFinished()) {
											sequence_step++;
										}
										break;
									case 4://
										UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ACTORCHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN6D, 7, 8, 9, false, 100);
										sequence_step++;
										break;
									case 5://
										if (UI_IsSpeechFinished()) {
											sequence_step++;
										}
										break;
									case 6://
										UI_ShowSpeech(gfx_chat_panel, SPRITE_GRAPHICS_ID_ENEMY2_CHAT, SPRITE_GRAPHICS_ID_CHAT, UI_TXT_SCN6D, 10, 11, 355, true, 100);
										sequence_step++;
										break;
									case 7://
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
					case 2:// Event 2. Entry
						if (map->event_enabling_room2[2]) {
							map->event_enabling_room2[2] = false;

							scene_step = 1;

							ACTOR_SetCombatMode(true);
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
						// Emeny throw items
						if (enemy[0].action_dead && (enemy[0].action_step == 1)) {
							switch (rand() % 8) {
								case 0:// Nothing
									break;
								case 1:// Medikit
									if (!item[7].is_loaded) ITEM_LoadItem(7, ENTITY_ID_ITEM_MEDIKIT, SPRITE_GRAPHICS_ID_ITEM_MEDIKIT, enemy[0].pos_x, enemy[0].pos_y + 16);
									break;
								case 2:// Grenades
									if (!item[7].is_loaded) ITEM_LoadItem(7, ENTITY_ID_ITEM_GRENADE, SPRITE_GRAPHICS_ID_ITEM_GRENADE, enemy[0].pos_x, enemy[0].pos_y + 16);
									break;
								case 3:// Ammo 3
									if (!item[7].is_loaded) ITEM_LoadItem(7, ENTITY_ID_ITEM_AMMO3, SPRITE_GRAPHICS_ID_ITEM_AMMO3, enemy[0].pos_x, enemy[0].pos_y + 16);
									break;
								case 4:// Ammo 4
									if (!item[7].is_loaded) ITEM_LoadItem(7, ENTITY_ID_ITEM_AMMO4, SPRITE_GRAPHICS_ID_ITEM_AMMO4, enemy[0].pos_x, enemy[0].pos_y + 16);
									break;
								default:
									break;
							}
						}

						if (!enemy[0].is_loaded) ENEMY_Load("ENEMY11.DAT", 0, 13 << 4, 23 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_RIGHT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_CHASE, 100);
						if (!enemy[1].is_loaded) ENEMY_Load("ENEMY11.DAT", 1, 14 << 4, 23 << 4, SPRITE_GRAPHICS_ID_ENEMY1_CHAT, SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT, SPRITE_GRAPHICS_ID_ENEMY1_FEET, SPRITE_GRAPHICS_ID_ENEMY1_BODY, SPRITE_GRAPHICS_ID_ENEMY1_HEAD, SPRITE_GRAPHICS_ID_ENEMY1_LARM, SPRITE_GRAPHICS_ID_ENEMY1_RARM, ENEMY_FACING_RIGHT, ENEMY_GUN_AK, SPRITE_GRAPHICS_ID_GUN3, SPRITE_GRAPHICS_ID_BULLET1, ENEMY_STATUS_CHASE, 100);

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

				engine.debug1_INT = enemy[0].action_hit;
				engine.debug2_INT = enemy[0].action_walk;
				engine.debug3_INT = enemy[1].action_hit;
				engine.debug4_INT = enemy[1].action_walk;
		}

		Update(true);

		// Finish all if actor is dead
		if (actor->status_dead) Scene6_GameOver();
	}

	VIDEO_FadeOut(4);
	VIDEO_ClearScreen();
	AUDIO_StopSong();
	ENEMY_UnloadEnemies();
	ITEM_UnloadItems();
	OBJECT_UnloadObjects();
	EFFECT_UnloadEffects();
	BULLET_UnloadBullets();
	MISILE_UnloadMisiles();
	PARTICLE_UnloadParticles();
	GRENADE_UnloadGrenades();
	ACTOR_UnloadActor();
	MAP_UnloadMap();
	Scene6_UnloadAssets();
}
void Scene6_Outro(void) {
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
	BOSS_Unload();
	ITEM_UnloadItems();
	GFX_UnloadSprites();
	AUDIO_StopSong();
	AUDIO_UnloadSong();
}
