
#ifndef FILE_H
#define FILE_H

#include "../types/types.h"
#include <stdio.h>

/// FILE.C ///
void FILE_LoadBinaryImage(const char *dat_name, const char *asset_name, byte *buffer);
void FILE_LoadPCXImage(const char *dat_name, const char *asset_name, byte *buffer, long size, word *width, word *height);
void FILE_LoadPCXTileset(const char *dat_name, const char *asset_name, byte *buffer, long size, word *width, word *height);
void FILE_LoadMap_CSV(const char *dat_name, unsigned short *back, unsigned short *fore, unsigned short *mask, unsigned short *col, unsigned short *anim, unsigned short *event_hspot, int size);
void FILE_LoadPCXSprite(const char *dat_name, const char *asset_name, byte *buffer, long size, word *width, word *height, int palette_offset);
void FILE_LoadPCXPalette(const char *dat_name, const char *asset_name, byte *buffer, long size);
dword FILE_SeekAssetOffset(FILE *fp, const char *filename);
byte *FILE_LoadA2MSongInfo(const char *dat_name, const char *asset_name, dword *size, int mem_type);
void FILE_LoadTextFile(const char *dat_name, const char *asset_name, TextFile *tf);
void FILE_LoadAnimationFile(const char *dat_name, const char *asset_name, SpriteAnimation *ani, int max_animations);
void FILE_LoadSettingsFile(const char *config_file);
void FILE_LoadSpeakerSong(const char *dat_name, const char *asset_name, Song *song);
void FILE_LoadSpriteConfigFile(const char *dat_name, const char *asset_name, SpriteConfig *cfg);
void FILE_SaveSettingsFile(const char *config_file);

#endif