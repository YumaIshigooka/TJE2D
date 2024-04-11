#include "camBorders.h"
#include "math.h"
#include <math.h>

camBorders::camBorders() {
	this->player_cam = { 0,0 };
	this->cam_offset = { 0,0 };
	this->cam_should = { 0,0 };
	this->x_limit = false;
	this->y_limit = false;
}

void camBorders::copy(camBorders* b2) {
	this->player_cam.x = b2->player_cam.x; this->player_cam.y = b2->player_cam.y;
	this->cam_offset.x = b2->cam_offset.x; this->cam_offset.y = b2->cam_offset.y;
	this->cam_should.x = b2->cam_should.x; this->cam_should.y = b2->cam_should.y;
	this->x_limit = b2->x_limit; this->y_limit = b2->y_limit;
}

void camBorders::box_camera(float& player_cam, float& cam_should, float& cam_offset, bool& border_limit, uint8 cam_limits[2], int length, float player_size, float player_coords, int frame_size) {
	if (abs(player_cam) >= cam_limits[0]) {
		cam_should = frame_size / 2 - player_size / 2;
		cam_should -= player_coords;
		if (player_cam >= cam_limits[0]) {
			// not in the borders
			cam_should -= !border_limit * (player_cam - 2 * cam_limits[0]);
			// near borders
			cam_should += border_limit * cam_limits[0];

			player_cam = cam_limits[0];
		}
		else if (player_cam <= -cam_limits[0]) {
			// not in the borders
			cam_should -= !border_limit * (player_cam + 2 * cam_limits[0]);
			// near borders
			cam_should -= border_limit * cam_limits[0];

			player_cam = -cam_limits[0];
		}

		if (cam_should >= 0) {
			player_cam = -cam_should - cam_limits[0];
			cam_offset = 0;
			border_limit = true;
		}
		else if (cam_should <= -length + frame_size) {
			player_cam = -cam_should + cam_limits[0] - length + frame_size;
			cam_offset = -length + frame_size;
			border_limit = true;
		}
		else {
			cam_offset = cam_should;
			border_limit = false;
		}
	}
}