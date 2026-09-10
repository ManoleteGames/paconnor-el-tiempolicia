#ifndef VIDEO_H
#define VIDEO_H

#include "../types/types.h"

#define VIDEO_MODE_VGA 0
#define VIDEO_MODE_EGA 1
#define VIDEO_MODE_CGA 2

#define VIDEO_TEXT_MODE 0x03
#define VIDEO_GRAPHICS_MODE 0x13

#define VIDEO_MAP_BUFFER_WIDTH 336
#define VIDEO_MAP_BUFFER_HEIGHT 224

#define VIDEO_TRANSPARENT_COLOR 201

#define VIDEO_PANEL_WIDTH 80
#define VIDEO_PANEL_HEIGHT 32
#define VIDEO_PANEL_POS_X 8
#define VIDEO_PANEL_POS_Y 8

#define VIDEO_SCREEN_BUFFER_BACK 0
#define VIDEO_SCREEN_BUFFER_FORE 1
#define VIDEO_SCREEN_BUFFER_MASK 2

extern Video *video;
void VIDEO_Init(void);
extern void (*VIDEO_VSync)(void);
extern void (*VIDEO_ScreenBufferToVRAM)(void);
void VIDEO_SetActiveBuffer(int bufferNumber);
void VIDEO_CharToScreenBuffer(int x, int y, word width, word height, byte transparent_color, byte *data);
void VIDEO_StringToScreenBuffer(int x, int y, char *string, int font_number);
bool VIDEO_StringToScreenBufferSteps(int x, int y, char *string, int font_number, int *char_index);
bool VIDEO_StringToScreenBufferRand(int x, int y, char *string, int font_number, int *char_index, int *counter);
void VIDEO_DrawSpriteToScreenBufferRLE(Graphic *spr_graphics, SpriteGfx *spr);
void VIDEO_MapBufferToScreenBuffer(void);
void VIDEO_DrawLifeBarOnScreenBuffer(int x, int y, int length, int life);
void VIDEO_PanelToScreenBuffer(StatusPanel *panel);
void VIDEO_DrawLifeBarOnScreenPanel(int x, int y, int length, int life, StatusPanel panel);
void VIDEO_BinaryImageToVRAM(const char *dat_name, const char *asset_name);
void VIDEO_PCXImageToVRAM(const char *dat_name, const char *asset_name, int size);
void VIDEO_PCXImageToScreenBuffer(const char *dat_name, const char *asset_name, int size);
void VIDEO_SetPalette(byte *palette);
void VIDEO_ClearScreen(void);
void VIDEO_ClearScreenBuffer(void);
void VIDEO_ClearPalette(void);
void VIDEO_FadeIn_Async(int speed);
bool VIDEO_AwaitFadedIn(void);
void VIDEO_FadeOut_Async(int speed);
bool VIDEO_AwaitFadedOut(void);
void VIDEO_FadeIn(int speed);
void VIDEO_FadeOut(int speed);
void VIDEO_FadeOutToWhite(int speed);
void VIDEO_DrawPixelOnScreenBuffer(int x, int y, byte color);
void VIDEO_FadeOutPause(int atenuation);
void VIDEO_TimerHandler(void);
void VIDEO_RotatePaletteStart_Async(int first_index, int last_index, int speed);
void VIDEO_RotatePaletteEnd_Async(void);
void VIDEO_BufferToScreenBuffer(byte *buffer, word buffer_width, word buffer_height, int window_width, int window_height, int screen_pos_x, int screen_pos_y);
bool VIDEO_ChatToScreenBuffer(ChatPanel *panel, bool finish_cmd);
void VIDEO_DrawSpriteInterlacedToScreenBufferRLE(Graphic *spr_graphics, SpriteGfx *spr, int odd);
void VIDEO_GraphicsBufferToScreenBuffer(byte *gfx_buffer, int gfx_width_px, int gfx_height_px, int screen_x, int screen_y, Camera cam);
void VIDEO_DrawSquareToScreenBuffer(byte *buffer, word buffer_width, word buffer_height, int square_width, int square_height, int screen_pos_x, int screen_pos_y, byte color);
bool VIDEO_StringToScreenBufferChat(int x, int y, char *string, int font_number, int *char_index);

#endif