#include "statusmenu.h"

StatusMenu::StatusMenu(int c) {
	character = static_cast<Party::Characters>(c);
}

StatusMenu::~StatusMenu() {
}

void StatusMenu::update() {
	// exit menu or cancel option
	if (input->getCancel()) {
		input->resetCancel();
		menuState->popMenu();		// exit item menu
		return;						// retun immediately
	} else if (input->getRButton()) {
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

	// input handled
	input->resetAll();
}

void StatusMenu::render() {
	// blue background and border entire window
	blueBackground(windowWidth, windowHeight);

	// borders around submenus
	subBorders();

	// current menu, sprite, name, ...
	header();

	// hp, mp, level, exp, ...
	overview();

	// weapon, shield, ...
	equip();

	// str, agl, int, ...
	stats();
}

void StatusMenu::subBorders() {
	const int LEFTX = windowWidth - 400;
	border(-windowWidth, windowWidth, windowHeight, -windowHeight);
	border(LEFTX, windowWidth, windowHeight, windowHeight - 124);
}

void StatusMenu::header() {
	// draw current menu centered in upper right corner
	SDL_Rect r = {0, 0, 0, 0};
	twenty.textSize("Status", &r);
	twenty.drawText(windowWidth - (400 / 2) - (r.w / 2), 
			windowHeight - 90, "Status");

	// draw character sprite
	party->render(character, -windowWidth + 80, windowHeight - 70);	

	// character name and job name
	twenty.drawText(-windowWidth + 250, windowHeight - 140, 
			party->getName(character).c_str());
	twenty.drawText(-150, windowHeight - 140, 
			party->getJob(character).c_str());

	// level
	std::string level = "Lv. " + 
		std::to_string(party->getAttribute(character, Character::LEVEL));
	twenty.drawText(-windowWidth + 250, windowHeight - 226, level.c_str());
}

void StatusMenu::overview() {
	const int TOPEDGE = windowHeight - 320;
	const int LEFTEDGE = -windowWidth + 80;
	const int LINEHEIGHT = twenty.getLineSkip();

	// hp/mp
	twenty.drawText(LEFTEDGE, TOPEDGE, "HP\nMP");
	std::string hpFraction = party->getHPFraction(character);
	std::string mpFraction = party->getMPFraction(character);

	// right align hp/mp fractions
	int rightEdge = -windowWidth + 470;
	SDL_Rect r = {0, 0, 0, 0};

	// hp fraction
	twenty.textSize(hpFraction.c_str(), &r);
	twenty.drawText(rightEdge - r.w, TOPEDGE, hpFraction.c_str());

	// mp fraction
	twenty.textSize(mpFraction.c_str(), &r);
	twenty.drawText(rightEdge - r.w, TOPEDGE - LINEHEIGHT, mpFraction.c_str());

	// labels
	rightEdge = -150;
	twenty.drawText(LEFTEDGE, TOPEDGE - LINEHEIGHT * 2, "Magic Level");
	twenty.drawText(LEFTEDGE, TOPEDGE - LINEHEIGHT * 3, "Current EXP");
	twenty.drawText(LEFTEDGE, TOPEDGE - LINEHEIGHT * 5, "Next Level");

	// values, right aligned
	rightEdge = -100;

	std::string value;

	// magic level
	value = std::to_string(party->getAttribute(character, Character::MAGLEVEL));
	twenty.textSize(value.c_str(), &r);
	twenty.drawText(rightEdge - r.w, TOPEDGE - LINEHEIGHT * 2, value.c_str());

	// current exp
	value = std::to_string(party->getAttribute(character, Character::EXP));
	twenty.textSize(value.c_str(), &r);
	twenty.drawText(rightEdge - r.w, TOPEDGE - LINEHEIGHT * 4, value.c_str());

	// exp to next level
	twenty.textSize("0", &r);
	twenty.drawText(rightEdge - r.w, TOPEDGE - LINEHEIGHT * 6, "0");
}

void StatusMenu::equip() {
	const int LINEHEIGHT = twenty.getLineSkip();
	const int TOPEDGE = windowHeight - 320 - LINEHEIGHT * 7;
	const int LEFTEDGE = -windowWidth + 80;

	// equipment slot labels
	std::string slots[] = {"WPN", "SHD", "HLM", "ARM", "GLV"};

	// display each label and current equipment for this character
	for (int i = 0; i < 5; i++) {
		twenty.drawText(LEFTEDGE, TOPEDGE - (LINEHEIGHT - 12) * i, 
				slots[i].c_str());
	}

}

void StatusMenu::stats() {
	const int TOPEDGE = windowHeight - 320;
	const int LEFTEDGE = windowWidth - 500;

	// right align values
	SDL_Rect r = {0, 0, 0, 0};
	const int RIGHTEDGE = windowWidth - 130;

	// stat labels
	std::string stats[] = {"STR", "AGL", "INT", "STA", "LCK", 
							"\nATK", "\nACC", "\nDEF", "\nEVA"};

	// display each label and stat value for this character
	for (int i = 0; i < 9; i++) {
		// draw current stat label
		twenty.drawText(LEFTEDGE, TOPEDGE - 65 * i, stats[i].c_str());

		std::string value;
		if (i > 4) {
			// skip a line after 5th
			value = '\n';
			twenty.drawText(LEFTEDGE + 200, TOPEDGE - 65 * i, "\n...");
		} else {
			twenty.drawText(LEFTEDGE + 200, TOPEDGE - 65 * i, "...");
		}

		// retrieve characters stat value
		Character::Stats stat = static_cast<Character::Stats>(Character::STR + i);
		value = value + std::to_string(party->getAttribute(character, stat));

		// display the value
		twenty.textSize(value.c_str(), &r);
		twenty.drawText(RIGHTEDGE - r.w, TOPEDGE - 65 * i, value.c_str());
	}
}
