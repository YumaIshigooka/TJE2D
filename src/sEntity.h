#ifndef SENTITY_H
#define SENTITY_H

#include "includes.h"
#include "utils.h"
#include "hitbox.h"


class sEntity {
public:
	Vector2 size = { 0.0f, 0.0f };
	Vector2 coords;
	Vector2 dv;
	Vector2 velocity;
	bool active;
	enum {
		RIGHT,
		LEFT,
		DOWN,
		UP
	};
	int direction;
	bool moving;
	bool grounded;
	hitBox* hitbox;

	sEntity();

	void copy(sEntity* e2);
	int collided(sEntity* e2);
};

#endif