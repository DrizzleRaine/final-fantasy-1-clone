#ifndef SHOPMENU_H
#define SHOPMENU_H

#include "menu.h"

class ShopMenu : public Menu {
	public:
		ShopMenu();
		~ShopMenu();
	private:
		// operations
		void update();
		void render();

		// blinking sub cursor
		Cursor subCursor;

		// the menu options available in shop menu
		enum Options {NONE = -1, BUY = 0, SELL, EXIT, COUNT};
	
		// the currently active menu option	
		Options currentOption;

		// render borders around the submenus
		void subBorders();

		// text for the submenus
		void subText();

		// render the cursor(s)
		void cursorRender();
};

#endif
