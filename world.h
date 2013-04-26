#ifndef WORLD_H
#define WORLD_H

#include "map.h"

class World : public Map {
	public:
		World();
		~World();
	private:
		// initialization
		void init();

		// operations
		void update();
		void render();

		// pause/unpause
		void pause();
		void unpause();

		// steps to next encounter
		int nextEncounter;
};

#endif
