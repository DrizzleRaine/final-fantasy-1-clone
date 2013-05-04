#include "battlebgmenu.h"

BattleBGMenu::BattleBGMenu() {
	sixteen.initFont("fonts/VarelaRound-Regular.ttf", 16);
}

BattleBGMenu::~BattleBGMenu() {
}

void BattleBGMenu::render(int windowWidth, int windowHeight, Party *party, std::string enemyNames, std::string enemyCounts) {
	// blue background and border bottom of battle area
	window(-windowWidth, windowWidth, -windowHeight + 200, -windowHeight);
	window(-100, windowWidth, -windowHeight + 200, -windowHeight);

	// render names, hp, mp
	for (int i = Party::FIRST; i < Party::SIZE; i++) {
		Party::Characters c = static_cast<Party::Characters>(i);

		int yPos = -windowHeight + 138 - 40 * i;

		// character name
		sixteen.drawText(-80, yPos, party->getName(c).c_str());

		// get hp fraction and mp amount
		std::string hpFraction = party->getHPFraction(c);
		std::string mpAmount = std::to_string(party->getAttribute(c, Character::MP));

		// hp/mp is right aligned
		const int RIGHTEDGE = windowWidth - 40;
		SDL_Rect r = {0, 0, 0, 0};

		// draw the characters current mp right aligned
		sixteen.textSize(mpAmount.c_str(), &r);
		sixteen.drawText(RIGHTEDGE - r.w, yPos, mpAmount.c_str());

		// draw the hp fraction right aligned
		sixteen.textSize(hpFraction.c_str(), &r);
		sixteen.drawText(RIGHTEDGE - 200 - r.w, yPos, hpFraction.c_str());
	}

	// render enemy names and counts
	sixteen.drawText(-windowWidth + 20, -windowHeight + 138, enemyNames.c_str());
	sixteen.drawText(-150, -windowHeight + 138, enemyCounts.c_str());
}
