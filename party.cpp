#include "party.h"

Party::Party() {
	// initialize characters
	for (int i = FIRST; i < SIZE; i++) {
		Characters c = static_cast<Characters>(i);
		Character::Jobs job = static_cast<Character::Jobs>(i);

		// initially party has: warrior, thief, white, black
		characters[c].setJob(job);
	}

	direction = SOUTH;
	stepDelay = 250.0;
	paused = 0;

	std::string textureNames[] = {"img/map_char.tga"};
	textures.createTextures(TEXTURECOUNT, textureNames);

	active = FIRST;
}

Party::~Party() {
}

void Party::setActive(int character) {
	if (character >= FIRST && character < SIZE) {
		active = static_cast<Characters>(character);
	}
}

void Party::setX(int newX) {
	step = 0;
	x = newX;
}

void Party::setY(int newY) {
	step = 0;
	y = newY;
}

float Party::getX(int tileSize) {
	float xStepDistance = 0.0;

	const int stepTime = SDL_GetTicks() - step;
	if (step && stepTime < stepDelay) {
		// if party is taking a step, smooth motion
		if (direction == EAST) {
			xStepDistance = (stepTime / stepDelay) * tileSize;
		} else if (direction == WEST) {
			xStepDistance = -(stepTime / stepDelay) * tileSize;
		}
	} else if (step) {
		stopStep(tileSize);
	}
	return (x + xStepDistance);
}

float Party::getY(int tileSize) {
	float yStepDistance = 0.0;

	const int stepTime = SDL_GetTicks() - step;
	if (step && stepTime < stepDelay) {
		// if party is taking a step, smooth motion
		if (direction == NORTH) {
			yStepDistance = -(stepTime / stepDelay) * tileSize;
		} else if (direction == SOUTH) {
			yStepDistance = (stepTime / stepDelay) * tileSize;
		}
	} else if (step) {
		stopStep(tileSize);
	}
	return (y + yStepDistance);
}

void Party::stopStep(int tileSize) {
	step = 0;	// finish step

	// update party position
	if (direction == NORTH) {
		y -= tileSize;
	} else if (direction == EAST) {
		x += tileSize;
	} else if (direction == SOUTH) {
		y += tileSize;
	} else {
		x -= tileSize;
	}
}

void Party::setDirection(int dir) {
	if (dir < 0 || dir >= DIRECTIONS) {
		return;	// out of bounds
	}
	direction = static_cast<Directions>(dir);
}

void Party::renderParty(int width, int height) {
	int spriteX, spriteY;
	if (direction == NORTH) {
		spriteX = 5;
	} else if (direction == EAST || direction == WEST) {
		// east will just reverse the west sprite
		spriteX = 3;
	} else {	// default to south
		spriteX = 1;
	}

	// if still in first half of taking a step
	if (step && (SDL_GetTicks() - step) < stepDelay / 2) {
		spriteX++;	// using stepping sprite
	}

	Character::Jobs firstJob = characters[FIRST].getJob();
	if (firstJob < 2) {
		spriteY = firstJob;		// warrior or thief
	} else if (firstJob < 4) {
		spriteY = firstJob + 2;	// white or black
	} else {
		spriteY = firstJob - 2;	// monk or red
	}
	spriteY++;

	// width and height of sprite sheet
	const float sheetWidth = 2860.0;
	const float sheetHeight = 1560.0;

	// width of character on sprite sheet
	const float spriteWidth = 130;

	// find coordinates of desired sprite
	GLfloat texCoords[4];
	texCoords[2] = ((spriteX * spriteWidth) / sheetWidth);
	texCoords[3] = ((spriteY * spriteWidth) / sheetHeight);
	texCoords[0] = (((spriteX - 1) * spriteWidth) / sheetWidth);
	texCoords[1] = (((spriteY - 1) * spriteWidth) / sheetHeight);
	if (direction == EAST) {	// reverse the sprite
		GLfloat temp = texCoords[0];
		texCoords[0] = texCoords[2];
		texCoords[2] = temp;
	}

	// half the dimensions the char will be displayed as on map
	int halfCharWidth = width * 0.0902778;	// 0.0902778 just looks right
	int halfCharHeight = height * 0.1354167;// 0.1354167 matches the width

	// draw the map sprite of the first character in the party
	glBindTexture(GL_TEXTURE_2D, textures.getTexture(CHARMAPSPRITES));
	glBegin(GL_QUADS);
		glTexCoord2f(texCoords[0], texCoords[1]);
		glVertex2f(-halfCharWidth, halfCharHeight);
		glTexCoord2f(texCoords[0], texCoords[3]);
		glVertex2f(-halfCharWidth, -halfCharHeight);
		glTexCoord2f(texCoords[2], texCoords[3]);
		glVertex2f(halfCharWidth, -halfCharHeight);
		glTexCoord2f(texCoords[2], texCoords[1]);
		glVertex2f(halfCharWidth, halfCharHeight);
	glEnd();
}

void Party::render(Characters c, int x, int y) {
	if (c >= FIRST && c < SIZE) {
		characters[c].render(x, y);
	}
}

void Party::render(Character::Jobs job, int x, int y) {
	// doesnt matter which character used to render given job
	characters[active].render(job, x, y);
}

void Party::swapCharacters(int c1, int c2) {
	if ((c1 >= FIRST && c1 < SIZE) && (c2 >= FIRST && c2 < SIZE)) {
		Character temp(characters[c1]);
		characters[c1] = characters[c2];
		characters[c2] = temp;

		if (getActive() == c1) {
			setActive(c2);
		} else if (getActive() == c2) {
			setActive(c1);
		}
	}
}

void Party::setName(std::string newName) {
	characters[active].setName(newName);
}

void Party::randomName() {
	characters[active].setRandomName();
}

std::string Party::getName() {
	return characters[active].getName();
}

std::string Party::getName(Characters c) {
	if (c >= FIRST && c < SIZE) {
		return characters[c].getName();
	}
	return "";
}

void Party::setJob(int newJob) {
	characters[active].setJob(static_cast<Character::Jobs>(newJob));
}

int Party::getJobInt(Characters c) {
	if (c >= FIRST && c < SIZE) {
		return characters[c].getJob();
	}
	return -1;
}

std::string Party::getJob(Characters c) {
	if (c >= FIRST && c < SIZE) {
		return characters[c].getJobName();
	}
	return "";
}

std::string Party::getJob(Character::Jobs job) {
	// again, not dependent on any character...
	return characters[active].getJobName(job);
}

bool Party::emptyName(Characters c) {
	if (c >= FIRST && c < SIZE && characters[c].getName() == "??????") {
		return 1;
	}
	return 0;
}

void Party::initialize() {
	ticksPlayed = 0;
	ticksUpdate = SDL_GetTicks();
	for (int i = FIRST; i < SIZE; i++) {
		characters[static_cast<Characters>(i)].initStats();
	}
	gil = 500;
	for (int i = 0; i < INVENTORY_SIZE; i++) {
		items[i] = -1;
		itemCount[i] = 0;
	}
}

int Party::getAttribute(Characters c, Character::Stats s) {
	if (c >= FIRST && c < SIZE) {
		return characters[c].getAttribute(s);
	}
	return 0;
}

std::string Party::getHPFraction(Characters c) {
	if (c >= FIRST && c < SIZE) {
		return characters[c].getHPFraction();
	}
	return "";
}

std::string Party::getMPFraction(Characters c) {
	if (c >= FIRST && c < SIZE) {
		return characters[c].getMPFraction();
	}
	return "";
}

int Party::addHP(Characters c, int amount) {
	if (c >= FIRST && c < SIZE) {
		return characters[c].addHP(amount);
	}
	return 0;
}

int Party::addMP(Characters c, int amount) {
	if (c >= FIRST && c < SIZE) {
		return characters[c].addMP(amount);
	}
	return 0;
}

void Party::renderHPMPChange(Characters c) {
	if (c >= FIRST && c < SIZE) {
		characters[c].renderHPMPChange();
	}
}

bool Party::renderingHPMPChange(Characters c) {
	if (c >= FIRST && c < SIZE) {
		return characters[c].renderingHPMPChange();
	}
	return false;
}

bool Party::hasStatus(Characters c, unsigned int status) {
	if (c >= FIRST && c < SIZE) {
		return characters[c].hasStatus(status);
	}
	return 0;
}

bool Party::setStatus(Characters c, unsigned int status) {
	if (c >= FIRST && c < SIZE) {
		return characters[c].setStatus(status);
	}
	return 0;
}

bool Party::removeStatus(Characters c, unsigned int status) {
	if (c >= FIRST && c < SIZE) {
		return characters[c].removeStatus(status);
	}
	return 0;
}

int Party::expToNext(Characters c) {
	if (c >= FIRST && c < SIZE) {
		return characters[c].expToNext();
	}
	return 0;
}

int Party::getSpell(Characters c, int level, int slot) {
	if (c >= FIRST && c < SIZE) {
		return characters[c].getSpell(level, slot);
	}
	return 0;
}

bool Party::addSpell(Characters c, int id, int level) {
	if (c >= FIRST && c < SIZE) {
		return characters[c].addSpell(id, level);
	}
	return 0;
}

bool Party::removeSpell(Characters c, int level, int slot) {
	if (c >= FIRST && c < SIZE) {
		return characters[c].removeSpell(level, slot);
	}
	return 0;
}

bool Party::hasSpell(Characters c, int id) {
	if (c >= FIRST && c < SIZE) {
		return characters[c].hasSpell(id);
	}
	return 0;
}

int Party::getEquip(Characters c, int slot) {
	if (c >= FIRST && c < SIZE) {
		return characters[c].getEquip(slot);
	}
	return 0;
}

bool Party::canEquip(Characters c, int type) {
	if (c >= FIRST && c < SIZE) {
		return characters[c].canEquip(type);
	}
	return 0;
}

void Party::setEquip(Characters c, int slot, int id, int vals[4]) {
	if (c >= FIRST && c < SIZE) {
		characters[c].setEquip(slot, id, vals);
	}
}

void Party::removeEquip(Characters c, int slot) {
	if (c >= FIRST && c < SIZE) {
		characters[c].removeEquip(slot);
	}
}

void Party::setTurn(Characters c, Character::Turn t) {
	if (c >= FIRST && c < SIZE) {
		characters[c].setTurn(t);
	}
}

void Party::resetTurn(Characters c) {
	if (c >= FIRST && c < SIZE) {
		characters[c].resetTurn();
	}
}

Character::Turn Party::getTurn(Characters c) {
	if (c >= FIRST && c < SIZE) {
		return characters[c].getTurn();
	}
	Character::Turn t;
	t.action = Character::Actions::NONE;
	t.actionID = -1;
	t.target = -1;
	return t;
}

void Party::setTarget(Characters c, int target) {
	if (c >= FIRST && c < SIZE) {
		characters[c].setTarget(target);
	}
}

int Party::act(Characters c) {
	if (c >= FIRST && c < SIZE) {
		return characters[c].act();
	}
	return 0;
}

void Party::stepCharForward(Characters c) {
	if (c >= FIRST && c < SIZE) {
		characters[c].stepForward();
	}
}

void Party::stepCharBackward(Characters c) {
	if (c >= FIRST && c < SIZE) {
		characters[c].stepBackward();
	}
}

bool Party::charForward(Characters c) {
	if (c >= FIRST && c < SIZE) {
		return characters[c].forward();
	}
	return false;
}

bool Party::charStepping(Characters c) {
	if (c >= FIRST && c < SIZE) {
		return characters[c].stepping();
	}
	return false;
}

void Party::addExp(Characters c, int exp) {
	if (c >= FIRST && c < SIZE) {
		characters[c].addExp(exp);
	}
}

int Party::getItemCount(int id) {
	if (id < 0 || id >= INVENTORY_SIZE) {
		return 0;
	}
	return itemCount[id];
}

void Party::addItem(int id, int amount) {
	if (id < 0 || id >= INVENTORY_SIZE) {
		return;
	}

	// update item count
	itemCount[id] += amount;
	
	if (itemCount[id] == amount) {
		// added item party did not previously have
		for (int i = 0; i < INVENTORY_SIZE; i++) {
			if (items[i] == -1) {
				// new item goes in first empty slot
				items[i] = id;
				return;
			}
		}
	} else if (itemCount[id] <= 0) {
		itemCount[id] = 0;

		// party no longer has any of the item
		for (int i = 0; i < INVENTORY_SIZE; i++) {
			if (items[i] == id) {
				// remove the item from inentory slot
				items[i] = -1;
				return;
			}
		}
	}
}

void Party::swapItems(int pos1, int pos2) {
	int temp = items[pos1];
	items[pos1] = items[pos2];
	items[pos2] = temp;
}

void Party::sortItems() {
	// sort items by ID
	for (int i = 0, j = 0; i < INVENTORY_SIZE; i++) {
		items[i] = -1;		// clear inventory
		if (itemCount[i]) {	// party has this item
			items[j] = i;	// put it in current slot
			j++;			// increment current slot
		}
	}
}

std::string Party::getTime() {
	// instead of letting clock roll over, stop it at 99 hours
	if (ticksPlayed >= 356400000) {
		return "99:00";
	}

	// get current ticks
	unsigned int ticksNow = SDL_GetTicks();

	// update time played
	ticksPlayed += ticksNow - ticksUpdate;

	// store last time updated
	ticksUpdate = ticksNow;

	// conver ticks played to hours and minutes played
	unsigned int hours = ticksPlayed / 1000 / 60 / 60;
	unsigned int minutes = (ticksPlayed / 1000 / 60) % 60;

	// convert hours and minutes to string
	std::string hrstr = std::to_string(hours);
	std::string minstr = std::to_string(minutes);

	// represented with two digits
	if (minutes < 10) {
		minstr = '0' + minstr;
	}
	if (hours < 10) {
		hrstr = '0' + hrstr;
	}

	// return in time format HH:MM
	return (hrstr + ':' + minstr);
}
