#ifndef SPELLS_H
#define SPELLS_H

#include <string>
#include <fstream>

class Spells {
	public:
		Spells();
		~Spells();

		std::string getName(int id);
		std::string getDescription(int id);
		bool menuUse(int id);

		int getLevel(int id);
		int getMPCost(int id);
	private:
		struct Spell {
			// name and description
			std::string name;
			std::string description;

			// spell types
			enum Types {HEAL_SINGLE, HEAL_PARTY, ADD_STATUS_SINGLE, ADD_STATUS_ALL, 
				REMOVE_STATUS_SINGLE, REMOVE_STATUS_ALL, DAMAGE_SINGLE, DAMAGE_ALL, 
				DAMAGE_SINGLE_UNDEAD, DAMAGE_ALL_UNDEAD, INFLICT_STATUS_SINGLE, 
				INFLICT_STATUS_ALL, MISC};

			// spell type
			Types type;

			// can the spell be used in menus
			bool menu;
		
			// magic level required, mp cost
			int level, mp;

			// heal/damage power, status effect, % success
			int power, status, success;
		};

		// array of spells in the game
		Spell *spells;

		// how many different spells the game has
		int spellCount;

		// id bounds check
		bool inBounds(int id);
};

#endif
