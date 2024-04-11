#include "player.h"
#include "math.h"
#include "game.h"
#include <math.h>

Player::Player() {
	this->size = { 0.0f, 0.0f };
	this->coords = { 80, 20 * 8 };
	this->dv = { 0.0f,0.0f };
	this->velocity = { 0.0f, -1.0f };;
	this->direction = LEFT;
	this->moving = false;
	this->grounded = false;
	this->last_jumptime = -10;
	this->active = true;
	this->teleporting = false;
	this->teleportdistance = 0;
	this->last_death = -5;
}

void Player::copy(Player* player_2) {
	this->size.x = player_2->size.x; size.y = player_2->size.y;
	this->coords.x = player_2->coords.x; coords.y = player_2->coords.y;
	this->velocity.x = -player_2->velocity.x; velocity.y = -player_2->velocity.y;
	this->dv.x = player_2->dv.x; this->dv.y = player_2->dv.y;
	this->direction = player_2->direction;
	this->moving = player_2->moving;
	this->hitbox.b_l.x = player_2->hitbox.b_l.x; hitbox.b_l.y = player_2->hitbox.b_l.y;
	this->hitbox.t_r.x = player_2->hitbox.t_r.x; hitbox.t_r.y = player_2->hitbox.t_r.y;
	this->grounded = player_2->grounded;
	this->last_jumptime = player_2->last_jumptime;
	this->active = player_2->active;
	this->last_death = player_2->last_death;
	this->teleportdistance = player_2->teleportdistance;
	this->teleporting = player_2->teleporting;
}

void Player::teleport(Vector2 new_coords, double elapsed_time, camBorders& cb, double time, Synth& synth) {
	if (!this->teleporting) {
		this->teleporting = true;
		if (this->teleportdistance == 0) {
			this->teleportdistance = abs(this->coords.x - new_coords.x) + abs(this->coords.y - new_coords.y);
			this->velocity.x = new_coords.x - this->coords.x;
			this->velocity.y = new_coords.y - this->coords.y;
		}
	}
	else if (this->teleporting) {
		if (abs(this->coords.x - new_coords.x) + abs(this->coords.y - new_coords.y) < sqrt(this->teleportdistance) * 3) {
			this->velocity -= this->velocity * sqrt(this->teleportdistance) * (0.3) * elapsed_time;
		}
		//this->velocity.x = new_coords.x - this->coords.x;
		//this->velocity.y = new_coords.y - this->coords.y;
		this->coords += this->velocity * elapsed_time;
		cb.player_cam += this->velocity * elapsed_time;
		if (abs(this->coords.x - new_coords.x) < 4 && abs(this->coords.y - new_coords.y) < 4) {
			this->teleportdistance = 0;
			this->teleporting = false;
			this->last_ground = time;
			this->last_ground_pos.x = this->coords.x; this->last_ground_pos.y = this->coords.y;
			synth.playSample("data/sfx/respawn.wav", 0.6, false);
		}
	}
}