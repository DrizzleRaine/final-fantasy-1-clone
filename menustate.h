#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "state.h"

#include "menu.h"
class Menu;

class MenuState : public State {
	public:
		MenuState();
		~MenuState();

		// push, pop or change menus
		void pushMenu(Menu *menu);
		void popMenu();
		void changeMenu(Menu *menu);

		// exit all menus, startGame flag for
		// starting the game from party creation menu
		void exitMenus(bool startGame = 0);
	private:
		// operations
		void update();
		void render();

		// head and tail of linked list of menus
		// the linked list is treated like a stack
		Menu *head, *tail;
};

#endif
