#include <bios.h>
#include <dpmi.h>
#include <sys/nearptr.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/farptr.h>
#include <go32.h>
#include <io.h>
#include <dos.h>
#include <string.h>
#include <pc.h>
#include <crt0.h>
#include <errno.h>
#include <assert.h>
#include <dos.h>
#include <dpmi.h>
#include <go32.h>
#include <pc.h>


#include "settings.h"
#include "types/types.h"


#include "utils/utils.h"

#include "timer/timer.h"

#include "video/video.h"
#include "video/vga.h"
#include "video/ega.h"
#include "video/cga.h"

#include "mouse/mouse.h"

#include "file/file.h"
#include "gfx/gfx.h"

#include "keyb/keyb.h"
#include "mem/mem.h"
#include "actor/actor.h"
#include "camera/camera.h"
#include "map/map.h"
#include "object/object.h"
#include "item/item.h"
#include "enemy/enemy.h"
#include "boss/boss.h"

#include "grenade/grenade.h"
#include "bullet/bullet.h"

#include "effect/effect.h"

#include "audio/audio.h"
#include "audio/speaker.h"
#include "audio/a2m.h"
#include "audio/adlib.h"

#include "npc/npc.h"

#include "particle/particle.h"
#include "ui/ui.h"


#define DOS_MAJOR_VERSION 5
#define DOS_MINOR_VERSION 0

#define PIC 0x20
#define NONSPECIFIC_EOI 0x20

#define ERROR_SYSTEM 1
#define ERROR_MEMORY 2
#define ERROR_VIDEO 3
#define ERROR_FILE 4
#define ERROR_SOUND 5
#define ERROR_GRAPHICS 6
#define ERROR_MOUSE 7
#define ERROR_BINARY_FILE 99

#define ENTITY_ID_EMPTY 0x00
#define ENTITY_ID_BACKGROUND 0x01
#define ENTITY_ID_BREAKABLE_BACKGROUND 0x02
#define ENTITY_ID_CURSOR 0x20
#define ENTITY_ID_ACTOR 0x30
#define ENTITY_ID_ACTOR_BULLET 0x40
#define ENTITY_ID_ACTOR_PUNCH 0x41
#define ENTITY_ID_ACTOR_KICK 0x42
#define ENTITY_ID_ACTOR_GRENADE 0x43
#define ENTITY_ID_NO_BREAK_OBJ 0x4F
#define ENTITY_ID_BARREL 0x50
#define ENTITY_ID_NPC 0x51
#define ENTITY_ID_TARGET 0x52
#define ENTITY_ID_ITEM_MEDIKIT 0x53
#define ENTITY_ID_ITEM_AMMO1 0x54
#define ENTITY_ID_ITEM_AMMO2 0x55
#define ENTITY_ID_ITEM_AMMO3 0x56
#define ENTITY_ID_ITEM_AMMO4 0x57
#define ENTITY_ID_ITEM_GRENADE 0x58
#define ENTITY_ID_ITEM_KEYRED 0x59
#define ENTITY_ID_ITEM_KEYBLUE 0x5A
#define ENTITY_ID_ITEM_KEYYELLOW 0x5B
#define ENTITY_ID_ITEM_KEYGREEN 0x5C
#define ENTITY_ID_ITEM_DOCU 0x5D
#define ENTITY_ID_ENEMY 0x60
#define ENTITY_ID_BOSS 0x61
#define ENTITY_ID_ENEMY_BULLET 0x70
#define ENTITY_ID_EXPLOSION 0x80
#define ENTITY_ID_BLOOD 0x81
#define ENTITY_ID_BTN 0x10
#define ENTITY_ID_HSPOT 0x90

// Lock memory macros
#define END_OF_FUNCTION(x) \
	void x##_end() {}

#define LOCK_VARIABLE(x) _go32_dpmi_lock_data((void *) &x, sizeof(x))

#define LOCK_FUNCTION(x) _go32_dpmi_lock_code(x, (long) x##_end - (long) x)

extern Settings settings;
extern System engine;
extern int errno;
void InitEngine(void);
void AwaitInput(void);
void Error(const char *quit_message1, const char *quit_message2, const char *quit_message3, int exit_code);
void ExitDos(void);
void Update(int player_follow);
void UpdateStatusPannel(void);
void FPS(void);
void SetDelayTime(int time);
bool AwaitDelayTime(void);
