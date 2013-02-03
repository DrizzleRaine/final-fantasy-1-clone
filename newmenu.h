#ifndef NEWMENU_H
#define NEWMENU_H

#include "menu.h"
#include "jobmenu.h"
#include "namemenu.h"

class NewMenu : public Menu {
	public:
		NewMenu();
		~NewMenu();
	private:
		// operations
		void update();
		void render();

		// blinking sub cursor
		Cursor subCursor;

		// display start game confirmation
		bool startConfirm;

		// ticks when forgot to name character(s)
		unsigned int startError;

		// render borders around the submenus
		void subBorders();

		// text for the submenus
		void subText();

		// render the cursor(s)
		void cursorRender(const int SPRITEX, const float *SPRITEY);
};

#endif
