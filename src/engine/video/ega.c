#include "../engine.h"
#include "ega.h"

byte *EGA;

/** EGA :: Vertical sync
 * 
 */
void EGA_VSync(void) {
}

/** EGA :: Draws screen buffer on Video memory
 *  - Draws an entire screen buffer on video ram
 */
void EGA_ScreenMainBufferToVRAM(void) {

	//disable();//disable interrupts

	memcpy(EGA, video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], EGA_RESOLUTION_WIDTH * EGA_RESOLUTION_HEIGHT);
	//enable();
}

/** EGA :: Draws image buffer data on Video memory
 *  - Draws an entire buffer on video ram 
 */
void EGA_ImageToVRAM(byte *buffer, int width, int height) {
	int i;
	int src_index, dst_index;

	//disable();//disable interrupts

	// Transfer data to video buffer
	src_index = 0;
	dst_index = 0;
	for (i = 0; i < height; i++) {
		memcpy(&EGA[dst_index], &buffer[src_index], width);
		src_index += width;
		dst_index += EGA_RESOLUTION_WIDTH;
	}

	//enable();
}

/** EGA :: Check graphics card compatibility
 *  - Returns true if a EGA compatible card is detected
 */
bool EGA_CheckGraphicsCard(void) {
	return false;
}