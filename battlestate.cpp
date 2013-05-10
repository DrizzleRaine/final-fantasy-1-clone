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

		// start with first character's slot
		currentSlot = ENEMYSLOTS;
	}

	// TODO battleGroup should be a random possible group
	std::string battleGroup = "A";
	std::string filename = "battle_data/" + std::to_string(-battleID) + "A";

	std::ifstream in;
	in.open(filename);

	// read in background type
	std::string background;
	in >> background;

	// read in enemy data
	for (int i = 0; i < ENEMYSLOTS; i++) {
		// read in enemy name's
		std::string name;
		in >> name;

		// initialize enemy
		enemy[i].init(name);

	}
	in.close();

	std::string textureNames[] = {"battle_data/backgrounds/" + background + ".tga"};
	textures.createTextures(1, textureNames);

	// reset every enities turn
	for (int i = 0; i < ENEMYSLOTS; i++) {
		enemy[i].resetTurn();
	}
	for (int i = ENEMYSLOTS; i < SLOTCOUNT; i++) {
		Party::Characters c = static_cast<Party::Characters>(i);
		party->resetTurn(c);
	}

	// initialize attack sequence array
	for (int i = 0; i < SLOTCOUNT; i++) {
		attackSequence[i] = i;
	}

	// not executing turns
	executingTurns = 0;

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
	if (executingTurns) {
		// store slot about to execute turn
		int slotToExecute = currentSlot;

		// execute next turn
		if (attackSequence[currentSlot] >= ENEMYSLOTS) {
			characterAct();	// execute character turn
		} else {
			enemyAct();		// execute enemy turn
		}

		if (currentSlot < slotToExecute) {
			// turn was completed, check if battle over
			if (battleOver()) {
				return;
			}

			// check if done executing turns
			if (currentSlot < 0){
				executingTurns = 0;
			}
		}
	} else {
		// decide enemy actions for next turn
		while (currentSlot < ENEMYSLOTS) {
			enemyDecide();
			currentSlot++;
		}

		// decide character's action for next turn
		characterDecide();

		if (currentSlot == SLOTCOUNT) {
			// all turns decided, execute them
			executingTurns = 1;
		
			// slot to execute first
			currentSlot--;

			// randomize the attack sequence
			int max = currentSlot;
			while (max) {
				int r = rand() % SLOTCOUNT;
				int temp = attackSequence[r];
				attackSequence[r] = attackSequence[max];
				attackSequence[max] = temp;
				max--;
			}
		}
	}

	input.resetAll();
}

void BattleState::render() {
	// render battle background
	glBindTexture(GL_TEXTURE_2D, textures.getTexture(0));
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

	// while drawing, get count of each type of enemy
	std::map<std::string, int> enemyTypes;

	// draw each enemy
	for (int i = 0; i < ENEMYSLOTS; i++) {
		// if enemy exists at location
		if (enemyLocs[i]) {
			int xPos = -windowWidth + enemy[i].getSpriteDim() * (i / 3) + 60;
			int yPos = windowHeight - enemy[i].getSpriteDim() * (i % 3) - 120;

			// render enemy at (xPos, yPos)
			enemy[i].render(xPos, yPos);

			if (enemyTypes.find(enemy[i].getName()) == enemyTypes.end()) {
				// this enemy is not in map yet, add it
				enemyTypes[enemy[i].getName()] = 1;
			} else {
				// enemy already in map, increment count
				enemyTypes[enemy[i].getName()]++;
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

void BattleState::enemyDecide() {
	if (enemy[currentSlot].getName() == "0" || 
			enemy[currentSlot].getAttribute(Enemy::HP) <= 0) {
		// empty slot or dead enemy, no action next turn
		enemyLocs[currentSlot] = 0;
		return;
	}

	// enemy exists and not dead at this location
	enemyLocs[currentSlot] = 1;

	// pick random player
	int target = rand() % CHARACTERSLOTS;

	// while targeted character is dead, choose another
	while (party->hasStatus(static_cast<Party::Characters>(target), 1)) {
		target = rand() % CHARACTERSLOTS;
	}

	// for now just attack randomly chosen character
	enemy[currentSlot].attack(target + ENEMYSLOTS);
}

void BattleState::enemyAct() {
	// enemy attacking
	Enemy *curEnemy = &enemy[attackSequence[currentSlot]];

	// get enemy turn
	Entity::Turn turn = curEnemy->getTurn();
	int attackerHP = curEnemy->HP;
	if (turn.action == Entity::Actions::NONE || attackerHP <= 0) {
		// enemy not taking action or dead
		currentSlot--;
		return;
	}

	curEnemy->blink();

	// if target enemy wanted to attack already dead
	// choose another character to attack at random
	while (party->getAttribute(static_cast<Party::Characters>(turn.target - ENEMYSLOTS), Character::HP) <= 0) {
		turn.target = (rand() % CHARACTERSLOTS) + ENEMYSLOTS;
	}

	// just have enemy hit for 1
	party->addHP(static_cast<Party::Characters>(turn.target - ENEMYSLOTS), -1);

	// turn executed, reset
	curEnemy->resetTurn();
	currentSlot--;
}

void BattleState::characterDecide() {
	// character in current slot
	Party::Characters c = static_cast<Party::Characters>(currentSlot - ENEMYSLOTS);

	// if character dead, move to next slot
	if (party->hasStatus(c, 1)) {
		currentSlot++;
		return;
	}

	// character turn to decide
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
				// if previous character, move back to it
				if (currentSlot != ENEMYSLOTS) {
					currentSlot--;
				}

				// step character back
				party->stepCharBackward(c);

				// cancel previous characters turn
				c = static_cast<Party::Characters>(currentSlot - ENEMYSLOTS);
				party->resetTurn(c);

				input.resetCancel();
			} else if (t.action == Character::Actions::NONE) {
				// player wants to go back a character
				if (currentSlot > ENEMYSLOTS) {
					currentSlot--;
				}	// else already at first character
			} else {
				// player turn finished, step back
				party->stepCharBackward(c);
				currentSlot++;
			}
		}
	}
}

void BattleState::characterAct() {
	// character attacking
	Party::Characters c = static_cast<Party::Characters>(attackSequence[currentSlot] - ENEMYSLOTS);

	// get characters turn
	Entity::Turn turn = party->getTurn(c);
	if (turn.action == Entity::Actions::NONE ||
			party->getAttribute(c, Character::HP) <= 0) {
		// character not taking action or dead
		currentSlot--;
		return;
	}

	// if the enemy the character chose to attack already dead
	// choose another enemy to attack at random
	while (enemy[turn.target].getName() == "0" || 
			enemy[turn.target].getAttribute(Enemy::HP) <= 0) {
		turn.target = rand() % ENEMYSLOTS;
	}

	// just have character hit for rand num 1-10
	int damage = rand() % 10 + 1;
	enemy[turn.target].addHP(-damage);

	// turn executed, reset
	party->resetTurn(c);
	currentSlot--;
}

bool BattleState::battleOver() {
	int totalExp = 0, totalGil = 0;
	// check if all enemies dead
	bool allDead = 1;
	for (int i = 0; i < ENEMYSLOTS; i++) {
		if (enemy[i].getName() != "0" && enemy[i].getAttribute(Enemy::HP) > 0) {
			allDead = 0;
			break;
		}
		totalExp += enemy[i].EXP;
		totalGil += enemy[i].GIL;
	}
	if (allDead) {
		printf("PARTY WINS\n");
		int aliveCharacters = 0;
		for (int i = Party::FIRST; i < Party::SIZE; i++) {
			Party::Characters c = static_cast<Party::Characters>(i);
			if (!party->hasStatus(c, 1)) {
				aliveCharacters++;
			}
			party->resetTurn(c);
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
