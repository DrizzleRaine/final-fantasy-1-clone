#include "map.h"
#include "partymenu.h"

Map::Map() {
	input = 0;
	party = 0;
	mapState = 0;

	next = 0;
	prev = 0;
	tiles = 0;
}

Map::~Map() {
	if (tiles) {
		int xTiles = mapWidth / tileSize;
		for (int i = 0; i < xTiles; i++) {
			delete[] tiles[i];
			tiles[i] = 0;
		}
		delete[] tiles;
		tiles = 0;
	}

	// set repeat delay back to default
	input->setRepeatDelay();
}

void Map::init(Party *p, Input *in, MapState *ms) {
	// initialize shared resources
	party = p;
	input = in;
	mapState = ms;

	// update parties current map
	party->setMap(mapID);
	party->setRegion(region);

	// generate textures
	std::string textureNames[] = {"map_data/" + mapFile + ".tga"};
	textures.createTextures(COUNT, textureNames);

	// map filename containing map data
	std::string mapData = "map_data/" + mapFile + ".map";

	// load map and tile info
	int direction;
	std::ifstream mapfile;
	mapfile.open(mapData.c_str());
	mapfile >> mapWidth; mapfile >> mapHeight; mapfile >> tileSize;
	mapfile >> currentX; mapfile >> currentY; mapfile >> direction;

	// update parties position
	party->setX(currentX);
	party->setY(currentY);

	// update parties direction
	party->setDirection(direction);

	// how many tiles wide and high the map is
	int xTiles = mapWidth / tileSize;
	int yTiles = mapHeight / tileSize;

	// fill tiles** array with each tile's info from the file
	tiles = new int *[xTiles];
	for (int i = 0; i < xTiles; i++) {
		tiles[i] = new int[yTiles];
	}
	for (int y = 0; y < yTiles; y++) {
		for (int x = 0; x < xTiles; x++) {
			mapfile >> tiles[x][y];
		}
	}
	mapfile.close();

	// initialize to regular walking speed
	party->setStepDelay(250.0);

	// remove repeat delay for walking
	input->setRepeatDelay(1);

	// unique map initializeation
	init();
}

void Map::updateMap() {
	if (input->getStart()) {
		input->resetStart();

		// enter party menu from any
		// map when start button pressed
		mapState->enterMenu()->pushMenu(new PartyMenu());
		return;
	}
	update();
}

void Map::renderMap(int width, int height) {
	// update window dimensions
	windowWidth = width;
	windowHeight = height;

	// render the map
	// player sees 15x10 tiles
	GLfloat texCoords[4];
	texCoords[2] = (party->getX(tileSize) + (tileSize * 7.5)) / mapWidth;
	texCoords[3] = (party->getY(tileSize) + (tileSize * 5.0)) / mapHeight;
	texCoords[0] = (party->getX(tileSize) - (tileSize * 7.5)) / mapWidth;
	texCoords[1] = (party->getY(tileSize) - (tileSize * 5.0)) / mapHeight;
	glBindTexture(GL_TEXTURE_2D, textures.getTexture(MAP));
	glBegin(GL_QUADS);
		glTexCoord2f(texCoords[0], texCoords[1]);
		glVertex2f(-windowWidth, windowHeight);
		glTexCoord2f(texCoords[0], texCoords[3]);
		glVertex2f(-windowWidth, -windowHeight);
		glTexCoord2f(texCoords[2], texCoords[3]);
		glVertex2f(windowWidth, -windowHeight);
		glTexCoord2f(texCoords[2], texCoords[1]);
		glVertex2f(windowWidth, windowHeight);
	glEnd();

	// render scene
	render();

	// render the lead character on top of the map
	party->renderParty(windowWidth, windowHeight);
}

void Map::pauseMap() {
	pause();
}

void Map::unpauseMap() {
	// restore party position
	party->setX(currentX);
	party->setY(currentY);

	// restore map name
	party->setRegion(region);

	unpause();
}

bool Map::blockedTile(int x, int y) {
	if (!tiles) {
		// tiles is uninitialized
		return 0;
	}

	if (!tiles[x][y]) {
		return 1;
	}
	return 0;
}
