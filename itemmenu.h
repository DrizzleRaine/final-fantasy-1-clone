#ifndef ITEMMENU_H
#define ITEMMENU_H

#include "items.h"
#include "menu.h"

class ItemMenu : public Menu {
	public:
		ItemMenu();
		~ItemMenu();
	private:
		// operations
		void update();
		void render();

		Items items;

		// blinking sub cursor
		Cursor subCursor;

		// item position swaps
		Cursor subCursorSwap;

		// how many items scrolled down currently
		// (# rolls scrolled down) * (2 columns)
		int scrolled;

		// currently selected item
		int itemSelected;

		// the menu options available in item menu
		enum Options {NONE = -1, USE = 0, SORT, KEYITEMS, COUNT};
	
		// the currently active menu option	
		Options currentOption;

		// render borders around the submenus
		void subBorders();

		// render all the text
		void renderText();

		// render the cursor(s)
		void cursorRender();
};

#endif
