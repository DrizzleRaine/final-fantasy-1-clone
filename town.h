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

		// pause/unpause
		void pause();
		void unpause();

		// NPCs
		NPC npcs;

		// dialog
		Dialog dialog;
};

#endif
