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
	GameMap* clouds;
	static gameStage_0* instance;
	// Terrain Hitboxes
	std::vector<hitBox*>* ground_hitboxes;
	std::vector<hitBox*>* platform_hitboxes;
	std::vector<hitBox*>* obstacle_hitboxes;
	std::vector<hitBox*>* other_hitboxes;

	hitBox currentground;
	int sueloStatus = hitBox::NO_HIT;

	// Timers
	float coyote = -5;
	float running = -5;
	float last_fired = -5;
	float tuto2_timer = -5;
	float status_change_time = -5;

	// Player & Bullets
	Player player;
	bool player_canjump = true;
	sEntity* bullets[N_BULLETS];
	bool bullet_fired[N_BULLETS] = { false, false, false, false, false };
	bool bullet_strong[N_BULLETS] = { false, false, false, false, false };
	bool bullet_fired_while_reverse = false;

	Vector2 checkpoint;
	camBorders cb;

	// For the Reverting
	sGameInfo revert[1000];
	int skip = 0;
	int idx = 0;
	int idx_should = 0;
	int idx_lowest = 1;
	bool reverting = false;
	float totaltime = 0;
	float fps;

	// Saving & Loading
	//sGameInfo::saveState save_state;
	//bool show;

	// Pausing
	bool paused = false;
	enum pauses : uint8 {
		CONTINUE_GAME,
		BACK_MENU,
		QUIT_GAME
	};
	int pause_option = pauses::CONTINUE_GAME;

	// Audio
	Synth::SamplePlayback* jump_sample;




	sEntity* flag;
	sEntity pillar;
	sEntity key_tobreak;
	sEntity* boxes;

	sEntity ground;

	Game::stages gotoStage;

	float box_break_time;
	float bullet_reverse = -10;

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


private:
	void addHitbox(std::vector<hitBox*>* l, float tr_x, float tr_y, float bl_x, float bl_y, sEntity* father = nullptr);
	void drawAllAssets(Image& fb);
	void drawPlayerandBullets(Image& fb);
	void drawInterface(Image& fb);
	void addBox(Vector2 coords, Vector2 size, hitBox::type_HB type, std::vector<hitBox*>* l1, std::vector<hitBox*>* l2);
	void restart_map_assets();
	void handle_shooting ();
	void handle_bullets (double seconds_elapsed);
	void compute_player_hitBox();


};

#endif