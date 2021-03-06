#ifndef PARTYMENU_H
#define PARTYMENU_H

#include "menu.h"

class PartyMenu : public Menu {
	public:
		PartyMenu();
		~PartyMenu();
	private:
		// operations
		void update();
		void render();

		// blinking sub cursor
		Cursor subCursor;

		// character position swaps
		Cursor subCursorSwap;

		// two characters being swapped
		bool swappingCharacters;

		// the menu options available in party menu
		enum Options {NONE = -1, ITEMS = 0, 
			MAGIC, EQUIPMENT, STATUS, FORMATION, CONFIG, SAVE, COUNT};
	
		// the currently active menu option	
		Options currentOption;

		// render borders around the submenus
		void subBorders();

		// text for the submenus
		void subText();

		// render the cursor(s)
		void cursorRender(const int SPRITEX, const float *SPRITEY);
};

#endif
