#include "menu.h"

Menu::Menu(Input *I, Character *C) {
	const char* TextureArray[] = {"img/menu_nw_corner.tga", "img/menu_nw_edge.tga", "img/menu_ne_corner.tga", "img/menu_se_edge.tga", "img/menu_se_corner.tga", "img/menu_sw_corner.tga", "img/cursor.tga", "img/warrior_menu.tga", "img/thief_menu.tga", "img/white_menu.tga", "img/black_menu.tga", "img/monk_menu.tga", "img/red_menu.tga"};
	MTextures = new Textures(13, TextureArray);
	TwentyPoint = new Font("fonts/VarelaRound-Regular.ttf", 20);

	MInput = I;
	MInput->SetRepeatDelay(); // set to default repeat delay
	Characters = C;
	MNew = new MenuNew(MInput, MTextures, TwentyPoint, Characters, CursorPosition);
	CursorPosition[0] = 0;
	CurrentMenu = NewGame;
}

Menu::~Menu() {
	delete MNew;
	MNew = 0;

	delete MTextures;
	MTextures = 0;

	delete TwentyPoint;
	TwentyPoint = 0;
}

void Menu::Update() {
	if (CurrentMenu == NewGame) {
		MNew->RenderScene();
	} else if (CurrentMenu == JobChoose) {
		MJob->RenderScene();
	} else if (CurrentMenu == NameChar) {
		MName->RenderScene();
	}
	SDL_GL_SwapBuffers();
}

int Menu::Process() {
	if (CurrentMenu == NewGame) {
		int Status = MNew->Process();
		if (!Status) {
			return 0; // exit back to title screen
		} else if (Status == 2) { // option selected
			CursorPosition[1] = CursorPosition[0];
			CursorPosition[0] = 0;
			if (CursorPosition[1] % 2) { // cursor selected name
				MName = new MenuName(MInput, MTextures, TwentyPoint, Characters, CursorPosition);
				MName->UpdateCoordinates(WindowWidth, WindowHeight);
				CurrentMenu = NameChar;
			} else { // cursor selected job
				MJob = new MenuJob(MInput, MTextures, TwentyPoint, Characters, CursorPosition);
				MJob->UpdateCoordinates(WindowWidth, WindowHeight);
				CurrentMenu = JobChoose;
			}
			delete MNew;
			MNew = 0;
		} else if (Status == 3) {
			return 2; // exit menus, change game state to WorldS
		}
	} else if (CurrentMenu == JobChoose) {
		if (!MJob->Process()) {
			CursorPosition[0] = CursorPosition[1];
			MNew = new MenuNew(MInput, MTextures, TwentyPoint, Characters, CursorPosition);
			MNew->UpdateCoordinates(WindowWidth, WindowHeight);
			CurrentMenu = NewGame;
			delete MJob;
			MJob = 0;
		}
	} else if (CurrentMenu == NameChar) {
		if (!MName->Process()) {
			CursorPosition[0] = CursorPosition[1];
			MNew = new MenuNew(MInput, MTextures, TwentyPoint, Characters, CursorPosition);
			MNew->UpdateCoordinates(WindowWidth, WindowHeight);
			CurrentMenu = NewGame;
			delete MName;
			MName = 0;
		}
	}
	return 1;
}

void Menu::UpdateCoordinates(int NewWidth, int NewHeight) {
	WindowWidth = NewWidth;
	WindowHeight = NewHeight;
	if (CurrentMenu == NewGame) {
		MNew->UpdateCoordinates(NewWidth, NewHeight);
	} else if (CurrentMenu == JobChoose) {
		MJob->UpdateCoordinates(NewWidth, NewHeight);
	} else if (CurrentMenu == NameChar) {
		MName->UpdateCoordinates(NewWidth, NewHeight);
	}
}
