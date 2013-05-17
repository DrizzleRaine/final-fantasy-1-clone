#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <SDL/SDL.h>

#include "font.h"

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

		// status effects
		enum Status {KO = 1, POISON, DARKNESS, SILENCE, STONE};

		// check if entity has status
		bool hasStatus(unsigned int status);

		// set/remove status, return true if successful
		bool setStatus(unsigned int status);
		bool removeStatus(unsigned int status);

		// heal amount hp/mp , return amount healed
		int addHP(int amount);
		int addMP(int amount);

		// render most recent hp/mp change
		void renderHPMPChange();
		bool renderingHPMPChange() { return textTicks; }

		// set/get entity name
		void setName(std::string n) { name = n; }
		std::string getName() const { return name; }

		// get entity sprite dimensions
		int getSpriteDim() { return spriteDim; }

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

		// status effect bits
		int statusBits;

		// ticks when entity began animating
		unsigned int animateTicks;

		// current rendered x, y position
		int renderx, rendery;

		// entity sprite dimensions
		int spriteDim;
	private:
		// entities name
		std::string name;

		// twenty point font
		Font twenty;

		// the entities turn
		Turn turn;

		// ticks when text was displayed
		unsigned int textTicks;

		// change in HP/MP to display
		int change;

		// current color to display text in
		enum textColors {WHITE, RED, GREEN, BLUE, YELLOW, PURPLE};
		int textColor;
};

#endif
