#ifndef STATUSMENU_H
#define STATUSMENU_H

#include "items.h"
#include "menu.h"

class StatusMenu : public Menu {
	public:
		StatusMenu(int c);
		~StatusMenu();
	private:
		// operations
		void update();
		void render();

		Items items;

		// current character
		Party::Characters character;

		// render borders around the submenus
		void subBorders();

		// draw sprite, name, job, level
		void header();

		// draw hp, mp, mag lvl, exp, next level
		void overview();

		// draw characters equip
		void equip();

		// draw stats (str, agi, int...)
		void stats();
};

#endif
