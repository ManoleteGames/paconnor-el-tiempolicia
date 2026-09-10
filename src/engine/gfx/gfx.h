#ifndef GFX_H
#define GFX_H

#include "../types/types.h"

#define SPRITE_MAX_GRAPHICS 255
#define SPRITE_MAX_STACK 128
#define SPRITE_TRANSP_COLOR 201
#define SPRITE_HIT_COLOR 234

#define FP 16// fraction bits
#define FONT_SLIM_GFX_WIDTH 128
#define FONT_SLIM_GFX_HEIGHT 32
#define FONT_BIG_GFX_WIDTH 256
#define FONT_BIG_GFX_HEIGHT 64
#define FONT_TRANSPARENT_COLOR 201
#define FONT_ASCII_CODE_N 165

#define FONT_SLIM_BLACK 0
#define FONT_SLIM_WHITE 1
#define FONT_BIG_WHITE 2
#define FONT_BIG_BLACK 3
#define FONT_SLIM_YELLOW 4

#define SPRITE_SUBSPRITES_MAX 5
#define SPRITE_ANIMATION_MAX_FRAMES 20

// Cursor graphics
#define SPRITE_GRAPHICS_ID_CURSOR 1
#define SPRITE_GRAPHICS_ID_AIM 2

// Actor graphics
#define SPRITE_GRAPHICS_ID_ACTOR_PORTAIT 6
#define SPRITE_GRAPHICS_ID_ACTORCHAT 7
#define SPRITE_GRAPHICS_ID_ACTOR_FEET 8
#define SPRITE_GRAPHICS_ID_ACTOR_BODY 9
#define SPRITE_GRAPHICS_ID_ACTOR_HEAD 10
#define SPRITE_GRAPHICS_ID_ACTOR_LARM 11
#define SPRITE_GRAPHICS_ID_ACTOR_RARM_BARE_HANDS 12
#define SPRITE_GRAPHICS_ID_ACTOR_RARM_PISTOL 13
#define SPRITE_GRAPHICS_ID_ACTOR_RARM_SHOTGUN 14
#define SPRITE_GRAPHICS_ID_ACTOR_RARM_UZI 15
#define SPRITE_GRAPHICS_ID_ACTOR_RARM_RIFLE 16
#define SPRITE_GRAPHICS_ID_ACTOR_INTRO 17

// Gun graphics
#define SPRITE_GRAPHICS_ID_ACTOR_BARE_HANDS 20
#define SPRITE_GRAPHICS_ID_ACTOR_PISTOL 21
#define SPRITE_GRAPHICS_ID_ACTOR_SHOOTGUN 22
#define SPRITE_GRAPHICS_ID_ACTOR_UZI 23
#define SPRITE_GRAPHICS_ID_ACTOR_SNIPPER 24

// Items graphics
#define SPRITE_GRAPHICS_ID_ITEM_DOCU 29
#define SPRITE_GRAPHICS_ID_ITEM_MEDIKIT 30
#define SPRITE_GRAPHICS_ID_ITEM_AMMO1 31
#define SPRITE_GRAPHICS_ID_ITEM_AMMO2 32
#define SPRITE_GRAPHICS_ID_ITEM_AMMO3 33
#define SPRITE_GRAPHICS_ID_ITEM_AMMO4 34
#define SPRITE_GRAPHICS_ID_ITEM_GRENADE 35
#define SPRITE_GRAPHICS_ID_ITEM_RED_KEY 36
#define SPRITE_GRAPHICS_ID_ITEM_BLUE_KEY 37
#define SPRITE_GRAPHICS_ID_ITEM_YELLOW_KEY 38
#define SPRITE_GRAPHICS_ID_ITEM_GREEN_KEY 39

// Ammo graphics
#define SPRITE_GRAPHICS_ID_BULLET1 40
#define SPRITE_GRAPHICS_ID_BULLET2 41
#define SPRITE_GRAPHICS_ID_BULLET3 42
#define SPRITE_GRAPHICS_ID_BULLET4 43
#define SPRITE_GRAPHICS_ID_BULLET5 44
#define SPRITE_GRAPHICS_ID_PUKE 45
#define SPRITE_GRAPHICS_ID_WEB 46
#define SPRITE_GRAPHICS_ID_WEBS 47
#define SPRITE_GRAPHICS_ID_GRENADE1 48
#define SPRITE_GRAPHICS_ID_GRENADE2 49
#define SPRITE_GRAPHICS_ID_GRENADE_SHADOW 50
#define SPRITE_GRAPHICS_ID_MISILE1 51
#define SPRITE_GRAPHICS_ID_MISILE2 52
#define SPRITE_GRAPHICS_ID_MISILE_SHADOW 53
#define SPRITE_GRAPHICS_ID_RAY 54

// Object graphics
#define SPRITE_GRAPHICS_ID_OBJECT1 60
#define SPRITE_GRAPHICS_ID_OBJECT1_PORTAIT 61
#define SPRITE_GRAPHICS_ID_OBJECT2 62
#define SPRITE_GRAPHICS_ID_OBJECT2_PORTAIT 63
#define SPRITE_GRAPHICS_ID_OBJECT3 64
#define SPRITE_GRAPHICS_ID_OBJECT3_PORTAIT 65
#define SPRITE_GRAPHICS_ID_OBJECT4 66
#define SPRITE_GRAPHICS_ID_OBJECT4_PORTAIT 67
#define SPRITE_GRAPHICS_ID_OBJECT5 68
#define SPRITE_GRAPHICS_ID_OBJECT5_PORTAIT 69

// Effects graphics
#define SPRITE_GRAPHICS_ID_EXPLOSION1 70
#define SPRITE_GRAPHICS_ID_TIMER 71
#define SPRITE_GRAPHICS_ID_CLICK 72
#define SPRITE_GRAPHICS_ID_FIRE1 73
#define SPRITE_GRAPHICS_ID_ELECTRIC1 74
#define SPRITE_GRAPHICS_ID_PUNCH_EFFECT 75
#define SPRITE_GRAPHICS_ID_BLOOD 76
#define SPRITE_GRAPHICS_ID_PUNCH 77
#define SPRITE_GRAPHICS_ID_PISTOL_EFFECT 78
#define SPRITE_GRAPHICS_ID_SHOTGUN_EFFECT 79
#define SPRITE_GRAPHICS_ID_UZI_EFFECT 80
#define SPRITE_GRAPHICS_ID_SNIPPER_EFFECT 81
#define SPRITE_GRAPHICS_ID_SPARK_EFFECT 82
#define SPRITE_GRAPHICS_ID_RAIN_EFFECT 83
#define SPRITE_GRAPHICS_ID_WATER1 84

// Panel graphics
#define SPRITE_GRAPHICS_ID_LIFEBAR 85
#define SPRITE_GRAPHICS_ID_GRENADE 86
#define SPRITE_GRAPHICS_ID_CHAT 87
#define SPRITE_GRAPHICS_ID_GUN0 88
#define SPRITE_GRAPHICS_ID_GUN1 89
#define SPRITE_GRAPHICS_ID_GUN2 90
#define SPRITE_GRAPHICS_ID_GUN3 91
#define SPRITE_GRAPHICS_ID_GUN4 92
#define SPRITE_GRAPHICS_ID_GUN5 93

// Menu graphics
#define SPRITE_GRAPHICS_ID_BTN_NEW 95
#define SPRITE_GRAPHICS_ID_BTN_OPT 96
#define SPRITE_GRAPHICS_ID_BTN_PASS 97
#define SPRITE_GRAPHICS_ID_BTN_CREDITS 98
#define SPRITE_GRAPHICS_ID_BTN_EXIT 99
#define SPRITE_GRAPHICS_ID_BTN_BACK 100
#define SPRITE_GRAPHICS_ID_BTN_LEFT 101
#define SPRITE_GRAPHICS_ID_BTN_RIGHT 102
#define SPRITE_GRAPHICS_ID_TITLE 103
#define SPRITE_GRAPHICS_ID_PNL_OPTIONS 104
#define SPRITE_GRAPHICS_ID_MCHAT 105
#define SPRITE_GRAPHICS_ID_PNL_PASS 106
#define SPRITE_GRAPHICS_ID_BTN_OK 107

// Enemy graphics
#define SPRITE_GRAPHICS_ID_ENEMY1 110
#define SPRITE_GRAPHICS_ID_ENEMY1_PORTAIT 111
#define SPRITE_GRAPHICS_ID_ENEMY1_CHAT 112
#define SPRITE_GRAPHICS_ID_ENEMY1_FEET 113
#define SPRITE_GRAPHICS_ID_ENEMY1_BODY 114
#define SPRITE_GRAPHICS_ID_ENEMY1_HEAD 115
#define SPRITE_GRAPHICS_ID_ENEMY1_LARM 116
#define SPRITE_GRAPHICS_ID_ENEMY1_RARM 117

#define SPRITE_GRAPHICS_ID_ENEMY2 120
#define SPRITE_GRAPHICS_ID_ENEMY2_PORTAIT 121
#define SPRITE_GRAPHICS_ID_ENEMY2_CHAT 122
#define SPRITE_GRAPHICS_ID_ENEMY2_FEET 123
#define SPRITE_GRAPHICS_ID_ENEMY2_BODY 124
#define SPRITE_GRAPHICS_ID_ENEMY2_HEAD 125
#define SPRITE_GRAPHICS_ID_ENEMY2_LARM 126
#define SPRITE_GRAPHICS_ID_ENEMY2_RARM 127

// Enemy graphics
#define SPRITE_GRAPHICS_ID_ENEMY3 130
#define SPRITE_GRAPHICS_ID_ENEMY3_PORTAIT 131
#define SPRITE_GRAPHICS_ID_ENEMY3_CHAT 132
#define SPRITE_GRAPHICS_ID_ENEMY3_FEET 133
#define SPRITE_GRAPHICS_ID_ENEMY3_BODY 134
#define SPRITE_GRAPHICS_ID_ENEMY3_HEAD 135
#define SPRITE_GRAPHICS_ID_ENEMY3_LARM 136
#define SPRITE_GRAPHICS_ID_ENEMY3_RARM 137

#define SPRITE_GRAPHICS_ID_ENEMY4 140
#define SPRITE_GRAPHICS_ID_ENEMY4_PORTAIT 141
#define SPRITE_GRAPHICS_ID_ENEMY4_CHAT 142
#define SPRITE_GRAPHICS_ID_ENEMY4_FEET 143
#define SPRITE_GRAPHICS_ID_ENEMY4_BODY 144
#define SPRITE_GRAPHICS_ID_ENEMY4_HEAD 145
#define SPRITE_GRAPHICS_ID_ENEMY4_LARM 146
#define SPRITE_GRAPHICS_ID_ENEMY4_RARM 147

// Enemy graphics
#define SPRITE_GRAPHICS_ID_ENEMY5 150
#define SPRITE_GRAPHICS_ID_ENEMY5_PORTAIT 151
#define SPRITE_GRAPHICS_ID_ENEMY5_CHAT 152
#define SPRITE_GRAPHICS_ID_ENEMY5_FEET 153
#define SPRITE_GRAPHICS_ID_ENEMY5_BODY 154
#define SPRITE_GRAPHICS_ID_ENEMY5_HEAD 155
#define SPRITE_GRAPHICS_ID_ENEMY5_LARM 156
#define SPRITE_GRAPHICS_ID_ENEMY5_RARM 157

#define SPRITE_GRAPHICS_ID_ENEMY6 160
#define SPRITE_GRAPHICS_ID_ENEMY6_PORTAIT 161
#define SPRITE_GRAPHICS_ID_ENEMY6_CHAT 162
#define SPRITE_GRAPHICS_ID_ENEMY6_FEET 163
#define SPRITE_GRAPHICS_ID_ENEMY6_BODY 164
#define SPRITE_GRAPHICS_ID_ENEMY6_HEAD 165
#define SPRITE_GRAPHICS_ID_ENEMY6_LARM 166
#define SPRITE_GRAPHICS_ID_ENEMY6_RARM 167

// NPCS
#define SPRITE_GRAPHICS_ID_NPC_SARA 170
#define SPRITE_GRAPHICS_ID_NPC_SARA_CHAT 171
#define SPRITE_GRAPHICS_ID_NPC_DROID1 172
#define SPRITE_GRAPHICS_ID_NPC_DROID1_CHAT 173
#define SPRITE_GRAPHICS_ID_NPC_BOSS 174
#define SPRITE_GRAPHICS_ID_NPC_BOSS_CHAT 175
#define SPRITE_GRAPHICS_ID_NPC_AGENT1 176
#define SPRITE_GRAPHICS_ID_NPC_AGENT1_CHAT 177
#define SPRITE_GRAPHICS_ID_NPC_AGENT2 178
#define SPRITE_GRAPHICS_ID_NPC_AGENT2_CHAT 179
#define SPRITE_GRAPHICS_ID_NPC_DROID2 180
#define SPRITE_GRAPHICS_ID_NPC_DROID2_CHAT 181
#define SPRITE_GRAPHICS_ID_NPC_DROID3 182
#define SPRITE_GRAPHICS_ID_NPC_DROID3_CHAT 183
#define SPRITE_GRAPHICS_ID_NPC_CHEF 184
#define SPRITE_GRAPHICS_ID_NPC_CHEF_CHAT 185

// Temporary graphics
#define SPRITE_GRAPHICS_ID_WORMS 200
#define SPRITE_GRAPHICS_ID_CHAT_SMALL 201
#define SPRITE_GRAPHICS_ID_FUCK 202
#define SPRITE_GRAPHICS_ID_EMPTY 203
#define SPRITE_GRAPHICS_ID_CHAT_BIG 204

/// GFX.C ///
extern Graphics *gfx;
extern StatusPanel *gfx_actor_status_panel;
extern StatusPanel *gfx_enemy_status_panel;
extern ChatPanel *gfx_chat_panel;

extern Graphic *gfx_sprite_graphics_stack;
extern Sprite *gfx_sprite_stack;
extern Sprite *gfx_sprite_cursor;

extern int gfx_sprite_counter;
void GFX_Init(void);
void GFX_LoadSpriteGraphicsRLE(const char *dat_name, const char *asset_name, int id, int width_px, int height_px, int num_frames, int transparent_color, int hit_color, int mem_type);
void GFX_UnloadSpriteGraphics(void);
void GFX_InitSprite(byte entity_id, byte entity_num, int number, byte priority, int width_px, int height_px);
void GFX_InitCursorSprite(byte entity_id, byte entity_num, int width_px, int height_px);
void GFX_SetSpriteCursorGraphic(int graphics_id, int offset_x, int offset_y);
void GFX_SetDefaultCursorAnimation(bool inverted, bool loop, byte speed);
void GFX_SetCursorSpritePosition(int screen_pos_x, int screen_pos_y);
void GFX_SetCursorSpriteFrame(int anim_number, int frame);
void GFX_DrawCursorSprite(void);
void GFX_UnloadSprite(int number);
void GFX_UnloadSprites(void);
void GFX_DrawSprites(void);
void GFX_LoadImage(const char *filename, const char *subfile, int size);
void GFX_LoadFont(const char *filename, const char *subfile, int size, word char_w, word char_h, int font_number);
void GFX_LoadPalette(const char *dat_file, const char *asset_file, int size);
void GFX_UpdateSprites(void);
void GFX_UpdateCursorSprite(void);
void GFX_SetPanelGraphics(StatusPanel *panel, int portait_graphics_id, int lifebar_graphics_id, int gun_graphics_id, int key_graphics_id);
void GFX_SetPanelPosition(StatusPanel *panel, int x, int y, int portait_x, int portait_y, int lifebar_x, int lifebar_y, int gun_x, int gun_y, int grenade_x, int grenade_y, int key_x, int key_y);
void GFX_UpdatePanelGun(StatusPanel *panel, int gun_graphics_id, int gun_type, int total_bullets, int bullets_number, int grenade_graphics_id, int grenades_number, int key_type, int key_graphics_id);
void GFX_ShowPanel(StatusPanel *panel, bool show, int timeout);
void GFX_UpdatePanel(StatusPanel *panel, int life, int old_life, int max_life, int portait_frame);
void GFX_SetPanelPortait(StatusPanel *panel, int portait_frame);
void GFX_PCXImageToBuffer(const char *dat_name, const char *asset_name, int size, byte *buffer, word *width, word *height);
void GFX_SetSpritePosition(int sprite_num, int screen_pos_x, int screen_pos_y);
void GFX_SetSpriteAnimation(int sprite_num, int anim_num, SpriteAnimation anim_data, bool reset_current_frame);
void GFX_ClearBuffer(byte *buffer, int width, int heigth, byte clear_byte);
void GFX_SetChatGraphics(ChatPanel *panel, int portait_graphics_id, int chat_graphics_id);
void GFX_SetChatPosition(ChatPanel *panel, int x, int y, int portait_x, int portait_y, bool portait_inverted, int chat_x, int chat_y, bool chat_inverted);
void GFX_SetLineChatPanel(ChatPanel *panel, const char c[40], int line);
int GFX_GetSpriteWidth_px(int spr_num);
int GFX_GetSpriteHeight_px(int spr_num);
void GFX_SetSpriteUnmasked(int spr_num, bool value);
int GFX_GetSpriteScreenPosX(int spr_num);
int GFX_GetSpriteScreenPosY(int spr_num);
void GFX_SetSpriteGraphic(int spr_num, int index, int graphics_id, int offset_x, int offset_y);
bool GFX_IsSpriteAnimationEnded(int spr_num, int anim_num);
int GFX_GetSpriteGraphicsId(int spr_num, int graphic_index);
int GFX_GetSpriteAnimationFrames(int spr_num, int anim_num);
int GFX_GetSpriteGraphicsFrames(int gfx_id);
void GFX_SetSpriteFrame(int sprite_num, int anim_number, int frame);
void GFX_SetSpriteBlinkingProperties(int spr_num, bool blink, byte time, byte color);
void GFX_SetDefaultAnimation(int sprite_num, bool inverted, bool loop, byte speed);
void GFX_LoadAnimations(const char *dat_file, const char *asset_file, SpriteAnimation *anim, int total_anims);
void GFX_UpdateSpriteAnimation(int sprite_num, int anim_num, SpriteAnimation anim_data);
void GFX_SetSpriteGraphicsId(int spr_num, int graphic_index, int graphics_id);
void GFX_SetSingleFrameAnimation(int sprite_num, int frame);
int GFX_GetSpriteGraphicsWidth_px(int graphics_id);
int GFX_GetSpriteGraphicsHeight_px(int graphics_id);
void GFX_SetSpriteGfxOrder(int sprite_num, int one, int two, int three, int four, int five);
void GFX_SetSpritePriority(int sprite_num, byte priority);
bool GFX_IsSpriteOnScreen(int sprite_num);
int GFX_FindEmptySpriteSlot(void);
void GFX_HideSprite(int sprite_num);
void GFX_ShowSprite(int sprite_num);
void GFX_ShowCursorSprite(void);
void GFX_HideCursorSprite(void);
bool GFX_IsBlinkActive(int sprite_num);
void GFX_UnloadSpriteGraphic(int graphics_id);

#endif