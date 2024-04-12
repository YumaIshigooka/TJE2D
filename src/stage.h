#ifndef STAGE_H
#define STAGE_H

#include "game.h"
#include "includes.h"
#include "utils.h"
#include "json.hpp"
#include "image.h"
#include "player.h"
#include "input.h"

#include <fstream>

#define N_BULLETS 5

struct sGameInfo {
	Player player;
	camBorders camborders;

	enum saveState : uint8 {
		DEFAULT_NO_SAVESTATE_CHANGE = -1, SAVESTATE_SAVED, SAVESTATE_LOADED
	};

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


class Stage {
public:
	//Global Values
	static float gravity;
	static float jump;
	static float x_vel;
	static float bullet_vel;

	// Map
	GameMap* map;
	int stage_number;

	// Terrain Hitboxes
	std::vector<hitBox*> *ground_hitboxes;
	std::vector<hitBox*> *platform_hitboxes;
	std::vector<hitBox*> *obstacle_hitboxes;
	std::vector<hitBox*> *other_hitboxes;

	hitBox currentground;
	int sueloStatus = hitBox::NO_HIT;

	// Timers
	double coyote = -5;
	double running = -5;
	double last_fired = -5;
	double tuto2_timer = -5;
	double status_change_time = -5;

	// Player & Bullets
	Player player;
	sEntity bullets[N_BULLETS];
	bool bullet_fired[5] = { false, false, false, false, false };

	Vector2 checkpoint;
	camBorders cb;

	// For the Reverting
	sGameInfo revert[1000];
	int skip = 0;
	int idx = 0;
	int idx_should = 0;
	int idx_lowest = 1;
	bool reverting = false;
	float fps;

	// Saving & Loading
	sGameInfo::saveState save_state;
	bool show;

	// Pausing
	bool paused = false;
	enum pauses {
		CONTINUE_GAME,
		BACK_MENU,
		QUIT_GAME
	};
	int pause_option = pauses::CONTINUE_GAME;

	// Audio
	Synth synth;
	Synth::SamplePlayback* jump_sample;

	int fb_size[2] = {160, 120};

	// Main Functions
		//main functions
	virtual void render(Image& fb) {};
	virtual void update(double dt) {};

	virtual void onEnter() {};
	virtual void onLeave() {};

	//events
	virtual void onKeyDown(SDL_KeyboardEvent event) {};
	virtual void onKeyUp(SDL_KeyboardEvent event) {};
	virtual void onMouseButtonDown(SDL_MouseButtonEvent event) {};
	virtual void onMouseButtonUp(SDL_MouseButtonEvent event) {};
	virtual void onMouseMove(SDL_MouseMotionEvent event) {};
	virtual void onMouseWheel(SDL_MouseWheelEvent event) {};
	virtual void onGamepadButtonDown(SDL_JoyButtonEvent event) {};
	virtual void onGamepadButtonUp(SDL_JoyButtonEvent event) {};
};

#endif