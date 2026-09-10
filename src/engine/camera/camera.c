#include "../engine.h"
#include "camera.h"

Camera *camera;

/** CAM :: Initialize Camera
 * Initializes camera structure and default values
 */
void CAM_Init(int x_limit, int y_limit, int x_pos, int y_pos) {
	int i;

	camera = MM_PushChunk(sizeof(Camera), CT_ENGINE);

	// Check camera resolution
	if (video->screen_width < CAM_VISIBLE_WIDTH || video->screen_height < CAM_VISIBLE_HEIGHT) {
		sprintf(engine.system_error_message1, "CAM_Init function error");
		sprintf(engine.system_error_message2, "Camera resolution too high for current resolution");
		sprintf(engine.system_error_message3, "Camera resolution: %dx%d", CAM_VISIBLE_WIDTH, CAM_VISIBLE_HEIGHT);
		Error(engine.system_error_message1, engine.system_error_message2, engine.system_error_message3, ERROR_FILE);
	}
	camera->visible_width = CAM_VISIBLE_WIDTH;
	camera->visible_height = CAM_VISIBLE_HEIGHT;
	camera->offset_x = CAM_OFFSET_X;
	camera->offset_y = CAM_OFFSET_Y;
	camera->adjustment_x = 0;
	camera->adjustment_y = 0;
	camera->pos_x = x_pos;
	camera->pos_y = y_pos;
	camera->old_pos_x = x_pos;
	camera->old_pos_y = y_pos;
	camera->pos_abs_x = x_pos % video->map_buffer_width;
	camera->pos_abs_y = y_pos % video->map_buffer_height;
	camera->old_pos_abs_x = x_pos % video->map_buffer_width;
	camera->old_pos_abs_y = y_pos % video->map_buffer_height;
	camera->pos_max_x = x_limit - CAM_VISIBLE_WIDTH;
	camera->pos_max_y = y_limit - CAM_VISIBLE_HEIGHT;
	camera->speed_index = 0;

	camera->speed[0] = 0;
	camera->speed[1] = 0;
	camera->speed[2] = 0;
	camera->speed[3] = 0;
	camera->speed[4] = 0;
	camera->speed[5] = 0;
	camera->speed[6] = 0;
	camera->speed[7] = 0;

	camera->speed[8] = CAM_SPEED;
	camera->speed[9] = CAM_SPEED;
	camera->speed[10] = 0;
	camera->speed[11] = 0;
	camera->speed[12] = CAM_SPEED;
	camera->speed[13] = CAM_SPEED;
	camera->speed[14] = 0;
	camera->speed[15] = 0;

	camera->speed[16] = CAM_SPEED;
	camera->speed[17] = CAM_SPEED;
	camera->speed[18] = CAM_SPEED;
	camera->speed[19] = 0;
	camera->speed[20] = CAM_SPEED;
	camera->speed[21] = CAM_SPEED;
	camera->speed[22] = CAM_SPEED;
	camera->speed[23] = 0;

	camera->speed[24] = CAM_SPEED;
	camera->speed[25] = CAM_SPEED;
	camera->speed[26] = CAM_SPEED;
	camera->speed[27] = CAM_SPEED;
	camera->speed[28] = CAM_SPEED;
	camera->speed[29] = CAM_SPEED;
	camera->speed[30] = CAM_SPEED;
	camera->speed[31] = CAM_SPEED;

	camera->speed[32] = CAM_SPEED << 1;
	camera->speed[33] = CAM_SPEED << 1;
	camera->speed[34] = CAM_SPEED << 1;
	camera->speed[35] = CAM_SPEED << 1;
	camera->speed[36] = CAM_SPEED << 1;
	camera->speed[37] = CAM_SPEED << 1;
	camera->speed[38] = CAM_SPEED << 1;
	camera->speed[39] = CAM_SPEED << 1;

	camera->speed[40] = CAM_SPEED << 2;
	camera->speed[41] = CAM_SPEED << 2;
	camera->speed[42] = CAM_SPEED << 2;
	camera->speed[43] = CAM_SPEED << 2;
	camera->speed[44] = CAM_SPEED << 2;
	camera->speed[45] = CAM_SPEED << 2;
	camera->speed[46] = CAM_SPEED << 2;
	camera->speed[47] = CAM_SPEED << 2;
	camera->speed[48] = CAM_SPEED << 2;
	camera->speed[49] = CAM_SPEED << 2;

	camera->speed_array[0] = 0;
	camera->speed_array[1] = 1;
	camera->speed_array[2] = 2;
	camera->speed_array[3] = 3;
	camera->speed_array[4] = 4;
	camera->speed_array[5] = 4;
	camera->speed_array[6] = 4;
	camera->speed_array[7] = 4;
	camera->speed_array[8] = 4;
	camera->speed_array[9] = 4;

	for (i = 10; i < 24; i++) camera->speed_array[i] = 4;
	for (i = 24; i < 65; i++) camera->speed_array[i] = 5;
}

/** CAM :: Set Position
 * Sets the current camera position
 */
void CAM_SetPosition(int x, int y) {
	// Check is inside limits
	if (x > camera->pos_max_x) {
		camera->pos_x = camera->pos_max_x;
	} else if (x < 0) {
		camera->pos_x = 0;
	} else {
		camera->pos_x = x;
	}

	if (y > camera->pos_max_y) {
		camera->pos_y = camera->pos_max_y;
	} else if (y < 0) {
		camera->pos_y = 0;
	} else {
		camera->pos_y = y;
	}

	// Calculate vertical and horizontal breakpoint
	camera->pos_abs_x = camera->pos_x % video->map_buffer_width;
	camera->pos_abs_y = camera->pos_y % video->map_buffer_height;
}

/** CAM :: Set Limits
 * Sets the maximum scrolling limits for the camera
 */
void CAM_SetLimits(int x, int y) {
	camera->pos_max_x = x - camera->visible_width;
	camera->pos_max_y = y - camera->visible_height;
}

/** CAM :: Main camera position Control
 * Smoothly moves the camera to follow the player
 */
void CAM_MainPositionControl(bool player_follow, bool *map_update_flag) {
	int target_cam_x, target_cam_y;
	int displacement_x, displacement_y;
	int displacement_abs_x, displacement_abs_y;
	int speed_x, speed_y;

	camera->old_pos_x = camera->pos_x;
	camera->old_pos_y = camera->pos_y;
	camera->old_pos_abs_x = camera->pos_abs_x;
	camera->old_pos_abs_y = camera->pos_abs_y;

	if (player_follow) {
		// Calculate target scroll position
		target_cam_x = actor->pos_x + ((cursor.pos_x - CAM_VISIBLE_WIDTH) - ((cursor.pos_x - CAM_VISIBLE_WIDTH) >> 2));
		target_cam_y = actor->pos_y + ((cursor.pos_y - CAM_VISIBLE_HEIGHT) - ((cursor.pos_y - CAM_VISIBLE_HEIGHT) >> 2));

	} else {
		target_cam_x = camera->pos_x;
		target_cam_y = camera->pos_y;
	}

	// Limit target scroll position
	if (target_cam_x < 0) { target_cam_x = 0; }
	if (target_cam_x > camera->pos_max_x) { target_cam_x = camera->pos_max_x; }
	if (target_cam_y < 0) { target_cam_y = 0; }
	if (target_cam_y > camera->pos_max_y) { target_cam_y = camera->pos_max_y; }

	// Calculate displacement
	displacement_x = target_cam_x - camera->pos_x;
	displacement_y = target_cam_y - camera->pos_y;

	displacement_abs_x = abs(displacement_x);
	displacement_abs_y = abs(displacement_y);

	// Limit speed
	if (displacement_abs_x > 64) { displacement_abs_x = 64; }
	if (displacement_abs_x < 0) { displacement_abs_x = 0; }
	if (displacement_abs_y > 64) { displacement_abs_y = 64; }
	if (displacement_abs_y < 0) { displacement_abs_y = 0; }

	// Reset camera
	if (camera->speed_index == 8) { camera->speed_index = 0; }

	// Calculate scroll speed
	speed_x = camera->speed[(camera->speed_array[displacement_abs_x] << 3) + camera->speed_index];
	speed_y = camera->speed[(camera->speed_array[displacement_abs_y] << 3) + camera->speed_index];

	if ((displacement_x > 0) && (camera->pos_x < camera->pos_max_x)) {
		switch (speed_x) {
			case 5:
				if ((camera->pos_x & 0xF) == 0) { *map_update_flag = true; }
				camera->pos_x++;
			case 4:
				if ((camera->pos_x & 0xF) == 0) { *map_update_flag = true; }
				camera->pos_x++;
			case 3:
				if ((camera->pos_x & 0xF) == 0) { *map_update_flag = true; }
				camera->pos_x++;
			case 2:
				if ((camera->pos_x & 0xF) == 0) { *map_update_flag = true; }
				camera->pos_x++;
			case 1:
				if ((camera->pos_x & 0xF) == 0) { *map_update_flag = true; }
				camera->pos_x++;
			default:
				break;
		}
		//camera->pos_x += speed_x;
	}
	if ((displacement_x < 0) && (camera->pos_x > 0)) {
		switch (speed_x) {
			case 5:

				if ((camera->pos_x & 0xF) == 0) { *map_update_flag = true; }
				camera->pos_x--;
			case 4:
				if ((camera->pos_x & 0xF) == 0) { *map_update_flag = true; }
				camera->pos_x--;
			case 3:
				if ((camera->pos_x & 0xF) == 0) { *map_update_flag = true; }
				camera->pos_x--;
			case 2:
				if ((camera->pos_x & 0xF) == 0) { *map_update_flag = true; }
				camera->pos_x--;
			case 1:
				if ((camera->pos_x & 0xF) == 0) { *map_update_flag = true; }
				camera->pos_x--;
			default:
				break;
		}
		//camera->pos_x -= speed_x;
	}
	if ((displacement_y > 0) && (camera->pos_y < camera->pos_max_y)) {
		switch (speed_y) {
			case 5:
				if ((camera->pos_y & 0xF) == 0) { *map_update_flag = true; }
				camera->pos_y++;
			case 4:

				if ((camera->pos_y & 0xF) == 0) { *map_update_flag = true; }
				camera->pos_y++;
			case 3:

				if ((camera->pos_y & 0xF) == 0) { *map_update_flag = true; }
				camera->pos_y++;
			case 2:

				if ((camera->pos_y & 0xF) == 0) { *map_update_flag = true; }
				camera->pos_y++;
			case 1:

				if ((camera->pos_y & 0xF) == 0) { *map_update_flag = true; }
				camera->pos_y++;
			default:
				break;
		}

		//camera->pos_y += speed_y;
	}
	if ((displacement_y < 0) && (camera->pos_y > 0)) {
		switch (speed_y) {
			case 5:

				if ((camera->pos_y & 0xF) == 0) { *map_update_flag = true; }
				camera->pos_y--;
			case 4:

				if ((camera->pos_y & 0xF) == 0) { *map_update_flag = true; }
				camera->pos_y--;
			case 3:

				if ((camera->pos_y & 0xF) == 0) { *map_update_flag = true; }
				camera->pos_y--;
			case 2:

				if ((camera->pos_y & 0xF) == 0) { *map_update_flag = true; }
				camera->pos_y--;
			case 1:

				if ((camera->pos_y & 0xF) == 0) { *map_update_flag = true; }
				camera->pos_y--;
			default:
				break;
		}
		//camera->pos_y -= speed_y;
	}

	camera->speed_index++;

	// Calculate vertical and horizontal breakpoint
	camera->pos_abs_x = camera->pos_x % video->map_buffer_width;
	camera->pos_abs_y = camera->pos_y % video->map_buffer_height;
}
