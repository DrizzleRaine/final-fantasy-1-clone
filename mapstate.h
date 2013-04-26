#ifndef MAPSTATE_H
#define MAPSTATE_H

#include "state.h"
#include "menustate.h"
#include "battlestate.h"

#include "map.h"
class Map;

class MapState : public State {
	public:
		MapState();
		~MapState();

		// push, pop or change maps
		void pushMap(Map *map);
		void popMap();
		void changeMap(Map *map);

		// push MenuState on top of state stack
		// and return the new MenuState on top
		MenuState *enterMenu();

		// push BattleState on top of state stack
		// and return the new BattleState on top
		BattleState *enterBattle(int battleID);
	private:
		// operations
		void update();
		void render();

		// pause/unpause menu
		void pause();
		void unpause();

		// head and tail of linked list of maps
		// the linked list is treated like a stack
		Map *head, *tail;
};

#endif
