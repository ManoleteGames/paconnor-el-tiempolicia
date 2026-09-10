#include "../engine.h"
#include "cga.h"

byte *CGA;

/** CGA :: Vertical sync
 * 
 */
void CGA_VSync(void) {
}

/** CGA :: Check graphics card compatibility
 *  - Returns true if a CGA compatible card is detected
 */
bool CGA_CheckGraphicsCard(void) {
	return false;
}

/** CGA :: Draws screen buffer on Video memory
 *  - Draws an entire screen buffer on video ram 
 */
void CGA_ScreenMainBufferToVRAM(void) {

	//disable();//disable interrupts

	memcpy(CGA, video->screen_buffer[VIDEO_SCREEN_BUFFER_BACK], CGA_RESOLUTION_WIDTH * CGA_RESOLUTION_HEIGHT);

	//enable();
}

/** CGA :: Draws image buffer data on Video memory
 *  - Draws an entire buffer on video ram 
 */
void CGA_ImageToVRAM(byte *buffer, int width, int height) {
	int i;
	int src_index, dst_index;

	//disable();//disable interrupts

	// Transfer data to video buffer
	src_index = 0;
	dst_index = 0;
	for (i = 0; i < height; i++) {
		memcpy(&CGA[dst_index], &buffer[src_index], width);
		src_index += width;
		dst_index += CGA_RESOLUTION_WIDTH;
	}

	//enable();
}