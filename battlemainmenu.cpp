#include "battlemainmenu.h"

BattleMainMenu::BattleMainMenu(Party::Characters c, bool *enemyLocs) : subCursor(1) {
	sixteen.initFont("fonts/VarelaRound-Regular.ttf", 16);
	currentOption = NONE;

	character = c;
	for (int i = 0; i < ENEMYSLOTS; i++) {
		enemyLocations[i] = enemyLocs[i];
	}
}

BattleMainMenu::~BattleMainMenu() {
}

void BattleMainMenu::update() {
	const int CURSEL = cursor.getSelection();
	int newCurSel = CURSEL;

	// exit menu or cancel option
	if (input->getCancel()) {
		if (currentOption != NONE) {
			newCurSel = subCursor.getSelection();	// restore cursor
			currentOption = NONE;					// return to menu
			input->resetCancel();					// cancel handled
		}
	}

	// action or target selected
	if (input->getConfirm()) {
		input->resetConfirm();
		if (currentOption == ATTACK) {
			// set character turn
			Character::Turn t;
			t.action = Character::Actions::ATTACK;
			t.actionID = -1;
			t.target = newCurSel;
			party->setTurn(character, t);

			// exit menu
			menuState->popMenu();
			return;
		} else {
			currentOption = ATTACK;			// Attack selected
			subCursor.setSelection(CURSEL);	// points at Attack option
			newCurSel = 0;					// points at first enemy
		}
	}

	// cursor movement
	if (currentOption == NONE) {
		if (input->downPressed()) {
			newCurSel++;
		} else if (input->upPressed()) {
			newCurSel--;
		}

		// check for wrap around
		if (newCurSel == COUNT) {
			newCurSel = ATTACK;
		} else if (newCurSel == NONE) {
			newCurSel = FLEE;
		}
	} else {
		// enemy target selection
		// TODO fix this logic
		if (input->downPressed()) {
			newCurSel++;
			while (newCurSel < ENEMYSLOTS && !enemyLocations[newCurSel]) {
				newCurSel++;
			}
		} else if (input->upPressed()) {
			newCurSel--;
		} else if (input->rightPressed()) {
			newCurSel += 3;
		} else if (input->leftPressed()) {
			newCurSel -= 3;
		}

		if (newCurSel < 0) {
			newCurSel = ENEMYSLOTS - 1;
			while (newCurSel >= 0 && !enemyLocations[newCurSel]) {
				newCurSel--;
			}
		} else if (newCurSel >= ENEMYSLOTS) {
			newCurSel = 0;
			while (newCurSel < ENEMYSLOTS && !enemyLocations[newCurSel]) {
				newCurSel++;
			}
		}

		while (newCurSel < ENEMYSLOTS && !enemyLocations[newCurSel]) {
			newCurSel++;
		}
	}

	// update cursor selection
	cursor.setSelection(newCurSel);

	// input handled
	input->resetAll();
}

void BattleMainMenu::render() {
	// draw window
	window(-340, -100, -windowHeight + 240, -windowHeight);

	// render the text
	renderText();

	// render the cursor(s)
	cursorRender();
}

void BattleMainMenu::renderText() {
	std::string menuOptions[COUNT] = {"Attack", "Magic", "Items", "Equip", "Flee"};
	for (int i = ATTACK; i < COUNT; i++) {
		sixteen.drawText(-300, -windowHeight + 178 - 40 * i, menuOptions[i].c_str());
	}
}

void BattleMainMenu::cursorRender() {
	// locations to base cursor drawings
	const int ROW1Y = windowHeight - 140;
	const int COL1X = -windowWidth + 100;	// left col
	const int COL2X = 50;					// right col

	// how far to move when cursor goes to new line
	const int LINEHEIGHT = sixteen.getLineSkip();

	if (currentOption == NONE) {
		// pointing at a menu option
		cursor.render(-300, -windowHeight + 240 - 40 * cursor.getSelection());
	} else {
		// subCursor is pointing at selected menu option
		subCursor.render(-300, -windowHeight + 240 - 40 * subCursor.getSelection());

		// cursor is pointing at enemy to target
		int curSel = cursor.getSelection();
		int xPos = -windowWidth + 192 * (curSel / 3) + 80;
		int yPos = windowHeight - 192 * (curSel % 3) - 120;
		cursor.render(xPos, yPos);
	}
}
