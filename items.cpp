#include "items.h"

Items::Items() {
	std::ifstream in;
	in.open("items.data");

	int itemCount;
	in >> itemCount;

	for (int i = 0; i < itemCount; i++) {
		in.ignore(256, '\n');	// go to the next line
		getline(in, items[i].name, '\n');
		getline(in, items[i].description, '\n');

		int tempType;
		in >> tempType;
		items[i].type = static_cast<Types>(tempType);

		switch (items[i].type) {
			case HEAL_SINGLE:
			case HEAL_PARTY:
				in >> items[i].values[0];
				in >> items[i].values[1];
				break;
			case HEAL_STATUS:
				in >> items[i].values[0];
				break;
			default:
				in >> items[i].values[0];
				in >> items[i].values[1];
				in >> items[i].values[2];
				in >> items[i].values[3];
				break;
		}
	}
	in.close();
}

Items::~Items() {
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
	if (id >= 0 && id < 256) {
		return true;
	}
	return false;
}
