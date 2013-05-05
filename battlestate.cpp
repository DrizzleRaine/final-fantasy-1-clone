#include "battlestate.h"
#include "battlemainmenu.h"

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

	std::ifstream in, enemyIn;
	in.open(filename);

	// read in background type
	std::string background;
	in >> background;

	// read in enemy data
	for (int i = 0; i < ENEMYSLOTS; i++) {
		// read in enemy name
		in >> enemy[i].name;

		// process the texture name
		if (enemy[i].name == "0") {	// no enemy in this slot
			enemy[i].textureID = -1;// no texture for this enemy
			enemy[i].HP = 0;		// no hp, exp or gil for this enemy
			enemy[i].EXP = 0;
			enemy[i].GIL = 0;
		} else {					// else check for duplicate textures
			enemyIn.open("battle_data/data/" + enemy[i].name + ".data");
			enemyIn >> enemy[i].HP;
			enemyIn >> enemy[i].ATK;
			enemyIn >> enemy[i].ACC;
			enemyIn >> enemy[i].CRT;
			enemyIn >> enemy[i].DEF;
			enemyIn >> enemy[i].EVA;
			enemyIn >> enemy[i].MDEF;
			enemyIn >> enemy[i].MOR;
			enemyIn >> enemy[i].EXP;
			enemyIn >> enemy[i].GIL;
			enemyIn.close();

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

	menuState = 0;
	battleBGMenu = new BattleBGMenu();
}

BattleState::~BattleState() {
	delete battleBGMenu;
	battleBGMenu = 0;

	if (menuState) {
		delete menuState;
		menuState = 0;
	}
}

void BattleState::update() {
	if (currentSlot == 0) {
		if (battleOver()) {
			return;
		}

		// battle not over, enemy turn, decide actions
		while (currentSlot < ENEMYSLOTS) {
			enemyLocs[currentSlot] = 1;
			if (enemy[currentSlot].name == "0" || enemy[currentSlot].HP <= 0) {
				// empty slot or dead enemy, move to next
				enemyLocs[currentSlot] = 0;
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
			slot[currentSlot].target = target + ENEMYSLOTS;

			// enemy's turn over
			currentSlot++;
		}
	}

	if (currentSlot >= ENEMYSLOTS && currentSlot < SLOTCOUNT) {
		// find first non dead character
		int character = currentSlot - ENEMYSLOTS;

		while (currentSlot < SLOTCOUNT && party->hasStatus(static_cast<Party::Characters>(character), 1)) {
			currentSlot++;
			character = currentSlot - ENEMYSLOTS;
		}

		Party::Characters c = static_cast<Party::Characters>(character);

		// player turn
		if (!party->charForward(c)) {
			party->stepCharForward(c);
		} else if (!menuState) {
			// open menu for player
			menuState = new MenuState();
			menuState->init(party, stateManager);
			menuState->pushMenu(new BattleMainMenu(c, enemyLocs));
		} else {
			menuState->setInput(input);
			menuState->updateState();

			if (!menuState->getSize()) {
				// player done deciding or wanting to go back
				// TODO check if character turn decidided
				delete menuState;
				menuState = 0;

				Character::Turn t = party->getTurn(c);
				if (input.getCancel()) {
					if (currentSlot != ENEMYSLOTS) {
						currentSlot--;
					}
					party->stepCharBackward(c);
					input.resetCancel();
				} else if (t.action == Character::Actions::NONE) {
					// player wants to go back a character
					if (currentSlot > ENEMYSLOTS) {
						currentSlot--;
					}	// else already at first character
				} else {
					// TODO just setting actino to ATTACK
					// need a way to convert better
					// maybe just use characters public implementation
					slot[currentSlot].action = ATTACK;
					slot[currentSlot].actionID = t.actionID;
					slot[currentSlot].target = t.target;
					currentSlot++;

					// player turn finished, step back
					party->stepCharBackward(c);
				}
			}
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
			Turn curSlot = slot[attacker];
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
				while (enemy[curSlot.target].name == "0" || enemy[curSlot.target].HP <= 0) {
					curSlot.target = rand() % ENEMYSLOTS;
				}

				// just have character hit for rand num 1-10
				int damage = rand() % 10 + 1;
				printf("character %i hits %i for %i HP\n", attacker - ENEMYSLOTS, curSlot.target, damage);
				enemy[curSlot.target].HP -= damage;
				if (enemy[curSlot.target].HP <= 0) {
					printf("ENEMY %i WAS KILLED\n", curSlot.target);
				}

				curSlot.action = NONE;
			} else {
				int attackerHP = enemy[attacker].HP;
				if (attackerHP <= 0) {
					// dead enemies cant attack
					continue;
				}

				// if character enemy wanted to attack already dead
				// choose another character to attack at random
				while (party->getAttribute(static_cast<Party::Characters>(curSlot.target - ENEMYSLOTS), Character::HP) <= 0) {
					curSlot.target = rand() % 4 + ENEMYSLOTS;
				}

				// just have enemy hit for 1
				printf("enemy %i hits %i for 1 HP\n", attacker, curSlot.target - ENEMYSLOTS);
				party->addHP(static_cast<Party::Characters>(curSlot.target - ENEMYSLOTS), -1);
				if (party->getAttribute(static_cast<Party::Characters>(curSlot.target - ENEMYSLOTS), Character::HP) <= 0) {
					printf("CHARACTER %i WAS KILLED\n", curSlot.target - ENEMYSLOTS);
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

	input.resetAll();
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

	// while drawing, get count of each type of enemy
	std::map<std::string, int> enemyTypes;

	// draw each enemy
	for (int i = 0; i < ENEMYSLOTS; i++) {
		// if enemy exists and is alive
		if (enemy[i].textureID >= 0 && enemy[i].HP > 0) {
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

			if (enemyTypes.find(enemy[i].name) == enemyTypes.end()) {
				// this enemy is not in map yet, add it
				enemyTypes[enemy[i].name] = 1;
			} else {
				// enemy already in map, increment count
				enemyTypes[enemy[i].name]++;
			}
		}
	}

	// draw characters
	for (int i = Party::FIRST; i < Party::SIZE; i++) {
		Party::Characters c = static_cast<Party::Characters>(i);

		int xPos = windowWidth - 380 + 50 * i;
		int yPos = windowHeight - 170 - 120 * i;
		party->render(c, xPos, yPos);
	}

	// get enemy names and counts strings to render
	std::string enemyNames, enemyCounts;
	std::map<std::string, int>::iterator it;
	for (it = enemyTypes.begin(); it != enemyTypes.end(); ++it) {
		enemyNames.append(it->first + '\n');
		enemyCounts.append(std::to_string(it->second) + '\n');
	}

	// render menu for enemy names/count and characters name/hp/mp
	battleBGMenu->render(windowWidth, windowHeight, party, enemyNames, enemyCounts);

	if (menuState) {
		menuState->renderState(windowWidth, windowHeight);
	}
}

bool BattleState::battleOver() {
	int totalExp = 0, totalGil = 0;
	// check if all enemies dead
	bool allDead = 1;
	for (int i = 0; i < ENEMYSLOTS; i++) {
		if (enemy[i].name != "0" && enemy[i].HP > 0) {
			allDead = 0;
			break;
		} else {
			totalExp += enemy[i].EXP;
			totalGil += enemy[i].GIL;
		}
	}
	if (allDead) {
		printf("PARTY WINS\n");
		int aliveCharacters = 0;
		for (int i = Party::FIRST; i < Party::SIZE; i++) {
			Party::Characters c = static_cast<Party::Characters>(i);
			if (!party->hasStatus(c, 1)) {
				aliveCharacters++;
			}
		}
		totalExp /= aliveCharacters;
		printf("exp: %d, gil: %d\n", totalExp, totalGil);
		for (int i = Party::FIRST; i < Party::SIZE; i++) {
			Party::Characters c = static_cast<Party::Characters>(i);
			if (!party->hasStatus(c, 1)) {
				party->addExp(c, totalExp);
				aliveCharacters++;
			}
		}
		party->addGil(totalGil);
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
