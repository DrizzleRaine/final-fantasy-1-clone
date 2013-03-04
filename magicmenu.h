#ifndef MAGICMENU_H
#define MAGICMENU_H

#include "spells.h"
#include "menu.h"

class MagicMenu : public Menu {
	public:
		MagicMenu(int c);
		~MagicMenu();
	private:
		// operations
		void update();
		void render();

		Spells spells;

		// current character
		Party::Characters character;

		// current spell id pointed at by cursor
		int curSpellID;

		// blinking sub cursor
		Cursor subCursor;

		// the menu options available in item menu
		enum Options {NONE = -1, USE = 0, DISCARD, COUNT};
	
		// the currently active menu option	
		Options currentOption;

		// render borders around the submenus
		void subBorders();

		// render all the text
		void renderText();

		// current character window
		void charInfo();

		// render the cursor(s)
		void cursorRender();
};

#endif
