#include "../engine.h"
#include "adlib.h"

Adlib *adlib;

void ADLIB_Init(void) {
	adlib = MM_PushChunk(sizeof(Adlib), CT_ENGINE);
}

void ADLIB_Shutdown(void) {
}

void ADLIB_Handler(SoundEffect *sound, Song *song) {
	//A2M_TimerHandler();

	/////// Song management //////////
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

/**************************************************************************
** Writes a value to a specified index register on the FM card           **
**************************************************************************/
void ADLIB_WriteFM(byte Register, byte Value) {
	byte Counter;

	//Select register
	outportb(ADLIB_FM_PORT, Register);
	//Wait for card to accept value
	for (Counter = 1; Counter < 25; Counter++) { inportb(ADLIB_FM_PORT); }
	//Send value
	outportb(ADLIB_FM_DATA, Value);
	//Wait for card to accept value
	for (Counter = 1; Counter < 100; Counter++) { inportb(ADLIB_FM_PORT); }
}

/**************************************************************************
** Checks for the presence of an FM card                                 **
**************************************************************************/
bool ADLIB_CheckFMInstalled(void) {
	byte value_a, value_b;

	ADLIB_WriteFM(1, 0);
	ADLIB_WriteFM(4, 0x60);
	ADLIB_WriteFM(4, 0x80);
	value_a = inportb(ADLIB_FM_PORT);
	ADLIB_WriteFM(2, 0xFF);
	ADLIB_WriteFM(4, 0x21);
	value_b = inportb(ADLIB_FM_PORT);
	ADLIB_WriteFM(4, 0x60);
	ADLIB_WriteFM(4, 0x80);
	if ((value_a & 0xE0) == 0 && (value_b & 0xE0) == 0xC0) {
		adlib->opl = 2;
		if ((inportb(ADLIB_FM_PORT) & 0x06) == 0) { adlib->opl = 3; }
		return true;
	} else {
		return false;
	}
}

void ADLIB_PlaySound(SoundEffect *sound, int number) {
}

void ADLIB_LoadSong(Song *song, int number) {
	switch (number) {
		case 1:
			A2M_LoadFile("MINTRO.DAT", "intro1.a2m");
			song->loaded = true;
			song->current_line = 0;
			song->current_order = 0;
			song->speed = songinfo->speed;
			song->tempo = songinfo->tempo;
			TIMER_UpdateTimerTime(TIMER_AUDIO_NUMBER, song->tempo);
			break;
		case 2:
			A2M_LoadFile("MINTRO.DAT", "intro1.a2m");
			song->loaded = true;
			song->current_line = 0;
			song->current_order = 0;
			song->speed = songinfo->speed;
			song->tempo = songinfo->tempo;
			TIMER_UpdateTimerTime(TIMER_AUDIO_NUMBER, song->tempo);
			break;
		case 3:
			A2M_LoadFile("MSCENE1.DAT", "scene1.a2m");
			song->loaded = true;
			song->current_line = 0;
			song->current_order = 0;
			song->speed = songinfo->speed;
			song->tempo = songinfo->tempo;
			TIMER_UpdateTimerTime(TIMER_AUDIO_NUMBER, song->tempo);
			break;
		case 4:
			break;
		default:
			break;
	}
}

void ADLIB_PlaySong(Song *song) {
	if (song->loaded) song->play = true;
}

void ADLIB_StopSong(Song *song) {
	song->play = false;
	A2M_Stop();
}