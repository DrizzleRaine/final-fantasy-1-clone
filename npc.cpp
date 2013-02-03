#include "npc.h"

NPC::NPC() {
	party = 0;
	dialog = 0;
	npcs = 0;

	npcCount = 0;
}

NPC::~NPC() {
	if (npcs) {
		delete[] npcs;
		npcs = 0;
	}
}

void NPC::init(std::string mapName, int tSize, Party *p, Dialog *d) {
	party = p;
	dialog = d;
	tileSize = tSize;

	// open the npc file for this map
	std::string npcFile = "npc_data/" + mapName + ".npc";
	std::ifstream in;
	in.open(npcFile.c_str());

	// get number of npcs
	in >> npcCount;

	// generate array of npc data	
	npcs = new NPCData[npcCount];

	// array of texture file names
	// there will be <= npcCount textures read in
	std::string textureNames[npcCount];

	// how many textures already found
	int textureCount = 0;

	in.ignore(256, '\n');		// go to the next line
	// fill the array from the file
	for (int i = 0; i < npcCount; i++) {
		// read in this npcs texture file
		std::string curTex = "";
		getline(in, curTex, '\n');

		// process the texture file name
		if (curTex == "") {			// empty texture name means
			npcs[i].textureID = -1;	// no texture for this npc
		} else {					// else check for duplicate textures
			bool textureExists = 0; 
			for (int j = 0; j < textureCount; j++) {
				if (curTex == textureNames[j]) {
					// this npc is using the same texture as another npc
					textureExists = 1;
					npcs[i].textureID = j;
					break;
				}
			}
			if (!textureExists) {
				// no npc so far with this texture, add it
				npcs[i].textureID = textureCount;
				textureNames[textureCount] = curTex;
				textureCount++;
			}
		}

		// read the rest of the npc file
		in >> npcs[i].sheetWidth;
		in >> npcs[i].sheetHeight;
		in >> npcs[i].moves;
		in >> npcs[i].animates;
		in >> npcs[i].stepDelay;
		in >> npcs[i].x;
		in >> npcs[i].y;
		in >> npcs[i].direction;
		in.ignore(256, '\n');	// go to the next line
		getline(in, npcs[i].message, '\n');

		// initialize npc variables
		npcs[i].step = SDL_GetTicks();
		npcs[i].animation = SDL_GetTicks();
		npcs[i].doubleSpeed = 0;
		npcs[i].newDirection = 0;
		npcs[i].blocked = 0;
		npcs[i].xStepDistance = 0.0;
		npcs[i].yStepDistance = 0.0;
	}
	in.close();

	// create textureCount textures, no duplicates
	textures.createTextures(textureCount, textureNames);
}

void NPC::interact(int x, int y) {
	int found = exists(x, y);
	if (found < 0) {
		return;	// no npc to interact with
	}

	// the npc being interacted with
	NPCData *npc = &npcs[found];

	if (npc->moves && npc->newDirection && 
			(SDL_GetTicks() - npc->step) > npc->stepDelay && !npc->blocked) {
		return;	// cant interact while npc is moving
	}

	// if a dialog exists
	if (dialog->exists()) {
		dialog->pop();						// pop it
		if (!dialog->exists()) {			// if it was the last dialog
			party->unpause();				// unpause the party
			npc->step = SDL_GetTicks();		// start walking again
			npc->animation = SDL_GetTicks();// and animating
		}
		return;
	}

	// stop walking
	npc->step = 0;
	npc->animation = 0;

	// face the party
	if (party->getDirection() == Party::NORTH) {		// party north
		npc->direction = Party::SOUTH;					// npc south
	} else if (party->getDirection() == Party::EAST) {	// party east
		npc->direction = Party::WEST;					// npc west, etc..
	} else if (party->getDirection() == Party::SOUTH) {
		npc->direction = Party::NORTH;
	} else if (party->getDirection() == Party::WEST) {
		npc->direction = Party::EAST;
	}

	// pause the party during dialog
	party->pause();

	// push npc dialog
	dialog->push(npc->message);
}

int NPC::exists(int x, int y) {
	return exists(x, y, -1);
}

int NPC::exists(int x, int y, int exception) {
	for (int i = 0; i < npcCount; i++) {
		if (i == exception) {
			continue;	// skip this npc
		}

		int curX = npcs[i].x / tileSize;
		int curY = npcs[i].y / tileSize;

		if (curX == x && curY == y) {
			return i;	// npc exists at (x, y)
		}

		// is the npc moving or ignore movement?
		if (!npcs[i].moves || !npcs[i].newDirection || npcs[i].blocked) {
			continue;
		}

		// exists at tile npc is moving to also
		if (npcs[i].direction == Party::NORTH) {
			curY--;
		} else if (npcs[i].direction == Party::EAST) {
			curX++;
		} else if (npcs[i].direction == Party::SOUTH) {
			curY++;
		} else if (npcs[i].direction == Party::WEST) {
			curX--;
		}

		if (curX == x && curY == y) {
			return i;	// npc is moving to (x, y)
		}
	}
	return -1;
}

void NPC::doubleSpeed(int npcID) {
	if (!npcs[npcID].doubleSpeed && !npcs[npcID].newDirection) {
		npcs[npcID].stepDelay /= 2;			// increase speed
		npcs[npcID].step = SDL_GetTicks();	// reset ticks (prevent jumpy movement)
		npcs[npcID].doubleSpeed = 1;		// set flag
	}
}

void NPC::defaultSpeed() {
	for (int i = 0; i < npcCount; i++) {
		if (npcs[i].doubleSpeed && !npcs[i].newDirection) {
			npcs[i].stepDelay *= 2;			// default speed
			npcs[i].doubleSpeed = 0;		// reset flag
		}
	}
}

void NPC::update(int **tiles) {
	for (int i = 0; i < npcCount; i++) {
		if (!npcs[i].moves || !npcs[i].step) {	// this npc doesnt/isnt moving
			continue;							// go to the next npc
		}

		// steps:
		// 1) stand still for stepDelay ticks
		// 2) pick a direction
		// 3) walk in that direction for stepDelay ticks
		//    or if blocked, face that direction for stepDelay ticks
		// 4) reset

		// time since npc began last step
		unsigned int stepElapsed = SDL_GetTicks() - npcs[i].step;
		if (!npcs[i].newDirection && stepElapsed > npcs[i].stepDelay) {
			// after standing still for stepDelay, pick a direction
			npcs[i].direction = (rand() % Party::DIRECTIONS);
			npcs[i].newDirection = 1;

			// check if chosen direction is blocked
			int curX = npcs[i].x / tileSize;
			int curY = npcs[i].y / tileSize;
			if (npcs[i].direction == Party::NORTH) {
				npcs[i].blocked = tileBlocked(tiles, i, curX, curY - 1);
			} else if (npcs[i].direction == Party::EAST) {
				npcs[i].blocked = tileBlocked(tiles, i, curX + 1, curY);
			} else if (npcs[i].direction == Party::SOUTH) {
				npcs[i].blocked = tileBlocked(tiles, i, curX, curY + 1);
			} else if (npcs[i].direction == Party::WEST) {
				npcs[i].blocked = tileBlocked(tiles, i, curX - 1, curY);
			}
		} else if (npcs[i].newDirection && stepElapsed > npcs[i].stepDelay * 2) {
			// finish step in newDirection
			npcs[i].xStepDistance = 0.0;
			npcs[i].yStepDistance = 0.0;

			if (!npcs[i].blocked) {
				// if npc wasn't blocked, move them to new position
				if (npcs[i].direction == Party::NORTH) {
					npcs[i].y -= tileSize;
				} else if (npcs[i].direction == Party::EAST) {
					npcs[i].x += tileSize;
				} else if (npcs[i].direction == Party::SOUTH) {
					npcs[i].y += tileSize;
				} else if (npcs[i].direction == Party::WEST) {
					npcs[i].x -= tileSize;
				}
			}

			// reset
			npcs[i].blocked = 0;
			npcs[i].newDirection = 0;
			npcs[i].step = SDL_GetTicks();		// start next step
		} else if (npcs[i].newDirection && stepElapsed > npcs[i].stepDelay 
				&& !npcs[i].blocked) {
			// smoothly move in newDirection
			float stepDist = (stepElapsed - npcs[i].stepDelay) / npcs[i].stepDelay;
			if (npcs[i].direction == Party::NORTH) {
				npcs[i].yStepDistance = -stepDist * tileSize;
			} else if (npcs[i].direction == Party::EAST) {
				npcs[i].xStepDistance = stepDist * tileSize;
			} else if (npcs[i].direction == Party::SOUTH) {
				npcs[i].yStepDistance = stepDist * tileSize;
			} else if (npcs[i].direction == Party::WEST) {
				npcs[i].xStepDistance = -stepDist * tileSize;
			}
		}
	}
}

void NPC::render(int windowWidth, int windowHeight) {
	for (int i = 0; i < npcCount; i++) {
		if (npcs[i].textureID < 0) {	// this npc has no sprite
			continue;					// go to the next npc
		}

		// distance from party (center of screen)
		float xDist = (npcs[i].x + npcs[i].xStepDistance) - party->getX(tileSize);
		float yDist = (npcs[i].y + npcs[i].yStepDistance) - party->getY(tileSize);

		// multiplied by (window size / tileSize / # tiles)
		xDist *= windowWidth / tileSize / 7.5;
		yDist *= windowHeight / tileSize / 5.0;

		// sprite coordinates
		int spriteX, spriteY = 1;
		if (npcs[i].direction == Party::NORTH) {
			spriteX = 5;
		} else if (npcs[i].direction == Party::EAST 
				|| npcs[i].direction == Party::WEST) {
			// east will just reverse the west sprite
			spriteX = 3;
		} else {	// default to south
			spriteX = 1;
		}

		// animate npcs
		if (npcs[i].animates && npcs[i].animation) {
			// animation cycle is half a step cycle
			unsigned int animationDelay = npcs[i].stepDelay / 2;
			unsigned int animateElapsed = SDL_GetTicks() - npcs[i].animation;
			if (animateElapsed < animationDelay / 2) {
				// still in first half of animation
				spriteX++;	// next sprite
			} else if (animateElapsed >= animationDelay) {
				// one animation cycle complete, start next
				npcs[i].animation = SDL_GetTicks();
			}
		}

		// sprite sheet coordinates
		GLfloat texCoords[4];
		texCoords[2] = ((spriteX * 100.0) / npcs[i].sheetWidth);
		texCoords[3] = ((spriteY * 100.0) / npcs[i].sheetHeight);
		texCoords[0] = (((spriteX - 1) * 100.0) / npcs[i].sheetWidth);
		texCoords[1] = (((spriteY - 1) * 100.0) / npcs[i].sheetHeight);

		// for east, reverse west sprite
		if (npcs[i].direction == Party::EAST) {
			GLfloat temp = texCoords[0];
			texCoords[0] = texCoords[2];
			texCoords[2] = temp;
		}

		// move to npc location
		glTranslatef(xDist, -yDist, 0.0f);

		// draw the npc
		glBindTexture(GL_TEXTURE_2D, textures.getTexture(npcs[i].textureID));
		glBegin(GL_QUADS);
			glTexCoord2f(texCoords[0], texCoords[1]);
			glVertex2f(-50, 50);
			glTexCoord2f(texCoords[0], texCoords[3]);
			glVertex2f(-50, -50);
			glTexCoord2f(texCoords[2], texCoords[3]);
			glVertex2f(50, -50);
			glTexCoord2f(texCoords[2], texCoords[1]);
			glVertex2f(50, 50);
		glEnd();

		// return to party location
		glLoadIdentity();
	}
}

void NPC::pause() {
	for (int i = 0; i < npcCount; i++) {
		npcs[i].stepPause = SDL_GetTicks() - npcs[i].step;
		npcs[i].animationPause = SDL_GetTicks() - npcs[i].animation;
	}
}

void NPC::unpause() {
	for (int i = 0 ; i < npcCount; i++) {
		npcs[i].step = SDL_GetTicks() - npcs[i].stepPause;
		npcs[i].animation = SDL_GetTicks() - npcs[i].animationPause;
	}
}

bool NPC::tileBlocked(int **tiles, int npc, int x, int y) {
	int partyX = party->getX() / tileSize;
	int partyY = party->getY() / tileSize;
	
	// is the tile currently occupied
	if (tiles[x][y] >= 0 || (partyX == x && partyY == y) || exists(x, y, npc) >= 0) {
		return 1;	// (x, y) tile blocked, or party/npc exists
	}

	// check if party is moving to (x, y)
	if (party->stepping()) {
		if (party->getDirection() == Party::NORTH) {
			partyY--;
		} else if (party->getDirection() == Party::EAST) {
			partyX++;
		} else if (party->getDirection() == Party::SOUTH) {
			partyY++;
		} else if (party->getDirection() == Party::WEST) {
			partyX--;
		}

		if (partyX == x && partyY == y) {
			return 1;	// party is moving to (x, y)
		}
	}
	return 0;
}
