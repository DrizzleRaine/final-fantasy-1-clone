#include "mapstate.h"

MapState::MapState() {
	head = 0;
	tail = 0;
}

MapState::~MapState() {
	// cleanup any maps
	while (tail) {
		popMap();
	}
}

void MapState::pushMap(Map *map) {
	// push and initialize the new map
	if (tail) {
		tail->pauseMap();
		tail->setNext(map);
	}
	map->setPrev(tail);
	tail = map;
	tail->init(party, &input, this);
}

void MapState::popMap() {
	// cleanup and pop current map
	if (tail) {
		Map *toPop = tail;
		tail = tail->getPrev();
		if (tail) {
			tail->unpauseMap();
		}

		delete toPop;
		toPop = 0;
	}
}

void MapState::changeMap(Map *map) {
	// pop the current map
	popMap();

	// push the new map
	pushMap(map);
}

MenuState *MapState::enterMenu() {
	MenuState *ms = new MenuState();
	stateManager->pushState(ms);
	return ms;
}

BattleState *MapState::enterBattle(int battleID) {
	// calculate preemptive/ambushed value
	int initiative = (party->getAttribute(Party::FIRST, Character::AGL) +
					  party->getAttribute(Party::FIRST, Character::LCK)) / 8;
	int advantage = rand() % (101 - initiative) + initiative * 2 - 4;

	// enter battle state
	BattleState *bs = new BattleState(battleID, advantage);
	stateManager->pushState(bs);
	return bs;
}

void MapState::update() {
	tail->updateMap();
}

void MapState::render() {
	tail->renderMap(windowWidth, windowHeight);
}

void MapState::pause() {
	if (tail) {
		tail->pauseMap();
	}
}

void MapState::unpause() {
	if (tail) {
		tail->unpauseMap();
	}
}
