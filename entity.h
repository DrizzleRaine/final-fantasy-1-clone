#ifndef ENTITY_H
#define ENTITY_H

#include <string>

class Entity {
	public:
		Entity();
		virtual ~Entity();

		// entity stats
		enum Stats {LEVEL, EXP, GIL, MAGLEVEL, HP, MP, HPMAX, MPMAX,
			STR, AGL, INT, STA, LCK, ATK, ACC, DEF, EVA, MDEF, CRT, MOR, 
			STATSCOUNT};

		// set entity attribute
		void setAttribute(Stats s, int val);

		// heal amount hp/mp , return amount healed
		int addHP(int amount);
		int addMP(int amount);

		// set/get entity name
		void setName(std::string n) { name = n; }
		std::string getName() const { return name; }

		// actions entity can take in battle
		enum Actions {NONE, ATTACK, SPELL, ITEM, FLEE, COUNT};
		
		// battle turn
		struct Turn {
			// action to take
			Actions action;

			// action target
			int target;

			// id of action (spellID/itemID)
			int actionID;
		};

		// set/reset/get entity turn
		void setTurn(Turn t) { turn = t; }
		void resetTurn();
		Turn getTurn() const { return turn; }

		// change target without changing action
		void setTarget(int target);

		// attack given target on next turn
		void attack(int target);
	protected:
		// every entity has attributes
		int attributes[STATSCOUNT];
	private:
		// entities name
		std::string name;

		// the entities turn
		Turn turn;
};

#endif
