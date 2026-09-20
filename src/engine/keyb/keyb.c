#include "keyb.h"
#include "../engine.h"

byte kbLastScancode;
bool kbKeySpace_FP, kbKeySpace_old;
bool kbKeyState[128];
static const unsigned char kbKeyASCII[128] = {
		0,
		27,
		'!',
		'@',
		'#',
		'$',
		'%',
		'^',
		'&',
		'*',
		'(',
		')',
		'_',
		'+',
		'\b',
		'\t',
		'Q',
		'W',
		'E',
		'R',
		'T',
		'Y',
		'U',
		'I',
		'O',
		'P',
		'{',
		'}',
		'\n',
		0,
		'A',
		'S',
		'D',
		'F',
		'G',
		'H',
		'J',
		'K',
		'L',
		':',
		'"',
		'~',
		0,
		'|',
		'Z',
		'X',
		'C',
		'V',
		'B',
		'N',
		'M',
		'<',
		'>',
		'?',
};

byte kbBindingUp;
byte kbBindingDown;
byte kbBindingLeft;
byte kbBindingRight;
byte kbBindingLoop;
byte kbBindingFire;
byte kbBindingThrow;
byte kbBindingChangeGun;
byte kbBindingCombat;

static _go32_dpmi_seginfo old_keyb_handler, new_keyb_handler;

bool KEYB_IsAnyKeyPressed(void) {
	int i;
	for (i = 0; i < 0x54; i++) {
		if (kbKeyState[i]) {
			return true;
		}
	}
	return false;
}

byte KEYB_GetLastKeyPressed_ASCII(void) {

	switch (kbLastScancode & 0x7f) {
		case SCANCODE_Q:
		case SCANCODE_W:
		case SCANCODE_E:
		case SCANCODE_R:
		case SCANCODE_T:
		case SCANCODE_Y:
		case SCANCODE_U:
		case SCANCODE_I:
		case SCANCODE_O:
		case SCANCODE_P:
		case SCANCODE_A:
		case SCANCODE_S:
		case SCANCODE_D:
		case SCANCODE_F:
		case SCANCODE_G:
		case SCANCODE_H:
		case SCANCODE_J:
		case SCANCODE_K:
		case SCANCODE_L:
		case SCANCODE_Z:
		case SCANCODE_X:
		case SCANCODE_C:
		case SCANCODE_V:
		case SCANCODE_B:
		case SCANCODE_N:
		case SCANCODE_M:
			return kbKeyASCII[kbLastScancode];
			break;
		default:
			return 0;
			break;
	}
}

byte KEYB_GetLastKeyPressed_CODE(void) {
	return kbLastScancode;
}

/** KEYB :: Keyboard interrupt handler
 */
static void KEYB_Handler(void) {
	byte keyb_ctrl;

	kbLastScancode = inportb(0x60);
	// Ignore extended multi-byte scancodes (introduced with the IBM PS/2 line).
	if (kbLastScancode != SCANCODE_EXTENDED) {
		// Update our key state array
		if ((kbLastScancode & 0x80) != 0) {
			kbKeyState[kbLastScancode & 0x7f] = false;
		} else {
			kbKeyState[kbLastScancode & 0x7f] = true;
		}
	}

	/* Tell the keyboard that the key is processed */
	keyb_ctrl = inportb(0x61);
	outportb(0x61, keyb_ctrl | 0x80); /* set the "enable kbd" bit */
	outportb(0x61, keyb_ctrl);        /* write back the original control value */

	// Send end-of-interrupt (EOI) signal to PCI (programmable interrupt controller) to resume its normal operation
	//outportb(0x20, inportb(0x20)); /* End-Of-Interrupt command */
	outportb(PIC, NONSPECIFIC_EOI);
}
END_OF_FUNCTION(KEYB_Handler);

/** KEYB :: Keyboard Initialization
 *  - Initializes keyboard interrupt
 */
void KEYB_Init(void) {
	if (!engine.keyboard_initialized) {

		LOCK_VARIABLE(old_keyb_handler);
		LOCK_VARIABLE(new_keyb_handler);
		LOCK_VARIABLE(kbLastScancode);
		LOCK_VARIABLE(kbKeyState);
		LOCK_FUNCTION(KEYB_Handler);

		_go32_dpmi_get_protected_mode_interrupt_vector(KEYB_IRS, &old_keyb_handler);
		new_keyb_handler.pm_offset = (int) KEYB_Handler;
		new_keyb_handler.pm_selector = _go32_my_cs();
		_go32_dpmi_allocate_iret_wrapper(&new_keyb_handler);
		_go32_dpmi_set_protected_mode_interrupt_vector(KEYB_IRS, &new_keyb_handler);
	}

	// Set keys
	kbBindingUp = settings.up_key;
	kbBindingDown = settings.down_key;
	kbBindingLeft = settings.left_key;
	kbBindingRight = settings.right_key;
	kbBindingLoop = settings.loop_key;
	kbBindingFire = settings.fire_key;
	kbBindingThrow = settings.throw_key;
	kbBindingChangeGun = settings.change_gun_key;
	kbBindingCombat = 46;

	engine.keyboard_initialized = true;
}

void KEYB_Shutdown(void) {
	if (engine.keyboard_initialized) {
		if (_go32_dpmi_set_protected_mode_interrupt_vector(KEYB_IRS, &old_keyb_handler) == -1)
			Error("Failed to free the keyboard", "", "", ERROR_SYSTEM);
		else
			_go32_dpmi_free_iret_wrapper(&new_keyb_handler);
	}
	engine.keyboard_initialized = false;
}

void KEYB_Update(void) {
	kbKeySpace_FP = false;
	if (kbKeyState[SCANCODE_SPACE] && !kbKeySpace_old) {
		kbKeySpace_FP = true;
	}
	kbKeySpace_old = kbKeyState[SCANCODE_SPACE];
}