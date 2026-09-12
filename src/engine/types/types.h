#ifndef TYPES_H
#define TYPES_H

enum { false = 0,
	   true = 1 };

typedef unsigned char byte;
typedef unsigned int word;
typedef unsigned long dword;
typedef int ibool;
typedef byte bool;
typedef signed char sbyte;

typedef struct {
	int video_mode;
	int sound_device;
	int sound_volume;
	int music_device;
	int music_volume;
	int mouse_enabled;

	int language;

	int up_key;
	int down_key;
	int left_key;
	int right_key;
	int loop_key;
	int fire_key;
	int throw_key;
	int change_gun_key;
	int combat_mode_key;

} Settings;

typedef struct {
	bool debug_mode;
	bool good_mode;

	bool dosVersionOK;
	bool memOK;
	bool videoOK;
	bool audioOK;
	bool mouseOK;

	bool loading;
	bool logo;
	bool sequence;
	bool ingame;
	bool exit_game;
	bool exit_menu;
	int scene;
	int room;

	// Timer
	bool timer_initialized;

	// Keyboard
	bool keyboard_initialized;

	// Audio
	bool audio_initialized;
	bool adlib_present;
	bool speaker_present;
	bool sb_present;

	// Memory
	bool mem_initialized;

	// Mouse
	bool mouse_initialized;
	bool mouse_present;

	// Video
	bool video_initialized;
	bool VGA_present;
	bool EGA_present;
	bool CGA_present;

	char system_error_message1[80];
	char system_error_message3[80];
	char system_error_message2[80];

	int debug1_INT;
	int debug2_INT;
	int debug3_INT;
	int debug4_INT;
	int debug5_INT;
	int debug6_INT;
	int debug7_INT;
	int debug8_INT;

	int fps;
	int frame_counter;
	dword limit_old_time;
	dword fps_old_time;
	dword start_time;
	dword sample_time;

	int interrupt_time_ms;
	int delay_time_ms;

} System;

typedef struct {
	byte tremolo;
	byte vibrato;
	byte eg_type;
	byte ksr;
	byte multi;
	byte ksl;
	byte total_level;
	byte attack;
	byte decay;
	byte sustain;
	byte release;
	byte wave_shape;
} Operator;

typedef struct {
	Operator modulator;
	Operator carrier;
	byte feedback;
	byte syn_type;

} Instrument;

typedef struct {
	word base;
	word dsp_version;
	byte lo_DMA;
	byte hi_DMA;
	byte irq;
	int fm_port;
} SBlaster;

typedef struct {
	word opl;
	word port;
	int current_block;
	int current_note;
} Adlib;

typedef struct {
	bool shown;
	int timeout;
	int current_time;

	int portait_graphics_id;
	int portait_x, portait_y;
	int portait_frame;

	int lifebar_graphics_id;
	int lifebar_x, lifebar_y;
	int current_life, old_life;
	int life_speed, life_speed_counter;

	int key_type;
	int key_graphics_id;
	int key_x, key_y;

	int gun_type;
	int gun_graphics_id;
	int gun_x, gun_y;
	int bullets_number;
	int total_bullets;

	int grenade_graphics_id;
	int grenade_x, grenade_y;
	int grenades_number;

	word width;
	word height;
	int pos_x;
	int pos_y;
	int transition_width;
	int transition_height;
} StatusPanel;

typedef struct {
	int timeout;
	int current_time;

	int portait_graphics_id;
	int portait_x, portait_y;
	int portait_frame;
	bool portait_inverted;

	int chat_graphics_id;
	int chat_x, chat_y;
	int chat_frame;
	bool chat_inverted;

	char line[3][40];
	int line_length[3];
	int line_count[3];

	int pos_x;
	int pos_y;

} ChatPanel;

typedef struct {

	int screen_width, screen_height;
	byte *screen_buffer[3];

	byte *map_buffer[3];
	word map_buffer_width, map_buffer_height;

	bool fading_in_async;
	bool faded_in;
	bool fading_out_async;
	bool faded_out;
	int fading_speed;
	int fading_step;
	bool rotate_palette_async;
	int rotate_first_index;
	int rotate_last_index;
	int rotate_speed;


} Video;

typedef struct {

	int visible_width;
	int visible_height;
	int offset_x;
	int offset_y;
	int adjustment_x;
	int adjustment_y;
	int pos_x, pos_y;
	int old_pos_x, old_pos_y;
	int pos_abs_x, pos_abs_y;
	int old_pos_abs_x, old_pos_abs_y;
	int pos_max_x, pos_max_y;
	byte speed_index;
	byte speed_array[65];
	byte speed[49];

} Camera;

typedef struct {
	word width_px;
	word height_px;
	int width;
	int height;
	int num_tiles;
	byte *buffer;

} Tileset;

typedef struct {
	unsigned short *layer[6];
	Tileset layer_tiles[3];

	word width;
	word height;
	word width_px;
	word height_px;

	bool update_required;
	bool loaded;
	bool shown;

	bool animation_frame;
	int animation_counter;
	int animation_counter_x;
	int animation_counter_y;

	bool *hotspot_enabling_room1;
	bool *event_enabling_room1;
	bool *hotspot_enabling_room2;
	bool *event_enabling_room2;

} Map;

typedef struct {
	byte width;
	byte height;
	byte face_width;
	byte face_height;
	byte face_frames;
	byte portait_width;
	byte portait_height;
	byte portait_frames;
	byte feet_width;
	byte feet_height;
	byte feet_frames;
	byte body_width;
	byte body_height;
	byte body_frames;
	byte head_width;
	byte head_height;
	byte head_frames;
	byte larm_width;
	byte larm_height;
	byte larm_frames;
	byte rarm_width;
	byte rarm_height;
	byte rarm_frames;
} SpriteConfig;

typedef struct {
	bool active;
	bool blinked;
	byte time;
	byte counter;
	byte color;
} SpriteBlink;


typedef struct {
	int offset_x;
	int offset_y;
	int frame;
} AnimData;

typedef struct {
	byte current_frame;
	bool loop;
	bool end;
	bool inverted;
	byte frames;
	byte speed;
	byte counter;
	AnimData anim_data[10];
} SpriteAnimation;

typedef struct {
	bool loaded;
	bool invisible;
	bool blink;
	bool inverted;
	bool unmasked;

	byte blink_color;

	int graphics_id;
	int width_px;
	int heigth_px;
	int screen_pos_x;
	int screen_pos_y;
	int offset_x;
	int offset_y;
	int frame;

	byte anim_current_frame;
	bool anim_loop;
	bool anim_end;
	byte anim_frames;
	byte anim_speed;
	byte anim_counter;
	AnimData anim_data[10];
} SpriteGfx;

typedef struct {
	bool loaded;
	bool shown;
	bool invisible;
	bool unmasked;

	int id;
	byte entity_id;
	byte priority;

	int screen_pos_x;
	int screen_pos_y;
	int width_px;
	int height_px;

	SpriteBlink blink;
	SpriteGfx *gfx;
	byte gfx_order[5];
} Sprite;

typedef struct {
	bool loaded;
	int width_px;
	int height_px;
	int num_frames;
	unsigned short *frame_offset;
	unsigned short *row_offset;
	byte transparent_color;
	byte hit_color;
	byte *buffer;
} Graphic;

typedef struct {
	bool left_click;      // left click flag
	bool right_click;     // right click flag
	bool left_click_FP;   // left click flange positive
	bool right_click_FP;  // right click flange positive
	bool left_click_FN;   // left click flange negative
	bool right_click_FN;  // right click flange negative
	bool right_click_mask;// right click mask to avoid fast double right click
	bool left_click_status_old;
	bool right_click_status_old;

	int pos_x, pos_y;// Position values
	int width_px, height_px;
	int screen_middle_x_px, screen_middle_y_px;
	int sprite_num;
	int current_cursor_id;
	int point_on;
	int right_clicK_delay_time;
	int right_clicK_delay_counter;
} MouseData;

typedef struct {
	int lines;
	char line[356][40];
} TextFile;

typedef struct {
	bool show_description;
	bool show_speech;
	bool freeze;
	bool pause;
	bool exit_request;
	bool actor_was_on_combat_mode;
	int button_pressed;

	int language;// 0=ES; 1=EN; 2=FR

	byte name_length;
	byte speech_length;
	byte speech_timeout;
	byte speech_time;
	unsigned char object_name[40];
	unsigned char speech[40];
	TextFile *txt_file[25];
} UI;

typedef struct {
	int points[4][2];
} Area;

typedef struct {
	bool loaded;
	bool shown;
	bool on_target;

	int pos_x, pos_y;
	byte speed;
	byte damage;
	int hit_on;
	int steps;
	int current_step;
	int target_x;
	int target_y;

	dword vx_FP;
	dword vy_FP;
	dword speed_FP;
	dword x_FP, y_FP;

	int sprite_num;
	int graphics_id;
	int width_px;
	int height_px;

	Area colission_area;

} Bullet;

typedef struct {
	bool loaded;
	bool shown;
	bool hold;
	bool on_target;

	int pos_x, pos_y, pos_z;
	byte speed;
	byte damage;
	int hit_on;
	int steps;
	int current_step;
	int target_x;
	int target_y;

	int current_time;
	int end_time_ms;
	int current_time_ms;
	bool tick_played, tack_played;

	dword vx_FP;
	dword vy_FP;
	dword vz_FP;
	dword speed_FP;
	dword g_FP;
	dword x_FP, y_FP, z_FP;

	int sprite_num;
	int graphics_id;
	int width_px;
	int height_px;

	Area colission_area;

} Grenade;

typedef struct {
	bool loaded;
	bool shown;
	bool on_target;

	int pos_x, pos_y;
	byte speed;
	int hit_on;
	int steps;
	int current_step;
	int target_x;
	int target_y;

	dword vx_FP;
	dword vy_FP;
	dword speed_FP;
	dword x_FP, y_FP;

	int damage;

	int sprite_num;
	int graphics_id;

	Area colission_area;

} Particle;

typedef struct {
	bool is_loaded;
	int life;
	byte max_life;
	byte type;
	bool life_shown;
	byte life_shown_counter;
	bool on_screen;

	int width_px;
	int height_px;
	int pos_x, pos_y;

	bool is_hit;
	bool explode;
	byte hit_by;
	byte damage;

	int steps;
	int current_step;
	int num_sprite;

	int sprite_graphics_id;
	int portait_graphics_id;

	SpriteConfig *cfg;
	Area colission_area;
	Area hit_area;
} Object;

typedef struct {
	bool is_loaded;
	byte type;
	bool on_screen;
	int pos_x, pos_y;
	int num_sprite;
	Area colission_area;
} Item;

typedef struct {
	bool is_loaded;
	byte type;

	int pos_x, pos_y;

	int steps;
	int current_step;

	int num_sprite;
} Effect;

typedef struct {
	bool is_loaded;
	byte type;

	int pos_x, pos_y;

	byte hit_by;

	int steps;
	int current_step;
	int num_sprite;
	int graphic_index;

	Area hit_area;
} Button;

typedef struct {
	bool accurate;
	byte type;
	byte max_accuracy;
	byte shoots;
	byte current_recoil;
	byte recoil_time;
	byte damage;
	byte bullet_speed;

	int current_bullets;
	int current_pistol_bullets;
	int current_shotgun_bullets;
	int current_uzi_bullets;
	int current_snipper_bullets;

	int total_bullets;
	int total_pistol_bullets;
	int total_shotgun_bullets;
	int total_uzi_bullets;
	int total_snipper_bullets;

	int max_distance;
	int graphics_id;
	int effect_graphics_id;
	int bullet_graphics_id;
} Gun;

typedef struct {

	bool is_loaded;

	bool action_shoot;
	bool action_throw_grenade;
	bool action_punch;
	bool action_loop;
	bool action_walk;
	bool action_stand;
	bool action_dying;
	bool action_dead;
	bool action_hit;
	bool action_change_gun;
	byte action_punch_combo;
	byte action_reload;

	bool is_hit;
	byte hit_by;
	bool life_shown;
	byte life_shown_counter;

	bool status_facing_up;
	bool status_facing_up_right;
	bool status_facing_right;
	bool status_facing_down_right;
	bool status_facing_down;
	bool status_facing_down_left;
	bool status_facing_left;
	bool status_facing_up_left;
	bool status_idle;
	bool status_dead;
	bool status_freeze;

	bool mode_combat;

	int pos_x, pos_y;
	int middle_x, middle_y;
	int speed;
	dword vx_FP;
	dword vy_FP;
	dword speed_horizontal_FP;
	dword speed_vertical_FP;
	dword speed_diagonalv_FP;
	dword speed_diagonalh_FP;
	dword speed_horizontal_loop_FP;
	dword speed_vertical_loop_FP;
	dword speed_diagonalv_loop_FP;
	dword speed_diagonalh_loop_FP;
	dword x_FP, y_FP;

	int max_life;
	int life;
	byte damage;

	int idle_counter;
	int last_action;
	byte last_movement;
	int last_facing;
	int action_step;

	int shoot_x, shoot_y;

	byte shoot_accuracy;
	byte shoot_accuracy_speed;
	byte shoot_accuracy_counter;

	byte throw_accuracy;
	byte throw_accuracy_speed;
	byte throw_accuracy_counter;

	int current_grenades;
	int grenades_max_distance;
	int grenades_max_time;
	int grenade_index;
	byte current_throw_time;
	byte throw_time;

	int sprite_num;
	int width_px;
	int height_px;

	int blink_counter;

	Area colission_area;
	Area hit_area;

	Gun *gun;
	int new_gun;

	int key_entity_id;
	int key_graphics_id;

	SpriteAnimation *feet_animation;
	SpriteAnimation *body_animation;
	SpriteAnimation *head_animation;
	SpriteAnimation *left_arm_animation;
	SpriteAnimation *right_arm_animation;
} Actor;

typedef struct {
	bool is_loaded;
	bool is_hit;

	bool action_shoot;
	bool action_punch;
	bool action_walk;
	bool action_stand;
	bool action_hit;
	bool action_dead;

	int status_behavior;

	bool status_facing_up;
	bool status_facing_up_right;
	bool status_facing_right;
	bool status_facing_down_right;
	bool status_facing_down;
	bool status_facing_down_left;
	bool status_facing_left;
	bool status_facing_up_left;
	int status_facing;

	int pos_x, pos_y;
	int middle_x, middle_y;
	int speed;
	dword vx_FP;
	dword vy_FP;
	dword speed_horizontal_FP;
	dword speed_vertical_FP;
	dword speed_diagonalv_FP;
	dword speed_diagonalh_FP;
	dword x_FP, y_FP;

	bool movement_allowed;
	int max_life;
	int life;
	byte damage;
	byte hit_by;
	dword hit_vx_FP;
	dword hit_vy_FP;

	int idle_counter;
	int last_action;
	int action_step;
	int pattern_step;
	int current_pattern;

	int blink_counter;

	bool on_screen;
	bool in_shoot_range;
	bool can_see_actor;
	int shoot_range;
	int shoot_x, shoot_y;
	byte shoot_accuracy;
	byte shoot_recoil_time;
	byte shoot_recoil_counter;
	byte reaction_time;
	byte reaction_counter;

	byte gfx_portait;

	byte movement;
	int sprite_num;
	int width_px;
	int height_px;

	Area colission_area;
	Area hit_area;

	Gun *gun;
	SpriteConfig *cfg;

	SpriteAnimation *feet_animation;
	SpriteAnimation *body_animation;
	SpriteAnimation *head_animation;
	SpriteAnimation *left_arm_animation;
	SpriteAnimation *right_arm_animation;
} Enemy;

typedef struct {
	bool is_loaded;
	bool is_hit;

	int status_behavior;

	bool action_walk;
	bool action_stand;
	bool action_hit;
	bool action_dead;

	bool status_facing_up;
	bool status_facing_up_right;
	bool status_facing_right;
	bool status_facing_down_right;
	bool status_facing_down;
	bool status_facing_down_left;
	bool status_facing_left;
	bool status_facing_up_left;
	int status_facing;

	int pos_x, pos_y;
	int speed;
	dword vx_FP;
	dword vy_FP;
	dword speed_horizontal_FP;
	dword speed_vertical_FP;
	dword speed_diagonalv_FP;
	dword speed_diagonalh_FP;
	dword x_FP, y_FP;

	bool movement_allowed;
	int max_life;
	int life;
	byte damage;
	byte hit_by;
	dword hit_vx_FP;
	dword hit_vy_FP;

	int idle_counter;
	int last_action;
	int action_step;
	int pattern_step;
	int current_pattern;

	bool on_screen;
	bool in_range;
	bool can_see_actor;
	int range;
	byte reaction_time;
	byte reaction_counter;

	byte movement;
	int sprite_num;
	int width_px;
	int height_px;

	Area colission_area;
	Area hit_area;

	SpriteConfig *cfg;
} NPC;

typedef struct {
	bool is_loaded;
	bool is_hit;

	bool action_shoot;
	bool action_punch;
	bool action_walk;
	bool action_stand;
	bool action_hit;
	bool action_dead;

	int status_behavior;

	int pos_x, pos_y;
	int middle_x, middle_y;
	int speed;
	dword vx_FP;
	dword vy_FP;
	dword speed_horizontal_FP;
	dword speed_vertical_FP;
	dword speed_diagonalv_FP;
	dword speed_diagonalh_FP;
	dword x_FP, y_FP;

	bool movement_allowed;
	int max_life;
	int life;
	byte damage;
	byte hit_by;
	dword hit_vx_FP;
	dword hit_vy_FP;

	int idle_counter;
	int last_action;
	int action_step;
	int pattern_step;
	int current_pattern;

	int blink_counter;

	byte boss_update_counter;
	byte boss_counter;

	bool on_screen;
	bool in_punch_range;
	bool in_shoot_range;
	bool can_see_actor;
	int punch_range;
	int shoot_range;
	int shoot_x, shoot_y;
	byte shoot_accuracy;
	byte shoot_recoil_time;
	byte shoot_recoil_counter;
	byte reaction_time;
	byte reaction_counter;

	byte gfx_portait;

	byte movement;
	int sprite_num;
	int width_px;
	int height_px;

	Area colission_area;
	Area hit_area;

	Gun *gun;

	SpriteConfig *cfg;
	SpriteAnimation *feet_animation;
	SpriteAnimation *body_animation;
	SpriteAnimation *head_animation;
	SpriteAnimation *left_arm_animation;
	SpriteAnimation *right_arm_animation;

	int *boss_hold_on_pattern;
	int *boss_chase_pattern;
	int *boss_attack_pattern;
	int *boss_rampage_pattern;
	int *boss_static_pattern;

} Boss;

typedef enum {
	CT_ENGINE,
	CT_PALETTE,
	CT_FONT,
	CT_GRAPHICS,
	CT_VIDEO_BUFFER,
	CT_MOUSE,
	CT_TILES,
	CT_MAP,
	CT_COMMON,
	CT_ACTOR,
	CT_SPRITE,
	CT_MAP_DATA,
	CT_MUSIC,
	CT_TEMPORARY,
	CT_TEMPORARY_SPRITE
} ChunkType;

typedef struct {
	unsigned char r, g, b;
} Color;

typedef struct {

	byte *buffer;
	int note;
	int note_index;
	int note_duration;
	int note_time;
	byte current_sound_priority;
	bool play, playing_note;

} SoundEffect;

typedef struct {
	bool enabled;
	void (*callback)(byte);// Function pointer for the callback
	byte param;
	dword current_time;
	dword end_time;// When the callback should be called
} Timeout;

typedef struct {
	bool loaded;
	bool play;
	bool stop;
	bool pause;
	bool songend;
	bool loop;

	bool speed_update;

	int current_pattern;
	int current_line;
	int current_order;
	bool pattern_break;
	bool pattern_delay;
	int tickXF;
	int tickD;
	int ticks;
	int next_line;
	int next_order;
	bool irq_mode;

	int ticklooper;
	int macro_ticklooper;
	int speed;
	int tempo;
	int macro_speedup;

	byte *buffer;
	long buffer_max_size;

} Song;

typedef struct {
	byte *data;
	word char_width;
	word char_height;
} Font;

typedef struct {
	byte *palette_shown;
	byte *palette_loaded;
	Font font[5];
	byte *image_buffer1;
	word image_buffer1_width;
	word image_buffer1_height;
	byte *image_buffer2;
	word image_buffer2_width;
	word image_buffer2_height;
	byte *image_buffer3;
	word image_buffer3_width;
	word image_buffer3_height;

} Graphics;


#endif
