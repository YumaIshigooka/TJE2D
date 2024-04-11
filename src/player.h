#ifndef PLAYER_H
#define PLAYER_H

#include "includes.h"
#include "utils.h"
#include "sEntity.h"
#include "synth.h"
#include "camBorders.h"


class Player : public sEntity {
public:
	double last_jumptime;
	double last_death;
	double last_ground;
	Vector2 last_ground_pos;
	bool teleporting = false;
	float teleportdistance = 0;


	Player();

	void copy(Player* player_2);

	void teleport(Vector2 new_coords, double elapsed_time, camBorders& cb, double time, Synth& synth);
};
#endif