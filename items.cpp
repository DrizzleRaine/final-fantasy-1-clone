#include "items.h"

Items::Items() {
	std::ifstream in;
	in.open("items.data");

	// get number of items
	in >> itemCount;

	// generate an array of items
	items = new Item[itemCount];

	// fill the array from the file
	int cat, type;
	for (int i = 0; i < itemCount; i++) {
		// go to the next line
		in.ignore(256, '\n');

		// category and type
		in >> cat;
		items[i].category = static_cast<Categories>(cat);
		in >> type;
		items[i].type = static_cast<Types>(type);

		// item worth
		in >> items[i].worth;

		// go to the next line
		in.ignore(256, '\n');

		// name and description
		getline(in, items[i].name, '\n');
		getline(in, items[i].description, '\n');

		// item values
		for (int j = 0; j < 4; j++) {
			in >> items[i].values[j];
		}

	}
	in.close();
}

Items::~Items() {
	delete[] items;
	items = 0;
}

std::string Items::getName(int id) {
	if (!inBounds(id)) {
		return "";
	}
	return items[id].name;
}

std::string Items::getDescription(int id) {
	if (!inBounds(id)) {
		return "";
	}
	return items[id].description;
}

int Items::getWorth(int id) {
	if (!inBounds(id)) {
		return 0;
	}
	return items[id].worth;
}

bool Items::equippable(int id, int job) {
	if (!inBounds(id)) {
		return false;
	}

	if (items[id].type < SWORD) {
		return false;
	}

	if (job == -1) {
		// no job specified and it is equippable by some job
		return true;
	}

	switch(items[id].type) {
		case SWORD:
		case FIST:
		case HAMMER:
		case STAFF:
		case SHIELD:
		case LIGHT_ARM:
		case HEAVY_ARM:
		case HELMET:
		case GLOVE:
		default:
			return true;
	}

	return false;
}

bool Items::inBounds(int id) {
	if (id >= 0 && id < itemCount) {
		return true;
	}
	return false;
}
