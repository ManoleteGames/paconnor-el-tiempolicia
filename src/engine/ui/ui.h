#ifndef UI_H
#define UI_H

#include "../types/types.h"

#define UI_LANGUAGE_ES 0
#define UI_LANGUAGE_EN 1
#define UI_LANGUAGE_FR 2

#define UI_PANEL_WIDTH
#define UI_PANEL_HEIGHT
#define UI_MAX_BUTTONS 32

#define UI_BUTTON_IDLE_FRAME_NUM 0
#define UI_BUTTON_OVER_FRAME_NUM 1
#define UI_BUTTON_CLICK_FRAME_NUM 2

#define UI_TXT_INTRO 0
#define UI_TXT_GLOBAL 1
#define UI_TXT_CREDITS 2
#define UI_TXT_NAMES 3
#define UI_TXT_SCN1D 4
#define UI_TXT_SCN1H 5
#define UI_TXT_SCN1I 6
#define UI_TXT_SCN2D 7
#define UI_TXT_SCN2H 8
#define UI_TXT_SCN2I 9
#define UI_TXT_SCN3D 10
#define UI_TXT_SCN3H 11
#define UI_TXT_SCN3I 12
#define UI_TXT_SCN4D 13
#define UI_TXT_SCN4H 14
#define UI_TXT_SCN4I 15
#define UI_TXT_SCN5D 16
#define UI_TXT_SCN5H 17
#define UI_TXT_SCN5I 18
#define UI_TXT_SCN6D 19
#define UI_TXT_SCN6H 20
#define UI_TXT_SCN6I 21
#define UI_TXT_END 22


extern UI *ui;
extern Button *ui_button;
void UI_Init(void);
void UI_SetStatusPanels(void);
void UI_SetDefaultLanguage(void);
void UI_UpdateUI(bool combat_mode);
void UI_LoadButton(byte number, byte entity_id, byte graphics_id, int pos_x, int pos_y);
void UI_UpdateButtons(void);
void UI_ButtonOver(int button_number);
void UI_ButtonClick(int button_number);
void UI_SetButtonPosition(byte number, int x, int y);
int UI_GetButtonXPosition(byte number);
int UI_GetButtonYPosition(byte number);
void UI_ShowSpeech(ChatPanel *pannel, int portait_graphics_id, int box_graphics_id, int text_file, int line1, int line2, int line3, bool inverted, int timeout);
void UI_ShowDescription(int text_file, int line);
void UI_UnloadButtons(void);
bool UI_IsSpeechFinished(void);
void UI_ShowLoadingScreen(void);
void UI_HideLoadingScreen(void);

#endif