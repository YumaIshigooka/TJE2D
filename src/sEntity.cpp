#include "sEntity.h"
#include "math.h"
#include <math.h>

sEntity::sEntity() {
	this->coords = { -100.0f, -60.0f };
	this->dv = { 0.0f,0.0f };
	this->velocity = { 0.0f, 0.0f };;
	this->direction = LEFT;
	this->moving = false;
	this->grounded = false;
	this->active = true;
	this->hitbox = new hitBox();
}

void sEntity::copy(sEntity* e2) {
	this->coords.x = e2->coords.x; coords.y = e2->coords.y;
	this->velocity.x = -e2->velocity.x; velocity.y = -e2->velocity.y;
	this->dv.x = e2->dv.x; this->dv.y = e2->dv.y;
	this->direction = e2->direction;
	this->moving = e2->moving;
	this->hitbox->b_l.x = e2->hitbox->b_l.x; this->hitbox->b_l.y = e2->hitbox->b_l.y;
	this->hitbox->t_r.x = e2->hitbox->t_r.x; this->hitbox->t_r.y = e2->hitbox->t_r.y;
	this->grounded = e2->grounded;
	this->active = e2->active;
}

int sEntity::collided(sEntity* e2) {
	return this->hitbox->collided_status(e2->hitbox);
}