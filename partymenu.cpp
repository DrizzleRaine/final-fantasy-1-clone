#include "partymenu.h"
#include "itemmenu.h"
#include "magicmenu.h"
#include "equipmenu.h"
#include "statusmenu.h"

PartyMenu::PartyMenu() : subCursor(1), subCursorSwap(1) {
	currentOption = NONE;
	swappingCharacters = 0;
}

PartyMenu::~PartyMenu() {
}

void PartyMenu::update() {
	const int CURSEL = cursor.getSelection();
	int newCurSel = CURSEL;

	// exit menu or cancel option
	if (input->getCancel()) {
		input->resetCancel();
		if (currentOption == NONE) {
			menuState->exitMenus();
			return;
		} else if (swappingCharacters) {
			// re-select first character to swap
			newCurSel = subCursor.getSelection();
			subCursor.setSelection(subCursorSwap.getSelection());
			swappingCharacters = 0;					// swap canceled
		} else {
			newCurSel = currentOption;				// restore cursor
			currentOption = NONE;					// return to menu options
		}
	}

	// option or character selected
	if (input->getConfirm()) {
		input->resetConfirm();
		if (swappingCharacters) {
			// second character to swap selected, swap members
			party->swapCharacters(subCursor.getSelection(), CURSEL);
			swappingCharacters = 0;					// swap completed
			newCurSel = currentOption;				// restore cursor
			currentOption = NONE;					// return to menu options
		} else if (currentOption == FORMATION) {
			swappingCharacters = 1;					// 1st char to swap selected
			subCursorSwap.setSelection(FORMATION);	// points at formation option
			subCursor.setSelection(CURSEL);			// points at 1st swap character
		} else if (currentOption == MAGIC) {
			currentOption = NONE;
			cursor.setSelection(ITEMS);				// reset cursor before switching
			menuState->pushMenu(new MagicMenu(CURSEL));
			return;
		} else if (currentOption == EQUIPMENT) {
			currentOption = NONE;
			cursor.setSelection(ITEMS);				// reset cursor before switching
			menuState->pushMenu(new EquipMenu(CURSEL));
			return;
		} else if (currentOption == STATUS) {
			currentOption = NONE;
			cursor.setSelection(ITEMS);				// reset cursor before switching
			menuState->pushMenu(new StatusMenu(CURSEL));
			return;
		} else if (CURSEL == FORMATION) {
			currentOption = FORMATION;				// formation option selected
			subCursor.setSelection(CURSEL);			// update cursors
			newCurSel = Party::FIRST;
		} else if (CURSEL == ITEMS) {
			menuState->pushMenu(new ItemMenu());
			return;
		} else if (CURSEL == MAGIC) {
			currentOption = MAGIC;
			subCursor.setSelection(CURSEL);
			newCurSel = Party::FIRST;
		} else if (CURSEL == EQUIPMENT) {
			currentOption = EQUIPMENT;
			subCursor.setSelection(CURSEL);
			newCurSel = Party::FIRST;
		} else if (CURSEL == STATUS) {
			currentOption = STATUS;
			subCursor.setSelection(CURSEL);
			newCurSel = Party::FIRST;
		}
	}

	// cursor movement
	if (input->downPressed()) {
		newCurSel++;
	} else if (input->upPressed()) {
		newCurSel--;
	}

	// check for wrap wround
	if (currentOption == NONE) {
		if (newCurSel > SAVE) {
			newCurSel = ITEMS;
		} else if (newCurSel < ITEMS) {
			newCurSel = SAVE;
		}
	} else {	// pointing at characters
		if (newCurSel > Party::FOURTH) {
			newCurSel = Party::FIRST;
		} else if (newCurSel < Party::FIRST) {
			newCurSel = Party::FOURTH;
		}
	}

	// update cursor selection
	cursor.setSelection(newCurSel);

	// input handled
	input->resetAll();
}

void PartyMenu::render() {
	// x pos of character sprites
	const int SPRITEX = -windowWidth / 1.25;

	// y positions of the four different characters
	// static_cast to get rid of compiler warnings about double to float
	const float SPRITEY[Party::SIZE] = {static_cast<float>(windowHeight * 0.69),
										static_cast<float>(windowHeight * 0.23),
										static_cast<float>(-windowHeight * 0.23),
										static_cast<float>(-windowHeight * 0.69)};

	// blue background and border entire window
	blueBackground(windowWidth, windowHeight);
	border(-windowWidth, windowWidth, windowHeight, -windowHeight);

	// borders around submenus
	subBorders();

	// text in submenus
	subText();

	// render names, sprites, lv, job, hp, mp
	for (int i = Party::FIRST; i < Party::SIZE; i++) {
		Party::Characters c = static_cast<Party::Characters>(i);

		// character name and sprite
		twenty.drawText(SPRITEX - 35, SPRITEY[i] + twenty.getLineSkip() * 0.75, 
				party->getName(c).c_str());
		party->render(c, SPRITEX, SPRITEY[i] + 50);

		// draw character details
		std::string level = "Lv. " + 
				std::to_string(party->getAttribute(c, Character::LEVEL)) + "  ";
		std::string details = level + party->getJob(c) + "\nHP\nMP";
		twenty.drawText(-windowWidth / 1.75, SPRITEY[i], details.c_str());

		// get hp/mp fraction strings
		std::string hpFraction = "\n" + party->getHPFraction(c);
		std::string mpFraction = "\n\n" + party->getMPFraction(c);

		// hp/mp is right aligned
		const int RIGHTEDGE = -windowWidth / 1.75 + 390;
		SDL_Rect r = {0, 0, 0, 0};

		// draw the hp fraction right aligned
		twenty.textSize(hpFraction.c_str(), &r);
		twenty.drawText(RIGHTEDGE - r.w, SPRITEY[i], hpFraction.c_str());

		// draw the mp fraction right aligned
		twenty.textSize(mpFraction.c_str(), &r);
		twenty.drawText(RIGHTEDGE - r.w, SPRITEY[i], mpFraction.c_str());
	}

	// render the cursor(s)
	cursorRender(SPRITEX, SPRITEY);
}

void PartyMenu::subBorders() {
	const int LEFTX = windowWidth - 550;
	border(LEFTX, windowWidth, windowHeight, -windowHeight + 500);
	border(LEFTX, windowWidth, -windowHeight + 500, -windowHeight + 324);
	border(LEFTX, windowWidth, -windowHeight + 324, -windowHeight + 124);
	border(LEFTX - 100, windowWidth, -windowHeight + 124, -windowHeight);
}

void PartyMenu::subText() {
	const int LINEHEIGHT = twenty.getLineSkip();

	// draw menu options
	int leftX = windowWidth - 450;
	twenty.drawText(leftX, windowHeight - 82, 
			"Items\nMagic\nEquipment\nStatus\nFormation\nConfig\nSave\n");

	// gil and time played labels
	leftX = windowWidth - 510;
	twenty.drawText(leftX, -windowHeight + 410, "Gil");
	twenty.drawText(leftX, -windowHeight + 410 - LINEHEIGHT, "Time");

	// get current gil and time strings
	std::string gil = std::to_string(party->getGil());
	std::string time = party->getTime();

	// right aligned
	int rightEdge = windowWidth - 50;
	SDL_Rect r = {0, 0, 0, 0};

	// draw the gil right aligned
	twenty.textSize(gil.c_str(), &r);
	twenty.drawText(rightEdge - r.w, -windowHeight + 410, gil.c_str());

	// draw the play time right aligned
	twenty.textSize(time.c_str(), &r);
	twenty.drawText(rightEdge - r.w, -windowHeight + 410 - LINEHEIGHT, 
			party->getTime().c_str());

	// draw region name centered
	std::string region = party->getRegion();
	twenty.textSize(region.c_str(), &r);
	twenty.drawText(windowWidth - (650 / 2) - (r.w / 2), -windowHeight + 35, 
			region.c_str());
}

void PartyMenu::cursorRender(const int SPRITEX, const float *SPRITEY) {
	// locations to base cursor drawings
	const int OPTIONSX = windowWidth - 455;
	const int OPTIONSY = windowHeight - 10;
	
	// how far to move when cursor goes to new line
	const int LINEHEIGHT = twenty.getLineSkip();

	int curY;
	if (swappingCharacters) {
		// subCursorSwap is pointing at formation option
		curY = OPTIONSY - LINEHEIGHT * subCursorSwap.getSelection();
		subCursorSwap.render(OPTIONSX, curY);

		// subCursor is pointing at first character to swap
		curY = SPRITEY[subCursor.getSelection()];
		subCursor.render(SPRITEX - 20, curY);

		// cursor is pointing at second character to swap
		curY = SPRITEY[cursor.getSelection()] - 20;
		cursor.render(SPRITEX - 20, curY);
	} else if (currentOption == NONE) {
		// pointing at menu optino
		curY = OPTIONSY - LINEHEIGHT * cursor.getSelection();
		cursor.render(OPTIONSX, curY);
	} else {
		// subCursor is pointing at current menu option
		curY = OPTIONSY - LINEHEIGHT * subCursor.getSelection();
		subCursor.render(OPTIONSX, curY);

		// cursor is pointing at character to select/swap
		curY = SPRITEY[cursor.getSelection()];
		cursor.render(SPRITEX - 20, curY);
	}
}
