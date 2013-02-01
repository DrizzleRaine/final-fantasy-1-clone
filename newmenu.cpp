#include "newmenu.h"

NewMenu::NewMenu() : subCursor(1) {
	// initialize the possible random names
	std::string names[Party::SIZE][10] = {{"Doma", "Leo", "Giott", "Baron", "Banon", "Argus", "Desh", "Sasuun", "Arus", "Zeza"},
								{"Wedge", "Blank", "Kelga", "Daryl", "Tuss", "Sarisa", "Biggs", "Gerad", "Baku", "Puck"},
								{"Ruby", "Elia", "Luca", "Mikoto", "Sherko", "Kuku", "Noah", "Jenica", "Sarina", "Sara"},
								{"Stella", "Kokkol", "Homac", "Koko", "Zok", "Topapa", "Nina", "Delila", "Gungho", "Gilles"}};
	for (int i = 0; i < Party::SIZE; i++) {
		for (int j = 0; j < 10; j++) {
			randomNames[i][j] = names[i][j];
		}
	}

	startConfirm = 0;
	startError = 0;
}

NewMenu::~NewMenu() {
}

void NewMenu::update() {
	const int CURSEL = cursor.getSelection();
	int newCurSel = CURSEL;

	if (startError && input->anyKey()) {
		// if user hits a key while error displayed
		// cancel the error, it has been acknowledged
		startError = 0;
		input->resetAll();
		return;
	}

	// check action keys
	if (input->getCancel()) {						// cancel button
		input->resetCancel();						// handled
		if (startConfirm) {							// if in confirmation menu
			newCurSel = subCursor.getSelection();	// restore cursor
			startConfirm = 0;						// cancel confirmation
		} else {
			menuState->exitMenus();					// return to title screen
			return;
		}
	} else if (input->getConfirm()) {				// confirm button
		input->resetConfirm();
		if (startConfirm) {							// if in confirmation menu
			if (CURSEL == 0) {						// 'yes' selected
				party->initialize();				// initialize the party
				menuState->exitMenus(1);			// start the game!
				return;	
			}						
			
			// else 'no' was selected
			newCurSel = subCursor.getSelection();	// restore cursor
			startConfirm = 0;						// end confirmation
		} else {									// option selected
			// set the character to be modified
			party->setActive(CURSEL / 2);
			if (CURSEL % 2) {						// name was clicked on
				menuState->pushMenu(new NameMenu(party->getName()));// switch menus
			} else {								// a job was clicked on
				menuState->pushMenu(new JobMenu());	// switch menus
			}
			return;
		}
	} else if (input->getSelect() && !startConfirm) {
		// set the character to be modified
		party->setActive(CURSEL / 2);

		// assign the character a pseudo random name
		party->setName(randomNames[CURSEL / 2][rand() % 10]);
	} else if (input->getStart()) {				// attempt to start the game
		input->resetStart();
		for (int i = Party::FIRST; i < Party::SIZE; i++) {
			if (party->emptyName(static_cast<Party::Characters>(i))) {
				// if a character has not been givin a name
				startError = SDL_GetTicks();	// inform user of error
				return;
			}
		}
		if (!startConfirm) {					// no error, confirm settings
			subCursor.setSelection(CURSEL);		// remember cursor position
			newCurSel = 0;						// set cursor to default
			startConfirm = 1;					// start game confirm menu
		}
	}

	// check movement keys
	if (input->rightPressed() || input->leftPressed()) {
		if (CURSEL % 2) {						// cursor on name
			newCurSel = CURSEL - 1;				// move it to job
		} else {								// else on a job
			newCurSel = CURSEL + 1;				// move it to name
		}
	}

	if (input->upPressed() || input->downPressed()) {
		if (startConfirm) {						// in confirmation window
			if (CURSEL == 0) {					// if on yes
				newCurSel = 1;					// move it to no
			} else {							// else on no
				newCurSel = 0;					// move it to yes
			}
		} else if (input->upPressed()) {		// move up a character
			newCurSel = CURSEL - 2;
		} else {								// move down a character
			newCurSel = CURSEL + 2;
		}
	}

	// check for cursor wrap around
	if (newCurSel == 9) {
		newCurSel = 1;
	} else if (newCurSel == -1) {
		newCurSel = 7;
	} else if (newCurSel > 7) {
		newCurSel = 0;
	} else if (newCurSel < 0) {
		newCurSel = 6;
	}

	// update cursor selection
	cursor.setSelection(newCurSel);

	// input handled
	input->resetAll();
}

void NewMenu::render() {
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

	// render sprites, names, and jobs
	for (int i = Party::FIRST; i < Party::SIZE; i++) {
		Party::Characters c = static_cast<Party::Characters>(i);

		party->render(c, SPRITEX, SPRITEY[i] + 72);
		twenty.drawText(-windowWidth / 2, SPRITEY[i], party->getName(c).c_str());
		twenty.drawText(-windowWidth / 2, SPRITEY[i] - 80, party->getJob(c).c_str());
	}

	// render the cursor(s)
	cursorRender(SPRITEX, SPRITEY);
}

void NewMenu::subBorders() {
	// x coordinate of the left side of all the sub borders
	const int LEFTX = windowWidth - 480;

	border(LEFTX, windowWidth, windowHeight, windowHeight - 150);
	if (startConfirm) {
		border(LEFTX, windowWidth, windowHeight - 150, windowHeight - 480);
		border(LEFTX, windowWidth, windowHeight - 480, windowHeight - 750);
		border(LEFTX, windowWidth, windowHeight - 750, -windowHeight);
	} else {
		border(LEFTX, windowWidth, windowHeight - 150, windowHeight - 530);
		border(LEFTX, windowWidth, windowHeight - 530, -windowHeight);
	}
}

void NewMenu::subText() {
	const int LEFTX = windowWidth - 430;

	SDL_Rect r = {0, 0, 0, 0};
	twenty.textSize("New Game", &r);
	const int CENTERED = windowWidth - (480 / 2) - (r.w / 2);

	twenty.drawText(CENTERED, windowHeight - 100, "New Game");
	if (startConfirm) {
		int y = windowHeight - 600;
		twenty.drawText(LEFTX, windowHeight - 300, "Begin game\nwith this party?");
		twenty.drawText(windowWidth - 250, y, "Yes");
		twenty.drawText(windowWidth - 250, windowHeight - 670, "No");
		twenty.drawText(LEFTX, windowHeight - 850, "A      Confirm");
		twenty.drawText(LEFTX, windowHeight - 920, "B      Back");
		return;
	} else if (startError) {
		twenty.drawText(LEFTX, windowHeight - 300, 
						"You must name\nall characters.");
	} else {
		twenty.drawText(LEFTX, windowHeight - 300, 
						"Assign each\ncharacter's\nname and class.");
	}
	twenty.drawText(LEFTX, windowHeight - 630, "A      Confirm");
	twenty.drawText(LEFTX - 2, windowHeight - 680, "B      Back");
	twenty.drawText(LEFTX, windowHeight - 740, "Start\n    Done");
	twenty.drawText(LEFTX, windowHeight - 860, "Select\n    Auto-Name");
}

void NewMenu::cursorRender(const int SPRITEX, const float *SPRITEY) {
	if (startError && (SDL_GetTicks() - startError < 1000)) {
		// tried to start game with unnamed character(s)
		for (int i = Party::FIRST; i < Party::SIZE; i++) {
			Party::Characters c = static_cast<Party::Characters>(i);
			if (party->emptyName(c)) {	// if this character has no name
				// put a blinking cursor by it
				subCursor.render(SPRITEX, SPRITEY[i]);
			}
		}
	} else if (startConfirm) {
		if (cursor.getSelection()) {
			cursor.render(windowWidth - 270, windowHeight - 610);
		} else {
			cursor.render(windowWidth - 270, windowHeight - 540);
		}
	} else {
		startError = 0;
		if (cursor.getSelection() % 2) { 	// cursor on name
			cursor.render(SPRITEX + 220, SPRITEY[cursor.getSelection() / 2] + 70);
		} else {							// cursor on job
			cursor.render(SPRITEX, SPRITEY[cursor.getSelection() / 2] + 30);
		}
	}
}


