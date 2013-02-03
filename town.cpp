#include "town.h"
#include "world.h"

Town::Town(int mapID) {
	this->mapID = mapID;

	std::ifstream in;
	in.open("map_data/maps");
	for (int i = 0; i < mapID; i++) {
		in >> mapName;
	}
	in.close();
}

Town::~Town() {
}

void Town::init() {
	// initialize npcs
	npcs.init(mapName, tileSize, party, &dialog);

	// display town name for 2 seconds
	dialog.push(mapName, 2000);
}

void Town::update() {
	int xPos = party->getX() / tileSize;
	int yPos = party->getY() / tileSize;
	if (!party->stepping()) {	// if not already taking a step
		bool dirSet = 0;		// was party direction set
		bool blocked = 0;		// is party blocked by tile
		int npcBlock = -1;		// is npc blocking the party

		// if direction pressed, and party isnt paused in dialog, step
		if (input->upPressed() && !party->isPaused()) {
			// set new party direction
			party->setDirection(Party::NORTH);
			dirSet = 1;

			// check if blocked
			blocked = blockedTile(xPos, yPos - 1);
			npcBlock = npcs.exists(xPos, yPos - 1);
		} else if (input->downPressed() && !party->isPaused()) {
			// set new party direction
			party->setDirection(Party::SOUTH);
			dirSet = 1;

			// check if blocked
			blocked = blockedTile(xPos, yPos + 1);
			npcBlock = npcs.exists(xPos, yPos + 1);
		} else if (input->rightPressed() && !party->isPaused()) {
			// set new party direction
			party->setDirection(Party::EAST);
			dirSet = 1;

			// check if blocked
			blocked = blockedTile(xPos + 1, yPos);
			npcBlock = npcs.exists(xPos + 1, yPos);
		} else if (input->leftPressed() && !party->isPaused()) {
			// set new party direction
			party->setDirection(Party::WEST);
			dirSet = 1;

			// check if blocked
			blocked = blockedTile(xPos - 1, yPos);
			npcBlock = npcs.exists(xPos - 1, yPos);
		}

		if (dirSet) {						// player wants to move
			if (!blocked && npcBlock < 0) {	// not blocked by anything
				party->startStep();			// start step in new direction
			} else if (!blocked) {			// blocked by npc
				npcs.doubleSpeed(npcBlock);	// move npc faster
			}
		}

		if (npcBlock < 0) {					// not blocked by any npc
			npcs.defaultSpeed();			// normal speed
		}

		if (input->getCancel()) {			// if cancel button down
			party->setStepDelay(125.0);		// run
		} else {							// else
			party->setStepDelay(250.0);		// walk
		}

		if (input->getConfirm()) {			// attempt to interact
			input->resetConfirm();			// reset confirm button
			input->setRepeatDelay();		// set to default for interactions
			
			// get tile in front of party
			int frontX = party->getX() / tileSize;
			int frontY = party->getY() / tileSize;
			if (party->getDirection() == Party::NORTH) {
				frontY--;
			} else if (party->getDirection() == Party::EAST) {
				frontX++;
			} else if (party->getDirection() == Party::SOUTH) {
				frontY++;
			} else if (party->getDirection() == Party::WEST) {
				frontX--;
			}

			// interact with npc in front of party (if there is one)
			npcs.interact(frontX, frontY);
			input->setRepeatDelay(1);		// remove delay for walking
		}
	}

	// check if current tile is a map change tile
	int currentTile = tiles[xPos][yPos];
	if (currentTile > 0) {
		if (currentTile > mapID) {
			// entering sub map (house, store, etc...)
			mapState->pushMap(new Town(currentTile));
		} else {
			// exiting sub map (back out to town, world map, etc...)
			mapState->popMap();
		}
		return;	// done with this map
	} else {
		currentX = party->getX();
		currentY = party->getY();
	}

	// update npcs
	npcs.update(tiles);
	
	// update dialogs
	dialog.update();
}

void Town::render() {
	// render npcs
	npcs.render(windowWidth, windowHeight);

	// render dialogs
	dialog.render(windowWidth, windowHeight);
}

void Town::pause() {
	npcs.pause();
	dialog.pause();
}

void Town::unpause() {
	npcs.unpause();
	dialog.unpause();
}
