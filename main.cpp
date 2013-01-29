#include "game.h"
#include <SDL/SDL.h>

int main() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}

	Game game;
	game.init();
	game.play();
	game.cleanup();

	SDL_Quit();
	return 0;
}
