#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "includes.h"
#include "utils.h"
#include "json.hpp"
#include "image.h"
#include "player.h"

#include <fstream>


struct sCell {
	int type;
	int item;
};

class sLayer {
public:
	sCell* data;
};

class GameMap {
public:

	int width;
	int height;
	int num_layers;
	sLayer* layers;

	GameMap();

	GameMap(int w, int h);

	GameMap(int w, int h, int numLayers);

	sCell& getCell(int x, int y, int l);

	static GameMap* loadGameMap(const char* filename);

	static void drawAsset(Image& fb, Image asset, Area area, Vector2& coords, Vector2 offset, Vector2 player_coords);
	static void drawAsset(Image& fb, Image asset, Vector2& coords, Vector2 offset, Vector2 player_coords);

	static void drawTileset(GameMap* map, int cs, int numtiles, Image tileset, Image& fb, Vector2 offset, double time);
};

#endif