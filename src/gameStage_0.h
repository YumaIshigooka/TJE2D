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


	// Constructor
	gameStage_0();

	// Main Functions
		//main functions
	void render(void);
	void update(double dt);


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