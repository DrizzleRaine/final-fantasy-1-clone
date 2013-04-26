#include "battlestate.h"

BattleState::BattleState(int battleID, int advantage) {
	this->battleID = battleID;

	// start at slot 0
	currentSlot = 0;

	// determine if preemptive/surprise attack
	preemptive = 0; surprise = 0;
	if (advantage <= 10) {
		surprise = 1;
	} else if (advantage >= 90) {
		preemptive = 1;
		currentSlot = 8;	// start with first character's slot
	}

	// number of unique enemy textures found
	int textureCount = 0;

	// there will be <= ENEMYSLOTS textureNames + 1 background texture
	std::string textureNames[ENEMYSLOTS + 1];

	// TODO battleGroup should be a random possible group
	std::string battleGroup = "A";
	std::string filename = "battle_data/" + std::to_string(-1 * battleID) + "A";

	std::ifstream in;
	in.open(filename);

	// read in background type
	std::string background;
	in >> background;

	// read in enemy data
	for (int i = 0; i < ENEMYSLOTS; i++) {
		// read in enemy name
		in >> enemy[i].name;

		// temporary, just give them 10 HP
		enemy[i].HP = 10;

		// process the texture name
		if (enemy[i].name == "0") {	// no enemy in this slot
			enemy[i].textureID = -1;// no texture for this enemy
		} else {					// else check for duplicate textures

			// full texture path and filename
			std::string texFile = "battle_data/sprites/" + enemy[i].name + ".tga";

			bool textureExists = 0;
			for (int j = 0; j < textureCount; j++) {
				if (texFile == textureNames[j]) {
					// this enemy using same texture as another enemy
					textureExists = 1;
					enemy[i].textureID = j;
					break;
				}
			}

			if (!textureExists) {
				// no enemy so far with this texture, add it
				enemy[i].textureID = textureCount;
				textureNames[textureCount++] = texFile;
			}

		}
	}
	in.close();

	// add background texture to texture array
	backgroundTexID = textureCount;
	textureNames[textureCount++] = "battle_data/backgrounds/" + background + ".tga";

	// create textureCount enemy textures with no duplicates
	// and the battle background texture
	textures.createTextures(textureCount, textureNames);

	for (int i = 0; i < SLOTCOUNT; i++) {
		// initialize all slots to no action decided
		slot[i].action = NONE;
	}
}

BattleState::~BattleState() {
}

void BattleState::update() {
	if (currentSlot == 0) {
		if (battleOver()) {
			return;
		}

		// battle not over, enemy turn, decide actions
		while (currentSlot < ENEMYSLOTS) {
			if (enemy[currentSlot].name == "0" || enemy[currentSlot].HP <= 0) {
				// empty slot or dead enemy, move to next
				currentSlot++;
				continue;
			}

			// for now just attack random character
			slot[currentSlot].action = ATTACK;

			// pick random player
			int target = rand() % 4;

			// while targeted player is dead, choose another
			while (party->hasStatus(static_cast<Party::Characters>(target), 1)) {
				target = rand() % 4;
			}

			printf("enemy %i decides to attack %i\n", currentSlot, target);

			// target decided
			slot[currentSlot].targetSlot = target + ENEMYSLOTS;

			// enemy's turn over
			currentSlot++;
		}
	}

	if (input.getConfirm()) {
		input.resetConfirm();

		if (currentSlot >= ENEMYSLOTS && currentSlot < SLOTCOUNT) {
			// find first non dead character
			int character = currentSlot - ENEMYSLOTS;
			while (currentSlot < SLOTCOUNT && party->hasStatus(static_cast<Party::Characters>(character), 1)) {
				currentSlot++;
				character = currentSlot - ENEMYSLOTS;
			}

			// have them attack
			slot[currentSlot].action = ATTACK;

			// pick random enemy that exists and is alive
			int target = rand() % ENEMYSLOTS;
			while (enemy[target].name == "0" || enemy[target].HP <= 0) {
				target = rand() % ENEMYSLOTS;
			}

			printf("character %i decides to attack %i\n", currentSlot - ENEMYSLOTS, target);

			// target decided
			slot[currentSlot].targetSlot = target;

			// players turn over
			currentSlot++;
		}
	}

	if (currentSlot == SLOTCOUNT) {
		printf("\n--EXECUTING--\n");
		// all enemies and characters have chosen actions
		int attackSequence[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

		// randomize the attack sequence
		int max = SLOTCOUNT - 1;
		while (max) {
			int r = rand() % SLOTCOUNT;
			int temp = attackSequence[r];
			attackSequence[r] = attackSequence[max];
			attackSequence[max] = temp;
			max--;
		}

		// now execute actions in randomized order
		while (currentSlot) {
			currentSlot--;
			int attacker = attackSequence[currentSlot];
			Slot curSlot = slot[attacker];
			if (curSlot.action == NONE) {
				continue;
			}

			if (attacker >= ENEMYSLOTS) {
				int attackerHP = party->getAttribute(static_cast<Party::Characters>(attacker - ENEMYSLOTS), Character::HP);
				if (attackerHP <= 0) {
					// dead characters cant attack
					continue;
				}


				// if enemy player wanted to attack already dead
				// choose another enemy to attack at random
				while (enemy[curSlot.targetSlot].name == "0" || enemy[curSlot.targetSlot].HP <= 0) {
					curSlot.targetSlot = rand() % ENEMYSLOTS;
				}

				// just have character hit for rand num 1-10
				int damage = rand() % 10 + 1;
				printf("character %i hits %i for %i HP\n", attacker - ENEMYSLOTS, curSlot.targetSlot, damage);
				enemy[curSlot.targetSlot].HP -= damage;
				if (enemy[curSlot.targetSlot].HP <= 0) {
					printf("ENEMY %i WAS KILLED\n", curSlot.targetSlot);
				}
			} else {
				int attackerHP = enemy[attacker].HP;
				if (attackerHP <= 0) {
					// dead enemies cant attack
					continue;
				}

				// if character enemy wanted to attack already dead
				// choose another character to attack at random
				while (party->getAttribute(static_cast<Party::Characters>(curSlot.targetSlot - ENEMYSLOTS), Character::HP) <= 0) {
					curSlot.targetSlot = rand() % 4 + ENEMYSLOTS;
				}

				// just have enemy hit for 1
				printf("enemy %i hits %i for 1 HP\n", attacker, curSlot.targetSlot - ENEMYSLOTS);
				party->addHP(static_cast<Party::Characters>(curSlot.targetSlot - ENEMYSLOTS), -1);
				if (party->getAttribute(static_cast<Party::Characters>(curSlot.targetSlot - ENEMYSLOTS), Character::HP) <= 0) {
					printf("CHARACTER %i WAS KILLED\n", curSlot.targetSlot - ENEMYSLOTS);
				}
			}
			curSlot.action = NONE;

			if (battleOver()) {
				return;
			}
		}
		printf("\n--STATUS--\n");
		for (int i = 0; i < ENEMYSLOTS; i++) {
			if (enemy[i].name != "0") {
				if (enemy[i].HP > 0) {
					printf("enemy %i HP: %i\n", i, enemy[i].HP);
				} else {
					printf("enemy %i DEAD, HP: %i\n", i, enemy[i].HP);
				}
			}
		}
		for (int i = ENEMYSLOTS; i < SLOTCOUNT; i++) {
			int HP = party->getAttribute(static_cast<Party::Characters>(i - ENEMYSLOTS), Character::HP);
			if (HP > 0) {
				printf("character %i HP: %i\n", i - ENEMYSLOTS, HP);
			} else {
				printf("character %i DEAD, HP: %i\n", i - ENEMYSLOTS, HP);
			}
		}
		printf("\n\n--NEXTROUND--\n");
	}

	if (input.getCancel()) {
		input.resetCancel();

		// temporary, let cancel button exit battles
		stateManager->popState();
		return;
	}
}

void BattleState::render() {
	// render battle background
	glBindTexture(GL_TEXTURE_2D, textures.getTexture(backgroundTexID));
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(-windowWidth, windowHeight, 0.0f);
		glTexCoord2f(0, 1);
		glVertex3f(-windowWidth, -windowHeight * 0.8, 0.0f);
		glTexCoord2f(1, 1);
		glVertex3f(windowWidth, -windowHeight * 0.8, 0.0f);
		glTexCoord2f(1, 0);
		glVertex3f(windowWidth, windowHeight, 0.0f);
	glEnd();

	// enemy sprite dimensions
	const int SPRITEDIM = 192;

	// draw each enemy
	for (int i = 0; i < ENEMYSLOTS; i++) {
		if (enemy[i].textureID >= 0) {
			int xPos = -windowWidth + SPRITEDIM * (i / 3) + 60;
			int yPos = windowHeight - SPRITEDIM * (i % 3) - 120;

			// render enemy at correct position
			glBindTexture(GL_TEXTURE_2D, textures.getTexture(enemy[i].textureID));
			glBegin(GL_QUADS);
				glTexCoord2f(0, 0);
				glVertex3f(xPos, yPos, 0.0f);
				glTexCoord2f(0, 1);
				glVertex3f(xPos, yPos - SPRITEDIM, 0.0f);
				glTexCoord2f(1, 1);
				glVertex3f(xPos + SPRITEDIM, yPos - SPRITEDIM, 0.0f);
				glTexCoord2f(1, 0);
				glVertex3f(xPos + SPRITEDIM, yPos, 0.0f);
			glEnd();
		}
	}

	// draw characters
	for (int i = Party::FIRST; i < Party::SIZE; i++) {
		Party::Characters c = static_cast<Party::Characters>(i);

		int xPos = windowWidth - 360 + 50 * i;
		int yPos = windowHeight - 180 - 120 * i;
		party->render(c, xPos, yPos);
	}
}

bool BattleState::battleOver() {
	// check if all enemies dead
	bool allDead = 1;
	for (int i = 0; i < ENEMYSLOTS; i++) {
		if (enemy[i].name != "0" && enemy[i].HP > 0) {
			allDead = 0;
			break;
		}
	}
	if (allDead) {
		printf("PARTY WINS\n");
		stateManager->popState();
		return 1;
	}

	// check if all players dead
	for (int i = Party::FIRST; i < Party::SIZE; i++) {
		Party::Characters c = static_cast<Party::Characters>(i);
		if (!party->hasStatus(c, 1)) {
			allDead = 0;
			break;
		}
	}
	if (allDead) {
		printf("GAME OVER\n");
		stateManager->popState();
		return 1;
	}

	// battle not over
	return 0;
}

void BattleState::pause() {
}

void BattleState::unpause() {
}
