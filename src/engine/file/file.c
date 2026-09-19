#include "../engine.h"
#include "dos.h"
#include "fcntl.h"
#include "string.h"
#include "unistd.h"
#include <stdio.h>
#include "file.h"

/** FILE :: Seek files offset inside a DAT file (custom format similar to WAD)
 *  - File name must be a 17 char max.
 *  - Max. 64 files inside DAT file
 *  - Returns offset (long)
 */
long FILE_GetAssetOffset(int fd, const char *filename) {

	byte line = 0;
	char name[17];//name of the file inside DAT
	bool found = false;
	word result;
	long offset = 0;//LDAT_Offset

	//Seek data
	lseek(fd, 32, SEEK_SET);// Jump first 2 lines of .DAT file

	while (!found) {// Read 16 byte lines of a total of 64 lines
		if (!_dos_read(fd, name, 16, &result)) {
			// If strings matches returns 0
			if (!stricmp(name, filename)) { found = true; }// Returns 0 if strings are equal
		} else {
			_dos_close(fd);
			sprintf(engine.system_error_message1, "FILE_GetAssetOffset function error");
			sprintf(engine.system_error_message2, "Unable to find asset file %s offset", filename);
			sprintf(engine.system_error_message3, "Reading error");
			Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
		}

		line++;
		// last line
		if (line == 64) {
			_dos_close(fd);
			sprintf(engine.system_error_message1, "FILE_GetAssetOffset function error");
			sprintf(engine.system_error_message2, "Unable to find asset file %s offset", filename);
			sprintf(engine.system_error_message3, "Last line reached");
			Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
		}
	}
	_dos_read(fd, &offset, sizeof(offset), &result);
	return offset;
}

/** FILE :: Seek files offset inside a DAT file (custom format similar to WAD)
 *  - File name must be a 17 char max.
 *  - Max. 64 files inside DAT file
 *  - Returns offset (long)
 */
dword FILE_SeekAssetOffset(FILE *fp, const char *filename) {
	unsigned char line = 0;
	char name[17];//name of the file inside DAT
	unsigned char data_name = 0;
	dword offset = 0;//LDAT_Offset

	//Check LDAT file
	//Seek data
	fseek(fp, 32, SEEK_SET);// Check first line of names inside the .DAT

	while (data_name == 0) {// Read 16 byte lines of a total of 64 lines
		memset(name, 0, 17);

		fgets(name, 17, fp);// Get filename

		if (!stricmp(name, filename)) data_name = 1;// Returns 0 if strings are equal
		else
			fseek(fp, 16, SEEK_CUR);// Go to next line

		line++;
		// End line. Just finish returning 0 as offset
		if (line == 64) {
			fclose(fp);
			offset = 0;
			data_name = 2;
			//sprintf(engine.system_error_message1, "FILE_SeekAssetOffset function error");
			//sprintf(engine.system_error_message2, "Unable to find asset file %s offset", filename);
			//sprintf(engine.system_error_message3, "Last line reached");
			//Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
		}
	}

	if (data_name == 1) fread(&offset, sizeof(offset), 1, fp);//read offset of file in DAT file

	return offset;
}

/** FILE :: Seek files offset inside a DAT file (custom format similar to WAD)
 *  - File name must be a 17 char max.
 *  - Max. 64 files inside DAT file
 *  - Returns offset (long)
 */
dword FILE_SeekAssetLength(FILE *fp, const char *filename) {
	unsigned char line = 0;
	char name[17];//name of the file inside DAT
	unsigned char data_name = 0;
	dword offset = 0;// DAT_Offset
	dword length = 0;// data length

	//Check LDAT file
	//Seek data
	fseek(fp, 32, SEEK_SET);// Check first line of names inside the .DAT

	while (data_name == 0) {// Read 16 byte lines of a total of 64 lines
		memset(name, 0, 17);

		fgets(name, 17, fp);// Get filename

		if (!stricmp(name, filename)) data_name = 1;// Returns 0 if strings are equal
		else
			fseek(fp, 16, SEEK_CUR);// Go to next line

		line++;
		if (line == 64) {
			fclose(fp);
			sprintf(engine.system_error_message1, "FILE_SeekAssetOffset function error");
			sprintf(engine.system_error_message2, "Unable to find asset file %s offset", filename);
			sprintf(engine.system_error_message3, "Last line reached");
			Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
		}
	}

	fread(&offset, sizeof(offset), 1, fp);//read offset of file in DAT file
	fread(&length, sizeof(length), 1, fp);//read offset of file in DAT file

	return length;
}

/** FILE :: Loads binary image 
 */
void FILE_LoadBinaryImage(const char *dat_name, const char *asset_name, byte *buffer) {
	int file_handler;
	long offset;
	word bytesRead;

	if (_dos_open(dat_name, O_RDONLY, &file_handler)) {
		sprintf(engine.system_error_message1, "FILE_LoadBinaryImage function error");
		sprintf(engine.system_error_message2, "Unable to open DAT file %s ", dat_name);
		sprintf(engine.system_error_message3, "Asset file %s", asset_name);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_BINARY_FILE);
	}

	offset = FILE_GetAssetOffset(file_handler, asset_name);
	if (offset == 0) {
		_dos_close(file_handler);
		sprintf(engine.system_error_message1, "FILE_LoadBinaryImage function error");
		sprintf(engine.system_error_message2, "Invalid offset for file %s ", asset_name);
		sprintf(engine.system_error_message3, "DAT file %s", dat_name);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_BINARY_FILE);
	}
	// Move pointer to offset
	lseek(file_handler, offset, SEEK_SET);
	_dos_read(file_handler, buffer, 4000, &bytesRead);
	_dos_close(file_handler);
}

/* PCX Image reader
 * - Reads a 256 colors pcx file inside a .DAT file
 * - Image must as maximum as 352x200 pixels
 * - Image must be 8 bits per pixel
 * - Sets image information on inout buffer
*/
void FILE_LoadPCXImage(const char *dat_name, const char *asset_name, byte *buffer, long size, word *width, word *height) {

	int fileHandler;
	long image_size, offset, l;
	int i, cnt;
	word result;
	byte ident;// must be 0x0A for PCX file
	byte version;
	byte encoding;
	byte bitsPerPixel;
	byte chr;
	word xMin, xMax, yMin, yMax, w, h;

	if (_dos_open(dat_name, O_RDONLY, &fileHandler)) {
		sprintf(engine.system_error_message1, "FILE_LoadPCXImage function error");
		sprintf(engine.system_error_message2, "Unable to open DAT file %s ", dat_name);
		sprintf(engine.system_error_message3, "");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}

	offset = FILE_GetAssetOffset(fileHandler, asset_name);
	if (offset == 0) {
		_dos_close(fileHandler);
		sprintf(engine.system_error_message1, "FILE_LoadPCXImage function error");
		sprintf(engine.system_error_message2, "Invalid offset inside DAT file %s ", dat_name);
		sprintf(engine.system_error_message3, "Asset file %s", asset_name);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}
	// Move pointer to offset
	lseek(fileHandler, offset, SEEK_SET);

	//Read PCX header
	_dos_read(fileHandler, &ident, sizeof(ident), &result);
	if (ident != 0x0A) {
		_dos_close(fileHandler);
		sprintf(engine.system_error_message1, "FILE_LoadPCXImage function error");
		sprintf(engine.system_error_message2, "Error found on PCX identifier %x on file %s ", ident, asset_name);
		sprintf(engine.system_error_message3, "Ident found %x, ident expected 0x0A", ident);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}
	_dos_read(fileHandler, &version, sizeof(version), &result);
	_dos_read(fileHandler, &encoding, sizeof(encoding), &result);

	if (encoding != 1) {
		_dos_close(fileHandler);
		sprintf(engine.system_error_message1, "FILE_LoadPCXImage function error");
		sprintf(engine.system_error_message2, "Not RLE encoding file %s ", asset_name);
		sprintf(engine.system_error_message3, "Encoding found %u, encoding expected 1", encoding);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}
	_dos_read(fileHandler, &bitsPerPixel, sizeof(bitsPerPixel), &result);
	if (bitsPerPixel != 8) {
		_dos_close(fileHandler);
		sprintf(engine.system_error_message1, "FILE_LoadPCXImage function error");
		sprintf(engine.system_error_message2, "Not 8 bits per pixel image file %s ", asset_name);
		sprintf(engine.system_error_message3, "bpx found %u, bpx expected 8", bitsPerPixel);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}
	xMin = 0;
	_dos_read(fileHandler, &xMin, 2, &result);
	yMin = 0;
	_dos_read(fileHandler, &yMin, 2, &result);
	xMax = 0;
	_dos_read(fileHandler, &xMax, 2, &result);
	yMax = 0;
	_dos_read(fileHandler, &yMax, 2, &result);

	// Calculate image width and heigth
	w = xMax - xMin + 1;
	*width = w;
	h = yMax - yMin + 1;
	*height = h;

	image_size = (long) w * h;

	if (image_size > size) {
		_dos_close(fileHandler);
		sprintf(engine.system_error_message1, "FILE_LoadPCXImage function error");
		sprintf(engine.system_error_message2, "Excesive image size of file %s ", asset_name);
		sprintf(engine.system_error_message3, "Image size %ld, Max. expected %ld", image_size, size);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}

	// Skip some data with no use in this case, like hDpi, vDpi, EGA palette, planes, bytes per line, palette info, device width and device heigth
	lseek(fileHandler, 116, SEEK_CUR);

	// Get image data
	l = 0;
	while (l < image_size) {
		// Get value
		_dos_read(fileHandler, &chr, sizeof(chr), &result);
		if (0xC0 == (0xC0 & chr)) {// is it a RLE repeater
			cnt = 0x3F & chr;      // Get count
			_dos_read(fileHandler, &chr, sizeof(chr), &result);
			for (i = 0; i < cnt; i++) {
				buffer[l] = chr;
				l++;
			}
		} else {// not a RLE...just a single color
			buffer[l] = chr;
			l++;
		}
	}

	_dos_close(fileHandler);
}

/* PCX Tileset
 * - Reads a 256 colors pcx file inside a .DAT file
 * - Image must as maximum as 352x416 pixels
 * - Image must be 8 bits per pixel
 * - Sets image information on inout buffer
*/
void FILE_LoadPCXTileset(const char *dat_name, const char *asset_name, byte *buffer, long size, word *width, word *height) {

	int fileHandler;
	dword image_size, offset, l;
	int i, cnt;
	word result;
	byte ident;// must be 0x0A for PCX file
	byte version;
	byte encoding;
	byte bitsPerPixel;
	byte chr;
	word xMin, xMax, yMin, yMax, w, h;

	if (_dos_open(dat_name, O_RDONLY, &fileHandler)) {
		sprintf(engine.system_error_message1, "FILE_LoadPCXTileset function error");
		sprintf(engine.system_error_message2, "Unable to open DAT file %s ", dat_name);
		sprintf(engine.system_error_message3, "");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}

	offset = FILE_GetAssetOffset(fileHandler, asset_name);
	if (offset == 0) {
		_dos_close(fileHandler);
		sprintf(engine.system_error_message1, "FILE_LoadPCXTileset function error");
		sprintf(engine.system_error_message2, "Invalid offset inside DAT file %s ", dat_name);
		sprintf(engine.system_error_message3, "Asset file %s", asset_name);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}

	// Move pointer to offset
	lseek(fileHandler, offset, SEEK_SET);

	//Read PCX header
	_dos_read(fileHandler, &ident, sizeof(ident), &result);
	if (ident != 0x0A) {
		_dos_close(fileHandler);
		sprintf(engine.system_error_message1, "FILE_LoadPCXImage function error");
		sprintf(engine.system_error_message2, "Error found on PCX identifier %x on file %s ", ident, asset_name);
		sprintf(engine.system_error_message3, "Ident found %x, ident expected 0x0A", ident);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}
	_dos_read(fileHandler, &version, sizeof(version), &result);
	_dos_read(fileHandler, &encoding, sizeof(encoding), &result);

	if (encoding != 1) {
		_dos_close(fileHandler);
		sprintf(engine.system_error_message1, "FILE_LoadPCXImage function error");
		sprintf(engine.system_error_message2, "Not RLE encoding file %s ", asset_name);
		sprintf(engine.system_error_message3, "Encoding found %u, encoding expected 1", encoding);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}
	_dos_read(fileHandler, &bitsPerPixel, sizeof(bitsPerPixel), &result);
	if (bitsPerPixel != 8) {
		_dos_close(fileHandler);
		sprintf(engine.system_error_message1, "FILE_LoadPCXImage function error");
		sprintf(engine.system_error_message2, "Not 8 bits per pixel image file %s ", asset_name);
		sprintf(engine.system_error_message3, "bpx found %u, bpx expected 8", bitsPerPixel);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}
	xMin = 0;
	_dos_read(fileHandler, &xMin, 2, &result);
	yMin = 0;
	_dos_read(fileHandler, &yMin, 2, &result);
	xMax = 0;
	_dos_read(fileHandler, &xMax, 2, &result);
	yMax = 0;
	_dos_read(fileHandler, &yMax, 2, &result);

	// Calculate image width and heigth
	w = xMax - xMin + 1;
	*width = w;
	h = yMax - yMin + 1;
	*height = h;

	if (w > 320) {
		_dos_close(fileHandler);
		sprintf(engine.system_error_message1, "FILE_LoadPCXImage function error");
		sprintf(engine.system_error_message2, "Excesive width on image file %s ", asset_name);
		sprintf(engine.system_error_message3, "Image width %u, Max. expected 320", w);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}
	if (h > 416) {
		_dos_close(fileHandler);
		sprintf(engine.system_error_message1, "FILE_LoadPCXImage function error");
		sprintf(engine.system_error_message2, "Excesive height on image file %s ", asset_name);
		sprintf(engine.system_error_message3, "Image height %u, Max. expected 200", h);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}
	image_size = (long) w * h;

	if (image_size > size) {
		_dos_close(fileHandler);
		sprintf(engine.system_error_message1, "FILE_LoadPCXImage function error");
		sprintf(engine.system_error_message2, "Excesive image size of file %s ", asset_name);
		sprintf(engine.system_error_message3, "Image size %ld, Max. expected %ld", image_size, size);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}

	// Skip some data with no use in this case, like hDpi, vDpi, EGA palette, planes, bytes per line, palette info, device width and device heigth
	lseek(fileHandler, 116, SEEK_CUR);

	// Get image data
	l = 0;
	while (l < image_size) {
		// Get value
		_dos_read(fileHandler, &chr, sizeof(chr), &result);
		if (0xC0 == (0xC0 & chr)) {// is it a RLE repeater
			cnt = 0x3F & chr;      // Get count
			_dos_read(fileHandler, &chr, sizeof(chr), &result);
			for (i = 0; i < cnt; i++) {
				buffer[l] = chr;
				l++;
			}
		} else {// not a RLE...just a single color
			buffer[l] = chr;
			l++;
		}
	}

	_dos_close(fileHandler);
}
/** FILE :: Load PCX Sprite
 *  - Loads PCX data from a image file inside DAT file and transfer it to a buffer
 *  - inputs:
 *     o DAT filename
 *     o PCX filename inside DAT file
 *     o Graphics buffer address
 *     o Graphics file size (file width * file height) in pixels
 *     o Palette color offset
 *  - inouts
 *     o Image width in pixels
 *     o Image height in pixels 
 */
void FILE_LoadPCXSprite(const char *dat_name, const char *asset_name, byte *buffer, long size, word *width, word *height, int palette_offset) {

	int fileHandler;
	long image_size, offset;
	int i, l, cnt;
	word result;
	byte ident;// must be 0x0A for PCX file
	byte version, encoding, bitsPerPixel;
	byte chr;
	word xMin, xMax, yMin, yMax, w, h;

	if (_dos_open(dat_name, O_RDONLY, &fileHandler)) {
		sprintf(engine.system_error_message1, "FILE_LoadPCXSprite function error");
		sprintf(engine.system_error_message2, "Unable to open DAT file %s ", dat_name);
		sprintf(engine.system_error_message3, "");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}

	offset = FILE_GetAssetOffset(fileHandler, asset_name);
	if (offset == 0) {
		_dos_close(fileHandler);
		sprintf(engine.system_error_message1, "FILE_LoadPCXSprite function error");
		sprintf(engine.system_error_message2, "Unable to find offset inside DAT file %s ", dat_name);
		sprintf(engine.system_error_message3, "Asset file %s", asset_name);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}
	// Move pointer to offset
	lseek(fileHandler, offset, SEEK_SET);

	//Read PCX header
	_dos_read(fileHandler, &ident, sizeof(ident), &result);
	if (ident != 0x0A) {
		_dos_close(fileHandler);
		sprintf(engine.system_error_message1, "FILE_LoadPCXSprite function error");
		sprintf(engine.system_error_message2, "Error found on PCX identifier %x on file %s ", ident, asset_name);
		sprintf(engine.system_error_message3, "Ident found %x, ident expected 0x0A", ident);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}
	_dos_read(fileHandler, &version, sizeof(version), &result);
	_dos_read(fileHandler, &encoding, sizeof(encoding), &result);

	if (encoding != 1) {
		_dos_close(fileHandler);
		sprintf(engine.system_error_message1, "FILE_LoadPCXSprite function error");
		sprintf(engine.system_error_message2, "Not RLE encoding file %s ", asset_name);
		sprintf(engine.system_error_message3, "Encoding found %u, encoding expected 1", encoding);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}
	_dos_read(fileHandler, &bitsPerPixel, sizeof(bitsPerPixel), &result);
	if (bitsPerPixel != 8) {
		_dos_close(fileHandler);
		sprintf(engine.system_error_message1, "FILE_LoadPCXSprite function error");
		sprintf(engine.system_error_message2, "Not 8 bits per pixel image file %s ", asset_name);
		sprintf(engine.system_error_message3, "bpx found %u, bpx expected 8", bitsPerPixel);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}
	xMin = 0;
	_dos_read(fileHandler, &xMin, 2, &result);
	yMin = 0;
	_dos_read(fileHandler, &yMin, 2, &result);
	xMax = 0;
	_dos_read(fileHandler, &xMax, 2, &result);
	yMax = 0;
	_dos_read(fileHandler, &yMax, 2, &result);

	// Calculate image width and heigth
	w = xMax - xMin + 1;
	*width = w;
	h = yMax - yMin + 1;
	*height = h;

	image_size = (long) w * h;

	if (image_size > size) {
		_dos_close(fileHandler);
		sprintf(engine.system_error_message1, "FILE_LoadPCXSprite function error");
		sprintf(engine.system_error_message2, "Excesive image size of file %s ", asset_name);
		sprintf(engine.system_error_message3, "Image size %ld, Max. expected %ld", image_size, size);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}

	// Skip some data with no use in this case, like hDpi, vDpi, EGA palette, planes, bytes per line, palette info, device width and device heigth
	lseek(fileHandler, 116, SEEK_CUR);

	// Get image data
	l = 0;
	while (l < image_size) {
		// Get value
		_dos_read(fileHandler, &chr, sizeof(chr), &result);
		if (0xC0 == (0xC0 & chr)) {// is it a RLE repeater
			cnt = 0x3F & chr;      // Get count
			_dos_read(fileHandler, &chr, sizeof(chr), &result);
			for (i = 0; i < cnt; i++) {
				buffer[l] = chr + palette_offset;
				l++;
			}
		} else {// not a RLE...just a single color
			buffer[l] = chr + palette_offset;
			l++;
		}
	}

	_dos_close(fileHandler);
}

/** FILE :: Load map
 *  - File format CSV
 */
void FILE_LoadMap_CSV(const char *dat_name, unsigned short *back, unsigned short *fore, unsigned short *mask, unsigned short *col, unsigned short *anim, unsigned short *event_hspot, int size) {
	FILE *f;
	int tile;
	word index;
	dword offset;

	// Open DAT file and search TMX file inside
	f = fopen(dat_name, "rb");
	if (!f) {
		sprintf(engine.system_error_message1, "FILE_LoadMap_CSV function error");
		sprintf(engine.system_error_message2, "Unable to open DAT file %s ", dat_name);
		sprintf(engine.system_error_message3, "");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}
	// Load background tiles
	offset = FILE_SeekAssetOffset(f, "m_back.csv");
	fseek(f, offset, SEEK_SET);// Set file pointer at the begining of the file
	// Read layer data
	for (index = 0; index < size; index++) {
		fscanf(f, "%d,", &tile);// get the tile number
		if (tile != -1) back[index] = tile;
		else
			back[index] = 0;
	}

	// Load foreground tiles
	offset = FILE_SeekAssetOffset(f, "m_fore.csv");
	fseek(f, offset, SEEK_SET);// Set file pointer at the begining of the file
	// Read layer data
	for (index = 0; index < size; index++) {
		fscanf(f, "%d,", &tile);// get the tile number
		if (tile != -1) fore[index] = tile;
		else
			fore[index] = 0;
	}

	// Load mask tiles
	offset = FILE_SeekAssetOffset(f, "m_mask.csv");
	fseek(f, offset, SEEK_SET);// Set file pointer at the begining of the file
	// Read layer data
	for (index = 0; index < size; index++) {
		fscanf(f, "%d,", &tile);// get the tile number
		if (tile != -1) mask[index] = tile;
		else
			mask[index] = 0;
	}

	// Load colission tiles
	offset = FILE_SeekAssetOffset(f, "m_col.csv");
	fseek(f, offset, SEEK_SET);// Set file pointer at the begining of the file
	// Read layer data
	for (index = 0; index < size; index++) {
		fscanf(f, "%d,", &tile);// get the tile number
		if (tile != -1) col[index] = tile;
		else
			col[index] = 0;
	}

	// Load animation tiles
	offset = FILE_SeekAssetOffset(f, "m_anim.csv");
	fseek(f, offset, SEEK_SET);// Set file pointer at the begining of the file
	// Read layer data
	for (index = 0; index < size; index++) {
		fscanf(f, "%d,", &tile);// get the tile number
		if (tile != -1) anim[index] = tile;
		else
			anim[index] = 0;
	}

	// Load event tiles
	offset = FILE_SeekAssetOffset(f, "m_event.csv");
	fseek(f, offset, SEEK_SET);// Set file pointer at the begining of the file
	// Read layer data
	for (index = 0; index < size; index++) {
		fscanf(f, "%d,", &tile);// get the tile number
		if (tile != -1) event_hspot[index] = ((tile & 0xFF) + 1) << 8;
		else
			event_hspot[index] = 0;
	}

	// Load hotspot tiles
	offset = FILE_SeekAssetOffset(f, "m_hspot.csv");
	fseek(f, offset, SEEK_SET);// Set file pointer at the begining of the file
	// Read layer data
	for (index = 0; index < size; index++) {
		fscanf(f, "%d,", &tile);// get the tile number
		if (tile != -1) event_hspot[index] = event_hspot[index] | ((tile & 0xFF) + 1);
		else
			event_hspot[index] = event_hspot[index] & 0xFF00;
	}


	fclose(f);
}

/** FILE :: Load palette
 *  - Palette format PCX
 */
void FILE_LoadPCXPalette(const char *dat_name, const char *asset_name, byte *buffer, long size) {
	int fileHandler;
	long offset;
	int i, j;
	word result;
	byte ident;// must be 0x0A for PCX file
	byte version;
	byte encoding;
	byte bitsPerPixel;
	byte color;
	byte chr;

	if (_dos_open(dat_name, O_RDONLY, &fileHandler)) {
		sprintf(engine.system_error_message1, "FILE_LoadPCXPalette function error");
		sprintf(engine.system_error_message2, "Unable to open DAT file %s ", dat_name);
		sprintf(engine.system_error_message3, "");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}

	offset = FILE_GetAssetOffset(fileHandler, asset_name);
	if (offset == 0) {
		_dos_close(fileHandler);
		sprintf(engine.system_error_message1, "FILE_LoadPCXPalette function error");
		sprintf(engine.system_error_message2, "Unable to find offset inside DAT file %s ", dat_name);
		sprintf(engine.system_error_message3, "Asset file %s", asset_name);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}
	// Move pointer to offset
	lseek(fileHandler, offset, SEEK_SET);

	//Read PCX header
	_dos_read(fileHandler, &ident, sizeof(ident), &result);
	if (ident != 0x0A) {
		_dos_close(fileHandler);
		sprintf(engine.system_error_message1, "FILE_LoadPCXPalette function error");
		sprintf(engine.system_error_message2, "Error found on PCX identifier %x on file %s ", ident, asset_name);
		sprintf(engine.system_error_message3, "Ident found %x, ident expected 0x0A", ident);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}
	_dos_read(fileHandler, &version, sizeof(version), &result);
	_dos_read(fileHandler, &encoding, sizeof(encoding), &result);

	if (encoding != 1) {
		_dos_close(fileHandler);
		sprintf(engine.system_error_message1, "FILE_LoadPCXPalette function error");
		sprintf(engine.system_error_message2, "Not RLE encoding file %s ", asset_name);
		sprintf(engine.system_error_message3, "Encoding found %u, encoding expected 1", encoding);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}
	_dos_read(fileHandler, &bitsPerPixel, sizeof(bitsPerPixel), &result);
	if (bitsPerPixel != 8) {
		_dos_close(fileHandler);
		sprintf(engine.system_error_message1, "FILE_LoadPCXPalette function error");
		sprintf(engine.system_error_message2, "Not 8 bits per pixel image file %s ", asset_name);
		sprintf(engine.system_error_message3, "bpx found %u, bpx expected 8", bitsPerPixel);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}

	// Jump to special char identifier
	lseek(fileHandler, 444, SEEK_CUR);
	_dos_read(fileHandler, &chr, 1, &result);

	if (chr != 0x0C) {
		_dos_close(fileHandler);
		sprintf(engine.system_error_message1, "FILE_LoadPCXPalette function error");
		sprintf(engine.system_error_message2, "Error trying to find special char on image file %s ", asset_name);
		sprintf(engine.system_error_message3, "Char found %u, char expected 0x0C", chr);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}

	for (i = 0; i < size; i++) {
		for (j = 0; j < 3; j++) {
			if (_dos_read(fileHandler, &color, 1, &result)) {
				_dos_close(fileHandler);
				sprintf(engine.system_error_message1, "FILE_LoadPCXPalette function error");
				sprintf(engine.system_error_message2, "Error reading palette color on file %s ", asset_name);
				sprintf(engine.system_error_message3, "Palette color number %u", i);
				Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
			}
			if (color == EOF) {
				_dos_close(fileHandler);
				sprintf(engine.system_error_message1, "FILE_LoadPCXImage function error");
				sprintf(engine.system_error_message2, "Error reading palette color on file %s ", asset_name);
				sprintf(engine.system_error_message3, "EOF found when reading palette color number %u", i);
				Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
			} else {
				*buffer++ = color >> 2;
			}
		}
	}

	_dos_close(fileHandler);
}

/** FILE :: Load text from txt file
 * - Reads a TXT file inside a .DAT file
 * - Text lines must start with '#<line_number>'
 * - Text lines must finish with '$'
 */
void FILE_LoadTextFile(const char *dat_name, const char *asset_name, TextFile *tf) {
	FILE *f;
	bool eof;
	word line_readed;
	int line_counter;
	dword offset;

	// Open DAT file and search txt file inside
	f = fopen(dat_name, "rb");
	if (!f) {
		sprintf(engine.system_error_message1, "FILE_LoadTextFile function error");
		sprintf(engine.system_error_message2, "Unable to open DAT file %s ", dat_name);
		sprintf(engine.system_error_message3, "");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}

	// Search asset file
	offset = FILE_SeekAssetOffset(f, asset_name);
	fseek(f, offset, SEEK_SET);// Set file pointer at the begining of the file
	if (offset == 0) {
		fclose(f);
		sprintf(engine.system_error_message1, "FILE_LoadTextFile function error");
		sprintf(engine.system_error_message2, "Unable to find offset inside DAT file %s ", dat_name);
		sprintf(engine.system_error_message3, "Asset file %s", asset_name);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}

	eof = false;
	line_counter = 0;

	while (!eof) {
		fscanf(f, "#%d#", &line_readed);
		// Check EOF
		if ((line_readed < 1) || (line_readed > 355)) {// EOF
			eof = true;
		} else {
			fgets(tf->line[line_readed], 40, f);
		}

		if (line_readed == 356) {// EOF
			eof = true;
		}

		line_counter++;
		if (line_counter == 356) {// EOF
			eof = true;
		}
	}

	fclose(f);
}

/** FILE :: Load animation data from ani file
 * - Reads an .ANI file inside a .DAT file 
 */
void FILE_LoadAnimationFile(const char *dat_name, const char *asset_name, SpriteAnimation *ani, int max_animations) {
	FILE *f;
	bool eof;
	word line_readed;
	int line_counter;
	dword offset;
	int i, loop, inverted, frames, speed;
	int frame, offsetx, offsety;

	// Open DAT file and search txt file inside
	f = fopen(dat_name, "rb");
	if (!f) {
		sprintf(engine.system_error_message1, "FILE_LoadAnimationFile function error");
		sprintf(engine.system_error_message2, "Unable to open DAT file %s ", dat_name);
		sprintf(engine.system_error_message3, "");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}

	// Search asset file
	offset = FILE_SeekAssetOffset(f, asset_name);
	fseek(f, offset, SEEK_SET);// Set file pointer at the begining of the file
	if (offset == 0) {
		fclose(f);
		sprintf(engine.system_error_message1, "FILE_LoadAnimationFile function error");
		sprintf(engine.system_error_message2, "Unable to find offset inside DAT file %s ", dat_name);
		sprintf(engine.system_error_message3, "Asset file %s", asset_name);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}

	eof = false;
	line_counter = 0;

	while (!eof) {
		fscanf(f, "#%d#", &line_readed);

		// Check EOF
		if ((line_readed < 1) || (line_readed > 255)) {// EOF
			sprintf(engine.system_error_message1, "FILE_LoadAnimationFile function error");
			sprintf(engine.system_error_message2, "Wrong line readed %u on file %s ", line_readed, dat_name);
			sprintf(engine.system_error_message3, "Asset file %s", asset_name);
			Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
		}
		// Check EOF
		if (line_readed == 255) {// EOF
			sprintf(engine.system_error_message1, "FILE_LoadAnimationFile function error");
			sprintf(engine.system_error_message2, "Last line reached on file %s ", dat_name);
			sprintf(engine.system_error_message3, "Asset file %s", asset_name);
			Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
		}

		if (line_readed == max_animations) {
			eof = true;
		}

		// Read general parameters
		fscanf(f, " %d,%d,%d,%d ", &loop, &inverted, &frames, &speed);
		if (frames > 20) {
			sprintf(engine.system_error_message1, "FILE_LoadAnimationFile function error");
			sprintf(engine.system_error_message2, "Excessive animations %u found in %s ", frames, dat_name);
			sprintf(engine.system_error_message3, "Asset file %s, line %u", asset_name, line_readed);
			Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
		}

		// Set values
		ani[line_readed].loop = loop;
		ani[line_readed].inverted = inverted;
		ani[line_readed].frames = frames;
		ani[line_readed].speed = speed;
		ani[line_readed].current_frame = 0;

		// Read frame values
		for (i = 0; i < frames; i++) {
			fscanf(f, " %d,%d,%d ", &frame, &offsetx, &offsety);
			ani[line_readed].anim_data[i].frame = frame;
			ani[line_readed].anim_data[i].offset_x = offsetx;
			ani[line_readed].anim_data[i].offset_y = offsety;
		}

		line_counter++;
		// Check EOF
		if (line_counter == 255) {// EOF
			sprintf(engine.system_error_message1, "FILE_LoadAnimationFile function error");
			sprintf(engine.system_error_message2, "Line counter overrun on file %s ", dat_name);
			sprintf(engine.system_error_message3, "Asset file %s", asset_name);
			Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
		}
	}

	fclose(f);
}

/** FILE :: Load sprite configuration data from cfg file
 * - Reads a .CFG file inside a .DAT file 
 */
void FILE_LoadSpriteConfigFile(const char *dat_name, const char *asset_name, SpriteConfig *cfg) {
	FILE *f;
	bool eof;
	word line_readed;
	int line_counter;
	dword offset;
	int width, height, frames;

	// Open DAT file and search txt file inside
	f = fopen(dat_name, "rb");
	if (!f) {
		sprintf(engine.system_error_message1, "FILE_LoadSpriteConfigFile function error");
		sprintf(engine.system_error_message2, "Unable to open DAT file %s ", dat_name);
		sprintf(engine.system_error_message3, "");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}

	// Search asset file
	offset = FILE_SeekAssetOffset(f, asset_name);
	fseek(f, offset, SEEK_SET);// Set file pointer at the begining of the file
	if (offset == 0) {
		fclose(f);
		sprintf(engine.system_error_message1, "FILE_LoadSpriteConfigFile function error");
		sprintf(engine.system_error_message2, "Unable to find offset inside DAT file %s ", dat_name);
		sprintf(engine.system_error_message3, "Asset file %s", asset_name);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}

	eof = false;
	line_counter = 0;

	while (!eof) {
		fscanf(f, "#%d#", &line_readed);

		// Check EOF
		if ((line_readed < 1) || (line_readed > 355)) {// EOF
			sprintf(engine.system_error_message1, "FILE_LoadSpriteConfigFile function error");
			sprintf(engine.system_error_message2, "Wrong line readed %u on file %s ", line_readed, dat_name);
			sprintf(engine.system_error_message3, "Asset file %s", asset_name);
			Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
		}
		// Check EOF
		if (line_readed == 356) {// EOF
			sprintf(engine.system_error_message1, "FILE_LoadSpriteConfigFile function error");
			sprintf(engine.system_error_message2, "Last line reached on file %s ", dat_name);
			sprintf(engine.system_error_message3, "Asset file %s", asset_name);
			Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
		}

		if (line_readed == 355) {
			eof = true;
		}

		// Read configuration parameters
		fscanf(f, " %d,%d,%d ", &width, &height, &frames);

		// Set values
		switch (line_readed) {
			case 1:// General dimensions
				cfg->width = width;
				cfg->height = height;
				break;
			case 2:// Face dimensions
				cfg->face_width = width;
				cfg->face_height = height;
				cfg->face_frames = frames;
				break;
			case 3:// Portait dimensions
				cfg->portait_width = width;
				cfg->portait_height = height;
				cfg->portait_frames = frames;
				break;
			case 4:// Feet dimensions
				cfg->feet_width = width;
				cfg->feet_height = height;
				cfg->feet_frames = frames;
				break;
			case 5:// Body dimensions
				cfg->body_width = width;
				cfg->body_height = height;
				cfg->body_frames = frames;
				break;
			case 6:// Head dimensions
				cfg->head_width = width;
				cfg->head_height = height;
				cfg->head_frames = frames;
				break;
			case 7:// Left arm dimensions
				cfg->larm_width = width;
				cfg->larm_height = height;
				cfg->larm_frames = frames;
				break;
			case 8:// Right arm dimensions
				cfg->rarm_width = width;
				cfg->rarm_height = height;
				cfg->rarm_frames = frames;
				break;
			default:
				break;
		}

		line_counter++;
		// Check EOF
		if (line_counter == 13) {// EOF
			sprintf(engine.system_error_message1, "FILE_LoadSpriteConfigFile function error");
			sprintf(engine.system_error_message2, "Line counter overrun on file %s ", dat_name);
			sprintf(engine.system_error_message3, "Asset file %s", asset_name);
			Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
		}
	}

	fclose(f);
}

byte *FILE_LoadA2MSongInfo(const char *dat_name, const char *asset_name, dword *size, int mem_type) {

	FILE *f;
	dword offset;
	dword filesize;
	byte *buffer;

	// Open DAT file and search txt file inside
	f = fopen(dat_name, "rb");
	if (!f) {
		sprintf(engine.system_error_message1, "FILE_LoadA2MSongInfo function error");
		sprintf(engine.system_error_message2, "Unable to open DAT file %s ", dat_name);
		sprintf(engine.system_error_message3, "");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}

	// Search asset file
	offset = FILE_SeekAssetOffset(f, asset_name);
	if (offset == 0) {
		fclose(f);
		sprintf(engine.system_error_message1, "FILE_LoadA2MSongInfo function error");
		sprintf(engine.system_error_message2, "Unable to find offset inside DAT file %s ", dat_name);
		sprintf(engine.system_error_message3, "Asset file %s", asset_name);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}

	// Get file length
	filesize = FILE_SeekAssetLength(f, asset_name);
	if (filesize == 0) {
		fclose(f);
		sprintf(engine.system_error_message1, "FILE_LoadA2MSongInfo function error");
		sprintf(engine.system_error_message2, "Unable to find length inside DAT file %s ", dat_name);
		sprintf(engine.system_error_message3, "Asset file %s", asset_name);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}
	*size = filesize;
	fseek(f, offset, SEEK_SET);// Set file pointer at the begining of the file

	buffer = MM_PushChunk(filesize, mem_type);

	// read file data
	fread(buffer, filesize, 1, f);
	fclose(f);

	return buffer;
}

/** FILE :: Load settigs data from cfg file
 */
void FILE_LoadSettingsFile(const char *config_file) {
	FILE *f;
	bool eof;
	word line_readed;
	int line_counter;
	int dummy;

	// Open settings file and search parameters
	f = fopen(config_file, "rb");
	if (!f) {

		// Unable to open settings file.
		// Just create it with default settings
		settings.video_mode = 0;  // VGA
		settings.sound_device = 1;// PCSpeaker
		settings.sound_volume = 50;
		settings.scenes_music = 1;// ON
		settings.ingame_music = 1;// ON
		settings.music_volume = 50;
		settings.mouse_enabled = 1;// ON
		settings.language = 1;     // EN

		settings.up_key = 17;   // W
		settings.down_key = 31; // S
		settings.left_key = 30; // A
		settings.right_key = 32;// D

		settings.loop_key = 57;      // SPACE
		settings.fire_key = 11;      //
		settings.throw_key = 28;     //
		settings.change_gun_key = 15;// TAB

		FILE_SaveSettingsFile(config_file);
	}

	f = fopen(config_file, "rb");
	if (!f) {
		sprintf(engine.system_error_message1, "FILE_LoadSettingsFile function error");
		sprintf(engine.system_error_message2, "Unable to open settings file %s ", config_file);
		sprintf(engine.system_error_message3, "");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}

	// Set pointer at the begining
	fseek(f, 0, SEEK_SET);// Set file pointer at the begining of the file

	eof = false;
	line_counter = 0;

	while (!eof) {
		fscanf(f, "#%d#", &line_readed);

		switch (line_readed) {
			case 1:// Video mode
				fscanf(f, " %d,", &settings.video_mode);
				break;
			case 2:// Sound device
				fscanf(f, " %d,", &settings.sound_device);
				break;
			case 3:// Sound volume
				fscanf(f, " %d,", &settings.sound_volume);
				break;
			case 4:// Music on scenes
				fscanf(f, " %d,", &settings.scenes_music);
				break;
			case 5:// Music ingame
				fscanf(f, " %d,", &settings.ingame_music);
				break;
			case 6:// Music volume
				fscanf(f, " %d,", &settings.music_volume);
				break;
			case 7:// Mouse enabled
				fscanf(f, " %d,", &settings.mouse_enabled);
				break;
			case 8:// Language
				fscanf(f, " %d,", &settings.language);
				break;
			case 10:// Up key
				fscanf(f, " %d,", &settings.up_key);
				break;
			case 11:// Down key
				fscanf(f, " %d,", &settings.down_key);
				break;
			case 12:// Left key
				fscanf(f, " %d,", &settings.left_key);
				break;
			case 13:// Right key
				fscanf(f, " %d,", &settings.right_key);
				break;
			case 14:// Loop key
				fscanf(f, " %d,", &settings.loop_key);
				break;
			case 15:// Fire key
				fscanf(f, " %d,", &settings.fire_key);
				break;
			case 16:// Throw key
				fscanf(f, " %d,", &settings.throw_key);
				break;
			case 17:// Change gun key
				fscanf(f, " %d,", &settings.change_gun_key);
				break;
			default:
				fscanf(f, " %d,", &dummy);
				break;
		}

		// Check EOF
		if ((line_readed < 1) || (line_readed > 255)) {// EOF
			sprintf(engine.system_error_message1, "FILE_LoadSettingsFile function error");
			sprintf(engine.system_error_message2, "Wrong line readed %u on file %s ", line_readed, config_file);
			sprintf(engine.system_error_message3, "");
			Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
		}

		// Check EOF
		if (line_readed == 20) {// EOF
			eof = true;
		}

		line_counter++;
		// Check EOF
		if (line_counter == 255) {// EOF
			sprintf(engine.system_error_message1, "FILE_LoadSettingsFile function error");
			sprintf(engine.system_error_message2, "Line counter overrun on file %s ", config_file);
			sprintf(engine.system_error_message3, "");
			Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
		}
	}

	fclose(f);
}

/** FILE :: Save settigs data to cfg file
 */
void FILE_SaveSettingsFile(const char *config_file) {
	FILE *f;

	// Open settings file and search parameters
	f = fopen(config_file, "w");
	if (!f) {
		sprintf(engine.system_error_message1, "FILE_SaveSettingsFile function error");
		sprintf(engine.system_error_message2, "Unable to open settings file %s ", config_file);
		sprintf(engine.system_error_message3, "");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}

	// Set pointer at the begining
	fseek(f, 0, SEEK_SET);// Set file pointer at the begining of the file

	fprintf(f, "#001# %u,\n", settings.video_mode);
	fprintf(f, "#002# %u,\n", settings.sound_device);
	fprintf(f, "#003# %u,\n", settings.sound_volume);
	fprintf(f, "#004# %u,\n", settings.scenes_music);
	fprintf(f, "#005# %u,\n", settings.ingame_music);
	fprintf(f, "#006# %u,\n", settings.music_volume);
	fprintf(f, "#007# %u,\n", settings.mouse_enabled);
	fprintf(f, "#008# %u,\n", settings.language);
	fprintf(f, "#009# 0,\n");
	fprintf(f, "#010# %u,\n", settings.up_key);
	fprintf(f, "#011# %u,\n", settings.down_key);
	fprintf(f, "#012# %u,\n", settings.left_key);
	fprintf(f, "#013# %u,\n", settings.right_key);
	fprintf(f, "#014# %u,\n", settings.loop_key);
	fprintf(f, "#015# %u,\n", settings.fire_key);
	fprintf(f, "#016# %u,\n", settings.throw_key);
	fprintf(f, "#017# %u,\n", settings.change_gun_key);
	fprintf(f, "#018# 0,\n");
	fprintf(f, "#019# 0,\n");
	fprintf(f, "#020# 0,\n");
	fprintf(f, "#255# 0,\n");
	fprintf(f, "#255# EOF,\n");
	fprintf(f, "#999# #INFO>> Param 1 : Video mode = VGA(0), EGA(1), CGA(2)\n");
	fprintf(f, "#999# #INFO>> Param 2 : Sound = OFF(0), SPK(1), ADLIB(2), SBLASTER(3)\n");
	fprintf(f, "#999# #INFO>> Param 3 : Sound Volume = 0..100\n");
	fprintf(f, "#999# #INFO>> Param 4 : Music = OFF(0), SPK(1), ADLIB(2), SBLASTER(3)\n");
	fprintf(f, "#999# #INFO>> Param 5 : Music (ingame) = OFF(0), ON(1)\n");
	fprintf(f, "#999# #INFO>> Param 6 : Music Volume = 0..100\n");
	fprintf(f, "#999# #INFO>> Param 7 : Mouse= Disable(0), Enable(1)\n");
	fprintf(f, "#999# #INFO>> Param 8 : Language = SP(0), EN(1), FR(2), GR(3)\n");
	fprintf(f, "#999# #INFO>> Param 9 : Spare\n");
	fprintf(f, "#999# #INFO>> Param 10 : Up key\n");
	fprintf(f, "#999# #INFO>> Param 11 : Down key\n");
	fprintf(f, "#999# #INFO>> Param 12 : Left key\n");
	fprintf(f, "#999# #INFO>> Param 13 : Right key\n");
	fprintf(f, "#999# #INFO>> Param 14 : Loop key\n");
	fprintf(f, "#999# #INFO>> Param 15 : Fire key\n");
	fprintf(f, "#999# #INFO>> Param 16 : Throw key\n");
	fprintf(f, "#999# #INFO>> Param 17 : Change gun key\n");
	fprintf(f, "#999# #INFO>> Param 18 : Spare\n");
	fprintf(f, "#999# #INFO>> Param 19 : Spare\n");
	fprintf(f, "#999# #INFO>> Param 20 : Spare\n");

	fclose(f);
}

void FILE_LoadSpeakerSong(const char *dat_name, const char *asset_name, Song *song) {
	FILE *f;
	dword offset;
	dword filesize;
	bool eof = false;
	byte duration, note, sustain, octave;
	int index = 0;

	// Open DAT file and search txt file inside
	f = fopen(dat_name, "rb");
	if (!f) {
		sprintf(engine.system_error_message1, "FILE_LoadSpeakerSong function error");
		sprintf(engine.system_error_message2, "Unable to open DAT file %s ", dat_name);
		sprintf(engine.system_error_message3, "");
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}

	// Search asset file
	offset = FILE_SeekAssetOffset(f, asset_name);
	if (offset == 0) {
		fclose(f);
		sprintf(engine.system_error_message1, "FILE_LoadSpeakerSong function error");
		sprintf(engine.system_error_message2, "Unable to find offset inside DAT file %s ", dat_name);
		sprintf(engine.system_error_message3, "Asset file %s", asset_name);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}

	// Get file length
	filesize = FILE_SeekAssetLength(f, asset_name);
	if (filesize == 0) {
		fclose(f);
		sprintf(engine.system_error_message1, "FILE_LoadSpeakerSong function error");
		sprintf(engine.system_error_message2, "Unable to find length inside DAT file %s ", dat_name);
		sprintf(engine.system_error_message3, "Asset file %s", asset_name);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}
	fseek(f, offset, SEEK_SET);// Set file pointer at the begining of the file

	if (filesize > song->buffer_max_size) {
		fclose(f);
		sprintf(engine.system_error_message1, "FILE_LoadSpeakerSong function error");
		sprintf(engine.system_error_message2, "Song size exceeds max length = %ld ", song->buffer_max_size);
		sprintf(engine.system_error_message3, "Loaded song size %ld", filesize);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}
	fseek(f, offset, SEEK_SET);// Set file pointer at the begining of the file

	// read data
	while (!eof) {
		fscanf(f, "%c%c%c%c, ", &duration, &note, &sustain, &octave);// get the tile number
		song->buffer[index] = duration;
		index++;
		song->buffer[index] = note;
		index++;
		song->buffer[index] = sustain;
		index++;
		song->buffer[index] = octave;
		index++;

		if (index > song->buffer_max_size - 4) {
			fclose(f);
			sprintf(engine.system_error_message1, "FILE_LoadSpeakerSong function error");
			sprintf(engine.system_error_message2, "Song size exceeds max length = %ld ", song->buffer_max_size);
			sprintf(engine.system_error_message3, "Loading stopped at %u index", index);
			Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
		}

		if (duration == 'F') {
			song->buffer[index] = 'F';
			song->buffer[index + 1] = 'F';
			song->buffer[index + 2] = 'F';
			song->buffer[index + 3] = 'F';
			eof = true;
		}
	}
	fclose(f);
}