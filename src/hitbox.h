#ifndef HITBOX_H
#define HITBOX_H

#include "includes.h"
#include "utils.h"
class sEntity;

class hitBox {
public:
	Vector2 t_r; // top right
	Vector2 b_l; // bottom left


	enum type_HB {
		NONE = -1,
		KEY_0,
		BOX_WOOD,
		BOX_METAL,
		END_GOAL
	};
	type_HB type;
	enum coltype{
		NO_HIT,
		UP_HIT,
		DOWN_HIT,
		LEFT_HIT,
		RIGHT_HIT,
		INSIDE
	};

	sEntity *father;
	hitBox* current_ground;

	hitBox(Vector2* t_r, Vector2* b_l);
	hitBox(Vector2* t_r, Vector2* b_l, type_HB type);
	hitBox() ;

	int collided_status(hitBox* hb2);
	int collided_status_sides(hitBox* hb2);
	int collided_status_up(hitBox* hb2);
	bool collided(hitBox* hb2);
	bool touching(hitBox* hb2);
	void copy(hitBox* h2);
};

#endif