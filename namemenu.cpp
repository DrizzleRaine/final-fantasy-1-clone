#include "namemenu.h"

NameMenu::NameMenu(std::string name) : subCursor(1) {
	// the current cursor starts at 0
	cursor.setSelection(0);

	newName = name;				// edit current name of active character
	if (newName == "??????") {
		newName.clear();		// if all ?, clear it for editing
	}
}

NameMenu::~NameMenu() {
}

void NameMenu::update() {
	const int CURSEL = cursor.getSelection();// save some typing

	// check action keys
	if (input->getCancel()) {				// cancel button
		input->resetCancel();				// handled
		if (!newName.length()) {			// no letters to backspace
			party->setName("??????");		// set name back to all ?
			menuState->popMenu();			// and return to new menu
			return;
		}

		// else delete last letter in newName
		newName = newName.substr(0, newName.length() - 1);
	} else if (input->getConfirm()) {	// confirm button
		// figure out which letter was selected
		char selected;
		if (CURSEL < 26) {					// uppercase letter
			selected = CURSEL + 65;
		} else if (CURSEL < 30) {			// space
			selected = ' ';
		} else if (CURSEL < 56) {			// lowercase letter
			selected = CURSEL + 67;
		} else if (CURSEL < 60) { 			// space
			selected = ' ';
		} else {							// number
			selected = CURSEL - 12;
		}

		if (newName.length() < 6) { 		// append selected letter to end
			newName.push_back(selected);
		} else {	// newName is full, replace last letter of newName with selected
			newName = newName.substr(0, newName.length() - 1);
			newName.push_back(selected);
		}
	} else if (input->getStart()) {			// start button
		input->resetStart();

		// first check if newName is empty
		bool empty = 1;
		unsigned int newNameLength = newName.length();
		for (unsigned int i = 0; i < newNameLength; i++) {
			if (newName[i] != ' ') {		// if a character other than space
				empty = 0;					// not empty
			}
		}
		
		if (newNameLength == 0 || empty) {	// length of 0 or all spaces
			party->setName("??????");		// uninitialize active char's name
		} else {	// otherwise give the active character the selected name
			party->setName(newName);
		}
		menuState->popMenu();				// return to new menu
		return;
	}

	// check movement keys
	if (input->rightPressed()) {
		if ((CURSEL - 9) % 10) {			// move cursor right
			cursor.setSelection(CURSEL + 1);
		} else {							// or wrap around
			cursor.setSelection(CURSEL - 9);
		}
	} else if (input->leftPressed()) {
		if (CURSEL % 10) {					// move cursor left
			cursor.setSelection(CURSEL - 1);
		} else {							// or wrap around
			cursor.setSelection(CURSEL + 9);
		}
	} else if (input->upPressed()) {
		if (CURSEL > 9) {					// etc...
			cursor.setSelection(CURSEL - 10);
		} else {
			cursor.setSelection(CURSEL + 60);
		}
	} else if (input->downPressed()) {
		if (CURSEL < 60) {
			cursor.setSelection(CURSEL + 10);
		} else {
			cursor.setSelection(CURSEL - 60);
		}
	}

	// input handled
	input->resetAll();
}

void NameMenu::render() {
	// x pos of character sprites
	const int SPRITEX = -windowWidth / 1.25;

	// y positions of the four different characters
	// static_cast to get rid of compiler warnings about double to float
	const float SPRITEY[Party::SIZE] = {static_cast<float>(windowHeight / 1.6),
										static_cast<float>(windowHeight / 4.8),
										static_cast<float>(-windowHeight / 4.8),
										static_cast<float>(-windowHeight / 1.6)};

	// blue background and border entire window
	blueBackground(windowWidth, windowHeight);
	border(-windowWidth, windowWidth, windowHeight, -windowHeight);

	// borders around submenus
	subBorders();

	// text in submenus
	subText();

	// render sprites, names, and jobs of the party
	for (int i = Party::FIRST; i < Party::SIZE; i++) {
		Party::Characters c = static_cast<Party::Characters>(i);

		party->render(c, SPRITEX - 20, SPRITEY[i] + 100);
		if (party->getActive() == c) {	// the character who's name is being edited
			twenty.drawText(SPRITEX + 140, SPRITEY[i] + 18, newName.c_str());
		} else {						// display current name for other characters
			twenty.drawText(SPRITEX + 140, SPRITEY[i] + 18, party->getName(c).c_str());
		}
		twenty.drawText(SPRITEX, SPRITEY[i] - 110, party->getJob(c).c_str());
	}

	// blinking cursor next to the active character
	subCursor.render(SPRITEX + 140, SPRITEY[party->getActive()] + 78);

	// render the cursor for letters
	letterCursor();
}

void NameMenu::subBorders() {
	// x coordinate of the left side of all the sub borders
	const int LEFTX = -150;

	border(LEFTX, windowWidth, windowHeight, windowHeight - 150);
	border(LEFTX, windowWidth, windowHeight - 150, windowHeight - 760);
	border(LEFTX, windowWidth, windowHeight - 760, -windowHeight);
}

void NameMenu::subText() {
	// x coordinate of the sub text
	const int LEFTX = -100;

	twenty.drawText(LEFTX, windowHeight - 100, "Enter name.");

	// array of 100 const char*
	char row[100][2] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J",
						"K", "L", "M", "N", "O", "P", "Q", "R", "S", "T",
						"U", "V", "W", "X", "Y", "Z", " ", " ", " ", " ",
						"a", "b", "c", "d", "e", "f", "g", "h", "i", "j",
						"k", "l", "m", "n", "o", "p", "q", "r", "s", "t",
						"u", "v", "w", "x", "y", "z", " ", " ", " ", " ",
						"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			twenty.drawText(LEFTX + (80*i), windowHeight - 270 - (70 * j), row[i + (j * 10)]);
		}
	}
	twenty.drawText(LEFTX, -windowHeight + 100, "      A    Confirm            B    Erase");
	twenty.drawText(LEFTX, -windowHeight + 40, "Start    Done");
}

void NameMenu::letterCursor() {
	float xPos = -115 + (80 * (cursor.getSelection() % 10));
	float yPos = windowHeight - 270 - (70 * (cursor.getSelection() / 10));

	glDisable(GL_TEXTURE_2D);
	glColor3f(0.8f, 0.8f, 0.8f);
	glLineWidth(2.0f);
	glBegin(GL_LINES);
		glVertex2f(xPos, yPos + 65);		// left side
		glVertex2f(xPos, yPos);
		glVertex2f(xPos, yPos);				// bottom
		glVertex2f(xPos + 65, yPos);
		glVertex2f(xPos + 65, yPos);		// right
		glVertex2f(xPos + 65, yPos + 65);
		glVertex2f(xPos + 65, yPos + 65);	// top
		glVertex2f(xPos, yPos + 65);
	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
}
