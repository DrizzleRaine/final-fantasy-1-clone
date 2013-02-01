#include "jobmenu.h"

JobMenu::JobMenu() : subCursor(1) {
	// the current cursor starts at 0
	cursor.setSelection(0);
}

JobMenu::~JobMenu() {
}

void JobMenu::update() {
	const int CURSEL = cursor.getSelection();

	// check action keys
	if (input->getCancel()) {				// cancel button
		input->resetCancel();				// handled
		menuState->popMenu();				// change back to new menu
		return;
	} else if (input->getConfirm()) {		// confirm button
		input->resetConfirm();				// handled

		// change the active characters job to the selected job
		if (CURSEL < 2) {
			party->setJob(CURSEL);
		} else if (CURSEL < 4) {
			party->setJob(CURSEL + 2);
		} else if (CURSEL < 6) {
			party->setJob(CURSEL);
		} else if (CURSEL < 8) {
			party->setJob(CURSEL - 4);
		}
		menuState->popMenu();				// change back to new menu
		return;
	}

	/*  -- selection menu --
	 *  0warrior      1thief
	 *  2monk		  3red					// two identical rows
	 *  4monk		  5red					// to simulate scrolling
	 *  6white		  7black
	 *  --------------------*/

	// check movement keys
	if (input->rightPressed()) {
		if (CURSEL == 3) {
			cursor.setSelection(6);
		} else if (CURSEL < 7) {
			cursor.setSelection(CURSEL + 1);
		}
	} else if (input->leftPressed()) {
		if (CURSEL == 4) {
			cursor.setSelection(1);
		} else if (CURSEL > 0) {
			cursor.setSelection(CURSEL - 1);
		}
	} else if (input->upPressed()) {
		if (CURSEL == 4) {
			cursor.setSelection(0);
		} else if (CURSEL == 5) {
			cursor.setSelection(1);
		} else if (CURSEL > 1) {
			cursor.setSelection(CURSEL - 2);
		}
	} else if (input->downPressed()) {
		if (CURSEL == 2) {
			cursor.setSelection(6);
		} else if (CURSEL == 3) {
			cursor.setSelection(7);
		} else if (CURSEL < 6) {
			cursor.setSelection(CURSEL + 2);
		}
	}

	// input handled
	input->resetAll();
}

void JobMenu::render() {
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

		party->render(c, SPRITEX, SPRITEY[i] + 90);
		twenty.drawText(SPRITEX + 140, SPRITEY[i] + 18, party->getName(c).c_str());
		twenty.drawText(SPRITEX, SPRITEY[i] - 110, party->getJob(c).c_str());
	}

	// render the cursors
	cursorRender(SPRITEX, SPRITEY);
}

void JobMenu::subBorders() {
	// x coordinate of the left side of all the sub borders
	const int LEFTX = -150;

	border(LEFTX, windowWidth, windowHeight, windowHeight - 150);
	border(LEFTX, windowWidth, windowHeight - 150, windowHeight - 650);
	border(LEFTX, windowWidth, windowHeight - 650, windowHeight - 840);
	border(LEFTX, windowWidth, windowHeight - 840, -windowHeight);
}

void JobMenu::subText() {
	// x coordinate of the sub text (except job names)
	const int LEFTX = -100, TEXTY = windowHeight - 745;

	twenty.drawText(LEFTX, windowHeight - 100, "Select a class.");
	switch (cursor.getSelection()) {
		case 1:
			twenty.drawText(LEFTX, TEXTY, 
				"Nimbly runs circles around\nopponents.");
			break;
		case 2:
		case 4:
			twenty.drawText(LEFTX, TEXTY, 
				"Specializes in bare-knuckled\natacks.");
			break;
		case 3:
		case 5:
			twenty.drawText(LEFTX, TEXTY, 
				"Proficient in both black and\nwhite magic.");
			break;
		case 6:
			twenty.drawText(LEFTX, TEXTY, 
				"Supports the party with white\nmagic.");
			break;
		case 7:
			twenty.drawText(LEFTX, TEXTY, 
				"Damages opponents with black\nmagic.");
			break;
		default:
			twenty.drawText(LEFTX, TEXTY, 
				"Can use a wide array of weapons\nand armor.");
			break;
	}
	twenty.drawText(LEFTX, -windowHeight + 30, 
		"A    Confirm                  B    Back");

	// render the actual job menu: sprites and job names
	int centered;
	SDL_Rect r = {0, 0, 0, 0};
	const int SUBCOL = (windowWidth + 150) / 4;	// (sub window width / 4)
	const int TOPSPRITE = windowHeight - 190, BOTSPRITE = windowHeight - 410;
	const int TOPTEXT = windowHeight - 390, BOTTEXT = windowHeight - 610;
	const int LEFTSPRITE = SUBCOL - 200, RIGHTSPRITE = windowWidth - SUBCOL - 100;
	if (cursor.getSelection() > 3) {			// if scrolled down
		// monk top left
		twenty.textSize(party->getJob(Character::MONK).c_str(), &r);
		centered = SUBCOL - 140 - r.w / 2;
		party->render(Character::MONK, LEFTSPRITE, TOPSPRITE);
		twenty.drawText(centered, TOPTEXT, party->getJob(Character::MONK).c_str());

		// red mage top right
		twenty.textSize(party->getJob(Character::RED).c_str(), &r);
		centered = windowWidth - SUBCOL - 40 - r.w / 2;
		party->render(Character::RED, RIGHTSPRITE, TOPSPRITE);
		twenty.drawText(centered, TOPTEXT, party->getJob(Character::RED).c_str());

		// white mage bottom left
		twenty.textSize(party->getJob(Character::WHITE).c_str(), &r);
		centered = SUBCOL - 140 - r.w / 2;
		party->render(Character::WHITE, LEFTSPRITE, BOTSPRITE);
		twenty.drawText(centered, BOTTEXT, party->getJob(Character::WHITE).c_str());

		// black mage bottom right
		twenty.textSize(party->getJob(Character::BLACK).c_str(), &r);
		centered = windowWidth - SUBCOL - 40 - r.w / 2;
		party->render(Character::BLACK, RIGHTSPRITE, BOTSPRITE);
		twenty.drawText(centered, BOTTEXT, party->getJob(Character::BLACK).c_str());
	} else {	// not scrolled down
		// warrior top left
		twenty.textSize(party->getJob(Character::WARRIOR).c_str(), &r);
		centered = SUBCOL - 140 - r.w / 2;
		party->render(Character::WARRIOR, LEFTSPRITE, TOPSPRITE);
		twenty.drawText(centered, TOPTEXT, party->getJob(Character::WARRIOR).c_str());

		// thief top right
		twenty.textSize(party->getJob(Character::THIEF).c_str(), &r);
		centered = windowWidth - SUBCOL - 40 - r.w / 2;
		party->render(Character::THIEF, RIGHTSPRITE, TOPSPRITE);
		twenty.drawText(centered, TOPTEXT, party->getJob(Character::THIEF).c_str());

		// monk bottom left
		twenty.textSize(party->getJob(Character::MONK).c_str(), &r);
		centered = SUBCOL - 140 - r.w / 2;
		party->render(Character::MONK, LEFTSPRITE, BOTSPRITE);
		twenty.drawText(centered, BOTTEXT, party->getJob(Character::MONK).c_str());

		// red mage bottom right
		twenty.textSize(party->getJob(Character::RED).c_str(), &r);
		centered = windowWidth - SUBCOL - 40 - r.w / 2;
		party->render(Character::RED, RIGHTSPRITE, BOTSPRITE);
		twenty.drawText(centered, BOTTEXT, party->getJob(Character::RED).c_str());
	}
}

void JobMenu::cursorRender(const int SPRITEX, const float *SPRITEY) {
	for (int i = 0; i < 4; i++) {
		if (party->getActive() == i) {
			// blinking cursor next to character whos job is being changed
			subCursor.render(SPRITEX, SPRITEY[i] + 48);
		}
	}

	const int CURSEL = cursor.getSelection();// save some typing
	if (CURSEL == 0 || CURSEL == 1 || CURSEL == 4 || CURSEL == 5) {	
		// top row
		if (CURSEL % 2) {	// right column
			cursor.render(windowWidth - 400, windowHeight - 230);
		} else {			// left column
			cursor.render(-100, windowHeight - 230);
		}
	} else {												
		// bottom row
		if (CURSEL % 2) {	// right column
			cursor.render(windowWidth - 400, windowHeight - 460);
		} else {		// left column
			cursor.render(-100, windowHeight - 460);
		}
	}
}
