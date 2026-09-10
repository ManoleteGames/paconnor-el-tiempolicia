#include "../engine.h"
#include "timer.h"

volatile static dword milliseconds = 0;

volatile static Timeout timeouts[MAX_TIMERS] = {0};// Array to track multiple timeouts

static _go32_dpmi_seginfo old_timer_handler, new_timer_handler;

void TIMER_ClearAllTimeouts() {
	// Reset timers
	for (int i = 0; i < MAX_TIMERS; i++) {
		timeouts[i].enabled = false;
		timeouts[i].current_time = 0;
		timeouts[i].callback = NULL;
		timeouts[i].end_time = 9999;
		timeouts[i].param = 0;
	}
}

// Register a timeout callback
byte TIMER_SetTimeout(int timer, void (*callback)(byte), byte param, dword ms) {
	if (!callback) return -1;// Ignore if NULL function

	disable();

	// Find an empty slot
	if (timeouts[timer].callback == NULL) {
		timeouts[timer].callback = callback;
		timeouts[timer].param = param;
		// Prevent overflow
		if (UINT32_MAX - milliseconds < ms) {
			timeouts[timer].end_time = ms - (UINT32_MAX - milliseconds);
		} else {
			timeouts[timer].end_time = milliseconds + ms;
		}
		timeouts[timer].enabled = true;
		enable();
		return 0;// Success
	}

	enable();
	return 1;// No available slot
}

void TIMER_UpdateTimerTime(int timer, dword ms) {

	timeouts[timer].enabled = false;
	timeouts[timer].current_time = 0;
	timeouts[timer].end_time = ms;
	timeouts[timer].enabled = true;
}

void TIMER_AudioCallback(byte param) {
	if (engine.audio_initialized) AUDIO_TimerHandler();
}
END_OF_FUNCTION(TIMER_AudioCallback);

void TIMER_VideoCallback(byte param) {
	if (engine.video_initialized) VIDEO_TimerHandler();
}
END_OF_FUNCTION(TIMER_VideoCallback);

void TIMER_MiscCallback(byte param) {
	if (kbKeyState[SCANCODE_SPACE]) {
		if (engine.sequence) {
			SetDelayTime(0);
			engine.sequence = false;
		}
	}
}
END_OF_FUNCTION(TIMER_MiscCallback);

static void TIMER_Handler(void) {
	disable();
	//milliseconds += 55;// ~55 ms per tick at 18.2 Hz// ~1 ms per tick at 1000 Hz
	milliseconds += engine.interrupt_time_ms;

	// Check each registered timeout
	for (int i = 0; i < MAX_TIMERS; i++) {
		if (timeouts[i].enabled) {
			timeouts[i].current_time += engine.interrupt_time_ms;
			if (timeouts[i].callback && (timeouts[i].current_time >= timeouts[i].end_time)) {
				timeouts[i].current_time = 0;
				timeouts[i].callback(timeouts[i].param);
			}
		}
	}

	// evaluate delay time
	if (engine.delay_time_ms > 0) engine.delay_time_ms = engine.delay_time_ms - engine.interrupt_time_ms;

	outportb(PIC, NONSPECIFIC_EOI);// End-of-interrupt (EOI) signal to PIC
	enable();
}
END_OF_FUNCTION(TIMER_Handler);

/** TIMER :: TIMER_Init
 *  - Sets PIT timer interrupt at a custom frecuency
 *  - Sets second timer callback for audio issues
 *  - Sets third timer callback for video effects
 */
void TIMER_Init(void) {
	dword new_freq;

	if (!engine.timer_initialized) {

		disable();

		LOCK_VARIABLE(timeouts);
		LOCK_FUNCTION(TIMER_Handler);
		LOCK_FUNCTION(TIMER_AudioCallback);
		LOCK_FUNCTION(TIMER_VideoCallback);
		LOCK_FUNCTION(TIMER_MiscCallback);

		_go32_dpmi_get_protected_mode_interrupt_vector(TIMER_IRS, &old_timer_handler);
		new_timer_handler.pm_offset = (uint32_t) TIMER_Handler;
		new_timer_handler.pm_selector = _go32_my_cs();
		_go32_dpmi_chain_protected_mode_interrupt_vector(TIMER_IRS, &new_timer_handler);

		// Reconfigure timer
		// - by default 18.2 Hz wich is 55 ms. aprox.
		// - 10 ms -> 100 Hz
		// - 5 ms. -> 250 Hz
		// - 2 ms. -> 500 Hz
		// - 1 ms. -> 1000 Hz
		engine.interrupt_time_ms = 5;
		new_freq = 1193182 / (dword) 250;

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
		outportb(0x43, 0x36);//0011 0110; Channel 0; access lobyte/high byte; mode 3 square wave; 16-bit binary
		// Set divisor low byte.
		outp(0x40, (byte) (new_freq & 0xff));
		// Set divisor high byte.
		outp(0x40, (byte) ((new_freq >> 8) & 0xff));

		enable();
	}

	// Reset timers
	TIMER_ClearAllTimeouts();

	// Initialize audio callback
	TIMER_SetTimeout(TIMER_AUDIO_NUMBER, TIMER_AudioCallback, TIMER_AUDIO_CALLBACK, TIMER_AUDIO_TIME);

	// Initialize video callback
	TIMER_SetTimeout(TIMER_VIDEO_NUMBER, TIMER_VideoCallback, TIMER_VIDEO_CALLBACK, TIMER_VIDEO_TIME);

	// Initialize misc callback
	TIMER_SetTimeout(TIMER_MISC_NUMBER, TIMER_MiscCallback, TIMER_MISC_CALLBACK, TIMER_MISC_TIME);

	engine.timer_initialized = true;
}


void TIMER_Shutdown(void) {
	disable();
	if (engine.timer_initialized) {
		TIMER_ClearAllTimeouts();
		if (_go32_dpmi_set_protected_mode_interrupt_vector(TIMER_IRS, &old_timer_handler) == -1)
			Error("Failed to free the timer", "", "", ERROR_SYSTEM);
	}
	engine.timer_initialized = false;
	enable();
}

void TIMER_ResetTime(void) {
	milliseconds = 0;
}

dword TIMER_GetMilliseconds(void) {
	return milliseconds;
}
