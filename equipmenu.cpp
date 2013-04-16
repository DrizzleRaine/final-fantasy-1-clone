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
		} else if (equippable.size()) {
			// equip change cancelled, re-select current equip slot
			newCurSel = subCursor.getSelection();
			subCursor.setSelection(subCursorEquip.getSelection());
			curItemID = party->getEquip(character, newCurSel);
			equippable.clear();	
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
				curItemID = party->getEquip(character, newCurSel);
			}
		} else if (equippable.size()) {
			// get the itemID to be unequipped
			int SUBCURSEL = subCursor.getSelection();
			int itemIDRemoved = party->getEquip(character, SUBCURSEL);
			party->removeEquip(character, SUBCURSEL);	// unequip it
			party->addItem(itemIDRemoved);				// add it to party inventory

			// get the values of the item to be equipped
			int values[4];
			for (int i = 0; i < 4; i++) {
				values[i] = items.getValue(curItemID, i);
			}

			// equip the item
			party->setEquip(character, SUBCURSEL, curItemID, values);

			// remove the item from party inventory
			party->addItem(curItemID, -1);

			// restore cursors
			newCurSel = subCursor.getSelection();
			subCursor.setSelection(subCursorEquip.getSelection());
			equippable.clear();
		} else if (currentOption == EQUIP) {
			// fill vector of items that can be equipped
			for (int i = 0; i < party->INVENTORY_SIZE; i++) {
				int itemID = party->getItem(i);
				if (items.equippable(itemID, CURSEL, party->getJobInt(character))) {
					// if equippable by character, add to vector
					equippable.push_back(itemID);
				}
			}

			if (equippable.size()) {
				// if inventory contains items character can equip in this slot
				subCursorEquip.setSelection(EQUIP);	// points at equip option
				subCursor.setSelection(CURSEL);		// points at equip to be removed
				newCurSel = 0;						// points at first available
				curItemID = equippable[newCurSel];
			}
			// else if no equippable items, do nothing
		} else if (currentOption == REMOVE) {
			// get the itemID to be removed
			int itemIDRemoved = party->getEquip(character, CURSEL);

			party->removeEquip(character, CURSEL);	// remove the equip at CURSEL
			curItemID = -1;							// slot pointed at is now empty

			// add the itemID removed, to party inventory
			party->addItem(itemIDRemoved);
		}
	}

	// switching characters (if not changing equipment)
	if (input->getRButton() && !equippable.size()) {
		character = static_cast<Party::Characters>(character + 1);
		if (character > Party::FOURTH) {
			character = Party::FIRST;
		}

		// update itemID in slot being pointed at
		curItemID = party->getEquip(character, newCurSel);
	} else if (input->getLButton() && !equippable.size()) {
		character = static_cast<Party::Characters>(character - 1);
		if (character < Party::FIRST) {
			character = Party::FOURTH;
		}

		// update itemID in slot being pointed at
		curItemID = party->getEquip(character, newCurSel);
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
	} else if (equippable.size()) {
		if (input->downPressed()) {
			newCurSel++;
		} else if (input->upPressed()) {
			newCurSel--;
		}

		// check for wrap around on equipment list
		// TODO need to be able to scroll through the list
		if (newCurSel == equippable.size()) {
			newCurSel = equippable.size() - 1;
		} else if (newCurSel < 0) {
			newCurSel = 0;
		}
		curItemID = equippable[newCurSel];
	} else {
		if (input->downPressed() || input->rightPressed()) {
			newCurSel++;
		} else if (input->upPressed() || input->leftPressed()) {
			newCurSel--;
		}

		// check for wrap around on equipped list
		if (newCurSel > 4) {
			newCurSel = 0;
		} else if (newCurSel < 0) {
			newCurSel = 4;
		}
		curItemID = party->getEquip(character, newCurSel);
	}

	// update cursor selection
	cursor.setSelection(newCurSel);

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
	border(-windowWidth, 80, -windowHeight + 392, -windowHeight + 116);
	border(80, windowWidth, -windowHeight + 392, -windowHeight + 116);
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

	// available items to equip in current slot
	const int ROW1Y = -windowHeight + 300;
	const int LHSMALL = LINEHEIGHT - 8;
	int numEquippable = equippable.size();
	for (int i = 0; i < numEquippable; i++) {
		twenty.drawText(-windowWidth + 100, ROW1Y - LHSMALL * i, 
				items.getName(equippable[i]).c_str());
	}

	// characters current stats
	std::string label[] = {"ATK", "ACC", "DEF", "EVA"};
	for (int i = 0; i < 4; i++) {
		int yPos = ROW1Y - LHSMALL * i;

		// get character's stat value with current equipment
		Character::Stats stat = static_cast<Character::Stats>(Character::ATK + i);
		int statVal = party->getAttribute(character, stat);

		// draw the label
		twenty.drawText(150, yPos, label[i].c_str());

		// draw the value right aligned
		std::string statStr = std::to_string(statVal);
		twenty.textSize(statStr.c_str(), &r);
		twenty.drawText(400 - r.w, yPos, statStr.c_str());

		// draw the arrow
		twenty.drawText(440, yPos, ">");
	}

	// if changing equipment, show what stats would become
	if (equippable.size()) {
		// get the current itemID equipped at slot to change
		int curEquipID = party->getEquip(character, subCursor.getSelection());

		// the itemID currently pointed at and its values
		int newItemID = equippable[cursor.getSelection()];
		int values[4];
		for (int i = 0; i < 4; i++) {
			values[i] = items.getValue(newItemID, i);
		}

		// temporarily equip the currently pointed at item on character
		party->setEquip(character, subCursor.getSelection(), newItemID, values);

		for (int i = 0; i < 4; i++) {
			int yPos = ROW1Y - LHSMALL * i;

			// get character's stat value with changed equipment
			Character::Stats stat = static_cast<Character::Stats>(Character::ATK+i);
			int statVal = party->getAttribute(character, stat);

			// show what stats would be with this item equipped
			std::string newStr = std::to_string(statVal);
			twenty.textSize(newStr.c_str(), &r);
			twenty.drawText(windowWidth - 100 - r.w, yPos, newStr.c_str());
		}

		// re-equip current itemID on the character
		for (int i = 0; i < 4; i++) {
			values[i] = items.getValue(curEquipID, i);
		}
		party->setEquip(character, subCursor.getSelection(), curEquipID, values);
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
	twenty.drawText(-windowWidth + 500, TOPEDGE, ("Lv. " +
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
	const int ROW2Y = -windowHeight + 370;

	// how far to move when cursor goes to new line
	const int LHSMALL = twenty.getLineSkip() - 8;

	if (equippable.size()) {
		// subCursorEquip is pointing at equip option
		subCursorEquip.render(-windowWidth + 100, windowHeight - 20);

		// subCursor is pointing at slot to equip
		subCursor.render(150, ROW1Y - LHSMALL * subCursor.getSelection());

		// cursor pointing at item to equip in that slot
		cursor.render(-windowWidth + 100, ROW2Y - LHSMALL * cursor.getSelection());
	} else if (currentOption == NONE) {
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
