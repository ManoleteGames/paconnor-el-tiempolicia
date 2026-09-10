#include "../engine.h"
#include "sblaster.h"

bool sblaster_present;
SBlaster sblaster;

static _go32_dpmi_seginfo old_handler_71, new_handler_71;
static _go32_dpmi_seginfo old_handler_72, new_handler_72;
static _go32_dpmi_seginfo old_handler_73, new_handler_73;
static _go32_dpmi_seginfo old_handler_11, new_handler_11;
static _go32_dpmi_seginfo old_handler_13, new_handler_13;
static _go32_dpmi_seginfo old_handler_15, new_handler_15;


//List of FNumbers
const word FNr[12] = {
		/*C*/ 343,
		/*C#/Db*/ 363,
		/*D*/ 385,
		/*D#/Eb*/ 408,
		/*E*/ 432,
		/*F*/ 458,
		/*F#/Gb*/ 485,
		/*G*/ 514,
		/*G#/Ab*/ 544,
		/*A*/ 577,
		/*A#/Bb*/ 611,
		/*B*/ 647};

/** SB :: Reset DSP
 *  - Check if there is a sound blaster
 */
bool SB_ResetDSP(word port) {
	//Reset the DSP
	outportb(port + SB_RESET, 1);
	delay(10);
	outportb(port + SB_RESET, 0);
	delay(10);
	//Check if reset was succesfull
	if (((inportb(port + SB_READ_DATA_STATUS) & 0x80) == 0x80) && (inportb(port + SB_READ_DATA) == 0xAA)) {
		return true;
	} else
		return false;
}

/****************************************************************************
** Read a byte from the DSP (Digital Signal Processor) on the Sound Blaster**
****************************************************************************/
byte SB_ReadDSP(void) {
	while (!(inportb(sblaster.base + SB_READ_DATA_STATUS) & 0x80));
	return (inportb(sblaster.base + SB_READ_DATA));
}

/****************************************************************************
** Send a byte to the DSP (Digital Signal Processor) on the Sound Blaster  **
****************************************************************************/
void SB_WriteDSP(byte value) {
	//Wait for the DSP to be ready to accept data
	while ((inportb(sblaster.base + SB_WRITE_DATA) & 0x80) == 0x80);
	//Send byte
	outportb(sblaster.base + SB_WRITE_DATA, value);
}

/**************************************************************************
** Writes a value to a specified index register on the FM card           **
**************************************************************************/
void SB_WriteFM(byte reg, byte value) {
	unsigned char Counter;

	//Select register
	outportb(0x388, reg);
	//Wait for card to accept value
	for (Counter = 1; Counter < 25; Counter++) { inportb(0x388); }
	//Send value
	outportb(0x389, value);
	//Wait for card to accept value
	for (Counter = 1; Counter < 100; Counter++) { inportb(0x388); }
}

/****************************************************************************
** Returns the version number of the DSP.                                  **
****************************************************************************/
word SB_GetDSPVersion(void) {
	unsigned int Version;
	SB_WriteDSP(0xE1);
	Version = SB_ReadDSP();
	return (SB_ReadDSP() + (Version << 8));
}


byte SB_TestLoDMA(void) {
	unsigned char Before, After;
	unsigned int Count;

	//Reset DSP
	SB_ResetDSP(sblaster.base);
	SB_WriteDSP(0xD3);//DSP-command D3h - Speaker Off
	SB_WriteDSP(0x40);//DSP-command 40h - Set sample frequency
	SB_WriteDSP(165); //Write time constant for 11025Hz
	SB_WriteDSP(0x14);//DSP-command 14h - 8bit playback
	SB_WriteDSP(0x01);//Set the block length to 1

	Count = 0;

	//Disable interrupts
	disable();

	//Look at the current state of the DMA requests
	Before = inportb(0x8) & 0xE0;

	//Start playback, * note : this is still part of "set block length"
	SB_WriteDSP(0x00);

	do {
		//Check them again until there is a change or it takes too long
		After = inportb(0x8) & 0xE0;
	} while ((Before == After) && (++Count < 60000));

	//Reenable interrupts
	enable();

	//Reset DSP
	SB_ResetDSP(sblaster.base);

	//Is there a change?
	if (Before != After) {
		Count = 0;
		After ^= (Before);
		//If yes, which bit is set?
		while (After > 0) {
			Count++;
			After >>= 1;
		}
		return (Count - 5);
	}
	//No DMA???
	return (0xFF);
}

/****************************************************************************
** This does the same thing for the 16bit DMA channel, if used             **
****************************************************************************/
byte SB_TestHiDMA(void) {
	byte Before, After;
	word Count;

	//Reset DSP
	SB_ResetDSP(sblaster.base);
	SB_WriteDSP(0xB0);//DSP-command B0h - 16bit DMA
	SB_WriteDSP(0x10);//Mono
	SB_WriteDSP(0x03);//Set the block length to 3 = 1 DWORD

	Count = 0;

	//Disable interrupts
	disable();

	//Look at the current state of the DMA requests
	Before = inportb(0xD0) & 0xF0;

	//Start playback, * note : this is still part of "set block length"
	SB_WriteDSP(0x00);

	do {
		//Check them again until there is a change or it takes too long
		After = inportb(0xD0) & 0xF0;
	} while ((Before == After) && (++Count < 60000));

	//Reenable interrupts
	enable();

	//Reset DSP
	SB_ResetDSP(sblaster.base);

	//Is there a change?
	if (Before != After) {
		Count = 0;
		After ^= (Before);
		//If yes, which bit is set?
		while (After > 0) {
			Count++;
			After >>= 1;
		}
		return (Count - 1);
	}
	//No DMA???
	return (0xFF);
}


word SB_GetBasePort(void) {
	//Check for Sound Blaster, address: ports 220, 230, 240, 250, 260 or 280
	int port;
	for (port = 1; port < 9; port++) {
		if (port != 7)
			if (SB_ResetDSP(0x200 + (port << 4))) {
				return 0x200 + (port << 4);
			}
	}
}

static void SB_ServiceIRQ2_Handler(void) {
	//Relieve DSP
	inportb(sblaster.base + SB_READ_DATA_STATUS);
	//Acknowledge hardware interrupt
	outportb(0x20, 0x20);
	//Since IRQ 2 has been redirected, we need to acknowledge two interrupt
	//controllers
	outportb(0xA0, 0x20);
	//Save interrupt channel number
	sblaster.irq = 2;
}
static void SB_ServiceIRQ3_Handler(void) {
	inportb(sblaster.base + SB_READ_DATA_STATUS);
	outportb(0x20, 0x20);
	sblaster.irq = 3;
}
static void SB_ServiceIRQ5_Handler(void) {
	inportb(sblaster.base + SB_READ_DATA_STATUS);
	outportb(0x20, 0x20);
	sblaster.irq = 5;
}
static void SB_ServiceIRQ7_Handler(void) {
	inportb(sblaster.base + SB_READ_DATA_STATUS);
	outportb(0x20, 0x20);
	sblaster.irq = 7;
}
static void SB_ServiceIRQ10_Handler(void) {
	inportb(sblaster.base + SB_READ_DATA_STATUS);
	outportb(0x20, 0x20);
	outportb(0xA0, 0x20);
	sblaster.irq = 10;
}
static void SB_ServiceIRQ11_Handler(void) {
	inportb(sblaster.base + SB_READ_DATA_STATUS);
	outportb(0x20, 0x20);
	outportb(0xA0, 0x20);
	sblaster.irq = 11;
}
static void SB_TestIRQ(void) {
	unsigned int Count;
	sblaster.irq = 0;
	SB_WriteDSP(0xD1);                         //DSP-command D1h - Enable speaker, required
											   //on older SB's
	SB_WriteDSP(0x40);                         //DSP-command 40h - Set sample frequency
	SB_WriteDSP(165);                          //Write time constant for 11025Hz
	outportb(0x0A, 4 + sblaster.lo_DMA);       //Mask DMA channel
	outportb(0x0C, 0);                         //Clear byte pointer
	outportb(0x0B, 0x49);                      //Set mode
	outportb(1 + (sblaster.lo_DMA << 1), 0x03);//Set the block length to 3 == 1 DWORD
	outportb(1 + (sblaster.lo_DMA << 1), 0x00);

	outportb(0x0A, sblaster.lo_DMA);//Unmask DMA channel

	SB_WriteDSP(0x14);//DSP-command 14h - Single cycle playback
	SB_WriteDSP(0x03);
	SB_WriteDSP(0x00);

	Count = 0;
	do
		//Wait until either one of the IRQ service routines has been called
		//or it takes too long
		Count++;
	while ((!sblaster.irq) && (Count != 0xFFFF));
}

void SB_SetInstrument(byte voice, Instrument instr) {
	//Addresses of the operators used to form voice data
	const byte OpAdr[9] = {0, 1, 2, 8, 9, 10, 16, 17, 18};
	byte value;

	//Set up voice modulator
	value = instr.modulator.tremolo << 8 | instr.modulator.vibrato << 7 | instr.modulator.eg_type << 6 | instr.modulator.ksr << 5 | instr.modulator.multi;
	SB_WriteFM(0x20 + OpAdr[voice], value);

	value = instr.modulator.ksl << 7 | instr.modulator.total_level;
	SB_WriteFM(0x40 + OpAdr[voice], value);

	value = instr.modulator.attack << 4 | instr.modulator.decay;
	SB_WriteFM(0x60 + OpAdr[voice], value);

	value = instr.modulator.sustain << 4 | instr.modulator.release;
	SB_WriteFM(0x80 + OpAdr[voice], value);

	SB_WriteFM(0xE0 + OpAdr[voice], instr.modulator.wave_shape);

	//Set up voice carrier
	value = instr.carrier.tremolo << 8 | instr.carrier.vibrato << 7 | instr.carrier.eg_type << 6 | instr.carrier.ksr << 5 | instr.carrier.multi;
	SB_WriteFM(0x23 + OpAdr[voice], value);

	value = instr.carrier.ksl << 7 | instr.carrier.total_level;
	SB_WriteFM(0x43 + OpAdr[voice], value);

	value = instr.carrier.attack << 4 | instr.carrier.decay;
	SB_WriteFM(0x63 + OpAdr[voice], value);

	value = instr.carrier.sustain << 4 | instr.carrier.release;
	SB_WriteFM(0x83 + OpAdr[voice], value);

	SB_WriteFM(0xE3 + OpAdr[voice], instr.carrier.wave_shape);

	value = instr.feedback << 1 | instr.syn_type;
	SB_WriteFM(0xC0 + OpAdr[voice], value);
}

void SB_SetDrum1(byte voice, byte velocity) {
	//Addresses of the operators used to form voice data
	const byte OpAdr1[9] = {0x00, 0x1, 0x2, 0x8, 0x9, 0xA, 0x10, 0x11, 0x12};
	const byte OpAdr2[9] = {0x03, 0x4, 0x5, 0xB, 0xC, 0xD, 0x13, 0x14, 0x15};

	SB_WriteFM(0x20 + OpAdr1[voice], 0);   // AmpMod(7)|Vib(6)|EGtype(5)|KSR(4)|Multi(3..0)
	SB_WriteFM(0x40 + OpAdr1[voice], 0xC0);// KSL(7..6)|TL(5..0)
	SB_WriteFM(0x60 + OpAdr1[voice], 0xF8);// Attack(7..4)|Decay(3..0)
	SB_WriteFM(0x80 + OpAdr1[voice], 0x26);// Sustain(7..4)|Release(3..0)
	SB_WriteFM(0xE0 + OpAdr1[voice], 0x0); // Waveform (1..0)

	// Carrier
	SB_WriteFM(0x20 + OpAdr2[voice], 0);
	SB_WriteFM(0x40 + OpAdr2[voice], velocity);
	SB_WriteFM(0x60 + OpAdr2[voice], 0xF2);// Attack(7..4)|Decay(3..0)
	SB_WriteFM(0x80 + OpAdr2[voice], 0x66);
	SB_WriteFM(0xE0 + OpAdr2[voice], 0x0);

	SB_WriteFM(0xC0 + OpAdr1[voice], 0x06);// Feedback(3..1)|Algorithm(0)
}

void SB_SetDrum2(byte voice, byte velocity) {
	//Addresses of the operators used to form voice data
	const byte OpAdr1[9] = {0x00, 0x1, 0x2, 0x8, 0x9, 0xA, 0x10, 0x11, 0x12};
	const byte OpAdr2[9] = {0x03, 0x4, 0x5, 0xB, 0xC, 0xD, 0x13, 0x14, 0x15};

	SB_WriteFM(0x20 + OpAdr1[voice], 0x01);// AmpMod(7)|Vib(6)|EGtype(5)|KSR(4)|Multi(3..0)
	SB_WriteFM(0x40 + OpAdr1[voice], 0x80);// KSL(7..6)|TL(5..0)
	SB_WriteFM(0x60 + OpAdr1[voice], 0xF3);// Attack(7..4)|Decay(3..0)
	SB_WriteFM(0x80 + OpAdr1[voice], 0x0A);// Sustain(7..4)|Release(3..0)
	SB_WriteFM(0xE0 + OpAdr1[voice], 0x0); // Waveform (1..0)

	// Carrier
	SB_WriteFM(0x20 + OpAdr2[voice], 0x01);
	SB_WriteFM(0x40 + OpAdr2[voice], 0x00);//velocity);
	SB_WriteFM(0x60 + OpAdr2[voice], 0xF4);// Attack(7..4)|Decay(3..0)
	SB_WriteFM(0x80 + OpAdr2[voice], 0x06);
	SB_WriteFM(0xE0 + OpAdr2[voice], 0x0);

	SB_WriteFM(0xC0 + OpAdr1[voice], 0x03 << 1);// Feedback(3..1)|Algorithm(0)
}

void SB_SetDrum3(byte voice, byte velocity) {
	//Addresses of the operators used to form voice data
	const byte OpAdr1[9] = {0x00, 0x1, 0x2, 0x8, 0x9, 0xA, 0x10, 0x11, 0x12};
	const byte OpAdr2[9] = {0x03, 0x4, 0x5, 0xB, 0xC, 0xD, 0x13, 0x14, 0x15};

	SB_WriteFM(0x20 + OpAdr1[voice], 0);   // AmpMod(7)|Vib(6)|EGtype(5)|KSR(4)|Multi(3..0)
	SB_WriteFM(0x40 + OpAdr1[voice], 0xC0);// KSL(7..6)|TL(5..0)
	SB_WriteFM(0x60 + OpAdr1[voice], 0xF8);// Attack(7..4)|Decay(3..0)
	SB_WriteFM(0x80 + OpAdr1[voice], 0x2A);// Sustain(7..4)|Release(3..0)
	SB_WriteFM(0xE0 + OpAdr1[voice], 0x0); // Waveform (1..0)
	SB_WriteFM(0xC0 + OpAdr1[voice], 0x06);// Feedback(3..1)|Algorithm(0)

	// Carrier
	SB_WriteFM(0x20 + OpAdr2[voice], 0);
	SB_WriteFM(0x40 + OpAdr2[voice], velocity);
	SB_WriteFM(0x60 + OpAdr2[voice], 0xF2);// Attack(7..4)|Decay(3..0)
	SB_WriteFM(0x80 + OpAdr2[voice], 0x6A);
	SB_WriteFM(0xE0 + OpAdr2[voice], 0x0);
}

/**************************************************************************
** Activates a voice on the FM card                                      **
***************************************************************************
** Voice selects one of the 9 FM voices                                  **
** FNumber selects the note to be played                                 **
** Block selects the octave for the specified note                       **
**************************************************************************/
void SB_NoteOn(byte voice, byte note, byte block) {
	SB_WriteFM(0xA0 + voice, FNr[note] & 0xFF);
	SB_WriteFM(0xB0 + voice, (FNr[note] >> 8) + (block << 2) + 32);
}

/**************************************************************************
** Deactivates a voice on the FM card                                    **
***************************************************************************
** Make sure to give the same values for Note and Block or this will     **
** sound very odd.                                                       **
**************************************************************************/
void SB_NoteOff(byte voice, byte note, byte block) {
	SB_WriteFM(0xA0 + voice, FNr[note] & 0xFF);
	SB_WriteFM(0xB0 + voice, (FNr[note] >> 8) + (block << 2));
}


void SB_Shutdown(void) {
	if (_go32_dpmi_set_protected_mode_interrupt_vector(ISR_VECT_71, &old_handler_71) == -1)
		Error("Failed to free the interrupt 0x71", "", "", ERROR_SYSTEM);
	else
		_go32_dpmi_free_iret_wrapper(&new_handler_71);

	if (_go32_dpmi_set_protected_mode_interrupt_vector(ISR_VECT_72, &old_handler_72) == -1)
		Error("Failed to free the interrupt 0x72", "", "", ERROR_SYSTEM);
	else
		_go32_dpmi_free_iret_wrapper(&new_handler_72);

	if (_go32_dpmi_set_protected_mode_interrupt_vector(ISR_VECT_73, &old_handler_73) == -1)
		Error("Failed to free the interrupt 0x73", "", "", ERROR_SYSTEM);
	else
		_go32_dpmi_free_iret_wrapper(&new_handler_73);

	if (_go32_dpmi_set_protected_mode_interrupt_vector(ISR_VECT_11, &old_handler_11) == -1)
		Error("Failed to free the interrupt 11", "", "", ERROR_SYSTEM);
	else
		_go32_dpmi_free_iret_wrapper(&new_handler_11);

	if (_go32_dpmi_set_protected_mode_interrupt_vector(ISR_VECT_13, &old_handler_13) == -1)
		Error("Failed to free the interrupt 13", "", "", ERROR_SYSTEM);
	else
		_go32_dpmi_free_iret_wrapper(&new_handler_13);

	if (_go32_dpmi_set_protected_mode_interrupt_vector(ISR_VECT_15, &old_handler_15) == -1)
		Error("Failed to free the interrupt 15", "", "", ERROR_SYSTEM);
	else
		_go32_dpmi_free_iret_wrapper(&new_handler_15);
}

//This resets the drums
void SB_EnableDrums() {
	byte ByteBD;
	//Enable waveform select
	SB_WriteFM(1, 0x20);
	//Enable percussion mode, amplify AM & VIB
	ByteBD = (char) (1 << 7) | (0 << 6) | 0x20;
	SB_WriteFM(0xBD, ByteBD);
	//Set drum frequencies
	SB_WriteFM(0xA6, 400 & 0xFF);
	SB_WriteFM(0xB6, (400 >> 8) + (2 << 2));
	SB_WriteFM(0xA7, 500 & 0xFF);
	SB_WriteFM(0xB7, (500 >> 8) + (2 << 2));
	SB_WriteFM(0xA8, 650 & 0xFF);
	SB_WriteFM(0xB8, (650 >> 8) + (2 << 2));
}

void SB_Init(void) {
	byte a, b;

	sblaster.base = SB_GetBasePort();
	sblaster.dsp_version = SB_GetDSPVersion();
	sblaster.lo_DMA = SB_TestLoDMA();
	sblaster.hi_DMA = SB_TestHiDMA();
	if (sblaster.hi_DMA == 0xFF) sblaster.hi_DMA = sblaster.lo_DMA;

	_go32_dpmi_get_protected_mode_interrupt_vector(ISR_VECT_71, &old_handler_71);
	new_handler_71.pm_offset = (dword) SB_ServiceIRQ2_Handler;
	new_handler_71.pm_selector = _go32_my_cs();
	_go32_dpmi_allocate_iret_wrapper(&new_handler_71);
	_go32_dpmi_set_protected_mode_interrupt_vector(ISR_VECT_71, &new_handler_71);

	_go32_dpmi_get_protected_mode_interrupt_vector(ISR_VECT_11, &old_handler_11);
	new_handler_11.pm_offset = (dword) SB_ServiceIRQ3_Handler;
	new_handler_11.pm_selector = _go32_my_cs();
	_go32_dpmi_allocate_iret_wrapper(&new_handler_11);
	_go32_dpmi_set_protected_mode_interrupt_vector(ISR_VECT_11, &new_handler_11);

	_go32_dpmi_get_protected_mode_interrupt_vector(ISR_VECT_13, &old_handler_13);
	new_handler_13.pm_offset = (dword) SB_ServiceIRQ5_Handler;
	new_handler_13.pm_selector = _go32_my_cs();
	_go32_dpmi_allocate_iret_wrapper(&new_handler_13);
	_go32_dpmi_set_protected_mode_interrupt_vector(ISR_VECT_13, &new_handler_13);

	_go32_dpmi_get_protected_mode_interrupt_vector(ISR_VECT_15, &old_handler_15);
	new_handler_15.pm_offset = (dword) SB_ServiceIRQ7_Handler;
	new_handler_15.pm_selector = _go32_my_cs();
	_go32_dpmi_allocate_iret_wrapper(&new_handler_15);
	_go32_dpmi_set_protected_mode_interrupt_vector(ISR_VECT_15, &new_handler_15);


	_go32_dpmi_get_protected_mode_interrupt_vector(ISR_VECT_72, &old_handler_72);
	new_handler_72.pm_offset = (dword) SB_ServiceIRQ10_Handler;
	new_handler_72.pm_selector = _go32_my_cs();
	_go32_dpmi_allocate_iret_wrapper(&new_handler_72);
	_go32_dpmi_set_protected_mode_interrupt_vector(ISR_VECT_72, &new_handler_72);

	_go32_dpmi_get_protected_mode_interrupt_vector(ISR_VECT_73, &old_handler_73);
	new_handler_73.pm_offset = (dword) SB_ServiceIRQ11_Handler;
	new_handler_73.pm_selector = _go32_my_cs();
	_go32_dpmi_allocate_iret_wrapper(&new_handler_73);
	_go32_dpmi_set_protected_mode_interrupt_vector(ISR_VECT_73, &new_handler_73);

	//Enable IRQs
	outportb(0x21, inportb(0x21) & (!(1 << 2)));
	outportb(0x21, inportb(0x21) & (!(1 << 3)));
	outportb(0x21, inportb(0x21) & (!(1 << 5)));
	outportb(0x21, inportb(0x21) & (!(1 << 7)));
	outportb(0xA1, inportb(0xA1) & (!(1 << 1)));
	outportb(0xA1, inportb(0xA1) & (!(1 << 2)));
	outportb(0xA1, inportb(0xA1) & (!(1 << 3)));

	SB_TestIRQ();

	SB_WriteFM(1, 0x0);
	SB_WriteFM(4, 0x60);
	SB_WriteFM(4, 0x80);
	a = inportb(SB_FM_PORT);
	SB_WriteFM(2, 0xFF);
	SB_WriteFM(4, 0x21);
	b = inportb(SB_FM_PORT);
	SB_WriteFM(4, 0x60);
	SB_WriteFM(4, 0x80);
	if ((a & 0xE0) == 0 && (b & 0xE0) == 0xC0) {
		sblaster.fm_port = SB_FM_PORT;
	} else {
		Error("no sb fm port", "", "", 1);
	}
}

bool SB_Check(void) {
	bool detected;
	byte a, b;

	int port;
	for (port = 1; port < 9; port++) {
		if (port != 7)
			if (SB_ResetDSP(0x200 + (port << 4))) {
				return true;
			}
	}
	if (port == 9) {
		return false;
	}
}

void SB_Handler(SoundEffect *sound, Song *song) {
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

void SB_PlaySound(SoundEffect *sound, int number) {
}

void SB_LoadSong(Song *song, int number) {
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


void SB_PlaySong(Song *song) {
	if (song->loaded) song->play = true;
}

void SB_StopSong(Song *song) {
	song->play = false;
	A2M_Stop();
}