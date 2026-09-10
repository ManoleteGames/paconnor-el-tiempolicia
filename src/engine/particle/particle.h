#ifndef PARTICLE_H
#define PARTICLE_H

#include "../types/types.h"

#define PARTICLE_MAX_PARTICLES 64

extern Particle *particle;
void PARTICLE_Init(void);
void PARTICLE_LoadParticle(int graphics_id, int entity_id, int source_x, int source_y, int target_x, int target_y, int speed, int damage, int range_x, int range_y);
int PARTICLE_CheckParticleColission(Particle obj);
void PARTICLE_UpdateParticles(void);
void PARTICLE_DrawColissionPixels(Particle p);
void PARTICLE_UnloadParticles(void);

#endif