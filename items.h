#ifndef ITEMS_H
#define ITEMS_H

#include <string>
#include <fstream>

class Items {
	public:
		Items();
		~Items();

		// equipment slots
		enum Slots {WPN, SHD, HLM, ARM, GLV};

		// name and description
		std::string getName(int id);
		std::string getDescription(int id);

		// cost and worth
		int getCost(int id);
		int getWorth(int id);

		// item types
		enum Types {KEY, HEAL_SINGLE, HEAL_PARTY, CURE_STATUS, 
					WEAPON, SHIELD, HELMET, ARMOR, GLOVE};

		// type
		int getType(int id);

		// is item equippable in given slot, -1 means any slot
		bool equippable(int id, int slot = -1, int job = -1);

		// get value[v]
		int getValue(int id, int v);

		// compare two pieces of equipment
		// returns negative if this < that
		// returns 0 		if this == that
		// returns positive if this > that
		int compare(int thisID, int thatID);
	private:
		struct Item {
			// name and description
			std::string name;
			std::string description;

			int cost;	// how much shop sells item for
			int worth;	// how much player sells item for

			// item type
			int type;

			/* which jobs can equip bits
			 *
			 * war thf wMg bMg mnk rMg
			 * kni nin wWz bWz mst rWz
			 *
			 * 1   2   4   8   16   32
			 * 64  128 256 512 1024 2048
			 *
			 * so 4095 means all can equip, 0 means none
			 * 3 means only war and thf, etc..
			 */
			int equipBits;

			// items have a max of 4 values
			int values[4];
		};

		// array of items in the game
		Item *items;

		// how many different items the game has
		int itemCount;

		// id bounds check
		bool inBounds(int id);
};

#endif
