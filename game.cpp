#include "game.h"
#include "titlestate.h"

void Game::init() {
	// push the initial state
	stateManager.pushState(new TitleState());
}

void Game::cleanup() {
	stateManager.popAll();
}

void Game::play() {
	SDL_Event event;
	while (1) {	// loop forever until SDL_QUIT event
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:			// quit event received
					return;				// stop playing
				case SDL_VIDEORESIZE:	// video resize event received
					window.resize(event.resize.w, event.resize.h);
					break;
			}

			// process the other events
			stateManager.process(event);
		}

		// update the current state
		stateManager.update();

		// render the current state
		stateManager.render(window.getWidth(), window.getHeight());

		SDL_Delay(1);	// dont need all that cpu time
	}
}
