#include "window.h"
#include "game.h"

int main() {
	Window GameWindow;
	Game GameManager;

	GameManager.UpdateCoordinates(GameWindow.GetWindowWidth(), GameWindow.GetWindowHeight());

	int Playing = 1;
	while (Playing) {
		SDL_Event Event;
		while (SDL_PollEvent(&Event)) {
			switch (Event.type) {
				case SDL_QUIT:
					Playing = 0;
					break;
				case SDL_VIDEORESIZE:
					GameWindow.Resize(Event.resize.w, Event.resize.h);
					GameManager.UpdateCoordinates(Event.resize.w, Event.resize.h);
					break;
			}
			GameManager.Process(Event);
		}
		GameManager.Update();
		SDL_Delay(1); // dont need that much cpu time (yet)
	}

	SDL_Quit();
}
