#include "game.h"
#include "utils.h"
#include "input.h"
#include "image.h"
#include "hitbox.h"
#include "sEntity.h"
#include "player.h"
#include "camBorders.h"
#include "gameMap.h"
#include "gameStage_0.h"
#include "menuStage.h"
#include "stage.h"
#include "json.hpp"


double last_transition;


gameStage_0* tutorial;
menuStage* menu;

Game* Game::instance = NULL;

GameMap* map;


Color bgcolor(130, 80, 100);



std::unordered_map<Game::stages, Stage*> stage_map;
Game::stages current_stage_id = Game::stages::MENU;
Stage* current_stage;


Game::Game(int window_width, int window_height, SDL_Window* window)
{
	Vector2 caca;
	caca.length();
	//stage_map = new std::unordered_map<Game::stages, Stage*>;
	stage_map[Game::stages::MENU] = menu;
	stage_map[Game::stages::STAGE0] = tutorial;

	font.loadTGA("data/bitmap-font-white.tga");
	minifont.loadTGA("data/mini-font-white-4x6.tga");
	minifont_b.loadTGA("data/mini-font-black-4x6.tga");
	bigfont.loadTGA("data/big-font-white-14x18.tga");

	sprite.loadTGA("data/spritesheet.tga");
	sprite_mini.loadTGA("data/player_mini.tga");
	tileset.loadTGA("data/paper-tileset.tga");
	keyset.loadTGA("data/keys_tileset.tga");
	select_arrows.loadTGA("data/select_arrows.tga");
	simple_box.loadTGA("data/stick.tga");
	box_tileset.loadTGA("data/box_tileset.tga");
	explosion.loadTGA("data/player_explosion.tga");
	grounding.loadTGA("data/grounding.tga");
	opening_keys.loadTGA("data/opening_keys.tga");
	tutorial_help1.loadTGA("data/tutorial_help1.tga");
	tutorial_help2.loadTGA("data/tutorial_help2.tga");
	tutorial_help3.loadTGA("data/tutorial_help3.tga");
	tutorial_help4.loadTGA("data/tutorial_help4.tga");
	clock_sprite.loadTGA("data/clock.tga");

	title_screen.loadTGA("data/title_screen.tga");
	pause_screen.loadTGA("data/pause_screen.tga");
	revert_overlay.loadTGA("data/bg.tga");


	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	this->renderer = NULL;
	instance = this;
	
	must_exit = false;
	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;
	
	menu = new menuStage();
	tutorial = new gameStage_0();
	current_stage = menu;

	enableAudio(); //enable this line if you plan to add audio to your application
}

void Game::switch_stage(Game::stages new_stage_id) {
	////current_stage_id = new_stage;
	//std::cout << new_stage_id;
	//Stage* new_stage;
	//std::unordered_map<Game::stages, Stage*>::const_iterator pair = stage_map.find(new_stage_id);
	//current_stage = pair->second;
	//std::cout << current_stage;
	//current_stage = tutorial;

	// He intentado hacerlo con un map pero el puntero no se reescribia bien.
	
	

	current_stage->onLeave();
	switch (new_stage_id) {
	case stages::MENU:
		current_stage = menu;
		break;
	case stages::STAGE0:
		current_stage = tutorial;
		break;
	}
	current_stage->onEnter();
	return;
}


//what to do when the image has to be draw
void Game::render(void)
{
	Image fb(160, 120);

	current_stage->render(fb);

	showFramebuffer(&fb);
}

void Game::update(double seconds_elapsed)
{
	current_stage->update(seconds_elapsed);
}

//Keyboard event handler (sync input)
void Game::onKeyDown(SDL_KeyboardEvent event)
{
	current_stage->onKeyDown(event);
}

void Game::onKeyUp(SDL_KeyboardEvent event)
{
	current_stage->onKeyUp(event);
}


void Game::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Game::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Game::onMouseMove(SDL_MouseMotionEvent event)
{

}

void Game::onMouseButtonDown(SDL_MouseButtonEvent event)
{

}

void Game::onMouseButtonUp(SDL_MouseButtonEvent event)
{

}

void Game::onMouseWheel(SDL_MouseWheelEvent event)
{

}

void Game::onResize(int width, int height)
{
	std::cout << "window resized: " << width << "," << height << std::endl;
#ifdef USE_OPENGL
	glViewport(0, 0, width, height);
#endif
	window_width = width;
	window_height = height;
}

//sends the image to the framebuffer of the GPU
void Game::showFramebuffer(Image* img)
{
	float startx = -1.0; float starty = -1.0;
	float width = 2.0; float height = 2.0;

	//center in window
	float real_aspect = window_width / (float)window_height;
	float desired_aspect = img->width / (float)img->height;
	float diff = desired_aspect / real_aspect;
	width *= diff;
	startx = -diff;

#ifdef USE_OPENGL
	static GLuint texture_id = -1;
	static GLuint shader_id = -1;
	if (!texture_id)
		glGenTextures(1, &texture_id);

	//upload as texture
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, img->width, img->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);
	glDisable(GL_CULL_FACE); glDisable(GL_DEPTH_TEST); glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex2f(startx, starty + height);
	glTexCoord2f(1.0, 0.0); glVertex2f(startx + width, starty + height);
	glTexCoord2f(1.0, 1.0); glVertex2f(startx + width, starty);
	glTexCoord2f(0.0, 1.0); glVertex2f(startx, starty);
	glEnd();
#else
	static SDL_Texture* texture = NULL;
	static int tex_width = 0;
	static int tex_height = 0;
	if (!texture || tex_width != img->width || tex_height != img->height)
	{
		if (texture)
			SDL_DestroyTexture(texture);
		texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_BGR888, SDL_TEXTUREACCESS_TARGET, img->width, img->height);
		tex_width = img->width;
		tex_height = img->height;
	}

	//SDL_RenderClear(renderer);
	SDL_Rect rect = { 0,0,img->width,img->height };
	SDL_UpdateTexture(texture, &rect, img->pixels, img->width * 4);
	SDL_RenderCopy(renderer, texture, NULL, NULL);

#endif
	/* this version resizes the image which is slower
	Image resized = *img;
	//resized.quantize(1); //change this line to have a more retro look
	resized.scale(window_width, window_height);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	if (1) //flip
	{
	glRasterPos2f(-1, 1);
	glPixelZoom(1, -1);
	}
	glDrawPixels( resized.width, resized.height, GL_RGBA, GL_UNSIGNED_BYTE, resized.pixels );
	*/
}

//AUDIO STUFF ********************

SDL_AudioSpec audio_spec;

void AudioCallback(void* userdata,
	Uint8* stream,
	int    len)
{
	static double audio_time = 0;

	memset(stream, 0, len);//clear
	if (!Game::instance)
		return;

	Game::instance->onAudio((float*)stream, len / sizeof(float), audio_time, audio_spec);
	audio_time += len / (double)audio_spec.freq;
}

void Game::enableAudio()
{
	SDL_memset(&audio_spec, 0, sizeof(audio_spec)); /* or SDL_zero(want) */
	audio_spec.freq = 48000;
	audio_spec.format = AUDIO_F32;
	audio_spec.channels = 1;
	audio_spec.samples = 1024;
	audio_spec.callback = AudioCallback; /* you wrote this function elsewhere. */
	if (SDL_OpenAudio(&audio_spec, &audio_spec) < 0) {
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		exit(-1);
	}
	SDL_PauseAudio(0);
}

void Game::onAudio(float* buffer, unsigned int len, double time, SDL_AudioSpec& audio_spec)
{
	//fill the audio buffer using our custom retro synth
	synth.generateAudio(buffer, len, audio_spec);
}


