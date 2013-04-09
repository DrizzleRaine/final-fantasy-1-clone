#ifndef ITEMS_H
#define ITEMS_H

#include <string>
#include <fstream>

class Items {
	public:
		Items();
		~Items();

		std::string getName(int id);
		std::string getDescription(int id);

		int getWorth(int id);
		bool equippable(int id, int job = -1);
		void getValues(int id, int *v) { v = items[id].values; }
	private:
		// item categories
		enum Categories {KEY, HEAL, CURE, EQUIPMENT};

		// item types
		enum Types {NONE, HEAL_SINGLE, HEAL_PARTY, CURE_STATUS, SWORD, FIST, 
			HAMMER, STAFF, SHIELD, LIGHT_ARM, HEAVY_ARM, HELMET, GLOVE};

		struct Item {
			// item category and type
			Categories category;
			Types type;

			// sell value to shops
			int worth;

			// name and description
			std::string name;
			std::string description;

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
