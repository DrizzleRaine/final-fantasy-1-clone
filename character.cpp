#include "character.h"

Character::Character() {
	setName("??????");

	std::string textureNames[] = {"img/warrior_menu.tga", "img/thief_menu.tga", "img/white_menu.tga", "img/black_menu.tga", "img/monk_menu.tga", "img/red_menu.tga"};
	textures.createTextures(JOBSCOUNT, textureNames);
}

Character::~Character() {
}

void Character::initStats() {
	attributes[LEVEL] = 1;
	if (job == WARRIOR) {
		attributes[HPMAX] = 35;
	} else if (job == WHITE || job == MONK) {
		attributes[HPMAX] = 33;
	} else if (job == THIEF || job == RED) {
		attributes[HPMAX] = 30;
	} else {
		attributes[HPMAX] = 25;
	}
	if (job < WHITE || job == MONK) {
		attributes[MPMAX] = 0;
	} else {
		attributes[MPMAX] = 10;
	}
	attributes[HP] = attributes[HPMAX];
	attributes[MP] = attributes[MPMAX];
}

void Character::render(int x, int y) {
	render(job, x, y);
}

void Character::render(Jobs job, int x, int y) {
	glBindTexture(GL_TEXTURE_2D, textures.getTexture(job));
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(x, y, 0.0f);
		glTexCoord2f(0, 1);
		glVertex3f(x, y - 148, 0.0f);
		glTexCoord2f(1, 1);
		glVertex3f(x + 120, y - 148, 0.0f);
		glTexCoord2f(1, 0);
		glVertex3f(x + 120, y, 0.0f);
	glEnd();
}

void Character::setName(std::string newName) {
	name = newName;
}

std::string Character::getName() {
	return name;
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
		default:
			return "";
	}
}

int Character::getAttribute(Stats s) {
	if (s < STATSCOUNT) {
		return attributes[s];
	}
	return 0;
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
