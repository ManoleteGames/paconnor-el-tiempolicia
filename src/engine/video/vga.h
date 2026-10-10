#ifndef VGA_H
#define VGA_H

#include "../types/types.h"

#define VGA_VIDEO_INT 0x10// Video interrupt number
#define VGA_TEXT_MODE 0x03// use to set back to 80x25 text mode.
#define VGA_GRAPHIC_MODE 0x13
#define VGA_ADC_PORT 0x3DA// Attribute controller data port

#define VGA_PALETTE_INDEX_RD 0x03c7
#define VGA_PALETTE_INDEX_WR 0x03c8
#define VGA_PALETTE_DATA 0x03c9

#define VGA_RESOLUTION_WIDTH 320
#define VGA_RESOLUTION_HEIGHT 200//208

#define VGA_TRANSPARENT_COLOR 220

/// VGA.C //
extern byte *VGA;
void VGA_HideCursor(void);
bool VGA_CheckGraphicsCard(void);
void VGA_SetPalette(byte *palette);
void VGA_SetPaletteRange(byte *palette, int start, int end);
void VGA_ClearPalete(void);
void VGA_SetMode(int mode);
void VGA_VSync(void);
void VGA_ScreenMainBufferToVRAM(void);
void VGA_ImageToVRAM(byte *buffer, int width, int height);
void VGA_ClearScreen(void);
void VGA_ClearPalette(void);
void VGA_FadeIn(int speed);
void VGA_FadeOut(int speed);
bool VGA_FadeIn_Async(int speed, int *step);
bool VGA_FadeOut_Async(int speed, int *step);
void VGA_FadeOutToWhite(int speed);
void VGA_RotatePaletteAsync(int index1, int index2);
void VGA_RotatePalette(int index1, int index2, int speed);

#endif