#ifndef BATTLEMAINMENU_H
#define BATTLEMAINMENU_H

#include "menu.h"

class BattleMainMenu : public Menu {
	public:
		BattleMainMenu(Party::Characters c, bool *enemyLocs);
		~BattleMainMenu();
	private:
		// operations
		void update();
		void render();

		Font sixteen;

		enum { ENEMYSLOTS = 8 };
		bool enemyLocations[ENEMYSLOTS];

		// character who is acting
		Party::Characters character;

		// blinking sub cursor
		Cursor subCursor;

		// the menu options available in item menu
		enum Options {NONE = -1, ATTACK, MAGIC, ITEMS, EQUIP, FLEE, COUNT};
	
		// the currently active menu option	
		Options currentOption;

		// render all the text
		void renderText();

		// render the cursor(s)
		void cursorRender();
};

#endif
