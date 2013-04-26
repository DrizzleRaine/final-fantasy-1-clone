#ifndef BATTLESTATE_H
#define BATTLESTATE_H

#include <fstream>
#include "state.h"

class BattleState : public State {
	public:
		BattleState(int battleID, int advantage);
		~BattleState();
	private:
		// battle ID
		int battleID;

		// preemptive or surpsise attack?
		bool preemptive, surprise;

		// differenty types of actions player can take
		enum Actions {NONE, ATTACK, SPELL, ITEM, FLEE, COUNT};

		struct Slot {
			// action slot is making
			Actions action;

			// id of action (spellID/itemID)
			int actionID;

			// slot to target
			int targetSlot;
		};

		// slots 0-7 are enemies, 8-11 characters
		enum { SLOTCOUNT = 12 };
		Slot slot[SLOTCOUNT];

		// current slot picking an action
		int currentSlot;

		struct Enemy {
			// enemy name
			std::string name;

			int HP;

			// associated textureID for enemy
			int textureID;
		};

		// 8 slots for enemies
		enum { ENEMYSLOTS = 8 };
		Enemy enemy[ENEMYSLOTS];

		// background and enemy sprites
		Textures textures;
		int backgroundTexID;

		// operations
		void update();
		void render();

		// is the battle over
		bool battleOver();

		// pause/unpause battle
		void pause();
		void unpause();
};

#endif
