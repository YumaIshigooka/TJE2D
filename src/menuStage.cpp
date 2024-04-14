#include "menuStage.h"
#include "math.h"
#include <math.h>
#include "game.h"
#include "gameStage_0.h"

menuStage* menuStage::instance = NULL;

void menuStage::saveScore()
{
	if (besttime > gameStage_0::instance->totaltime) {
		besttime = gameStage_0::instance->totaltime;
		FILE* fp = fopen("best_time.bin", "wb");
		fwrite(&besttime, sizeof(float), 1, fp);
		fclose(fp);
	}
}

void menuStage::loadScore()
{
	FILE* fp = fopen("best_time.bin", "rb");
	if (fp == NULL) {
		besttime = 70.123;
		return;
	} 
	fread(&besttime, sizeof(float), 1, fp);
	fclose(fp);
}

menuStage::menuStage() {
	loadScore();
	bgmusic = new Synth::SamplePlayback();
	map = GameMap::loadGameMap("data/bgmap.json");
	win = false;
	tosave = false;
	instance = this;
}


void menuStage::onEnter() {
	loadScore();
	transitioning = false;
	transition_start = Game::instance->time;
	tr_in = true;
	if (win) {
		saveScore();
	}
	bgmusic = Game::instance->synth.playSample("data/sfx/menubgm.wav", 1, true);
}

void menuStage::onLeave() {
	bgmusic->stop();
}


void menuStage::render(Image& fb) {
	//Create a new Image (or we could create a global one if we want to keep the previous frame)
	//do not change framebuffer size
	int cs = 8;
	int numtiles = 18;

	GameMap::drawTileset(map, cs, numtiles, Game::instance->tileset, fb, Vector2(-int(Game::instance->time * 20) % 240, -int(Game::instance->time * 10) % 120), Game::instance->time);

	if (win) {
		fb.drawRectangle(0, 0, 160, 120, Color(105, 132, 132, 120));
		fb.drawImage(Game::instance->end_screen, 24, 20);
		fb.drawText(toString(gameStage_0::instance->totaltime), 71, 90, Game::instance->font_b);
		fb.drawText(toString(gameStage_0::instance->totaltime), 70, 89, Game::instance->font);

	}
	else {
		fb.drawRectangle(0, 0, 160, 120, Color(0, 0, 0, 120));
		fb.drawImage(Game::instance->title_screen, 0, 0);
		fb.drawImage(Game::instance->keyset, 88, 69, Area(17 * (int(Game::instance->time * 3 / 2) % 2), 17, 17, 17));
	}
	fb.drawText("Your best time: " + toString(besttime), 37, 110, Game::instance->minifont_b, 4, 6);
	fb.drawText("Your best time: " + toString(besttime), 36, 109, Game::instance->minifont, 4, 6);

	// Transition
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
	if (Input::wasKeyPressed(SDL_SCANCODE_Z)) {

		if (!tr_in && !transitioning) {
			if (win) {
				win = false;
			}
			else {
				transitioning = true; transition_start = Game::instance->time; tr_in = false;
				Game::instance->synth.playSample("data/sfx/select.wav", 0.6, false);
			}
			
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
