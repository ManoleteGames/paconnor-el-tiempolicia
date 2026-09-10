#ifndef EFFECT_H
#define EFFECT_H

#include "../types/types.h"

#define EFFECT_MAX_EFFECTS 64

#define EFFECT_LABEL_WIDTH 24
#define EFFECT_LABEL_HEIGHT 16
#define EFFECT_LABEL_NUM_FRAMES 1

extern Effect *effect;
extern int effects_counter;
void EFFECT_InitEffects(void);
void EFFECT_LoadEffect(byte entity_id, byte graphics_id, int pos_x, int pos_y, bool full_animation, int frame, bool inverted, bool loop, int speed);
void EFFECT_UpdateEffects(void);
bool EFFECT_NoEffectsActive(void);
void EFFECT_UnloadEffects(void);

#endif