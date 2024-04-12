/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This class encapsulates the game, is in charge of creating the game, getting the user input, process the update and render.
*/

#ifndef GAME_H
#define GAME_H

#include "includes.h"
#include "image.h"
#include "utils.h"
#include "synth.h"
#include <fstream>
#include <cmath>

class Game
{
public:
	static Game* instance;

	//window
	SDL_Window* window;
	SDL_Renderer* renderer;
	int window_width;
	int window_height;


	//some globals
	long frame;
    float time;
	float elapsed_time;
	int fps;
	bool must_exit;

	// Fonts
	Image font, minifont, bigfont;

	// Sprite/Tile Sets
	Image sprite, sprite_mini;
	Image explosion, grounding;
	Image tileset;
	Image keyset, select_arrows;
	Image simple_box, opening_keys, clock_sprite;

	Image tutorial_help1, tutorial_help2, tutorial_help3, tutorial_help4;

	// Other
	Image title_screen, pause_screen, revert_overlay;
	Color bgcolor;

	//audio
	Synth synth;

	// Stages
	enum stages {
		MENU = 1,
		STAGE0,
		STAGE1
	};

	//ctor
	Game( int window_width, int window_height, SDL_Window* window );

	//util
	static int sign(float value) {
		return value >= 0 ? 1 : -1;
	}
	static float norm(float target, float value) {
		return abs(value) > target ? sign(value) * target : value;
	}
	

	//main functions
	void render( void );
	void update( double dt );

	void showFramebuffer(Image* img);

	//switcg stage
	void switch_stage(Game::stages new_stage);

	//events
	void onKeyDown( SDL_KeyboardEvent event );
	void onKeyUp(SDL_KeyboardEvent event);
	void onMouseButtonDown( SDL_MouseButtonEvent event );
	void onMouseButtonUp(SDL_MouseButtonEvent event);
	void onMouseMove(SDL_MouseMotionEvent event);
	void onMouseWheel(SDL_MouseWheelEvent event);
	void onGamepadButtonDown(SDL_JoyButtonEvent event);
	void onGamepadButtonUp(SDL_JoyButtonEvent event);
	void onResize(int width, int height);

	//audio stuff
	void enableAudio(); //opens audio channel to play sound
	void onAudio(float* buffer, unsigned int len, double time, SDL_AudioSpec &audio_spec); //called constantly to fill the audio buffer
};


#endif 