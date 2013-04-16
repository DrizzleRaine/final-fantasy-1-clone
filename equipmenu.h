#ifndef EQUIPMENU_H
#define EQUIPMENU_H

#include <vector>

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

		// vector of item IDs in party inventory that can
		// be equipped by the current job in current slot
		std::vector<int> equippable;

		// blinking sub cursor
		Cursor subCursor;

		// changing current equip
		Cursor subCursorEquip;

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
