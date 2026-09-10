/**************************************************
 * VGA graphics functions
 *   - Manages all the video issues about vga video card
 *   - Video memory allocation on UMA for VGA (320x200 256 colours)
 *	*** A0000h - FFFFFh ***** UMA
 *		****** A0000h - AFFFFh ***** UMA>> EGA/VGA Buffer (64kb)
 *		****** B0000h - B7FFFh ***** UMA>> MDA/HERCULES Monochrome video adapter (32kb)
 *		****** B8000h - BFFFFh ***** UMA>> CGA/VGA Text mode (32kb)           
 *    ****** C0000h - FFFFFh ***** UMA>> ROM Code memory, BIOS, etc... (256kb)
 *		********* C0000h - C7FFFh ***** UMA>> EGA/VGA BIOS rom
 *		************ C4000**************** UMA >> Video adapter ROM space
 *		************ C6000**************** UMA >> PGA Communication (254 bytes)
 * 		********* C8000h - CFFFFh ***** UMA>> BIOS, etc...
 *    ************ C8000h ************** UMA >> Hard disk adapter bios ROM (16kb)
 *     ************ C8000:5h ************ UMA >> XT Hard disk ROM format, AH=Drive, AL=Interleave
 *		********* D0000h - DFFFFh ***** UMA>> BIOS >> Free.
 *		************ D0000h - D4999h ***** UMA>> Cluster adapter BIOS ROM (32kb)
 *		************ D5000h - D5BFFh ***** UMA>> Free>> Spare.
 *		************ D5C00h - D83FFh ***** UMA>> Free>> Spare.
 *		************ D8400h - DCBFFh ***** UMA>> Free>> Spare
 *		************ DCC00h - D????h ***** UMA>> Free>> Fonts
 *		********* E0000h - EFFFFh ***** UMA>> Expansion ROM space (hardwired on AT+)(64kb) & PS/2 System ROM (thru F000) (128kb)
 *		********* F0000h - FFFFFh ***** UMA>> Motherboard
 *		************ F0000h - F3FFFh ***** UMA>> System monitor ROM
 *		************ F4000h - F5FFFh ***** UMA>> System expansion ROM
 *		************ F6000h - F7FFFh ***** UMA>> IBM ROM BASIC (AT)
 *		************ F8000h - FBFFFh ***** UMA>> PCjr software cartridge override address
 *		************ FC000h - FEFFFh ***** UMA>> BIOS ROM
 *		************ FF000h - FFA60h ***** UMA>> System ROM
 *		************ FFA60:E ************ UMA>>	ROM graphics character table
 *		************ FFFFF:0 ************ UMA>>	ROM bootstrap code
 *		************ FFFFF:5 ************ UMA>> (8 bytes)	ROM date (not applicable for all clones)
 *		************ FFFFF:E ************ UMA>>	(byte)	ROM machine id	(see MACHINE ID)
 ************************/

#include "../engine.h"
#include "pc.h"
#include <string.h>
#include "vga.h"
#include "../camera/camera.h"
#include "video.h"

byte *VGA;


/** VGA :: Check graphics card compatibility
 *  - Returns true if a VGA compatible card is detected
 */
bool VGA_CheckGraphicsCard(void) {
	union REGS regs;
	byte active_display;
	//byte inactive_display;
	bool compatible;

	regs.x.ax = 0x1a00; /* VGA only function get video displays */
	int86(0x10, &regs, &regs);
	// returns
	// bl := active display
	// bh := inactive display

	if (regs.h.al == 0x1A) {
		// VGA card detected
		// keep current active and inactive display
		// FF  Unrecognized video system
		// 00  No display
		// 01  MDA with monochrome display
		// 02  CGA with color display *** compatible mode
		// 03  Reserved
		// 04  EGA with color display *** compatible mode
		// 05  EGA with monochrome display *** compatible mode
		// 06  Professional graphics controller
		// 07  VGA with analog monochrome display  *** compatible mode
		// 08  VGA with analog color display  *** compatible mode
		// 09  Reserved
		// 0A  MCGA with digital color display
		// 0B  MCGA with analog monochrome display
		// 0C  MCGA with analog color display
		active_display = regs.h.bl;
		//inactive_display = regs.h.bh;
	} else {
		active_display = 0;
		//inactive_display = 0;
	}

	// Check only compatible displays
	switch (active_display) {
		case 7:
			compatible = true;
			break;
		case 8:
			compatible = true;
			break;
		default:
			compatible = false;
			break;
	}

	return compatible;
}

/** VGA :: Vertical sync
 * 
 */
void VGA_VSync(void) {
	/* wait until any previous retrace has ended */
	do {
	} while (inportb(VGA_ADC_PORT) & 0x08);// Check bit 3

	/* wait until a new retrace has just begun */
	do {
	} while (!(inportb(VGA_ADC_PORT) & 0x08));// Check bit 3
}

/** VGA :: Show cursor
 */
void VGA_HideCursor(void) {
	__dpmi_regs regs = {0};
	regs.x.cx = 0x2607;
	regs.h.ah = 0x01;
	__dpmi_int(0x10, &regs);
}

/** Sets VGA 256-color palette entry at specified index
 *
 * index must be between 0 and 255, and each color component (r, g, b) must be
 * between 0 and 63.
 */
void VGA_SetPalette(byte *palette) {
	int i;
	memcpy(gfx->palette_shown, palette, 255 * 3);
	outportb(VGA_PALETTE_INDEX_WR, 0);
	for (i = 0; i < 255 * 3; i++) {
		outportb(VGA_PALETTE_DATA, palette[i]);// Set the 256 palette colors
	}
}

/** VGA :: Clear palette
 */
void VGA_ClearPalette(void) {
	int i;
	memset(gfx->palette_shown, 0, 255 * 3);
	outportb(VGA_PALETTE_INDEX_WR, 0);
	for (i = 0; i < 256 * 3; i++) {
		outportb(VGA_PALETTE_DATA, 0);// Set the 256 palette colors
	}
}

/** VGA :: Set palette range
 */
void VGA_SetPaletteRange(byte *palette, int start, int end) {
	int i;
	outportb(VGA_PALETTE_INDEX_WR, start);
	for (i = start * 3; i < end * 3; i++) {
		outportb(VGA_PALETTE_DATA, palette[i]);// Set the custom number of palette colors
	}
}

/** VGA :: Set video mode
 */
void VGA_SetMode(int mode) {
	__dpmi_regs regs = {0};
	regs.x.ax = mode;
	__dpmi_int(0x10, &regs);
}

/** VGA :: Fast copy data to vram
 */
void VGA_FastCopy(byte *buffer, byte *vram, int size) {
	asm volatile(
			"cld\n\t"       // Clear direction flag (forward copying)
			"rep movsd\n\t" // Copy ECX DWORDs from ESI (backbuffer) to EDI (VGA memory)
			:               /* no output */
			: "S"(buffer),  // ESI = backbuffer source
			  "D"(vram),    // EDI = VGA memory destination
			  "c"(size >> 2)// ECX = Number of DWORDs to copy (320*200/4)
			: "memory");
}

/** VGA :: Draws screen buffer on Video memory
 *  - Draws an entire screen buffer on video ram 
 */
void VGA_ScreenMainBufferToVRAM(void) {
	VGA = (byte *) (__djgpp_conventional_base + 0xA0000);
	memcpy(VGA, video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], VGA_RESOLUTION_WIDTH * VGA_RESOLUTION_HEIGHT);
}

/** VGA :: Draws image buffer data on Video memory
 *  - Draws an entire buffer on video ram 
 */
void VGA_ImageToVRAM(byte *buffer, int width, int height) {
	int i;
	int src_index, dst_index;
	VGA = (byte *) (__djgpp_conventional_base + 0xA0000);

	// Transfer data to video buffer
	src_index = 0;
	dst_index = 0;
	for (i = 0; i < height; i++) {
		memcpy(&VGA[dst_index], &buffer[src_index], width);
		src_index += width;
		dst_index += VGA_RESOLUTION_WIDTH;
	}
}

/**  VGA :: Clear screen buffer
  */
void VGA_ClearScreen(void) {
	VGA = (byte *) (__djgpp_conventional_base + 0xA0000);
	memset(VGA, 0, VGA_RESOLUTION_WIDTH * VGA_RESOLUTION_HEIGHT);
}

/** VGA :: Syncronous fade in
 */
void VGA_FadeIn(int speed) {
	volatile int i;
	volatile int step, max_steps;

	VGA_VSync();
	VGA_ClearPalette();
	max_steps = (64 / speed) - 1;
	step = 0;
	while (step < max_steps) {
		for (i = 0; i < 255 * 3; i++) {
			gfx->palette_shown[i] += speed;
			if (gfx->palette_shown[i] > gfx->palette_loaded[i]) {
				gfx->palette_shown[i] = gfx->palette_loaded[i];
			}
			gfx->palette_shown[i + 1] += speed;
			if (gfx->palette_shown[i + 1] > gfx->palette_loaded[i + 1]) {
				gfx->palette_shown[i + 1] = gfx->palette_loaded[i + 1];
			}
			gfx->palette_shown[i + 2] += speed;
			if (gfx->palette_shown[i + 2] > gfx->palette_loaded[i + 2]) {
				gfx->palette_shown[i + 2] = gfx->palette_loaded[i + 2];
			}
		}
		VGA_VSync();
		VGA_SetPalette(gfx->palette_shown);
		step++;
	}

	VGA_SetPalette(gfx->palette_loaded);
	memcpy(gfx->palette_shown, gfx->palette_loaded, 255 * 3);
}

/** VGA :: Syncronous fade in
 */
bool VGA_FadeIn_Async(int speed, int *step) {
	volatile int i;
	volatile int sp;
	volatile int max_steps;

	sp = speed;
	if (sp > 4) { sp = 4; }
	if (sp < 1) { sp = 1; }
	max_steps = 8;

	if (*step <= max_steps) {
		for (i = 0; i < 256 * 3; i++) {
			gfx->palette_shown[i] += gfx->palette_loaded[i] >> (max_steps - *step);
			if (gfx->palette_shown[i] > gfx->palette_loaded[i]) {
				gfx->palette_shown[i] = gfx->palette_loaded[i];
			}
		}
		VGA_SetPalette(gfx->palette_shown);
		*step = *step + 1;
		return false;
	} else {
		return true;
	}
}

/** VGA :: Syncronous fade out
 */
void VGA_FadeOut(int speed) {
	volatile int i;
	volatile int step, max_steps;
	memcpy(gfx->palette_loaded, gfx->palette_shown, 255 * 3);

	VGA_VSync();
	VGA_SetPalette(gfx->palette_shown);

	max_steps = (64 / speed) - 1;
	step = 0;
	while (step < max_steps) {
		for (i = 0; i < 255 * 3; i++) {
			if ((gfx->palette_shown[i] - speed) > 0) {
				gfx->palette_shown[i] -= speed;
			} else {
				gfx->palette_shown[i] = 0;
			}
			if ((gfx->palette_shown[i + 1] - speed) > 0) {
				gfx->palette_shown[i + 1] -= speed;
			} else {
				gfx->palette_shown[i + 1] = 0;
			}
			if ((gfx->palette_shown[i + 2] - speed) > 0) {
				gfx->palette_shown[i + 2] -= speed;
			} else {
				gfx->palette_shown[i + 2] = 0;
			}
		}
		VGA_VSync();
		VGA_SetPalette(gfx->palette_shown);
		step++;
	}

	memset(gfx->palette_shown, 0, 255 * 3);
	VGA_SetPalette(gfx->palette_shown);
}

/** VGA :: Syncronous fade out
 */
void VGA_FadeOutToWhite(int speed) {
	volatile int i;
	volatile int step;
	volatile bool not_ended;
	memcpy(gfx->palette_loaded, gfx->palette_shown, 255 * 3);

	VGA_VSync();
	VGA_SetPalette(gfx->palette_shown);

	not_ended = true;
	while (not_ended) {
		not_ended = false;
		for (i = 0; i < 255 * 3; i += 3) {
			if ((gfx->palette_shown[i]) < 255) {
				gfx->palette_shown[i] += 1;
				not_ended = true;
			} else {
				gfx->palette_shown[i] = 255;
			}
			if ((gfx->palette_shown[i + 1]) < 255) {
				gfx->palette_shown[i + 1] += 1;
				not_ended = true;
			} else {
				gfx->palette_shown[i + 1] = 255;
			}
			if ((gfx->palette_shown[i + 2]) < 255) {
				gfx->palette_shown[i + 2] += 1;
				not_ended = true;
			} else {
				gfx->palette_shown[i + 2] = 255;
			}
		}
		VGA_VSync();
		VGA_SetPalette(gfx->palette_shown);
	}
	//	memset(gfx->palette_shown, 255, 255 * 3);
	//	VGA_SetPalette(gfx->palette_shown);
}

/** VGA :: Syncronous fade out
 */
bool VGA_FadeOut_Async(int speed, int *step) {
	volatile int i;
	volatile int sp;
	volatile int max_steps;

	sp = speed;
	if (sp > 4) { sp = 4; }
	if (sp < 1) { sp = 1; }
	max_steps = 8;

	if (*step <= max_steps) {
		for (i = 0; i < 256 * 3; i++) {
			gfx->palette_shown[i] -= gfx->palette_shown[i] >> (max_steps - *step);
			if (gfx->palette_shown[i] < 0) {
				gfx->palette_shown[i] = 0;
			}
		}
		*step = *step + 1;
		return false;
	} else {
		return true;
	}
}

/////////////////////////////////////////////////////////
// Rotate palette sync
// - Rotates pallete of a current image
// - Rotates colors just one by one
/////////////////////////////////////////////////////////
void VGA_RotatePaletteAsync(int index1, int index2) {
	int i;
	int colors;

	unsigned char auxColorR;
	unsigned char auxColorG;
	unsigned char auxColorB;

	int firstIndex = index1 * 3;
	int lastIndex = index2 * 3;
	colors = index2 - index1;

	// first thing first...save last index colour
	auxColorR = gfx->palette_shown[lastIndex];
	auxColorG = gfx->palette_shown[lastIndex + 1];
	auxColorB = gfx->palette_shown[lastIndex + 2];

	// rotate all colors
	for (i = 0; i < colors; i++) {
		gfx->palette_shown[lastIndex - (i * 3)] = gfx->palette_shown[lastIndex - (i * 3) - 3];
		gfx->palette_shown[lastIndex - (i * 3) + 1] = gfx->palette_shown[lastIndex - (i * 3) - 2];
		gfx->palette_shown[lastIndex - (i * 3) + 2] = gfx->palette_shown[lastIndex - (i * 3) - 1];
	}

	// restore last index colour on first index
	gfx->palette_shown[firstIndex] = auxColorR;
	gfx->palette_shown[firstIndex + 1] = auxColorG;
	gfx->palette_shown[firstIndex + 2] = auxColorB;

	VGA_SetPaletteRange(gfx->palette_shown, index1, index2);
}


/////////////////////////////////////////////////////////
// Rotate palette sync
// - Rotates pallete of a current image
// - Stucks CPU no other processes can be done while
//   rotating
/////////////////////////////////////////////////////////
void VGA_RotatePalette(int index1, int index2, int speed) {
	int i, j;
	int iterances, colors;
	int waitcount;
	int waitcounter;

	unsigned char auxColorR;
	unsigned char auxColorG;
	unsigned char auxColorB;

	int firstIndex = index1 * 3;
	int lastIndex = index2 * 3;
	//bytes = lastIndex - firstIndex;
	colors = index2 - index1;
	iterances = index2 - index1;

	waitcount = 100 - speed;

	for (i = 0; i <= iterances; i++) {

		// first thing first...save last index colour
		auxColorR = gfx->palette_shown[lastIndex];
		auxColorG = gfx->palette_shown[lastIndex + 1];
		auxColorB = gfx->palette_shown[lastIndex + 2];

		// rotate all colors
		for (j = 0; j < colors; j++) {
			gfx->palette_shown[lastIndex - (j * 3)] = gfx->palette_shown[lastIndex - (j * 3) - 3];
			gfx->palette_shown[lastIndex - (j * 3) + 1] = gfx->palette_shown[lastIndex - (j * 3) - 2];
			gfx->palette_shown[lastIndex - (j * 3) + 2] = gfx->palette_shown[lastIndex - (j * 3) - 1];
		}

		// restore last index colour on first index
		gfx->palette_shown[firstIndex] = auxColorR;
		gfx->palette_shown[firstIndex + 1] = auxColorG;
		gfx->palette_shown[firstIndex + 2] = auxColorB;

		VGA_SetPalette(gfx->palette_shown);

		waitcounter = 0;
		while (waitcounter < waitcount) {
			waitcounter++;
			VGA_VSync();
		}
	}
}