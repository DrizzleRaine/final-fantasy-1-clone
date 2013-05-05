#ifndef BATTLESTATE_H
#define BATTLESTATE_H

#include <fstream>
#include <map>

#include "state.h"
#include "menustate.h"
#include "battlebgmenu.h"

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

		struct Turn {
			// action to take during turn
			Actions action;

			// id of action (spellID/itemID)
			int actionID;

			// action target
			int target;
		};

		// slots 0-7 are enemies, 8-11 characters
		enum { SLOTCOUNT = 12 };
		Turn slot[SLOTCOUNT];

		// current slot picking an action
		int currentSlot;

		struct Enemy {
			// enemy name
			std::string name;

			int HP, ATK, ACC, CRT, DEF, EVA, MDEF, MOR;

			int EXP, GIL;

			// associated textureID for enemy
			int textureID;
		};

		// 8 slots for enemies
		enum { ENEMYSLOTS = 8 };
		Enemy enemy[ENEMYSLOTS];
		bool enemyLocs[ENEMYSLOTS];

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

		// menu state for character turns
		MenuState *menuState;

		// bottom battle menu background
		// enemies and characters list
		BattleBGMenu *battleBGMenu;
};

#endif
