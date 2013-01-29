#ifndef JOBMENU_H
#define JOBMENU_H

#include "menu.h"

class JobMenu : public Menu {
	public:
		JobMenu();
		~JobMenu();
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
