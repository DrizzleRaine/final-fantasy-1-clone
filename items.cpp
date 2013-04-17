#include "items.h"

Items::Items() {
	std::ifstream in;
	in.open("items.data");

	// get number of items
	in >> itemCount;

	// generate an array of items
	items = new Item[itemCount];

	// fill the array from the file
	for (int i = 0; i < itemCount; i++) {
		// go to the next line
		in.ignore(256, '\n');

		// name and description
		getline(in, items[i].name, '\n');
		getline(in, items[i].description, '\n');

		// cost and worth
		in >> items[i].cost;
		in >> items[i].worth;

		// equip bits
		in >> items[i].type;
		in >> items[i].equipBits;

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

int Items::getCost(int id) {
	if (!inBounds(id)) {
		return 0;
	}
	return items[id].cost;
}

int Items::getWorth(int id) {
	if (!inBounds(id)) {
		return 0;
	}
	return items[id].worth;
}

int Items::getType(int id) {
	if (!inBounds(id)) {
		return -1;
	}
	return items[id].type;
}

int Items::getValue(int id, int v) {
	if (!inBounds(id) || v < 0 || v > 3) {
		return 0;
	}
	return items[id].values[v];
}

bool Items::equippable(int id, int slot, int job) {
	if (!inBounds(id)) {
		return false;
	}

	if (!items[id].equipBits) {
		// item not equippable
		return false;
	}

	if (slot == -1 && items[id].type >= WEAPON) {
		// no slot specified and it is equippable in some slot
		return true;
	}

	if (slot != (items[id].type - WEAPON)) {
		// not equippable in this slot
		return false;
	}

	if (job == -1 && items[id].equipBits) {
		//  no job specified and it is equippable by some job
		return true;
	}

	if (items[id].equipBits & (1 << job)) {
		// if bit job is set this job can equip
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
