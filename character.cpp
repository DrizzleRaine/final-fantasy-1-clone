#include "character.h"

Character::Character() {
	setName("??????");

	std::string textureNames[] = {"img/battle_sprites.tga"};
	textures.createTextures(TEXTURECOUNT, textureNames);

	// initialize the possible random names
	std::string names[JOBSCOUNT][10] = {
		// warrior names
		{"Argus", "Arus", "Banon", "Baron", "Desh",
		"Doma", "Giott", "Leo", "Sasuun", "Zeza"},

		// thief names
		{"Baku", "Biggs", "Blank", "Daryl", "Gerad",
		"Kelga", "Puck", "Sarisa", "Tuss", "Wedge"},

		// white mage names
		{"Elia", "Kuku", "Jenica", "Luca", "Mikoto", 
		"Noah", "Ruby", "Sara", "Sarina", "Sherko"},

		// black mage names
		{"Delila", "Gilles", "Gungho", "Homac", "Kokkol",
		"Koko", "Nina", "Stella", "Topapa", "Zok"},
		
		// monk names
		{"Arvis", "Cinna", "Drogan", "Duncan", "Gorn",
		"Maduin", "Marcus", "Owain", "Owzer", "Walz"},

		// red mage names
		{"Duane", "Elayne", "Gill", "KluYa", "Kory",
		"Lalibo", "Lola", "Mid", "Tot", "Yura"}};
	for (int i = 0; i < JOBSCOUNT; i++) {
		for (int j = 0; j < 10; j++) {
			randomNames[i][j] = names[i][j];
		}
	}

	// initialize character to not have any spells
	for (int lvl = 0; lvl < 8; lvl++) {
		for (int slot = 0; slot < 3; slot++) {
			spells[lvl][slot] = -1;
		}
	}

	// no status effects
	statusBits = 0;

	// taking no action
	turn.action = NONE;

	// time to take a step 
	stepDelay = 200.0;

	// character not stepped/stepping forward
	step = 0;
	steppedForward = 0;
}

Character::~Character() {
}

Character::Character(const Character &cSource) {
	makeCopy(cSource);
}

Character &Character::operator=(const Character &rhs) {
	// check for self-assignemnt
	if (this == &rhs) {
		return *this;
	}

	// perform assignment operation
	makeCopy(rhs);

	// return the existing object
	return *this;
}

void Character::makeCopy(const Character &source) {
	name = source.name;
	job = source.job;

	for (int i = 0; i < STATSCOUNT; i++) {
		attributes[i] = source.attributes[i];
	}

	// no need to copy textures yet, 
	// for now every character uses same textures
	// textures = source.textures;
}

void Character::initStats() {
	attributes[LEVEL] = 1;
	attributes[EXP] = 0;
	switch (job) {
		case WARRIOR: {
			attributes[MAGLEVEL] = 0;

			// initial warrior stats (hpmax, mpmax, str, agl, ..., def, eva)
			int attrValues[] = {35, 0, 10, 8, 1, 15, 8, 5, 18, 0, 61};

			// initial warrior equip (knife and clothes)
			int equipValues[5][5] = {{10, 5, 10, 0, 0}, {-1, 0, 0, 0, 0}, 
				{-1, 0, 0, 0, 0}, {15, 0, 0, 1, -2}, {-1, 0, 0, 0, 0}};

			// initialize character stats and equipment
			fillValues(attrValues, equipValues);
			break;
		} case THIEF: {
			attributes[MAGLEVEL] = 0;

			// initial thief stats (hpmax, mpmax, str, agl, ..., def, eva)
			int attrValues[] = {30, 0, 5, 15, 1, 5, 15, 2, 30, 0, 73};

			// initial thief equip (knife and clothes)
			int equipValues[5][5] = {{10, 5, 10, 0, 0}, {-1, 0, 0, 0, 0}, 
				{-1, 0, 0, 0, 0}, {15, 0, 0, 1, -2}, {-1, 0, 0, 0, 0}};

			// initialize character stats and equipment
			fillValues(attrValues, equipValues);
			break;
		} case WHITE: {
			attributes[MAGLEVEL] = 1;

			// initial white mage stats (hpmax, mpmax, str, agl, ..., def, eva)
			int attrValues[] = {33, 10, 5, 5, 15, 8, 5, 2, 10, 0, 58};

			// initial white mage equip (staff and clothes)
			int equipValues[5][5] = {{14, 6, 0, 0, 0}, {-1, 0, 0, 0, 0}, 
				{-1, 0, 0, 0, 0}, {15, 0, 0, 1, -2}, {-1, 0, 0, 0, 0}};

			// initialize character stats and equipment
			fillValues(attrValues, equipValues);
			break;
		} case BLACK: {
			attributes[MAGLEVEL] = 1;

			// initial black mage stats (hpmax, mpmax, str, agl, ..., def, eva)
			int attrValues[] = {25, 10, 3, 5, 20, 2, 10, 1, 13, 0, 58};

			// initial black mage equip (knife and clothes)
			int equipValues[5][5] = {{10, 5, 10, 0, 0}, {-1, 0, 0, 0, 0}, 
				{-1, 0, 0, 0, 0}, {15, 0, 0, 1, -2}, {-1, 0, 0, 0, 0}};

			// initialize character stats and equipment
			fillValues(attrValues, equipValues);
			break;
		} case MONK: {
			attributes[MAGLEVEL] = 0;

			// initial monk stats (hpmax, mpmax, str, agl, ..., def, eva)
			int attrValues[] = {33, 0, 12, 5, 1, 10, 5, 14, 13, 5, 55};

			// initial monk equip (staff and clothes)
			int equipValues[5][5] = {{14, 6, 0, 0, 0}, {-1, 0, 0, 0, 0}, 
				{-1, 0, 0, 0, 0}, {15, 0, 0, 1, -2}, {-1, 0, 0, 0, 0}};

			// initialize character stats and equipment
			fillValues(attrValues, equipValues);
			break;
		} case RED: {
			attributes[MAGLEVEL] = 1;

			// initial red mage stats (hpmax, mpmax, str, agl, ..., def, eva)
			int attrValues[] = {30, 10, 5, 10, 10, 5, 5, 2, 22, 0, 63};

			// initial red mage equip (knife and clothes)
			int equipValues[5][5] = {{10, 5, 10, 0, 0}, {-1, 0, 0, 0, 0}, 
				{-1, 0, 0, 0, 0}, {15, 0, 0, 1, -2}, {-1, 0, 0, 0, 0}};

			// initialize character stats and equipment
			fillValues(attrValues, equipValues);
			break;
		} default:
			break;
	}
	attributes[HP] = attributes[HPMAX];
	attributes[MP] = attributes[MPMAX];
}

void Character::fillValues(int initAttrs[STATSCOUNT], int initEquip[5][5]) {
	for (int i = HPMAX; i < STATSCOUNT; i++) {
		attributes[i] = initAttrs[i - HPMAX];
	}
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			equipment[i][j] = initEquip[i][j];
		}
	}
}

void Character::render(int x, int y) {
	render(job, x, y);
}

void Character::render(Jobs job, int x, int y) {
	// calculate step offset
	float xStepDistance = 0.0;
	float totalDistance = 150.0;
	if (step) {
		const int stepTime = SDL_GetTicks() - step;

		if (step && stepTime < stepDelay) {
			if (steppedForward) {
				xStepDistance = -totalDistance + (stepTime / stepDelay) * totalDistance;
			} else {
				xStepDistance = -(stepTime / stepDelay) * totalDistance;
			}
		} else if (step) {
			step = 0;
			if (steppedForward) {
				steppedForward = 0;
			} else {
				steppedForward = 1;
				xStepDistance = -totalDistance;
			}
		}
	} else if (steppedForward) {
		xStepDistance = -totalDistance;
	}

	// sprite's (x, y) coordinates
	int spriteX = 1, spriteY;

	// if taking step, animate it
	if (step && (SDL_GetTicks() - step) < (stepDelay * (1 / 3.0))) {
		spriteX = 2;
	} else if (step && (SDL_GetTicks() - step) < (stepDelay * (2 / 3.0))) {
		spriteX = 3;
	}

	// TODO if character has dead or critical status, change sprite x
	if (job < WHITE) {
		spriteY = job;		// warrior or thief
	} else if (job < MONK) {
		spriteY = job + 2;	// white or black
	} else {
		spriteY = job - 2;	// monk or red
	}
	spriteY++;

	// width and height of sprite sheet
	const float sheetWidth = 624.0;
	const float sheetHeight = 624.0;

	// width of character on sprite sheet
	const float spriteWidth = 52;

	// find coordinates of desired sprite
	GLfloat texCoords[4];
	texCoords[2] = ((spriteX * spriteWidth) / sheetWidth);
	texCoords[3] = ((spriteY * spriteWidth) / sheetHeight);
	texCoords[0] = (((spriteX - 1) * spriteWidth) / sheetWidth);
	texCoords[1] = (((spriteY - 1) * spriteWidth) / sheetHeight);
	if (step && steppedForward) {	// reverse the sprite
		GLfloat temp = texCoords[0];
		texCoords[0] = texCoords[2];
		texCoords[2] = temp;
	}

	// scaled dimensions char will be displayed as
	float scaledWidth = 156;
	float scaledHeight = 156;

	glBindTexture(GL_TEXTURE_2D, textures.getTexture(CHARBATTLESPRITES));
	glBegin(GL_QUADS);
		glTexCoord2f(texCoords[0], texCoords[1]);
		glVertex2f(x + xStepDistance, y);
		glTexCoord2f(texCoords[0], texCoords[3]);
		glVertex2f(x + xStepDistance, y - scaledHeight);
		glTexCoord2f(texCoords[2], texCoords[3]);
		glVertex2f(x + xStepDistance + scaledWidth, y -scaledHeight);
		glTexCoord2f(texCoords[2], texCoords[1]);
		glVertex2f(x + xStepDistance + scaledWidth, y);
	glEnd();
}

void Character::setName(std::string newName) {
	name = newName;
}

std::string Character::getName() {
	return name;
}

void Character::setRandomName() {
	name = randomNames[job][rand() % 10];
}

void Character::setJob(Jobs newJob) {
	if (newJob >= WARRIOR && newJob < JOBSCOUNT) {
		job = newJob;
	}
}

Character::Jobs Character::getJob() {
	return job;
}

std::string Character::getJobName() {
	return getJobName(job);
}

std::string Character::getJobName(Jobs job) {
	switch (job) {
		case WARRIOR:
			return "Warrior";
		case THIEF:
			return "Thief";
		case WHITE:
			return "White Mage";
		case BLACK:
			return "Black Mage";
		case MONK:
			return "Monk";
		case RED:
			return "Red Mage";
		case KNIGHT:
			return "Knight";
		case NINJA:
			return "Ninja";
		case WHITEW:
			return "White Wizard";
		case BLACKW:
			return "Black Wizard";
		case MASTER:
			return "Master";
		case REDW:
			return "Red Wizard";
		default:
			return "";
	}
}

int Character::getAttribute(Stats s) {
	if (s < ATK) {
		// attribute unmodified by equipment
		return attributes[s];
	}

	// return value modified by current equipment
	// TODO: Some of these formulas are outdated and
	//       from the original FF1, (specifically monk)
	if (s == ATK) {
		if (job == MONK && equipment[0][0] == -1) {
			// monk not wearing weapon
			return attributes[LEVEL] * 2;
		} else if (job == MONK || job == BLACK) {
			// monk or blm with weapon: weaonatk + str/2 + 1
			return equipment[0][1] + attributes[STR] / 2 + 1;
		}
		// weaponatk + str/2
		return equipment[0][1] + attributes[STR] / 2;
	} else if (s == ACC) {
		// weaponacc + characteracc
		return equipment[0][2] + attributes[ACC];
	} else if (s == DEF) {
		if (job == MONK) {
			// monk defense = level
			return attributes[LEVEL];
		}

		// total defense of equipment
		int totalEquippedDef = 0;
		for (int i = 0; i < 4; i++) {
			totalEquippedDef += equipment[i][3];
		}
		return totalEquippedDef + attributes[DEF];
	} else if (s == EVA) {
		// 48 + agl - armor weight
		// armor weights are represented with negative eva
		int totalWeight = 0;
		for (int i = 0; i < 4; i++) {
			totalWeight += equipment[i][4];
		}
		return attributes[EVA] + totalWeight;
	}
	return 0;
}

void Character::addExp(int exp) {
	attributes[EXP] += exp;

	while (expToNext() <= 0) {
		levelUp();
	}
}

void Character::levelUp() {
	attributes[LEVEL]++;
}

std::string Character::getHPFraction() {
	return getFraction(HP, HPMAX);
}

std::string Character::getMPFraction() {
	return getFraction(MP, MPMAX);
}

std::string Character::getFraction(Stats num, Stats denom) {
	std::string cur = std::to_string(attributes[num]);
	std::string max = std::to_string(attributes[denom]);

	// add whitespace
	const int DIGITS = 4;	// max digits
	int spacesNeeded = DIGITS - cur.length();
	cur.insert(0, spacesNeeded, ' ');

	spacesNeeded = DIGITS - max.length();
	max.insert(0, spacesNeeded, ' ');

	// return fraction format
	return (cur + '/' + max);
}

int Character::addHP(int amount) {
	int excess = 0;
	attributes[HP] += amount;
	if (attributes[HP] > attributes[HPMAX]) {
		// hp exceeds maxhp
		excess = attributes[HP] - attributes[HPMAX];
		attributes[HP] = attributes[HPMAX];
	} else if (attributes[HP] < 0) {
		// hp fell below 0
		excess = attributes[HP];
		attributes[HP] = 0;
	}

	// calculate how much hp was added
	return (amount - excess);
}

int Character::addMP(int amount) {
	int excess = 0;
	attributes[MP] += amount;
	if (attributes[MP] > attributes[MPMAX]) {
		// mp exceeds maxmp
		excess = attributes[MP] - attributes[MPMAX];
		attributes[MP] = attributes[MPMAX];
	} else if (attributes[MP] < 0) {
		// mp fell below 0
		excess = attributes[MP];
		attributes[MP] = 0;
	}

	// calculate how much mp was added
	return (amount - excess);
}

bool Character::hasStatus(unsigned int status) {
	return (statusBits & (1 << status));
}

bool Character::setStatus(unsigned int status) {
	if (statusBits & (1 << status)) {
		return 0;	// character already has status
	}

	// set status bit
	statusBits |= 1 << status;
	return 1;	// status successfully set
}

bool Character::removeStatus(unsigned int status) {
	if (statusBits & (1 << status)) {
		// clear status bit
		statusBits &= ~(1 << status);
		return 1;	// 	status successfully removed
	}
	return 0;	// character does not have status
}

int Character::expToNext() {
	// experience needed for first 20 levels
	int expNeeded[] = {0, 14, 42, 98, 196, 350, 574, 882, 1288, 1806, 2450, 3234, 4172, 5278, 6566, 8050, 9744, 11662, 13818, 16226};

	if (attributes[LEVEL] >= 20) {
		return 0;	// only supporting first 20 levels for now
	}

	return (expNeeded[attributes[LEVEL]] - attributes[EXP]);
}

int Character::getSpell(int level, int slot) {
	level--;	// convert to 0-based index
	if (level < 0 || level > 7 || slot < 0 || slot > 2) {
		return 0; // out of bounds
	}
	return spells[level][slot];
}

bool Character::addSpell(int id, int level) {
	level--;	// convert to 0 based index
	if (level >= 0 && level < 8) {
		for (int slot = 0; slot < 3; slot++) {
			if (spells[level][slot] == -1) {
				spells[level][slot] = id;
				return 1;
			}
		}
	}
	// spell not learned
	return 0;
}

bool Character::removeSpell(int level, int slot) {
	level--;	// conver to 0 based index
	if (level >= 0 && level < 8 && slot >= 0 && slot < 3) {
		// if in bounds
		if (spells[level][slot] != -1) {
			// if spell exists at given level and slot
			spells[level][slot] = -1;	// remove the spell

			// shift all slots after removed spell left by 1
			while (slot < 2) {
				spells[level][slot] = spells[level][slot + 1];
				spells[level][slot + 1] = -1;
				slot++;
			}
	
			// spell removed successfully
			return 1;
		}
	}
	// spell not removed
	return 0;
}

bool Character::hasSpell(int id) {
	for (int lvl = 0; lvl < 8; lvl++) {
		for (int slot = 0; slot < 3; slot++) {
			if (spells[lvl][slot] == id) {
				return 1;
			}
		}
	}
	return 0;
}

int Character::getEquip(int slot) {
	if (slot < 0 || slot > 4) {
		return 0; // out of bounds
	}
	return equipment[slot][0];
}

bool Character::canEquip(int type) {
	if (type == -1) {
		return 0;
	}
	return 1;
}

void Character::setEquip(int slot, int id, int values[4]) {
	if (slot < 0 || slot > 4) {
		return; // out of bounds
	}
	equipment[slot][0] = id;
	for (int i = 0; i < 4; i++) {
		equipment[slot][i + 1] = values[i];
	}
}

void Character::removeEquip(int slot) {
	int values[4] = {0, 0, 0, 0};
	setEquip(slot, -1, values);
}

void Character::stepForward() {
	if (!step) {
		step = SDL_GetTicks();
	}
}

void Character::stepBackward() {
	if (!step) {
		step = SDL_GetTicks();
	}
}

bool Character::forward() {
	return steppedForward;
}
