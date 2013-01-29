#ifndef GAME_H
#define GAME_H

#include "window.h"
#include "statemanager.h"

class Game {
	public:
		// initialize/cleanup
		void init();
		void cleanup();

		// game loop
		void play();
	private:
		// the game window
		Window window;

		// stack of states manager
		StateManager stateManager;
};

#endif
