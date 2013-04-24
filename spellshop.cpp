#include "spellshop.h"

SpellShop::SpellShop(std::string type, std::string file) : ShopMenu(type, file), subCursorBuy(1) {
	currentOption = NONE;
	spellSelected = -1;
	message = "Find anything to your liking?";

	std::string textureNames[] = {"img/empty_circle.tga", "img/white_circle.tga", "img/black_circle.tga"};
	textures.createTextures(3, textureNames);

	price = new int[stockCount];
	stock = new std::string[stockCount];

	for (int i = 0; i < stockCount; i++) {
		stock[i] = spells.getName(stockID[i]);

		if (file.substr(0, 8) == "Cornelia") {
			// spells in cornelia cost 50 gil
			price[i] = 50;
		} else {
			price[i] = 0;
		}
	}

	// which type of spells are sold
	magicType = file.find("Black") == std::string::npos;
}

SpellShop::~SpellShop() {
	delete[] price;
	delete[] stock;
}

void SpellShop::update() {
	const int CURSEL = cursor.getSelection();
	int newCurSel = CURSEL;

	// exit menu or cancel option
	if (input->getCancel()) {
		input->resetCancel();
		if (currentOption == NONE) {
			menuState->exitMenus();		// exit shop menu
			return;						// return immediately
		} else if (spellSelected == -1) {
			message = "Will that be all?";
			newCurSel = currentOption;	// restore cursor
			currentOption = NONE;		// return to menu options
		} else {
			message = "Which spell interests you?";
			newCurSel = subCursor.getSelection();	// cursor on spell stock
			subCursor.setSelection(BUY);			// sub cursor on Buy option
			spellSelected = -1;						// cancel item buy/sell
		}

		// remove timed message
		msgTimer = 0;
	}

	// check if option selected
	if (input->getConfirm()) {
		if (currentOption == NONE) {
			if (CURSEL == EXIT) {
				menuState->exitMenus();	// exit shop menu
				return;					// return immediately
			}
			// show buy message
			message = "Which spell interests you?";

			// remove timed message
			msgTimer = 0;

			// buy option selected
			currentOption = BUY;
			subCursor.setSelection(CURSEL);
			newCurSel = 0;
		} else if (spellSelected > -1) {
			Party::Characters c = static_cast<Party::Characters>(cursor.getSelection());
			if (!spells.learnable(spellSelected, party->getJobInt(c))) {
				timedMessage = "You cannot learn this spell.";
			} else if (party->hasSpell(c, spellSelected)) {
				timedMessage = "You already know this spell.";
			} else if (party->addSpell(c, spellSelected, spells.getLevel(spellSelected))) {
				// update party gil
				party->addGil(price[subCursor.getSelection()] * -1);

				// transaction complete
				message = "Which spell interests you?";
				timedMessage = "Thank you.";

				newCurSel = subCursor.getSelection();	// cursor on spell stock
				subCursor.setSelection(BUY);			// sub cursor on Buy option
				spellSelected = -1;						// cancel item buy/sell
			} else {
				// character can not learn more spells for this level
				timedMessage = "You've learned all you can.";
			}
			msgTimer = SDL_GetTicks();
		} else if (currentOption == BUY) {
			if (price[CURSEL] > party->getGil()) {
				// party cant afford the item
				timedMessage = "You don't have enough gil.";
				msgTimer = SDL_GetTicks();
			} else {
				// spell to buy selected
				spellSelected = stockID[CURSEL];

				// set message and remove timed message
				message = "Who will learn the spell?";
				msgTimer = 0;

				// store and reset cursor
				subCursorBuy.setSelection(subCursor.getSelection());
				subCursor.setSelection(cursor.getSelection());
				newCurSel = 0;
			}
		}
	}

	// cursor movement
	if (spellSelected > -1) {
		if (input->rightPressed()) {
			newCurSel++;
		} else if (input->leftPressed()) {
			newCurSel--;
		}

		if (newCurSel < Party::FIRST) {
			newCurSel = Party::FOURTH;
		} else if (newCurSel > Party::FOURTH) {
			newCurSel = Party::FIRST;
		}
	} else if (currentOption == NONE) {
		if (input->rightPressed()) {
			newCurSel += 2;
		} else if (input->leftPressed()) {
			newCurSel -= 2;
		}

		// check for wrap around
		if (newCurSel > EXIT) {
			newCurSel = BUY;
		} else if (newCurSel < BUY) {
			newCurSel = EXIT;
		}
	} else if (currentOption == BUY) {
		if (input->downPressed()) {
			newCurSel++;
		} else if (input->upPressed()) {
			newCurSel--;
		}

		// check for wrap around
		if (newCurSel < 0) {
			newCurSel = stockCount - 1;
		} else if (newCurSel > (stockCount - 1)) {
			newCurSel = 0;
		}
	}	

	// update cursor selection
	cursor.setSelection(newCurSel);

	// input handled
	input->resetAll();
}

void SpellShop::render() {
	// basic common shop outline
	basicFormat();

	// render stock list
	renderStock();

	const int LINEHEIGHT = twenty.getLineSkip();

	// stock spells level and prices
	for (int i = 0; i < stockCount; i++) {
		std::string level = "Lv. " + std::to_string(spells.getLevel(stockID[i]));
		twenty.drawText(-220, windowHeight - 335 - LINEHEIGHT * i, level.c_str());
	}

	if (currentOption == BUY) {
		// party members
		for (int i = Party::FIRST; i < Party::SIZE; i++) {
			int xPos = -windowWidth + 140 + (windowWidth / 2) * i;
			int yPos = -windowHeight + 320;

			// draw character
			Party::Characters c = static_cast<Party::Characters>(i);
			party->render(c, xPos, yPos);

			// if spell selected
			if (spellSelected > -1) {
				xPos += 120;
				
				// show which slots available/taken for this character
				const int CIRCLESIZE = 46;
				for (int j = 0; j < 3; j++) {
					yPos -= CIRCLESIZE;

					// if slot filled
					int slotID = party->getSpell(c, spells.getLevel(stockID[j]), j);
				
					// display correct cirlce	
					if (slotID == -1) {	
						// unfilled slot, empty circle
						glBindTexture(GL_TEXTURE_2D, textures.getTexture(0));
					} else if (spells.learnable(slotID, Character::WHITE)) {
						// filled with white magic spell
						glBindTexture(GL_TEXTURE_2D, textures.getTexture(1));
					} else {
						// filled with black magic spell
						glBindTexture(GL_TEXTURE_2D, textures.getTexture(2));
					}

					// draw circle texture
					glBegin(GL_QUADS);
						glTexCoord2f(0, 0);
						glVertex3f(xPos, yPos + CIRCLESIZE, 0.0f);
						glTexCoord2f(0, 1);
						glVertex3f(xPos, yPos, 0.0f);
						glTexCoord2f(1, 1);
						glVertex3f(xPos + CIRCLESIZE, yPos, 0.0f);
						glTexCoord2f(1, 0);
						glVertex3f(xPos + CIRCLESIZE, yPos + CIRCLESIZE, 0.0f);
					glEnd();
				}
			}
		}

		// white magic and black magic jobs
		Character::Jobs wJobs[] = {Character::RED, Character::WHITE,
			Character::KNIGHT, Character::REDW, Character::WHITEW};
		Character::Jobs bJobs[] = {Character::RED, Character::BLACK, 
			Character::NINJA, Character::REDW, Character::BLACKW};

		// which jobs can learn spells in this shop
		Character::Jobs jobs[5];
		for (int i = 0; i < 5; i++) {
			if (magicType) {	// white magic shop
				jobs[i] = wJobs[i];
			} else {			// black magic shop
				jobs[i] = bJobs[i];
			}
		}

		// list which jobs can learn current spell
		for (int i = 0; i < 5; i++) {
			const char *jobName = party->getJob(jobs[i]).c_str();

			// find spellID currently selected
			int curSpellID;
			if (spellSelected > -1) {
				curSpellID = stockID[subCursor.getSelection()];
			} else {
				curSpellID = stockID[cursor.getSelection()];
			}

			// display job in gray if cant learn, white if can
			if (spells.learnable(curSpellID, jobs[i])) {
				glColor3f(1.0f, 1.0f, 1.0f);	// can learn spell, list in white
			} else {
				glColor3f(0.6f, 0.6f, 0.6f);	// cant learn, list in gray
			}
			twenty.drawText(windowWidth - 500, windowHeight - 335 - LINEHEIGHT * i, 
					jobName);
		}

		// spell description
		int spellID;
		if (spellSelected == -1) {
			spellID = stockID[cursor.getSelection()];
		} else {
			spellID = stockID[subCursor.getSelection()];
		}
		twenty.drawText(-windowWidth + 50, -windowHeight + 35,
				spells.getDescription(spellID).c_str());
	}

	// render cursor(s) on top
	if (spellSelected != -1) {
		// second sub cursor points at buy option
		subCursorBuy.render(-windowWidth + 100, windowHeight - 144);

		// sub cursor points at spell selected
		subCursor.render(-windowWidth + 100, 
				windowHeight - 265 - (subCursor.getSelection() * LINEHEIGHT));

		// cursor points at character to learn
		cursor.render(-windowWidth + 140 + (windowWidth / 2) * cursor.getSelection(), -windowHeight + 300);
	} else {
		cursorRender();
	}
}
