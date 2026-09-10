#ifndef MEM_H
#define MEM_H

#include "../types/types.h"

#define MM_CURRENT_TOP_PTR() (void *) (mmRawMem + mmMemUsed)
#define MM_MAX_NUM_CHUNKS 1550
#define MM_RAM_REQ 8000

void MM_Init(void);
void MM_Shutdown(void);
void *MM_PushChunk(word size, ChunkType type);
void MM_PopChunks(ChunkType type);
dword MM_GetMemTotal(void);
dword MM_GetMemUsed(void);
word MM_GetChunksUsed(void);
void *MM_GetCurrentMemAddress(void);
byte *MM_GetRawMemAddress(void);

#endif