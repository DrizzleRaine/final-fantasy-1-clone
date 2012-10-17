#include "input.h"

Input::Input() {
	ResetTab();
	ResetBackspace();
	ResetEnter();
	ResetInput();
	ResetKeys();

	Up = 0;
	Down = 0;
	Left = 0;
	Right = 0;

	if (SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL) == -1) {
		printf("ERROR: Failed to enable key repeat\n");
	}
}

void Input::SetRepeatDelay(int Delay) {
	if (SDL_EnableKeyRepeat(Delay, SDL_DEFAULT_REPEAT_INTERVAL)) {
		printf("ERROR: Failed to enable key repeat\n");
	}
}

void Input::UpdateCoordinates(int NewWidth, int NewHeight) {
	WindowWidth = NewWidth;
	WindowHeight = NewHeight;
}

bool Input::PollInput(SDL_Event &Event) {
	ResetInput();

	Uint8 *KeyState = SDL_GetKeyState(NULL);
	switch (Event.type) {
		case SDL_KEYUP:
			if ((KeyState[SDLK_LALT] || KeyState[SDLK_RALT]) && Event.key.keysym.sym == 'f') {
				// toggle fullscreen
			}
			if (Event.key.keysym.sym == SDLK_UP) {
				Up = 0;
			}
			if (Event.key.keysym.sym == SDLK_DOWN) {
				Down = 0;
			}
			if (Event.key.keysym.sym == SDLK_LEFT) {
				Left = 0;
			}
			if (Event.key.keysym.sym == SDLK_RIGHT) {
				Right = 0;
			}
			break;
		case SDL_KEYDOWN: { // define scope specific to case for int i
			int i = 0;
			for (char ch = ' '; ch <= '~'; ch++) {
				if (Event.key.keysym.sym == ch) {
					if ((KeyState[SDLK_LSHIFT] || KeyState[SDLK_RSHIFT]) && !KeyState[SDLK_SPACE]) { // shift being pressed, but space should still be treated normally
						HandleModifiedKeys(ch); // SHIFT symbol characters
						if (i > 64 && i < 91) { // letter character
							if (KeyState[SDLK_CAPSLOCK]) { // lowercase
								Keys[i] = 1;
							} else { // uppercase
								int UppercaseLetter = i - 32;
								Keys[UppercaseLetter] = 1;
							}
						}
					} else if (KeyState[SDLK_CAPSLOCK] && !KeyState[SDLK_SPACE]) { // caps lock pressed but not shift, space should be treated normally
						if (i > 64 && i < 91) { // uppercase letter character
							int UppercaseLetter = i - 32;
							Keys[UppercaseLetter] = 1;
						} else { // symbol or number
							Keys[i] = 1;
						}
					} else { // no caps and no shift
						Keys[i] = 1;
					}
				}
				i++;
			}
			HandleNumPad(KeyState); // numpad keys
			if (KeyState[SDLK_TAB]) {
				Tab = 1;
			}
			if (KeyState[SDLK_BACKSPACE]) {
				Backspace = 1;
			}
			if (KeyState[SDLK_RETURN]) {
				Enter = 1;
			}
			if (KeyState[SDLK_UP]) {
				Up = 1;
			}
			if (KeyState[SDLK_DOWN]) {
				Down = 1;
			}
			if (KeyState[SDLK_LEFT]) {
				Left = 1;
			}
			if (KeyState[SDLK_RIGHT]) {
				Right = 1;
			}
			break;
		} case SDL_MOUSEBUTTONUP:
			if (Event.button.button == SDL_BUTTON_LEFT) {
				LeftMouseUp[XPos] = Event.button.x;
				LeftMouseUp[YPos] = WindowHeight - Event.button.y; // invert SDL y-axis
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (SDL_GetMouseState(0, 0) &SDL_BUTTON_LMASK) {
				if (SDL_GetTicks() - LeftClickTime < 500) {
					DoubleClickPos[XPos] = Event.button.x;
					DoubleClickPos[YPos] = Event.button.y;
					LeftClickTime = -1;
				} else {
					LeftClickTime = SDL_GetTicks();
				}

				LeftMouseDown[XPos] = Event.button.x;
				LeftMouseDown[YPos] = WindowHeight - Event.button.y; // invert SDL y-axis
			}
			if (SDL_GetMouseState(0, 0) &SDL_BUTTON_MMASK) {
			}
			if (SDL_GetMouseState(0, 0) &SDL_BUTTON_RMASK) {
				RightMouseDown[XPos] = Event.button.x;
				RightMouseDown[YPos] = WindowHeight - Event.button.y; // invert SDL y-axis
			}
			break;
		case SDL_MOUSEMOTION:
			MousePos[XPos] = Event.button.x;
			MousePos[YPos] = Event.button.y;
			break;
	}
	return true;
}

void Input::HandleModifiedKeys(char ch) { // handle SHIFT modified keys
	int ModifiedASCII = -1;
	if (ch == '1' || (ch >= '3' && ch <= '5')) { // either !, #, $, or % symbol
		// luckily the char value is the same as ASCII decimal value for these
		int Converted = ch - '0'; // convert char to int
		ModifiedASCII = Converted;
	} else if (ch == '2') { // @ symbol
		ModifiedASCII = 32;
	} else if (ch == '6') { // ^ symbol
		ModifiedASCII = 62;
	} else if (ch == '7' || ch == '9') { // & or ( symbol
		// the char value is one more than ASCII decimal value for these
		int Converted = (ch - '0') - 1;
		ModifiedASCII = Converted;
	} else if (ch == '8') { // * symbol
		ModifiedASCII = 10;
	} else if (ch == '0') { // ) symbol
		ModifiedASCII = 9;
	} else if (ch == '`') { // ~ symbol
		ModifiedASCII = 94;
	} else if (ch == '-') { // _ symbol
		ModifiedASCII = 63;
	} else if (ch == '=') { // + symbol
		ModifiedASCII = 11;
	} else if (ch >= '[' && ch <= ']') { // {, | or } symbol
		ModifiedASCII = ch;
	} else if (ch == ';') { // : symbol
		ModifiedASCII = 26;
	} else if (ch == '\'') { // " symbol
		ModifiedASCII = 2;
	} else if (ch == ',') { // < symbol
		ModifiedASCII = 28;
	} else if (ch == '.') { // > symbol
		ModifiedASCII = 30;
	} else if (ch == '/') { // ? symbol
		ModifiedASCII = 31;
	}
	if (ModifiedASCII > -1) { 
		Keys[ModifiedASCII] = 1;
	}
}

void Input::HandleNumPad(Uint8 *KeyState) {
	//if (KeyState[SDLK_NUMLOCK]) {
		// toggle numlock
	//}
	if (KeyState[SDLK_KP0]) {
		Keys[16] = 1;
	}
	if (KeyState[SDLK_KP1]) {
		Keys[17] = 1;
	}
	if (KeyState[SDLK_KP2]) {
		Keys[18] = 1;
	}
	if (KeyState[SDLK_KP3]) {
		Keys[19] = 1;
	}
	if (KeyState[SDLK_KP4]) {
		Keys[20] = 1;
	}
	if (KeyState[SDLK_KP5]) {
		Keys[21] = 1;
	}
	if (KeyState[SDLK_KP6]) {
		Keys[22] = 1;
	}
	if (KeyState[SDLK_KP7]) {
		Keys[23] = 1;
	}
	if (KeyState[SDLK_KP8]) {
		Keys[24] = 1;
	}
	if (KeyState[SDLK_KP9]) {
		Keys[25] = 1;
	}
	if (KeyState[SDLK_KP_PERIOD]) { // '.' key on numpad
		Keys[14] = 1;
	}
	if (KeyState[SDLK_KP_DIVIDE]) { // '/' key on numpad
		Keys[15] = 1;
	}
	if (KeyState[SDLK_KP_MULTIPLY]) { // '*' key on numpad
		Keys[10] = 1;
	}
	if (KeyState[SDLK_KP_MINUS]) { // '-' key on numpad
		Keys[13] = 1;
	}
	if (KeyState[SDLK_KP_PLUS]) { // '+' key on numpad
		Keys[11] = 1;
	}
	if (KeyState[SDLK_KP_ENTER] || KeyState[SDLK_KP_EQUALS]) { // enter or equals key on numpad
		Enter = 1;
	}
}

void Input::ResetInput() {
	LeftMouseUp[XPos] = -1;
	LeftMouseUp[YPos] = -1;
	LeftMouseDown[XPos] = -1;
	LeftMouseDown[YPos] = -1;
	RightMouseDown[XPos] = -1;
	RightMouseDown[YPos] = -1;
	DoubleClickPos[XPos] = -1;
	DoubleClickPos[YPos] = -1;
}

void Input::ResetTab() {
	Tab = 0;
}

void Input::ResetBackspace() {
	Backspace = 0;
}

void Input::ResetEnter() {
	Enter = 0;
}

void Input::ResetKeys() {
	for (int i = 0; i < 96; i++) {
		Keys[i] = 0;
	}
}
