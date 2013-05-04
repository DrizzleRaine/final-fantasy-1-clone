#ifndef BATTLEBGMENU_H
#define BATTLEBGMENU_H

#include "gui.h"
#include "party.h"

class BattleBGMenu : public GUI {
	public:
		BattleBGMenu();
		~BattleBGMenu();

		void render(int windowWidth, int windowHeight, Party *party, std::string enemyNames, std::string enemyCounts);
	private:
		Font sixteen;
};

#endif
