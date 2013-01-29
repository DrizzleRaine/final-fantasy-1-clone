#ifndef MENU_H
#define MENU_H

#include "gui.h"
#include "cursor.h"

#include "menustate.h"
class MenuState;

class Menu : public GUI {
	public:
		Menu();
		virtual ~Menu();

		// initialize shared resources
		void init(Party *p, Input *in, MenuState *ms);

		// linked list node operations
		Menu *getNext() { return next; }
		Menu *getPrev() { return prev; }
		void setNext(Menu *menu) { next = menu; }
		void setPrev(Menu *menu) { prev = menu; }

		// handle menu operations
		void updateMenu();
		void renderMenu(int width, int height);
	protected:
		// every menu inherits access to the input
		Input *input;

		// every menu inherits access to the party
		Party *party;

		// every menu has a cursor
		Cursor cursor;

		// menus can push/pop/change menu stack
		MenuState *menuState;
	private:
		// menu operations
		virtual void update() = 0;
		virtual void render() = 0;

		// next and prev menus
		Menu *next, *prev;
};

#endif
