#include "itemshop.h"

ItemShop::ItemShop(std::string type, std::string file) : ShopMenu(type, file) {
	scrolled = 0;
	itemSelected = -1;

	std::string textureNames[] = {"img/green_up.tga", "img/red_down.tga", "img/equals.tga"};
	textures.createTextures(3, textureNames);

	price = new int[stockCount];
	stock = new std::string[stockCount];

	for (int i = 0; i < stockCount; i++) {
		stock[i] = items.getName(stockID[i]);
		price[i] = items.getCost(stockID[i]);
	}
}

ItemShop::~ItemShop() {
	delete[] price;
	delete[] stock;
}

void ItemShop::update() {
	const int CURSEL = cursor.getSelection();
	int newCurSel = CURSEL;

	// exit menu or cancel option
	if (input->getCancel()) {
		input->resetCancel();
		if (currentOption == NONE) {
			menuState->exitMenus();		// exit shop menu
			return;						// return immediately
		} else if (itemSelected == -1) {
			message = "Thanks! Anything else?";
			newCurSel = currentOption;	// restore cursor
			currentOption = NONE;		// return to menu options
		} else {
			if (currentOption == BUY) {
				message = "So, what'll it be?";
			} else if (currentOption == SELL) {
				message = "What would you like to sell?";
			}
			newCurSel = prevCurPos;		// restore cursor
			prevCurPos = -1;			// unset previous position
			itemSelected = -1;			// cancel item buy/sell
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
			} else if (CURSEL == BUY) {
				message = "So, what'll it be?";
			} else if (CURSEL == SELL) {
				message = "What would you like to sell?";
			}

			// remove timed message
			msgTimer = 0;

			// buy or sell option selected
			currentOption = static_cast<Options>(CURSEL);
			subCursor.setSelection(CURSEL);
			newCurSel = 0;
		} else if (itemSelected > -1) {
			// buy/sell selected item and amount
			int gilToAdd, numberItems = amountSelected;
			if (currentOption == BUY) {
				// removing gil from party, adding items
				gilToAdd = numberItems * items.getCost(itemSelected) * -1;
			} else if (currentOption == SELL) {
				// removing items from party, adding gil
				gilToAdd = numberItems * items.getWorth(itemSelected);
				numberItems *= -1;	// removing item(s)
			}

			// update party gil and inventory
			party->addGil(gilToAdd);
			party->addItem(itemSelected, numberItems);

			// transaction complete
			message = "So, what'll it be?";
			timedMessage = "Thanks!";
			msgTimer = SDL_GetTicks();

			newCurSel = prevCurPos;		// restore cursor
			prevCurPos = -1;			// unset previous position
			itemSelected = -1;			// cancel item buy/sell
		} else if (currentOption == BUY) {
			if (price[CURSEL] > party->getGil()) {
				// party cant afford the item
				timedMessage = "You don't have enough gil.";
				msgTimer = SDL_GetTicks();
			} else {
				// item to buy selected
				itemSelected = stockID[CURSEL];
				amountSelected = 1;

				// set message and remove timed message
				message = "How many would you like?";
				msgTimer = 0;

				// store and reset cursor
				prevCurPos = newCurSel;
				newCurSel = 0;
			}
		} else if (party->getItemCount(party->getItem(CURSEL + scrolled)) > 0) {
			// selling an item party has more than 0 of
			itemSelected = party->getItem(CURSEL + scrolled);
			amountSelected = 1;
			
			// set message and remove timed message
			message = "How many are you selling?";
			msgTimer = 0;

			// store and reset cursor
			prevCurPos = newCurSel;
			newCurSel = 0;
		}
	}

	// cursor movement
	if (itemSelected > -1) {
		// update amount to buy/sell
		if (input->downPressed()) {
			amountSelected -= 10;
		} else if (input->upPressed()) {
			amountSelected += 10;
		} else if (input->rightPressed()) {
			amountSelected++;
		} else if (input->leftPressed()) {
			amountSelected--;
		}

		// keep amount between 1 and 99
		if (amountSelected < 1) {
			amountSelected = 1;
		} else if (amountSelected > 99) {
			amountSelected = 99;
		}

		if (currentOption == BUY) {
			// limit amount able to buy by current party gil
			int maxPartyCanAfford = party->getGil() / price[prevCurPos];
			if (amountSelected > maxPartyCanAfford) {
				amountSelected = maxPartyCanAfford;
			}
		} else if (currentOption == SELL) {
			// limit amount able to sell by party stock
			if (amountSelected > party->getItemCount(itemSelected)) {
				amountSelected = party->getItemCount(itemSelected);
			}
		}
	} else if (currentOption == NONE) {
		if (input->rightPressed()) {
			newCurSel++;
		} else if (input->leftPressed()) {
			newCurSel--;
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
	} else if (currentOption == SELL) {
		if (input->downPressed()) {
			newCurSel += 2;
		} else if (input->upPressed()) {
			newCurSel -= 2;
		} else if (input->rightPressed()) {
			newCurSel++;
		} else if (input->leftPressed()) {
			newCurSel--;
		}

		// scroll at top and bottom
		if (newCurSel < 0) {
			if (newCurSel == -1) {
				// wrap to top right
				newCurSel = 1;
			} else {
				// wrap to top left
				newCurSel = 0;
			}

			// scroll up if not at top
			if (scrolled > 0) {
				scrolled -= 2;
			}
		} else if (newCurSel > 17) {
			if (newCurSel == 18) {
				// wrap to bottom left
				newCurSel = 16;
			} else {
				// wrap to bottom right
				newCurSel = 17;
			}

			// scroll down if not at bottom
			if (scrolled < Party::INVENTORY_SIZE / 2) {
				scrolled += 2;
			}
		}
	}	

	// update cursor selection
	cursor.setSelection(newCurSel);

	// input handled
	input->resetAll();
}

void ItemShop::render() {
	if (itemSelected > -1) {
		// item selected to buy/sell render basic shop format
		basicFormat();

		// draw item name
		twenty.drawText(-windowWidth + 140, windowHeight - 335,
				items.getName(itemSelected).c_str());

		// current amount to buy/sell
		std::string amountStr = std::to_string(amountSelected);

		// draw amount selected right aligned
		SDL_Rect r = {0, 0, 0, 0};
		twenty.textSize(amountStr.c_str(), &r);
		twenty.drawText((windowWidth / 2) - 270 - r.w, windowHeight - 335,
				amountStr.c_str());

		// draw 'gil'
		twenty.drawText((windowWidth / 2) - 320, windowHeight - 410, "gil");

		// current total cost
		std::string totalStr;
		if (currentOption == BUY) {
			totalStr = std::to_string(amountSelected * items.getCost(itemSelected));
		} else if (currentOption == SELL) {
			totalStr = std::to_string(amountSelected * items.getWorth(itemSelected));
		}

		// draw total cost right aligned
		twenty.textSize(totalStr.c_str(), &r);
		twenty.drawText((windowWidth / 2) - 330 - r.w, windowHeight - 410, 
				totalStr.c_str());

		// render everything else
		basicDetails(itemSelected);
	} else if (currentOption == BUY) {
		// basic common shop outline
		basicFormat();

		// render stock list
		renderStock();

		// render everything else
		basicDetails(stockID[cursor.getSelection()]);
	} else if (currentOption == SELL) {
		sellFormat();

		const int LINEHEIGHT = twenty.getLineSkip();
		int yPos, itemID, itemCount;
		std::string name, count;
		for (int i = 0; i < 18; i += 2) {
			// for 9 rows of items with 2 columns
			yPos = windowHeight - 335 - (i / 2) * LINEHEIGHT;

			// first column
			itemID = party->getItem(i + scrolled);
			itemCount = party->getItemCount(itemID);
			name = items.getName(itemID);
			count = std::to_string(itemCount);
			if (itemCount) {
				twenty.drawText(-windowWidth + 100, yPos, name.c_str());
				twenty.drawText(-50, yPos, count.c_str());
			}

			// second column
			itemID = party->getItem(i + 1 + scrolled);
			itemCount = party->getItemCount(itemID);
			name = items.getName(itemID);
			count = std::to_string(itemCount);
			if (itemCount) {
				twenty.drawText(50, yPos, name.c_str());
				twenty.drawText(windowWidth - 100, yPos, count.c_str());
			}
		}

		// item description
		itemID = party->getItem(cursor.getSelection() + scrolled);
		if (party->getItemCount(itemID)) {
			twenty.drawText(-windowWidth + 50, -windowHeight + 35,
					items.getDescription(itemID).c_str());
		}
	} else {
		emptyFormat();
	}

	// ItemShop allows party to sell, draw SELL option
	twenty.drawText(-windowWidth + 300, windowHeight - 214, "Sell");

	// render cursor(s) on top
	cursorRender();
}

void ItemShop::basicDetails(int itemID) {
	// draw 'stock'
	twenty.drawText((windowWidth / 2) - 150, windowHeight - 335, "Stock");

	// number party currently has
	std::string partyStock = std::to_string(party->getItemCount(itemID));

	// draw party stock right aligned
	SDL_Rect r = {0, 0, 0, 0};
	twenty.textSize(partyStock.c_str(), &r);
	twenty.drawText(windowWidth - 70 - r.w, windowHeight - 380, partyStock.c_str());

	// if item is equippable
	if (items.equippable(itemID)) {
		// draw 'equipped'
		twenty.drawText((windowWidth / 2) - 150, windowHeight - 450, "Equipped");

		// find out how many of itemID are currently equipped on party members
		int numEquipped = 0;
		int slot = items.getType(itemID) - Items::WEAPON;
		if (slot >= 0 && slot <= 4) {
			for (int i = Party::FIRST; i < Party::SIZE; i++) {
				Party::Characters c = static_cast<Party::Characters>(i);
				if (party->getEquip(c, slot) == itemID) {
					numEquipped++;
				}
			}
		}

		// draw number equipped right aligned
		const char *numEquippedStr = std::to_string(numEquipped).c_str();
		twenty.textSize(numEquippedStr, &r);
		twenty.drawText(windowWidth - 70 - r.w, windowHeight - 495, numEquippedStr);

		if (currentOption == BUY) {
			// draw characters, dance if can equip and show comparison
			for (int i = Party::FIRST; i < Party::SIZE; i++) {
				Party::Characters c = static_cast<Party::Characters>(i);
				int xPos = -windowWidth + 140 + (windowWidth / 2) * i;

				if (items.equippable(itemID, slot, party->getJobInt(c))) {
					// draw comparison textures
					int result = items.compare(itemID, party->getEquip(c, slot));
					if (result > 0) {		// itemID is better than current equip
						result = 0;
					} else if (result < 0) {// itemID is worse than current equip
						result = 1;
					} else {				// itemID is equal with current equip
						result = 2;
					}

					// render texture
					glBindTexture(GL_TEXTURE_2D, textures.getTexture(result));
					glBegin(GL_QUADS);
						glTexCoord2f(0, 0);
						glVertex3f(xPos - 50, -windowHeight + 254, 0.0f);
						glTexCoord2f(0, 1);
						glVertex3f(xPos - 50, -windowHeight + 230, 0.0f);
						glTexCoord2f(1, 1);
						glVertex3f(xPos - 18, -windowHeight + 230, 0.0f);
						glTexCoord2f(1, 0);
						glVertex3f(xPos - 18, -windowHeight + 254, 0.0f);
					glEnd();

					// TODO: draw character dancing
					party->render(c, xPos, -windowHeight + 320);
				} else {
					party->render(c, xPos, -windowHeight + 320);
				}
			}
		}

		if (itemSelected != -1) {
			// if equipment selected, show its stats
			int vals[2] = {0};
			if (items.getType(itemID) == Items::WEAPON) {
				// weapon, draw atk and acc
				twenty.drawText(-windowWidth + 400, windowHeight - 495, "ATK");
				twenty.drawText(-windowWidth + 400, windowHeight - 540, "ACC");

				// get atk and acc values
				vals[0] = items.getValue(itemID, 0);
				vals[1] = items.getValue(itemID, 1);

				// draw atk and acc values right aligned
				const int RIGHTEDGE = (windowWidth / 2) - 260;
				for (int i = 0; i < 2; i++) {
					int yPos = windowHeight - 495 - 45 * i;

					const char *valStr = std::to_string(vals[i]).c_str();
					twenty.textSize(valStr, &r);
					twenty.drawText(RIGHTEDGE - r.w, yPos, valStr);
					
				}
			} else if (items.getType(itemID) > Items::WEAPON) {
				// armor, draw def and wgt
				twenty.drawText(-windowWidth + 400, windowHeight - 495, "DEF");
				twenty.drawText(-windowWidth + 400, windowHeight - 540, "WGT");

				// get def and wgt values
				vals[0] = items.getValue(itemID, 2);
				vals[1] = items.getValue(itemID, 3) * -1;	// show positive wgt

				// draw def and wgt values right aligned
				const int RIGHTEDGE = (windowWidth / 2) - 260;
				for (int i = 0; i < 2; i++) {
					int yPos = windowHeight - 495 - 45 * i;

					const char *valStr = std::to_string(vals[i]).c_str();
					twenty.textSize(valStr, &r);
					twenty.drawText(RIGHTEDGE - r.w, yPos, valStr);
					
				}
			}
		}
	}

	// item description
	twenty.drawText(-windowWidth + 50, -windowHeight + 35,
				items.getDescription(itemID).c_str());
}
