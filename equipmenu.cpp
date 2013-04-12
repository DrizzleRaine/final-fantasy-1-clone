#include "equipmenu.h"

EquipMenu::EquipMenu(int c) : subCursor(1) {
	character = static_cast<Party::Characters>(c);
	currentOption = NONE;
	curItemID = -1;
}

EquipMenu::~EquipMenu() {
}

void EquipMenu::update() {
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
			if (CURSEL == OPTIMAL) {
				printf("optimize\n");
				newCurSel = 0;
			} else {
				// option selected (Equip or Remove)
				currentOption = static_cast<Options>(CURSEL);
				subCursor.setSelection(CURSEL);	// points at selected option
				newCurSel = 0;					// start at weapon slot
			}
		} else if (currentOption == EQUIP) {
			// TODO
		} else if (currentOption == REMOVE) {
			// TODO
		}
	}

	// switching characters
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
		if (newCurSel > REMOVE) {
			newCurSel = EQUIP;
		} else if (newCurSel < EQUIP) {
			newCurSel = REMOVE;
		}
	} else {
		if (input->downPressed() || input->rightPressed()) {
			newCurSel++;
		} else if (input->upPressed() || input->leftPressed()) {
			newCurSel--;
		}

		// check for warp around on equipped list
		if (newCurSel > 4) {
			newCurSel = 0;
		} else if (newCurSel < 0) {
			newCurSel = 4;
		}
	}

	// update cursor selection
	cursor.setSelection(newCurSel);

	if (currentOption == EQUIP || currentOption == REMOVE) {
		curItemID = party->getEquip(character, newCurSel);
	}

	// input handled
	input->resetAll();
}

void EquipMenu::render() {
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

void EquipMenu::subBorders() {
	const int LEFTX = windowWidth - 400;
	border(-windowWidth, LEFTX, windowHeight, windowHeight - 124);
	border(LEFTX, windowWidth, windowHeight, windowHeight - 124);
	border(-windowWidth, windowWidth, windowHeight - 124, -windowHeight + 392);
	border(-windowWidth, windowWidth, -windowHeight + 392, -windowHeight + 116);
	border(-windowWidth, windowWidth, -windowHeight + 116, -windowHeight);
}

void EquipMenu::renderText() {
	const int LINEHEIGHT = twenty.getLineSkip();

	// menu options
	twenty.drawText(-windowWidth + 100, windowHeight - 90, "Equip");
	twenty.drawText(-windowWidth + 400, windowHeight - 90, "Optimal");
	twenty.drawText(-windowWidth + 750, windowHeight - 90, "Remove");

	// draw current menu centered in upper right corner
	SDL_Rect r = {0, 0, 0, 0};
	twenty.textSize("Equip", &r);
	twenty.drawText(windowWidth - (400 / 2) - (r.w / 2), windowHeight - 90, "Equip");

	// characters current stats
	const int ROW1Y = -windowHeight + 300;
	const int LHSMALL = LINEHEIGHT - 8;
	std::string label[] = {"ATK", "ACC", "DEF", "EVA"};
	for (int i = 0; i < 4; i++) {
		int yPos = ROW1Y - LHSMALL * i;
		Character::Stats stat = static_cast<Character::Stats>(Character::ATK + i);
		std::string statStr = std::to_string(party->getAttribute(character, stat));

		// draw the label
		twenty.drawText(150, yPos, label[i].c_str());

		// draw the value right aligned
		twenty.textSize(statStr.c_str(), &r);
		twenty.drawText(380 - r.w, yPos, statStr.c_str());

		// draw the arrow
		twenty.drawText(420, yPos, ">");
	}

	// current item description
	if (curItemID != -1) {
		twenty.drawText(-windowWidth + 50, -windowHeight + 30, 
				items.getDescription(curItemID).c_str());
	}
}

void EquipMenu::charInfo() {
	const int TOPEDGE = windowHeight - 270;
	const int LHSMALL = twenty.getLineSkip() - 8;

	// draw character sprite
	party->render(character, -windowWidth + 50, windowHeight - 280);

	// character name, level, and job
	twenty.drawText(-windowWidth + 50, TOPEDGE, 
		party->getName(character).c_str());
	twenty.drawText(-windowWidth + 400, TOPEDGE, ("Lv. " +
		std::to_string(party->getAttribute(character, Character::LEVEL))).c_str());
	twenty.drawText(-windowWidth + 200, TOPEDGE - LHSMALL * 1.4, 
		party->getJob(character).c_str());

	// hp/mp
	twenty.drawText(-windowWidth + 200, TOPEDGE - LHSMALL * 3, "HP\nMP");
	std::string hpFraction = party->getHPFraction(character);
	std::string mpFraction = party->getMPFraction(character);

	// right align hp/mp fractions
	int rightEdge = -windowWidth + 610;
	SDL_Rect r = {0, 0, 0, 0};

	// hp fraction
	twenty.textSize(hpFraction.c_str(), &r);
	twenty.drawText(rightEdge - r.w, TOPEDGE - LHSMALL * 3, hpFraction.c_str());

	// mp fraction
	twenty.textSize(mpFraction.c_str(), &r);
	twenty.drawText(rightEdge - r.w, TOPEDGE - LHSMALL * 4, mpFraction.c_str());

	// character current equipment
	std::string label[] = {"WPN", "SHD", "HLM", "ARM", "GLV"};
	for (int i = 0; i < 5; i++) {
		int yPos = TOPEDGE - LHSMALL * i;
		int itemID = party->getEquip(character, i);

		twenty.drawText(0, yPos, label[i].c_str());
		if (i == 0 && itemID == -1) {
			// if no weapon equipped, display "Hands"
			twenty.drawText(150, yPos, "Hands");
		} else {
			twenty.drawText(150, yPos, items.getName(itemID).c_str());
		}
	}
}

void EquipMenu::cursorRender() {
	// locations to base cursor drawings
	const int ROW1Y = windowHeight - 200;

	// how far to move when cursor goes to new line
	const int LHSMALL = twenty.getLineSkip() - 8;

	if (currentOption == NONE) {
		// pointing at a menu option
		if (cursor.getSelection() == REMOVE) {
			cursor.render(-windowWidth + 750, windowHeight - 20);
		} else if (cursor.getSelection() == OPTIMAL) {
			cursor.render(-windowWidth + 400, windowHeight - 20);
		} else {
			cursor.render(-windowWidth + 100, windowHeight - 20);
		}
	} else {
		// subCursor is pointing at a menu option
		if (subCursor.getSelection() == REMOVE) {
			subCursor.render(-windowWidth + 750, windowHeight - 20);
		} else {
			subCursor.render(-windowWidth + 100, windowHeight - 20);
		}

		// cursor is pointing at item to use/swap
		int curSel = cursor.getSelection();
		cursor.render(150, ROW1Y - LHSMALL * curSel);
	}
}
