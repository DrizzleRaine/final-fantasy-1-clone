#include "enemy.h"

Enemy::Enemy() {
	spriteDim = 192;
	blinkTicks = 0;
}

Enemy::~Enemy() {
}

void Enemy::init(std::string name) {
	setName(name);
	if (name == "0") {
		return;	// no enemy
	}

	std::ifstream in;
	in.open("battle_data/data/" + name + ".data");

	// read in enemy attribute values
	int val;
	in >> val; attributes[HPMAX] = val;
			   attributes[HP] = val;
	in >> val; attributes[ATK]= val;
	in >> val; attributes[ACC] = val;
	in >> val; attributes[CRT] = val;
	in >> val; attributes[DEF] = val;
	in >> val; attributes[EVA] = val;
	in >> val; attributes[MDEF] = val;
	in >> val; attributes[MOR] = val;
	in >> val; attributes[EXP] = val;
	in >> val; attributes[GIL] = val;

	in.close();

	std::string textureNames[] = {"battle_data/sprites/" + name + ".tga"};
	textures.createTextures(1, textureNames);
}

void Enemy::render(int x, int y) {
	if (getName() == "0") {
		return;	// no enemy to draw
	}

	if (blinking()) {
		// blink enemy darker before attacking
		int ticksPassed = SDL_GetTicks() - blinkTicks;
		if ((ticksPassed / 100) % 2) {
			glColor3f(0.5f, 0.5f, 0.5f);
		}
	}

	// render enemy at correct position
	glBindTexture(GL_TEXTURE_2D, textures.getTexture(0));
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(x, y, 0.0f);
		glTexCoord2f(0, 1);
		glVertex3f(x, y - spriteDim, 0.0f);
		glTexCoord2f(1, 1);
		glVertex3f(x + spriteDim, y - spriteDim, 0.0f);
		glTexCoord2f(1, 0);
		glVertex3f(x + spriteDim, y, 0.0f);
	glEnd();

	// restore colors
	glColor3f(1.0f, 1.0f, 1.0f);
}

void Enemy::blink() {
	blinkTicks = SDL_GetTicks();
}

bool Enemy::blinking() {
	return (SDL_GetTicks() - blinkTicks) < 400;
}

int Enemy::getAttribute(Stats s) {
	if (s >= LEVEL && s < STATSCOUNT) {
		return attributes[s];
	}
	return 0;
}
