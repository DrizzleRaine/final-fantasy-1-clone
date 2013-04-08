#include "world.h"
#include "town.h"

World::World() {
	mapID = 1;
	region = "World";	// TODO update based on possible encounters
	mapFile = "World/World";
}

World::~World() {
}

void World::init() {
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
	if (currentTile > 0) {			// if map change tile
		mapState->pushMap(new Town(currentTile));
		return;						// change maps
	} else { 						// else if not a map change tile
		currentX = party->getX();	// store current position
		currentY = party->getY();
	}
}

void World::render() {
}

void World::pause() {
}

void World::unpause() {
}
