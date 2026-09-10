#ifndef MAP_H
#define MAP_H

#include "../types/types.h"


#define MAP_MAX_WIDTH 120
#define MAP_MAX_HEIGHT 120

#define MAP_TILE_WIDTH 16
#define MAP_TILE_WIDTH_BASE2 4
#define MAP_TILE_HEIGHT 16
#define MAP_TILE_HEIGHT_BASE2 4

#define MAP_MAX_BACK_TILESET_WIDTH 320
#define MAP_MAX_BACK_TILESET_HEIGHT 460
#define MAP_MAX_FORE_TILESET_WIDTH 320
#define MAP_MAX_FORE_TILESET_HEIGHT 460
#define MAP_MAX_MASK_TILESET_WIDTH 128
#define MAP_MAX_MASK_TILESET_HEIGHT 128

#define MAP_BACKGROUND_LAYER 0
#define MAP_FOREGROUND_LAYER 1
#define MAP_MASK_LAYER 2
#define MAP_COLISSION_LAYER 3
#define MAP_ANIM_LAYER 4
#define MAP_EVENT_HSPOT_LAYER 5

#define MAP_COLISSION_NONE 0
#define MAP_COLISSION_FULL 1
#define MAP_COLISSION_HALF 2

#define MAP_ANIMATION_NONE 0
#define MAP_ANIMATION_BREAKABLE 1
#define MAP_ANIMATION_SWAPABLE 2
#define MAP_ANIMATION_ANIMATED 3
#define MAP_ANIMATION_BREAKABLE_ELECTRIC 4
#define MAP_ANIMATION_BREAKABLE_FIRE 5
#define MAP_ANIMATION_BREAKABLE_WATER 6

#define MAP_MASK_NONE 0

#define MAP_ANIMATION_SPEED 15

/// MAP.C ///
extern Map *map;
void MAP_Init(void);
void MAP_LoadTileset(const char *filename, const char *subfile, Tileset *tset, int size);
void MAP_LoadMap(const char *dat_file_map, int width, int height, const char *dat_file_tileset, const char *asset_file_back, const char *asset_file_fore, const char *asset_file_mask, int back_tileset_size, int fore_tileset_size, int mask_tileset_size);
void MAP_DrawMapToMapVideoBuffer(void);
void MAP_Update(void);
void MAP_Update_Paralax(void);
int MAP_GetTileNumber(int x_px, int y_px);
int MAP_CheckColissionTile(int x_px, int y_px);
int MAP_CheckAnimationTile(int x_px, int y_px);
void MAP_DrawForegroundTile(int x, int y, int num_tile);
void MAP_DrawToVideoBuffer(int x_px, int y_px, int width_px, int height_px, byte *src_buffer, byte *video_buffer);
int MAP_CheckHotspotTile(int x_px, int y_px);
int MAP_CheckEventTile(int x_px, int y_px);
void MAP_ShowSwapableTile(int x_px, int y_px);
void MAP_HideSwapableTile(int x_px, int y_px);
void MAP_UnloadMap(void);

#endif
