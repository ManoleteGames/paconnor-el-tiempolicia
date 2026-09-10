#include <stdio.h>
#include "../engine.h"
#include "audio.h"
#include "adlib.h"
#include "speaker.h"
#include "sblaster.h"
#include "a2m.h"

static SoundEffect sound_effect;
static Song song;

void AUDIO_Init(void) {
	if (!engine.audio_initialized) {

		A2M_Init();

		switch (settings.sound_device) {
			case AUDIO_DEVICE_NONE:
				break;
			case AUDIO_DEVICE_SPEAKER:// PC Speaker;
				SPK_Init();

				// Allocate mem for song buffer
				song.buffer = MM_PushChunk(AUDIO_MAX_SONG_SIZE, CT_MUSIC);
				song.buffer_max_size = AUDIO_MAX_SONG_SIZE;

				break;
			case AUDIO_DEVICE_ADLIB:// Adlib
				if (!engine.adlib_present) {
					sprintf(engine.system_error_message1, "AUDIO_Init function error");
					sprintf(engine.system_error_message2, "Adlib card not present");
					sprintf(engine.system_error_message3, "Please run setup.exe to identify your audio card");
					Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_SOUND);
				}
				ADLIB_Init();
				break;
			case AUDIO_DEVICE_SBLASTER:// Sound blaster
				if (!engine.sb_present) {
					sprintf(engine.system_error_message1, "AUDIO_Init function error");
					sprintf(engine.system_error_message2, "Sound blaster card not present");
					sprintf(engine.system_error_message3, "Please run setup.exe to identify your audio card");
					Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_SOUND);
				}
				SB_Init();
				break;
			default:
				sprintf(engine.system_error_message1, "AUDIO_Init function error");
				sprintf(engine.system_error_message2, "Undefined audio device number %u", settings.sound_device);
				sprintf(engine.system_error_message3, "Please run setup.exe to identify your audio card");
				Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_SOUND);
				break;
		}

		song.current_pattern = 0;
		song.current_line = 0;
		song.pattern_break = false;
		song.pattern_delay = false;
		song.tickXF = 0;
		song.ticks = 0;
		song.next_line = 0;
		song.irq_mode = true;
		song.loop = true;

		song.ticklooper = 0;
		song.macro_ticklooper = 0;
		song.speed = 10;
		song.macro_speedup = 10;

		song.loaded = false;
		song.play = false;
		song.pause = false;
	}
	engine.audio_initialized = true;
}

void AUDIO_Shutdown(void) {
	if (engine.audio_initialized) {
		switch (settings.sound_device) {
			case AUDIO_DEVICE_NONE:// NONE
				break;
			case AUDIO_DEVICE_SPEAKER:// PC Speaker;
				break;
			case AUDIO_DEVICE_ADLIB:// Adlib
				ADLIB_Shutdown();
				break;
			case AUDIO_DEVICE_SBLASTER:// Sound blaster
				SB_Shutdown();
				break;
			default:
				sprintf(engine.system_error_message1, "AUDIO_Shutdown function error");
				sprintf(engine.system_error_message2, "Undefined audio device number %u", settings.sound_device);
				sprintf(engine.system_error_message3, "Please run setup.exe to identify your audio card");
				Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_SOUND);
				break;
		}
	}
	engine.audio_initialized = false;
}

void AUDIO_PlaySound(byte sound, byte priority) {
	if (!sound_effect.play || (priority > sound_effect.current_sound_priority)) {
		switch (settings.sound_device) {
			case AUDIO_DEVICE_NONE:
				break;
			case AUDIO_DEVICE_SPEAKER:// PC Speaker;
				SPK_PlaySound(&sound_effect, sound);
				break;
			case AUDIO_DEVICE_ADLIB:
				ADLIB_PlaySound(&sound_effect, sound);
				break;
			case AUDIO_DEVICE_SBLASTER:
				SB_PlaySound(&sound_effect, sound);
				break;
			default:
				sprintf(engine.system_error_message1, "AUDIO_PlaySound function error");
				sprintf(engine.system_error_message2, "Undefined audio device number %u", settings.sound_device);
				sprintf(engine.system_error_message3, "Please run setup.exe to identify your audio card");
				Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_SOUND);
				break;
		}
	}
}

bool AUDIO_CheckAdlib(void) {
	return ADLIB_CheckFMInstalled();
}

bool AUDIO_CheckSoundBlaster(void) {
	return SB_Check();
}

void AUDIO_TimerHandler(void) {
	switch (settings.sound_device) {
		case AUDIO_DEVICE_NONE:
			break;
		case AUDIO_DEVICE_SPEAKER:// PC Speaker;
			SPK_Handler(&sound_effect, &song);
			break;
		case AUDIO_DEVICE_ADLIB:// Adlib
			ADLIB_Handler(&sound_effect, &song);
			break;
		case AUDIO_DEVICE_SBLASTER:// Sound blaster
			SB_Handler(&sound_effect, &song);
			break;
		default:
			sprintf(engine.system_error_message1, "AUDIO_TimerHandler function error");
			sprintf(engine.system_error_message2, "Undefined audio device number %u", settings.sound_device);
			sprintf(engine.system_error_message3, "Please run setup.exe to identify your audio card");
			Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_SOUND);
			break;
	}
}

void AUDIO_LoadSong(int song_number) {
	switch (settings.music_device) {
		case AUDIO_DEVICE_NONE:
			song.loaded = false;
			break;
		case AUDIO_DEVICE_SPEAKER:// PC Speaker;
			SPK_LoadSong(&song, song_number);
			break;
		case AUDIO_DEVICE_ADLIB:// Adlib
			ADLIB_LoadSong(&song, song_number);
			break;
		case AUDIO_DEVICE_SBLASTER:// Sound blaster
			SB_LoadSong(&song, song_number);
			break;
		default:
			sprintf(engine.system_error_message1, "AUDIO_LoadSong function error");
			sprintf(engine.system_error_message2, "Undefined music device number %u", settings.music_device);
			sprintf(engine.system_error_message3, "Please run setup.exe to identify your audio card");
			Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_SOUND);
			break;
	}
}

void AUDIO_PlaySong(bool loop) {
	song.loop = loop;
	switch (settings.music_device) {
		case AUDIO_DEVICE_NONE:
			song.loaded = false;
			break;
		case AUDIO_DEVICE_SPEAKER:// PC Speaker;
			SPK_PlaySong(&song);
			break;
		case AUDIO_DEVICE_ADLIB:// Adlib
			ADLIB_PlaySong(&song);
			break;
		case AUDIO_DEVICE_SBLASTER:// Sound blaster
			SB_PlaySong(&song);
			break;
		default:
			sprintf(engine.system_error_message1, "AUDIO_PlaySong function error");
			sprintf(engine.system_error_message2, "Undefined music device number %u", settings.music_device);
			sprintf(engine.system_error_message3, "Please run setup.exe to identify your audio card");
			Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_SOUND);
			break;
	}
}

void AUDIO_StopSong(void) {
	switch (settings.music_device) {
		case AUDIO_DEVICE_NONE:
			song.loaded = false;
			break;
		case AUDIO_DEVICE_SPEAKER:// PC Speaker;
			SPK_StopSong(&song);
			break;
		case AUDIO_DEVICE_ADLIB:// Adlib
			ADLIB_StopSong(&song);
			break;
		case AUDIO_DEVICE_SBLASTER:// Sound blaster
			SB_StopSong(&song);
			break;
		default:
			sprintf(engine.system_error_message1, "AUDIO_StopSong function error");
			sprintf(engine.system_error_message2, "Undefined music device number %u", settings.music_device);
			sprintf(engine.system_error_message3, "Please run setup.exe to identify your audio card");
			Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_SOUND);
			break;
	}
}

int AUDIO_GetCurrentLine(void) {
	return song.current_line;
}

void AUDIO_UnloadSong(void) {
	AUDIO_StopSong();
	song.loaded = false;
	song.current_line = 0;
}

bool AUDIO_IsSongEnded(void) {
	return song.stop;
}