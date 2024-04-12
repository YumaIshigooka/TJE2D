#include "gameStage_0.h"
#include "math.h"
#include <math.h>
#include "game.h"

float Stage::gravity = 800;
float Stage::jump = 120;
float Stage::x_vel = 60;
float Stage::bullet_vel = 180;

sEntity box_breakable;
sEntity key_tobreak;
double box_break_time;

void addHitbox(std::vector<hitBox*> *l, float tr_x, float tr_y, float bl_x, float bl_y) {
	hitBox* h = new hitBox(new Vector2(tr_x, tr_y), new Vector2(bl_x, bl_y));
	l->push_back(h);
	return;
}

gameStage_0::gameStage_0() {
	ground_hitboxes = new std::vector<hitBox*>;
	platform_hitboxes = new std::vector<hitBox*>;
	obstacle_hitboxes = new std::vector<hitBox*>;
	other_hitboxes = new std::vector<hitBox*>;

	addHitbox(ground_hitboxes, 15*8,	28*8,	0*8,	30*8);
	addHitbox(ground_hitboxes, 26*8,	27*8,	14*8,	30*8);
	addHitbox(ground_hitboxes, 3*8,		0*8,	0*8,	26*8);
	addHitbox(ground_hitboxes, 26*8,	26*8,	25*8,	27*8);
	addHitbox(ground_hitboxes, 106*8,	29*8,	86*8,	30*8);
	addHitbox(ground_hitboxes, 105*8,	23*8,	104*8,	26*8);
	addHitbox(ground_hitboxes, 160*8,	28*8,	129*8,	29*8);

	addHitbox(platform_hitboxes, 25 * 8,	24 * 8,		16 * 8,		25 * 8);
	addHitbox(platform_hitboxes, 32 * 8,	22 * 8,		26 * 8,		23 * 8);
	addHitbox(platform_hitboxes, 27 * 8,	18 * 8,		23 * 8,		19 * 8);
	addHitbox(platform_hitboxes, 89 * 8,	26 * 8,		37 * 8,		27 * 8);
	addHitbox(platform_hitboxes, 105 * 8,	23 * 8,		95 * 8,		24 * 8);
	addHitbox(platform_hitboxes, 124 * 8,	26 * 8,		105 * 8,	27 * 8);

	addHitbox(obstacle_hitboxes, 52 * 8,	28 * 8,		26 * 8,		30 * 8);
	addHitbox(obstacle_hitboxes, 300 * 8,	28 * 8,		101 * 8, 	30 * 8);


	box_breakable.coords = { 129 * 8, 24 * 8 };
	box_breakable.hitbox->t_r = { 129 * 8 + 13, 19 * 8 };
	box_breakable.hitbox->b_l = { 129 * 8, 24 * 8 };
	ground_hitboxes->push_back(box_breakable.hitbox);


	key_tobreak.coords = { 126 * 8, 27 * 8 };
	key_tobreak.hitbox->t_r = { 127 * 8, 27 * 8 - 4 };
	key_tobreak.hitbox->b_l = { 126 * 8, 28 * 8 };
	key_tobreak.hitbox->type = hitBox::KEY_0;
	other_hitboxes->push_back(key_tobreak.hitbox);


	player.size = { 8, 8 };
	player.hitbox->b_l = { player.coords.x + player.size.x / 3, player.coords.y + player.size.y };
	player.hitbox->t_r = { player.coords.x + player.size.x * 2 / 3, player.coords.y + player.size.y * 5 / 6 };
	
	map = GameMap::loadGameMap("data/tutorial.json");

	cb.cam_should = { 160 / 2 - player.size.x / 2, 120 / 2 - player.size.y / 2 };
	cb.cam_should -= player.coords;
	cb.cam_offset = cb.cam_should;
	cb.player_cam = { 0,0 };



	checkpoint = { 80, 216 };
}

void gameStage_0::onEnter() {
	skip = 0;
	idx = 0;
	idx_should = 0;
	idx_lowest = 1;
	reverting = false;
}

void gameStage_0::onLeave() {
	if (paused) paused = !paused;
}

//gameStage_0::~gameStage_0() {
//	for(hitBox h : )
//}

void gameStage_0::render(Image& fb) {
	// FrameBuffer
	//Create a new Image (or we could create a global one if we want to keep the previous frame)
	//do not change framebuffer size
	int cs = 8;
	int numtiles = 18;
	Vector2 player_print_coords = { fb.width / 2 - player.size.x / 2 + cb.player_cam.x , fb.height / 2 - player.size.y / 2 + cb.player_cam.y };

	if (paused) {
		GameMap::drawTileset(map, cs, numtiles, Game::instance->tileset, fb, cb.cam_offset, Game::instance->time);
		fb.drawImage(Game::instance->sprite_mini,
			player_print_coords.x,
			player_print_coords.y,
			Area(player.moving * player.size.x * (int(Game::instance->time * 8) % 6),
				player.direction * player.size.y,
				player.size.x,
				player.size.y));



		fb.drawImage(Game::instance->pause_screen, 0, 0);
		fb.drawImage(Game::instance->select_arrows, 22, 20 + 31 * pause_option, Area(17 * (int(Game::instance->time * 3 / 2) % 2), pause_option * 17, 17, 17));
	}
	else {
		fb.fill(Game::instance->bgcolor);	//fills the image with one color

		// Size in pixels of a cell

		GameMap::drawTileset(map, cs, numtiles, Game::instance->tileset, fb, cb.cam_offset, Game::instance->time);

		GameMap::drawAsset(fb, Game::instance->tutorial_help1, Vector2(3 * 8, 16 * 8 - 4), cb.cam_offset, player.coords);
		GameMap::drawAsset(fb, Game::instance->keyset, Area(17 * (int(Game::instance->time * 1.5) % 2), 34, 17, 17), Vector2(10 * 8 + 4, 21 * 8 - 3), cb.cam_offset, player.coords);
		GameMap::drawAsset(fb, Game::instance->keyset, Area(17 * (int(Game::instance->time * 1.5 + 0.33) % 2), 4 * 17, 17, 17), Vector2(8 * 8 + 2, 19 * 8 - 5), cb.cam_offset, player.coords);
		GameMap::drawAsset(fb, Game::instance->keyset, Area(17 * (int(Game::instance->time * 1.5 + 0.69) % 2), 3 * 17, 17, 17), Vector2(6 * 8, 21 * 8 - 3), cb.cam_offset, player.coords);
		GameMap::drawAsset(fb, Game::instance->keyset, Area(0, 5 * 17, 17, 17), Vector2(8 * 8 + 2, 21 * 8 - 3), cb.cam_offset, player.coords);


		GameMap::drawAsset(fb, Game::instance->tutorial_help2, Vector2(40 * 8, 16 * 8 - 4), cb.cam_offset, player.coords);
		if (Game::instance->time - tuto2_timer < 0.66) {
			GameMap::drawAsset(fb, Game::instance->keyset, Area(17 * (int((Game::instance->time - tuto2_timer) * 6) % 2), 34, 17, 17), Vector2(47 * 8 + 8, 21 * 8 - 3), cb.cam_offset, player.coords);
		}
		else if ((Game::instance->time - tuto2_timer < 1.5)) {
			GameMap::drawAsset(fb, Game::instance->keyset, Area(0, 34, 17, 17), Vector2(47 * 8 + 8, 21 * 8 - 3), cb.cam_offset, player.coords);
		}
		else {
			GameMap::drawAsset(fb, Game::instance->keyset, Area(0, 34, 17, 17), Vector2(47 * 8 + 8, 21 * 8 - 3), cb.cam_offset, player.coords);
			tuto2_timer = Game::instance->time;
		}

		GameMap::drawAsset(fb, Game::instance->keyset, Area(0, 4 * 17, 17, 17), Vector2(45 * 8 + 6, 19 * 8 - 5), cb.cam_offset, player.coords);
		GameMap::drawAsset(fb, Game::instance->keyset, Area(0, 3 * 17, 17, 17), Vector2(43 * 8 + 4, 21 * 8 - 3), cb.cam_offset, player.coords);
		GameMap::drawAsset(fb, Game::instance->keyset, Area(0, 5 * 17, 17, 17), Vector2(45 * 8 + 6, 21 * 8 - 3), cb.cam_offset, player.coords);

		GameMap::drawAsset(fb, Game::instance->tutorial_help3, Vector2(80 * 8, 16 * 8 - 4), cb.cam_offset, player.coords);

		GameMap::drawAsset(fb, Game::instance->tutorial_help4, Vector2(112 * 8, 16 * 8 - 4), cb.cam_offset, player.coords);
		GameMap::drawAsset(fb, Game::instance->keyset, Area(17 * (int(Game::instance->time * 2) % 2), 0, 17, 17), Vector2(119 * 8, 16 * 8), cb.cam_offset, player.coords);



		fb.drawText("FPS: " + toString(int(fps)), 125, 1, Game::instance->minifont, 4, 6);

		if (!player.teleporting) {
			fb.drawImage(Game::instance->sprite_mini,
				player_print_coords.x,
				player_print_coords.y,
				Area(player.moving * player.size.x * (int(Game::instance->time * 8) % 6),
					player.direction * player.size.y,
					player.size.x,
					player.size.y));	//draws only a part of an image
			if (Game::instance->time - player.last_ground < 0.5) {
				fb.drawImage(Game::instance->grounding,
					player.last_ground_pos.x - 12 + cb.cam_offset.x,
					player.last_ground_pos.y - 12 + cb.cam_offset.y,
					Area(32 * ((int((Game::instance->time - player.last_ground) * 8) % 4)),
						0,
						32,
						32));	//draws only a part of an image
			}
		}
		else if (Game::instance->time - player.last_death < 0.87) {
			fb.drawImage(Game::instance->explosion,
				player_print_coords.x - 12,
				player_print_coords.y - 12,
				Area(32 * ((int((Game::instance->time - player.last_death) * 8) % 8)),
					7 * 32,
					32,
					32));	//draws only a part of an image
		}
		else {
			Vector2 what = Vector2(player.velocity.x, player.velocity.y).normalize_unit(400) / 30;
			std::cout << what.x << " " << what.y << "\n";
			fb.drawLine(player_print_coords.x + 4,
				player_print_coords.y + 4,
				player_print_coords.x - what.x + 4,
				player_print_coords.y - what.y + 4,
				Color::RANDOM());
		}
		fb.drawImage(Game::instance->clock_sprite, 148, 71, Area(9 * (int(Game::instance->time * 25) % 16), 0, 9, 11));

		fb.drawRectangle(150, 84, 5, 31, Color::GRAY);
		fb.drawRectangle(151, 85, 3, 30 * (idx_should - idx_lowest) / 1000, Color::CYAN);
		fb.drawRectangle(143, 110, 5, 5, Color::GRAY);
		fb.drawRectangle(144, 111, 3, 6 * Game::instance->norm(0.5, Game::instance->time - last_fired), Color::RED);

		// Draw region of tileset inside framebuffer
		// fb.drawImage(tileset, screenx, screeny, area);
		for (sEntity bullet : bullets) {
			int screenx = bullet.coords.x + cb.cam_offset.x;
			int screeny = bullet.coords.y + cb.cam_offset.y;
			fb.drawLine(screenx, screeny, screenx + 3, screeny, Color::RED);
		}


		fb.drawText("BoxBreak: " + toString(box_breakable.active) + " | KeyThing: " + toString(key_tobreak.active), 2, 2, Game::instance->minifont, 4, 6);
		fb.drawText("TR: " + toString(box_breakable.hitbox->t_r.x) + ":" + toString(box_breakable.hitbox->t_r.y) +
			" | BL: " + toString(box_breakable.hitbox->b_l.x) + ":" + toString(box_breakable.hitbox->b_l.y), 2, 9, Game::instance->minifont, 4, 6);

		if (box_breakable.active) {
			fb.drawImage(Game::instance->simple_box, box_breakable.coords.x + cb.cam_offset.x, box_breakable.coords.y + cb.cam_offset.y - 40, Area(0, 0, 13, 40));
			fb.drawImage(Game::instance->opening_keys, box_breakable.coords.x + cb.cam_offset.x + 3, box_breakable.coords.y + cb.cam_offset.y - 22 + int(Game::instance->time * 2) % 2, Area(0, 0, 7, 4));
		}
		else if (Game::instance->time - box_break_time < 0.8) {
			fb.drawImage(Game::instance->simple_box, box_breakable.coords.x + cb.cam_offset.x, box_breakable.coords.y + cb.cam_offset.y - 40, Area(13 * (int(8 * (Game::instance->time - box_break_time)) % 8), 0, 13, 40));
		}
		else {
			fb.drawImage(Game::instance->simple_box, box_breakable.coords.x + cb.cam_offset.x, box_breakable.coords.y + cb.cam_offset.y - 40, Area(13 * 7, 0, 13, 40));
		}
		if (key_tobreak.active) {
			fb.drawImage(Game::instance->opening_keys, key_tobreak.coords.x + cb.cam_offset.x, key_tobreak.coords.y + cb.cam_offset.y + int(Game::instance->time * 2) % 2, Area(0, 0, 7, 4));
		}


		// DRAW HITBOXES
		//float player_print_hitbox[4] = { player_print_coords.x + player.size.x / 3, player_print_coords.y + player.size.y,
		//							player_print_coords.x + player.size.x * 2 / 3, player_print_coords.y + player.size.y * 5 / 6 };
		//fb.drawLine(player_print_hitbox[0], player_print_hitbox[1] - 1, player_print_hitbox[2], player_print_hitbox[3] - 1, Color::RED);
		//fb.drawLine(player_print_hitbox[2], player_print_hitbox[1] - 1, player_print_hitbox[0], player_print_hitbox[3] - 1, Color::RED);


		if (reverting) {
			fb.drawImage(Game::instance->revert_overlay, 0, 0);
			fb.drawText(toString((float)(idx_should - idx_lowest) / fps), 30, 51, Game::instance->bigfont, 14, 18);
		}
	}
}


void gameStage_0::update(double seconds_elapsed)
{
	// std::cout << player.hitbox.collided(ground_hitboxes[0]);
	if (paused) {
		if (Input::wasKeyPressed(SDL_SCANCODE_UP)) {
			if (--pause_option < pauses::CONTINUE_GAME) pause_option = pauses::QUIT_GAME;
			synth.playSample("data/sfx/select.wav", 0.6, false);
		}
		else if (Input::wasKeyPressed(SDL_SCANCODE_DOWN)) {
			if (++pause_option > pauses::QUIT_GAME) pause_option = pauses::CONTINUE_GAME;
			synth.playSample("data/sfx/select.wav", 0.6, false);
		}
		else if ((Input::wasKeyPressed(SDL_SCANCODE_N))) {
			switch (pause_option) {
			case pauses::CONTINUE_GAME: paused = !paused; break;
			case pauses::BACK_MENU:  Game::instance->switch_stage(Game::stages::MENU); break; // TO DO: go back to menu
			case pauses::QUIT_GAME:	Game::instance->must_exit = true; break;
			}
			synth.playSample("data/sfx/select.wav", 0.6, false);
		}
	}
	else {
		if (player.coords.x > 100 * 8) checkpoint = Vector2(106 * 8, 24 * 8);

		skip = (skip + 1) % 3;

		if (skip && !reverting) {
			idx = (idx + 1) % 1000;
			idx_should++;
			if (idx_should - idx_lowest > 999) idx_lowest++;
			revert[idx].player.copy(&player);
			revert[idx].camborders.copy(&cb);
		}

		if (Input::isKeyPressed(SDL_SCANCODE_M) && idx_should > idx_lowest) //if key right
		{
			--idx;
			if (idx == -1) idx = 999;
			--idx_should;
			player.copy(&revert[idx].player);
			cb.copy(&revert[idx].camborders);
		}
		else {
			reverting = false;
		}

		// std::cout << "idx:" << idx << "|" << int(revert[idx].player.coords.x) << ":" << int(revert[idx].player.coords.y) << "\t";

		// Print the saving or loading text on the screen
		if (save_state != -1) {
			if ((Game::instance->time - status_change_time) > 2) save_state = sGameInfo::DEFAULT_NO_SAVESTATE_CHANGE;
			else if ((Game::instance->time - status_change_time) > 0.05) show = true;
		}

		// compute frames per second
		fps = 1 / seconds_elapsed;

		player.dv.y = 1;
		if (!reverting && !player.teleporting) {
			//Read the keyboard state, to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
			if (Input::wasKeyPressed(SDL_SCANCODE_UP)) {
				if ((Game::instance->time - coyote) < 0.15) {
					coyote = -5;
					player.velocity.y = -jump;
					player.grounded = false;
					player.last_jumptime = Game::instance->time;
					jump_sample = synth.playSample("data/sfx/jump.wav", 0.6, false);
				}
			}
			if (Input::isKeyPressed(SDL_SCANCODE_UP)) {
				if (Game::instance->time - player.last_jumptime < 0.2) {
					player.velocity.y -= gravity * seconds_elapsed;
				}
			}

			if (Input::wasKeyPressed(SDL_SCANCODE_LEFT))
			{
				if (player.direction == player.LEFT && (Game::instance->time - running) < 0.3)
					x_vel = 100;
				else x_vel = 60;
				running = Game::instance->time;
			}
			if (Input::wasKeyPressed(SDL_SCANCODE_RIGHT))
			{
				if (player.direction == player.RIGHT && (Game::instance->time - running) < 0.3)
					x_vel = 100;
				else x_vel = 60;
				running = Game::instance->time;
			}
			if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) //if key left
			{
				player.dv.x -= 1;
				player.direction = player.LEFT;
				player.moving = 1;
				player.velocity.x = x_vel;
			}
			if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) //if key right
			{
				player.dv.x += 1;
				player.direction = player.RIGHT;
				player.moving = 1;
				player.velocity.x = x_vel;
			}

			if (Input::wasKeyPressed(SDL_SCANCODE_N)) {
				if (Game::instance->time - last_fired > 0.5) {
					for (int i = 0; i < 5; i++) {
						if (!bullet_fired[i]) {
							last_fired = Game::instance->time;
							bullet_fired[i] = true;
							bullets[i].coords = player.coords + Vector2(0, 4);
							synth.playSample("data/sfx/shoot.wav", 0.6, false);
							if (player.direction == player.RIGHT) bullets[i].velocity = { bullet_vel, 0 };
							else bullets[i].velocity = { -bullet_vel, 0 };
							break;
						}
					}
				}
			}

			// Finish Reading Inputs

			for (int i = 0; i < N_BULLETS; i++) {
				if (bullet_fired[i]) {
					bullets[i].coords += bullets[i].velocity * seconds_elapsed;
					bullets[i].hitbox->t_r = { bullets[i].coords.x + 2, bullets[i].coords.y };
					bullets[i].hitbox->b_l = { bullets[i].coords.x, bullets[i].coords.y + 2 };
					for (hitBox* h : *ground_hitboxes) {
						if (bullets[i].hitbox->collided(h) || abs(bullets[i].coords.x - player.coords.x) > fb_size[0]) {
							bullets[i].coords = { -1, -1 };
							bullet_fired[i] = false;
							break;
						}
					}
				}
			}



			// Compute new coords
			Vector2 new_coords = player.coords;
			if (abs(player.dv.x) > 1) player.dv.x /= abs(player.dv.x);
			Vector2 movement = { player.dv.x * player.velocity.x * (float)seconds_elapsed, player.velocity.y * (float)seconds_elapsed };
			new_coords += movement;


			// Velocity Handling
			if (player.velocity.x > 0.005) player.velocity.x -= 200 * seconds_elapsed;
			else if (player.velocity.x != 0) {
				player.velocity.x = 0;
				player.dv.x = 0;
			};
			if (!player.grounded) {
				if (player.velocity.y < 180) {
					player.velocity.y += gravity * seconds_elapsed;
				}
			}
			else {
				if (player.velocity.y > 0) player.velocity.y = 0;
			}




			player.hitbox->b_l = { new_coords.x , new_coords.y + player.size.y - 1 };
			player.hitbox->t_r = { new_coords.x + player.size.x * 3 / 4, new_coords.y + player.size.y * 1 / 8 - 1 };

			if (player.hitbox->touching(key_tobreak.hitbox) && key_tobreak.active) {
				key_tobreak.active = false;
				box_breakable.active = false;
				box_break_time = Game::instance->time;
				box_breakable.hitbox->t_r.y += 39;
			}

			int hit_type = hitBox::NO_HIT;


			if (player.grounded || player.velocity.y > 0) {
				for (hitBox* h : *ground_hitboxes) {
					if (player.hitbox->collided_status(h) == hitBox::RIGHT_HIT || player.hitbox->collided_status(h) == hitBox::LEFT_HIT) {
						hit_type = hitBox::LEFT_HIT;
					}
					else if (!player.grounded && player.hitbox->collided_status(h) == hitBox::DOWN_HIT) {
						currentground = *h;
						break;
					}
				}
			}
			else {
				for (hitBox* h : *ground_hitboxes) {
					if (player.hitbox->collided_status_sides(h) == hitBox::RIGHT_HIT || player.hitbox->collided_status_sides(h) == hitBox::LEFT_HIT) {
						hit_type = hitBox::LEFT_HIT;
					}
					else if (!player.grounded && player.hitbox->collided_status_sides(h) == hitBox::DOWN_HIT) {
						currentground = *h;
						break;
					}
				}
			}

			for (hitBox* h : *platform_hitboxes) {
				if (player.hitbox->collided_status(h) == hitBox::DOWN_HIT) {
					currentground = *h;
					break;
				}
			}





			if (!currentground.collided(player.hitbox) || player.velocity.y < 0) {
				if (hit_type == hitBox::LEFT_HIT) {
					player.coords.y = new_coords.y;
					cb.player_cam.y += movement.y;
				}
				else if (true) {
					player.coords = new_coords;
					cb.player_cam += movement;
				}
			}


			if (player.hitbox->touching(&currentground)) {
				player.grounded = true;
				coyote = Game::instance->time;
			}
			else {
				player.grounded = false;
				currentground.b_l = { -1,-1 };
				currentground.t_r = { -1,-1 };
			}



			for (hitBox* h : *obstacle_hitboxes) {
				if (player.hitbox->touching(h) || (Input::wasKeyPressed(SDL_SCANCODE_S) && !player.teleporting)) {
					player.teleport(Vector2(checkpoint.x, checkpoint.y), seconds_elapsed, cb, Game::instance->time, synth);
					player.last_death = Game::instance->time;
					synth.playSample("data/sfx/rip.wav", 0.6, false);
				}
			}



		}
		else {
			if (player.teleporting) {
				idx_lowest = idx_should;
				if ((Game::instance->time - player.last_death) > .9) {
					player.teleport(Vector2(checkpoint.x, checkpoint.y), seconds_elapsed, cb, Game::instance->time, synth);
				}
				if (!player.teleporting && !box_breakable.active) {
					key_tobreak.active = true;
					box_breakable.active = true;
					box_breakable.hitbox->t_r.y -= 39;
				}
			}
		}

		// Update camera offset
		uint8 xcamlimit[2] = { 15, 10 };
		uint8 ycamlimit[2] = { 20, 10 };

		camBorders::box_camera(cb.player_cam.x, cb.cam_should.x, cb.cam_offset.x, cb.x_limit, xcamlimit, map->width * 8, player.size.x, player.coords.x, fb_size[0]);
		camBorders::box_camera(cb.player_cam.y, cb.cam_should.y, cb.cam_offset.y, cb.y_limit, ycamlimit, map->height * 8, player.size.y, player.coords.y, fb_size[1]);



		if (Input::wasKeyPressed(SDL_SCANCODE_M)) {
			synth.playSample("data/sfx/reverse.wav");
			reverting = true;
		}
	}
}

//Keyboard event handler (sync input)
void gameStage_0::onKeyDown(SDL_KeyboardEvent event)
{
	switch (event.keysym.sym) {
	case SDLK_ESCAPE:
		if (!paused) paused = !paused;
		pause_option = pauses::CONTINUE_GAME;
		break;
	case SDLK_k: sGameInfo::saveGameInfo(player, Game::instance->time, save_state, show, status_change_time); break;
	case SDLK_l: sGameInfo::loadGameInfo(player, Game::instance->time, save_state, show, status_change_time); break;
	}
}

void gameStage_0::onKeyUp(SDL_KeyboardEvent event)
{
	if (event.keysym.sym == SDLK_DOWN ||
		event.keysym.sym == SDLK_RIGHT ||
		event.keysym.sym == SDLK_LEFT) player.moving = 0;
	if (event.keysym.sym == SDLK_a) {
		player.moving = 0;
		coyote = Game::instance->time;
	}
	if (event.keysym.sym == SDLK_UP) {
		player.moving = 0;
		jump_sample->stop();
	}
	if (event.keysym.sym == SDLK_m) {
		idx_lowest = idx_should;
	}
}


void gameStage_0::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void gameStage_0::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void gameStage_0::onMouseMove(SDL_MouseMotionEvent event)
{
}

void gameStage_0::onMouseButtonDown(SDL_MouseButtonEvent event)
{
}

void gameStage_0::onMouseButtonUp(SDL_MouseButtonEvent event)
{
}

void gameStage_0::onMouseWheel(SDL_MouseWheelEvent event)
{
}
