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

#define N_BULLETS 5


class Stage0 : public Stage {
public:
	//Global Values
	static float gravity;
	static float jump;
	static float x_vel;
	static float bullet_vel;

	// Map
	GameMap* map;
	int stage_number;

	// Fonts
	Image font, minifont, bigfont;

	// Sprite/Tile Sets
	Image sprite_mini, explosion, grounding;
	Image tileset;
	Image keyset, select_arrows;
	Image simple_box, opening_keys, clock_sprite;

	Image tutorial_help1, tutorial_help2, tutorial_help3, tutorial_help4;

	// Other
	Image pause_screen, revert_overlay;
	Color Stage0::bgcolor;

	// Terrain Hitboxes
	std::vector<hitBox> ground_hitboxes;
	std::vector<hitBox> platform_hitboxes;
	std::vector<hitBox> obstacle_hitboxes;
	std::vector<hitBox> other_hitboxes;

	hitBox currentground;

	// Timers
	double time;
	double coyote;
	double running;
	double last_fired;
	double tuto2_timer;

	// Player & Bullets
	Player player;
	sEntity bullets[N_BULLETS];
	bool bullet_fired[5] = { false, false, false, false, false };

	Vector2 check_point;
	camBorders cb;

	// For the Reverting
	sGameInfo revert[1000];
	int idx, idx_should, idx_lowest;
	float fps;
	int idx = 0;		//
	int idx_should = 0; //
	int idx_lowest = 1; //
	bool reverting = false;


	hitBox currentground;

	// Constructor
	Stage0();

	// Main Functions
		//main functions
	void render(void);
	void update(double dt);

	void showFramebuffer(Image* img);

	//events
	void onKeyDown(SDL_KeyboardEvent event);
	void onKeyUp(SDL_KeyboardEvent event);
	void onMouseButtonDown(SDL_MouseButtonEvent event);
	void onMouseButtonUp(SDL_MouseButtonEvent event);
	void onMouseMove(SDL_MouseMotionEvent event);
	void onMouseWheel(SDL_MouseWheelEvent event);
	void onGamepadButtonDown(SDL_JoyButtonEvent event);
	void onGamepadButtonUp(SDL_JoyButtonEvent event);
	void onResize(int width, int height);
};

#endif