#ifndef BATTLESTATE_H
#define BATTLESTATE_H

#include <fstream>
#include <map>

#include "state.h"
#include "enemy.h"
#include "menustate.h"
#include "battlebgmenu.h"

class BattleState : public State {
	public:
		BattleState(int battleID, int advantage);
		~BattleState();
	private:
		// battle ID
		int battleID;

		// operations
		void update();
		void render();

		// preemptive or surpsise attack?
		bool preemptive, surprise;

		// slots 0-7 are enemies, 8-11 characters
		enum { CHARACTERSLOTS = 4, ENEMYSLOTS = 8, SLOTCOUNT = 12 };

		// current slot picking an action
		int currentSlot;

		// array of enemies, textures and locations
		Enemy enemy[ENEMYSLOTS];
		bool enemyLocs[ENEMYSLOTS];

		// character/enemy turns being executed
		bool executingTurns;

		// order of character/enemy turns
		int attackSequence[SLOTCOUNT];

		// enemy decide/act
		void enemyDecide();
		void enemyAct();

		// character decide/act
		void characterDecide();
		void characterAct();

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

		// battlefield background
		Textures textures;
};

#endif
