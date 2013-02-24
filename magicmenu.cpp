#include "magicmenu.h"

MagicMenu::MagicMenu(int c) : subCursor(1) {
	character = static_cast<Party::Characters>(c);
	currentOption = USE;
}

MagicMenu::~MagicMenu() {
}

void MagicMenu::update() {
	const int CURSEL = cursor.getSelection();
	int newCurSel = CURSEL;

	// exit menu or cancel option
	if (input->getCancel()) {
		input->resetCancel();
		if (currentOption == NONE) {
			menuState->popMenu();		// exit item menu
			return;						// retun immediately
		} else {
			newCurSel = currentOption;	// restore cursor
			currentOption = NONE;		// return to menu options
		}
	}

	// option or item selected
	if (input->getConfirm()) {
		if (currentOption == NONE) {
			// option selected (Use or Discard
			currentOption = static_cast<Options>(CURSEL);
			subCursor.setSelection(CURSEL);	// points at selected option
			newCurSel = 0;					// start at first magic spell position
		} else {
			if (subCursor.getSelection() == DISCARD) {
				printf("DISCARD SPELL\n");
			} else {
				printf("USE SPELL\n");
			}
		}
	}

	if (input->getRButton()) {
		character = static_cast<Party::Characters>(character + 1);
		if (character > Party::FOURTH) {
			character = Party::FIRST;
		}
	} else if (input->getLButton()) {
		character = static_cast<Party::Characters>(character - 1);
		if (character < Party::FIRST) {
			character = Party::FOURTH;
		}
	}

	// cursor movement
	if (currentOption == NONE) {
		if (input->rightPressed()) {
			newCurSel++;
		} else if (input->leftPressed()) {
			newCurSel--;
		}

		// check for wrap around
		if (newCurSel > DISCARD) {
			newCurSel = USE;
		} else if (newCurSel < USE) {
			newCurSel = DISCARD;
		}
	} else {
		if (input->downPressed()) {
			newCurSel += 3;
		} else if (input->upPressed()) {
			newCurSel -= 3;
		} else if (input->rightPressed()) {
			newCurSel++;
		} else if (input->leftPressed()) {
			newCurSel--;
		}

		// check for warp around on spell list
		if (newCurSel > 23) {
			newCurSel = newCurSel % 3;
		} else if (newCurSel < 0) {
			newCurSel += 24;
		}
	}

	// update cursor selection
	cursor.setSelection(newCurSel);

	// input handled
	input->resetAll();
}

void MagicMenu::render() {
	// blue background and border entire window
	blueBackground(windowWidth, windowHeight);

	// borders around submenus
	subBorders();

	// render the text
	renderText();

	// draw current character info
	charInfo();

	// render the cursor(s)
	cursorRender();
}

void MagicMenu::subBorders() {
	const int LEFTX = windowWidth - 400;
	border(-windowWidth, LEFTX, windowHeight, windowHeight - 124);
	border(LEFTX, windowWidth, windowHeight, windowHeight - 124);
	border(-windowWidth, windowWidth, windowHeight - 124, windowHeight - 364);
	border(-windowWidth, windowWidth, windowHeight - 364, windowHeight - 844);
	border(-windowWidth, windowWidth, windowHeight - 844, -windowHeight);
}

void MagicMenu::renderText() {
	const int LINEHEIGHT = twenty.getLineSkip();

	// menu options
	twenty.drawText(-windowWidth * 0.70, windowHeight - 90, "Use");
	twenty.drawText(-windowWidth * 0.15, windowHeight - 90, "Discard");

	// draw current menu centered in upper right corner
	SDL_Rect r = {0, 0, 0, 0};
	twenty.textSize("Magic", &r);
	twenty.drawText(windowWidth - (400 / 2) - (r.w / 2), windowHeight - 90, "Magic");

	// current spell mp cost
	twenty.drawText(windowWidth - 350, windowHeight - 215 - LINEHEIGHT * 2,
			"MP Cost   -");

	for (int i = 1; i < 9; i++) {
		if (party->getAttribute(character, Character::MAGLEVEL) < i) {
			// TODO: fix font class method of changing colors
			//twenty.setColor(0.5, 0.5, 0.5);	// gray
		}

		std::string lvl = "Lv. " + std::to_string(i);
		twenty.drawText(-windowWidth + 50, windowHeight - 455 - (LINEHEIGHT - 8) * (i - 1), lvl.c_str());

		twenty.drawText(-windowWidth + 280, windowHeight - 455 - (LINEHEIGHT - 8) * (i - 1), "Protect");
		twenty.drawText(-40, windowHeight - 455 - (LINEHEIGHT - 8) * (i - 1), "Protect");
		twenty.drawText(360, windowHeight - 455 - (LINEHEIGHT - 8) * (i - 1), "Protect");
	}
	//twenty.setColor(1.0, 1.0, 1.0);	// white
}

void MagicMenu::charInfo() {
	const int LINEHEIGHT = twenty.getLineSkip();

	// draw character sprite
	party->render(character, -windowWidth + 100, windowHeight - 165);

	// character name, level, and job
	twenty.drawText(-windowWidth + 300, windowHeight - 215, 
			party->getName(character).c_str());
	twenty.drawText(-windowWidth + 600, windowHeight - 215, ("Lv. " +
			std::to_string(party->getAttribute(character, Character::LEVEL))).c_str());
	twenty.drawText(-windowWidth + 800, windowHeight - 215, 
			party->getJob(character).c_str());

	// hp/mp
	twenty.drawText(-windowWidth + 300, windowHeight - 215 - LINEHEIGHT, "HP\nMP");
}

void MagicMenu::cursorRender() {
	// locations to base cursor drawings
	const int ROW1Y = windowHeight - 385;
	const int COL1X = -windowWidth + 280;
	const int COL2X = -40;
	const int COL3X = 360;

	// how far to move when cursor goes to new line
	const int LINEHEIGHT = twenty.getLineSkip();

	if (currentOption == NONE) {
		// pointing at a menu option
		if (cursor.getSelection() == DISCARD) {
			cursor.render(-windowWidth * 0.15 - 10, windowHeight - 20);
		} else {
			cursor.render(-windowWidth * 0.70 + 10, windowHeight - 20);
		}
	} else {
		// subCursor is pointing at a menu option
		if (subCursor.getSelection() == DISCARD) {
			subCursor.render(-windowWidth * 0.15 - 10, windowHeight - 20);
		} else {
			subCursor.render(-windowWidth * 0.70 + 10, windowHeight - 20);
		}

		// cursor is pointing at item to use/swap
		int curSel = cursor.getSelection();
		if (curSel % 3 == 2) {
			cursor.render(COL3X, ROW1Y - ((curSel / 3) * (LINEHEIGHT - 8)));
		} else if (curSel % 3) {	// right column
			cursor.render(COL2X, ROW1Y - ((curSel / 3) * (LINEHEIGHT - 8)));
		} else {			// left column
			cursor.render(COL1X, ROW1Y - ((curSel / 3) * (LINEHEIGHT - 8)));
		}
	}
}
