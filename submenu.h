#ifndef SUBMENU_H
#define SUBMENU_H

#include "input.h"
#include "textures.h"
#include "font.h"
#include "character.h"

#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

class SubMenu {
	public:
		SubMenu(Input *I, Textures *T, Font *F, Character *C, int *CPos);
		~SubMenu();
		void UpdateCoordinates(int NewWidth, int NewHeight);
	protected:
		Input *MInput;
		Font *TwentyPoint;
		Textures *SceneTextures;
		Character *Characters;
		int *CPositions;
		int WindowWidth, WindowHeight;

		const char *JobName(int Job);
		void DrawCursor(float CursorX, float CursorY, bool Blinking);
		void DrawCharacter(int Type, float XPos, float YPos);
		void RenderWindow(int X1, int X2, int Y1, int Y2);
		void BlueBackground();
};

#endif
