#include "partymenu.h"

PartyMenu::PartyMenu() : subCursor(1), subCursor2(1) {
	currentOption = NONE;
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
		} else if (currentOption == FORMATIONSECOND) {
			// re-select first character to swap
			newCurSel = subCursor.getSelection();
			subCursor.setSelection(subCursor2.getSelection());
			currentOption = FORMATION;
		} else {	// back to menu options
			newCurSel = currentOption;
			currentOption = NONE;
		}
	}

	// menu option selected
	if (input->getConfirm()) {
		if (currentOption == FORMATIONSECOND) {
			// second character to swap selected, swap members
			party->swapCharacters(subCursor.getSelection(), CURSEL);
			newCurSel = FORMATION;				// restore cursor
			currentOption = NONE;				// return to menu options
		} else if (currentOption == FORMATION) {
			currentOption = FORMATIONSECOND;	// first character to swap selected
			subCursor2.setSelection(FORMATION);	// points at formation option
			subCursor.setSelection(CURSEL);		// points at first swap character
		} else if (CURSEL == FORMATION) {
			currentOption = FORMATION;			// formation option selected
			subCursor.setSelection(CURSEL);		// update cursors
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
		int lineHeight = twenty.getLineSkip();
		twenty.drawText(SPRITEX - 35, SPRITEY[i] + lineHeight * 0.75, 
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
	border(LEFTX, windowWidth, -windowHeight + 500, -windowHeight + 325);
	border(LEFTX, windowWidth, -windowHeight + 325, -windowHeight + 125);
	border(LEFTX - 100, windowWidth, -windowHeight + 125, -windowHeight);
}

void PartyMenu::subText() {
	int lineHeight = twenty.getLineSkip();

	// draw menu options
	int leftX = windowWidth - 450;
	twenty.drawText(leftX, windowHeight - 82, 
			"Items\nMagic\nEquipment\nStatus\nFormation\nConfig\nSave\n");

	// gil and time played labels
	leftX = windowWidth - 510;
	twenty.drawText(leftX, -windowHeight + 410, "Gil");
	twenty.drawText(leftX, -windowHeight + 410 - lineHeight, "Time");

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
	twenty.drawText(rightEdge - r.w, -windowHeight + 410 - lineHeight, 
			party->getTime().c_str());

	// draw map name centered
	std::string mapName = party->getMapName();
	twenty.textSize(mapName.c_str(), &r);
	twenty.drawText(windowWidth - (650 / 2) - (r.w / 2), -windowHeight + 35, 
			mapName.c_str());
}

void PartyMenu::cursorRender(const int SPRITEX, const float *SPRITEY) {
	int lineHeight = twenty.getLineSkip();
	int menuOptionsX = windowWidth - 455;
	int menuOptionsY = windowHeight - 10;

	int curY;
	if (currentOption == FORMATIONSECOND) {
		// subCursor2 is pointing at formation option
		curY = menuOptionsY - lineHeight * subCursor2.getSelection();
		subCursor2.render(menuOptionsX, curY);

		// subCursor is pointing at first character to swap
		curY = SPRITEY[subCursor.getSelection()];
		subCursor.render(SPRITEX - 20, curY);

		// cursor is pointing at second character to swap
		curY = SPRITEY[cursor.getSelection()] - 20;
		cursor.render(SPRITEX - 20, curY);
	} else if (currentOption == NONE) {
		// pointing at menu optino
		curY = menuOptionsY - lineHeight * cursor.getSelection();
		cursor.render(menuOptionsX, curY);
	} else {
		// subCursor is pointing at current menu option
		curY = menuOptionsY - lineHeight * subCursor.getSelection();
		subCursor.render(menuOptionsX, curY);

		// cursor is pointing at character to select/swap
		curY = SPRITEY[cursor.getSelection()];
		cursor.render(SPRITEX - 20, curY);
	}
}
