#include "gameMap.h"

GameMap::GameMap() {
	this->width = this->height = this->num_layers = 0;
	this->layers = nullptr;
}

GameMap::GameMap(int w, int h) {
	this->width = w;
	this->height = h;
	this->layers = nullptr;
	this->num_layers = 0;
}

GameMap::GameMap(int w, int h, int numLayers) {
	width = w;
	height = h;
	num_layers = numLayers;
	layers = new sLayer[numLayers];
}

sCell& GameMap::getCell(int x, int y, int l) {
	return layers[l].data[x + y * width];
}

GameMap* GameMap::loadGameMap(const char* filename)
{
	using json = nlohmann::json;
	std::ifstream f(filename);
	if (!f.good())
		return nullptr;
	json jData = json::parse(f);

	int w = jData["width"];
	int h = jData["height"];
	int numLayers = jData["layers"].size();

	GameMap* map = new GameMap(w, h);
	// Allocate memory each layer
	map->num_layers = numLayers;
	map->layers = new sLayer[numLayers];

	for (int l = 0; l < numLayers; l++) {
		// Allocate memory for data inside each layer
		map->layers[l].data = new sCell[w * h];
		json layer = jData["layers"][l];
		for (int x = 0; x < map->width; x++) {
			for (int y = 0; y < map->height; y++) {
				int index = x + y * map->width;
				int type = layer["data"][index].get<int>();
				map->getCell(x, y, l).type = type - 1;
			}
		}
	}

	return map;
}

void GameMap::drawTileset(GameMap* map, int cs, int numtiles, Image tileset, Image& fb, Vector2 offset, double time) {
	for (int x = 0; x < map->width; ++x)
		for (int y = 0; y < map->height; ++y)
			for (int l = 0; l < map->num_layers; l++) {

				// Get cell info
				sCell& cell = map->getCell(x, y, l);

				if (cell.type == -1)
					continue;
				int type = (int)cell.type;

				if (type == 325) type += int(time * 8) % 8;

				// Compute tile pos in tileset image
				int tilex = (type % numtiles) * cs;
				int tiley = floor(type / numtiles) * cs;

				// Create tile area
				Area area(tilex, tiley, cs, cs);

				// Place offset here if you want
				int screenx = x * cs + offset.x;
				int screeny = y * cs + offset.y;

				// Avoid rendering out of screen stuff
				if (screenx < -cs || screenx >(int)fb.width ||
					screeny < -cs || screeny >(int)fb.height)
					continue;

				// Draw region of tileset inside framebuffer
				fb.drawImage(tileset, screenx, screeny, area);
			}
}

void GameMap::drawAsset(Image& fb, Image asset, Area area, Vector2& coords, Vector2 offset, Vector2 player_coords) {
	if (coords.x - player_coords.x < fb.width && player_coords.x - coords.x < fb.width + asset.width &&
		coords.y - player_coords.y < fb.width && player_coords.y - coords.y < fb.height + asset.height) {
		fb.drawImage(asset, coords.x + offset.x, coords.y + offset.y, area);
	}
}

void GameMap::drawAsset(Image & fb, Image asset, Vector2& coords, Vector2 offset, Vector2 player_coords) {
	if (coords.x - player_coords.x < fb.width && player_coords.x - coords.x < fb.width + asset.width &&
		coords.y - player_coords.y < fb.width && player_coords.y - coords.y < fb.height + asset.height) {
		fb.drawImage(asset, coords.x + offset.x, coords.y + offset.y);
	}
}