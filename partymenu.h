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

		// render borders around the submenus
		void subBorders();

		// text for the submenus
		void subText();

		// render the cursor(s)
		void cursorRender(const int SPRITEX, const float *SPRITEY);
};

#endif
