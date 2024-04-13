#ifndef STAGE0_H
#define STAGE0_H

#include "includes.h"
#include "gameMap.h"
#include "hitbox.h"
#include "math.h"
#include "stage.h"
#include "utils.h"
#include "json.hpp"
#include "image.h"
#include "player.h"

#include <fstream>


class gameStage_0 : public Stage {
public:
	static gameStage_0* instance;
	// Terrain Hitboxes
	std::vector<hitBox*>* ground_hitboxes;
	std::vector<hitBox*>* platform_hitboxes;
	std::vector<hitBox*>* obstacle_hitboxes;
	std::vector<hitBox*>* other_hitboxes;

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
	sEntity* bullets[N_BULLETS];
	bool bullet_fired[N_BULLETS] = { false, false, false, false, false };
	bool bullet_strong[N_BULLETS] = { false, false, false, false, false };

	Vector2 checkpoint;
	camBorders cb;

	// For the Reverting
	sGameInfo revert[1000];
	int skip = 0;
	int idx = 0;
	int idx_should = 0;
	int idx_lowest = 1;
	bool reverting = false;
	double totaltime = 0;
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



	int fb_size[2] = { 160, 120 };

	// Constructor
	gameStage_0();

	// Main Functions
		//main functions
	void render(Image& fb);
	void update(double dt);

	void onEnter();
	void onLeave();


	//events
	void onKeyDown(SDL_KeyboardEvent event);
	void onKeyUp(SDL_KeyboardEvent event);
	void onMouseButtonDown(SDL_MouseButtonEvent event);
	void onMouseButtonUp(SDL_MouseButtonEvent event);
	void onMouseMove(SDL_MouseMotionEvent event);
	void onMouseWheel(SDL_MouseWheelEvent event);
	void onGamepadButtonDown(SDL_JoyButtonEvent event);
	void onGamepadButtonUp(SDL_JoyButtonEvent event);
};

#endif