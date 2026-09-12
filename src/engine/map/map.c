#include "../engine.h"
#include <string.h>
#include "map.h"

Map *map;

/** MAP :: Initialize map
 */
void MAP_Init(void) {

	map = MM_PushChunk(sizeof(Map), CT_ENGINE);

	// Reserve memory for background tileset
	map->layer_tiles[MAP_BACKGROUND_LAYER].buffer = MM_PushChunk(MAP_MAX_BACK_TILESET_WIDTH * MAP_MAX_BACK_TILESET_HEIGHT, CT_ENGINE);
	// Reserve memory for foreground tileset
	map->layer_tiles[MAP_FOREGROUND_LAYER].buffer = MM_PushChunk(MAP_MAX_FORE_TILESET_WIDTH * MAP_MAX_FORE_TILESET_HEIGHT, CT_ENGINE);
	// Reserve memory for mask tileset
	map->layer_tiles[MAP_MASK_LAYER].buffer = MM_PushChunk(MAP_MAX_MASK_TILESET_WIDTH * MAP_MAX_MASK_TILESET_HEIGHT, CT_ENGINE);
	// Reserve memory for map background
	map->layer[MAP_BACKGROUND_LAYER] = MM_PushChunk((MAP_MAX_WIDTH * MAP_MAX_HEIGHT) << 1, CT_ENGINE);
	// Reserve memory for map foreground
	map->layer[MAP_FOREGROUND_LAYER] = MM_PushChunk((MAP_MAX_WIDTH * MAP_MAX_HEIGHT) << 1, CT_ENGINE);
	// Reserve memory for map mask
	map->layer[MAP_MASK_LAYER] = MM_PushChunk((MAP_MAX_WIDTH * MAP_MAX_HEIGHT) << 1, CT_ENGINE);
	// Reserve memory for map colission
	map->layer[MAP_COLISSION_LAYER] = MM_PushChunk((MAP_MAX_WIDTH * MAP_MAX_HEIGHT) << 1, CT_ENGINE);
	// Reserve memory for map animation
	map->layer[MAP_ANIM_LAYER] = MM_PushChunk((MAP_MAX_WIDTH * MAP_MAX_HEIGHT) << 1, CT_ENGINE);
	// Reserve memory for map event
	map->layer[MAP_EVENT_HSPOT_LAYER] = MM_PushChunk((MAP_MAX_WIDTH * MAP_MAX_HEIGHT) << 1, CT_ENGINE);

	map->hotspot_enabling_room1 = MM_PushChunk(32, CT_ENGINE);
	map->hotspot_enabling_room2 = MM_PushChunk(32, CT_ENGINE);
	map->event_enabling_room1 = MM_PushChunk(32, CT_ENGINE);
	map->event_enabling_room2 = MM_PushChunk(32, CT_ENGINE);
}

int MAP_GetTileNumber(int x_px, int y_px) {
	return ((y_px >> 4) * map->width) + (x_px >> 4);
}

int MAP_CheckColissionTile(int x_px, int y_px) {
	return map->layer[MAP_COLISSION_LAYER][MAP_GetTileNumber(x_px, y_px)];
}

int MAP_CheckAnimationTile(int x_px, int y_px) {
	return map->layer[MAP_ANIM_LAYER][MAP_GetTileNumber(x_px, y_px)];
}

int MAP_CheckHotspotTile(int x_px, int y_px) {
	return (map->layer[MAP_EVENT_HSPOT_LAYER][MAP_GetTileNumber(x_px, y_px)]) & 0xFF;
}

int MAP_CheckEventTile(int x_px, int y_px) {
	return (map->layer[MAP_EVENT_HSPOT_LAYER][MAP_GetTileNumber(x_px, y_px)] >> 8) & 0xFF;
}

/** MAP :: Tileset load
 *  - Tileset must be a pcx indexed color file 
 *  - Max. number of colors 256
 *  - Colors from 220..255 are reserved to sprites
 *  - Tiles size must be 16x16 px
 */
void MAP_LoadTileset(const char *dat_file, const char *asset_file, Tileset *tset, int size) {
	dword i, j, k;
	dword src_index, dst_index;

	byte *data_loaded = MM_PushChunk(size, CT_TEMPORARY);

	FILE_LoadPCXTileset(dat_file, asset_file, data_loaded, size, &tset->width_px, &tset->height_px);

	tset->width = tset->width_px >> 4;
	tset->height = tset->height_px >> 4;
	tset->num_tiles = tset->width * tset->height;

	// Transfer data to tileset buffer
	src_index = 0;
	dst_index = 0;
	for (i = 0; i < tset->height; i++) {
		for (j = 0; j < tset->width; j++) {
			src_index = ((i << 4) * tset->width_px) + (j << 4);
			for (k = 0; k < MAP_TILE_HEIGHT; k++) {
				memcpy(&tset->buffer[dst_index], &data_loaded[src_index], MAP_TILE_WIDTH);
				src_index += tset->width_px;
				dst_index += 16;
			}
		}
	}

	MM_PopChunks(CT_TEMPORARY);
}

/** MAP :: Load map
 *  - Loads 5 layers of map from a dat file
 *  - Background file must be "back.csv"
 *  - Foreground or mask file must be "fore.csv"
 *  - ...
 */
void MAP_LoadMap(const char *dat_file_map, int width, int height, const char *dat_file_tileset, const char *asset_file_back, const char *asset_file_fore, const char *asset_file_mask, int back_tileset_size, int fore_tileset_size, int mask_tileset_size) {

	// Load tilesets
	MAP_LoadTileset(dat_file_tileset, asset_file_back, &map->layer_tiles[MAP_BACKGROUND_LAYER], back_tileset_size);
	MAP_LoadTileset(dat_file_tileset, asset_file_fore, &map->layer_tiles[MAP_FOREGROUND_LAYER], fore_tileset_size);
	MAP_LoadTileset(dat_file_tileset, asset_file_mask, &map->layer_tiles[MAP_MASK_LAYER], mask_tileset_size);

	// Check map size
	if (width > MAP_MAX_WIDTH || height > MAP_MAX_HEIGHT) {
		Error("Map size exceeds maximum allowed", "", "", ERROR_SYSTEM);
	}

	FILE_LoadMap_CSV(dat_file_map, map->layer[MAP_BACKGROUND_LAYER], map->layer[MAP_FOREGROUND_LAYER], map->layer[MAP_MASK_LAYER], map->layer[MAP_COLISSION_LAYER], map->layer[MAP_ANIM_LAYER], map->layer[MAP_EVENT_HSPOT_LAYER], width * height);

	map->width = width;
	map->height = height;
	map->width_px = map->width << 4;
	map->height_px = map->height << 4;

	map->loaded = true;
}

/** MAP :: Draw a solid zone on video buffer
 *  - No transparency, all pixels are printed
 *  - x_px: Left pixel of the zone to print
 *  - y_px: Top pixel of the zone to print
 *  - width_px: Width in pixels of the zone to print
 *  - height_px: Height in pixels of the zone to print
 *  - buffer: Source buffer
 */
void MAP_DrawToVideoBuffer(int x_px, int y_px, int width_px, int height_px, byte *src_buffer, byte *video_buffer) {
	register int i;
	register int src_index, dst_index;

	src_index = 0;
	dst_index = (y_px * video->map_buffer_width) + x_px;
	for (i = 0; i < height_px; i++) {
		memcpy(&video_buffer[dst_index], &src_buffer[src_index], width_px);
		src_index += width_px;
		dst_index += video->map_buffer_width;
	}
}

void MAP_ShowSwapableTile(int x_px, int y_px) {
	int tile_number, tile_screen_x, tile_screen_y, tile_x, tile_y, tile_data, tileset_index;

	tile_number = MAP_GetTileNumber(x_px, y_px);

	// enable colission
	map->layer[MAP_COLISSION_LAYER][tile_number] = MAP_COLISSION_HALF;

	// enable swapable
	map->layer[MAP_ANIM_LAYER][tile_number] = MAP_ANIMATION_SWAPABLE;

	// Redraw tile if it is on screen
	if ((x_px > camera->pos_x) && (x_px < (camera->pos_x + camera->visible_width)) && (y_px > camera->pos_y) && (y_px < (camera->pos_y + camera->visible_height))) {
		// Get map tile position on screen
		tile_screen_x = x_px - camera->pos_x;
		tile_screen_y = y_px - camera->pos_y;

		// Get map tile position on video buffer
		tile_x = camera->pos_abs_x + tile_screen_x;
		if (tile_x >= video->map_buffer_width) {
			tile_x = tile_x - video->map_buffer_width;
		}
		tile_y = camera->pos_abs_y + tile_screen_y;
		if (tile_y >= video->map_buffer_height) {
			tile_y = tile_y - video->map_buffer_height;
		}

		tile_data = map->layer[MAP_FOREGROUND_LAYER][tile_number];
		tileset_index = tile_data << 8;
		MAP_DrawToVideoBuffer(tile_x, tile_y, MAP_TILE_WIDTH, MAP_TILE_HEIGHT, &map->layer_tiles[MAP_FOREGROUND_LAYER].buffer[tileset_index], video->map_buffer[MAP_BACKGROUND_LAYER]);
	}
}

void MAP_HideSwapableTile(int x_px, int y_px) {
	int tile_number, tile_screen_x, tile_screen_y, tile_x, tile_y, tile_data, tileset_index;

	tile_number = MAP_GetTileNumber(x_px, y_px);

	// disable colission
	map->layer[MAP_COLISSION_LAYER][tile_number] = MAP_COLISSION_NONE;

	// disable animation
	map->layer[MAP_ANIM_LAYER][tile_number] = MAP_ANIMATION_NONE;

	// disable mask
	map->layer[MAP_MASK_LAYER][tile_number] = MAP_MASK_NONE;

	// Redraw tile if it is on screen
	if ((x_px > camera->pos_x) && (x_px < (camera->pos_x + camera->visible_width)) && (y_px > camera->pos_y) && (y_px < (camera->pos_y + camera->visible_height))) {

		// Get map tile position on screen
		tile_screen_x = x_px - camera->pos_x;
		tile_screen_y = y_px - camera->pos_y;
		// Get map tile position on video buffer
		tile_x = camera->pos_abs_x + tile_screen_x;
		if (tile_x >= video->map_buffer_width) {
			tile_x = tile_x - video->map_buffer_width;
		}
		tile_y = camera->pos_abs_y + tile_screen_y;
		if (tile_y >= video->map_buffer_height) {
			tile_y = tile_y - video->map_buffer_height;
		}

		// redraw tile
		tile_data = map->layer[MAP_BACKGROUND_LAYER][tile_number];
		tileset_index = tile_data << 8;
		MAP_DrawToVideoBuffer(tile_x, tile_y, MAP_TILE_WIDTH, MAP_TILE_HEIGHT, &map->layer_tiles[MAP_BACKGROUND_LAYER].buffer[tileset_index], video->map_buffer[MAP_BACKGROUND_LAYER]);

		// unmask tile
		tile_data = map->layer[MAP_MASK_LAYER][tile_number];
		tileset_index = tile_data << 8;
		MAP_DrawToVideoBuffer(tile_x, tile_y, MAP_TILE_WIDTH, MAP_TILE_HEIGHT, &map->layer_tiles[MAP_MASK_LAYER].buffer[tileset_index], video->map_buffer[MAP_MASK_LAYER]);
	}
}

/** MAP :: Clear a solid zone on video buffer
 *  - No transparency, all pixels are printed
 *  - x_px: Left pixel of the zone to print
 *  - y_px: Top pixel of the zone to print
 *  - width_px: Width in pixels of the zone to print
 *  - height_px: Height in pixels of the zone to print
 *  - buffer: Source buffer
 */
void MAP_ClearVideoBuffer(int x_px, int y_px, int width_px, int height_px, byte *video_buffer) {
	register int i;
	register int dst_index;

	dst_index = (y_px * video->map_buffer_width) + x_px;
	for (i = 0; i < height_px; i++) {
		memset(&video_buffer[dst_index], 0, width_px);
		dst_index += video->map_buffer_width;
	}
}

/** MAP :: Draw a transparent zone on video buffer
 *  - Transparency active, transparent pixel will be skipped
 *  - x_px: Left pixel of the zone to print
 *  - y_px: Top pixel of the zone to print
 *  - width_px: Width in pixels of the zone to print
 *  - height_px: Height in pixels of the zone to print
 *  - buffer: Source buffer
 */
void MAP_DrawToVideoBuffer_WithTransparency(int x_px, int y_px, int width_px, int height_px, byte *src_buffer, byte *video_buffer) {
	register int i, j;
	register int dst_index;
	byte *src, *dst;
	byte value;

	dst_index = (y_px * video->map_buffer_width) + x_px;

	src = src_buffer;
	dst = &video_buffer[dst_index];

	for (i = 0; i < height_px; i++) {
		for (j = 0; j < width_px; j++) {
			value = *src++;
			if (value) *dst++ = value;
			else
				dst++;
		}
		dst += (video->map_buffer_width - 16);
	}
}

void MAP_DrawToScreenBuffer(int x_px, int y_px, int width_px, int height_px, byte *src_buffer, byte *screen_buffer) {
	register int i;
	register int src_index, dst_index;

	src_index = 0;
	dst_index = (y_px * video->screen_width) + x_px;
	for (i = 0; i < height_px; i++) {
		memcpy(&screen_buffer[dst_index], &src_buffer[src_index], width_px);
		src_index += width_px;
		dst_index += video->screen_width;
	}
}

void MAP_DrawToScreenBuffer_WithTransparency(int x_px, int y_px, int width_px, int height_px, byte *src_buffer, byte *screen_buffer) {
	register int i, j;
	register int dst_index;
	byte *src, *dst;
	byte value;

	dst_index = (y_px * video->screen_width) + x_px;

	src = src_buffer;
	dst = &screen_buffer[dst_index];

	for (i = 0; i < height_px; i++) {
		for (j = 0; j < width_px; j++) {
			value = *src++;
			if (value) *dst++ = value;
			else
				dst++;
		}
		dst += (video->screen_width - 16);
	}
}

/** MAP :: Draw background zone
 *  - x_px: Left pixel of the zone to print
 *  - y_px: Top pixel of the zone to print
 *  - width_px: Width in pixels of the zone to print
 *  - height_px: Height in pixels of the zone to print
 *  - buffer: Source buffer
 */
void MAP_DrawBackground(int x_px, int y_px, int width_px, int height_px, byte *buffer) {
	register int i;
	register int src_index, dst_index;

	src_index = 0;
	dst_index = (y_px * video->map_buffer_width) + x_px;
	for (i = 0; i < height_px; i++) {
		memcpy(&video->map_buffer[MAP_BACKGROUND_LAYER][dst_index], &buffer[src_index], width_px);
		src_index += width_px;
		dst_index += video->map_buffer_width;
	}
}

/** MAP:: Draw mask zone in pixels
 *  - x_px: Left pixel of the zone to print
 *  - y_px: Top pixel of the zone to print
 *  - width_px: Width in pixels of the zone to print
 *  - height_px: Height in pixels of the zone to print
 *  - buffer: Source buffer
 */
void MAP_DrawMask(int x_px, int y_px, int width_px, int height_px, byte *buffer) {
	register int i;
	register int src_index, dst_index;

	src_index = 0;
	dst_index = (y_px * video->map_buffer_width) + x_px;
	for (i = 0; i < height_px; i++) {
		memcpy(&video->map_buffer[MAP_MASK_LAYER][dst_index], &buffer[src_index], width_px);
		src_index += width_px;
		dst_index += video->map_buffer_width;
	}
}

/** MAP :: Draw background tile
 *  - Tile dimensions 16x16 px
 *  - x: Tile x position on video buffer
 *  - y: Tile y position on video buffer
 */
void MAP_DrawBackgroundTile(int x, int y, int num_tile) {
	register int src_index;
	src_index = (num_tile << 8);// num tile * 256 to get pixel offset
	MAP_DrawBackground(x << 4, y << 4, MAP_TILE_WIDTH, MAP_TILE_HEIGHT, &map->layer_tiles[MAP_BACKGROUND_LAYER].buffer[src_index]);
}

/** MAP :: Draw foreground tile
 *  - Tile dimensions 16x16 px
 *  - x: Tile x position on video buffer
 *  - y: Tile y position on video buffer
 */
void MAP_DrawForegroundTile(int x, int y, int num_tile) {
	register int tile_data, tileset_index;

	tile_data = map->layer[MAP_FOREGROUND_LAYER][num_tile];
	tileset_index = tile_data << 8;

	MAP_DrawToVideoBuffer(x << MAP_TILE_WIDTH_BASE2, y << MAP_TILE_HEIGHT_BASE2, MAP_TILE_WIDTH, MAP_TILE_HEIGHT, &map->layer_tiles[MAP_FOREGROUND_LAYER].buffer[tileset_index], video->map_buffer[MAP_BACKGROUND_LAYER]);
}

/** MAP :: Draw mask tile
 *  - Tile dimensions 16x16 px
 *  - x: Tile x position on video buffer
 *  - y: Tile y position on video buffer
 */
void MAP_DrawMaskTile(int x, int y, int num_tile) {
	register int src_index;
	if (num_tile == 255) { num_tile = 0; }
	src_index = (num_tile << 8);// num tile * 256 to get pixel offset
	MAP_DrawMask(x << 4, y << 4, MAP_TILE_WIDTH, MAP_TILE_HEIGHT, &map->layer_tiles[MAP_MASK_LAYER].buffer[src_index]);
}

/** MAP :: Draw map column
 *  - x: Left map position
 *  - y: Top map position
 *  - tile_x: Left map tile number
 *  - tile_y: Top map tile number
 */
void MAP_DrawMapColumn(int logical_pos_x, int logical_pos_y, int tile_x, int tile_y) {
	register int i;
	register int tile_number, tile_data, tileset_index;
	register int logical_x, logical_y;

	if (map->loaded) {

		logical_x = logical_pos_x;
		logical_y = logical_pos_y;

		for (i = 0; i < (camera->visible_height >> MAP_TILE_HEIGHT_BASE2) + 1; i++) {
			tile_number = ((tile_y + i) * map->width) + tile_x;

			switch (map->layer[MAP_ANIM_LAYER][tile_number]) {
				case MAP_ANIMATION_NONE:// Nothing. Draw background tile
					tile_data = map->layer[MAP_BACKGROUND_LAYER][tile_number];
					tileset_index = tile_data << 8;
					MAP_DrawToVideoBuffer(logical_x << MAP_TILE_WIDTH_BASE2, logical_y << MAP_TILE_HEIGHT_BASE2, MAP_TILE_WIDTH, MAP_TILE_HEIGHT, &map->layer_tiles[MAP_BACKGROUND_LAYER].buffer[tileset_index], video->map_buffer[MAP_BACKGROUND_LAYER]);
					break;
				case MAP_ANIMATION_BREAKABLE:         // Breakable. Draw foreground tile
				case MAP_ANIMATION_BREAKABLE_ELECTRIC:// Breakable with electric effect. Draw foreground tile
				case MAP_ANIMATION_BREAKABLE_FIRE:    // Breakable with fire effect. Draw foreground tile
				case MAP_ANIMATION_BREAKABLE_WATER:   // Breakable with water effect. Draw foreground tile
					tile_data = map->layer[MAP_FOREGROUND_LAYER][tile_number];
					tileset_index = tile_data << 8;
					MAP_DrawToVideoBuffer(logical_x << MAP_TILE_WIDTH_BASE2, logical_y << MAP_TILE_HEIGHT_BASE2, MAP_TILE_WIDTH, MAP_TILE_HEIGHT, &map->layer_tiles[MAP_FOREGROUND_LAYER].buffer[tileset_index], video->map_buffer[MAP_BACKGROUND_LAYER]);
					break;
				case MAP_ANIMATION_SWAPABLE:// Swapable. Draw foreground tile
					tile_data = map->layer[MAP_FOREGROUND_LAYER][tile_number];
					tileset_index = tile_data << 8;
					MAP_DrawToVideoBuffer(logical_x << MAP_TILE_WIDTH_BASE2, logical_y << MAP_TILE_HEIGHT_BASE2, MAP_TILE_WIDTH, MAP_TILE_HEIGHT, &map->layer_tiles[MAP_FOREGROUND_LAYER].buffer[tileset_index], video->map_buffer[MAP_BACKGROUND_LAYER]);
					break;
				case MAP_ANIMATION_ANIMATED:// Animated. Draw foreground or background tile
					if (map->animation_frame) {
						tile_data = map->layer[MAP_FOREGROUND_LAYER][tile_number];
						tileset_index = tile_data << 8;
						MAP_DrawToVideoBuffer(logical_x << MAP_TILE_WIDTH_BASE2, logical_y << MAP_TILE_HEIGHT_BASE2, MAP_TILE_WIDTH, MAP_TILE_HEIGHT, &map->layer_tiles[MAP_FOREGROUND_LAYER].buffer[tileset_index], video->map_buffer[MAP_BACKGROUND_LAYER]);
					} else {
						tile_data = map->layer[MAP_BACKGROUND_LAYER][tile_number];
						tileset_index = tile_data << 8;
						MAP_DrawToVideoBuffer(logical_x << MAP_TILE_WIDTH_BASE2, logical_y << MAP_TILE_HEIGHT_BASE2, MAP_TILE_WIDTH, MAP_TILE_HEIGHT, &map->layer_tiles[MAP_BACKGROUND_LAYER].buffer[tileset_index], video->map_buffer[MAP_BACKGROUND_LAYER]);
					}
					break;
				default:
					break;
			}

			// Mask layer
			tile_data = map->layer[MAP_MASK_LAYER][tile_number];
			tileset_index = tile_data << 8;
			MAP_DrawToVideoBuffer(logical_x << MAP_TILE_WIDTH_BASE2, logical_y << MAP_TILE_HEIGHT_BASE2, MAP_TILE_WIDTH, MAP_TILE_HEIGHT, &map->layer_tiles[MAP_MASK_LAYER].buffer[tileset_index], video->map_buffer[MAP_MASK_LAYER]);

			logical_y++;
			if (logical_y >= (video->map_buffer_height >> 4)) { logical_y = 0; }
		}
	} else {
		VGA_SetMode(VIDEO_TEXT_MODE);
		printf("Map not loaded \n");
		sleep(2);
		Error("Map not loaded \n", "", "", ERROR_SYSTEM);
	}
}

/** MAP :: Draw map column
 *  - x: Left map position
 *  - y: Top map position
 *  - tile_x: Left map tile number
 *  - tile_y: Top map tile number
 */
void MAP_DrawAnimatedTilesColumn(int logical_pos_x, int logical_pos_y, int tile_x, int tile_y) {
	register int i;
	register int tile_number, tile_data, tileset_index;
	register int logical_x, logical_y;

	logical_x = logical_pos_x;
	logical_y = logical_pos_y;

	for (i = 0; i < (camera->visible_height >> MAP_TILE_HEIGHT_BASE2) - 1; i++) {
		tile_number = ((tile_y + i) * map->width) + tile_x;

		switch (map->layer[MAP_ANIM_LAYER][tile_number]) {
			case MAP_ANIMATION_ANIMATED:// Animated. Draw foreground or background tile
				if (map->animation_frame) {
					tile_data = map->layer[MAP_FOREGROUND_LAYER][tile_number];
					tileset_index = tile_data << 8;
					MAP_DrawToVideoBuffer(logical_x << MAP_TILE_WIDTH_BASE2, logical_y << MAP_TILE_HEIGHT_BASE2, MAP_TILE_WIDTH, MAP_TILE_HEIGHT, &map->layer_tiles[MAP_FOREGROUND_LAYER].buffer[tileset_index], video->map_buffer[MAP_BACKGROUND_LAYER]);
				} else {
					tile_data = map->layer[MAP_BACKGROUND_LAYER][tile_number];
					tileset_index = tile_data << 8;
					MAP_DrawToVideoBuffer(logical_x << MAP_TILE_WIDTH_BASE2, logical_y << MAP_TILE_HEIGHT_BASE2, MAP_TILE_WIDTH, MAP_TILE_HEIGHT, &map->layer_tiles[MAP_BACKGROUND_LAYER].buffer[tileset_index], video->map_buffer[MAP_BACKGROUND_LAYER]);
				}
				break;
			default:
				break;
		}

		logical_y++;
		if (logical_y >= (video->map_buffer_height >> 4)) { logical_y = 0; }
	}
}

/** MAP :: Draw map row
 *  - x: Left map position
 *  - y: Top map position
 *  - tile_x: Left map tile number
 *  - tile_y: Top map tile number
  */
void MAP_DrawMapRow(int logical_pos_x, int logical_pos_y, int tile_x, int tile_y) {

	register int i;
	register int tile_number, tile_data, tileset_index;
	register int logical_x, logical_y;

	if (map->loaded) {

		logical_x = logical_pos_x;
		logical_y = logical_pos_y;

		for (i = 0; i < (camera->visible_width >> 4) + 1; i++) {
			tile_number = (tile_y * map->width) + tile_x + i;

			switch (map->layer[MAP_ANIM_LAYER][tile_number]) {
				case MAP_ANIMATION_NONE:// Nothing
					tile_data = map->layer[MAP_BACKGROUND_LAYER][tile_number];
					tileset_index = tile_data << 8;
					MAP_DrawToVideoBuffer(logical_x << MAP_TILE_WIDTH_BASE2, logical_y << MAP_TILE_HEIGHT_BASE2, MAP_TILE_WIDTH, MAP_TILE_HEIGHT, &map->layer_tiles[MAP_BACKGROUND_LAYER].buffer[tileset_index], video->map_buffer[MAP_BACKGROUND_LAYER]);
					break;
				case MAP_ANIMATION_BREAKABLE:         // Breakable. Draw foreground tile
				case MAP_ANIMATION_BREAKABLE_ELECTRIC:// Breakable with electric effect. Draw foreground tile
				case MAP_ANIMATION_BREAKABLE_FIRE:    // Breakable with fire effect. Draw foreground tile
				case MAP_ANIMATION_BREAKABLE_WATER:   // Breakable with water effect. Draw foreground tile
					tile_data = map->layer[MAP_FOREGROUND_LAYER][tile_number];
					tileset_index = tile_data << 8;
					MAP_DrawToVideoBuffer(logical_x << MAP_TILE_WIDTH_BASE2, logical_y << MAP_TILE_HEIGHT_BASE2, MAP_TILE_WIDTH, MAP_TILE_HEIGHT, &map->layer_tiles[MAP_FOREGROUND_LAYER].buffer[tileset_index], video->map_buffer[MAP_BACKGROUND_LAYER]);
					break;
				case MAP_ANIMATION_SWAPABLE:// Swapable. Draw foreground tile
					tile_data = map->layer[MAP_FOREGROUND_LAYER][tile_number];
					tileset_index = tile_data << 8;
					MAP_DrawToVideoBuffer(logical_x << MAP_TILE_WIDTH_BASE2, logical_y << MAP_TILE_HEIGHT_BASE2, MAP_TILE_WIDTH, MAP_TILE_HEIGHT, &map->layer_tiles[MAP_FOREGROUND_LAYER].buffer[tileset_index], video->map_buffer[MAP_BACKGROUND_LAYER]);
					break;
				case MAP_ANIMATION_ANIMATED:// Animated. Draw foreground or background tile
					if (map->animation_frame) {
						tile_data = map->layer[MAP_FOREGROUND_LAYER][tile_number];
						tileset_index = tile_data << 8;
						MAP_DrawToVideoBuffer(logical_x << MAP_TILE_WIDTH_BASE2, logical_y << MAP_TILE_HEIGHT_BASE2, MAP_TILE_WIDTH, MAP_TILE_HEIGHT, &map->layer_tiles[MAP_FOREGROUND_LAYER].buffer[tileset_index], video->map_buffer[MAP_BACKGROUND_LAYER]);
					} else {
						tile_data = map->layer[MAP_BACKGROUND_LAYER][tile_number];
						tileset_index = tile_data << 8;
						MAP_DrawToVideoBuffer(logical_x << MAP_TILE_WIDTH_BASE2, logical_y << MAP_TILE_HEIGHT_BASE2, MAP_TILE_WIDTH, MAP_TILE_HEIGHT, &map->layer_tiles[MAP_BACKGROUND_LAYER].buffer[tileset_index], video->map_buffer[MAP_BACKGROUND_LAYER]);
					}
					break;
				default:
					break;
			}

			// Mask layer
			tile_data = map->layer[MAP_MASK_LAYER][tile_number];
			tileset_index = tile_data << 8;
			MAP_DrawToVideoBuffer(logical_x << MAP_TILE_WIDTH_BASE2, logical_y << MAP_TILE_HEIGHT_BASE2, MAP_TILE_WIDTH, MAP_TILE_HEIGHT, &map->layer_tiles[MAP_MASK_LAYER].buffer[tileset_index], video->map_buffer[MAP_MASK_LAYER]);

			logical_x++;
			if (logical_x >= (video->map_buffer_width >> 4)) { logical_x = 0; }
		}
	} else {
		VGA_SetMode(VIDEO_TEXT_MODE);
		printf("Map not loaded \n");
		sleep(2);
		Error("Map not loaded \n", "", "", ERROR_SYSTEM);
	}
}

/** MAP :: Draw map
 *  - x: Left map logical position
 *  - y: Top map logical position
 */
void MAP_DrawMapToMapVideoBuffer(void) {
	register int i;
	register int pos_x, pos_y;
	register int tile_x, tile_y;

	if (map->loaded) {
		// Draw column by column
		for (i = 0; i < (video->map_buffer_width >> MAP_TILE_WIDTH_BASE2); i++) {

			pos_x = camera->pos_abs_x + (i << MAP_TILE_WIDTH_BASE2);
			if (pos_x >= video->map_buffer_width) { pos_x = pos_x - video->map_buffer_width; }
			pos_y = camera->pos_abs_y;

			tile_x = (camera->pos_x >> MAP_TILE_WIDTH_BASE2) + i;
			tile_y = (camera->pos_y >> MAP_TILE_HEIGHT_BASE2);
			MAP_DrawMapColumn(pos_x >> MAP_TILE_WIDTH_BASE2, pos_y >> MAP_TILE_HEIGHT_BASE2, tile_x, tile_y);
		}
	}
}

void MAP_DrawAnimatedTilesToMapVideoBuffer(void) {
	register int i;
	register int pos_x, pos_y;
	register int tile_x, tile_y;

	// Draw column by column
	for (i = 1; i < (video->map_buffer_width >> MAP_TILE_WIDTH_BASE2) - 1; i++) {

		pos_x = camera->pos_abs_x + (i << MAP_TILE_WIDTH_BASE2);
		if (pos_x >= video->map_buffer_width) { pos_x = pos_x - video->map_buffer_width; }
		pos_y = camera->pos_abs_y;

		tile_x = (camera->pos_x >> MAP_TILE_WIDTH_BASE2) + i;
		tile_y = (camera->pos_y >> MAP_TILE_HEIGHT_BASE2);

		MAP_DrawAnimatedTilesColumn(pos_x >> MAP_TILE_WIDTH_BASE2, pos_y >> MAP_TILE_HEIGHT_BASE2, tile_x, tile_y);
	}
}

void MAP_Update(void) {
	int tile_x, tile_y;
	int pos_x, pos_y;

	if (map->loaded) {
		map->animation_counter++;
		if (map->animation_counter >= MAP_ANIMATION_SPEED) {
			map->animation_counter = 0;
			// Swap animation tiles
			map->animation_frame = !map->animation_frame;
			MAP_DrawAnimatedTilesToMapVideoBuffer();
		}

		//MAP_DrawAnimatedTilesToMapVideoBuffer2();


		// Displacement right
		if ((map->update_required) && (camera->old_pos_x < camera->pos_x)) {
			//if (camera->old_pos_x < camera->pos_x) {
			pos_x = camera->pos_abs_x + camera->visible_width;
			if (pos_x >= video->map_buffer_width) { pos_x = pos_x - video->map_buffer_width; }
			pos_y = camera->pos_abs_y;
			tile_x = (camera->pos_x >> 4) + (camera->visible_width >> 4);
			tile_y = (camera->pos_y >> 4);
			MAP_DrawMapColumn(pos_x >> 4, pos_y >> 4, tile_x, tile_y);
		}

		// Displacement left
		if ((map->update_required) && (camera->pos_x < camera->old_pos_x)) {
			//if (camera->pos_x < camera->old_pos_x) {
			pos_x = camera->pos_abs_x;
			if (pos_x < 0) { pos_x = pos_x + video->map_buffer_width; }
			pos_y = camera->pos_abs_y;
			tile_x = (camera->pos_x >> 4);
			tile_y = (camera->pos_y >> 4);
			MAP_DrawMapColumn(pos_x >> 4, pos_y >> 4, tile_x, tile_y);
		}

		// Displacement down
		if ((map->update_required) && (camera->old_pos_y < camera->pos_y)) {
			//if (camera->old_pos_y < camera->pos_y) {
			pos_y = camera->pos_abs_y + camera->visible_height;
			if (pos_y >= video->map_buffer_height) { pos_y = pos_y - video->map_buffer_height; }
			pos_x = camera->pos_abs_x;
			tile_x = (camera->pos_x >> 4);
			tile_y = (camera->pos_y >> 4) + (camera->visible_height >> 4);
			MAP_DrawMapRow(pos_x >> 4, pos_y >> 4, tile_x, tile_y);
		}

		// Displacement up
		if ((map->update_required) && (camera->pos_y < camera->old_pos_y)) {
			//if (camera->pos_y < camera->old_pos_y) {
			pos_y = camera->pos_abs_y;
			if (pos_y < 0) { pos_y = pos_y + video->map_buffer_height; }
			pos_x = camera->pos_abs_x;
			tile_x = (camera->pos_x >> 4);
			tile_y = (camera->pos_y >> 4);
			MAP_DrawMapRow(pos_x >> 4, pos_y >> 4, tile_x, tile_y);
		}

		map->update_required = false;

		VIDEO_MapBufferToScreenBuffer();// Fills a second buffer with the area shown
	}
}

void MAP_UnloadMap(void) {
	map->width = 0;
	map->height = 0;
	map->width_px = 0;
	map->height_px = 0;

	memset(map->layer_tiles[MAP_BACKGROUND_LAYER].buffer, 0, MAP_MAX_BACK_TILESET_WIDTH * MAP_MAX_BACK_TILESET_HEIGHT);
	memset(map->layer_tiles[MAP_FOREGROUND_LAYER].buffer, 0, MAP_MAX_BACK_TILESET_WIDTH * MAP_MAX_BACK_TILESET_HEIGHT);
	memset(map->layer_tiles[MAP_MASK_LAYER].buffer, 0, MAP_MAX_BACK_TILESET_WIDTH * MAP_MAX_BACK_TILESET_HEIGHT);
	memset(map->layer[MAP_BACKGROUND_LAYER], 0, MAP_MAX_WIDTH * MAP_MAX_HEIGHT);
	memset(map->layer[MAP_FOREGROUND_LAYER], 0, MAP_MAX_WIDTH * MAP_MAX_HEIGHT);
	memset(map->layer[MAP_MASK_LAYER], 0, MAP_MAX_WIDTH * MAP_MAX_HEIGHT);
	memset(map->layer[MAP_COLISSION_LAYER], 0, MAP_MAX_WIDTH * MAP_MAX_HEIGHT);
	memset(map->layer[MAP_EVENT_HSPOT_LAYER], 0, MAP_MAX_WIDTH * MAP_MAX_HEIGHT);

	map->loaded = false;
}
