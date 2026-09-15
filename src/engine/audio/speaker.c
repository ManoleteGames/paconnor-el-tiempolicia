#include "../engine.h"
#include "audio.h"
#include "speaker.h"

static byte menu[12] = {56, 1, 52, 1, 51, 1, 45, 1, 99, 99};
static byte select[32] = {90, 1, 80, 1, 70, 1, 60, 1, 50, 1, 30, 1, 10, 1, 5, 1, 10, 1, 30, 1, 50, 1, 60, 1, 70, 1, 60, 1, 99, 99};
static byte open[32] = {90, 1, 80, 1, 70, 1, 60, 1, 50, 1, 30, 1, 10, 1, 5, 1, 10, 1, 30, 1, 50, 1, 60, 1, 70, 1, 60, 1, 99, 99};
static byte crash[32] = {69, 1, 3, 1, 120, 1, 32, 1, 39, 1, 200, 1, 20, 1, 60, 1, 16, 1, 106, 1, 12, 1, 87, 1, 8, 1, 70, 1, 99, 99};
static byte jump[32] = {30, 1, 35, 1, 40, 1, 43, 1, 44, 1, 45, 1, 46, 1, 47, 1, 48, 1, 49, 1, 50, 1, 51, 1, 52, 1, 53, 1, 99, 99};
//static byte getItem[32] = {69, 1, 70, 1, 71, 1, 72, 1, 69, 1, 64, 1, 58, 1, 40, 1, 30, 1, 12, 1, 12, 1, 16, 1, 20, 1, 70, 1, 99, 99};
static byte getItem[32] = {69, 1, 70, 1, 71, 1, 72, 1, 69, 1, 64, 1, 58, 1, 40, 1, 30, 1, 99, 99};
static byte punch[16] = {43, 3, 32, 3, 28, 5, 99, 99, 0, 0, 0, 0, 0, 0, 0, 0};
static byte kick[16] = {40, 3, 28, 3, 23, 5, 99, 99, 0, 0, 0, 0, 0, 0, 0, 0};
static byte pistol[16] = {48, 2, 45, 2, 0, 2, 0, 2, 0, 2, 43, 2, 99, 99, 0, 0};
//static byte shootgun[16] = {32, 2, 28, 2, 23, 2, 15, 2, 11, 2, 3, 2, 99, 99, 0, 0};
static byte shootgun[16] = {32, 2, 28, 2, 23, 2, 0, 2, 0, 2, 0, 2, 99, 99, 0, 0};
static byte uzi[16] = {62, 2, 59, 2, 56, 2, 43, 2, 99, 99, 0, 0, 0, 0, 0, 0};
static byte snipper[16] = {62, 2, 56, 2, 54, 2, 51, 2, 28, 4, 26, 3, 99, 99, 0, 0};
static byte explossion[35] = {43, 3, 42, 3, 41, 3, 40, 3, 39, 3, 38, 3, 37, 3, 36, 3, 35, 3, 34, 3, 32, 3, 30, 3, 28, 3, 27, 3, 26, 6, 24, 3, 99, 99, 0};
static byte dead[35] = {88, 3, 0, 1, 89, 2, 0, 1, 90, 2, 0, 1, 91, 2, 0, 1, 92, 2, 0, 1, 93, 2, 0, 1, 94, 2, 0, 1, 99, 99, 0, 0, 0, 0, 0};
static byte fail[16] = {38, 1, 99, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static byte reload_pistol[16] = {48, 3, 0, 3, 43, 3, 99, 99, 0, 0, 0, 0, 0, 0, 0, 0};
//static byte reload_shootgun[16] = {32, 3, 0, 3, 28, 3, 99, 99, 0, 0, 0, 0, 0, 0, 0, 0};
static byte reload_shootgun[16] = {48, 3, 0, 1, 48, 3, 99, 99, 0, 0, 0, 0, 0, 0, 0, 0};

static byte tick[16] = {56, 1, 99, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static byte tack[16] = {51, 1, 99, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


/*
	NOTE VALUES
	-----------
	Octave 0    1    2    3    4    5    6    7  
	Note
	 C     0    12   24   36   48   60   72   84 
	 C#    1    13   25   37   49   61   73   85 
	 D     2    14   26   38   50   62   74   86 
	 D#    3    15   27   39   51   63   75   87 
	 E     4    16   28   40   52   64   76   88 
	 F     5    17   29   41   53   65   77   89 
	 F#    6    18   30   42   54   66   78   90 
	 G     7    19   31   43   55   67   79   91 
	 G#    8    20   32   44   56   68   80   92 
	 A     9    21   33   45   57   69   81   93 
	 A#    10   22   34   46   58   70   82   94 
	 B     11   23   35   47   59   71   83   95 
*/
//1193180/Value
static int speaker_note[96] = {
		// mute
		//value = 1.9MHz/f
		//Note	    C    C#     D    D#     E     F    F#     G    G#     A    A#     B
		//Octave
		/*0 */ 65535,
		65535,
		65535,
		62799,
		56818,
		54235,
		51877,
		49716,
		45892,
		44192,
		41144,
		38490,
		/*1   */ 36157,
		34091,
		32248,
		30594,
		29102,
		27118,
		25939,
		24351,
		22946,
		21694,
		20572,
		19245,
		/*2   */ 18357,
		17292,
		16345,
		15297,
		14551,
		13715,
		12969,
		12175,
		11473,
		10847,
		10286,
		9701,
		/*3   */ 9108,
		8584,
		8117,
		7698,
		7231,
		6818,
		6450,
		6088,
		5736,
		5424,
		5121,
		4870,
		/*4   */ 4554,
		4308,
		4058,
		3837,
		3616,
		3419,
		3225,
		3044,
		2875,
		2712,
		2560,
		2415,
		/*5	*/ 2281,
		2154,
		2033,
		1918,
		1811,
		1709,
		1612,
		1522,
		1436,
		1356,
		1280,
		1208,
		/*6	*/ 1141,
		1076,
		1015,
		959,
		898,
		854,
		806,
		761,
		718,
		678,
		640,
		604,
		/*7	*/ 570,
		538,
		508,
		479,
		452,
		427,
		403,
		380,
		359,
		339,
		320,
		302,
};

void SPK_Init(void) {

	// Set timmer channel 2
	outportb(SPK_PIT_PORT, 0xB6);//1011 0110; Channel 2; access lobyte/high byte; mode 3 square wave; 16-bit binary
}

void SPK_PlaySound(SoundEffect *sound, int number) {
	switch (number) {
		case AUDIO_OPEN_DOOR_EFFECT:
			sound->buffer = &open[0];
			break;
		case AUDIO_PUNCH_EFFECT:
			sound->buffer = &punch[0];
			break;
		case AUDIO_KICK_EFFECT:
			sound->buffer = &kick[0];
			break;
		case AUDIO_GUN_EFFECT:
			sound->buffer = &pistol[0];
			break;
		case AUDIO_RELOAD_PISTOL_EFFECT:
			sound->buffer = &reload_pistol[0];
			break;
		case AUDIO_RELOAD_SHOOTGUN_EFFECT:
			sound->buffer = &reload_shootgun[0];
			break;
		case AUDIO_RELOAD_UZI_EFFECT:
			sound->buffer = &reload_pistol[0];
			break;
		case AUDIO_RELOAD_SNIPPER_EFFECT:
			sound->buffer = &reload_shootgun[0];
			break;
		case AUDIO_SHOTGUN_EFFECT:
			sound->buffer = &shootgun[0];
			break;
		case AUDIO_UZI_EFFECT:
			sound->buffer = &uzi[0];
			break;
		case AUDIO_SNIPPER_EFFECT:
			sound->buffer = &snipper[0];
			break;
		case AUDIO_EXPLOSSION:
			sound->buffer = &explossion[0];
			break;
		case AUDIO_ACTOR_DEAD:
			sound->buffer = &dead[0];
			break;
		case AUDIO_SHOT_FAIL_EFFECT:
			sound->buffer = &fail[0];
			break;
		case AUDIO_TICK_EFFECT:
			sound->buffer = &tick[0];
			break;
		case AUDIO_TACK_EFFECT:
			sound->buffer = &tack[0];
			break;
		case AUDIO_JUMP_EFFECT:
			sound->buffer = &jump[0];
			break;
		case AUDIO_CRASH_EFFECT:
			sound->buffer = &crash[0];
			break;
		case AUDIO_MENU_EFFECT:
			sound->buffer = &menu[0];
			break;
		case AUDIO_SELECT_EFFECT:
			sound->buffer = &select[0];
			break;
		case AUDIO_GET_ITEM_EFFECT:
			sound->buffer = &getItem[0];
			break;
		default:
			sprintf(engine.system_error_message1, "SPK_PlaySound function error");
			sprintf(engine.system_error_message2, "Undefined sound number %u", number);
			sprintf(engine.system_error_message3, "");
			Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_SOUND);
			break;
	}
	sound->note_time = 0;
	sound->note_index = 0;
	sound->playing_note = false;
	sound->play = true;
}

void SPK_Handler(SoundEffect *sound, Song *song) {

	byte duration, raw_note, note, sustain, octave;
	int note_index;

	//////////////////// SOUND MANAGEMENT ////////////////////
	// Send note
	if (sound->play & !sound->playing_note) {
		sound->note = sound->buffer[sound->note_index];
		sound->note_duration = sound->buffer[sound->note_index + 1];

		if (sound->note == 99) {
			sound->play = false;
			SPK_Mute();
		} else {
			if (sound->note != 0) {
				SPK_Unmute();
				SPK_Sound(sound->note);
			} else
				SPK_Mute();

			sound->playing_note = true;
		}
	}

	// Wait note time
	if (sound->play & sound->playing_note) {
		sound->note_time++;
		if (sound->note_time > sound->note_duration) {
			sound->note_index += 2;
			sound->note_time = 0;
			sound->playing_note = false;
			SPK_Mute();
		}
	}

	//////////////////// SONG MANAGEMENT ////////////////////
	if (song->play) {
		song->stop = false;

		song->current_order = song->current_line * 4;

		duration = song->buffer[song->current_order] - 48;
		raw_note = song->buffer[song->current_order + 1];
		sustain = song->buffer[song->current_order + 2];
		octave = song->buffer[song->current_order + 3] - 48;

		if (song->ticklooper < duration) {// current note being played
			song->ticklooper++;
		} else {// load new note
			song->ticklooper = 0;
			song->current_line++;
		}

		// not end of song yet
		if (sustain != 70) {// ASCII CHAR 'F'
			switch (raw_note) {
				case 67:                            //C
					if (sustain == 35) { note = 2; }// #
					else {
						note = 1;
					}
					break;
				case 68:                            //D
					if (sustain == 35) { note = 4; }// #
					else {
						note = 3;
					}
					break;
				case 69://E
					note = 5;
					break;
				case 70:                            //F
					if (sustain == 35) { note = 7; }// #
					else {
						note = 6;
					}
					break;
				case 71:                            //G
					if (sustain == 35) { note = 9; }// #
					else {
						note = 8;
					}
					break;
				case 65:                             //A
					if (sustain == 35) { note = 11; }// #
					else {
						note = 10;
					}
					break;
				case 66://B
					note = 12;
					break;
				default:
					note = 0;
					break;
			}

			note_index = (note - 1) + (octave * 12);

			// no sound being player
			if (sound->playing_note == 0) {
				if (note == 0) {
					SPK_Mute();
				} else {
					SPK_Unmute();
					SPK_Sound(note_index);
				}
			}
		} else {
			if (song->loop) {
				song->ticklooper = 0;
				song->current_line = 0;
			} else {
				song->play = false;
				song->stop = true;
				SPK_Mute();
			}
		}
	} else {
		song->stop = true;
	}
}

void SPK_Unmute(void) {
	byte data;
	// Unmute speaker
	// When audio is off, all functions are linked to speaker but buzzer will be not activated
	data = inportb(0x61);
	outportb(0x61, data | 3);
}

void SPK_Mute(void) {
	byte data;
	// Unmute speaker
	// When audio is off, all functions are linked to speaker but buzzer will be not activated
	data = inportb(0x61);
	outportb(0x61, data & 0xFC);
}

void SPK_Sound(int note) {
	//I/O port     Usage
	//0x40         Channel 0 data port (read/write) for interrupts
	//0x41         Channel 1 data port (read/write) unused
	//0x42         Channel 2 data port (read/write) for pcspeaker
	//0x43         Mode/Command register (write only, a read is ignored)

	// MODE/COMMAND 0x43
	// Bits         Usage
	// 7 and 6      Select channel :
	//        0 0 = Channel 0
	//        0 1 = Channel 1
	//        1 0 = Channel 2
	//        1 1 = Read-back command (8254 only)
	// 5 and 4      Access mode :
	//        0 0 = Latch count value command
	//        0 1 = Access mode: lobyte only
	//        1 0 = Access mode: hibyte only
	//        1 1 = Access mode: lobyte/hibyte
	// 3 to 1       Operating mode :
	//        0 0 0 = Mode 0 (interrupt on terminal count)
	//        0 0 1 = Mode 1 (hardware re-triggerable one-shot)
	//        0 1 0 = Mode 2 (rate generator)
	//        0 1 1 = Mode 3 (square wave generator)
	//        1 0 0 = Mode 4 (software triggered strobe)
	//        1 0 1 = Mode 5 (hardware triggered strobe)
	//        1 1 0 = Mode 2 (rate generator, same as 010b)
	//        1 1 1 = Mode 3 (square wave generator, same as 011b)
	// 0            BCD/Binary mode: 0 = 16-bit binary, 1 = four-digit BCD
	//outportb(SPK_PIT_PORT, 0xB6);//1011 0110; Channel 2; access lobyte/high byte; mode 3 square wave; 16-bit binary
	outportb(SPK_PIT_CH, (byte) speaker_note[note] & 0xFF);
	outportb(SPK_PIT_CH, (byte) (speaker_note[note] >> 8) & 0xFF);

	outportb(0x61, inportb(0x61) | 0x03);
}

void SPK_NoSound(void) {
	//outportb(SPK_PIT_CH, 0);
	//outportb(SPK_PIT_CH, 0);

	outportb(0x61, inportb(0x61) & 0x03);
}

void SPK_LoadSong(Song *song, int number) {
	switch (number) {
		case 1:// Song 1
			FILE_LoadSpeakerSong("MUSICSPK.DAT", "spk_song1.mus", song);
			song->loaded = true;
			break;
		case 2:// Song 2
			FILE_LoadSpeakerSong("MUSICSPK.DAT", "spk_song2.mus", song);
			song->loaded = true;
			break;
		case 3:// Song 3
			FILE_LoadSpeakerSong("MUSICSPK.DAT", "spk_song3.mus", song);
			song->loaded = true;
			break;
		case 4:// Song 4
			FILE_LoadSpeakerSong("MUSICSPK.DAT", "spk_song4.mus", song);
			song->loaded = true;
			break;
		case 5:// Song 5
			FILE_LoadSpeakerSong("MUSICSPK.DAT", "spk_song5.mus", song);
			song->loaded = true;
			break;
		case 6:// Song 6
			FILE_LoadSpeakerSong("MUSICSPK.DAT", "spk_song6.mus", song);
			song->loaded = true;
			break;
		case 7:// Song 7
			FILE_LoadSpeakerSong("MUSICSPK.DAT", "spk_song7.mus", song);
			song->loaded = true;
			break;
		case 8:// Song 8
			FILE_LoadSpeakerSong("MUSICSPK.DAT", "spk_song8.mus", song);
			song->loaded = true;
			break;
		default:
			sprintf(engine.system_error_message1, "SPK_LoadSong function error");
			sprintf(engine.system_error_message2, "Undefined speaker music number %u", number);
			sprintf(engine.system_error_message3, "");
			Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_SOUND);
			break;
	}
}

void SPK_PlaySong(Song *song) {
	if (song->loaded) {
		song->play = true;
		song->current_line = 0;
	}
}

void SPK_StopSong(Song *song) {
	song->play = false;
	SPK_Mute();
}

void SPK_PauseSong(Song *song) {
	song->play = false;
	SPK_Mute();
}

void SPK_ResumeSong(Song *song) {
	if (song->loaded) {
		song->play = true;
	}
}