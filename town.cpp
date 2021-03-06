#include "town.h"
#include "world.h"

#include "itemshop.h"
#include "spellshop.h"

Town::Town(int mapID) {
	this->mapID = mapID;

	std::ifstream in;
	in.open("map_data/maps");
	for (int i = 0; i < mapID; i++) {
		in >> region;
		in >> mapFile;
	}
	in.close();
}

Town::~Town() {
}

void Town::init() {
	// initialize npcs
	npcs.init(mapFile, tileSize, party, &dialog);

	if (mapFile.find('_') == std::string::npos) {
		// if region is not a sub region (e.g. Cornelia_Inn inside Cornelia)
		dialog.push(region, 2000);	// display region name for 2 seconds
	}
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

			// find out which, if any, npc is in front of character
			int npcInFront = npcs.exists(frontX, frontY);
			if (npcInFront > -1) {
				NPC::ShopTypes shopType = npcs.getShopType(npcInFront);
				if (shopType > NPC::NONE && shopType < NPC::COUNT) {
					// the npc is a shop, enter it
					ShopMenu *sm;
					if (shopType == NPC::ITEM) {
						sm = new ItemShop("Items", mapFile);
					} else if (shopType == NPC::WEAPON) {
						sm = new ItemShop("Weapons", mapFile);
					} else if (shopType == NPC::ARMOR) {
						sm = new ItemShop("Armor", mapFile);
					} else if (shopType == NPC::WHITE) {
						sm = new SpellShop("W. Magic", mapFile);
					} else if (shopType == NPC::BLACK) {
						sm = new SpellShop("B. Magic", mapFile);
					}
					mapState->enterMenu()->pushMenu(sm);
					return;
				} else {
					// set to default for interactions
					input->setRepeatDelay();

					// npc isnt a shop, interact with it
					npcs.interact(npcInFront);

					// remove delay for walking
					input->setRepeatDelay(1);
				}
			}
		}
	}

	// check if current tile is a map change tile
	int currentTile = tiles[xPos][yPos];
	if (currentTile > 0) {
		// return to regular walk speed
		party->setStepDelay(250.0);
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
