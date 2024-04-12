#include "menuStage.h"
#include "math.h"
#include <math.h>
#include "game.h"



menuStage::menuStage() {

}


void menuStage::onEnter() {
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
}


void menuStage::update(double seconds_elapsed)
{
	if (Input::wasKeyPressed(SDL_SCANCODE_N)) {
		Game::instance->switch_stage(Game::stages::STAGE0);
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
