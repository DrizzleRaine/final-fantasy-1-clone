#include "spells.h"

Spells::Spells() {
	std::ifstream in;
	in.open("spells.data");

	// get number of spells
	in >> spellCount;

	// generate an array of spells
	spells = new Spell[spellCount];

	// fill the array from the file
	int type;
	for (int i = 0; i < spellCount; i++) {
		// go to the next line
		in.ignore(256, '\n');

		// name and description
		getline(in, spells[i].name, '\n');
		getline(in, spells[i].description, '\n');

		// type
		in >> type;
		spells[i].type = static_cast<Spell::Types>(type);

		// values
		in >> spells[i].menu;
		in >> spells[i].level;
		in >> spells[i].mp;
		in >> spells[i].power;
		in >> spells[i].status;
		in >> spells[i].success;
	}
	in.close();
}

Spells::~Spells() {
	delete[] spells;
	spells = 0;
}

std::string Spells::getName(int id) {
	if (!inBounds(id)) {
		return "";
	}
	return spells[id].name;
}

std::string Spells::getDescription(int id) {
	if (!inBounds(id)) {
		return "";
	}
	return spells[id].description;
}

bool Spells::menuUse(int id) {
	if (!inBounds(id)) {
		return 0;
	}
	return spells[id].menu;
}

int Spells::getLevel(int id) {
	if (!inBounds(id)) {
		return -1;
	}
	return spells[id].level;
}

int Spells::getMPCost(int id) {
	if (!inBounds(id)) {
		return -1;
	}
	return spells[id].mp;
}

bool Spells::inBounds(int id) {
	if (id >= 0 && id < spellCount) {
		return true;
	}
	return false;
}
