#ifndef HITBOX_H
#define HITBOX_H

#include "includes.h"
#include "utils.h"

class hitBox {
public:
	Vector2 t_r; // top right
	Vector2 b_l; // bottom left
	enum type_HB {
		GROUND,
		PLATFORM,
		OBSTACLE,
		INTERACTIBLE,
		REUSABLE,
	};
	type_HB type;
	enum {
		NO_HIT,
		UP_HIT,
		DOWN_HIT,
		LEFT_HIT,
		RIGHT_HIT,
		INSIDE
	};
	hitBox(Vector2* t_r, Vector2* b_l);
	hitBox(Vector2* t_r, Vector2* b_l, type_HB type);
	hitBox() ;

	int collided_status(hitBox* hb2);
	int collided_status_sides(hitBox* hb2);
	bool collided(hitBox* hb2);
	bool touching(hitBox* hb2);
	void copy(hitBox* h2);
};

#endif