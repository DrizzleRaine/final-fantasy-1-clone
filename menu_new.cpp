#include "menu_new.h"

MenuNew::MenuNew(Input *I, Textures *T, Font *F, Character *C, int *CPos) : SubMenu(I, T, F, C, CPos) {
	StartConfirm = 0;
	StartError = 0;

	// initialize the possible random names
	std::string Names[4][10] = {{"Doma", "Leo", "Giott", "Baron", "Banon", "Argus", "Desh", "Sasuun", "Arus", "Zeza"}, 
							{"Wedge", "Blank", "Kelga", "Daryl", "Tuss", "Sarisa", "Biggs", "Gerad", "Baku", "Puck"},
							{"Ruby", "Elia", "Luca", "Mikoto", "Sherko", "Kuku", "Noah", "Jenica", "Sarina", "Sara"},
							{"Stella", "Kokkol", "Homac", "Koko", "Zok", "Topapa", "Nina", "Delila", "Gungho", "Gilles"}};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 10; j++) {
			RandomNames[i][j] = Names[i][j];
		}
	}
}

MenuNew::~MenuNew() {
}

int MenuNew::Process() {
	if (MInput->Keys[83]) {
		if (StartConfirm) {
			CPositions[0] = CPositions[1];
			StartConfirm = 0;
		} else if (!StartError) {
			MInput->Keys[83] = 0;
			return 0;
		}
		StartError = 0;
	} else if (MInput->Keys[68]) {
		if (StartConfirm) {
			if (CPositions[0] == 0) {
				return 3;
			} else {
				CPositions[0] = CPositions[1];
				StartConfirm = 0;
			}
		} else if (!StartError) {
			MInput->Keys[68] = 0;
			return 2;
		}
		StartError = 0;
	} else if (MInput->Keys[0]) { // random name TODO: srand()? do i need it here?
		if (!StartError) {
			int CIndex = CPositions[0] / 2;
			Characters[CIndex].Name = RandomNames[CIndex][rand() % 10];
		}
		StartError = 0;
	} else if (MInput->EnterPressed()) {
		bool AllNamed = 1;
		for (int i = 0; i < 4; i++) {
			if (Characters[i].Name == "??????") {
				AllNamed = 0;
				break;
			}
		}
		if (AllNamed && !StartConfirm) {
			CPositions[1] = CPositions[0];
			CPositions[0] = 0;
			StartConfirm = 1;
		} else if (!AllNamed && !StartError) {
			StartError = SDL_GetTicks();
		} else {
			StartError = 0;
		}
	}
	if (MInput->RightPressed() || MInput->LeftPressed()) {
		if (CPositions[0] % 2 && !StartError) {	// cursor currently on a name
			CPositions[0]--;		// move it to the job
		} else if (!StartError) { 	// else cursor currently on a job
			CPositions[0]++; 		// move it to the name
		}
		StartError = 0;
	}
	if (MInput->UpPressed()) {
		if (StartConfirm) {
			if (CPositions[0] == 0) {
				CPositions[0] = 1;
			} else {
				CPositions[0] = 0;
			}
		} else if (!StartError) {
			CPositions[0] -= 2;
		}
		StartError = 0;
	}
	if (MInput->DownPressed()) {
		if (StartConfirm) {
			if (CPositions[0] == 0) {
				CPositions[0] = 1;
			} else {
				CPositions[0] = 0;
			}
		} else if (!StartError) {
			CPositions[0] += 2;
		}
		StartError = 0;
	}

	if (CPositions[0] == 9) {
		CPositions[0] = 1;
	} else if (CPositions[0] == -1) {
		CPositions[0] = 7;
	}
	if (CPositions[0] > 7) {
		CPositions[0] = 0;
	} else if (CPositions[0] < 0) {
		CPositions[0] = 6;
	}
	return 1;
}

void MenuNew::RenderScene() {
	BlueBackground();
	RenderWindow(-WindowWidth, WindowWidth, WindowHeight, -WindowHeight);
	RenderWindow(WindowWidth - 480, WindowWidth, WindowHeight, WindowHeight - 150);
	if (StartConfirm) {
		RenderWindow(WindowWidth - 480, WindowWidth, WindowHeight - 150, WindowHeight - 480);
		RenderWindow(WindowWidth - 480, WindowWidth, WindowHeight - 480, WindowHeight - 750);
		RenderWindow(WindowWidth - 480, WindowWidth, WindowHeight - 750, -WindowHeight);
	} else {
		RenderWindow(WindowWidth - 480, WindowWidth, WindowHeight - 150, WindowHeight - 530);
		RenderWindow(WindowWidth - 480, WindowWidth, WindowHeight - 530, -WindowHeight);
	}
	int LeftXPos = -WindowWidth / 1.25;
	DrawCharacter(Characters[0].Job, LeftXPos, WindowHeight / 1.6 + 72);
	DrawCharacter(Characters[1].Job, LeftXPos, WindowHeight / 4.8 + 72);
	DrawCharacter(Characters[2].Job, LeftXPos, -WindowHeight / 4.8 + 72);
	DrawCharacter(Characters[3].Job, LeftXPos, -WindowHeight / 1.6 + 72);

	float CursorX, CursorY;
	const float CCursorY[4] = {static_cast<float>(WindowHeight / 1.6), static_cast<float>(WindowHeight / 4.8), static_cast<float>(-WindowHeight / 4.8), static_cast<float>(-WindowHeight / 1.6)}; // static_cast is there to get rid of compiler warning about converting from double to float
	if (StartError && SDL_GetTicks() - StartError < 1000) {
		CursorX = LeftXPos - 100;
		for (int i = 0; i < 4; i++) {
			if (Characters[i].Name == "??????") {
				DrawCursor(CursorX, CCursorY[i], 1);
			}
		}
	} else if (StartConfirm) {
		CursorX = WindowWidth - 370;
		if (CPositions[0]) {
			CursorY = WindowHeight - 610;
		} else {
			CursorY = WindowHeight - 540;
		}
		DrawCursor(CursorX, CursorY, 0);
	} else {
		StartError = 0;
		if (CPositions[0] % 2) { // cursor is on a name
			CursorX = LeftXPos + 120;
			CursorY = CCursorY[CPositions[0] / 2] + 70;
		} else { // cursor on job
			CursorX = LeftXPos - 100;
			CursorY = CCursorY[CPositions[0] / 2] + 30;
		}
		DrawCursor(CursorX, CursorY, 0);
	}
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);

	LeftXPos = -WindowWidth / 2.0;
	TwentyPoint->drawText(LeftXPos, CCursorY[0], Characters[0].Name.c_str());
	TwentyPoint->drawText(LeftXPos, CCursorY[0] - 80, JobName(Characters[0].Job)); 
	TwentyPoint->drawText(LeftXPos, CCursorY[1], Characters[1].Name.c_str());
	TwentyPoint->drawText(LeftXPos, CCursorY[1] - 80, JobName(Characters[1].Job)); 
	TwentyPoint->drawText(LeftXPos, CCursorY[2], Characters[2].Name.c_str());
	TwentyPoint->drawText(LeftXPos, CCursorY[2] - 80, JobName(Characters[2].Job)); 
	TwentyPoint->drawText(LeftXPos, CCursorY[3], Characters[3].Name.c_str());
	TwentyPoint->drawText(LeftXPos, CCursorY[3] - 80, JobName(Characters[3].Job)); 

	TwentyPoint->drawText(WindowWidth - 380, WindowHeight - 100, "New Game");
	if (StartConfirm) {
		TwentyPoint->drawText(WindowWidth - 430, WindowHeight - 300, "Begin game\nwith this party?");
		TwentyPoint->drawText(WindowWidth - 250, WindowHeight - 600, "Yes");
		TwentyPoint->drawText(WindowWidth - 250, WindowHeight - 670, "No");
		TwentyPoint->drawText(WindowWidth - 430, WindowHeight - 850, "A      Confirm");
		TwentyPoint->drawText(WindowWidth - 430, WindowHeight - 920, "B      Back");
	} else if (StartError) {
		TwentyPoint->drawText(WindowWidth - 430, WindowHeight - 300, "You must name\nall characters.");
		TwentyPoint->drawText(WindowWidth - 430, WindowHeight - 630, "A      Confirm");
		TwentyPoint->drawText(WindowWidth - 432, WindowHeight - 680, "B      Back");
		TwentyPoint->drawText(WindowWidth - 430, WindowHeight - 740, "Start\n    Done");
		TwentyPoint->drawText(WindowWidth - 430, WindowHeight - 860, "Select\n    Auto-Name");
	} else {
		TwentyPoint->drawText(WindowWidth - 430, WindowHeight - 300, "Assign each\ncharacter's\nname and class.");
		TwentyPoint->drawText(WindowWidth - 430, WindowHeight - 630, "A      Confirm");
		TwentyPoint->drawText(WindowWidth - 432, WindowHeight - 680, "B      Back");
		TwentyPoint->drawText(WindowWidth - 430, WindowHeight - 740, "Start\n    Done");
		TwentyPoint->drawText(WindowWidth - 430, WindowHeight - 860, "Select\n    Auto-Name");
	}
}
