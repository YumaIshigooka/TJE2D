#include "hitBox.h"
#include "math.h"
#include <math.h>

hitBox::hitBox(Vector2* t_r, Vector2* b_l) {
	this->t_r = *t_r;
	this->b_l = *b_l;
	this->type = PLATFORM;
}

hitBox::hitBox(Vector2* t_r, Vector2* b_l, type_HB type) {
	this->t_r = *t_r;
	this->b_l = *b_l;
	this->type = type;
}

hitBox::hitBox() {}

int hitBox::collided_status(hitBox* hb2) {
	if (this->collided(hb2)) {
		if (this->t_r.x + 0 < hb2->t_r.x && this->t_r.y - 0 > hb2->t_r.y && this->b_l.x - 0 > hb2->b_l.x && this->b_l.y + 0 < hb2->b_l.y) return INSIDE;
		else if (this->t_r.y < hb2->t_r.y) return DOWN_HIT;
		else if (this->t_r.x > hb2->t_r.x) return LEFT_HIT;
		else if (this->b_l.x < hb2->b_l.x) return RIGHT_HIT;
		else return UP_HIT; // this->b_l.y > hb2->b_l.y
	}
	return NO_HIT;
}
int hitBox::collided_status_sides(hitBox* hb2) {
	if (this->collided(hb2)) {
		if (this->t_r.x + 0 < hb2->t_r.x && this->t_r.y - 0 > hb2->t_r.y && this->b_l.x - 0 > hb2->b_l.x && this->b_l.y + 0 < hb2->b_l.y) return INSIDE;
		else if (this->t_r.x > hb2->t_r.x) return LEFT_HIT;
		else if (this->b_l.x < hb2->b_l.x) return RIGHT_HIT;
		else if (this->t_r.y < hb2->t_r.y) return DOWN_HIT;
		else return UP_HIT; // this->b_l.y > hb2->b_l.y
	}
	return NO_HIT;
}
bool hitBox::collided(hitBox* hb2) {
	return (this->t_r.x > hb2->b_l.x && hb2->t_r.x > this->b_l.x && this->t_r.y < hb2->b_l.y && hb2->t_r.y < this->b_l.y);
}
bool hitBox::touching(hitBox* hb2) {
	return (this->t_r.x + 1 > hb2->b_l.x && hb2->t_r.x > this->b_l.x - 1 && this->t_r.y - 1 < hb2->b_l.y && hb2->t_r.y < this->b_l.y + 1);
}
void hitBox::copy(hitBox* h2) {
	this->t_r = h2->t_r;
	this->b_l = h2->b_l;
}