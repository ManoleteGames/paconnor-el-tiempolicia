#include "../engine.h"
#include "pc.h"
#include <stdint.h>
#include "a2m.h"
#include "sblaster.h"

/* A2M music module loader
   version 0.2
   Depacks A2M v11 format music modules

   	A2M music v11 format specification
	----------------------------

	000000     10   (char)   ID string '_a2module_'
 	00000a      4   (dword)  32-bit cyclic redundancy check value
 	00000e      1   (byte)   file format version
 	00000f      1   (byte)   number of patterns {#pat}
   	
	000010      4   (dword)  length of data block 0  {%0}
 	000014      4   (dword)    ...    ...    ...  1  {%1}
 	000018      4   (dword)    ...    ...    ...  2  {%2}
 	00001c      4   (dword)    ...    ...    ...  3  {%3}
 	000020      4   (dword)    ...    ...    ...  4  {%4}
 	000024      4   (dword)    ...    ...    ...  5  {%5}
 	000028      4   (dword)    ...    ...    ...  6  {%6}
 	00002c      4   (dword)    ...    ...    ...  7  {%7}
 	000030      4   (dword)    ...    ...    ...  8  {%8}
 	000034      4   (dword)    ...    ...    ...  9  {%9}
 	000038      4   (dword)    ...    ...    ...  10 {%10}
 	00003c      4   (dword)    ...    ...    ...  11 {%11}
 	000040      4   (dword)    ...    ...    ...  12 {%12}
 	000044      4   (dword)    ...    ...    ...  13 {%13}
 	000048      4   (dword)    ...    ...    ...  14 {%14}
 	00004c      4   (dword)    ...    ...    ...  15 {%15}
 	000050      4   (dword)    ...    ...    ...  16 {%16}
 	000054     %0   (byte)   data block 0 (songdata)
  	+  %0     %1   (byte)   data block 1 (patterns) 00-07 ¿
  	+  %1     %2   (byte)   data block 2    ...     08-0f ³
  	+  %2     %3   (byte)   data block 3    ...     10-17 ³
  	+  %3     %4   (byte)   data block 4    ...     18-1f ³
  	+  %4     %5   (byte)   data block 5    ...     20-27 ³
  	+  %5     %6   (byte)   data block 6    ...     28-2f ³
  	+  %6     %7   (byte)   data block 7    ...     30-37 ³
  	+  %7     %8   (byte)   data block 8    ...     38-3f Ã corresponds with
  	+  %8     %9   (byte)   data block 9    ...     40-47 ³ #pat
  	+  %9    %10   (byte)   data block 10   ...     48-4f ³
  	+ %10    %11   (byte)   data block 11   ...     50-57 ³
  	+ %11    %12   (byte)   data block 12   ...     58-5f ³
  	+ %12    %13   (byte)   data block 13   ...     60-67 ³
  	+ %13    %14   (byte)   data block 14   ...     68-6f ³
  	+ %14    %15   (byte)   data block 15   ...     70-77 ³
  	+ %15    %16   (byte)   data block 16   ...     78-7f Ù

 	--EOF-

	structure of data block 0 (songdata) after decompression:

		Offset Length   Type

		000000     43   (string) songname [42 characters]
		00002b     43   (string) composer [...]
		000056 255*43   (string) table of 255 instrument names [42 chars each]
 		002b2b 255*14   (byte)   table of 255 instrument specifications:

							Offset Type

							| 000000 (byte) AM/Vibrato/EG   (modulator)
							| 000001 (byte) ...             (carrier)
							| 000002 (byte) KSL/Volume      (modulator)
							| 000003 (byte) ...             (carrier)
							| 000004 (byte) Attack/Decay    (modulator)
							| 000005 (byte) ...             (carrier)
				14 bytes -- | 000006 (byte) Sustain/Release (modulator)
							| 000007 (byte) ...             (carrier)
							| 000008 (byte) WaveForm        (modulator)
							| 000009 (byte) ...             (carrier)
							| 00000a (byte) Feedback/FM
							| 00000b (byte) panning (0=center,1=left,2=right)
							| 00000c (byte) fine-tune (signed)
							| 00000d (byte) voice type:

											ÚÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
											³ 0   ³ melodic                     ³
											³ 1-5 ³ percussion (BD,SD,TT,TC,HH) ³
											ÀÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

		00391d 255*3831 (byte)   table of 255 instrument macro-definitions:

			Offset Length Type

			000000      1 (byte) length
			000001      1 (byte) loop begin position
			000002      1 (byte) loop length
			000003      1 (byte) key-off position
			000004      1 (byte) arpeggio macro-table number
			000005      1 (byte) vibrato macro-table number

			000006 255*15 (???)  fm-register definitions:

								Offset Type

							| 000000 (byte) AM/Vibrato/EG   (modulator)
							| 000001 (byte) ...             (carrier)
							| 000002 (byte) KSL/Volume      (modulator)
							| 000003 (byte) ...             (carrier)
							| 000004 (byte) Attack/Decay    (modulator)
							| 000005 (byte) ...             (carrier)
				15 bytes  --| 000006 (byte) Sustain/Release (modulator)
							| 000007 (byte) ...             (carrier)
							| 000008 (byte) WaveForm        (modulator)
							| 000009 (byte) ...             (carrier)
	bit 7: retriggered flag>| 00000a (byte) Feedback/FM
							| 00000b (word) frequency slide (signed)
							| 00000d (byte) panning (0=center,1=left,2=right)
							| 00000e (byte) duration

		0f2126 255*521  (byte)   table of 255 arpeggio/vibrato macro-definitions:

			Offset Length Type

			000000      1 (byte) length
			000001      1 (byte) speed
			000002      1 (byte) loop begin position
			000003      1 (byte) loop length
			000004      1 (byte) key-off position
			000005    255 (byte) table of 255 arpeggio definitions:

								ÚÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
								³ 0          ³ default note                ³
								³ 1-96       ³ number of half-tones to add ³
								³ 1-97[+80h] ³ fixed-note                  ³
								ÀÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

			000104      1 (byte) length
			000105      1 (byte) speed
			000106      1 (byte) delay
			000107      1 (byte) loop begin position
			000108      1 (byte) loop length
			000109      1 (byte) key-off position
			00010a    255 (byte) table of 255 vibrato definitions:

								ÚÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
								³ 0       ³ default frequency               ³
								³ -7F..7F ³ frequency units to add (signed) ³
								ÀÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

		11281d    128   (byte)   pattern order table (order entries 00-7f)
		11289d      1   (byte)   initial tempo
		11289e      1   (byte)   initial speed
		11289f      1   (byte)   flags
		1128a0      2   (word)   pattern length
		1128a2      1   (byte)   number of tracks
		1128a3      2   (word)   macro speed-up factor
		1128a5      1   (byte)   4-op track extension flags
		1128a6     20   (byte)   initial lock flags for 20 tracks
		1128ba 128*43   (string) table of 128 pattern names [42 char each]
		113e3a 255*28   (byte)   table of 255 disabled fm-register columns:

			Offset Length Type
	        000000      1 (byte) attack rate       ┐
	        000001      1 (byte) decay rate        │
	        000002      1 (byte) sustain level     │
	        000003      1 (byte) release rate      │
	        000004      1 (byte) waveform type     │
	        000005      1 (byte) output level      ├ modulator
	        000006      1 (byte) key scaling level │
	        000007      1 (byte) multiplier        │
	        000008      1 (byte) tremolo           │
	        000009      1 (byte) vibrato           │
	        00000a      1 (byte) key scale rate    │
	        00000b      1 (byte) sustain           ┘
	        00000c      1 (byte) attack rate       ┐
	        00000d      1 (byte) decay rate        │
	        00000e      1 (byte) sustain level     │
	        00000f      1 (byte) release rate      │
	        000010      1 (byte) waveform type     │
	        000011      1 (byte) output level      ├ carrier
	        000012      1 (byte) key scaling level │
	        000013      1 (byte) multiplier        │
	        000014      1 (byte) tremolo           │
	        000015      1 (byte) vibrato           │
	        000016      1 (byte) key scale rate    │
	        000017      1 (byte) sustain           ┘
	        000018      1 (byte) connection type
	        000019      1 (byte) feedback
	        00001a      1 (byte) frequency slide
	        00001b      1 (byte) panning

		structure of 'flags' byte:
		 ┌─────┬────────────────────────────┐
		 │ BiT │ SWiTCH                     │
		 ├─────┼────────────────────────────┤
		 │  0  │ update speed               │
		 │  1  │ track volume lock          │
		 │  2  │ volume peak lock           │
		 │  3  │ tremolo depth              │
		 │  4  │ vibrato depth              │
		 │  5  │ track panning lock         │
		 │  6  │ percussion track extension │
		 │  7  │ volume scaling             │
		 └─────┴────────────────────────────┘

		structure of '4-op track extension flags' byte:

 		 ┌─────┬──────────────┐
		 │ BiT │ SWiTCH       │
		 ├─────┼──────────────┤
		 │  0  │ tracks 1,2   │
		 │  1  │ tracks 3,4   │
		 │  2  │ tracks 5,6   │
		 │  3  │ tracks 10,11 │
		 │  4  │ tracks 12,13 │
		 │  5  │ tracks 14,15 │
		 │  6  │ %unused%     │
		 │  7  │ %unused%     │
		 └─────┴──────────────┘

		structure of 'initial lock flags' byte:

		 ┌─────┬─────────────────────────────────────────┐
		 │ BiT │ SWiTCH                                  │
		 ├─────┼─────────────────────────────────────────┤
		 │  0─┐│ panning position:                       │
		 │  1─┘│ 0=center, 1=left, 2=right               │
		 │  2─┐│ volume slide type:                      │
		 │  3─┘│ 0=def, 1=car, 2=mod, 3=car w/mod        │
		 │  4  │ volume lock state                       │
		 │  5  │ peak lock state                         │
		 │  6  │ 4OP track volume lock (since FFver. 13) │
		 │  7  │ %unused%                                │
		 └─────┴─────────────────────────────────────────┘

 	structure of data block 1-16 (patterns) after decompression:

		Offset Length   Type

		000000 20*256*6 (byte)   pattern [block_number-1]*8+0
			³  ³
		channel Ù  À line        Offset Type
		1-20        0-255
							Ú 000000 (byte) note [0-96; +90h=fixed; 255=Key-Off]
							³ 000001 (byte) instrument [0-255]
					6 bytes ´ 000002 (byte) 1st effect command [0-39]
							³ 000003 (byte) 1st effect data
							³ 000004 (byte) 2nd effect command [0-39]
							À 000005 (byte) 2nd effect data

		007800 20*256*6 (byte)   pattern [block_number-1]*8+1
			.      ...    ...       ...         ...
			.      ...    ...       ...         ...
			.      ...    ...       ...         ...
		070800 20*256*6 (byte)   pattern [block_number-1]*8+7

*/

A2M_SONGINFO *songinfo;
A2M_CHDATA *ch_data;

/* internal data structure */
struct A2M_APDSTATE {
	const byte *source;
	byte *destination;
	word destlen;
	word tag;
	word bitcount;
};

void A2M_Init(void) {
	songinfo = MM_PushChunk(sizeof(A2M_SONGINFO), CT_ENGINE);

	ch_data = MM_PushChunk(sizeof(A2M_CHDATA), CT_ENGINE);
}


static word A2M_GetBit(struct A2M_APDSTATE *ud) {
	word bit;

	/* check if tag is empty */
	if (!ud->bitcount--) {
		/* load next tag */
		ud->tag = *ud->source++;
		ud->bitcount = 7;
	}

	/* shift bit out of tag */
	bit = (ud->tag >> 7) & 0x01;
	ud->tag <<= 1;

	return bit;
}

static word A2M_GetGamma(struct A2M_APDSTATE *ud) {
	unsigned int result = 1;

	/* input gamma2-encoded bits */
	do {
		result = (result << 1) + A2M_GetBit(ud);
	} while (A2M_GetBit(ud));

	return result;
}

word A2M_Depack(const byte *source, byte *destination, int srcsize, int dstsize) {
	struct A2M_APDSTATE ud;
	word offs, len, R0 /*, LWM*/;
	int done;
	int i;

	ud.source = (const byte *) source;
	ud.destination = destination;
	ud.destlen = 0;
	ud.bitcount = 0;

	R0 = (word) -1;
	//LWM = 0;
	done = 0;

	/* first byte verbatim */
	if (!srcsize) return ud.destlen;
	if (!dstsize) return ud.destlen;
	*ud.destination++ = *ud.source++;
	srcsize--;
	dstsize--;
	ud.destlen++;

	/* main decompression loop */
	while (!done) {
		if (A2M_GetBit(&ud)) {
			if (A2M_GetBit(&ud)) {
				if (A2M_GetBit(&ud)) {
					offs = 0;

					for (i = 4; i; i--) {
						offs = (offs << 1) + A2M_GetBit(&ud);
					}

					if (offs) {
						if (!dstsize) return ud.destlen;
						if (offs > ud.destlen) return ud.destlen;
						*ud.destination = *(ud.destination - offs);
						ud.destination++;
						dstsize--;
						ud.destlen++;
					} else {
						if (!dstsize) return ud.destlen;
						*ud.destination++ = 0x00;
						dstsize--;
						ud.destlen++;
					}

					//LWM = 0;
				} else {
					if (!srcsize) return ud.destlen;
					offs = *ud.source++;
					srcsize--;

					len = 2 + (offs & 0x0001);

					offs >>= 1;

					if (offs) {
						for (; len; len--) {
							if (!dstsize) return ud.destlen;
							if (offs > ud.destlen) return ud.destlen;
							*ud.destination = *(ud.destination - offs);
							ud.destination++;
							dstsize--;
							ud.destlen++;
						}
					} else {
						done = 1;
					}

					R0 = offs;
					//LWM = 1;
				}
			} else {
				offs = A2M_GetGamma(&ud);

				//if ((LWM == 0) && (offs == 2)) {
				if (offs == 2) {
					//-------------------
					offs = R0;

					len = A2M_GetGamma(&ud);

					for (; len; len--) {
						if (!dstsize) return ud.destlen;
						if (offs > ud.destlen) return ud.destlen;
						*ud.destination = *(ud.destination - offs);
						ud.destination++;
						dstsize--;
						ud.destlen++;
					}
				} else {
					/*
					if (LWM == 0) {
						offs -= 2;
					}
					else {
						offs -= 2;
					}
					*/
					offs -= 3;
					//-------------------

					offs <<= 8;
					if (!srcsize) return ud.destlen;
					offs += *ud.source++;
					srcsize--;

					len = A2M_GetGamma(&ud);

					if (offs >= 32000) {
						len++;
					}
					if (offs >= 1280) {
						len++;
					}
					if (offs < 128) {
						len += 2;
					}

					for (; len; len--) {
						if (!dstsize) return ud.destlen;
						if (offs > ud.destlen) return ud.destlen;
						*ud.destination = *(ud.destination - offs);
						ud.destination++;
						dstsize--;
						ud.destlen++;
					}

					R0 = offs;
				}

				//LWM = 1;
			}
		} else {
			if (!srcsize) return ud.destlen;
			if (!dstsize) return ud.destlen;
			*ud.destination++ = *ud.source++;
			srcsize--;
			dstsize--;
			ud.destlen++;
			//LWM = 0;
		}
	}

	return ud.destlen;
}

/* Clean songinfo before importing a2t tune */
static void A2M_InitSongData(void) {

	songinfo->patt_len = 64;
	songinfo->nm_tracks = 18;
	songinfo->tempo = 50;
	songinfo->speed = 6;
	songinfo->macro_speedup = 1;

	// common flags
	songinfo->speed_update = false;
	songinfo->lockvol = false;
	songinfo->panlock = false;
	songinfo->lockVP = false;
	songinfo->tremolo_depth = 0;
	songinfo->vibrato_depth = 0;
	songinfo->volume_scaling = false;
	songinfo->percussion_mode = false;
	songinfo->overall_volume = 63;
	songinfo->global_volume = 63;
}

static bool A2M_IsDataEmpty(void *data, unsigned int size) {
	while (size--) {
		if (*(char *) data++)
			return false;
	}

	return true;
}

// Fill tFM_INST_DATA from the raw byte array
static void A2M_GetFM_Data(A2M_FM_INST_DATA *fm, byte *src) {
	fm->multipM = src[0] & 0xf;
	fm->ksrM = (src[0] >> 4) & 1;
	fm->sustM = (src[0] >> 5) & 1;
	fm->vibrM = (src[0] >> 6) & 1;
	fm->tremM = (src[0] >> 7) & 1;

	fm->multipC = src[1] & 0xf;
	fm->ksrC = (src[1] >> 4) & 1;
	fm->sustC = (src[1] >> 5) & 1;
	fm->vibrC = (src[1] >> 6) & 1;
	fm->tremC = (src[1] >> 7) & 1;

	fm->volM = src[2] & 0x3f;
	fm->kslM = (src[2] >> 6) & 3;

	fm->volC = src[3] & 0x3f;
	fm->kslC = (src[3] >> 6) & 3;

	fm->decM = src[4] & 0xf;
	fm->attckM = (src[4] >> 4) & 0xf;

	fm->decC = src[5] & 0xf;
	fm->attckC = (src[5] >> 4) & 0xf;

	fm->relM = src[6] & 0xf;
	fm->sustnM = (src[6] >> 4) & 0xf;

	fm->relC = src[7] & 0xf;
	fm->sustnC = (src[7] >> 4) & 0xf;

	fm->wformM = src[8] & 7;
	fm->wformC = src[9] & 7;

	fm->connect = src[10] & 1;
	fm->feedb = (src[10] >> 1) & 7;

	//printf("feedback %u", fm->feedb);
	//delay(1000);
}

static void A2M_ImportFMregTable(dword n, byte *src) {
	int i, j;
	int rts_offset;

	// Note: for editor_mode allocate max entries possible
	for (i = 0; i < n; i++, src += tFMREG_TABLE_SIZE) {
		if (src[0] /* length */) {

			// Copy field by field
			songinfo->instrinfo.instruments[i].fmreg.length = src[0];        // length
			songinfo->instrinfo.instruments[i].fmreg.loop_begin = src[1];    // loop_begin
			songinfo->instrinfo.instruments[i].fmreg.loop_length = src[2];   // loop_length
			songinfo->instrinfo.instruments[i].fmreg.keyoff_pos = src[3];    // keyoff_pos
			songinfo->instrinfo.instruments[i].fmreg.arpeggio_table = src[4];// arpeggio_table
			songinfo->instrinfo.instruments[i].fmreg.vibrato_table = src[5]; // vibrato_table

			rts_offset = 6;
			for (j = 0; j < 255; j++, rts_offset += tREGISTER_TABLE_DEF_SIZE) {
				songinfo->instrinfo.instruments[i].fmreg.data[j].fm.multipM = src[rts_offset + 0] & 0xf;
				songinfo->instrinfo.instruments[i].fmreg.data[j].fm.ksrM = (src[rts_offset + 0] >> 4) & 1;
				songinfo->instrinfo.instruments[i].fmreg.data[j].fm.sustM = (src[rts_offset + 0] >> 5) & 1;
				songinfo->instrinfo.instruments[i].fmreg.data[j].fm.vibrM = (src[rts_offset + 0] >> 6) & 1;
				songinfo->instrinfo.instruments[i].fmreg.data[j].fm.tremM = (src[rts_offset + 0] >> 7) & 1;

				songinfo->instrinfo.instruments[i].fmreg.data[j].fm.multipC = src[rts_offset + 1] & 0xf;
				songinfo->instrinfo.instruments[i].fmreg.data[j].fm.ksrC = (src[rts_offset + 1] >> 4) & 1;
				songinfo->instrinfo.instruments[i].fmreg.data[j].fm.sustC = (src[rts_offset + 1] >> 5) & 1;
				songinfo->instrinfo.instruments[i].fmreg.data[j].fm.vibrC = (src[rts_offset + 1] >> 6) & 1;
				songinfo->instrinfo.instruments[i].fmreg.data[j].fm.tremC = (src[rts_offset + 1] >> 7) & 1;

				songinfo->instrinfo.instruments[i].fmreg.data[j].fm.volM = src[rts_offset + 2] & 0x3f;
				songinfo->instrinfo.instruments[i].fmreg.data[j].fm.kslM = (src[rts_offset + 2] >> 6) & 3;

				songinfo->instrinfo.instruments[i].fmreg.data[j].fm.volC = src[rts_offset + 3] & 0x3f;
				songinfo->instrinfo.instruments[i].fmreg.data[j].fm.kslC = (src[rts_offset + 3] >> 6) & 3;

				songinfo->instrinfo.instruments[i].fmreg.data[j].fm.decM = src[rts_offset + 4] & 0xf;
				songinfo->instrinfo.instruments[i].fmreg.data[j].fm.attckM = (src[rts_offset + 4] >> 4) & 0xf;

				songinfo->instrinfo.instruments[i].fmreg.data[j].fm.decC = src[rts_offset + 5] & 0xf;
				songinfo->instrinfo.instruments[i].fmreg.data[j].fm.attckC = (src[rts_offset + 5] >> 4) & 0xf;

				songinfo->instrinfo.instruments[i].fmreg.data[j].fm.relM = src[rts_offset + 6] & 0xf;
				songinfo->instrinfo.instruments[i].fmreg.data[j].fm.sustnM = (src[rts_offset + 6] >> 4) & 0xf;

				songinfo->instrinfo.instruments[i].fmreg.data[j].fm.relC = src[rts_offset + 7] & 0xf;
				songinfo->instrinfo.instruments[i].fmreg.data[j].fm.sustnC = (src[rts_offset + 7] >> 4) & 0xf;

				songinfo->instrinfo.instruments[i].fmreg.data[j].fm.wformM = src[rts_offset + 8] & 7;
				songinfo->instrinfo.instruments[i].fmreg.data[j].fm.wformC = src[rts_offset + 9] & 7;

				songinfo->instrinfo.instruments[i].fmreg.data[j].fm.connect = src[rts_offset + 10] & 1;
				songinfo->instrinfo.instruments[i].fmreg.data[j].fm.feedb = (src[rts_offset + 10] >> 1) & 7;

				songinfo->instrinfo.instruments[i].fmreg.data[j].freq_slide = (int16_t) (src[rts_offset + 11] | (src[rts_offset + 12] << 8));
				songinfo->instrinfo.instruments[i].fmreg.data[j].panning = src[rts_offset + 13]; // panning
				songinfo->instrinfo.instruments[i].fmreg.data[j].duration = src[rts_offset + 14];// duration
				songinfo->instrinfo.instruments[i].fmreg.data[j].macro_flags = src[rts_offset + 10] & 0xf0;
			}
		}
	}
}

void A2M_LoadFile(const char *dat_name, const char *asset_name) {
	word datablock_length[21];
	dword filesize;
	byte *rawdata;
	byte *blockptr;
	byte *header;
	byte *unpacked;

	////////////////////// 1st. Read raw data /////////////////////
	rawdata = FILE_LoadA2MSongInfo(dat_name, asset_name, &filesize, CT_TEMPORARY);

	////////////////////// 2nd. Get header data /////////////////////
	header = rawdata;
	blockptr = rawdata + 16;//header size = 16

	if (filesize < 16)
		Error("Error in function A2M_LoadFile!", "File too small", asset_name, ERROR_SOUND);

	if (strncmp((const char *) header, "_A2module_", 10))
		Error("Error in function A2M_LoadFile!", "File is not a valid A2M module", asset_name, ERROR_SOUND);

	A2M_InitSongData();

	songinfo->ffver = A2M_HEADER_FFVER(header);
	songinfo->num_patterns = A2M_HEADER_NPATT(header);

	if (songinfo->ffver != 11)
		Error("Error in function A2M_LoadFile!", "Only version 11 is supported", asset_name, ERROR_SOUND);

	// 17 fixed data blocks. Length size of each 4 bytes
	if (filesize < 17 << 2)
		Error("Error in function A2M_LoadFile!", "Impossible get block lengths", "File too small", ERROR_SOUND);

	// Get block lengths
	for (unsigned int i = 0; i < 17; i++)
		datablock_length[i] = UINT32LE(blockptr + (i * 4));

	blockptr += 17 << 2;

	////////////////////// 3rd. Get song data (block 0) /////////////////////
	if (datablock_length[0] > filesize)
		Error("Error in function A2M_LoadFile!", "File too small", "", ERROR_SOUND);

	// Block 0 has a fixes size of 0x115ea2 bytes
	unpacked = MM_PushChunk(0x115ea2, CT_TEMPORARY);

	A2M_Depack(blockptr, unpacked, datablock_length[0], 0x115ea2);

	// Fill songinfo structure
	memcpy(songinfo->songname, A2M_SONGDATA_SONGNAME_P(unpacked) + 1, 42);
	memcpy(songinfo->composer, A2M_SONGDATA_COMPOSER_P(unpacked) + 1, 42);

	// Calculate the real number of used instruments
	songinfo->instrinfo.count = 255;
	while (songinfo->instrinfo.count && A2M_IsDataEmpty(A2M_SONGDATA_INSTR_DATA_P(unpacked, songinfo->instrinfo.count - 1), tINSTR_DATA_SIZE))
		songinfo->instrinfo.count--;

	songinfo->instrinfo.size = songinfo->instrinfo.count * sizeof(A2M_INSTR_DATA_EXT);
	for (int i = 1; i <= songinfo->instrinfo.count; i++) {
		byte *srci = A2M_SONGDATA_INSTR_DATA_P(unpacked, i - 1);

		A2M_GetFM_Data(&songinfo->instrinfo.instruments[i].instr_data.fm, srci);

		songinfo->instrinfo.instruments[i].instr_data.panning = srci[11] & 3;
		songinfo->instrinfo.instruments[i].instr_data.fine_tune = srci[12];

		songinfo->instrinfo.instruments[i].instr_data.perc_voice = srci[13];

		if (songinfo->instrinfo.instruments[i].instr_data.panning >= 3) {
			songinfo->instrinfo.instruments[i].instr_data.panning = 0;
		}
	}

	for (int i = 1; i < 255; i++)
		memcpy(songinfo->instr_names[i], A2M_SONGDATA_INSTR_NAMES_P(unpacked, i) + 1, 42);


	// Allocate fmreg macro tables
	A2M_ImportFMregTable(songinfo->instrinfo.count, A2M_SONGDATA_FMREG_TABLE_P(unpacked, 0));

	for (int i = 1; i <= songinfo->instrinfo.count; i++) {
		// Instrument arpegio/vibrato references
		songinfo->instrinfo.instruments[i].arpeggio = songinfo->instrinfo.instruments[i].fmreg.arpeggio_table;
		songinfo->instrinfo.instruments[i].vibrato = songinfo->instrinfo.instruments[i].fmreg.vibrato_table;
	}

	// Allocate arpeggio/vibrato macro tables
	// TODO: Calculate actual num of arp/vib tables
	//arpvib_tables_allocate(255, A2M_SONGDATA_V9_14_ARPVIB_TABLE_P(unpacked, 0));

	memcpy(songinfo->pattern_order, A2M_SONGDATA_PATTERN_ORDER_P(unpacked, 0), 128);

	songinfo->tempo = A2M_SONGDATA_TEMPO(unpacked);
	songinfo->speed = A2M_SONGDATA_SPEED(unpacked);
	songinfo->common_flag = A2M_SONGDATA_COMMON_FLAG(unpacked);
	songinfo->patt_len = A2M_SONGDATA_PATT_LEN(unpacked);
	songinfo->nm_tracks = A2M_SONGDATA_NM_TRACKS(unpacked);
	songinfo->macro_speedup = A2M_SONGDATA_MACRO_SPEEDUP(unpacked);

	songinfo->speed_update = (songinfo->common_flag >> 0) & 1;
	songinfo->lockvol = (songinfo->common_flag >> 1) & 1;
	songinfo->lockVP = (songinfo->common_flag >> 2) & 1;
	songinfo->tremolo_depth = (songinfo->common_flag >> 3) & 1;
	songinfo->vibrato_depth = (songinfo->common_flag >> 4) & 1;
	songinfo->panlock = (songinfo->common_flag >> 5) & 1;
	songinfo->percussion_mode = (songinfo->common_flag >> 6) & 1;
	songinfo->volume_scaling = (songinfo->common_flag >> 7) & 1;

	// Free all allocated memory and allocate fixed memory for music events
	MM_PopChunks(CT_TEMPORARY);

	if (songinfo->nm_tracks > 9)
		Error("Error in function A2M_LoadFile!", "Excessive number of tracks (channels)", "Maximum number of channels == 9", ERROR_SOUND);

	if (songinfo->num_patterns > 0x70) {
		Error("Error in function A2M_LoadFile!", "Excessive number of patterns", "Maximum number of patterns == 16", ERROR_SOUND);
	}

	for (int p = 0; p < songinfo->num_patterns; p++) {
		songinfo->pattern[p].channels = songinfo->nm_tracks;

		for (int c = 0; c < songinfo->nm_tracks; c++) {
			songinfo->pattern[p].channel[c].rows = songinfo->patt_len;
			songinfo->pattern[p].channel[c].events = MM_PushChunk(songinfo->patt_len * sizeof(A2M_TRACK_EVENT), CT_MUSIC);
		}
	}

	////////////////////// 4th. Get events data (blocks 1..n) /////////////////////

	rawdata = MM_PushChunk(filesize, CT_TEMPORARY);
	unpacked = MM_PushChunk(8 * 20 * 256 * 6, CT_TEMPORARY);

	//f = fopen(filename, "rb");
	//fread(rawdata, filesize, 1, f);
	//fclose(f);
	rawdata = FILE_LoadA2MSongInfo(dat_name, asset_name, &filesize, CT_TEMPORARY);

	// Block 1 ( patterns 0..7 )
	blockptr = rawdata + 16;//header size = 16
	blockptr += 17 * 4 + datablock_length[0];
	if (datablock_length[1] != 0) {
		int src_index = 0;
		int dst_index = 0;
		A2M_Depack(blockptr, unpacked, datablock_length[1], 8 * 20 * 256 * 6);

		for (int p = 0; p < 8; p++) {
			for (int c = 0; c < songinfo->nm_tracks; c++) {// channel
				src_index = (p * 20 * 256 * 6) + (c * 256 * 6);
				dst_index = 0;
				for (int r = 0; r < songinfo->pattern[p].channel[c].rows; r++) {// row
					songinfo->pattern[p].channel[c].events[dst_index].note = unpacked[src_index];
					songinfo->pattern[p].channel[c].events[dst_index].instr_def = unpacked[src_index + 1];
					songinfo->pattern[p].channel[c].events[dst_index].eff[0].def = unpacked[src_index + 2];
					songinfo->pattern[p].channel[c].events[dst_index].eff[0].val = unpacked[src_index + 3];
					songinfo->pattern[p].channel[c].events[dst_index].eff[1].def = unpacked[src_index + 4];
					songinfo->pattern[p].channel[c].events[dst_index].eff[1].val = unpacked[src_index + 5];
					src_index += 6;
					dst_index++;
				}
			}
		}
	}

	MM_PopChunks(CT_TEMPORARY);

	/*FILE *f;
	f = fopen("song.out", "wb");
	fwrite(songinfo->pattern[0].channel[5].events, sizeof(A2M_TRACK_EVENT) * 64, 1, f);
	fclose(f);*/

	/*printf("A2M song name: %s\n", songinfo->songname);
	printf("A2M song name: %s\n", songinfo->composer);
	printf("A2M version: %d\n", songinfo->ffver);
	printf("Number of patterns: %d\n", songinfo->num_patterns);
	printf("Rows per pattern: %d\n", songinfo->patt_len);
	printf("Voices per pattern (channels): %d\n", songinfo->nm_tracks);
	printf("Intrument counter: %d\n", songinfo->instrinfo.count);
	printf("Tempo: %d\n", songinfo->tempo);
	printf("Speed: %d\n", songinfo->speed);
	printf("Volume scaling: %d\n", songinfo->volume_scaling);
	printf("Percussion mode: %d\n", songinfo->percussion_mode);
	printf("Track volume lock: %d\n", songinfo->lockvol);*/

	//sleep(50);
}

void A2M_Unload(Song *song) {
	disable();
	song->stop = true;
	song->play = false;
	MM_PopChunks(CT_MUSIC);
	enable();
}

byte A2M_ReadFM(byte reg) {
	byte Counter;

	//Select register
	outportb(0x388, reg);
	//Wait for card to accept value
	for (Counter = 1; Counter < 25; Counter++) { inportb(0x388); }
	//Send value
	return inportb(0x389);
	//Wait for card to accept value
	//for (Counter = 1; Counter < 100; Counter++) { inportb(0x388); }
}

void A2M_WriteFM(byte reg, byte value) {
	byte Counter;

	//Select register
	outportb(0x388, reg);
	//Wait for card to accept value
	for (Counter = 1; Counter < 25; Counter++) { inportb(0x388); }
	//Send value
	outportb(0x389, value);
	//Wait for card to accept value
	for (Counter = 1; Counter < 100; Counter++) { inportb(0x388); }
}

/** A2M_SetInstrument :: Set instrument parameters
 */
void A2M_SetInstrument(byte chan, A2M_INSTR_DATA instr) {
	//Addresses of the operators used to form voice data
	const byte OpAdr[9] = {0x00, 0x1, 0x2, 0x8, 0x9, 0xA, 0x10, 0x11, 0x12};
	byte value;
	/*if (chan == 4) {
		printf("CH %u >> INST >> Modulator\n", chan);
		printf(" amp modulation %u,", instr.fm.tremM);
		printf(" vibrato enable %u,", instr.fm.vibrM);
		printf(" sustain enable %u,", instr.fm.sustM);
		printf(" scaling rate %u,", instr.fm.ksrM);
		printf(" freq multiply %u,", instr.fm.multipM);

		printf(" scaling level %u,", instr.fm.kslM);
		printf(" volume %u", instr.fm.volM);
		printf(" attack %u,", instr.fm.attckM);
		printf(" decay %u,", instr.fm.decM);

		printf(" sust %u,", instr.fm.sustnM);
		printf(" rel %u,", instr.fm.relM);
		printf(" waveform %u\n", instr.fm.wformM);

		printf("CH %u >> INST >> Carrier\n", chan);
		printf(" amp modulation %u,", instr.fm.tremC);
		printf(" vibrato enable %u,", instr.fm.vibrC);
		printf(" sustain enable %u,", instr.fm.sustC);
		printf(" scaling rate %u,", instr.fm.ksrC);
		printf(" freq multip %u,", instr.fm.multipC);

		printf(" scaling level %u,", instr.fm.kslC);
		printf(" volume %u,", instr.fm.volC);
		printf(" attack %u,", instr.fm.attckC);
		printf(" decay %u,", instr.fm.decC);

		printf(" sus %u,", instr.fm.sustC);
		printf(" rel %u,", instr.fm.relC);
		printf(" waveform %u\n", instr.fm.wformC);

		printf("INST >> Feedback %u\n", instr.fm.feedb);
	}*/

	// Tremolo (amplitude modulation), Vibrato, Sustain (envelope generator type), KSR (Key scale rate) and Frequency multiplier
	// - Tremolo. When set, turns tremolo (volume vibrato) ON for the corresponding slot.
	//   The repetition rate is 3.7Hz, the depth is optional (1dB/4.8dB).
	// - Vibrato. When set, turns frequency vibrato ON for the corresponding slot.
	//   The repetition rate is 6.1Hz, the depth is optional (7%/14%).
	// - Sustain (envelope generator type).  When set, the sustain level of the voice is maintained until released.
	//   When clear, the sound begins to decay immediately after hitting
	//   the sustain phase..
	// - KSR (key scale rate). When set, makes the sound shorter at higher frequencies.
	//   With normal musical instruments, the attack and decay rate becomes faster
	//   as the pitch increases. The key scale rate controls simulation of
	//   this effect. An offset (rof) is added to the individual attack, decay,
	//	 and release rates depending on the following formula:
	//       actual_rate = (rate * 4) + rof
	// - Frequency data multiplier. Sets the multiplier for the frequency data specified by block and
	//   F-number. This multiplier is applied to the FM carrier or modulation
	//   frequencies. The multiplication factor and corresonding harmonic types are
	//   shown in the following table:
	//    0 >> 0.5 >> 1 octave below
	//    1 >> 1   >> at the voice specified freq
	//    2 >> 2   >> 1 octave above
	//    3 >> 3   >> 1 octave and a 5th above
	//    4 >> 4   >> 2 octaves abobe
	//    5 >> 5   >> 2 octaves abobe and a Major 3rd above
	//    6 >> 6   >> 2 octaves abobe and a 5th above
	//    7 >> 7   >> 2 octaves abobe and a Minor 7th above
	//    8 >> 8   >> 3 octaves abobe
	//    9 >> 9   >> 3 octaves abobe and a Major 2nd above
	//    A >> 10  >> 3 octaves abobe and a Major 3rd above
	//    B >> 10  >> ...
	//    C >> 12  >> 3 octaves abobe and a 5th above
	//    D >> 12  >> ...
	//    E >> 15  >> 3 octaves abobe and a Major 7th above
	//    F >> 15  >> ...
	value = (instr.fm.tremM & 0x01) << 7 | (instr.fm.vibrM & 0x01) << 6 | (instr.fm.sustM & 0x01) << 5 | (instr.fm.ksrM & 0x01) << 4 | (instr.fm.multipM & 0x0F);
	SB_WriteFM(0x20 + OpAdr[chan], value);
	//SB_WriteFM(0x23 + OpAdr[chan], value);

	// Key scaling and Volume (output) level
	// - When KLS is set, makes the sound softer at higher frequencies.
	//   With musical instruments, volume decreases as pitch increases.
	//   Level key scaling values are used to simulate this effect.
	//   If any (not zero), the diminishing factor can be 1.5 dB/octave,
	//   3.0 dB/octave, or 6.0 dB/octave.
	// - Volume (output) level. Ranges from 0 to 63 indicates the attenuation according to the
	//   envelope generator output. In Additive synthesis, varying
	//   the output level of any operator varies the volume of its corresponding
	//   channel. In FM synthesis, varying the output level of carrier varies
	//   the volume of its corresponding channel, but varying the output of
	//   the modulator will change the frequency spectrum produced by the carrier.
	value = ((instr.fm.kslM & 0x03)) << 6 | (instr.fm.volM & 0x3F);
	SB_WriteFM(0x40 + OpAdr[chan], value);
	//SB_WriteFM(0x43 + OpAdr[chan], value);

	// Attack and Decay rate
	// - Attack indicates how fast the sound volume goes to maximum. 1=slow, 15=fast, 0 means no atack phase
	// - Decay indicates how fast the sound volume goes from maximum to sustain level. 1=slow, 15=fast, 0 means no decay phase
	value = ((instr.fm.attckM & 0x0f) << 4) | (instr.fm.decM & 0x0F);
	SB_WriteFM(0x60 + OpAdr[chan], value);
	//SB_WriteFM(0x63 + OpAdr[chan], value);

	// Sustaing and release levels
	// - Sustain indicates the sustain level. 1=loudest, 15=softest. 0 means no sustain phase.
	// - Release indicates how fast the sound goes from sustain level to zero level.  1=slow, 15=fast. 0 means no release phase.
	value = ((instr.fm.sustnM & 0x0f) << 4) | (instr.fm.relM & 0x0f);
	SB_WriteFM(0x80 + OpAdr[chan], value);
	//SB_WriteFM(0x83 + OpAdr[chan], value);

	// Waveform select
	// - Specifies the output waveform type
	// - 0: Sine /\/
	// - 1: Half sine ^^^
	// - 2: ABS-sine ^-^-^
	// - 3: Pulse sine /|_/|_
	// - 4: Sine/EPO /\-/
	// - 5: ABS-Sine/EPO /\/\-/\/\-
	// - 6: Square _||_||_
	// - 7: Derived ...
	SB_WriteFM(0xE0 + OpAdr[chan], (instr.fm.wformM & 0x03));
	//SB_WriteFM(0xE3 + OpAdr[chan], instr.fm.wformM);


	// Same as modulator
	value = (instr.fm.tremC & 0x01) << 7 | (instr.fm.vibrC & 0x01) << 6 | (instr.fm.sustC & 0x01) << 5 | (instr.fm.ksrC & 0x01) << 4 | (instr.fm.multipC & 0x0F);
	SB_WriteFM(0x23 + OpAdr[chan], value);
	//SB_WriteFM(0x20 + OpAdr[chan], value);

	// Same as modulator
	value = ((instr.fm.kslC & 0x03) << 6) | (instr.fm.volC & 0x3F);
	SB_WriteFM(0x43 + OpAdr[chan], value);
	//SB_WriteFM(0x40 + OpAdr[chan], value);

	// Same as modulator
	value = ((instr.fm.attckC & 0x0f) << 4) | (instr.fm.decC & 0x0f);
	SB_WriteFM(0x63 + OpAdr[chan], value);
	//SB_WriteFM(0x60 + OpAdr[chan], value);

	// Same as modulator
	value = ((instr.fm.sustnC & 0x0f) << 4) | (instr.fm.relC & 0x0f);
	SB_WriteFM(0x83 + OpAdr[chan], value);
	//SB_WriteFM(0x80 + OpAdr[chan], value);

	// Same as modulator
	SB_WriteFM(0xE3 + OpAdr[chan], instr.fm.wformC & 0x03);
	//SB_WriteFM(0xE0 + OpAdr[chan], instr.fm.wformC);

	// Feedback strength and coonnection type
	// - Feedback ranged from 0 to 7, it indicates the modulation depth
	//   for the modulator stot FM feedback
	//    0: 0
	//    1: pi/16
	//    2: pi/8
	//    3: pi/4
	//    4: pi/2
	//    5: pi
	//    6: pi*2
	//    7: pi*4
	// - Connection type
	//    0: Frequency modulation (FM)
	//    1: Additive syntesis (AM)
	value = ((instr.fm.feedb & 0x07) << 1) | (instr.fm.connect & 0x01);
	SB_WriteFM(0xC0 + OpAdr[chan], value);


	/* Amplitude Modulation Depth / Vibrato Depth / Rhythm
	     7     6     5     4     3     2     1     0
     +-----+-----+-----+-----+-----+-----+-----+-----+
     | AM  | Vib | Rhy | BD  | SD  | TOM | Top | HH  |
     | Dep | Dep | Ena |     |     |     | Cym |     |
     +-----+-----+-----+-----+-----+-----+-----+-----+

          bit 7 - Set:    AM depth is 4.8dB
                  Clear:  AM depth is 1 dB
          bit 6 - Set:    Vibrato depth is 14 cent
                  Clear:  Vibrato depth is 7 cent
          bit 5 - Set:    Rhythm enabled  (6 melodic voices) 
                  Clear:  Rhythm disabled (9 melodic voices)
          bit 4 - Bass drum on/off
          bit 3 - Snare drum on/off
          bit 2 - Tom tom on/off
          bit 1 - Cymbal on/off
          bit 0 - Hi Hat on/off

          Note:  KEY-ON registers for channels 06, 07, and 08 must be OFF
                 in order to use the rhythm section.  Other parameters
                 such as attack/decay/sustain/release must also be set
                 appropriately.*/
	/*switch (instr.perc_voice) {
		case 0:// melodic
			printf("MELODIC!!!!!!!!!!!!!!\n");
			value = 0x20;
			break;
		case 1:// percussion, bass drum
			printf("BD!!!!!!!!!!!!!!\n");
			value = 0x10;
			break;
		case 2:// percussion, snare drum
			printf("SD!!!!!!!!!!!!!!\n");
			value = 0x08;
			break;
		case 3:// percussion, tom tom
			printf("TT!!!!!!!!!!!!!!\n");
			value = 0x04;
			break;
		case 4:// percussion, cymbal
			printf("CB!!!!!!!!!!!!!!\n");
			value = 0x02;
			break;
		case 5:// percussion, hi hat
			printf("HH!!!!!!!!!!!!!!\n");
			value = 0x01;
			break;
		default:
			printf("DEFAULT!!!!!!!!!!!!!!\n");
			value = 0;
			break;
	}
	SB_WriteFM(0xBD + OpAdr[chan], value);*/
}

static bool A2M_IsEffPortA(A2M_TRACK_EVENT event) {
	int eff0 = event.eff[0].def;
	bool is_p0 = (eff0 == A2M_EF_TONE_PORTAMENTO) ||
				 (eff0 == A2M_EF_TPORT_VOLSLIDE) ||
				 (eff0 == A2M_EF_TPORT_FINE_VOL_SLIDE);
	int eff1 = event.eff[1].def;
	bool is_p1 = (eff1 == A2M_EF_TONE_PORTAMENTO) ||
				 (eff1 == A2M_EF_TPORT_VOLSLIDE) ||
				 (eff1 == A2M_EF_TPORT_FINE_VOL_SLIDE);
	return is_p0 || is_p1;
}

static bool A2M_IsEffNotedelay(A2M_TRACK_EVENT event) {
	return (
			(event.eff[0].def == ef_Extended2 && (event.eff[0].val / 16 == 2)) ||//ef_ex2_NoteDelay
			(event.eff[1].def == ef_Extended2 && (event.eff[1].val / 16 == 2))); //ef_ex2_NoteDelay
}

static inline word A2M_RegsOffsets(int chan, int op) {
	static const byte _ch_n[2][9] = {
			{// mm
			 0x00, 0x01, 0x02, 0x08, 0x09, 0x0A, 0x10, 0x11, 0x12},
			{// pm
			 0x03, 0x04, 0x05, 0x0B, 0x0C, 0x0D, 0x13, 0x14, 0x15}};

	return _ch_n[op][chan];// percussion mode = 0
}

/** A2M :: Change frequency
 *   - A0..8 = Frequency (low 8 bits)
 *   - B0..8 = Key on / octave / freq (high 2 bits)
 *   -  bit 7..6 >> Unused
 *   -  bit 5    >> Key on
 *   -  bit 4..2 >> Octave
 *   -  bit 1..0 >> Freq (2 high bits)
 */
static void A2M_ChangeFreq(int chan, word freq) {

	ch_data->freq_table[chan] = freq & 0x03ff;// 00111111 11111111

	//printf("CH %u >> frequency %x \n", chan, ch_data->freq_table[chan]);
	A2M_WriteFM(0xA0 + chan, ch_data->freq_table[chan] & 0xFF);
	A2M_WriteFM(0xB0 + chan, A2M_ReadFM(0xB0 + chan) | ((ch_data->freq_table[chan] >> 8) & 0x03));
}

/** A2M :: Change frequency and update macro table
 */
static void A2M_ChangeFrequency(int chan, word freq) {

	A2M_ChangeFreq(chan, freq);

	// Update macro table
	ch_data->macro_table[chan].vib_paused = true;
	ch_data->macro_table[chan].vib_count = 1;
	ch_data->macro_table[chan].vib_pos = 0;
	ch_data->macro_table[chan].vib_freq = freq;
	ch_data->macro_table[chan].vib_paused = false;
}

/** A2M :: Key off
 *   - Reg B8..0 :: Key on / octave / freq (high 2 bits)
 *   -  bit 7..6 >> Unused
 *   -  bit 5    >> Key on
 *   -  bit 4..2 >> Octave
 *   -  bit 1..0 >> Freq (2 high bits) 
 */
static void A2M_KeyOFF(int chan) {
	ch_data->freq_table[chan] &= ~0x2000;
	A2M_ChangeFrequency(chan, ch_data->freq_table[chan]);
	ch_data->event_table[chan].note |= 0x80;//keyoff_flag

	//printf("CH %u >> key off\n", chan);

	//A2M_WriteFM(0xb0 + chan, 0);
}

/** A2M :: Key on
 *   - Reg B8..0 :: Key on / octave / freq (high 2 bits)
 *   -  bit 7..6 >> Unused
 *   -  bit 5    >> Key on
 *   -  bit 4..2 >> Octave
 *   -  bit 1..0 >> Freq (2 high bits) 
 */
static void A2M_KeyON(int chan) {
	//printf("CH %u >> key on\n", chan);
	//A2M_WriteFM(0xb0 + chan, A2M_ReadFM(0xb0 + chan) | 0x20);
	A2M_WriteFM(0xb0 + chan, 0);
}

static inline bool A2M_NoteInRange(uint8_t note) {
	return ((note & ~0x80) > 0) && ((note & ~0x80) < 12 * 8 + 1);
}

static int A2M_nFreq(byte note) {
	static int Fnum[13] = {0x156, 0x16b, 0x181, 0x198, 0x1b0, 0x1ca, 0x1e5,
						   0x202, 0x220, 0x241, 0x263, 0x287, 0x2ae};

	if (note >= 12 * 8)
		return (7 << 10) | 0x2ae;//FreqEnd;

	return (note / 12 << 10) | Fnum[note % 12];
}

static void A2M_InitMacroTable(int chan, byte note, byte ins, word freq) {

	//byte arp_table = instrument ? instrument->arpeggio : 0;
	byte arp_table = songinfo->instrinfo.instruments[ins].arpeggio & 0x01;
	ch_data->macro_table[chan].fmreg_pos = 0;
	ch_data->macro_table[chan].fmreg_duration = 0;
	ch_data->macro_table[chan].fmreg_ins = ins;// todo: check against instruments->fmreg.length
	ch_data->macro_table[chan].arpg_count = 1;
	ch_data->macro_table[chan].arpg_pos = 0;
	ch_data->macro_table[chan].arpg_table = arp_table;
	ch_data->macro_table[chan].arpg_note = note;

	byte vib_table = songinfo->instrinfo.instruments[ins].vibrato & 0x01;


	//	A2M_VIBRATO_TABLE *vib = get_vibrato_table(vib_table);

	//	uint8_t vib_delay = vib ? vib->delay : 0;
	byte vib_delay = 0;

	ch_data->macro_table[chan].vib_count = 1;
	ch_data->macro_table[chan].vib_paused = false;
	ch_data->macro_table[chan].vib_pos = 0;
	ch_data->macro_table[chan].vib_table = vib_table;
	ch_data->macro_table[chan].vib_freq = freq;
	ch_data->macro_table[chan].vib_delay = vib_delay;

	ch_data->zero_fq_table[chan] = 0;
}

static void A2M_OutputNote(byte note, uint8_t ins, int chan, bool restart_macro, bool restart_adsr) {
	word freq;

	//printf("Output note on channel %u \n", chan);

	if ((note == 0) && (ch_data->ftune_table[chan] == 0)) return;

	if ((note & 0x80) || !A2M_NoteInRange(note)) {//keyoff_flag
		freq = ch_data->freq_table[chan];
	} else {
		freq = A2M_nFreq(note - 1) + songinfo->instrinfo.instruments[ins].instr_data.fine_tune;

		if (restart_adsr) {
			A2M_KeyON(chan);
			//printf("CH %u >> note %u frequency %x \n", chan, note % 12, freq);
		}

		ch_data->freq_table[chan] |= 0x2000;
	}

	if (ch_data->ftune_table[chan] == -127)
		ch_data->ftune_table[chan] = 0;

	freq = freq + ch_data->ftune_table[chan];
	A2M_ChangeFrequency(chan, freq);
}

static void A2M_NewProcessNote(A2M_TRACK_EVENT event, int chan) {
	bool tporta_flag = A2M_IsEffPortA(event);
	bool notedelay_flag = A2M_IsEffNotedelay(event);

	if (event.note == 0)
		return;

	// This might delay even note-off
	// Or put this after key_off?
	if (notedelay_flag) {
		ch_data->event_table[chan].note = event.note;
		return;
	}

	//printf("CH %u >> Processing note %u \n", chan, event.note);

	if (event.note & 0x80) {// keyoff_flag
		A2M_KeyOFF(chan);
		SB_NoteOff(chan, ch_data->event_table[chan].note % 12, ch_data->event_table[chan].note / 12);
		return;
	}

	if (!tporta_flag) {
		//A2M_OutputNote(event.note, ch_data->voice_table[chan], chan, true, no_swap_and_restart(event));
		//A2M_OutputNote(event.note, ch_data->voice_table[chan], chan, true, true);
		A2M_KeyON(chan);
		SB_NoteOn(chan, event.note % 12, event.note / 12);
		return;
	}

	// if previous note was off'ed or restart_adsr enabled for channel
	// and we are doing portamento to a new note
	if (ch_data->event_table[chan].note & 0x80 || ch_data->portaFK_table[chan]) {//keyoff_flag
		//A2M_OutputNote(ch_data->event_table[chan].note & ~0x80, ch_data->voice_table[chan], chan, false, true);//keyoff_flag
		SB_NoteOn(chan, event.note % 12, event.note / 12);
	} else {
		ch_data->event_table[chan].note = event.note;
	}
}

static void A2M_SetInsData(byte ins, int chan) {
	static const byte _ch_n1[9] = {0x00, 0x01, 0x02, 0x08, 0x09, 0x0A, 0x10, 0x11, 0x012};
	static const byte _ch_n2[9] = {0x03, 0x04, 0x05, 0x0B, 0x0C, 0x0D, 0x13, 0x14, 0x015};
	static A2M_INSTR_DATA zeroins = {{0}, 0, 0, 0};
	const byte _panning[3] = {0x30, 0x10, 0x20};

	if (ins == 0) return;

	byte data[11];
	//raw_fill_from_fmdata(data, &i->fm);

	//if (is_data_empty(i, sizeof(tINSTR_DATA))) {
	//	release_sustaining_sound(chan);
	//}

	// Check if instrument changes from last instrument used
	if ((ins != ch_data->event_table[chan].instr_def) || ch_data->reset_chan[chan]) {

		//printf("- CH %u >> Set instrument %u ", chan, ins);
		ch_data->panning_table[chan] = !ch_data->pan_lock[chan] ? songinfo->instrinfo.instruments[ins].instr_data.panning : songinfo->lock_flags[chan] & 3;

		if (ch_data->panning_table[chan] >= sizeof(_panning))
			ch_data->panning_table[chan] = 0;// various code paths can lead to this value going out of the 0-2 range

		// Set instrument data
		A2M_SetInstrument(chan, songinfo->instrinfo.instruments[ins].instr_data);

		// Update table
		ch_data->fm_table[chan] = songinfo->instrinfo.instruments[ins].instr_data.fm;// Copy struct

		// Stop instr macro if resetting voice
		if (!ch_data->reset_chan[chan])
			ch_data->keyoff_loop[chan] = false;

		if (ch_data->reset_chan[chan]) {
			ch_data->voice_table[chan] = ins;
			//dbg reset_ins_volume(chan);
			ch_data->reset_chan[chan] = false;
		}

		byte note = ch_data->event_table[chan].note & ~0x80;//keyoff_flag;
		//note = A2M_NoteInRange(note) ? note : 0;

		A2M_InitMacroTable(chan, note, ins, ch_data->freq_table[chan]);
	}

	ch_data->voice_table[chan] = ins;
	byte old_ins = ch_data->event_table[chan].instr_def;
	ch_data->event_table[chan].instr_def = ins;

	//if (!ch_data->volume_lock[chan] || (ins != old_ins))
	//	reset_ins_volume(chan);
}

// Fill uint8_t data[11] array from tFM_INST_DATA
static void A2M_RawFillFromFMdata(byte *dst, A2M_FM_INST_DATA *fm) {
	dst[0] = (fm->multipM & 0xf) |
			 ((fm->ksrM & 1) << 4) |
			 ((fm->sustM & 1) << 5) |
			 ((fm->vibrM & 1) << 6) |
			 ((fm->tremM & 1) << 7);
	dst[1] = (fm->multipC & 0xf) |
			 ((fm->ksrC & 1) << 4) |
			 ((fm->sustC & 1) << 5) |
			 ((fm->vibrC & 1) << 6) |
			 ((fm->tremC & 1) << 7);
	dst[2] = (fm->volM & 0x3f) | ((fm->kslM & 3) << 6);
	dst[3] = (fm->volC & 0x3f) | ((fm->kslC & 3) << 6);
	dst[4] = (fm->decM & 0xf) | ((fm->attckM & 0xf) << 4);
	dst[5] = (fm->decC & 0xf) | ((fm->attckC & 0xf) << 4);
	dst[6] = (fm->relM & 0xf) | ((fm->sustnM & 0xf) << 4);
	dst[7] = (fm->relC & 0xf) | ((fm->sustnC & 0xf) << 4);
	dst[8] = fm->wformM & 7;
	dst[9] = fm->wformC & 7;
	dst[10] = (fm->connect & 1) | ((fm->feedb & 7) << 1);
}

static bool A2M_IsChanAddrDataEmpty(int chan) {
	byte data[11];
	A2M_RawFillFromFMdata(data, &ch_data->fm_table[chan]);

	return (
			!data[4] &&
			!data[5] &&
			!data[6] &&
			!data[7]);
}

// inverted volume here
static byte A2M_ScaleVolume(byte volume, byte scale_factor) {
	return 63 - ((63 - volume) * (63 - scale_factor) / 63);
}

// TODO here: fade_out_volume
// inverted volume here
static void A2M_SetInsVolume(byte modulator, byte carrier, byte chan) {

	// ** OPL3 emulation workaround **
	// force muted instrument volume with missing channel ADSR data
	// when there is additionally no FM-reg macro defined for this instrument
	byte fmreg_length = songinfo->instrinfo.instruments[ch_data->voice_table[chan]].fmreg.length;

	if (A2M_IsChanAddrDataEmpty(chan) && !fmreg_length) {
		modulator = 63;
		carrier = 63;
	}

	// Note: fmpar[].volM/volC has pure unscaled volume,
	// modulator_vol/carrier_vol have scaled but without overall_volume
	if (modulator != 0xFF) {// Byte NULL
		byte regm;
		bool is_perc_chan = songinfo->instrinfo.instruments[ch_data->voice_table[chan]].instr_data.fm.connect;

		ch_data->fm_table[chan].volM = modulator;

		if (is_perc_chan) {// in [17..20]
			if (songinfo->volume_scaling)
				modulator = A2M_ScaleVolume(songinfo->instrinfo.instruments[ch_data->voice_table[chan]].instr_data.fm.volM, modulator);

			modulator = A2M_ScaleVolume(modulator, /*scale_volume(*/ 63 - songinfo->global_volume /*, 63 - fade_out_volume)*/);
			regm = A2M_ScaleVolume(modulator, 63 - songinfo->overall_volume) + (ch_data->fm_table[chan].kslM << 6);
		} else {
			regm = modulator + (ch_data->fm_table[chan].kslM << 6);
		}

		A2M_WriteFM(0x40 + A2M_RegsOffsets(chan, 0), regm);

		ch_data->modulator_vol[chan] = 63 - modulator;
	}

	if (carrier != 0xFF) {
		byte regc;

		ch_data->fm_table[chan].volC = carrier;

		if (songinfo->volume_scaling)
			carrier = A2M_ScaleVolume(songinfo->instrinfo.instruments[ch_data->voice_table[chan]].instr_data.fm.volC, carrier);

		carrier = A2M_ScaleVolume(carrier, /*scale_volume(*/ 63 - songinfo->global_volume /*, 63 - fade_out_volume)*/);
		regc = A2M_ScaleVolume(carrier, 63 - songinfo->overall_volume) + (ch_data->fm_table[chan].kslC << 6);

		//opl3out(0x40 + c, regc);
		A2M_WriteFM(0x40 + A2M_RegsOffsets(chan, 1), regc);
		ch_data->carrier_vol[chan] = 63 - carrier;
	}
}

static inline word A2M_Max(word value, word maximum) {
	return (value > maximum ? maximum : value);
}

static void A2M_UpdateCarrierAdsrw(int chan) {
	word c = A2M_RegsOffsets(chan, 1);
	byte data[11];
	A2M_RawFillFromFMdata(data, ch_data->fm_table);

	//opl3out(0x60 + c, data[5]);
	A2M_WriteFM(0x60 + c - 1, data[5]);
	A2M_WriteFM(0x80 + c - 1, data[7]);
	A2M_WriteFM(0xe0 + c - 1, data[8]);
}

static void A2M_SetGlobalVolume(void) {
	for (int chan = 0; chan < songinfo->nm_tracks; chan++) {
		if (ch_data->carrier_vol[chan] || ch_data->modulator_vol[chan]) {
			byte instr_fm_connect = songinfo->instrinfo.instruments[ch_data->voice_table[chan]].instr_data.fm.connect;
			A2M_SetInsVolume(instr_fm_connect ? ch_data->fm_table[chan].volM : 0xFF, ch_data->fm_table[chan].volC, chan);
		}
	}
}

/** A2M_ProcessEffects :: Process line effects
 *  - Do not change any parameter, just process event number and values
 *  - and store them to process them after
 *  - Effect list:
 *     0xy - Arpeggio
 *     1xx - Freq. Slide up (pitch bend up speed)
 *     2xx - Freq. Slide down (pitch bend down speed)
 *     3xx - Tone Portamento
 *     4xy - Vibrato (speed/depth)
 *     5xy - 3xx + Vol Slide (up/down)
 *     6xy - 4xy + Vol Slide (up/down)
 *     7xx & 8xx - Fine Freq. Slide up/down, respectively
 *     9xx - Set modulator volume (Goes up to 3F, dec. 63)
 *     Axy - Vol. Slide (up/down)
 *     Bxx - Position Jump (xx = pattern number)
 *     Cxx - Set intrument Volume (Goes up to 3F, dec. 63)
 *     Dxx - Pattern Break
 *     Exx - Set Tempo
 *     Fxx - Set Speed
 *     Gxy - 3xx + Fine Vol. Slide (up/down)
 *     Hxy - 4xy + Fine Vol. Slide (up/down)
 *     Ixx - Set carrier vol.
 *     Jxx - Set waveform
 *     Kxy - Fine vol. slide (up/down)
 *     Lxx - Retrig. Note (speed)
 *     Mxy - Tremolo (speed/depth)
 *     Nxy - Tremor (vol stays for x frames, fade to 0 for y frames)
 *     Oxy - Arp + Vol. Slide (up/down)
 *     Pxy - Arp + Fine Vol. Slide (up/down)
 *     Qxy - Multi Retrig. Note (Consult AT2's help for more information on this)
 *    Other useful commands:
 *     &2x - note delay by x interval
 *     &3x - note cut after x interval
 *     ZBx - dynamically set panning (0=center, 1=left, 2=right)
 *     %xx - set global volume (up to %3f)
 */
static void A2M_ProcessEffects(Song *song, A2M_TRACK_EVENT event, int slot, int chan) {
	byte def = event.eff[slot].def;
	byte val = event.eff[slot].val;

	ch_data->effect_table[slot][chan].def = def;
	ch_data->effect_table[slot][chan].val = val;

	switch (def) {
		case A2M_EF_ARPEGGIO:// 0xy :: x = semitone offset for tick 1, y = semitone offset for tick 2
			if (val != 0) {
				ch_data->effect_table[slot][chan].def = A2M_EF_ARPEGGIO;
				ch_data->effect_table[slot][chan].val = val;
			}
			break;
		case A2M_EF_FREQ_SLIDE_UP:// 1xx - Freq. Slide up (pitch bend up speed)
			ch_data->effect_table[slot][chan].def = A2M_EF_FREQ_SLIDE_UP;
			ch_data->effect_table[slot][chan].val = val;
			ch_data->fslide_table[slot][chan] = val;
			break;
		case A2M_EF_FREQ_SLIDE_DOWN://2xx - Freq. Slide down (pitch bend down speed)
			ch_data->effect_table[slot][chan].def = A2M_EF_FREQ_SLIDE_DOWN;
			ch_data->effect_table[slot][chan].val = val;
			ch_data->fslide_table[slot][chan] = val;
			break;
		case A2M_EF_TONE_PORTAMENTO://3xx - Tone Portamento
			ch_data->effect_table[slot][chan].def = A2M_EF_TONE_PORTAMENTO;
			ch_data->effect_table[slot][chan].val = val;
			ch_data->porta_table[slot][chan].freq = A2M_nFreq(event.note - 1) + songinfo->instrinfo.instruments[ch_data->event_table[chan].instr_def].instr_data.fine_tune;
			ch_data->porta_table[slot][chan].speed = val;
			break;
		case A2M_EF_VIBRATO://4xy - Vibrato (speed/depth)
			ch_data->effect_table[slot][chan].def = A2M_EF_VIBRATO;
			ch_data->effect_table[slot][chan].val = val;
			ch_data->vibr_table[slot][chan].fine = false;
			ch_data->vibr_table[slot][chan].speed = val / 16;
			ch_data->vibr_table[slot][chan].depth = val % 16;
			break;
		case A2M_EF_TPORT_VOLSLIDE://5xy - 3xx + Vol Slide (up/down)
			ch_data->effect_table[slot][chan].def = A2M_EF_TPORT_VOLSLIDE;
			ch_data->effect_table[slot][chan].val = val;
			break;
		case A2M_EF_VIB_VOLSLIDE://6xy - 4xy + Vol Slide (up/down)
			ch_data->effect_table[slot][chan].def = A2M_EF_VIB_VOLSLIDE;
			ch_data->effect_table[slot][chan].val = val;
			ch_data->vibr_table[slot][chan].fine = false;
			break;
		case A2M_EF_FREQ_SLIDE_UP_FINE://7xx & 8xx - Fine Freq.Slide up / down, respectively
			ch_data->effect_table[slot][chan].def = A2M_EF_FREQ_SLIDE_UP_FINE;
			ch_data->effect_table[slot][chan].val = val;
			ch_data->fslide_table[slot][chan] = val;
			break;
		case A2M_EF_FREQ_SLIDE_DOWN_FINE://7xx & 8xx - Fine Freq.Slide up / down, respectively
			ch_data->effect_table[slot][chan].def = A2M_EF_FREQ_SLIDE_DOWN_FINE;
			ch_data->effect_table[slot][chan].val = val;
			ch_data->fslide_table[slot][chan] = val;
			break;
		case A2M_EF_SET_MOD_VOL://9xx - Set modulator volume (Goes up to 3F, dec. 63)
			ch_data->effect_table[slot][chan].def = A2M_EF_SET_MOD_VOL;
			ch_data->effect_table[slot][chan].val = val;
			break;
		case A2M_EF_VOL_SLIDE://Axy - Vol.Slide(up / down)
			ch_data->effect_table[slot][chan].def = A2M_EF_VOL_SLIDE;
			ch_data->effect_table[slot][chan].val = val;
			break;
		case A2M_EF_POS_JUMP://Bxx - Position Jump (xx = pattern number)
			song->pattern_break = true;
			for (int c = 1; c <= chan; c++) {
				if ((ch_data->loop_table[c][song->current_line] != 0) &&
					(ch_data->loop_table[c][song->current_line] != 0xFF))
					song->pattern_break = false;
			}
			if (song->pattern_break) {
				ch_data->effect_table[slot][chan].def = A2M_EF_POS_JUMP;
				ch_data->effect_table[slot][chan].val = val;
				song->next_line = 0xf0 + chan;//pattern_break_flag
			}
			break;
		case A2M_EF_SET_INS_VOLUME://Cxx - Set instrument Volume (Goes up to 3F, dec. 63)
			ch_data->effect_table[slot][chan].def = A2M_EF_SET_INS_VOLUME;
			ch_data->effect_table[slot][chan].val = val;
			if (event.note == 0) {// Just silent current note, reset instrument so next note must set again the instrument and volume
				ch_data->event_table[chan].instr_def = 0xff;
			}
			break;
		case A2M_EF_PATTERN_BREAK://Dxx - Pattern Break
			ch_data->effect_table[slot][chan].def = A2M_EF_PATTERN_BREAK;
			ch_data->effect_table[slot][chan].val = val;

			//song.pattern_break = true;
			//song.next_line = A2M_Max(val, songinfo->patt_len - 1);

			/*for (int c = 0; c < chan; c++) {
				if ((ch_data->loop_table[c][song.current_line] != 0) &&
					(ch_data->loop_table[c][song.current_line] != 0xFF))
					song.pattern_break = false;
			}*/

			/*if (song.pattern_break) {
				ch_data->effect_table[slot][chan].def = A2M_EF_POS_JUMP;
				ch_data->effect_table[slot][chan].val = val;
				song.next_line = A2M_Max(val, songinfo->patt_len - 1);
			}*/
			break;
		case A2M_EF_SET_TEMPO://Exx - Set Tempo
			ch_data->effect_table[slot][chan].def = A2M_EF_SET_TEMPO;
			ch_data->effect_table[slot][chan].val = val;
			break;
		case A2M_EF_SET_SPEED://Fxx - Set Speed
			ch_data->effect_table[slot][chan].def = A2M_EF_SET_SPEED;
			ch_data->effect_table[slot][chan].val = val;
			break;
		case A2M_EF_TPORT_FINE_VOL_SLIDE://Gxy - 3xx + Fine Vol. Slide (up/down)
			ch_data->effect_table[slot][chan].def = A2M_EF_TPORT_FINE_VOL_SLIDE;
			ch_data->effect_table[slot][chan].val = val;
			break;
		case A2M_EF_VIBR_FINE_VOL_SLIDE://Hxy - 4xy + Fine Vol. Slide (up/down)
			ch_data->effect_table[slot][chan].def = A2M_EF_VIBR_FINE_VOL_SLIDE;
			ch_data->effect_table[slot][chan].val = val;
			break;
		case A2M_EF_SET_CARRIER_VOL://Ixx - Set carrier vol.
			ch_data->effect_table[slot][chan].def = A2M_EF_SET_CARRIER_VOL;
			ch_data->effect_table[slot][chan].val = val;
			break;
		case A2M_EF_SET_WAVEFORM://Jxx - Set waveform
			ch_data->effect_table[slot][chan].def = A2M_EF_SET_WAVEFORM;
			ch_data->effect_table[slot][chan].val = val;
			if (val / 16 <= 7) ch_data->fm_table[chan].wformC = val / 16;
			if (val % 16 <= 7) ch_data->fm_table[chan].wformM = val % 16;
			break;
		case A2M_EF_VOL_SLIDE_FINE://Kxy - Fine vol. slide (up/down)
			ch_data->effect_table[slot][chan].def = A2M_EF_VOL_SLIDE_FINE;
			ch_data->effect_table[slot][chan].val = val;
			break;
		case A2M_EF_RETRIG_NOTE://Lxx - Retrig. Note (speed)
			ch_data->effect_table[slot][chan].def = A2M_EF_RETRIG_NOTE;
			ch_data->effect_table[slot][chan].val = val;
			ch_data->retrig_table[slot][chan] = 1;
			break;
		case A2M_EF_TREMOLO://Mxy - Tremolo (speed/depth)
			ch_data->effect_table[slot][chan].def = A2M_EF_TREMOLO;
			ch_data->effect_table[slot][chan].val = val;
			ch_data->trem_table[slot][chan].speed = val / 16;
			ch_data->trem_table[slot][chan].depth = val % 16;
			ch_data->trem_table[slot][chan].fine = false;
			break;
		case A2M_EF_TREMOR://Nxy - Tremor (vol stays for x frames, fade to 0 for y frames)
			ch_data->effect_table[slot][chan].def = A2M_EF_TREMOR;
			ch_data->effect_table[slot][chan].val = val;
			if (ch_data->last_effect[slot][chan].def != A2M_EF_TREMOR) {
				ch_data->tremor_table[slot][chan].pos = 0;
				ch_data->tremor_table[slot][chan].volM = ch_data->fm_table[chan].volM;
				ch_data->tremor_table[slot][chan].volC = ch_data->fm_table[chan].volC;
			}
			break;
		case A2M_EF_ARPG_VSLIDE:// Oxy - Arp + Vol. Slide (up/down)
			ch_data->effect_table[slot][chan].def = A2M_EF_ARPG_VSLIDE;
			ch_data->effect_table[slot][chan].val = val;
			break;
		case A2M_EF_ARPG_VSLIDE_FINE:// Arp + Fine Vol. Slide (up/down)
			ch_data->effect_table[slot][chan].def = A2M_EF_ARPG_VSLIDE_FINE;
			ch_data->effect_table[slot][chan].val = val;
			break;
		case A2M_EF_MULTI_RETRIG_NOTE://Qxy - Multi Retrig. Note (Consult AT2's help for more information on this)
			ch_data->effect_table[slot][chan].def = A2M_EF_MULTI_RETRIG_NOTE;
			ch_data->effect_table[slot][chan].val = val;
			ch_data->retrig_table[slot][chan] = 1;
			break;
	}
}

static void A2M_PlayLine(Song *song) {
	int current_patern;
	A2M_TRACK_EVENT event;

	current_patern = songinfo->pattern_order[song->current_pattern];

	//printf("Playing order %u, pattern %u, line %u\n", song.current_order, song.current_pattern, song.current_line);

	// Check all channels
	for (int chan = 0; chan < songinfo->nm_tracks; chan++) {
		// save effect_table into last_effect
		for (int slot = 0; slot < 2; slot++) {

			// Save current effect as last executed effect
			if (ch_data->effect_table[slot][chan].def | ch_data->effect_table[slot][chan].val) {
				ch_data->last_effect[slot][chan].def = ch_data->effect_table[slot][chan].def;
				ch_data->last_effect[slot][chan].val = ch_data->effect_table[slot][chan].val;
			}

			// Load queued effect if there is any
			if (ch_data->glfsld_table[slot][chan].def | ch_data->glfsld_table[slot][chan].val) {
				ch_data->effect_table[slot][chan].def = ch_data->glfsld_table[slot][chan].def;
				ch_data->effect_table[slot][chan].val = ch_data->glfsld_table[slot][chan].val;
			} else {
				ch_data->effect_table[slot][chan].def = 0;
				ch_data->effect_table[slot][chan].val = 0;
			}
		}
		// Reset ftune table
		ch_data->ftune_table[chan] = 0;

		// Get current line event
		event = songinfo->pattern[current_patern].channel[chan].events[song->current_line];


		/*for (int slot = 0; slot < 2; slot++) {
			ch_data->event_table[chan].eff[slot].def = event.eff[slot].def;
			ch_data->event_table[chan].eff[slot].val = event.eff[slot].val;
		}*/

		A2M_SetInsData(event.instr_def, chan);

		A2M_ProcessEffects(song, event, 0, chan);
		A2M_ProcessEffects(song, event, 1, chan);
		A2M_UpdateEffectsSlot(song, 0, chan);
		A2M_UpdateEffectsSlot(song, 1, chan);

		//process_effects(event, 1, chan);

		// TODO: is that needed here?
		/*for (int slot = 0; slot < 2; slot++) {
			if (event.eff[slot].def | event.eff[slot].val) {
				ch_data->event_table[chan].eff[slot].def = event.eff[slot].def;
				ch_data->event_table[chan].eff[slot].val = event.eff[slot].val;
			} else if (ch_data->glfsld_table[slot][chan].def == 0 && ch_data->glfsld_table[slot][chan].val == 0) {
				ch_data->effect_table[slot][chan].def = 0;
				ch_data->effect_table[slot][chan].val = 0;
			}
		}*/

		// alters ch->event_table[].note
		A2M_NewProcessNote(event, chan);

		//check_swap_arp_vibr(event, 0, chan);
		//check_swap_arp_vibr(event, 1, chan);

		//update_fine_effects(0, chan);
		//update_fine_effects(1, chan);
	}
}

static void A2M_Arpeggio(int slot, int chan) {
	static byte arpgg_state[3] = {1, 2, 0};

	uint16_t freq;

	switch (ch_data->arpgg_table[slot][chan].state) {
		case 0:
			freq = A2M_nFreq(ch_data->arpgg_table[slot][chan].note - 1);
			break;
		case 1:
			freq = A2M_nFreq(ch_data->arpgg_table[slot][chan].note - 1 + ch_data->arpgg_table[slot][chan].add1);
			break;
		case 2:
			freq = A2M_nFreq(ch_data->arpgg_table[slot][chan].note - 1 + ch_data->arpgg_table[slot][chan].add2);
			break;
		default:
			freq = 0;
	}

	ch_data->arpgg_table[slot][chan].state = arpgg_state[ch_data->arpgg_table[slot][chan].state];
	A2M_ChangeFrequency(chan, freq + songinfo->instrinfo.instruments[ch_data->event_table[chan].instr_def].instr_data.fine_tune);
}

static void A2M_SlideVolumeDown(int chan, byte slide) {
}

static void A2M_SlideVolumeUp(int chan, byte slide) {
}

static void A2M_VolumeSlide(int chan, byte up_speed, byte down_speed) {
	if (up_speed)
		A2M_SlideVolumeUp(chan, up_speed);
	else if (down_speed) {
		A2M_SlideVolumeDown(chan, down_speed);
	}
}

static word A2M_CalcFreqShiftUp(word freq, word shift) {
	word oc = (freq >> 10) & 7;
	int fr = (freq & 0x3ff) + shift;

	if (fr > 0x2ae) {//FreqEnd
		if (oc == 7) {
			fr = 0x2ae;//FreqEnd
		} else {
			oc++;
			fr -= 0x2ae - 0x156;// FreqRange = freqend  -freq start;
		}
	}

	return (word) ((oc << 10) | fr);
}

static word A2M_CalcFreqShiftDown(word freq, word shift) {
	word oc = (freq >> 10) & 7;
	word fr = (freq & 0x3ff) - shift;

	if (fr < 0x156) {//FreqStart
		if (oc == 0) {
			fr = 0x156;//FreqStart
		} else {
			oc--;
			fr += 0x2ae - 0x156;// FreqRange = freqend  -freq start;
		}
	}

	return (word) ((oc << 10) | fr);
}

static void A2M_PortamentoUp(int chan, word slide, word limit) {
	uint16_t freq;

	if ((ch_data->freq_table[chan] & 0x1fff) == 0) return;

	freq = A2M_CalcFreqShiftUp(ch_data->freq_table[chan] & 0x1fff, slide);

	A2M_ChangeFrequency(chan, freq <= limit ? freq : limit);
}

static void A2M_PortamentoDown(int chan, word slide, word limit) {
	word freq;

	if ((ch_data->freq_table[chan] & 0x1fff) == 0) return;

	freq = A2M_CalcFreqShiftDown(ch_data->freq_table[chan] & 0x1fff, slide);

	A2M_ChangeFrequency(chan, freq >= limit ? freq : limit);
}

static void A2M_TonePortamento(int slot, int chan) {
	word freq = ch_data->freq_table[chan] & 0x1fff;
	word portafreq = ch_data->porta_table[slot][chan].freq & 0x1fff;

	if (freq > portafreq) {
		A2M_PortamentoDown(chan, ch_data->porta_table[slot][chan].speed, portafreq);
	} else if (freq < portafreq) {
		A2M_PortamentoUp(chan, ch_data->porta_table[slot][chan].speed, portafreq);
	}
}

/* == calc_vibtrem_shift() in AT2 */
static word A2M_CalcVibratoShift(byte depth, byte position) {
	byte vibr[32] = {
			0, 24, 49, 74, 97, 120, 141, 161, 180, 197, 212, 224, 235, 244, 250, 253, 255,
			253, 250, 244, 235, 224, 212, 197, 180, 161, 141, 120, 97, 74, 49, 24};

	/* ATTENTION: wtf this calculation should be ? */
	return (vibr[position & 0x1f] * depth) >> 6;
}

static void A2M_Vibrato(int slot, int chan) {
	uint16_t freq, slide;
	uint8_t direction;

	freq = ch_data->freq_table[chan];

	ch_data->vibr_table[slot][chan].pos += ch_data->vibr_table[slot][chan].speed;
	slide = A2M_CalcVibratoShift(ch_data->vibr_table[slot][chan].depth, ch_data->vibr_table[slot][chan].pos);
	direction = ch_data->vibr_table[slot][chan].pos & 0x20;

	if (direction == 0)
		A2M_PortamentoDown(chan, slide, A2M_nFreq(0));
	else
		A2M_PortamentoUp(chan, slide, A2M_nFreq(12 * 8 + 1));

	ch_data->freq_table[chan] = freq;
}

static void A2M_Tremolo(int slot, int chan) {
	word slide;
	byte direction;

	byte volM = ch_data->fm_table[chan].volM;
	byte volC = ch_data->fm_table[chan].volC;

	ch_data->trem_table[slot][chan].pos += ch_data->trem_table[slot][chan].speed;
	slide = A2M_CalcVibratoShift(ch_data->trem_table[slot][chan].depth, ch_data->trem_table[slot][chan].pos);
	direction = ch_data->trem_table[slot][chan].pos & 0x20;

	if (direction == 0)
		A2M_SlideVolumeDown(chan, slide);
	else
		A2M_SlideVolumeUp(chan, slide);

	// is this needed?
	ch_data->fm_table[chan].volM = volM;
	ch_data->fm_table[chan].volC = volC;
}

/** A2M_UpdateEffectsSlot :: 
 */
void A2M_UpdateEffectsSlot(Song *song, int slot, int chan) {
	byte def = ch_data->effect_table[slot][chan].def;
	byte val = ch_data->effect_table[slot][chan].val;

	switch (def) {
		case A2M_EF_ARPEGGIO:
			if (!val) break;
			A2M_Arpeggio(slot, chan);
			break;
		case A2M_EF_FREQ_SLIDE_UP:
			A2M_PortamentoUp(chan, val, A2M_nFreq(12 * 8 + 1));
			break;
		case A2M_EF_FREQ_SLIDE_DOWN:
			A2M_PortamentoDown(chan, val, A2M_nFreq(0));
			break;
		case A2M_EF_TONE_PORTAMENTO:
			A2M_TonePortamento(slot, chan);
			break;
		case A2M_EF_VIBRATO:
			if (!ch_data->vibr_table[slot][chan].fine)
				A2M_Vibrato(slot, chan);
			break;
		case A2M_EF_TPORT_VOLSLIDE:
			A2M_VolumeSlide(chan, val / 16, val % 16);
			A2M_TonePortamento(slot, chan);
			break;
		case A2M_EF_VIB_VOLSLIDE:
			A2M_VolumeSlide(chan, val / 16, val % 16);
			if (!ch_data->vibr_table[slot][chan].fine)
				A2M_Vibrato(slot, chan);
			break;
		case A2M_EF_FREQ_SLIDE_UP_FINE:
			A2M_PortamentoUp(chan, ch_data->fslide_table[slot][chan], A2M_nFreq(12 * 8 + 1));
			A2M_VolumeSlide(chan, val / 16, val % 16);
			break;

		case A2M_EF_FREQ_SLIDE_DOWN_FINE:
			A2M_PortamentoDown(chan, ch_data->fslide_table[slot][chan], A2M_nFreq(0));
			A2M_VolumeSlide(chan, val / 16, val % 16);
			break;
		case A2M_EF_SET_MOD_VOL://9xx - Set modulator volume (Goes up to 3F, dec. 63)
			A2M_SetInsVolume(63 - val, 0xFF, chan);
			break;
		case A2M_EF_VOL_SLIDE:
			A2M_VolumeSlide(chan, val / 16, val % 16);
			break;
		case A2M_EF_POS_JUMP://Bxx - Position Jump (xx = pattern number)
			break;
		case A2M_EF_SET_INS_VOLUME:
			//if (songinfo->percussion_mode && ((chan >= 16) && (chan <= 19))) {//  in [17..20]
			if (songinfo->percussion_mode) {//  in [17..20]
				//printf("CH %u >> set ins volume percussion, note %u \n", chan, event.note);
				A2M_SetInsVolume(63 - val, 0xFF, chan);
				//A2M_SetInsVolume(val, 0xFF, chan);
			} else if (songinfo->instrinfo.instruments[ch_data->voice_table[chan]].instr_data.fm.connect == 0) {
				//printf("CH %u >> set ins volume connect 0, note %u \n", chan, event.note);
				A2M_SetInsVolume(0xFF, 63 - val, chan);
				//A2M_SetInsVolume(0xFF, val, chan);
			} else {
				//printf("CH %u >> set ins volume , note %u \n", chan, event.note);
				A2M_SetInsVolume(63 - val, 63 - val, chan);
				//A2M_SetInsVolume(val, val, chan);
			}
			break;
		case A2M_EF_PATTERN_BREAK:
			song->pattern_break = true;
			song->next_line = A2M_Max(val, songinfo->patt_len - 1);
			song->next_line = song->next_line | A2M_PATTERN_BREAK_FLAG;
			break;
		case A2M_EF_SET_TEMPO:
			song->tempo = val;
			break;
		case A2M_EF_SET_SPEED:
			song->speed = val;
			break;
		case A2M_EF_TPORT_FINE_VOL_SLIDE:
			A2M_TonePortamento(slot, chan);
			break;
		case A2M_EF_VIBR_FINE_VOL_SLIDE:
			if (!ch_data->vibr_table[slot][chan].fine)
				A2M_Vibrato(slot, chan);
			break;
		case A2M_EF_SET_CARRIER_VOL:
			A2M_SetInsVolume(0xFF, 63 - val, chan);
			break;
		case A2M_EF_SET_WAVEFORM:
			if (val / 16 <= 7) {// in [0..7]
				//ch_data->fm_table[chan].wformC = val / 16;
				A2M_UpdateCarrierAdsrw(chan);
			}
			if (val % 16 <= 7) {// in [0..7]
				//ch_data->fm_table[chan].wformM = val % 16;
				A2M_UpdateCarrierAdsrw(chan);
			}
			break;
		case A2M_EF_VOL_SLIDE_FINE:
			A2M_SetInsVolume(0xFF, 63 - val, chan);
			break;
		case A2M_EF_RETRIG_NOTE:
			if (ch_data->retrig_table[slot][chan] >= val) {
				ch_data->retrig_table[slot][chan] = 0;
				A2M_OutputNote(ch_data->event_table[chan].note, ch_data->event_table[chan].instr_def, chan, true, true);
			} else {
				ch_data->retrig_table[slot][chan]++;
			}
			break;
		case A2M_EF_TREMOLO:
			if (!ch_data->trem_table[slot][chan].fine)
				A2M_Tremolo(slot, chan);
			break;
		case A2M_EF_TREMOR:
			if (ch_data->tremor_table[slot][chan].pos >= 0) {
				if ((ch_data->tremor_table[slot][chan].pos + 1) <= val / 16) {
					ch_data->tremor_table[slot][chan].pos++;
				} else {
					A2M_SlideVolumeDown(chan, 63);
					ch_data->tremor_table[slot][chan].pos = -1;
				}
			} else {
				if ((ch_data->tremor_table[slot][chan].pos - 1) >= -(val % 16)) {
					ch_data->tremor_table[slot][chan].pos--;
				} else {
					//set_ins_volume(ch->tremor_table[slot][chan].volM, ch->tremor_table[slot][chan].volC, chan);
					ch_data->tremor_table[slot][chan].pos = 1;
				}
			}
			break;
		case A2M_EF_ARPG_VSLIDE:
			A2M_VolumeSlide(chan, val / 16, val % 16);
			A2M_Arpeggio(slot, chan);
			break;
		case A2M_EF_ARPG_VSLIDE_FINE:
			A2M_Arpeggio(slot, chan);
			break;
		case A2M_EF_MULTI_RETRIG_NOTE:
			if (ch_data->retrig_table[slot][chan] >= val / 16) {
				switch (val % 16) {
					case 0:
						break;
					case 8:
						break;

					case 1:
						A2M_SlideVolumeDown(chan, 1);
						break;
					case 2:
						A2M_SlideVolumeDown(chan, 2);
						break;
					case 3:
						A2M_SlideVolumeDown(chan, 4);
						break;
					case 4:
						A2M_SlideVolumeDown(chan, 8);
						break;
					case 5:
						A2M_SlideVolumeDown(chan, 16);
						break;

					case 9:
						A2M_SlideVolumeUp(chan, 1);
						break;
					case 10:
						A2M_SlideVolumeUp(chan, 2);
						break;
					case 11:
						A2M_SlideVolumeUp(chan, 4);
						break;
					case 12:
						A2M_SlideVolumeUp(chan, 8);
						break;
					case 13:
						A2M_SlideVolumeUp(chan, 16);
						break;

					case 6:
						//A2M_SlideVolumeDown(chan, chanvol(chan) - chanvol(chan) * 2 / 3);
						break;

					case 7:
						//A2M_SlideVolumeDown(chan, chanvol(chan) - chanvol(chan) * 1 / 2);
						break;

					case 14:
						//A2M_SlideVolumeUp(chan, max(chanvol(chan) * 3 / 2 - chanvol(chan), 63));
						break;

					case 15:
						//A2M_SlideVolumeUp(chan, max(chanvol(chan) * 2 - chanvol(chan), 63));
						break;
				}

				ch_data->retrig_table[slot][chan] = 0;
				A2M_OutputNote(ch_data->event_table[chan].note, ch_data->event_table[chan].instr_def, chan, true, true);
			} else {
				ch_data->retrig_table[slot][chan]++;
			}
			break;
	}
}

/** A2M_UpdateEffects ::
 */
static void A2M_UpdateEffects(Song *song) {
	for (int chan = 0; chan < songinfo->nm_tracks; chan++) {
		A2M_UpdateEffectsSlot(song, 0, chan);
		A2M_UpdateEffectsSlot(song, 1, chan);
	}
}

/** A2M_SetCurrentOrder :: Set new pattern to be played
 *   - Ends song if no loop and new order is > 0x80
 *   - max pattern number = 0x7F
 */
static void A2M_SetCurrentOrder(Song *song, byte new_order) {
	song->current_order = new_order < 0x80 ? new_order : 0;
	/*if (songinfo->pattern_order[song.current_order] < 0x80)
		return;*/
}

/** A2M_UpdateSongPosition :: Updates song line and pattern position
 *  - Controls pattern breaks
 *  - Controls song end
 *  - Controls song jumps and loops
 *  - Controls speed changes
 */
void A2M_UpdateSongPosition(Song *song) {
	bool do_pattern_loop, do_position_jump, do_next_pattern;
	byte next_line, old_order, new_order;

	// Just next line
	if ((song->current_line < songinfo->patt_len) && !song->pattern_break) {
		song->current_line++;

		// End of pattern
		if (song->current_line > songinfo->patt_len - 1) {
			song->pattern_break = true;
			song->next_line = 0 | A2M_PATTERN_BREAK_FLAG;
		}
	}

	// Pattern break
	if (song->pattern_break) {
		do_position_jump = song->pattern_break && ((song->next_line & 0xF0) == A2M_PATTERN_JUMP_FLAG);
		do_pattern_loop = song->pattern_break && ((song->next_line & 0xF0) == A2M_PATTERN_LOOP_FLAG);
		do_next_pattern = song->pattern_break && ((song->next_line & 0xF0) == A2M_PATTERN_BREAK_FLAG);

		if (do_position_jump) {
			// A bit overkill to clean arrays here, better do it in the end of pattern loop
			memset(ch_data->loopbck_table, 0xff, sizeof(ch_data->loopbck_table));
			memset(ch_data->loop_table, 0xff, sizeof(ch_data->loop_table));

			// Bxx - order position jump
			old_order = song->current_order;
			next_line = song->next_line - A2M_PATTERN_BREAK_FLAG;
			new_order = song->next_order;
			A2M_SetCurrentOrder(song, new_order);
			song->songend = song->current_order <= old_order ? true : false;
		}

		if (do_pattern_loop) {
			// ZCx, ZDx - loop back
			next_line = song->next_line - A2M_PATTERN_LOOP_FLAG;
			song->next_line = next_line;

			/*if (ch_data->loop_table[chan][song->current_line] != 0)
				ch_data->loop_table[chan][song->current_line]--;*/
		}

		if (do_next_pattern) {
			new_order = song->current_order < 0x7f ? song->current_order + 1 : 0;
			A2M_SetCurrentOrder(song, new_order);
			song->next_line = 0;
		}

		if ((songinfo->pattern_order[song->current_order] > 0x7f)) {
			song->current_order = 0;
			song->next_line = 0;
		}

		song->current_pattern = songinfo->pattern_order[song->current_order];
		song->current_line = song->next_line;

		song->pattern_break = false;
	}

	// Speed update
	if (song->speed_update && song->current_line == 0) {
		song->speed = songinfo->speed;
	}

	// Clear table
	for (int chan = 0; chan < songinfo->nm_tracks; chan++) {
		ch_data->glfsld_table[0][chan].def = 0;
		ch_data->glfsld_table[0][chan].val = 0;
		ch_data->glfsld_table[1][chan].def = 0;
		ch_data->glfsld_table[1][chan].val = 0;
	}
}

static void global_volume_slide(byte up_speed, byte down_speed) {
	if (up_speed != 0xFF)
		songinfo->global_volume = A2M_Max(songinfo->global_volume + up_speed, 63);

	if (down_speed != 0xFF) {
		if (songinfo->global_volume >= down_speed)
			songinfo->global_volume -= down_speed;
		else
			songinfo->global_volume = 0;
	}

	A2M_SetGlobalVolume();
}

static void A2M_UpdateExtraFineEffectsSlot(int slot, int chan) {
	uint8_t def = ch_data->effect_table[slot][chan].def;
	uint8_t val = ch_data->effect_table[slot][chan].val;

	switch (def) {
		case ef_Extended2:
			switch (val / 16) {
				case ef_ex2_GlVolSldUpXF:
					global_volume_slide(val & 0xf, 0xFF);
					break;
				case ef_ex2_GlVolSldDnXF:
					global_volume_slide(0xFF, val & 0xf);
					break;
				case ef_ex2_VolSlideUpXF:
					A2M_VolumeSlide(chan, val & 0xf, 0);
					break;
				case ef_ex2_VolSlideDnXF:
					A2M_VolumeSlide(chan, 0, val & 0xf);
					break;
				case ef_ex2_FreqSlideUpXF:
					A2M_PortamentoUp(chan, val & 0xf, A2M_nFreq(12 * 8 + 1));
					break;
				case ef_ex2_FreqSlideDnXF:
					A2M_PortamentoDown(chan, val & 0xf, A2M_nFreq(0));
					break;
			}
			break;

		case ef_GlobalFreqSlideUpXF:
			A2M_PortamentoUp(chan, val, A2M_nFreq(12 * 8 + 1));
			break;
		case ef_GlobalFreqSlideDnXF:
			A2M_PortamentoDown(chan, val, A2M_nFreq(0));
			break;
		case ef_ExtraFineArpeggio:
			A2M_Arpeggio(slot, chan);
			break;
		case ef_ExtraFineVibrato:
			if (!ch_data->vibr_table[slot][chan].fine) A2M_Vibrato(slot, chan);
			break;
		case ef_ExtraFineTremolo:
			if (!ch_data->trem_table[slot][chan].fine) A2M_Tremolo(slot, chan);
			break;
	}
}

static void A2M_UpdateExtraFineEffects() {
	for (int chan = 0; chan < songinfo->nm_tracks; chan++) {
		A2M_UpdateExtraFineEffectsSlot(0, chan);
		A2M_UpdateExtraFineEffectsSlot(1, chan);
	}
}

/** A2M_Process :: Main process
 */
void A2M_Process(Song *song) {
	//printf("process\n");
	if (song->pattern_delay) {
		//printf("Delay detected, just update efects\n");
		A2M_UpdateEffects(song);
		if (song->tickD > 1) {
			song->tickD--;
		} else {
			song->pattern_delay = false;
		}
	} else {
		if (song->ticks == 0) {
			//printf("playing line %u \n", song->current_line);
			A2M_PlayLine(song);
			song->ticks = song->speed;
			A2M_UpdateSongPosition(song);
		}
		//A2M_UpdateEffects();
		song->ticks--;
	}

	song->tickXF++;
	if (song->tickXF % 4 == 0) {
		//printf("Update extra fine effects\n");
		A2M_UpdateExtraFineEffects();
		song->tickXF -= 4;
	}
}

/** A2M_MacroProcess :: WIP!!
 */
void A2M_MacroProcess(void) {
}


void opl_write(unsigned char reg, unsigned char val) {
	outp(0x388, reg);
	inp(0x388);
	inp(0x388);
	inp(0x388);
	inp(0x388);

	outp(0x389, val);
	for (int i = 0; i < 35; i++)
		inp(0x388);
}

void A2M_Stop(void) {
	for (int ch = 0; ch < 9; ch++) {
		opl_write(0xB0 + ch, 0x00);// Key off + block + fnum alto = 0
		opl_write(0xA0 + ch, 0x00);// fnum bajo = 0
	}

	for (int op = 0; op < 18; op++)
		opl_write(0x40 + op, 0x3F);

	int op_table[9][2] = {
			{0, 3},
			{1, 4},
			{2, 5},
			{6, 9},
			{7, 10},
			{8, 11},
			{12, 15},
			{13, 16},
			{14, 17}};

	for (int ch = 0; ch < 9; ch++) {
		int carrier = op_table[ch][1];
		opl_write(0x40 + carrier, 0x3F);// SOLO el carrier importa para volumen
	}

	for (int ch = 0; ch < 9; ch++) {
		int carrier = op_table[ch][1];
		opl_write(0x80 + carrier, 0xFF);// sustain=0, release rápido
	}

	for (int ch = 0; ch < 9; ch++) {
		opl_write(0xB0 + ch, 0x00);
		opl_write(0xA0 + ch, 0x00);
	}
	opl_write(0xBD, 0x00);
}


/** A2M_TimerHandler :: Timer callback
 *   - Count each timer tick and trigger
 *	   next process depending on song tempo
 */
void A2M_TimerHandler(Song *song) {
	int regNum;
	if (song->play) {
		song->stop = false;
		// Process tick
		if (song->ticklooper == 0) {
			A2M_Process(song);
		}
		if (song->macro_ticklooper == 0) {
			A2M_MacroProcess();
		}

		// Next tick
		song->ticklooper++;
		song->macro_ticklooper++;

		// Reset ticks
		if (song->ticklooper >= (TIMER_AUDIO_TIME / songinfo->tempo))//IRQ_freq/tempo
			song->ticklooper = 0;
		if (song->macro_ticklooper >= (TIMER_AUDIO_TIME / (songinfo->tempo * song->macro_speedup)))//IRQ_freq / (tempo * _macro_speedup()))
			song->macro_ticklooper = 0;
	} else {
		song->stop = true;
	}
}
