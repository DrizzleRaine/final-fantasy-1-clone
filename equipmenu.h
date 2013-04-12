#ifndef EQUIPMENU_H
#define EQUIPMENU_H

#include "items.h"
#include "menu.h"

class EquipMenu : public Menu {
	public:
		EquipMenu(int c);
		~EquipMenu();
	private:
		// operations
		void update();
		void render();

		Items items;

		// current character
		Party::Characters character;

		// current item id pointed at by cursor
		int curItemID;

		// blinking sub cursor
		Cursor subCursor;

		// the menu options available in item menu
		enum Options {NONE = -1, EQUIP = 0, OPTIMAL, REMOVE, COUNT};
	
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
