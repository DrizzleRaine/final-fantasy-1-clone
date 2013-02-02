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
		items[i].type = static_cast<Item::Types>(type);

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

bool Items::inBounds(int id) {
	if (id >= 0 && id < itemCount) {
		return true;
	}
	return false;
}
