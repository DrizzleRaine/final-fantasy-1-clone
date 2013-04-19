#ifndef USEITEMMENU_H
#define USEITEMMENU_H

#include "items.h"
#include "menu.h"

class UseItemMenu : public Menu {
	public:
		UseItemMenu(int itemID);
		~UseItemMenu();
	private:
		// operations
		void update();
		void render();

		// itemID to use
		int itemID;

		Items items;

		// render borders around the submenus
		void subBorders();

		// text for the submenus
		void subText();

		// render the cursor(s)
		void cursorRender(const int SPRITEX, const float *SPRITEY);
};

#endif
