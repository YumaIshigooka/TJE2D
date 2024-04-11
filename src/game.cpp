#include "game.h"
#include "utils.h"
#include "input.h"
#include "image.h"
#include "hitbox.h"
#include "sEntity.h"
#include "player.h"
#include "camBorders.h"
#include "gameMap.h"

#include "json.hpp"

#include <fstream>
#include <cmath>

const int N_BULLETS = 5;

Synth::SamplePlayback* jump_sample;

Game* Game::instance = NULL;

GameMap* map;

// Fonts
Image font;
Image minifont;
Image bigfont;

// Sprite/Tile Sets
Image sprite;
Image explosion;
Image sprite_mini;
Image tileset;
Image keyset;
Image select_arrows;
Image simple_box;
Image grounding;
Image opening_keys;
Image clock_sprite;

Image tutorial_help1;
Image tutorial_help2;
Image tutorial_help3;
Image tutorial_help4;

// Other
Image title_screen;
Image pause_screen;
Image revert_overlay;

Color bgcolor(130, 80, 100);

enum stages {
	MENU,
	PAUSE,
	STAGE1,
	STAGE2
};
int current_stage = stages::MENU;

enum pauses {
	CONTINUE_GAME,
	BACK_MENU,
	QUIT_GAME
};
int pause_option = pauses::CONTINUE_GAME;


enum saveState : uint8{
	DEFAULT_NO_SAVESTATE_CHANGE= -1, SAVESTATE_SAVED, SAVESTATE_LOADED
};

float gravity = 800;
float jump = 120;
float x_vel = 60;
float bullet_vel = 180;
Vector2 checkpoint;

bool bullet_fired[5] = { false, false, false, false, false };
int skip = 0;

double coyote = -5;
double running = -5;
double last_fired = -5;

int curr_ground_idx;


// Dont use uint8 for tilesets bigger
// than 16x16, since you will have more than
// 256 different tiles









camBorders cb;

struct sGameInfo {
	Player player;
	camBorders camborders;

	static void saveGameInfo(Player& player, float time, saveState& save_state, bool& show, double& status_change_time)
	{
		sGameInfo game_info;
		game_info.player = player;
		FILE* fp = fopen("savegame.bin", "wb");
		fwrite(&game_info, sizeof(sGameInfo), 1, fp);
		save_state = SAVESTATE_SAVED;
		show = false;
		status_change_time = time;

		fclose(fp);
	}

	static void loadGameInfo(Player& player, float time, saveState& save_state, bool& show, double& status_change_time)
	{
		sGameInfo game_info;


		// Load file

		FILE* fp = fopen("savegame.bin", "rb");
		if (fp == NULL) // No saved game found
			return;

		fread(&game_info, sizeof(sGameInfo), 1, fp);
		fclose(fp);

		// Transfer data from game_info to Game
		// ...

		player = game_info.player;
		player.moving = 0;

		save_state = SAVESTATE_LOADED;
		show = false;
		status_change_time = time;
	}
};


saveState save_state = DEFAULT_NO_SAVESTATE_CHANGE;
double status_change_time = -10;
bool show = false;
///////////////////////////////////////////////////////////////////////////////////////
Player player;
sEntity bullets[N_BULLETS];
sEntity box_breakable;
double box_break_time;
sEntity key_tobreak;
///////////////////////////////////////////////////////////////////////////////////////
hitBox currentground;
int fb_size[2] = { 160, 120 };

double fps;



sGameInfo revert[1000];
int idx = 0;
int idx_should = 0;
int idx_lowest = 1;
bool reverting = false;
bool overit = false;

std::vector<hitBox> tuto_hitboxes;
std::vector<hitBox> ground_hitboxes;
std::vector<hitBox> platform_hitboxes;
std::vector<hitBox> obstacle_hitboxes;
int sueloStatus = hitBox::NO_HIT;

double tuto2 = 0;

hitBox water;

Game::Game(int window_width, int window_height, SDL_Window* window)
{
	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	this->renderer = NULL;
	instance = this;
	must_exit = false;

	std::cout << int(player.coords.x) << " " << int(player.coords.y) << "\n" << player.velocity.x << "\n\n\n\n";

	hitBox tuto_1 = hitBox(new Vector2(15 * 8, 28 * 8), new Vector2(0, 30 * 8)); ground_hitboxes.push_back(tuto_1);
	hitBox tuto_2 = hitBox(new Vector2(26 * 8, 27 * 8), new Vector2(14 * 8, 30 * 8)); ground_hitboxes.push_back(tuto_2);
	hitBox tuto_3 = hitBox(new Vector2(3 * 8, 0), new Vector2(0, 26 * 8)); ground_hitboxes.push_back(tuto_3);
	hitBox tuto_4 = hitBox(new Vector2(26 * 8, 26 * 8), new Vector2(25 * 8 + 1, 27 * 8)); ground_hitboxes.push_back(tuto_4);
	hitBox tuto_9 = hitBox(new Vector2(106 * 8, 29 * 8), new Vector2(86 * 8, 30 * 8)); ground_hitboxes.push_back(tuto_9);
	hitBox tuto_11 = hitBox(new Vector2(105 * 8, 23 * 8), new Vector2(104 * 8, 26 * 8)); ground_hitboxes.push_back(tuto_11);
	hitBox tuto_13 = hitBox(new Vector2(160 * 8, 28 * 8), new Vector2(129 * 8, 29 * 8)); ground_hitboxes.push_back(tuto_13);
	

	hitBox tuto_5 = hitBox(new Vector2(25 * 8, 24 * 8), new Vector2(16 * 8, 25 * 8)); platform_hitboxes.push_back(tuto_5);
	hitBox tuto_6 = hitBox(new Vector2(32 * 8, 22 * 8), new Vector2(26 * 8, 23 * 8)); platform_hitboxes.push_back(tuto_6);
	hitBox tuto_7 = hitBox(new Vector2(27 * 8, 18 * 8), new Vector2(23 * 8, 19 * 8)); platform_hitboxes.push_back(tuto_7);
	hitBox tuto_8 = hitBox(new Vector2(89 * 8, 26 * 8), new Vector2(37 * 8, 27 * 8)); platform_hitboxes.push_back(tuto_8);
	hitBox tuto_10 = hitBox(new Vector2(105 * 8, 23 * 8), new Vector2(95 * 8, 24 * 8)); platform_hitboxes.push_back(tuto_10);
	hitBox tuto_12 = hitBox(new Vector2(124 * 8, 26 * 8), new Vector2(105 * 8, 27 * 8)); platform_hitboxes.push_back(tuto_12);

	hitBox d1 = hitBox(new Vector2(52 * 8, 28 * 8 + 6), new Vector2(26 * 8, 30 * 8)); obstacle_hitboxes.push_back(d1);
	hitBox d2 = hitBox(new Vector2(300 * 8, 28 * 8 + 6), new Vector2(101 * 8, 30 * 8)); obstacle_hitboxes.push_back(d2);

	box_breakable.coords = { 129 * 8, 24 * 8 };
	box_breakable.hitbox.t_r = { 129 * 8 + 13, 19 * 8 };
	box_breakable.hitbox.b_l = { 129 * 8, 24 * 8 };
	ground_hitboxes.push_back(box_breakable.hitbox);

	key_tobreak.coords = { 126 * 8, 27 * 8 };
	key_tobreak.hitbox.t_r = { 127 * 8, 27 * 8 - 4 };
	key_tobreak.hitbox.b_l = { 126 * 8, 28 * 8 };
	key_tobreak.hitbox.type = hitBox::INTERACTIBLE;

	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;

	font.loadTGA("data/bitmap-font-white.tga"); //load bitmap-font image
	minifont.loadTGA("data/mini-font-white-4x6.tga"); //load bitmap-font image
	bigfont.loadTGA("data/big-font-white-14x18.tga");

	sprite.loadTGA("data/spritesheet.tga");
	sprite_mini.loadTGA("data/player_mini.tga");
	tileset.loadTGA("data/paper-tileset.tga");
	keyset.loadTGA("data/keys_tileset.tga");
	select_arrows.loadTGA("data/select_arrows.tga");
	simple_box.loadTGA("data/stick.tga");
	explosion.loadTGA("data/player_explosion.tga");
	grounding.loadTGA("data/grounding.tga");
	opening_keys.loadTGA("data/opening_keys.tga");
	tutorial_help1.loadTGA("data/tutorial_help1.tga");
	tutorial_help2.loadTGA("data/tutorial_help2.tga");
	tutorial_help3.loadTGA("data/tutorial_help3.tga");
	tutorial_help4.loadTGA("data/tutorial_help4.tga");
	clock_sprite.loadTGA("data/clock.tga");

	title_screen.loadTGA("data/title_screen.tga");
	pause_screen.loadTGA("data/pause_screen.tga");
	revert_overlay.loadTGA("data/bg.tga");

	player.size = { 8, 8 };
	player.hitbox.b_l = { player.coords.x + player.size.x / 3, player.coords.y + player.size.y };
	player.hitbox.t_r = { player.coords.x + player.size.x * 2 / 3, player.coords.y + player.size.y * 5 / 6 };



	map = GameMap::loadGameMap("data/tutorial.json");

	cb.cam_should = { 160 / 2 - player.size.x / 2, 120 / 2 - player.size.y / 2 };
	cb.cam_should -= player.coords;
	cb.cam_offset = cb.cam_should;

	cb.player_cam = { 0,0 };

	checkpoint.x = 80;
	checkpoint.y = 216;

	enableAudio(); //enable this line if you plan to add audio to your application

}



//what to do when the image has to be draw
void Game::render(void)
{
	// FrameBuffer
	Image fb(160, 120);

	//Create a new Image (or we could create a global one if we want to keep the previous frame)
	//do not change framebuffer size
	int cs = 8;
	int numtiles = 18;
	Vector2 player_print_coords = { fb_size[0] / 2 - player.size.x / 2 + cb.player_cam.x , fb_size[1] / 2 - player.size.y / 2 + cb.player_cam.y };
	switch (current_stage) {
	case stages::MENU:
		fb.drawImage(title_screen, 0, 0);
		fb.drawImage(keyset, 90, 81, Area(17 * (int(time * 3 / 2) % 2), 17, 17, 17));
		break;
	case stages::PAUSE:
		GameMap::drawTileset(map, cs, numtiles, tileset, fb, cb.cam_offset, time);
		fb.drawImage(sprite_mini,
			player_print_coords.x,
			player_print_coords.y,
			Area(player.moving * player.size.x * (int(time * 8) % 6),
				player.direction * player.size.y,
				player.size.x,
				player.size.y));
		fb.drawImage(pause_screen, 0, 0);
		fb.drawImage(select_arrows, 22, 20 + 31 * pause_option, Area(17 * (int(time * 3 / 2) % 2), pause_option * 17, 17, 17));
		break;	
	case stages::STAGE1:

		fb.fill(bgcolor);	//fills the image with one color

		// Size in pixels of a cell

		GameMap::drawTileset(map, cs, numtiles, tileset, fb, cb.cam_offset, time);

		GameMap::drawAsset(fb, tutorial_help1, Vector2(3 * 8, 16 * 8 - 4), cb.cam_offset, player.coords);
		GameMap::drawAsset(fb, keyset, Area(17*(int(time * 1.5) % 2), 34, 17, 17), Vector2(10 * 8 + 4, 21 * 8 - 3), cb.cam_offset, player.coords);
		GameMap::drawAsset(fb, keyset, Area(17*(int(time * 1.5 + 0.33) % 2), 4 * 17, 17, 17), Vector2(8 * 8 + 2, 19 * 8 - 5), cb.cam_offset, player.coords);
		GameMap::drawAsset(fb, keyset, Area(17*(int(time * 1.5 + 0.69) % 2), 3 * 17, 17, 17), Vector2(6 * 8, 21 * 8 - 3), cb.cam_offset, player.coords);
		GameMap::drawAsset(fb, keyset, Area(0, 5 * 17, 17, 17), Vector2(8 * 8 + 2, 21 * 8 - 3), cb.cam_offset, player.coords);


		GameMap::drawAsset(fb, tutorial_help2, Vector2(40 * 8, 16 * 8 - 4), cb.cam_offset, player.coords);
		if (time - tuto2 < 0.66) {
			GameMap::drawAsset(fb, keyset, Area(17 * (int((time - tuto2)* 6) % 2), 34, 17, 17), Vector2(47 * 8 + 8, 21 * 8 - 3), cb.cam_offset, player.coords);
		}
		else if ((time - tuto2 < 1.5)){
			GameMap::drawAsset(fb, keyset, Area(0, 34, 17, 17), Vector2(47 * 8 + 8, 21 * 8 - 3), cb.cam_offset, player.coords);
		}
		else {
			GameMap::drawAsset(fb, keyset, Area(0, 34, 17, 17), Vector2(47 * 8 + 8, 21 * 8 - 3), cb.cam_offset, player.coords);
			tuto2 = time;
		}
		
		GameMap::drawAsset(fb, keyset, Area(0, 4 * 17, 17, 17), Vector2(45 * 8 + 6, 19 * 8 - 5), cb.cam_offset, player.coords);
		GameMap::drawAsset(fb, keyset, Area(0, 3 * 17, 17, 17), Vector2(43 * 8 + 4, 21 * 8 - 3), cb.cam_offset, player.coords);
		GameMap::drawAsset(fb, keyset, Area(0, 5 * 17, 17, 17), Vector2(45 * 8 + 6, 21 * 8 - 3), cb.cam_offset, player.coords);

		GameMap::drawAsset(fb, tutorial_help3, Vector2(80 * 8, 16 * 8 - 4), cb.cam_offset, player.coords);

		GameMap::drawAsset(fb, tutorial_help4, Vector2(112 * 8, 16 * 8 - 4), cb.cam_offset, player.coords);
		GameMap::drawAsset(fb, keyset, Area(17 * (int(time * 2) % 2) , 0, 17, 17), Vector2(119 * 8, 16 * 8), cb.cam_offset, player.coords);


		
		fb.drawText("FPS: " + toString(int(fps)), 125, 1, minifont, 4, 6);

		if (save_state == SAVESTATE_SAVED && show) {
			fb.drawText("GAME SAVED...", 1, 105, font);
		}
		else if (save_state == SAVESTATE_LOADED && show) fb.drawText("GAME LOADED...", 1, 105, font);

		if (!player.teleporting) {
			fb.drawImage(sprite_mini,
				player_print_coords.x,
				player_print_coords.y,
				Area(player.moving * player.size.x * (int(time * 8) % 6),
					player.direction * player.size.y,
					player.size.x,
					player.size.y));	//draws only a part of an image
			if (time - player.last_ground < 0.5) {
				fb.drawImage(grounding,
					player.last_ground_pos.x - 12 + cb.cam_offset.x,
					player.last_ground_pos.y - 12 + cb.cam_offset.y,
					Area(32 * ((int((time - player.last_ground) * 8) % 4)),
						0,
						32,
						32));	//draws only a part of an image
			}
		}
		else if (time - player.last_death < 0.87) {
			fb.drawImage(explosion,
				player_print_coords.x - 12,
				player_print_coords.y - 12,
				Area(32 * ((int((time - player.last_death) * 8) % 8)),
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
		fb.drawImage(clock_sprite, 148, 71, Area(9 * (int(time * 25) % 16), 0, 9, 11));
		
		fb.drawRectangle(150, 84, 5, 31, Color::GRAY);
		fb.drawRectangle(151, 85, 3, 30 * (idx_should - idx_lowest)/1000, Color::CYAN);
		fb.drawRectangle(143, 110, 5, 5, Color::GRAY);
		fb.drawRectangle(144, 111, 3, 6*norm(0.5, time - last_fired), Color::RED);

		// Draw region of tileset inside framebuffer
		// fb.drawImage(tileset, screenx, screeny, area);
		for (sEntity bullet : bullets) {
			int screenx = bullet.coords.x + cb.cam_offset.x;
			int screeny = bullet.coords.y + cb.cam_offset.y;
			fb.drawLine(screenx, screeny, screenx + 3, screeny, Color::RED);
		}


		fb.drawText("BoxBreak: " + toString(box_breakable.active) + " | KeyThing: " + toString(key_tobreak.active), 2, 2, minifont, 4, 6);
		fb.drawText("TR: " + toString(box_breakable.hitbox.t_r.x) + ":" + toString(box_breakable.hitbox.t_r.y) +
			" | BL: " + toString(box_breakable.hitbox.b_l.x) + ":" + toString(box_breakable.hitbox.b_l.y), 2, 9, minifont, 4, 6);

		if (box_breakable.active) {
			fb.drawImage(simple_box, box_breakable.coords.x + cb.cam_offset.x, box_breakable.coords.y + cb.cam_offset.y - 40, Area(0, 0, 13, 40));
			fb.drawImage(opening_keys, box_breakable.coords.x + cb.cam_offset.x + 3, box_breakable.coords.y + cb.cam_offset.y - 22 + int(time * 2) % 2, Area(0, 0, 7, 4));
		}
		else if (time - box_break_time < 0.8) {
			fb.drawImage(simple_box, box_breakable.coords.x + cb.cam_offset.x, box_breakable.coords.y + cb.cam_offset.y - 40, Area(13 * (int(8 * (time - box_break_time)) % 8), 0, 13, 40));
		}
		else {
			fb.drawImage(simple_box, box_breakable.coords.x + cb.cam_offset.x, box_breakable.coords.y + cb.cam_offset.y - 40, Area(13 * 7, 0, 13, 40));
		}
		if (key_tobreak.active) {
			fb.drawImage(opening_keys, key_tobreak.coords.x + cb.cam_offset.x, key_tobreak.coords.y + cb.cam_offset.y + int(time * 2) % 2, Area(0, 0, 7, 4));
		}


		// DRAW HITBOXES
		//float player_print_hitbox[4] = { player_print_coords.x + player.size.x / 3, player_print_coords.y + player.size.y,
		//							player_print_coords.x + player.size.x * 2 / 3, player_print_coords.y + player.size.y * 5 / 6 };
		//fb.drawLine(player_print_hitbox[0], player_print_hitbox[1] - 1, player_print_hitbox[2], player_print_hitbox[3] - 1, Color::RED);
		//fb.drawLine(player_print_hitbox[2], player_print_hitbox[1] - 1, player_print_hitbox[0], player_print_hitbox[3] - 1, Color::RED);


		if (reverting) {
			fb.drawImage(revert_overlay, 0, 0);
			fb.drawText(toString((float)(idx_should - idx_lowest)/fps), 30, 51, bigfont, 14, 18);
		}
		break;
	}


	//send image to screen
	showFramebuffer(&fb);
}

void Game::update(double seconds_elapsed)
{
	switch (current_stage) {
	case stages::MENU:
		if (Input::wasKeyPressed(SDL_SCANCODE_N)) current_stage = stages::STAGE1;
		break;
	case stages::PAUSE:
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
			case pauses::CONTINUE_GAME: current_stage = STAGE1; break;
			case pauses::BACK_MENU: current_stage = MENU; break;
			case pauses::QUIT_GAME:	must_exit = true; break;
			}
			synth.playSample("data/sfx/select.wav", 0.6, false);
		}
		break;
	case stages::STAGE1:
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
			if ((time - status_change_time) > 2) save_state = DEFAULT_NO_SAVESTATE_CHANGE;
			else if ((time - status_change_time) > 0.05) show = true;
		}

		// compute frames per second
		fps = 1 / seconds_elapsed;

		player.dv.y = 1;
		if (!reverting && !player.teleporting) {
			//Read the keyboard state, to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
			if (Input::wasKeyPressed(SDL_SCANCODE_UP)) { 
				if ((time - coyote) < 0.15) {
					coyote = -5;
					player.velocity.y = -jump;
					player.grounded = false;
					player.last_jumptime = time;
					jump_sample = synth.playSample("data/sfx/jump.wav", 0.6, false);
				}
			}
			if (Input::isKeyPressed(SDL_SCANCODE_UP)) {
				if (time - player.last_jumptime < 0.2) {
					player.velocity.y -= gravity * seconds_elapsed;
				}
			}

			if (Input::wasKeyPressed(SDL_SCANCODE_LEFT))
			{
				if (player.direction == player.LEFT && (time - running) < 0.3)
					x_vel = 100;
				else x_vel = 60;
				running = time;
			}
			if (Input::wasKeyPressed(SDL_SCANCODE_RIGHT))
			{
				if (player.direction == player.RIGHT && (time - running) < 0.3)
					x_vel = 100;
				else x_vel = 60;
				running = time;
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
				if (time - last_fired > 0.5) {
					for (int i = 0; i < 5; i++) {
						if (!bullet_fired[i]) {
							last_fired = time;
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
					bullets[i].hitbox.t_r = { bullets[i].coords.x + 2, bullets[i].coords.y };
					bullets[i].hitbox.b_l = { bullets[i].coords.x, bullets[i].coords.y + 2 };
					for (hitBox h : ground_hitboxes) {
						if (bullets[i].hitbox.collided(&h) || abs(bullets[i].coords.x - player.coords.x) > fb_size[0]) {
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




			player.hitbox.b_l = { new_coords.x , new_coords.y + player.size.y - 1 };
			player.hitbox.t_r = { new_coords.x + player.size.x * 3 / 4, new_coords.y + player.size.y * 1 / 8 - 1 };

			if (player.hitbox.touching(&key_tobreak.hitbox) && key_tobreak.active) {
				key_tobreak.active = false;
				box_breakable.active = false;
				ground_hitboxes.pop_back();
				box_break_time = time;
				box_breakable.hitbox.t_r.y += 39;
			}

			int hit_type = hitBox::NO_HIT;


			if (player.grounded || player.velocity.y > 0) {
				for (hitBox h : ground_hitboxes) {
					if (player.hitbox.collided_status(&h) == hitBox::RIGHT_HIT || player.hitbox.collided_status(&h) == hitBox::LEFT_HIT) {
						hit_type = hitBox::LEFT_HIT;
					}
					else if (!player.grounded && player.hitbox.collided_status(&h) == hitBox::DOWN_HIT) {
						currentground = h;
						break;
					}
				}
			}
			else {
				for (hitBox h : ground_hitboxes) {
					if (player.hitbox.collided_status_sides(&h) == hitBox::RIGHT_HIT || player.hitbox.collided_status_sides(&h) == hitBox::LEFT_HIT) {
						hit_type = hitBox::LEFT_HIT;
					}
					else if (!player.grounded && player.hitbox.collided_status_sides(&h) == hitBox::DOWN_HIT) {
						currentground = h;
						break;
					}
				}
			}

			for (hitBox h : platform_hitboxes) {
				if (player.hitbox.collided_status(&h) == hitBox::DOWN_HIT) {
					currentground = h;
					break;
				}
			}





			if (!currentground.collided(&player.hitbox) || player.velocity.y < 0) {
				if (hit_type == hitBox::LEFT_HIT) {
					player.coords.y = new_coords.y;
					cb.player_cam.y += movement.y;
				}
				else if (true) {
					player.coords = new_coords;
					cb.player_cam += movement;
				}
			}


			if (player.hitbox.touching(&currentground)) {
				player.grounded = true;
				coyote = time;
			}
			else {
				player.grounded = false;
				currentground.b_l = { -1,-1 };
				currentground.t_r = { -1,-1 };
			}



			for (hitBox h : obstacle_hitboxes) {
				if (player.hitbox.touching(&h) || Input::isKeyPressed(SDL_SCANCODE_S)) {
					player.teleport(Vector2(checkpoint.x, checkpoint.y), elapsed_time, cb, time, synth);
					player.last_death = time;
					synth.playSample("data/sfx/rip.wav", 0.6, false);
				}
			}



		}
		else {
			if (player.teleporting) {
				idx_lowest = idx_should;
				if ((time - player.last_death) > .9) {
					player.teleport(Vector2(checkpoint.x, checkpoint.y), elapsed_time, cb, time, synth);
				}
			}
		}

		// Update camera offset
		uint8 xcamlimit[2] = { 15, 10 };
		uint8 ycamlimit[2] = { 20, 10 };

		camBorders::box_camera(cb.player_cam.x, cb.cam_should.x, cb.cam_offset.x, cb.x_limit, xcamlimit, map->width * 8, player.size.x, player.coords.x, fb_size[0]);
		camBorders::box_camera(cb.player_cam.y, cb.cam_should.y, cb.cam_offset.y, cb.y_limit, ycamlimit, map->height * 8, player.size.y, player.coords.y, fb_size[1]);



		if (Input::wasKeyPressed(SDL_SCANCODE_M)){
			synth.playSample("data/sfx/reverse.wav");
			reverting = true;
		}

		break;
	}


}

//Keyboard event handler (sync input)
void Game::onKeyDown(SDL_KeyboardEvent event)
{
	switch (current_stage) {
	case stages::MENU:
		break;
	case stages::STAGE1:
		switch (event.keysym.sym)
		{
			//	case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
		case SDLK_ESCAPE:
			current_stage = stages::PAUSE;
			pause_option = pauses::CONTINUE_GAME;
			break;
		case SDLK_k: sGameInfo::saveGameInfo(player, time, save_state, show, status_change_time); break;
		case SDLK_l: sGameInfo::loadGameInfo(player, time, save_state, show, status_change_time); break;
		}
		break;
	}

}

void Game::onKeyUp(SDL_KeyboardEvent event)
{
	switch (current_stage) {
	case stages::MENU:
		break;
	case stages::STAGE1:
		if (event.keysym.sym == SDLK_DOWN ||
			event.keysym.sym == SDLK_RIGHT ||
			event.keysym.sym == SDLK_LEFT) player.moving = 0;
		if (event.keysym.sym == SDLK_a) {
			player.moving = 0;
			coyote = time;
		}
		if (event.keysym.sym == SDLK_UP) { 
			player.moving = 0; 
			jump_sample->stop();
		}
		if (event.keysym.sym == SDLK_m) {
			idx_lowest = idx_should;
		}
		break;
	}
}


void Game::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Game::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Game::onMouseMove(SDL_MouseMotionEvent event)
{
}

void Game::onMouseButtonDown(SDL_MouseButtonEvent event)
{
}

void Game::onMouseButtonUp(SDL_MouseButtonEvent event)
{
}

void Game::onMouseWheel(SDL_MouseWheelEvent event)
{
}

void Game::onResize(int width, int height)
{
	std::cout << "window resized: " << width << "," << height << std::endl;
#ifdef USE_OPENGL
	glViewport(0, 0, width, height);
#endif
	window_width = width;
	window_height = height;
}

//sends the image to the framebuffer of the GPU
void Game::showFramebuffer(Image* img)
{
	float startx = -1.0; float starty = -1.0;
	float width = 2.0; float height = 2.0;

	//center in window
	float real_aspect = window_width / (float)window_height;
	float desired_aspect = img->width / (float)img->height;
	float diff = desired_aspect / real_aspect;
	width *= diff;
	startx = -diff;

#ifdef USE_OPENGL
	static GLuint texture_id = -1;
	static GLuint shader_id = -1;
	if (!texture_id)
		glGenTextures(1, &texture_id);

	//upload as texture
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, img->width, img->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);
	glDisable(GL_CULL_FACE); glDisable(GL_DEPTH_TEST); glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex2f(startx, starty + height);
	glTexCoord2f(1.0, 0.0); glVertex2f(startx + width, starty + height);
	glTexCoord2f(1.0, 1.0); glVertex2f(startx + width, starty);
	glTexCoord2f(0.0, 1.0); glVertex2f(startx, starty);
	glEnd();
#else
	static SDL_Texture* texture = NULL;
	static int tex_width = 0;
	static int tex_height = 0;
	if (!texture || tex_width != img->width || tex_height != img->height)
	{
		if (texture)
			SDL_DestroyTexture(texture);
		texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_BGR888, SDL_TEXTUREACCESS_TARGET, img->width, img->height);
		tex_width = img->width;
		tex_height = img->height;
	}

	//SDL_RenderClear(renderer);
	SDL_Rect rect = { 0,0,img->width,img->height };
	SDL_UpdateTexture(texture, &rect, img->pixels, img->width * 4);
	SDL_RenderCopy(renderer, texture, NULL, NULL);

#endif
	/* this version resizes the image which is slower
	Image resized = *img;
	//resized.quantize(1); //change this line to have a more retro look
	resized.scale(window_width, window_height);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	if (1) //flip
	{
	glRasterPos2f(-1, 1);
	glPixelZoom(1, -1);
	}
	glDrawPixels( resized.width, resized.height, GL_RGBA, GL_UNSIGNED_BYTE, resized.pixels );
	*/
}

//AUDIO STUFF ********************

SDL_AudioSpec audio_spec;

void AudioCallback(void* userdata,
	Uint8* stream,
	int    len)
{
	static double audio_time = 0;

	memset(stream, 0, len);//clear
	if (!Game::instance)
		return;

	Game::instance->onAudio((float*)stream, len / sizeof(float), audio_time, audio_spec);
	audio_time += len / (double)audio_spec.freq;
}

void Game::enableAudio()
{
	SDL_memset(&audio_spec, 0, sizeof(audio_spec)); /* or SDL_zero(want) */
	audio_spec.freq = 48000;
	audio_spec.format = AUDIO_F32;
	audio_spec.channels = 1;
	audio_spec.samples = 1024;
	audio_spec.callback = AudioCallback; /* you wrote this function elsewhere. */
	if (SDL_OpenAudio(&audio_spec, &audio_spec) < 0) {
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		exit(-1);
	}
	SDL_PauseAudio(0);
}

void Game::onAudio(float* buffer, unsigned int len, double time, SDL_AudioSpec& audio_spec)
{
	//fill the audio buffer using our custom retro synth
	synth.generateAudio(buffer, len, audio_spec);
}
