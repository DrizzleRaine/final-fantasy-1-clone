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
	private:
		// item categories
		enum Categories {KEY, HEAL, CURE, EQUIPMENT};

		struct Item {
			// item types
			enum Types {KEY, HEAL_SINGLE, HEAL_PARTY, CURE_STATUS, SWORD, FIST, 
				HAMMER, STAFF, SHIELD, LIGHT_ARM, HEAVY_ARM, HELMET, GLOVE};

			// item category and type
			Categories category;
			Types type;

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
