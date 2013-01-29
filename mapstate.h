#ifndef MAPSTATE_H
#define MAPSTATE_H

#include "state.h"

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

		// push given menu on top of state stack
		void enterMenu();
	private:
		// operations
		void update();
		void render();

		// head and tail of linked list of maps
		// the linked list is treated like a stack
		Map *head, *tail;
};

#endif
