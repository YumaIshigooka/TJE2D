#ifndef MENUSTAGE_H
#define MENUSTAGE_H

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


class menuStage : public Stage {
public:
	static menuStage* instance;
	bool win;
	bool tosave;
	bool newbest;
	float besttime;
	// Constructor
	menuStage();

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

private:
	void saveScore();
	void loadScore();
};

#endif