#include "entity.h"

Entity::Entity() {
	attributes[LEVEL] = 1;
	for (int i = EXP; i < STATSCOUNT; i++) {
		attributes[i] = 0;
	}

	resetTurn();
}

Entity::~Entity() {
}

void Entity::setAttribute(Stats s, int val) {
	if (s >= LEVEL && s < STATSCOUNT) {
		attributes[s] = val;
	}
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

	// calculate how much hp was added
	return (amount - excess);
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

	// calculate how much mp was added
	return (amount - excess);
}

void Entity::resetTurn() {
	turn.action = NONE;
	turn.target = 0;
	turn.actionID = 0;
}

void Entity::setTarget(int target) {
	turn.target = target;
}

void Entity::attack(int target) {
	turn.action = ATTACK;
	turn.target = target;
}
