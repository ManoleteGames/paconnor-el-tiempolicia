#ifndef _AUDIO_H
#define _AUDIO_H

#include "../types/types.h"

#define IRS_SPEAKER 0x1C

#define AUDIO_DEVICE_NONE 0
#define AUDIO_DEVICE_SPEAKER 1
#define AUDIO_DEVICE_ADLIB 2
#define AUDIO_DEVICE_SBLASTER 3

#define AUDIO_GUN_EFFECT 1
#define AUDIO_PUNCH_EFFECT 2
#define AUDIO_KICK_EFFECT 3
#define AUDIO_SHOT_FAIL_EFFECT 4
#define AUDIO_RELOAD_PISTOL_EFFECT 5
#define AUDIO_RELOAD_SHOOTGUN_EFFECT 6
#define AUDIO_RELOAD_UZI_EFFECT 7
#define AUDIO_RELOAD_SNIPPER_EFFECT 8
#define AUDIO_SHOTGUN_EFFECT 9
#define AUDIO_UZI_EFFECT 10
#define AUDIO_SNIPPER_EFFECT 11
#define AUDIO_EXPLOSSION 15
#define AUDIO_ACTOR_DEAD 20
#define AUDIO_TICK_EFFECT 30
#define AUDIO_TACK_EFFECT 31
#define AUDIO_JUMP_EFFECT 32
#define AUDIO_CRASH_EFFECT 33
#define AUDIO_MENU_EFFECT 34
#define AUDIO_SELECT_EFFECT 35
#define AUDIO_GET_ITEM_EFFECT 36
#define AUDIO_OPEN_DOOR_EFFECT 37

#define AUDIO_MAX_SONG_SIZE 20000

#define AUDIO_SONG_1 1// Menu song
#define AUDIO_SONG_2 2// scene 1 intro song
#define AUDIO_SONG_3 3// scene 1 outro song
#define AUDIO_SONG_4 4// scene 1 travel
#define AUDIO_SONG_5 5
#define AUDIO_SONG_6 6
#define AUDIO_SONG_7 7
#define AUDIO_SONG_8 8

void AUDIO_Init(void);
void AUDIO_Shutdown(void);
void AUDIO_PlaySound(byte sound, byte priority);
bool AUDIO_CheckSoundBlaster(void);
bool AUDIO_CheckAdlib(void);
void AUDIO_PlayIntro(void);
void AUDIO_TimerHandler(void);
void AUDIO_LoadSong(int song_number);
void AUDIO_PlaySong(bool loop);
void AUDIO_StopSong(void);
void AUDIO_PauseSong(void);
void AUDIO_ResumeSong(void);
void AUDIO_UnloadSong(void);
bool AUDIO_IsSongEnded(void);
int AUDIO_GetCurrentLine(void);

#endif// _TIMER_H