#include "input.h"

Input::Input() {
	if (SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL) == -1) {
		printf("ERROR: Failed to enable key repeat\n");
	}

	// game controls configuration
	confirm = &keys[68];	// 'd'
	cancel = &keys[83];		// 's'
	start = &enter;			// enter
	select = &keys[0];		// space
	lButton = &keys[81];	// 'q'
	rButton = &keys[69];	// 'e'

	resetAll();
}

void Input::setRepeatDelay(int delay) {
	if (SDL_EnableKeyRepeat(delay, SDL_DEFAULT_REPEAT_INTERVAL)) {
		printf("ERROR: Failed to enable key repeat\n");
	}
}

bool Input::pollInput(SDL_Event &event) {
	Uint8 *keyState = SDL_GetKeyState(NULL);
	switch (event.type) {
		case SDL_KEYUP:
			for (char ch = ' '; ch <= '~'; ch++) {
				// i only care about character keys
				if (event.key.keysym.sym == ch) {
					// the first 32 ASCII keys are junk
					keys[ch - 32] = 0;
				}
			}
			if (event.key.keysym.sym == SDLK_TAB) {
				tab = 0;
			}
			if (event.key.keysym.sym == SDLK_BACKSPACE) {
				backspace = 0;
			}
			if (event.key.keysym.sym == SDLK_RETURN) {
				enter = 0;
			}
			if (event.key.keysym.sym == SDLK_UP) {
				up = 0;
			}
			if (event.key.keysym.sym == SDLK_DOWN) {
				down = 0;
			}
			if (event.key.keysym.sym == SDLK_LEFT) {
				left = 0;
			}
			if (event.key.keysym.sym == SDLK_RIGHT) {
				right = 0;
			}
			break;
		case SDL_KEYDOWN:
			for (char ch = ' '; ch <= '~'; ch++) {
				// i only care about character keys
				if (event.key.keysym.sym == ch) {
					// the first 32 ASCII keys are junk
					keys[ch - 32] = 1;
				}
			}
			if (event.key.keysym.sym == SDLK_TAB) {
				tab = 1;
			}
			if (event.key.keysym.sym == SDLK_BACKSPACE) {
				backspace = 1;
			}
			if (event.key.keysym.sym == SDLK_RETURN) {
				enter = 1;
			}
			if (event.key.keysym.sym == SDLK_UP) {
				up = 1;
			}
			if (event.key.keysym.sym == SDLK_DOWN) {
				down = 1;
			}
			if (event.key.keysym.sym == SDLK_LEFT) {
				left = 1;
			}
			if (event.key.keysym.sym == SDLK_RIGHT) {
				right = 1;
			}
			break;
	}
	return true;
}

void Input::resetAll() {
	tab = 0;
	backspace = 0;
	enter = 0;

	up = 0;
	down = 0;
	left = 0;
	right = 0;

	for (int i = 0; i < 96; i++) {
		keys[i] = 0;
	}
}

bool Input::anyKey() {
	for (int i = 0; i < 96; i++) {
		if (keys[i]) {
			return true;
		}
	}
	if (tab || backspace || enter || up || down || left || right) {
		return true;
	}
	return false;
}
