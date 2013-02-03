#include "state.h"

State::State() {
	// initialize opengl state
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	party = 0;
	stateManager = 0;

	next = 0;
	prev = 0;
}

State::~State() {
}

void State::init(Party *p, StateManager *sm) {
	// initialize shared resources
	party = p;
	stateManager = sm;
}

void State::processState(SDL_Event &event) {
	// poll/update input
	input.pollInput(event);
}

void State::updateState() {
	update();
}

void State::renderState(int width, int height) {
	// update window dimensions
	windowWidth = width;
	windowHeight = height;

	// reset scene
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	// render scene
	render();

	// swap buffers
	SDL_GL_SwapBuffers();
}

void State::pauseState() {
	pause();
}

void State::unpauseState() {
	unpause();
}
