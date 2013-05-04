#include "statemanager.h"
#include "world.h"

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
		tail->pauseState();
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
		if (tail) {
			tail->unpauseState();
		}

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
	// reset scene
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	// render state
	tail->renderState(width, height);

	// swap buffers
	SDL_GL_SwapBuffers();
}

void StateManager::startGame() {
	popAll();	// pop all current states

	// put party on world map
	MapState *ms = new MapState();
	pushState(ms);
	ms->pushMap(new World());
}
