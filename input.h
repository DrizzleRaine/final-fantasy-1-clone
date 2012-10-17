#ifndef INPUT_H
#define INPUT_H

#include <SDL/SDL.h>

class Input {
	public:
		Input();
		bool PollInput(SDL_Event &Event);
		void ResetInput();
		void ResetTab();
		void ResetBackspace();
		void ResetEnter();
		void ResetKeys();
		void SetRepeatDelay(int Delay = SDL_DEFAULT_REPEAT_DELAY);
		void UpdateCoordinates(int NewWidth, int NewHeight);
		void GetMousePos(int MPos[2]) { MPos[0] = MousePos[0]; MPos[1] = MousePos[1]; }
		void GetLeftMouseDown(int Click[2]) { Click[0] = LeftMouseDown[0]; Click[1] = LeftMouseDown[1]; }
		void GetLeftMouseUp(int Click[2]) { Click[0] = LeftMouseUp[0]; Click[1] = LeftMouseUp[1]; }
		void GetRightMouseDown(int Click[2]) { Click[0] = RightMouseDown[0]; Click[1] = RightMouseDown[1]; }

		bool TabPressed() { return Tab; }
		bool BackspacePressed() { return Backspace; }
		bool EnterPressed() { return Enter; }

		bool UpPressed() { return Up; }
		bool DownPressed() { return Down; }
		bool LeftPressed() { return Left; }
		bool RightPressed() { return Right; }

		bool Keys[96];
	private:
		enum Positions {XPos, YPos};
		int WindowWidth, WindowHeight;
		int LeftMouseDown[2]; // pos left mouse button pressed at
		int LeftMouseUp[2];	// pos left mouse button released at
		int RightMouseDown[2]; // pos right mouse button pressed at
		int DoubleClickPos[2]; // pos left mouse button double clicked at
		unsigned int LeftClickTime; // to detect double click		
		int MousePos[2]; // mouse position
		bool Tab, Backspace, Enter; // tab, backspace, enter pressed
		bool Up, Down, Left, Right; // arrow keys
		unsigned int TabTicks;
		void HandleModifiedKeys(char ch);
		void HandleNumPad(Uint8 *KeyState);
};

#endif
