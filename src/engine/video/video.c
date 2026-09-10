#include "video.h"
#include "vga.h"
#include "cga.h"
#include "ega.h"
#include "../engine.h"
#include <string.h>

Video *video;

/** VIDEO :: Initialize video buffers
 *  - Set video variables
 *  - Allocate video memory
 */
void VIDEO_Init(void) {

	video = MM_PushChunk(sizeof(Video), CT_ENGINE);

	switch (settings.video_mode) {
		case VIDEO_MODE_VGA:
			if (!engine.VGA_present) {
				sprintf(engine.system_error_message1, "VIDEO_Init function error");
				sprintf(engine.system_error_message2, "VGA video card configured is missing");
				sprintf(engine.system_error_message3, "Please run setup.exe to identify you video card");
				Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_VIDEO);
			}
			// Screen buffers. Must be the same size as the VGA resolution
			video->screen_width = VGA_RESOLUTION_WIDTH;
			video->screen_height = VGA_RESOLUTION_HEIGHT;
			VIDEO_ScreenBufferToVRAM = VGA_ScreenMainBufferToVRAM;
			VIDEO_VSync = VGA_VSync;
			break;
		case VIDEO_MODE_EGA:
			if (!engine.EGA_present) {
				sprintf(engine.system_error_message1, "VIDEO_Init function error");
				sprintf(engine.system_error_message2, "EGA video card configured is missing");
				sprintf(engine.system_error_message3, "Please run setup.exe to identify you video card");
				Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_VIDEO);
			}
			// Screen buffers. Must be the same size as the EGA resolution
			video->screen_width = EGA_RESOLUTION_WIDTH;
			video->screen_height = EGA_RESOLUTION_HEIGHT;
			VIDEO_ScreenBufferToVRAM = EGA_ScreenMainBufferToVRAM;
			VIDEO_VSync = EGA_VSync;
			break;
		case VIDEO_MODE_CGA:
			if (!engine.CGA_present) {
				sprintf(engine.system_error_message1, "VIDEO_Init function error");
				sprintf(engine.system_error_message2, "CGA video card configured is missing");
				sprintf(engine.system_error_message3, "Please run setup.exe to identify you video card");
				Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_VIDEO);
			}
			// Screen buffers. Must be the same size as the CGA resolution
			video->screen_width = CGA_RESOLUTION_WIDTH;
			video->screen_height = CGA_RESOLUTION_HEIGHT;
			VIDEO_ScreenBufferToVRAM = CGA_ScreenMainBufferToVRAM;
			VIDEO_VSync = CGA_VSync;
			break;
		default:
			sprintf(engine.system_error_message1, "VIDEO_Init function error");
			sprintf(engine.system_error_message2, "Unknown video mode selected");
			sprintf(engine.system_error_message3, " ");
			Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
			break;
	}

	// Reserve RAM memory for screen buffers
	video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK] = MM_PushChunk(video->screen_width * video->screen_height, CT_ENGINE);
	video->screen_buffer[VIDEO_SCREEN_BUFFER_FORE] = MM_PushChunk(video->screen_width * video->screen_height, CT_ENGINE);
	video->screen_buffer[VIDEO_SCREEN_BUFFER_MASK] = MM_PushChunk(video->screen_width * video->screen_height, CT_ENGINE);

	// Map buffers
	video->map_buffer_width = VIDEO_MAP_BUFFER_WIDTH;
	video->map_buffer_height = VIDEO_MAP_BUFFER_HEIGHT;
	// Reserve RAM memory for map buffers
	video->map_buffer[MAP_BACKGROUND_LAYER] = MM_PushChunk(VIDEO_MAP_BUFFER_WIDTH * VIDEO_MAP_BUFFER_HEIGHT, CT_ENGINE);
	video->map_buffer[MAP_FOREGROUND_LAYER] = MM_PushChunk(VIDEO_MAP_BUFFER_WIDTH * VIDEO_MAP_BUFFER_HEIGHT, CT_ENGINE);
	video->map_buffer[MAP_MASK_LAYER] = MM_PushChunk(VIDEO_MAP_BUFFER_WIDTH * VIDEO_MAP_BUFFER_HEIGHT, CT_ENGINE);

	engine.video_initialized = true;
}

/** VIDEO :: Video timer callback
 */
void VIDEO_TimerHandler(void) {
	// Depending on the video mode, call the correct function
	switch (settings.video_mode) {
		case VIDEO_MODE_VGA:
			// Asyncronous fade in activated
			if (video->fading_in_async) {
				if (VGA_FadeIn_Async(video->fading_speed, &video->fading_step)) {
					video->faded_in = true;
					video->fading_in_async = false;
				}
			}

			if (video->fading_out_async) {
				if (VGA_FadeOut_Async(video->fading_speed, &video->fading_step)) {
					video->faded_out = true;
					video->fading_out_async = false;
				}
			}

			if (video->rotate_palette_async) {
				VGA_RotatePaletteAsync(video->rotate_first_index, video->rotate_last_index);
			}

			break;
		case VIDEO_MODE_EGA:

			break;
		case VIDEO_MODE_CGA:

			break;
		default:
			sprintf(engine.system_error_message1, "VIDEO_TimerHandler function error");
			sprintf(engine.system_error_message2, "Unknown video mode selected");
			sprintf(engine.system_error_message3, " ");
			Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_VIDEO);
			break;
	}
}

/** VIDEO :: Vertical sync
 *  - Depending on the video mode, call the correct function
 */
void (*VIDEO_VSync)(void);

/** VIDEO :: Text image on Video memory
 *  - Loads a binary file and print it directly on video memory
 */
void VIDEO_BinaryImageToVRAM(const char *dat_name, const char *asset_name) {
	system("cls");
	FILE_LoadBinaryImage(dat_name, asset_name, EGA);
}

/** VIDEO :: PCX Image on Video memory
 *  - Loads a pcx image file and print it directly on video memory
 */
/** Load and display specified text-mode screen */
void VIDEO_PCXImageToVRAM(const char *dat_name, const char *asset_name, int size) {
	word width, height;

	byte *data_loaded = MM_PushChunk(size, CT_TEMPORARY);
	FILE_LoadPCXImage(dat_name, asset_name, data_loaded, size, &width, &height);

	// Depending on the video mode, call the correct function
	switch (settings.video_mode) {
		case VIDEO_MODE_VGA:
			VGA_ImageToVRAM(data_loaded, width, height);
			break;
		case VIDEO_MODE_EGA:
			EGA_ImageToVRAM(data_loaded, width, height);
			break;
		case VIDEO_MODE_CGA:
			CGA_ImageToVRAM(data_loaded, width, height);
			break;
		default:
			sprintf(engine.system_error_message1, "VIDEO_PCXImageToVRAM function error");
			sprintf(engine.system_error_message2, "Unknown video mode selected");
			sprintf(engine.system_error_message3, " ");
			Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_VIDEO);
			break;
	}

	MM_PopChunks(CT_TEMPORARY);
}

/** VIDEO :: Draws screen buffer on Video memory
 *  - Depending on the video mode, call the correct function
 */
void (*VIDEO_ScreenBufferToVRAM)(void);

/** VIDEO :: Fills screen buffer with the data on the current cam position
 */
void VIDEO_MapBufferToScreenBuffer(void) {
	int length1, length2;
	byte *s, *d;
	int scanline = 0;
	int scanline_counter;

	int debug1, debug2;

	debug1 = camera->pos_abs_x;
	debug2 = video->map_buffer_width;

	length1 = video->map_buffer_width - camera->pos_abs_x;
	if (length1 >= camera->visible_width) {
		length1 = camera->visible_width;
		length2 = 0;
	} else {
		length2 = camera->visible_width - length1;
	}

	/////// BACKGROUND ///////////////

	s = &video->map_buffer[MAP_BACKGROUND_LAYER][(camera->pos_abs_y * video->map_buffer_width) + camera->pos_abs_x];
	d = video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK];

	scanline_counter = 0;
	scanline = camera->pos_abs_y;
	while (scanline_counter < camera->visible_height) {
		memcpy(d, s, length1);
		s += video->map_buffer_width;
		d += camera->visible_width;
		scanline_counter++;
		scanline++;
		if (scanline == video->map_buffer_height) {
			scanline = 0;
			s = &video->map_buffer[MAP_BACKGROUND_LAYER][camera->pos_abs_x];
		}
	}

	s = &video->map_buffer[MAP_BACKGROUND_LAYER][(camera->pos_abs_y * video->map_buffer_width)];
	d = video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK] + length1;

	scanline_counter = 0;
	scanline = camera->pos_abs_y;
	while (scanline_counter < camera->visible_height) {
		memcpy(d, s, length2);
		s += video->map_buffer_width;
		d += camera->visible_width;
		scanline_counter++;
		scanline++;
		if (scanline == video->map_buffer_height) {
			scanline = 0;
			s = video->map_buffer[MAP_BACKGROUND_LAYER];
		}
	}

	/////// MASK ///////////////

	s = &video->map_buffer[MAP_MASK_LAYER][(camera->pos_abs_y * video->map_buffer_width) + camera->pos_abs_x];
	d = video->screen_buffer[VIDEO_SCREEN_BUFFER_MASK];

	scanline_counter = 0;
	scanline = camera->pos_abs_y;
	while (scanline_counter < camera->visible_height) {
		memcpy(d, s, length1);
		s += video->map_buffer_width;
		d += camera->visible_width;
		scanline_counter++;
		scanline++;
		if (scanline == video->map_buffer_height) {
			scanline = 0;
			s = &video->map_buffer[MAP_MASK_LAYER][camera->pos_abs_x];
		}
	}

	s = &video->map_buffer[MAP_MASK_LAYER][(camera->pos_abs_y * video->map_buffer_width)];
	d = video->screen_buffer[VIDEO_SCREEN_BUFFER_MASK] + length1;

	scanline_counter = 0;
	scanline = camera->pos_abs_y;
	while (scanline_counter < camera->visible_height) {
		memcpy(d, s, length2);
		s += video->map_buffer_width;
		d += camera->visible_width;
		scanline_counter++;
		scanline++;
		if (scanline == video->map_buffer_height) {
			scanline = 0;
			s = video->map_buffer[MAP_MASK_LAYER];
		}
	}
}

/** VIDEO :: Fills screen buffer with the data on the current cam position
 */
void VIDEO_GraphicsBufferToScreenBuffer(byte *gfx_buffer, int gfx_width_px, int gfx_height_px, int screen_x, int screen_y, Camera cam) {
	int length1, length2;
	byte *s, *d;
	int scanline = 0;
	int scanline_counter;

	length1 = gfx_width_px - cam.pos_abs_x;
	if (length1 >= cam.visible_width) {
		length1 = cam.visible_width;
		length2 = 0;
	} else {
		length2 = cam.visible_width - length1;
	}

	s = &gfx_buffer[(cam.pos_abs_y * gfx_width_px) + cam.pos_abs_x];
	d = video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK];

	scanline_counter = 0;
	scanline = cam.pos_abs_y;
	while ((scanline_counter < cam.visible_height) && (scanline_counter < gfx_height_px)) {
		memcpy(d, s, length1);
		s += gfx_width_px;
		d += cam.visible_width;
		scanline_counter++;
		scanline++;
		if (scanline == gfx_height_px) {
			scanline = 0;
			s = &gfx_buffer[cam.pos_abs_x];
		}
	}

	s = &gfx_buffer[(cam.pos_abs_y * gfx_width_px)];
	d = video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK] + length1;

	scanline_counter = 0;
	scanline = cam.pos_abs_y;
	while ((scanline_counter < cam.visible_height) && (scanline_counter < gfx_height_px)) {
		memcpy(d, s, length2);
		s += gfx_width_px;
		d += cam.visible_width;
		scanline_counter++;
		scanline++;
		if (scanline == gfx_height_px) {
			scanline = 0;
			s = gfx_buffer;
		}
	}
}

/** VIDEO :: PCX Image on Video memory
 *  - Loads a pcx image file and print it directly on video memory
 */
/** Load and display specified text-mode screen */
void VIDEO_PCXImageToScreenBuffer(const char *dat_name, const char *asset_name, int size) {
	int i, src_index, dst_index;
	word width, height;

	byte *data_loaded = MM_PushChunk(size, CT_TEMPORARY);
	FILE_LoadPCXImage(dat_name, asset_name, data_loaded, size, &width, &height);

	//disable();
	src_index = 0;
	dst_index = 0;

	for (i = 0; i < height; i++) {
		memcpy(&video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index], &data_loaded[src_index], width);
		src_index += width;
		dst_index += CAM_VISIBLE_WIDTH;
	}


	MM_PopChunks(CT_TEMPORARY);
}

void VIDEO_DrawPixelOnScreenBuffer(int x, int y, byte color) {
	int dst_index;

	dst_index = y * video->screen_width + x;
	video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = color;
}

/** VIDEO :: Fills screen buffer with the data on the sent data buffer
 *  - x: Position x on the current screen
 *  - y: Position y on the current screen
 *  - width: Width of the status panel
 *  - height: Height of the status panel
 */
void VIDEO_PanelToScreenBuffer(StatusPanel *panel) {
	int i;
	int src_index, dst_index;
	byte *s, *d;
	byte color, count;
	bool end_of_scanline;
	char str[6];

	if (panel->shown) {

		// Draw portait
		src_index = gfx_sprite_graphics_stack[panel->portait_graphics_id].frame_offset[panel->portait_frame];
		s = &gfx_sprite_graphics_stack[panel->portait_graphics_id].buffer[src_index];
		dst_index = ((panel->pos_y + panel->portait_y) * video->screen_width) + (panel->pos_x + panel->portait_x);
		d = &video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index];

		for (i = 0; i < gfx_sprite_graphics_stack[panel->portait_graphics_id].height_px; i++) {
			end_of_scanline = false;
			while (!end_of_scanline) {
				color = *s++;
				switch (color) {
					case 1:// start of transparent bunch
						count = *s++;
						d += count;
						break;
					case SPRITE_TRANSP_COLOR:// transparent pixel
						d++;
						break;
					case 0:// end of scanline
						d += video->screen_width - gfx_sprite_graphics_stack[panel->portait_graphics_id].width_px;
						end_of_scanline = true;
						break;
					default:// just a color!
						*d++ = color;
						break;
				}
			}
		}

		// Draw lifebar
		src_index = 0;
		s = &gfx_sprite_graphics_stack[panel->lifebar_graphics_id].buffer[src_index];
		dst_index = ((panel->pos_y + panel->lifebar_y) * video->screen_width) + (panel->pos_x + panel->lifebar_x);
		d = &video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index];

		for (i = 0; i < gfx_sprite_graphics_stack[panel->lifebar_graphics_id].height_px; i++) {
			end_of_scanline = false;
			while (!end_of_scanline) {
				color = *s++;
				switch (color) {
					case 1:// start of transparent bunch
						count = *s++;
						d += count;
						break;
					case SPRITE_TRANSP_COLOR:// transparent pixel
						d++;
						break;
					case 0:// end of scanline
						d += video->screen_width - gfx_sprite_graphics_stack[panel->lifebar_graphics_id].width_px;
						end_of_scanline = true;
						break;
					default:// just a color!
						*d++ = color;
						break;
				}
			}
		}

		// Draw key
		src_index = 0;
		s = &gfx_sprite_graphics_stack[panel->key_graphics_id].buffer[src_index];
		dst_index = ((panel->pos_y + panel->key_y) * video->screen_width) + (panel->pos_x + panel->key_x);
		d = &video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index];

		for (i = 0; i < gfx_sprite_graphics_stack[panel->key_graphics_id].height_px; i++) {
			end_of_scanline = false;
			while (!end_of_scanline) {
				color = *s++;
				switch (color) {
					case 1:// start of transparent bunch
						count = *s++;
						d += count;
						break;
					case SPRITE_TRANSP_COLOR:// transparent pixel
						d++;
						break;
					case 0:// end of scanline
						d += video->screen_width - gfx_sprite_graphics_stack[panel->key_graphics_id].width_px;
						end_of_scanline = true;
						break;
					default:// just a color!
						*d++ = color;
						break;
				}
			}
		}

		// Draw gun
		src_index = 0;
		s = &gfx_sprite_graphics_stack[panel->gun_graphics_id].buffer[src_index];
		dst_index = ((panel->pos_y + panel->gun_y) * video->screen_width) + (panel->pos_x + panel->gun_x);
		d = &video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index];

		for (i = 0; i < gfx_sprite_graphics_stack[panel->gun_graphics_id].height_px; i++) {
			end_of_scanline = false;
			while (!end_of_scanline) {
				color = *s++;
				switch (color) {
					case 1:// start of transparent bunch
						count = *s++;
						d += count;
						break;
					case SPRITE_TRANSP_COLOR:// transparent pixel
						d++;
						break;
					case 0:// end of scanline
						d += video->screen_width - gfx_sprite_graphics_stack[panel->gun_graphics_id].width_px;
						end_of_scanline = true;
						break;
					default:// just a color!
						*d++ = color;
						break;
				}
			}
		}

		// Depending on the gun, draw current and total bullets
		switch (panel->gun_type) {
			case ACTOR_GUN_TYPE_BARE_HANDS:
				// no bullets at all
				break;
			case ACTOR_GUN_TYPE_PISTOL:
				// Draw current bullets
				switch (panel->bullets_number) {
					case 6:
						dst_index = ((panel->pos_y + panel->gun_y + 12) * video->screen_width) + (panel->pos_x + panel->gun_x + 21);
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 254;// red
						dst_index += video->screen_width;
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 254;// red
						dst_index += video->screen_width;
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 254;// red
					case 5:
						dst_index = ((panel->pos_y + panel->gun_y + 12) * video->screen_width) + (panel->pos_x + panel->gun_x + 19);
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 253;// red
						dst_index += video->screen_width;
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 253;// red
						dst_index += video->screen_width;
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 253;// red
					case 4:
						dst_index = ((panel->pos_y + panel->gun_y + 12) * video->screen_width) + (panel->pos_x + panel->gun_x + 17);
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 253;// red
						dst_index += video->screen_width;
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 253;// red
						dst_index += video->screen_width;
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 253;// red
					case 3:
						dst_index = ((panel->pos_y + panel->gun_y + 12) * video->screen_width) + (panel->pos_x + panel->gun_x + 15);
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 253;// red
						dst_index += video->screen_width;
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 253;// red
						dst_index += video->screen_width;
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 253;// red
					case 2:
						dst_index = ((panel->pos_y + panel->gun_y + 12) * video->screen_width) + (panel->pos_x + panel->gun_x + 13);
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 253;// red
						dst_index += video->screen_width;
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 253;// red
						dst_index += video->screen_width;
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 253;// red
					case 1:
						dst_index = ((panel->pos_y + panel->gun_y + 12) * video->screen_width) + (panel->pos_x + panel->gun_x + 11);
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 253;// red
						dst_index += video->screen_width;
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 253;// red
						dst_index += video->screen_width;
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 253;// red
						break;
					default:
						break;
				}
				// Draw total gun bullets
				sprintf(str, "%03d\n", panel->total_bullets);
				VIDEO_StringToScreenBuffer(panel->pos_x + panel->gun_x, panel->pos_y + panel->gun_y + 16, str, FONT_SLIM_YELLOW);
				break;
			case ACTOR_GUN_TYPE_SHOTGUN:
				// Draw current bullets
				switch (panel->bullets_number) {
					case 1:
						dst_index = ((panel->pos_y + panel->gun_y + 12) * video->screen_width) + (panel->pos_x + panel->gun_x + 19);
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 253;// red
						dst_index += video->screen_width;
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 253;// red
						dst_index += video->screen_width;
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 253;// red
						dst_index = ((panel->pos_y + panel->gun_y + 12) * video->screen_width) + (panel->pos_x + panel->gun_x + 20);
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 253;// red
						dst_index += video->screen_width;
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 253;// red
						dst_index += video->screen_width;
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 253;// red
						break;
					default:
						break;
				}
				// Draw total gun bullets
				sprintf(str, "%03d\n", panel->total_bullets);
				VIDEO_StringToScreenBuffer(panel->pos_x + panel->gun_x, panel->pos_y + panel->gun_y + 16, str, FONT_SLIM_YELLOW);
				break;
			case ACTOR_GUN_TYPE_UZI:
				// Draw current bullets
				switch (panel->bullets_number) {
					case 15:
						dst_index = ((panel->pos_y + panel->gun_y + 13) * video->screen_width) + (panel->pos_x + panel->gun_x + 25);
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 254;// red
						dst_index += video->screen_width;
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 254;// red
					case 14:
						dst_index = ((panel->pos_y + panel->gun_y + 13) * video->screen_width) + (panel->pos_x + panel->gun_x + 23);
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 254;// red
						dst_index += video->screen_width;
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 254;// red
					case 13:
						dst_index = ((panel->pos_y + panel->gun_y + 13) * video->screen_width) + (panel->pos_x + panel->gun_x + 21);
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 254;// red
						dst_index += video->screen_width;
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 254;// red
					case 12:
						dst_index = ((panel->pos_y + panel->gun_y + 13) * video->screen_width) + (panel->pos_x + panel->gun_x + 19);
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 254;// red
						dst_index += video->screen_width;
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 254;// red
					case 11:
						dst_index = ((panel->pos_y + panel->gun_y + 13) * video->screen_width) + (panel->pos_x + panel->gun_x + 17);
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 254;// red
						dst_index += video->screen_width;
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 254;// red
					case 10:
						dst_index = ((panel->pos_y + panel->gun_y + 10) * video->screen_width) + (panel->pos_x + panel->gun_x + 25);
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 254;// red
						dst_index += video->screen_width;
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 254;// red
					case 9:
						dst_index = ((panel->pos_y + panel->gun_y + 10) * video->screen_width) + (panel->pos_x + panel->gun_x + 23);
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 254;// red
						dst_index += video->screen_width;
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 254;// red
					case 8:
						dst_index = ((panel->pos_y + panel->gun_y + 10) * video->screen_width) + (panel->pos_x + panel->gun_x + 21);
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 254;// red
						dst_index += video->screen_width;
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 254;// red
					case 7:
						dst_index = ((panel->pos_y + panel->gun_y + 10) * video->screen_width) + (panel->pos_x + panel->gun_x + 19);
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 254;// red
						dst_index += video->screen_width;
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 254;// red
					case 6:
						dst_index = ((panel->pos_y + panel->gun_y + 10) * video->screen_width) + (panel->pos_x + panel->gun_x + 17);
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 254;// red
						dst_index += video->screen_width;
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 254;// red
					case 5:
						dst_index = ((panel->pos_y + panel->gun_y + 7) * video->screen_width) + (panel->pos_x + panel->gun_x + 25);
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 254;// red
						dst_index += video->screen_width;
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 254;// red
					case 4:
						dst_index = ((panel->pos_y + panel->gun_y + 7) * video->screen_width) + (panel->pos_x + panel->gun_x + 23);
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 254;// red
						dst_index += video->screen_width;
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 254;// red
					case 3:
						dst_index = ((panel->pos_y + panel->gun_y + 7) * video->screen_width) + (panel->pos_x + panel->gun_x + 21);
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 254;// red
						dst_index += video->screen_width;
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 254;// red
					case 2:
						dst_index = ((panel->pos_y + panel->gun_y + 7) * video->screen_width) + (panel->pos_x + panel->gun_x + 19);
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 254;// red
						dst_index += video->screen_width;
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 254;// red
					case 1:
						dst_index = ((panel->pos_y + panel->gun_y + 7) * video->screen_width) + (panel->pos_x + panel->gun_x + 17);
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 254;// red
						dst_index += video->screen_width;
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 254;// red
					default:
						break;
				}
				// Draw total gun bullets
				sprintf(str, "%03d\n", panel->total_bullets);
				VIDEO_StringToScreenBuffer(panel->pos_x + panel->gun_x, panel->pos_y + panel->gun_y + 16, str, FONT_SLIM_YELLOW);
				break;
			case ACTOR_GUN_TYPE_SNIPPER:
				// Draw current bullets
				switch (panel->bullets_number) {
					case 1:
						dst_index = ((panel->pos_y + panel->gun_y + 11) * video->screen_width) + (panel->pos_x + panel->gun_x + 20);
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 253;// red
						dst_index += video->screen_width;
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 253;// red
						dst_index += video->screen_width;
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 253;// red
						dst_index += video->screen_width;
						video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = 253;// red
						break;
					default:
						break;
				}
				// Draw total gun bullets
				sprintf(str, "%03d\n", panel->total_bullets);
				VIDEO_StringToScreenBuffer(panel->pos_x + panel->gun_x, panel->pos_y + panel->gun_y + 16, str, FONT_SLIM_YELLOW);
				break;
		}

		// Draw grenade
		src_index = 0;
		s = &gfx_sprite_graphics_stack[panel->grenade_graphics_id].buffer[src_index];
		dst_index = ((panel->pos_y + panel->grenade_y) * video->screen_width) + (panel->pos_x + panel->grenade_x);
		d = &video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index];

		for (i = 0; i < gfx_sprite_graphics_stack[panel->grenade_graphics_id].height_px; i++) {
			end_of_scanline = false;
			while (!end_of_scanline) {
				color = *s++;
				switch (color) {
					case 1:// start of transparent bunch
						count = *s++;
						d += count;
						break;
					case SPRITE_TRANSP_COLOR:// transparent pixel
						d++;
						break;
					case 0:// end of scanline
						d += video->screen_width - gfx_sprite_graphics_stack[panel->grenade_graphics_id].width_px;
						end_of_scanline = true;
						break;
					default:// just a color!
						*d++ = color;
						break;
				}
			}
		}

		// Draw bullets
		sprintf(str, "%02d\n", panel->grenades_number);
		VIDEO_StringToScreenBuffer(panel->pos_x + panel->grenade_x, panel->pos_y + panel->grenade_y + 16, str, FONT_SLIM_YELLOW);

		// Draw current life
		dst_index = ((panel->pos_y + panel->lifebar_y + 1) * video->screen_width) + (panel->pos_x + panel->lifebar_x + 1);
		d = &video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index];

		for (i = 0; i < 45; i++) {
			if (i < panel->current_life)
				*d++ = 228;
			if (i > panel->current_life && i < panel->old_life)
				*d++ = 253;
			if (i > panel->old_life)
				*d++ = 252;
		}
		src_index = ((panel->pos_y + panel->lifebar_y + 1) * video->screen_width) + (panel->pos_x + panel->lifebar_x + 1);
		s = &video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][src_index];
		dst_index = ((panel->pos_y + panel->lifebar_y + 2) * video->screen_width) + (panel->pos_x + panel->lifebar_x + 1);
		d = &video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index];
		memcpy(d, s, 45);
		src_index = ((panel->pos_y + panel->lifebar_y + 1) * video->screen_width) + (panel->pos_x + panel->lifebar_x + 1);
		s = &video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][src_index];
		dst_index = ((panel->pos_y + panel->lifebar_y + 3) * video->screen_width) + (panel->pos_x + panel->lifebar_x + 1);
		d = &video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index];
		memcpy(d, s, 45);

		// Update life counter
		if (panel->old_life > panel->current_life) {
			panel->life_speed_counter++;
			if (panel->life_speed_counter > panel->life_speed) {
				panel->life_speed_counter = 0;
				panel->old_life--;
			}
		}
		if (panel->old_life < panel->current_life) {
			panel->life_speed_counter++;
			if (panel->life_speed_counter > panel->life_speed) {
				panel->life_speed_counter = 0;
				panel->old_life++;
			}
		}

		// Update time
		if (panel->timeout != 0) {
			panel->current_time++;

			if (panel->current_time >= panel->timeout) {
				panel->shown = false;
				panel->current_time = 0;
			}
		}
	}
}


/** VIDEO :: Fills screen buffer with the data on the sent data buffer
 *  - x: Position x on the current screen
 *  - y: Position y on the current screen
 *  - width: Width of the status panel
 *  - height: Height of the status panel
 */
bool VIDEO_ChatToScreenBuffer(ChatPanel *panel, bool finish_cmd) {
	int i;
	int src_index, dst_index;
	byte *s, *d;
	byte color, count;
	bool end_of_scanline;
	bool eol[3] = {false, false, false};
	bool speech_finished;

	// Draw portait
	src_index = gfx_sprite_graphics_stack[panel->portait_graphics_id].frame_offset[panel->portait_frame];
	s = &gfx_sprite_graphics_stack[panel->portait_graphics_id].buffer[src_index];
	dst_index = ((panel->pos_y + panel->portait_y) * video->screen_width) + (panel->pos_x + panel->portait_x);
	d = &video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index];

	for (i = 0; i < gfx_sprite_graphics_stack[panel->portait_graphics_id].height_px; i++) {
		end_of_scanline = false;
		while (!end_of_scanline) {
			color = *s++;
			switch (color) {
				case 1:// start of transparent bunch
					count = *s++;
					d += count;
					break;
				case SPRITE_TRANSP_COLOR:// transparent pixel
					d++;
					break;
				case 0:// end of scanline
					d += video->screen_width - gfx_sprite_graphics_stack[panel->portait_graphics_id].width_px;
					end_of_scanline = true;
					break;
				default:// just a color!
					*d++ = color;
					break;
			}
		}
	}

	// Draw chat box
	if (panel->chat_inverted) {
		src_index = gfx_sprite_graphics_stack[panel->chat_graphics_id].frame_offset[panel->chat_frame];
		s = &gfx_sprite_graphics_stack[panel->chat_graphics_id].buffer[src_index];
		dst_index = ((panel->pos_y + panel->chat_y) * video->screen_width) + (panel->pos_x + panel->chat_x + gfx_sprite_graphics_stack[panel->chat_graphics_id].width_px);
		d = &video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index];

		for (i = 0; i < gfx_sprite_graphics_stack[panel->chat_graphics_id].height_px; i++) {
			end_of_scanline = false;
			while (!end_of_scanline) {
				color = *s++;
				switch (color) {
					case 1:// start of transparent bunch
						count = *s++;
						d -= count;
						break;
					case SPRITE_TRANSP_COLOR:// transparent pixel
						d--;
						break;
					case 0:// end of scanline
						d += video->screen_width + gfx_sprite_graphics_stack[panel->chat_graphics_id].width_px;
						end_of_scanline = true;
						break;
					default:// just a color!
						*d-- = color;
						break;
				}
			}
		}

	} else {
		src_index = gfx_sprite_graphics_stack[panel->chat_graphics_id].frame_offset[panel->chat_frame];
		s = &gfx_sprite_graphics_stack[panel->chat_graphics_id].buffer[src_index];
		dst_index = ((panel->pos_y + panel->chat_y) * video->screen_width) + (panel->pos_x + panel->chat_x);
		d = &video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index];

		for (i = 0; i < gfx_sprite_graphics_stack[panel->chat_graphics_id].height_px; i++) {
			end_of_scanline = false;
			while (!end_of_scanline) {
				color = *s++;
				switch (color) {
					case 1:// start of transparent bunch
						count = *s++;
						d += count;
						break;
					case SPRITE_TRANSP_COLOR:// transparent pixel
						d++;
						break;
					case 0:// end of scanline
						d += video->screen_width - gfx_sprite_graphics_stack[panel->chat_graphics_id].width_px;
						end_of_scanline = true;
						break;
					default:// just a color!
						*d++ = color;
						break;
				}
			}
		}
	}

	// Draw text

	if ((panel->line_count[0] >= strlen(panel->line[0])) && (panel->line_count[1] >= strlen(panel->line[1])) && (panel->line_count[2] >= strlen(panel->line[2]))) {
		speech_finished = true;
	} else {
		speech_finished = false;
	}

	if (finish_cmd) {
		panel->line_count[0] = strlen(panel->line[0]);
		panel->line_count[1] = strlen(panel->line[1]);
		panel->line_count[2] = strlen(panel->line[2]);
	}

	eol[0] = VIDEO_StringToScreenBufferChat(panel->pos_x + panel->chat_x + 16, panel->pos_y + panel->chat_y + 16, panel->line[0], FONT_SLIM_BLACK, &panel->line_count[0]);
	if (eol[0]) eol[1] = VIDEO_StringToScreenBufferChat(panel->pos_x + panel->chat_x + 16, panel->pos_y + panel->chat_y + 32, panel->line[1], FONT_SLIM_BLACK, &panel->line_count[1]);
	if (eol[1]) eol[2] = VIDEO_StringToScreenBufferChat(panel->pos_x + panel->chat_x + 16, panel->pos_y + panel->chat_y + 48, panel->line[2], FONT_SLIM_BLACK, &panel->line_count[2]);

	return speech_finished;
}

/** VIDEO :: Draw char on screen buffer
 *  - x: Video buffer x pixel position
 *  - y: Video buffer y pixel position
 *  - width: Character width in pixels
 *  - height: Character height in pixels
 *  - data: character pixels buffer
 */
void VIDEO_CharToScreenBuffer(int x, int y, word width, word height, byte transparent_color, byte *data) {
	register int src_index, dst_index;
	register int i, j;
	register byte color;

	dst_index = y * camera->visible_width + x;
	src_index = 0;

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			color = data[src_index];
			if (color != transparent_color) { video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index] = color; }
			src_index++;
			dst_index++;
		}
		dst_index += (camera->visible_width - width);
	}
}

/** VIDEO :: Print string on screen buffer
 *  - x: Position x on the current screen
 *  - y: Position y on the current screen
 *  - length: String length
 *  - string: String data buffer
 */
void VIDEO_StringToScreenBuffer(int x, int y, char *string, int font_number) {
	register int i;
	register int src_index;
	register int pos_x, pos_y;
	register byte chr;

	pos_x = x;
	pos_y = y;

	for (i = 0; i < strlen(string) - 1; i++) {
		chr = string[i];
		switch (chr) {
			case 0xD1:// Ñ
				chr = 0x40;
				break;
			case 0xA1:// ¡
				chr = 0x5F;
				break;
			case 0xBF:// ¿
				chr = 0x5C;
				break;
			default:
				break;
		}

		src_index = (chr - 32) * gfx->font[font_number].char_width * gfx->font[font_number].char_height;
		VIDEO_CharToScreenBuffer(pos_x, pos_y, gfx->font[font_number].char_width, gfx->font[font_number].char_height, VIDEO_TRANSPARENT_COLOR, &gfx->font[font_number].data[src_index]);
		switch (font_number) {
			case FONT_SLIM_BLACK:
			case FONT_SLIM_WHITE:
			case FONT_SLIM_YELLOW:
				pos_x += gfx->font[font_number].char_width - 1;// less 1 to make chars closer
				break;
			case FONT_BIG_BLACK:
			case FONT_BIG_WHITE:
				pos_x += gfx->font[font_number].char_width - 2;// less 2 to make chars closer
				break;
			default:
				pos_x += gfx->font[font_number].char_width - 1;// less 1 to make chars closer
				break;
		}
	}
}

/** VIDEO :: Print string on screen buffer
 *  - x: Position x on the current screen
 *  - y: Position y on the current screen
 *  - length: String length
 *  - string: String data buffer
 */
bool VIDEO_StringToScreenBufferRand(int x, int y, char *string, int font_number, int *char_index, int *counter) {
	register int i;
	register int src_index;
	register int pos_x, pos_y;
	register byte chr;
	register int index;

	index = *char_index;
	pos_y = y;

	for (i = 0; i <= index; i++) {
		chr = string[i];
		switch (chr) {
			case 0xD1:// Ñ
				chr = 0x40;
				break;
			case 0xA1:// ¡
				chr = 0x5F;
				break;
			case 0xBF:// ¿
				chr = 0x5C;
				break;
			default:
				break;
		}
		if (i < index) {
			src_index = (chr - 32) * gfx->font[font_number].char_width * gfx->font[font_number].char_height;
			pos_x = x + i * gfx->font[font_number].char_width - 2;
			switch (font_number) {
				case FONT_SLIM_BLACK:
				case FONT_SLIM_WHITE:
				case FONT_SLIM_YELLOW:
					pos_x = x + (i * (gfx->font[font_number].char_width - 1));// less 1 to make chars closer
					break;
				case FONT_BIG_BLACK:
				case FONT_BIG_WHITE:
					pos_x = x + (i * (gfx->font[font_number].char_width - 2));// less 2 to make chars closer
					break;
				default:
					pos_x = x + (i * (gfx->font[font_number].char_width - 1));// less 1 to make chars closer
					break;
			}
			VIDEO_CharToScreenBuffer(pos_x, pos_y, gfx->font[font_number].char_width, gfx->font[font_number].char_height, VIDEO_TRANSPARENT_COLOR, &gfx->font[font_number].data[src_index]);
		}

		if (i == index) {
			if (*counter >= 5) {
				*char_index += 1;
				*counter = 0;
			} else {
				src_index = (rand() % 32) * gfx->font[font_number].char_width * gfx->font[font_number].char_height;
				switch (font_number) {
					case FONT_SLIM_BLACK:
					case FONT_SLIM_WHITE:
					case FONT_SLIM_YELLOW:
						pos_x = x + (i * (gfx->font[font_number].char_width - 1));// less 1 to make chars closer
						break;
					case FONT_BIG_BLACK:
					case FONT_BIG_WHITE:
						pos_x = x + (i * (gfx->font[font_number].char_width - 2));// less 2 to make chars closer
						break;
					default:
						pos_x = x + (i * (gfx->font[font_number].char_width - 1));// less 1 to make chars closer
						break;
				}
				VIDEO_CharToScreenBuffer(pos_x, pos_y, gfx->font[font_number].char_width, gfx->font[font_number].char_height, VIDEO_TRANSPARENT_COLOR, &gfx->font[font_number].data[src_index]);
				*counter += 1;
			}
		}
	}

	if (*char_index >= strlen(string)) return true;
	else
		return false;
}

/** VIDEO :: Print string on screen buffer
 *  - x: Position x on the current screen
 *  - y: Position y on the current screen
 *  - length: String length
 *  - string: String data buffer
 */
bool VIDEO_StringToScreenBufferSteps(int x, int y, char *string, int font_number, int *char_index) {
	register int src_index;
	register int pos_x, pos_y;
	register byte chr;
	register int index;

	index = *char_index;
	switch (font_number) {
		case FONT_SLIM_BLACK:
		case FONT_SLIM_WHITE:
		case FONT_SLIM_YELLOW:
			pos_x = x + (index * (gfx->font[font_number].char_width - 1));// less 1 to make chars closer
			break;
		case FONT_BIG_BLACK:
		case FONT_BIG_WHITE:
			pos_x = x + (index * (gfx->font[font_number].char_width - 2));// less 2 to make chars closer
			break;
		default:
			pos_x = x + (index * (gfx->font[font_number].char_width - 1));// less 1 to make chars closer
			break;
	}
	pos_y = y;

	//	for (i = 0; i < strlen(string) - 1; i++) {
	chr = string[index];
	switch (chr) {
		case 0xD1:// Ñ
			chr = 0x40;
			break;
		case 0xA1:// ¡
			chr = 0x5F;
			break;
		case 0xBF:// ¿
			chr = 0x5C;
			break;
		default:
			break;
	}

	src_index = (chr - 32) * gfx->font[font_number].char_width * gfx->font[font_number].char_height;
	VIDEO_CharToScreenBuffer(pos_x, pos_y, gfx->font[font_number].char_width, gfx->font[font_number].char_height, VIDEO_TRANSPARENT_COLOR, &gfx->font[font_number].data[src_index]);
	*char_index += 1;
	if (*char_index >= strlen(string)) return true;
	else
		return false;
}
/** VIDEO :: Print string on screen buffer
 *  - x: Position x on the current screen
 *  - y: Position y on the current screen
 *  - length: String length
 *  - string: String data buffer
 */
bool VIDEO_StringToScreenBufferChat(int x, int y, char *string, int font_number, int *char_index) {
	register int i;
	register int src_index;
	register int pos_x, pos_y;
	register byte chr;

	pos_x = x;
	pos_y = y;

	for (i = 0; i < *char_index - 1; i++) {
		chr = string[i];
		switch (chr) {
			case 0xD1:// Ñ
				chr = 0x40;
				break;
			case 0xA1:// ¡
				chr = 0x5F;
				break;
			case 0xBF:// ¿
				chr = 0x5C;
				break;
			default:
				break;
		}

		src_index = (chr - 32) * gfx->font[font_number].char_width * gfx->font[font_number].char_height;
		VIDEO_CharToScreenBuffer(pos_x, pos_y, gfx->font[font_number].char_width, gfx->font[font_number].char_height, VIDEO_TRANSPARENT_COLOR, &gfx->font[font_number].data[src_index]);
		switch (font_number) {
			case FONT_SLIM_BLACK:
			case FONT_SLIM_WHITE:
			case FONT_SLIM_YELLOW:
				pos_x += gfx->font[font_number].char_width - 1;// less 1 to make chars closer
				break;
			case FONT_BIG_BLACK:
			case FONT_BIG_WHITE:
				pos_x += gfx->font[font_number].char_width - 2;// less 2 to make chars closer
				break;
			default:
				pos_x += gfx->font[font_number].char_width - 1;// less 1 to make chars closer
				break;
		}
	}

	if (*char_index < strlen(string)) {
		*char_index += 1;
		return false;
	} else {
		return true;
	}
}

/** VIDEO :: Draw sprite on screen buffer
 *  - x_px = x position in pixels
 *  - y_px = y position  in pixels
 */
void VIDEO_DrawSpriteInterlacedToScreenBufferRLE(Graphic *spr_graphics, SpriteGfx *spr, int odd) {
	int i;
	int src_index, dst_index;
	int dst_x_index, dst_y_index;
	byte *s, *d, *m;
	byte color, count;
	bool end_of_scanline;
	bool hide_color;
	int horizontal_pixel_count;
	int break_point_x_right, break_point_x_left;

	if ((spr->screen_pos_x + spr_graphics->width_px) > video->screen_width) {
		break_point_x_right = spr_graphics->width_px - ((spr->screen_pos_x + spr_graphics->width_px) - video->screen_width);
	} else {
		break_point_x_right = spr_graphics->width_px;
	}

	if (spr->screen_pos_x < 0) {
		break_point_x_left = abs(spr->screen_pos_x);
	} else {
		break_point_x_left = 0;
	}

	// Print sprite
	if (!spr->inverted) {
		// Calculate pointers
		src_index = spr_graphics->frame_offset[spr->frame];
		dst_y_index = spr->screen_pos_y;
		dst_x_index = spr->screen_pos_x;
		dst_index = (dst_y_index * video->screen_width) + dst_x_index;

		s = &spr_graphics->buffer[src_index];
		m = &video->screen_buffer[VIDEO_SCREEN_BUFFER_MASK][dst_index];
		d = &video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index];

		for (i = 0; i < spr_graphics->height_px; i++) {
			end_of_scanline = false;
			horizontal_pixel_count = 0;
			while (!end_of_scanline) {
				hide_color = false;
				if (horizontal_pixel_count >= break_point_x_right) {
					hide_color = true;
				}
				if (horizontal_pixel_count < break_point_x_left) {
					hide_color = true;
				}

				// Print sprite
				color = *s++;
				switch (color) {
					case 1:// start of transparent bunch
						count = *s++;
						d += count;
						m += count;
						horizontal_pixel_count += count;
						break;
					case 0:// end of scanline
						d += video->screen_width - spr_graphics->width_px;
						m += video->screen_width - spr_graphics->width_px;
						end_of_scanline = true;
						break;
					case SPRITE_TRANSP_COLOR:// transparent pixel
						d++;
						m++;
						horizontal_pixel_count++;
						break;
					default:// just a color!
						// Print unmasked pixels
						if (((*m++ == 0) || (spr->unmasked)) && !spr->invisible && (i % 2 == odd)) {
							if (spr->blink) {
								d++;
							} else {
								if (!hide_color) {
									*d++ = color;
								} else {
									d++;
								}
							}
						} else {
							d++;
						}
						horizontal_pixel_count++;
						break;
				}
			}
		}
	} else {
		// Calculate pointers
		src_index = spr_graphics->frame_offset[spr->frame];
		dst_y_index = spr->screen_pos_y;
		dst_x_index = spr->screen_pos_x + spr_graphics->width_px;
		dst_index = (dst_y_index * video->screen_width) + dst_x_index;

		s = &spr_graphics->buffer[src_index];
		m = &video->screen_buffer[VIDEO_SCREEN_BUFFER_MASK][dst_index];
		d = &video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index];

		// print sprite
		for (i = 0; i < spr_graphics->height_px; i++) {
			end_of_scanline = false;
			horizontal_pixel_count = spr_graphics->width_px;
			while (!end_of_scanline) {
				hide_color = false;
				if (horizontal_pixel_count >= break_point_x_right) {
					hide_color = true;
				}
				if (horizontal_pixel_count < break_point_x_left) {
					hide_color = true;
				}

				// Print sprite
				color = *s++;
				switch (color) {
					case 1:// start of transparent bunch
						count = *s++;
						d -= count;
						m -= count;
						horizontal_pixel_count -= count;
						break;
					case 0:// end of scanline
						d += video->screen_width + spr_graphics->width_px;
						m += video->screen_width + spr_graphics->width_px;
						end_of_scanline = true;
						break;
					case SPRITE_TRANSP_COLOR:// transparent pixel
						d--;
						m--;
						horizontal_pixel_count--;
						break;
					default:// just a color!
						// Print unmasked pixels
						if (((*m-- == 0) || (spr->unmasked)) && !spr->invisible && (i % 2 == odd)) {
							if (spr->blink) {
								d--;
							} else {
								if (!hide_color) {
									*d-- = color;
								} else {
									d--;
								}
							}
						} else {
							d--;
						}
						horizontal_pixel_count--;
						break;
				}
			}
		}
	}
}

/** VIDEO :: Draw sprite on screen buffer
 *  - x_px = x position in pixels
 *  - y_px = y position  in pixels
 */
void VIDEO_DrawSpriteToScreenBufferRLE(Graphic *spr_graphics, SpriteGfx *spr) {
	int i;
	int src_index, dst_index;
	int dst_x_index, dst_y_index;
	byte *s, *d, *m;
	byte color, count;
	bool end_of_scanline;
	bool hide_color;
	int scanline_counter;
	int horizontal_pixel_count;
	int break_point_x_right, break_point_x_left;
	int break_point_y_up, break_point_y_down;

	int debug1, debug2, debug3, debug4;

	SpriteGfx debug_spr;
	Graphic debug_graphic;


	debug_spr = *spr;
	debug_graphic = *spr_graphics;

	if ((spr->screen_pos_x + spr_graphics->width_px) > video->screen_width) {
		break_point_x_right = spr_graphics->width_px - ((spr->screen_pos_x + spr_graphics->width_px) - video->screen_width);
	} else {
		break_point_x_right = spr_graphics->width_px;
	}

	if (spr->screen_pos_x < 0) {
		break_point_x_left = abs(spr->screen_pos_x);
	} else {
		break_point_x_left = 0;
	}

	if (spr->screen_pos_y < 0)
		break_point_y_up = abs(spr->screen_pos_y);
	else
		break_point_y_up = 0;

	if ((spr->screen_pos_y + spr_graphics->height_px) > video->screen_height)
		break_point_y_down = spr_graphics->height_px - ((spr->screen_pos_y + spr_graphics->height_px) - video->screen_height);
	else
		break_point_y_down = spr_graphics->height_px;

	scanline_counter = 0;

	// Print sprite
	if (!spr->inverted) {
		// Calculate pointers
		// - source pointer
		src_index = spr_graphics->frame_offset[spr->frame];
		s = &spr_graphics->buffer[src_index];
		if (break_point_y_up > 0) {
			while (scanline_counter < break_point_y_up) {
				// Print sprite
				color = *s++;
				if (color == 0) scanline_counter++;
			}
		}

		dst_y_index = spr->screen_pos_y + break_point_y_up;
		dst_x_index = spr->screen_pos_x;
		dst_index = (dst_y_index * video->screen_width) + dst_x_index;

		m = &video->screen_buffer[VIDEO_SCREEN_BUFFER_MASK][dst_index];
		d = &video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index];

		//for (i = 0; i < spr_graphics->height_px; i++) {
		for (i = break_point_y_up; i < break_point_y_down; i++) {

			end_of_scanline = false;
			horizontal_pixel_count = 0;
			while (!end_of_scanline) {
				hide_color = false;
				if (horizontal_pixel_count >= break_point_x_right) {
					hide_color = true;
				}
				if (horizontal_pixel_count < break_point_x_left) {
					hide_color = true;
				}

				// Print sprite
				color = *s++;
				switch (color) {
					case 1:// start of transparent bunch
						count = *s++;
						d += count;
						m += count;
						horizontal_pixel_count += count;
						break;
					case 0:// end of scanline
						d += video->screen_width - spr_graphics->width_px;
						m += video->screen_width - spr_graphics->width_px;
						end_of_scanline = true;
						break;
					case SPRITE_TRANSP_COLOR:// transparent pixel
						d++;
						m++;
						horizontal_pixel_count++;
						break;
					default:// just a color!
						// Print unmasked pixels
						if (((*m == 0) || (spr->unmasked)) && !spr->invisible) {
							if (spr->blink) {
								d++;
								m++;
							} else {
								if (!hide_color) {
									*d++ = color;
									m++;
								} else {
									d++;
									m++;
								}
							}
						} else {
							d++;
							m++;
						}
						horizontal_pixel_count++;
						break;
				}
			}
		}
	} else {
		// Calculate pointers
		// - source pointer
		src_index = spr_graphics->frame_offset[spr->frame];
		s = &spr_graphics->buffer[src_index];
		if (break_point_y_up > 0) {
			while (scanline_counter < break_point_y_up) {
				// Print sprite
				color = *s++;
				if (color == 0) scanline_counter++;
			}
		}

		dst_y_index = spr->screen_pos_y + break_point_y_up;
		dst_x_index = spr->screen_pos_x + spr_graphics->width_px;
		dst_index = (dst_y_index * video->screen_width) + dst_x_index;


		m = &video->screen_buffer[VIDEO_SCREEN_BUFFER_MASK][dst_index];
		d = &video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index];

		// print sprite
		for (i = break_point_y_up; i < break_point_y_down; i++) {
			end_of_scanline = false;
			horizontal_pixel_count = spr_graphics->width_px;
			while (!end_of_scanline) {
				hide_color = false;
				if (horizontal_pixel_count >= break_point_x_right) {
					hide_color = true;
				}
				if (horizontal_pixel_count < break_point_x_left) {
					hide_color = true;
				}

				// Print sprite
				color = *s++;
				switch (color) {
					case 1:// start of transparent bunch
						count = *s++;
						d -= count;
						m -= count;
						horizontal_pixel_count -= count;
						break;
					case 0:// end of scanline
						d += video->screen_width + spr_graphics->width_px;
						m += video->screen_width + spr_graphics->width_px;
						end_of_scanline = true;
						break;
					case SPRITE_TRANSP_COLOR:// transparent pixel
						d--;
						m--;
						horizontal_pixel_count--;
						break;
					default:// just a color!
						// Print unmasked pixels
						if (((*m == 0) || (spr->unmasked)) && !spr->invisible) {
							if (spr->blink) {
								d--;
								m--;
							} else {
								if (!hide_color) {
									*d-- = color;
									m--;
								} else {
									d--;
									m--;
								}
							}
						} else {
							d--;
							m--;
						}
						horizontal_pixel_count--;
						break;
				}
			}
		}
	}
}

/** VIDEO :: Set palette
 */
void VIDEO_SetPalette(byte *palette) {
	// Depending on the video mode, call the correct function
	switch (settings.video_mode) {
		case VIDEO_MODE_VGA:
			VGA_SetPalette(palette);
			break;
		case VIDEO_MODE_EGA:
			VGA_SetPalette(palette);
			break;
		case VIDEO_MODE_CGA:
			VGA_SetPalette(palette);
			break;
		default:
			sprintf(engine.system_error_message1, "VIDEO_SetPalette function error");
			sprintf(engine.system_error_message2, "Unknown video mode selected");
			sprintf(engine.system_error_message3, " ");
			Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_VIDEO);
			break;
	}
}

/** VIDEO :: Clear palette
 */
void VIDEO_ClearPalette(void) {
	// Depending on the video mode, call the correct function
	switch (settings.video_mode) {
		case VIDEO_MODE_EGA:
			VGA_ClearPalette();
			break;
		case VIDEO_MODE_VGA:
			VGA_ClearPalette();
			break;
		case VIDEO_MODE_CGA:
			VGA_ClearPalette();
			break;
		default:
			sprintf(engine.system_error_message1, "VIDEO_SetPalette function error");
			sprintf(engine.system_error_message2, "Unknown video mode selected");
			sprintf(engine.system_error_message3, " ");
			Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_VIDEO);
			break;
	}
}

/** VIDEO :: Clear screen
 */
void VIDEO_ClearScreen(void) {
	// Depending on the video mode, call the correct function
	switch (settings.video_mode) {
		case VIDEO_MODE_EGA:
			memset(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], 0, video->screen_width * video->screen_height);
			memset(video->screen_buffer[VIDEO_SCREEN_BUFFER_MASK], 0, video->screen_width * video->screen_height);
			memset(video->screen_buffer[VIDEO_SCREEN_BUFFER_FORE], 0, video->screen_width * video->screen_height);
			VGA_ClearScreen();
			break;
		case VIDEO_MODE_VGA:
			memset(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], 0, video->screen_width * video->screen_height);
			memset(video->screen_buffer[VIDEO_SCREEN_BUFFER_MASK], 0, video->screen_width * video->screen_height);
			memset(video->screen_buffer[VIDEO_SCREEN_BUFFER_FORE], 0, video->screen_width * video->screen_height);
			VGA_ClearScreen();
			break;
		case VIDEO_MODE_CGA:
			memset(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], 0, video->screen_width * video->screen_height);
			memset(video->screen_buffer[VIDEO_SCREEN_BUFFER_MASK], 0, video->screen_width * video->screen_height);
			memset(video->screen_buffer[VIDEO_SCREEN_BUFFER_FORE], 0, video->screen_width * video->screen_height);
			VGA_ClearScreen();
			break;
		default:
			sprintf(engine.system_error_message1, "VIDEO_ClearScreen function error");
			sprintf(engine.system_error_message2, "Unknown video mode selected");
			sprintf(engine.system_error_message3, " ");
			Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_VIDEO);
			break;
	}
}

/** VIDEO :: Clear screen
 */
void VIDEO_ClearScreenBuffer(void) {
	// Depending on the video mode, call the correct function
	switch (settings.video_mode) {
		case VIDEO_MODE_EGA:
			memset(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], 0, video->screen_width * video->screen_height);
			memset(video->screen_buffer[VIDEO_SCREEN_BUFFER_MASK], 0, video->screen_width * video->screen_height);
			memset(video->screen_buffer[VIDEO_SCREEN_BUFFER_FORE], 0, video->screen_width * video->screen_height);
			break;
		case VIDEO_MODE_VGA:
			memset(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], 0, video->screen_width * video->screen_height);
			memset(video->screen_buffer[VIDEO_SCREEN_BUFFER_MASK], 0, video->screen_width * video->screen_height);
			memset(video->screen_buffer[VIDEO_SCREEN_BUFFER_FORE], 0, video->screen_width * video->screen_height);
			break;
		case VIDEO_MODE_CGA:
			memset(video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], 0, video->screen_width * video->screen_height);
			memset(video->screen_buffer[VIDEO_SCREEN_BUFFER_MASK], 0, video->screen_width * video->screen_height);
			memset(video->screen_buffer[VIDEO_SCREEN_BUFFER_FORE], 0, video->screen_width * video->screen_height);
			break;
		default:
			sprintf(engine.system_error_message1, "VIDEO_ClearScreenBuffer function error");
			sprintf(engine.system_error_message2, "Unknown video mode selected");
			sprintf(engine.system_error_message3, " ");
			Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_VIDEO);
			break;
	}
}


/** VIDEO :: Asyncronous palette rotation set
 */
void VIDEO_RotatePaletteStart_Async(int first_index, int last_index, int speed) {
	video->rotate_palette_async = true;
	video->rotate_first_index = first_index;
	video->rotate_last_index = last_index;
	video->rotate_speed = speed;
}

/** VIDEO :: Asyncronous palette rotation reet
 */
void VIDEO_RotatePaletteEnd_Async(void) {
	video->rotate_palette_async = false;
}

/** VIDEO :: Asyncronous fade in
 */
void VIDEO_FadeIn_Async(int speed) {
	video->fading_speed = speed;
	video->fading_step = 0;
	video->faded_in = false;
	video->faded_out = false;
	video->fading_out_async = false;
	video->fading_in_async = true;
}

bool VIDEO_AwaitFadedIn(void) {
	while (!video->faded_in) {
		return false;
	}
	return true;
}

/** VIDEO :: Asyncronous fade out
 */
void VIDEO_FadeOut_Async(int speed) {
	video->fading_step = 0;
	video->faded_in = false;
	video->faded_out = false;
	video->fading_in_async = false;
	video->fading_out_async = true;
}

bool VIDEO_AwaitFadedOut(void) {
	while (!video->faded_out) {
		return false;
	}
	return true;
}

/** VIDEO :: Syncronous fade in
 */
void VIDEO_FadeIn(int speed) {
	video->fading_in_async = false;
	video->fading_out_async = false;
	// Depending on the video mode, call the correct function
	switch (settings.video_mode) {
		case VIDEO_MODE_EGA:
			VGA_FadeIn(speed);
			break;
		case VIDEO_MODE_VGA:
			VGA_FadeIn(speed);
			break;
		case VIDEO_MODE_CGA:
			VGA_FadeIn(speed);
			break;
		default:
			sprintf(engine.system_error_message1, "VIDEO_FadeIn function error");
			sprintf(engine.system_error_message2, "Unknown video mode selected");
			sprintf(engine.system_error_message3, " ");
			Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_VIDEO);
			break;
	}
	video->faded_in = true;
	video->faded_out = false;
}

/** VIDEO :: Syncronous fade out
 */
void VIDEO_FadeOut(int speed) {
	video->fading_in_async = false;
	video->fading_out_async = false;
	// Depending on the video mode, call the correct function
	switch (settings.video_mode) {
		case VIDEO_MODE_EGA:
			VGA_FadeOut(speed);
			break;
		case VIDEO_MODE_VGA:
			VGA_FadeOut(speed);
			break;
		case VIDEO_MODE_CGA:
			VGA_FadeOut(speed);
			break;
		default:
			sprintf(engine.system_error_message1, "VIDEO_FadeOut function error");
			sprintf(engine.system_error_message2, "Unknown video mode selected");
			sprintf(engine.system_error_message3, " ");
			Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_VIDEO);
			break;
	}
	video->faded_in = false;
	video->faded_out = true;
}

/** VIDEO :: Syncronous fade out
 */
void VIDEO_FadeOutToWhite(int speed) {
	video->fading_in_async = false;
	video->fading_out_async = false;
	// Depending on the video mode, call the correct function
	switch (settings.video_mode) {
		case VIDEO_MODE_EGA:
			VGA_FadeOutToWhite(speed);
			break;
		case VIDEO_MODE_VGA:
			VGA_FadeOutToWhite(speed);
			break;
		case VIDEO_MODE_CGA:
			VGA_FadeOutToWhite(speed);
			break;
		default:
			sprintf(engine.system_error_message1, "VIDEO_FadeOutToWhite function error");
			sprintf(engine.system_error_message2, "Unknown video mode selected");
			sprintf(engine.system_error_message3, " ");
			Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_VIDEO);
			break;
	}
	video->faded_in = false;
	video->faded_out = true;
}


void VIDEO_FadeOutPause(int atenuation) {
	int i;
	for (i = 0; i < 219 * 3; i++) {
		if ((gfx->palette_shown[i] - atenuation) > 0) {
			gfx->palette_shown[i] -= atenuation;
		} else {
			gfx->palette_shown[i] = 0;
		}
		if ((gfx->palette_shown[i + 1] - atenuation) > 0) {
			gfx->palette_shown[i + 1] -= atenuation;
		} else {
			gfx->palette_shown[i + 1] = 0;
		}
		if ((gfx->palette_shown[i + 2] - atenuation) > 0) {
			gfx->palette_shown[i + 2] -= atenuation;
		} else {
			gfx->palette_shown[i + 2] = 0;
		}
	}
	VGA_SetPalette(gfx->palette_shown);
}

/** VIDEO :: Image buffer to screen buffer
 *  - Prints to screen buffer a previously loaded image on a buffer
 */
/** Load and display specified text-mode screen */
void VIDEO_BufferToScreenBuffer(byte *buffer, word buffer_width, word buffer_height, int window_width, int window_height, int screen_pos_x, int screen_pos_y) {
	int i, src_index, dst_index, length;

	src_index = 0;
	dst_index = screen_pos_y * CAM_VISIBLE_WIDTH + screen_pos_x;
	if (window_width > CAM_VISIBLE_WIDTH) length = CAM_VISIBLE_WIDTH;
	else
		length = window_width;

	for (i = 0; i < window_height; i++) {
		memcpy(&video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index], &buffer[src_index], length);
		src_index += buffer_width;
		dst_index += CAM_VISIBLE_WIDTH;
	}
}


void VIDEO_DrawSquareToScreenBuffer(byte *buffer, word buffer_width, word buffer_height, int square_width, int square_height, int screen_pos_x, int screen_pos_y, byte color) {

	int i, dst_index;
	dst_index = screen_pos_y * CAM_VISIBLE_WIDTH + screen_pos_x;
	for (i = 0; i < square_height; i++) {
		memset(&video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK][dst_index], color, square_width);
		dst_index += CAM_VISIBLE_WIDTH;
	}
}
