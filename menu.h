#ifndef MENU_H
#define MENU_H

#include "input.h"
#include "character.h"
#include "menu_new.h"
#include "menu_job.h"
#include "menu_name.h"

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

class Menu {
	public:
		Menu(Input *I, Character *C);
		~Menu();
		void Update();
		int Process();
		void UpdateCoordinates(int NewWidth, int NewHeight);
	private:
		Textures *MTextures;
		Font *TwentyPoint;
		Input *MInput;
		MenuNew *MNew;
		MenuJob *MJob;
		MenuName *MName;
		Character *Characters;

		int CursorPosition[2]; // active and inactive (blinking) cursors
		int WindowWidth, WindowHeight;

		enum Menus {NewGame, NameChar, JobChoose};
		Menus CurrentMenu;
};

#endif
