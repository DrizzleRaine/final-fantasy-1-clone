#ifndef ITEMS_H
#define ITEMS_H

#include <string>
#include <fstream>

// what i should do is inherit from this class
// with a bunch of sub classes for each type of item

class Items {
	public:
		Items();
		~Items();

		std::string getName(int id);
		std::string getDescription(int id);
	private:
		// item types
		enum Types {NONE, HEAL_SINGLE, HEAL_PARTY, HEAL_STATUS, SWORD, FIST, 
			HAMMER, STAFF, SHIELD, LIGHT_ARM, HEAVY_ARM, HELMET, GLOVE};

		struct Item {
			std::string name;
			std::string description;
			Types type;

			// todo specific types of items have different fields
			// should be reprsented by different classes
			// right now there are up to 4 values
			// for HEAL_SINGLE/HEAL_PARTY: HP and MP values
			// for HEAL_STATUS: which status it heals
			// for weapons and armor: atk, acc, def, eva
			int values[4];
		};

		Item items[256];

		// id bounds check
		bool inBounds(int id);
};

#endif
