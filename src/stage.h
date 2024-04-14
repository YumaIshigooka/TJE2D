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
	sEntity bullets[N_BULLETS];
	bool bullet_fired[N_BULLETS];

	enum saveState : int8 {
		DEFAULT_NO_SAVESTATE_CHANGE = -1, SAVESTATE_SAVED, SAVESTATE_LOADED
	};

	static void saveGameInfo(Player& player, float time, saveState& save_state, bool& show, float& status_change_time)
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

	static void loadGameInfo(Player& player, float time, saveState& save_state, bool& show, float& status_change_time)
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

	Synth::SamplePlayback* bgmusic;
	// Map
	GameMap* map;
	int stage_number;

	// Transition
	bool transitioning = false;
	bool tr_in = true;
	double transition_start = -10;

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