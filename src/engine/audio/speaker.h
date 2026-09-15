#ifndef _SPEAKER_H
#define _SPEAKER_H

#include "../types/types.h"

#define SPK_PIT_PORT 0x43
#define SPK_PIT_CH 0x42

#define SPK_MAX_MUSIC_LENGTH 8000

void SPK_Init(void);
void SPK_Unmute(void);
void SPK_Mute(void);
void SPK_Sound(int note);
void SPK_NoSound(void);
void SPK_Handler(SoundEffect *sound, Song *song);
void SPK_PlaySound(SoundEffect *sound, int number);
void SPK_LoadSong(Song *song, int number);
void SPK_PlaySong(Song *song);
void SPK_StopSong(Song *song);
void SPK_PauseSong(Song *song);
void SPK_ResumeSong(Song *song);

#endif// _SPEAKER_H