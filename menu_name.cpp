#include "menu_name.h"

MenuName::MenuName(Input *I, Textures *T, Font *F, Character *C, int *CPos) : SubMenu(I, T, F, C, CPos) {
	NewName = Characters[CPos[1] / 2].Name;
	if (NewName == "??????") {
		NewName.clear();
	}
}

MenuName::~MenuName() {
}

bool MenuName::NameIsEmpty() {
	bool EmptyName = 1;
	for (unsigned int i = 0; i < NewName.length(); i++) {
		if (NewName[i] != ' ') {
			EmptyName = 0;
		}
	}
	if (NewName.length() == 0 || EmptyName) {
		return 1;
	}
	return 0;
}

int MenuName::Process() {
	if (MInput->Keys[83]) {
		MInput->Keys[83] = 0;
		if (NameIsEmpty()) {
			Characters[CPositions[1] / 2].Name = "??????";
			return 0;
		} 
		NewName = NewName.substr(0, NewName.length() - 1);
	} else if (MInput->Keys[68]) {
		MInput->Keys[68] = 0;
		char Selected;
		if (CPositions[0] < 26) { // uppercase letter
			Selected = CPositions[0] + 65;
		} else if (CPositions[0] < 30) { // space
			Selected = ' ';
		} else if (CPositions[0] < 56) { // lowercase letter
			Selected = CPositions[0] + 67;
		} else if (CPositions[0] < 60) { // space
			Selected = ' ';
		} else { // number
			Selected = CPositions[0] - 12;
		}
		if (NewName.length() < 6) { // add character to end
			NewName.push_back(Selected);
		} else { // replace last character
			NewName = NewName.substr(0, NewName.length() - 1);
			NewName.push_back(Selected);
		}
	} else if (MInput->EnterPressed()) {
		MInput->ResetEnter();
		if (NameIsEmpty()) {
			Characters[CPositions[1] / 2].Name = "??????";
			return 0;
		}
		Characters[CPositions[1] / 2].Name = NewName;
		return 0;
	}

	if (MInput->RightPressed()) {
		if ((CPositions[0] - 9) % 10) {
			CPositions[0]++;
		} else {
			CPositions[0] -= 9;
		}
	} else if (MInput->LeftPressed()) {
		if (!(CPositions[0] % 10)) {
			CPositions[0] += 9;
		} else {
			CPositions[0]--;
		}
	} else if (MInput->UpPressed()) {
		if (CPositions[0] > 9) {
			CPositions[0] -= 10;
		} else {
			CPositions[0] += 60;
		}
	} else if (MInput->DownPressed()) {
		if (CPositions[0] < 60) {
			CPositions[0] += 10;
		} else {
			CPositions[0] -= 60;
		}
	}
	return 1;
}

void MenuName::RenderScene() {
	BlueBackground();
	RenderWindow(-WindowWidth, WindowWidth, WindowHeight, -WindowHeight);
	RenderWindow(-150, WindowWidth, WindowHeight, WindowHeight - 150);
	RenderWindow(-150, WindowWidth, WindowHeight - 150, WindowHeight - 760);
	RenderWindow(-150, WindowWidth, WindowHeight - 760, -WindowHeight);

	TwentyPoint->drawText(-100, WindowHeight - 100, "Enter name.");
	TwentyPoint->drawText(-100, -WindowHeight + 100, "A    Confirm                  B    Erase");
	TwentyPoint->drawText(-100, -WindowHeight + 40, "Start    Done");

	char Row[100][2] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J",
						"K", "L", "M", "N", "O", "P", "Q", "R", "S", "T",
						"U", "V", "W", "X", "Y", "Z", " ", " ", " ", " ",
						"a", "b", "c", "d", "e", "f", "g", "h", "i", "j",
						"k", "l", "m", "n", "o", "p", "q", "r", "s", "t",
						"u", "v", "w", "x", "y", "z", " ", " ", " ", " ",
						"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			TwentyPoint->drawText(-100 + (80*i), WindowHeight - 270 - (70 * j), Row[i + (j * 10)]);
		}
	}

	int LeftXPos = -WindowWidth / 1.25;
	DrawCharacter(Characters[0].Job, LeftXPos, WindowHeight / 1.6 + 90);
	DrawCharacter(Characters[1].Job, LeftXPos, WindowHeight / 4.8 + 90);
	DrawCharacter(Characters[2].Job, LeftXPos, -WindowHeight / 4.8 + 90);
	DrawCharacter(Characters[3].Job, LeftXPos, -WindowHeight / 1.6 + 90);

	int LeftNameX = LeftXPos + 140;
	if (CPositions[1] == 1) {
		TwentyPoint->drawText(LeftNameX, WindowHeight / 1.6 + 18, NewName.c_str());
	} else {
		TwentyPoint->drawText(LeftNameX, WindowHeight / 1.6 + 18, Characters[0].Name.c_str());
	}
	TwentyPoint->drawText(LeftXPos, WindowHeight / 1.6 - 110, JobName(Characters[0].Job));
	if (CPositions[1] == 3) {
		TwentyPoint->drawText(LeftNameX, WindowHeight / 4.8 + 18, NewName.c_str());
	} else {
		TwentyPoint->drawText(LeftNameX, WindowHeight / 4.8 + 18, Characters[1].Name.c_str());
	}
	TwentyPoint->drawText(LeftXPos, WindowHeight / 4.8 - 110, JobName(Characters[1].Job)); 
	if (CPositions[1] == 5) {
		TwentyPoint->drawText(LeftNameX, -WindowHeight / 4.8 + 18, NewName.c_str());
	} else {
		TwentyPoint->drawText(LeftNameX, -WindowHeight / 4.8 + 18, Characters[2].Name.c_str());
	}
	TwentyPoint->drawText(LeftXPos, -WindowHeight / 4.8 - 110, JobName(Characters[2].Job)); 
	if (CPositions[1] == 7) {
		TwentyPoint->drawText(LeftNameX, -WindowHeight / 1.6 + 18, NewName.c_str());
	} else {
		TwentyPoint->drawText(LeftNameX, -WindowHeight / 1.6 + 18, Characters[3].Name.c_str());
	}
	TwentyPoint->drawText(LeftXPos, -WindowHeight / 1.6 - 110, JobName(Characters[3].Job)); 

	float CursorX, CursorY;
	if (CPositions[1] < 2) { // cursor on first char
		CursorY = WindowHeight / 1.6 + 18;
	} else if (CPositions[1] < 4) { // cursor on second char...
		CursorY = WindowHeight / 4.8 + 18;
	} else if (CPositions[1] < 6) {
		CursorY = -WindowHeight / 4.8 + 18;
	} else {
		CursorY = -WindowHeight / 1.6 + 18;
	}
	CursorY += 60;
	CursorX = LeftXPos + 40;
	DrawCursor(CursorX, CursorY, 1);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
	CursorX = -100 + (80 * (CPositions[0] % 10));
	CursorY = WindowHeight - 270 - (70 * (CPositions[0] / 10));
	glColor3f(0.8, 0.8, 0.8);
	glLineWidth(2.0);
	glBegin(GL_LINES);
		glVertex2f(CursorX - 15, CursorY + 65); // left side
		glVertex2f(CursorX - 15, CursorY);
		glVertex2f(CursorX - 15, CursorY);		// bottom
		glVertex2f(CursorX + 50, CursorY);
		glVertex2f(CursorX + 50, CursorY); 		// right
		glVertex2f(CursorX + 50, CursorY + 65);
		glVertex2f(CursorX + 50, CursorY + 65); // top
		glVertex2f(CursorX - 15, CursorY + 65);
	glEnd();	
	glColor3f(1.0f, 1.0f, 1.0f);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
}
