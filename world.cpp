#include "world.h"
#include "town.h"

World::World() {
	mapID = 1;
	mapName = "World";
}

World::~World() {
}

void World::init() {
	party->restoreWorldPos();
}

void World::update() {
	int xPos = party->getX() / tileSize;
	int yPos = party->getY() / tileSize;
	if (!party->stepping()) {					// if not already taking a step
		if (input->upPressed()) {				// up pressed
			party->setDirection(Party::NORTH);	// face that direction
			if (!blockedTile(xPos, yPos - 1)) {	// north tile isnt blocked
				party->startStep();				// start stepping to it
			}
		} else if (input->downPressed()) {
			party->setDirection(Party::SOUTH);
			if (!blockedTile(xPos, yPos + 1)) {
				party->startStep();
			}
		} else if (input->rightPressed()) {
			party->setDirection(Party::EAST);
			if (!blockedTile(xPos + 1, yPos)) {
				party->startStep();
			}
		} else if (input->leftPressed()) {
			party->setDirection(Party::WEST);
			if (!blockedTile(xPos - 1, yPos)) {
				party->startStep();
			}
		}
	}

	// check if current tile is a map change tile
	int currentTile = tiles[xPos][yPos];
	if (currentTile > 0) {		// if map change tile
		mapState->changeMap(new Town(currentTile + 1));
		return;					// change maps
	} else { 					// else if not a map change tile
		party->storeWorldPos();	// store party pos on world map
	}
}

void World::render() {
}
