#include "spells.h"

Spells::Spells() {
	std::ifstream in;
	in.open("spells.data");

	// get number of spells
	in >> spellCount;

	// generate an array of spells
	spells = new Spell[spellCount];

	// fill the array from the file
	for (unsigned int i = 0; i < spellCount; i++) {
		// go to the next line
		in.ignore(256, '\n');

		// name and description
		getline(in, spells[i].name, '\n');
		getline(in, spells[i].description, '\n');

		// type and learn bits
		in >> spells[i].type;
		in >> spells[i].learnBits;

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

int Spells::getType(int id) {
	if (!inBounds(id)) {
		return -1;
	}
	return spells[id].type;
}

bool Spells::learnable(int id, int job) {
	if (!inBounds(id)) {
		return false;
	}

	if (!spells[id].learnBits) {
		// spell not learnable
		return false;
	}

	if (job == -1) {
		// no job specified, and learnable by some job
		return true;
	}

	if (spells[id].learnBits & (1 << job)) {
		// if bit job is set this job can learn
		return true;
	}

	return false;
}

bool Spells::menuUse(int id) {
	if (!inBounds(id)) {
		return 0;
	}
	return spells[id].menu;
}

int Spells::cast(int id) {
	if (!inBounds(id)) {
		return 0;
	}

	// TODO, return correct amount
	// depending on its type, use correct formula
	return 10;
}

bool Spells::inBounds(int id) {
	if (id >= 0 && id < spellCount) {
		return true;
	}
	return false;
}
