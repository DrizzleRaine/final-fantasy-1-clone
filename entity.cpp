#include "entity.h"

Entity::Entity() {
	twenty.initFont("fonts/VarelaRound-Regular.ttf", 16);
	attributes[LEVEL] = 1;
	for (int i = EXP; i < STATSCOUNT; i++) {
		attributes[i] = 0;
	}

	// no status effects
	statusBits = 0;

	// no text to display
	textTicks = 0;
	textColor = WHITE;

	resetTurn();
	animateTicks = 0;
}

Entity::~Entity() {
}

void Entity::setAttribute(Stats s, int val) {
	if (s >= LEVEL && s < STATSCOUNT) {
		attributes[s] = val;
	}
}

bool Entity::hasStatus(unsigned int status) {
	return (statusBits & (1 << status));
}

bool Entity::setStatus(unsigned int status) {
	if (hasStatus(status)) {
		return 0;	// entity already has status
	}

	// set status bit
	statusBits |= 1 << status;
	return 1;	// status successfully set
}

bool Entity::removeStatus(unsigned int status) {
	if (hasStatus(status)) {
		// clear status bit
		statusBits &= ~(1 << status);
		return 1;	// status successfully removed
	}
	return 0;	// entity does not have status
}

int Entity::addHP(int amount) {
	int excess = 0;
	attributes[HP] += amount;
	if (attributes[HP] > attributes[HPMAX]) {
		// hp exceeds maxhp
		excess = attributes[HP] - attributes[HPMAX];
		attributes[HP] = attributes[HPMAX];
	} else if (attributes[HP] < 0) {
		// hp fell below 0
		excess = attributes[HP];
		attributes[HP] = 0;
	}

	// update KO status
	if (attributes[HP] <= 0) {
		setStatus(KO);
	} else {
		removeStatus(KO);
	}

	// update most recent change
	change = amount - excess;
	if (amount >= 0) {
		// hp was added, green text
		textColor = GREEN;
	} else {
		// hp was lost, white text
		textColor = WHITE;
	}

	// start displaying text
	textTicks = SDL_GetTicks();

	// return amount of hp actually added
	return change;
}

int Entity::addMP(int amount) {
	int excess = 0;
	attributes[MP] += amount;
	if (attributes[MP] > attributes[MPMAX]) {
		// mp exceeds maxmp
		excess = attributes[MP] - attributes[MPMAX];
		attributes[MP] = attributes[MPMAX];
	} else if (attributes[MP] < 0) {
		// mp fell below 0
		excess = attributes[MP];
		attributes[MP] = 0;
	}

	// update most recent change
	change = amount - excess;
	if (amount >= 0) {
		// mp was added, blue text
		textColor = BLUE;
	} else {
		// mp was lost, purple text
		textColor = PURPLE;
	}

	// start displaying text
	textTicks = SDL_GetTicks();

	// return amount of mp actually added
	return change;
}

void Entity::renderHPMPChange() {
	unsigned int tickChange = SDL_GetTicks() - textTicks;
	if (!textTicks || tickChange >= 1000) {
		textTicks = 0;
		return;
	}

	// set font color
	switch(textColor) {
		case RED:
			glColor3f(1.0f, 0.0f, 0.0f);
			break;
		case GREEN:
			glColor3f(0.0f, 1.0f, 0.0f);
			break;
		case BLUE:
			glColor3f(0.0f, 0.0f, 1.0f);
			break;
		case YELLOW:
			glColor3f(1.0f, 1.0f, 0.0f);
			break;
		case PURPLE:
			glColor3f(0.0f, 0.8f, 1.0f);
			break;
		default:
			glColor3f(1.0f, 1.0f, 1.0f);
	}

	
	std::string sChange = std::to_string(change);
	if (sChange[0] == '-') {
		// don't display negative sign
		sChange = sChange.substr(1);
	}

	// draw hp/mp change amount at entity position
	twenty.drawText(renderx + 20, rendery - spriteDim, sChange.c_str());

	// restore text color to white
	glColor3f(1.0f, 1.0f, 1.0f);
}

void Entity::resetTurn() {
	turn.action = NONE;
}

void Entity::setTarget(int target) {
	turn.target = target;
}

void Entity::attack(int target) {
	turn.action = ATTACK;
	turn.target = target;
}
