#ifndef NAMEMENU_H
#define NAMEMENU_H

#include "menu.h"

class NameMenu : public Menu {
	public:
		NameMenu(std::string name);
		~NameMenu();
	private:
		//operations
		void update();
		void render();

		// blinking sub cursor
		Cursor subCursor;

		// the new name for the character
		std::string newName;

		// render borders around the submenus
		void subBorders();

		// text for the submenus
		void subText();

		// box around selected letter
		void letterCursor();
};

#endif
