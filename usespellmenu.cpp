#include "usespellmenu.h"

UseSpellMenu::UseSpellMenu(int spellID, Party::Characters c) {
	this->spellID = spellID;
	caster = c;
}

UseSpellMenu::~UseSpellMenu() {
}

void UseSpellMenu::update() {
	const int CURSEL = cursor.getSelection();
	int newCurSel = CURSEL;

	// exit menu or cancel option
	if (input->getCancel()) {
		input->resetCancel();	// reset cancel input
		menuState->popMenu();	// exit usespellmenu
		return;					// return immediately
	}

	// character selected
	if (input->getConfirm()) {
		input->resetConfirm();

		// character to use spell on
		Party::Characters c = static_cast<Party::Characters>(CURSEL);

		// attempt to use spell
		int spellType = spells.getType(spellID);
		if (spellType == Spells::HEAL_SINGLE) {
			// attempt to heal the character
			if (party->addHP(c, spells.cast(spellID))) {
				// if hp was healed, remove mp from caster
				party->addMP(caster, -1 * spells.getMPCost(spellID));
			}
		}

		// check if enough MP left to continue casting spell
		if (party->getAttribute(caster, Character::MP) < spells.getMPCost(spellID)) {
			// out of MP, return to magic menu
			menuState->popMenu();
			return;
		}
	}

	// cursor movement
	if (input->downPressed()) {
		newCurSel++;
	} else if (input->upPressed()) {
		newCurSel--;
	}

	// check for wrap wround
	if (newCurSel > Party::FOURTH) {
		newCurSel = Party::FIRST;
	} else if (newCurSel < Party::FIRST) {
		newCurSel = Party::FOURTH;
	}

	// update cursor selection
	cursor.setSelection(newCurSel);

	// input handled
	input->resetAll();
}

void UseSpellMenu::render() {
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
		party->render(c, SPRITEX - 20 , SPRITEY[i] + 50);

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

void UseSpellMenu::subBorders() {
	const int LEFTX = windowWidth - 550;
	border(LEFTX, windowWidth, windowHeight, windowHeight - 180);
	border(LEFTX, windowWidth, windowHeight - 180, windowHeight - 560);
	border(LEFTX, windowWidth, windowHeight - 560, -windowHeight);
}

void UseSpellMenu::subText() {
	const int LINEHEIGHT = twenty.getLineSkip();

	const int LEFTWIDTH = 550;				// width of left subwindows
	const int LEFTX = windowWidth - 480;	// left edge for text
	const int ITEMY = windowHeight - 120;	// y for spell name
	const int DESCY	= windowHeight - 280;	// y for spell description
	const int DESCWIDTH = 410;				// width available for desriptions

	// to store text dimensions
	SDL_Rect r = {0, 0, 0, 0};

	// draw spell name centered
	std::string spellName = spells.getName(spellID);
	twenty.textSize(spellName.c_str(), &r);
	twenty.drawText(windowWidth - (LEFTWIDTH / 2) - (r.w / 2), ITEMY, 
			spellName.c_str());

	// draw description in description area
	// add line breaks in between words when need to fit

	// get spell description, and number of characters
	std::string desc = spells.getDescription(spellID);
	int descSize = desc.size();

	// substr that fits on current line so far
	std::string curLine;

	// loop through string
	int i, j;
	for (i = 0, j = 0; i <= descSize; i++) {
		// if end of string, space or newline found
		if (!desc[i] || desc[i] == ' ' || desc[i] == '\n') {
			// add the next word to curLine
			curLine += desc.substr(j, i - j);;

			// check if curLine still fits within description area
			twenty.textSize(curLine.c_str(), &r);
			if (r.w > DESCWIDTH) {
				// doesnt fit, insert a newline before the word
				desc[j] = '\n';

				// move to new line and add to it the
				// word that wouldn't fit on the last
				curLine = desc.substr(j, i - j);
			}
			j = i;
		}
	}

	// draw description that will now fit
	twenty.drawText(LEFTX, DESCY, desc.c_str());

	const int CASTERY = -windowHeight + 352;	// y for caster sprite
	const int NAMELVLX = LEFTX + 150;			// x for caster name and level
	const int NAMEY = CASTERY - 60;				// y for caster name
	const int LEVELY = CASTERY - 150;			// y for caster level
	const int MPY = CASTERY - 240;				// y for caster MP label
	const int MPX = LEFTX + 120;				// x for caster MP fraction

	// draw caster
	party->render(caster, LEFTX - 20, CASTERY);

	// caster name
	twenty.drawText(NAMELVLX, NAMEY, party->getName(caster).c_str());

	// caster level
	std::string lvl = std::to_string(party->getAttribute(caster, Character::LEVEL));
	twenty.drawText(NAMELVLX, LEVELY, ("Lv. " + lvl).c_str());

	// caster MP
	std::string MPFraction = party->getMPFraction(caster);
	twenty.drawText(LEFTX, MPY, "MP");
	twenty.drawText(MPX, MPY - LINEHEIGHT, MPFraction.c_str());
}

void UseSpellMenu::cursorRender(const int SPRITEX, const float *SPRITEY) {
	// cursor is pointing at character to use spell on
	int curY = SPRITEY[cursor.getSelection()];
	cursor.render(SPRITEX - 20, curY);
}
