#ifndef _A2M_H
#define _A2M_H

#include "../types/types.h"

/* Helpers for importing A2M format V9-14 */
#define tINSTR_DATA_SIZE (14)
#define tREGISTER_TABLE_DEF_SIZE (15)
#define tFMREG_TABLE_SIZE (3831)
#define tARPVIB_TABLE_SIZE (521)
#define tINS_4OP_FLAGS_SIZE (129U)
#define tRESERVED_SIZE (1024)
#define tBPM_DATA_SIZE (3U)

// Macros for extracting little-endian integers from raw buffer
#define INT8LE(A) (int8_t) ((A)[0])
#define UINT8LE(A) (uint8_t) ((A)[0])
#define INT16LE(A) (int16_t) (((A)[0]) | ((A)[1] << 8))
#define UINT16LE(A) (uint16_t) (((A)[0]) | ((A)[1] << 8))
#define INT32LE(A) (int32_t) (((A)[0]) | ((A)[1] << 8) | ((A)[2] << 16) | ((A)[3] << 24))
#define UINT32LE(A) (uint32_t) (((A)[0]) | ((A)[1] << 8) | ((A)[2] << 16) | ((A)[3] << 24))

/* Helpers for importing A2M format V1-8 */
#define A2M_HEADER_FFVER(P) UINT8LE((P) + 14)
#define A2M_HEADER_NPATT(P) UINT8LE((P) + 15)
#define A2M_HEADER_SIZE (16)

#define A2M_SONGDATA_SONGNAME_P(P) (byte *) ((P) + 0)
#define A2M_SONGDATA_COMPOSER_P(P) (byte *) ((P) + 43)
#define A2M_SONGDATA_INSTR_DATA_P(P, I) (uint8_t *) ((P) + 11051 + (I) * tINSTR_DATA_SIZE)
#define A2M_SONGDATA_INSTR_NAMES_P(P, I) (uint8_t *) ((P) + 86 + (I) * 43)
#define A2M_SONGDATA_FMREG_TABLE_P(P, I) (uint8_t *) ((P) + 14621 + (I) * tFMREG_TABLE_SIZE)
#define A2M_SONGDATA_TEMPO(P) UINT8LE((P) + 1124509)
#define A2M_SONGDATA_SPEED(P) UINT8LE((P) + 1124510)
#define A2M_SONGDATA_COMMON_FLAG(P) UINT8LE((P) + 1124511)
#define A2M_SONGDATA_PATT_LEN(P) UINT16LE((P) + 1124512)
#define A2M_SONGDATA_NM_TRACKS(P) UINT8LE((P) + 1124514)
#define A2M_SONGDATA_MACRO_SPEEDUP(P) UINT16LE((P) + 1124515)
#define A2M_SONGDATA_PATTERN_ORDER_P(P, I) (uint8_t *) ((P) + 1124381 + (I))

#define A2M_PATTERN_JUMP_FLAG 0xD0
#define A2M_PATTERN_LOOP_FLAG 0xE0
#define A2M_PATTERN_BREAK_FLAG 0xF0

#define A2M_KEY_OFF_VALUE 0xFF
#define A2M_KEY_OFF_FLAG 0x80

#define A2M_EF_ARPEGGIO 0
#define A2M_EF_FREQ_SLIDE_UP 1
#define A2M_EF_FREQ_SLIDE_DOWN 2
#define A2M_EF_TONE_PORTAMENTO 3
#define A2M_EF_VIBRATO 4
#define A2M_EF_TPORT_VOLSLIDE 5
#define A2M_EF_VIB_VOLSLIDE 6
#define A2M_EF_FREQ_SLIDE_UP_FINE 7
#define A2M_EF_FREQ_SLIDE_DOWN_FINE 8
#define A2M_EF_SET_MOD_VOL 9
#define A2M_EF_VOL_SLIDE 10
#define A2M_EF_POS_JUMP 11
#define A2M_EF_SET_INS_VOLUME 12
#define A2M_EF_PATTERN_BREAK 13
#define A2M_EF_SET_TEMPO 14
#define A2M_EF_SET_SPEED 15
#define A2M_EF_TPORT_FINE_VOL_SLIDE 16
#define A2M_EF_VIBR_FINE_VOL_SLIDE 17
#define A2M_EF_SET_CARRIER_VOL 18
#define A2M_EF_SET_WAVEFORM 19
#define A2M_EF_VOL_SLIDE_FINE 20
#define A2M_EF_RETRIG_NOTE 21
#define A2M_EF_TREMOLO 22
#define A2M_EF_TREMOR 23
#define A2M_EF_ARPG_VSLIDE 24
#define A2M_EF_ARPG_VSLIDE_FINE 25
#define A2M_EF_MULTI_RETRIG_NOTE 26


#define ef_FSlideUpVSlide 27
#define ef_FSlideDownVSlide 28
#define ef_FSlUpFineVSlide 29
#define ef_FSlDownFineVSlide 30
#define ef_FSlUpVSlF 31
#define ef_FSlDownVSlF 32
#define ef_FSlUpFineVSlF 33
#define ef_FSlDownFineVSlF 34
#define ef_Extended 35
#define ef_Extended2 36
#define ef_SetGlobalVolume 37
#define ef_SwapArpeggio 38
#define ef_SwapVibrato 39
#define ef_ForceInsVolume 40
#define ef_Extended3 41
#define ef_ExtraFineArpeggio 42
#define ef_ExtraFineVibrato 43
#define ef_ExtraFineTremolo 44
#define ef_SetCustomSpeedTab 45
#define ef_GlobalFSlideUp 46
#define ef_GlobalFSlideDown 47
#define ef_GlobalFreqSlideUpXF 48// ef_fix2 replacement for >xx + ZFE
#define ef_GlobalFreqSlideDnXF 49// ef_fix2 replacement for <xx + ZFE

#define ef_ex_SetTremDepth 0
#define ef_ex_SetVibDepth 1
#define ef_ex_SetAttckRateM 2
#define ef_ex_SetDecayRateM 3
#define ef_ex_SetSustnLevelM 4
#define ef_ex_SetRelRateM 5
#define ef_ex_SetAttckRateC 6
#define ef_ex_SetDecayRateC 7
#define ef_ex_SetSustnLevelC 8
#define ef_ex_SetRelRateC 9
#define ef_ex_SetFeedback 10
#define ef_ex_SetPanningPos 11
#define ef_ex_PatternLoop 12
#define ef_ex_PatternLoopRec 13
#define ef_ex_ExtendedCmd 14
#define ef_ex_cmd_MKOffLoopDi 0
#define ef_ex_cmd_MKOffLoopEn 1
#define ef_ex_cmd_TPortaFKdis 2
#define ef_ex_cmd_TPortaFKenb 3
#define ef_ex_cmd_RestartEnv 4
#define ef_ex_cmd_4opVlockOff 5
#define ef_ex_cmd_4opVlockOn 6
#define ef_ex_cmd_ForceBpmSld 7
#define ef_ex_ExtendedCmd2 15
#define ef_ex_cmd2_RSS 0
#define ef_ex_cmd2_ResetVol 1
#define ef_ex_cmd2_LockVol 2
#define ef_ex_cmd2_UnlockVol 3
#define ef_ex_cmd2_LockVP 4
#define ef_ex_cmd2_UnlockVP 5
#define ef_ex_cmd2_VSlide_mod 6
#define ef_ex_cmd2_VSlide_car 7
#define ef_ex_cmd2_VSlide_def 8
#define ef_ex_cmd2_LockPan 9
#define ef_ex_cmd2_UnlockPan 10
#define ef_ex_cmd2_VibrOff 11
#define ef_ex_cmd2_TremOff 12
#define ef_ex_cmd2_FVib_FGFS 13
#define ef_ex_cmd2_FTrm_XFGFS 14
#define ef_ex_cmd2_NoRestart 15
#define ef_ex2_PatDelayFrame 0
#define ef_ex2_PatDelayRow 1
#define ef_ex2_NoteDelay 2
#define ef_ex2_NoteCut 3
#define ef_ex2_FineTuneUp 4
#define ef_ex2_FineTuneDown 5
#define ef_ex2_GlVolSlideUp 6
#define ef_ex2_GlVolSlideDn 7
#define ef_ex2_GlVolSlideUpF 8
#define ef_ex2_GlVolSlideDnF 9
#define ef_ex2_GlVolSldUpXF 10
#define ef_ex2_GlVolSldDnXF 11
#define ef_ex2_VolSlideUpXF 12
#define ef_ex2_VolSlideDnXF 13
#define ef_ex2_FreqSlideUpXF 14
#define ef_ex2_FreqSlideDnXF 15
#define ef_ex3_SetConnection 0
#define ef_ex3_SetMultipM 1
#define ef_ex3_SetKslM 2
#define ef_ex3_SetTremoloM 3
#define ef_ex3_SetVibratoM 4
#define ef_ex3_SetKsrM 5
#define ef_ex3_SetSustainM 6
#define ef_ex3_SetMultipC 7
#define ef_ex3_SetKslC 8
#define ef_ex3_SetTremoloC 9
#define ef_ex3_SetVibratoC 10
#define ef_ex3_SetKsrC 11
#define ef_ex3_SetSustainC 12

#define EFGR_ARPVOLSLIDE 1
#define EFGR_FSLIDEVOLSLIDE 2
#define EFGR_TONEPORTAMENTO 3
#define EFGR_VIBRATO 4
#define EFGR_TREMOLO 5
#define EFGR_VIBRATOVOLSLIDE 6
#define EFGR_PORTAVOLSLIDE 7
#define EFGR_RETRIGNOTE 8


/* Player data */
typedef struct {
	byte multipM : 4, ksrM : 1, sustM : 1, vibrM : 1, tremM : 1;
	byte multipC : 4, ksrC : 1, sustC : 1, vibrC : 1, tremC : 1;
	byte volM : 6, kslM : 2;
	byte volC : 6, kslC : 2;
	byte decM : 4, attckM : 4;
	byte decC : 4, attckC : 4;
	byte relM : 4, sustnM : 4;
	byte relC : 4, sustnC : 4;
	byte wformM : 3, : 5;
	byte wformC : 3, : 5;
	byte connect : 1, feedb : 3, : 4;// panning is not used here
} A2M_FM_INST_DATA;

typedef struct {
	A2M_FM_INST_DATA fm;
	byte panning;
	byte fine_tune;
	byte perc_voice;
} A2M_INSTR_DATA;

typedef struct {
	byte length;
	byte speed;
	byte loop_begin;
	byte loop_length;
	byte keyoff_pos;
	byte data[255];
} A2M_ARPEGGIO_TABLE;

typedef struct {
	byte length;
	byte speed;
	byte delay;
	byte loop_begin;
	byte loop_length;
	byte keyoff_pos;
	byte data[255];// array[1..255] of Shortint;
} A2M_VIBRATO_TABLE;

typedef struct {
	A2M_FM_INST_DATA fm;
	int freq_slide;
	byte panning;
	byte duration;
	byte macro_flags;// fm.data[10]
} A2M_REGISTER_TABLE_DEF;

typedef struct {
	byte length;
	byte loop_begin;
	byte loop_length;
	byte keyoff_pos;
	byte arpeggio_table;
	byte vibrato_table;
	A2M_REGISTER_TABLE_DEF data[255];
} A2M_FMREG_TABLE;

typedef struct {
	A2M_INSTR_DATA instr_data;
	byte vibrato;
	byte arpeggio;
	A2M_FMREG_TABLE fmreg;
	dword dis_fmreg_cols;
} A2M_INSTR_DATA_EXT;

typedef struct {
	byte note;
	byte instr_def;// TODO: rename to 'ins'
	struct {
		byte def;
		byte val;
	} eff[2];
} A2M_TRACK_EVENT;

typedef struct {
	word count;
	dword size;
	A2M_INSTR_DATA_EXT instruments[255];
} A2M_INSTR_INFO;


typedef struct {
	int rows;
	A2M_TRACK_EVENT *events;
} A2M_CHANNEL_INFO;

typedef struct {
	int channels;
	A2M_CHANNEL_INFO channel[16];
} A2M_PATTERN_INFO;

typedef struct {
	char songname[43];        // pascal String[42];
	char composer[43];        // pascal String[42];
	char instr_names[255][43];// array[1..255] of String[42];
	byte ffver;
	byte num_patterns;
	byte pattern_order[0x80];// array[0..0x7f] of Byte;
	byte tempo;
	byte speed;
	byte common_flag;
	word patt_len;
	byte nm_tracks;
	word macro_speedup;
	byte flag_4op;
	byte lock_flags[20];
	byte bpm_rows_per_beat;
	int bpm_tempo_finetune;

	// common flags
	bool speed_update;
	bool lockvol;
	bool panlock;
	bool lockVP;
	byte tremolo_depth;
	byte vibrato_depth;
	bool volume_scaling;
	bool percussion_mode;
	byte overall_volume;
	byte global_volume;
	byte misc_register;
	byte current_tremolo_depth;
	byte current_vibrato_depth;
	A2M_INSTR_INFO instrinfo;
	A2M_PATTERN_INFO pattern[128];

} A2M_SONGINFO;

typedef struct {
	byte def, val;
} A2M_EFFECT_TABLE;

typedef struct {
	int freq;
	byte speed;
} A2M_PORTA_TABLE;

typedef struct {
	byte state, note, add1, add2;
} A2M_ARPGG_TABLE;

typedef struct {
	byte pos;
	byte volM, volC;
} A2M_TREMOR_TABLE;

typedef struct {
	byte pos, dir, speed, depth;
	bool fine;
} A2M_VIBRTREM_TABLE;

typedef struct {
	int fmreg_pos,
			arpg_pos,
			vib_pos;
	byte
			fmreg_duration,
			arpg_count,
			vib_count,
			vib_delay,
			fmreg_ins,// fmreg_table
			arpg_table,
			vib_table,
			arpg_note;
	bool vib_paused;
	int vib_freq;
} A2M_CH_MACRO_TABLE;

typedef struct {
	A2M_FM_INST_DATA fm_table[20];
	bool volume_lock[20];
	bool vol4op_lock[20];
	bool peak_lock[20];
	bool pan_lock[20];
	byte modulator_vol[20];
	byte carrier_vol[20];
	// note/instr_def - memorized across rows
	// effects - change each row
	A2M_TRACK_EVENT event_table[20];
	byte voice_table[20];
	int freq_table[20];
	int zero_fq_table[20];
	A2M_EFFECT_TABLE effect_table[2][20];
	byte fslide_table[2][20];
	A2M_EFFECT_TABLE glfsld_table[2][20];
	A2M_PORTA_TABLE porta_table[2][20];
	bool portaFK_table[20];
	A2M_ARPGG_TABLE arpgg_table[2][20];
	A2M_VIBRTREM_TABLE vibr_table[2][20];
	A2M_VIBRTREM_TABLE trem_table[2][20];
	byte retrig_table[2][20];
	A2M_TREMOR_TABLE tremor_table[2][20];
	byte panning_table[20];
	A2M_EFFECT_TABLE last_effect[2][20];
	byte volslide_type[20];
	byte notedel_table[20];
	byte notecut_table[20];
	byte ftune_table[20];
	bool keyoff_loop[20];
	byte loopbck_table[20];
	byte loop_table[20][256];
	bool reset_chan[20];
	A2M_CH_MACRO_TABLE macro_table[20];
} A2M_CHDATA;

void A2M_Init(void);
void A2M_UnpackFile(void);
void A2M_LoadFile(const char *dat_name, const char *asset_name);
void A2M_TimerHandler(Song *song);
void A2M_SetInstrument(byte chan, A2M_INSTR_DATA instr);
void A2M_UpdateEffectsSlot(Song *song, int slot, int chan);
void A2M_Stop(void);
void A2M_Process(Song *song);
void A2M_MacroProcess(void);
void A2M_Unload(Song *song);
/// A2M.c //
extern A2M_SONGINFO *songinfo;

#endif