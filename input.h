#ifndef INPUT_H
#define INPUT_H

#include <SDL/SDL.h>

class Input {
	// this input class has a lot of room for improvement
	// but it works for now
	public:
		Input();
		bool pollInput(SDL_Event &event);

		// set delay between repeat while holding key down
		void setRepeatDelay(int delay = SDL_DEFAULT_REPEAT_DELAY);

		// reset input
		void resetAll();

		// key accessors
		bool getConfirm() { return *confirm; }
		bool getCancel() { return *cancel; }
		bool getStart() { return *start; }
		bool getSelect() { return *select; }
		bool getLButton() { return *lButton; }
		bool getRButton() { return *rButton; }

		// individual reset
		void resetConfirm() { *confirm = 0; }
		void resetCancel() { *cancel = 0; }
		void resetStart() { *start = 0; }
		void resetSelect() { *select = 0;}
		void resetLButton() { *lButton = 0; }
		void resetRButton() { *rButton = 0; }

		// directional keys
		bool upPressed() { return up; }
		bool downPressed() { return down; }
		bool leftPressed() { return left; }
		bool rightPressed() { return right; }

		// check if any key was pressed
		bool anyKey();
	private:
		// configuration for game control
		bool *confirm, *cancel, *start, *select, *lButton, *rButton;

		bool keys[96];				// character keys
		bool up, down, left, right; // arrow keys
		bool tab, backspace, enter; // tab, backspace, enter keys
};

#endif
