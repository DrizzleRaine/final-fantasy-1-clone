#include "partymenu.h"

PartyMenu::PartyMenu() : subCursor(1) {
}

PartyMenu::~PartyMenu() {
}

void PartyMenu::update() {
	if (input->getCancel()) {
		menuState->exitMenus();
		return;
	}

	int newCurSel = cursor.getSelection();
	if (input->downPressed()) {			// move cursor down
		newCurSel++;
	} else if (input->upPressed()) {	// move cursor up
		newCurSel--;
	}

	// check for wrap wround
	if (newCurSel > 6) {
		newCurSel = 0;
	} else if (newCurSel < 0) {
		newCurSel = 6;
	}

	// update cursor selection
	cursor.setSelection(newCurSel);
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

	int curY = windowHeight - 10 - lineHeight * cursor.getSelection();
	cursor.render(windowWidth - 455, curY);
}
