#ifndef CAMBORDERS_H
#define CAMBORDERS_H

#include "includes.h"
#include "utils.h"


class camBorders {
public:
	Vector2 player_cam;
	Vector2 cam_offset;
	Vector2 cam_should;
	bool x_limit;
	bool y_limit;

	camBorders();

	static void box_camera(float& player_cam, float& cam_should, float& cam_offset, bool& border_limit, uint8 cam_limits[2], int length, float player_size, float player_coords, int frame_size);
	void copy(camBorders* b2);

};
#endif