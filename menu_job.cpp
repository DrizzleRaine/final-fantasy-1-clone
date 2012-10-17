#include "menu_job.h"

MenuJob::MenuJob(Input *I, Textures *T, Font *F, Character *C, int *CPos) : SubMenu(I, T, F, C, CPos) {
}

MenuJob::~MenuJob() {
}

int MenuJob::Process() {
	if (MInput->Keys[83]) {
		MInput->Keys[83] = 0;
		return 0;
	} else if (MInput->Keys[68]) {
		MInput->Keys[68] = 0;
		if (CPositions[0] < 2) {
			Characters[CPositions[1] / 2].Job = CPositions[0];
		} else if (CPositions[0] == 2 || CPositions[0] == 4) {
			Characters[CPositions[1] / 2].Job = 4;
		} else if (CPositions[0] == 3 || CPositions[0] == 5) {
			Characters[CPositions[1] / 2].Job = 5;
		} else {
			Characters[CPositions[1] / 2].Job = (CPositions[0] - 4);
		}
		return 0;
	}

	/*  -- selection menu --
	 *  0warrior	1thief
	 *	2monk		3red
	 *	4monk		5red
	 *	6white		7black	
	 *	--------------------*/

	if (MInput->RightPressed()) {
		if (CPositions[0] == 3) {
			CPositions[0] = 6;
		} else if (CPositions[0] < 7) {
			CPositions[0]++;
		}
	} else if (MInput->LeftPressed()) {
		if (CPositions[0] == 4) {
			CPositions[0] = 1;
		} else if (CPositions[0] > 0) {
			CPositions[0]--;
		}
	} else if (MInput->UpPressed()) {
		if (CPositions[0] == 4) {
			CPositions[0] = 0;
		} else if (CPositions[0] == 5) {
			CPositions[0] = 1;
		} else if (CPositions[0] > 1) {
			CPositions[0] -= 2;
		}
	} else if (MInput->DownPressed()) {
		if (CPositions[0] == 2) {
			CPositions[0] = 6;
		} else if (CPositions[0] == 3) {
			CPositions[0] = 7;
		} else if (CPositions[0] < 6) {
			CPositions[0] += 2;
		}
	}
	return 1;
}

void MenuJob::RenderScene() {
	BlueBackground();
	RenderWindow(-WindowWidth, WindowWidth, WindowHeight, -WindowHeight);
	RenderWindow(-150, WindowWidth, WindowHeight, WindowHeight - 150);
	RenderWindow(-150, WindowWidth, WindowHeight - 150, WindowHeight - 650);
	RenderWindow(-150, WindowWidth, WindowHeight - 650, WindowHeight - 840);
	RenderWindow(-150, WindowWidth, WindowHeight - 840, -WindowHeight);

	int LeftXPos = -WindowWidth / 1.25;
	DrawCharacter(Characters[0].Job, LeftXPos, WindowHeight / 1.6 + 90);
	DrawCharacter(Characters[1].Job, LeftXPos, WindowHeight / 4.8 + 90);
	DrawCharacter(Characters[2].Job, LeftXPos, -WindowHeight / 4.8 + 90);
	DrawCharacter(Characters[3].Job, LeftXPos, -WindowHeight / 1.6 + 90);

	int LeftNameX = LeftXPos + 140;
	TwentyPoint->drawText(LeftNameX, WindowHeight / 1.6 + 18, Characters[0].Name.c_str());
	TwentyPoint->drawText(LeftXPos, WindowHeight / 1.6 - 110, JobName(Characters[0].Job));
	TwentyPoint->drawText(LeftNameX, WindowHeight / 4.8 + 18, Characters[1].Name.c_str());
	TwentyPoint->drawText(LeftXPos, WindowHeight / 4.8 - 110, JobName(Characters[1].Job)); 
	TwentyPoint->drawText(LeftNameX, -WindowHeight / 4.8 + 18, Characters[2].Name.c_str());
	TwentyPoint->drawText(LeftXPos, -WindowHeight / 4.8 - 110, JobName(Characters[2].Job)); 
	TwentyPoint->drawText(LeftNameX, -WindowHeight / 1.6 + 18, Characters[3].Name.c_str());
	TwentyPoint->drawText(LeftXPos, -WindowHeight / 1.6 - 110, JobName(Characters[3].Job)); 

	float CursorX, CursorY;
	if (CPositions[1] < 2) { // cursor on first char
		CursorY = WindowHeight / 1.6 + 48;
	} else if (CPositions[1] < 4) { // cursor on second char...
		CursorY = WindowHeight / 4.8 + 48;
	} else if (CPositions[1] < 6) {
		CursorY = -WindowHeight / 4.8 + 48;
	} else {
		CursorY = -WindowHeight / 1.6 + 48;
	}
	CursorX = LeftXPos - 100;
	DrawCursor(CursorX, CursorY, 1);

	if (CPositions[0] % 2) { // cursor in right column
		CursorX = WindowWidth - 400;
	} else { // cursor in left column
		CursorX = -100;
	}
	if (CPositions[0] == 0 || CPositions[0] == 1 || CPositions[0] == 4 || CPositions[0] == 5) { // top row
		CursorY = WindowHeight - 230;
	} else { // bottom row
		CursorY = WindowHeight - 460;
	}
	DrawCursor(CursorX, CursorY, 0);
	
	if (CPositions[0] > 3) {
		DrawCharacter(4, 0, WindowHeight - 190);
		TwentyPoint->drawText(-7, WindowHeight - 390, "Monk");
		DrawCharacter(5, WindowWidth - 300, WindowHeight - 190);
		TwentyPoint->drawText(WindowWidth - 360, WindowHeight - 390, "Red Mage");
		DrawCharacter(2, 0, WindowHeight - 410);
		TwentyPoint->drawText(-80, WindowHeight - 610, "White Mage");
		DrawCharacter(3, WindowWidth - 300, WindowHeight - 410);
		TwentyPoint->drawText(WindowWidth - 375, WindowHeight - 610, "Black Mage");
	} else {
		DrawCharacter(0, 0, WindowHeight - 190);
		TwentyPoint->drawText(-25, WindowHeight - 390, "Warrior");
		DrawCharacter(1, WindowWidth - 300, WindowHeight - 190);
		TwentyPoint->drawText(WindowWidth - 305, WindowHeight - 390, "Thief");
		DrawCharacter(4, 0, WindowHeight - 410);
		TwentyPoint->drawText(-7, WindowHeight - 610, "Monk");
		DrawCharacter(5, WindowWidth - 300, WindowHeight - 410);
		TwentyPoint->drawText(WindowWidth - 360, WindowHeight - 610, "Red Mage");
	}

	TwentyPoint->drawText(-100, WindowHeight - 100, "Select a class.");
	if (CPositions[0] == 1) {
		TwentyPoint->drawText(-100, WindowHeight - 745, "Nimbly runs circles around\nopponents.");
	} else if (CPositions[0] == 2 || CPositions[0] == 4) {
		TwentyPoint->drawText(-100, WindowHeight - 745, "Specializes in bare-knuckled\nattacks.");
	} else if (CPositions[0] == 3 || CPositions[0] == 5) {
		TwentyPoint->drawText(-100, WindowHeight - 745, "Proficient in both black and\nwhite magic.");
	} else if (CPositions[0] == 6) {
		TwentyPoint->drawText(-100, WindowHeight - 745, "Supports the party with white\nmagic.");
	} else if (CPositions[0] == 7) {	
		TwentyPoint->drawText(-100, WindowHeight - 745, "Damages opponents with black\nmagic.");
	} else {
		TwentyPoint->drawText(-100, WindowHeight - 745, "Can use a wide array of weapons\nand armor.");
	}
	TwentyPoint->drawText(-100, -WindowHeight + 30, "A    Confirm                  B    Back");

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
}
