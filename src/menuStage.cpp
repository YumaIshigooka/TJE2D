#include "menuStage.h"
#include "math.h"
#include <math.h>
#include "game.h"

menuStage::menuStage() {

}


void menuStage::onEnter() {
	transitioning = false;
	transition_start = Game::instance->time;
	tr_in = true;
}

void menuStage::onLeave() {
}


void menuStage::render(Image& fb) {
	//Create a new Image (or we could create a global one if we want to keep the previous frame)
	//do not change framebuffer size
	int cs = 8;
	int numtiles = 18;

	fb.drawImage(Game::instance->title_screen, 0, 0);
	fb.drawImage(Game::instance->keyset, 90, 81, Area(17 * (int(Game::instance->time * 3 / 2) % 2), 17, 17, 17));

	if (transitioning) {
		fb.drawRectangle(0, 0, fb.width,
			(Game::instance->time - transition_start) * 3 * fb.height, Color::BLACK);
		if ((Game::instance->time - transition_start) * 3 * fb.height > fb.height + 25) Game::instance->switch_stage(Game::stages::STAGE0);

	}
	else if (tr_in) {
		fb.drawRectangle(0, (Game::instance->time - transition_start) * 3 * fb.width, fb.width,
			(1 - Game::instance->time + transition_start) * 3 * fb.height, Color::BLACK);
		if (Game::instance->time - transition_start > .25) tr_in = false;
	}
}


void menuStage::update(double seconds_elapsed)
{
	if (Input::wasKeyPressed(SDL_SCANCODE_N)) {
		if (!tr_in && !transitioning) {
			transitioning = true; transition_start = Game::instance->time; tr_in = false;
		}
		std::cout << "Hello";
	} 
}


void menuStage::onKeyDown(SDL_KeyboardEvent event) {

}

void menuStage::onKeyUp(SDL_KeyboardEvent event) {

}

void menuStage::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void menuStage::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void menuStage::onMouseMove(SDL_MouseMotionEvent event)
{
}

void menuStage::onMouseButtonDown(SDL_MouseButtonEvent event)
{
}

void menuStage::onMouseButtonUp(SDL_MouseButtonEvent event)
{
}

void menuStage::onMouseWheel(SDL_MouseWheelEvent event)
{
}
