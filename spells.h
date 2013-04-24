#ifndef SPELLS_H
#define SPELLS_H

#include <string>
#include <fstream>

class Spells {
	public:
		Spells();
		~Spells();

		// name and description
		std::string getName(int id);
		std::string getDescription(int id);

		// level and mp cost
		int getLevel(int id);
		int getMPCost(int id);

		// spell types
		enum Types {HEAL_SINGLE, HEAL_PARTY, ADD_STATUS_SINGLE, ADD_STATUS_ALL, 
			REMOVE_STATUS_SINGLE, REMOVE_STATUS_ALL, DAMAGE_SINGLE, DAMAGE_ALL, 
			DAMAGE_SINGLE_UNDEAD, DAMAGE_ALL_UNDEAD, INFLICT_STATUS_SINGLE, 
			INFLICT_STATUS_ALL, MISC};

		// type
		int getType(int id);

		// is spell learnable by given job, -1 means any job
		bool learnable(int id, int job = -1);

		// can be used in menu
		bool menuUse(int id);

		// cast spell
		// return amount healed/damage done/status removed/added
		int cast(int id);
	private:
		struct Spell {
			// name and description
			std::string name;
			std::string description;

			// spell type
			int type;

			/* which jobs can learn bits
			 *
			 * war thf wMg bMg mnk rMg
			 * kni nin wWz bWz mst rWz
			 *
			 * 1   2   4   8   16   32
			 * 64  128 256 512 1024 2048
			 *
			 * so 4095 means all can learn, 0 means none
			 * 260 means only white mage and white wizard, etc..
			 */
			int learnBits;

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
		unsigned int spellCount;

		// id bounds check
		bool inBounds(int id);
};

#endif
