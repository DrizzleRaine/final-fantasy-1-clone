#ifndef USESPELLMENU_H
#define USESPELLMENU_H

#include "spells.h"
#include "menu.h"

class UseSpellMenu : public Menu {
	public:
		UseSpellMenu(int spellID, Party::Characters c);
		~UseSpellMenu();
	private:
		// operations
		void update();
		void render();

		// spellID to use
		int spellID;

		// character casting the spell
		Party::Characters caster;

		Spells spells;

		// render borders around the submenus
		void subBorders();

		// text for the submenus
		void subText();

		// render the cursor(s)
		void cursorRender(const int SPRITEX, const float *SPRITEY);
};

#endif
