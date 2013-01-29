#include "statemanager.h"

StateManager::StateManager() {
	head = 0;
	tail = 0;
}

StateManager::~StateManager() {
	popAll();
}

void StateManager::pushState(State *state) {
	// push and initialize the new state
	if (tail) {
		tail->setNext(state);
	}
	state->setPrev(tail);
	tail = state;
	tail->init(&party, this);
}

void StateManager::popState() {
	// cleanup and pop current state
	if (tail) {
		State *toPop = tail;
		tail = tail->getPrev();

		delete toPop;
		toPop = 0;
	}
}

void StateManager::changeState(State *state) {
	// pop the current state
	popState();

	// push the new state
	pushState(state);
}

void StateManager::popAll() {
	// pop any states
	while (tail) {
		popState();
	}
}

void StateManager::process(SDL_Event &event) {
	tail->processState(event);
}

void StateManager::update() {
	tail->updateState();
}

void StateManager::render(int width, int height) {
	tail->renderState(width, height);
}
