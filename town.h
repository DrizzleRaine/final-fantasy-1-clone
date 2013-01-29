#ifndef TOWN_H
#define TOWN_H

#include "map.h"
#include "dialog.h"
#include "npc.h"

class Town : public Map {
	public:
		Town(int mapID);
		~Town();
	private:
		// initialization
		void init();

		// operations
		void update();
		void render();

		// NPCs
		NPC npcs;

		// dialog
		Dialog dialog;
};

#endif
