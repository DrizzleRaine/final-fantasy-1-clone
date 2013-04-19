#include "itemmenu.h"
#include "useitemmenu.h"

ItemMenu::ItemMenu() : subCursor(1), subCursorSwap(1) {
	currentOption = USE;
	itemSelected = -1;
	scrolled = 0;
}

ItemMenu::~ItemMenu() {
}

void ItemMenu::update() {
	const int CURSEL = cursor.getSelection();
	int newCurSel = CURSEL;

	// exit menu or cancel option
	if (input->getCancel()) {
		input->resetCancel();
		if (currentOption == NONE) {
			menuState->popMenu();		// exit item menu
			return;						// retun immediately
		} else if (itemSelected >= 0) {
			// re-select first item to chosen to swap
			newCurSel = subCursor.getSelection();
			subCursor.setSelection(subCursorSwap.getSelection());
			itemSelected = -1;			// swap canceled
		} else {
			newCurSel = currentOption;	// restore cursor
			currentOption = NONE;		// return to menu options
		}
	}

	// option or item selected
	if (input->getConfirm()) {
		input->resetConfirm();
		if (itemSelected >= 0) {
			if (itemSelected == (CURSEL + scrolled)) {
				// get the itemID to use
				int itemID = party->getItem(itemSelected);

				// unselect item to use
				newCurSel = subCursor.getSelection();
				subCursor.setSelection(subCursorSwap.getSelection());
				itemSelected = -1;

				// use the item based on its type
				if (items.getType(itemID) == Items::HEAL_SINGLE ||
					items.getType(itemID) == Items::CURE_STATUS) {
					// use on a character, switch to UseItemMenu
					menuState->pushMenu(new UseItemMenu(itemID));
					return;
				} else if (items.getType(itemID) == Items::HEAL_PARTY) {
					printf("TODO HEAL PARTY WITH ITEM %i\n", itemID);
				} else if (itemID != -1) {
					printf("TODO SHOW EQUIP SCREEN FOR %i\n", itemID);
				}
			} else { // swap the items at the two positions
				// second item to swap selected, swap item positions
				party->swapItems(itemSelected, CURSEL + scrolled);
				itemSelected = -1;					// swap completed
			}
		} else if (currentOption == USE) {
			itemSelected = CURSEL + scrolled;		// 1st item to swap selected
			subCursorSwap.setSelection(USE);		// points at use option
			subCursor.setSelection(CURSEL);			// points at 1st swap item
		} else if (CURSEL == USE) {
			currentOption = USE;					// use option selected
			subCursor.setSelection(CURSEL);			// update cursors
			newCurSel = 0;							// start at first item position
		} else if (CURSEL == SORT) {
			party->sortItems();						// sort inventory
			newCurSel = USE;						// return to first menu option
		}
	}

	// cursor movement
	if (currentOption == NONE) {
		if (input->rightPressed()) {
			newCurSel++;
		} else if (input->leftPressed()) {
			newCurSel--;
		}

		// check for wrap around
		if (newCurSel > KEYITEMS) {
			newCurSel = USE;
		} else if (newCurSel < USE) {
			newCurSel = KEYITEMS;
		}
	} else {
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
				// wrap to bottom left
				newCurSel = 0;
			}

			// scroll up if not at top
			if (scrolled > 0) {
				scrolled -= 2;
			}
		} else if (newCurSel > 19) {
			if (newCurSel == 20) {
				// wrap to bottom left
				newCurSel = 18;
			} else {
				// wrap to bottom right
				newCurSel = 19;
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

void ItemMenu::render() {
	// blue background
	blueBackground(windowWidth, windowHeight);

	// borders around submenus
	subBorders();

	// render the text
	renderText();

	// render the cursor(s)
	cursorRender();
}

void ItemMenu::subBorders() {
	const int LEFTX = windowWidth - 400;
	border(-windowWidth, LEFTX, windowHeight, windowHeight - 124);
	border(LEFTX, windowWidth, windowHeight, windowHeight - 124);
	border(-windowWidth, windowWidth, windowHeight - 124, windowHeight - 764);
	border(-windowWidth, windowWidth, windowHeight - 764, -windowHeight);
}

void ItemMenu::renderText() {
	const int LINEHEIGHT = twenty.getLineSkip();

	// menu options
	twenty.drawText(-windowWidth * 0.86, windowHeight - 90, "Use");
	twenty.drawText(-windowWidth * 0.43, windowHeight - 90, "Sort");
	twenty.drawText(0, windowHeight - 90, "Key Items");

	// draw current menu centered in upper right corner
	SDL_Rect r = {0, 0, 0, 0};
	twenty.textSize("Items", &r);
	twenty.drawText(windowWidth - (400 / 2) - (r.w / 2), windowHeight - 90, "Items");

	int y, itemID, itemCount;
	std::string name, count;
	for (int i = 0; i < 20; i += 2) {
		// for 10 rows of items with 2 columns
		y = windowHeight - 210 - (i / 2) * LINEHEIGHT;

		// first column
		itemID = party->getItem(i + scrolled);
		itemCount = party->getItemCount(itemID);
		name = items.getName(itemID);
		count = std::to_string(itemCount);
		if (itemCount) {
			twenty.drawText(-windowWidth + 100, y, name.c_str());
			twenty.drawText(-50, y, count.c_str());
		}

		// second column
		itemID = party->getItem(i + 1 + scrolled);
		itemCount = party->getItemCount(itemID);
		name = items.getName(itemID);
		count = std::to_string(itemCount);
		if (itemCount) {
			twenty.drawText(50, y, name.c_str());
			twenty.drawText(windowWidth - 100, y, count.c_str());
		}
	}

	if (currentOption == USE) {
		// item description
		itemID = party->getItem(cursor.getSelection() + scrolled);
		if (party->getItemCount(itemID)) {
			twenty.drawText(-windowWidth + 50, -windowHeight + 35 + LINEHEIGHT, 
					items.getDescription(itemID).c_str());
		}
	}
}

void ItemMenu::cursorRender() {
	// locations to base cursor drawings
	const int ROW1Y = windowHeight - 140;
	const int COL1X = -windowWidth + 100;	// left col
	const int COL2X = 50;					// right col

	// how far to move when cursor goes to new line
	const int LINEHEIGHT = twenty.getLineSkip();

	if (itemSelected >= 0) {
		// subCursorSwap is pointing at use option
		subCursorSwap.render(-windowWidth * 0.88 + 10, windowHeight - 20);

		// subCursor is pointing at first item to use/swap
		int subSel = itemSelected - scrolled;
		if (subSel >= 0 && subSel < 20) {
			if (subSel % 2) { 	// right column
				subCursor.render(COL2X + 20, ROW1Y - ((subSel / 2) * LINEHEIGHT));
			} else {			// left column
				subCursor.render(COL1X + 20, ROW1Y - ((subSel / 2) * LINEHEIGHT));
			}
		}

		// cursor is pointing at second item to swap
		int curSel = cursor.getSelection();
		if (curSel % 2) {	// right column
			cursor.render(COL2X, ROW1Y - ((curSel / 2) * LINEHEIGHT));
		} else {			// left column
			cursor.render(COL1X, ROW1Y - ((curSel / 2) * LINEHEIGHT));
		}
	} else if (currentOption == NONE) {
		// pointing at a menu option
		if (cursor.getSelection() == KEYITEMS) {
			cursor.render(0, windowHeight - 20);
		} else if (cursor.getSelection() == SORT) {
			cursor.render(-windowWidth * 0.43 - 10, windowHeight - 20);
		} else {
			cursor.render(-windowWidth * 0.88 + 10, windowHeight - 20);
		}
	} else {
		// subCursor is pointing at use option
		subCursor.render(-windowWidth * 0.88 + 10, windowHeight - 20);

		// cursor is pointing at item to use/swap
		int curSel = cursor.getSelection();
		if (curSel % 2) {	// right column
			cursor.render(COL2X, ROW1Y - ((curSel / 2) * LINEHEIGHT));
		} else {			// left column
			cursor.render(COL1X, ROW1Y - ((curSel / 2) * LINEHEIGHT));
		}
	}
}
