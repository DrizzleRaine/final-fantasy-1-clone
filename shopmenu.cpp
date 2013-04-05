#include "shopmenu.h"

ShopMenu::ShopMenu() : subCursor(1) {
	currentOption = NONE;
}

ShopMenu::~ShopMenu() {
}

void ShopMenu::updateShop() {
	const int CURSEL = cursor.getSelection();
	int newCurSel = CURSEL;

	// exit menu or cancel option
	if (input->getCancel()) {
		input->resetCancel();
		if (currentOption == NONE) {
			menuState->exitMenus();
			return;
		} else {
			newCurSel = currentOption;				// restore cursor
			currentOption = NONE;					// return to menu options
		}
	}

	// update cursor selection
	cursor.setSelection(newCurSel);

	// input handled
	input->resetAll();
}

void ShopMenu::renderShop() {
	// blue background and border entire window
	blueBackground(windowWidth, windowHeight);
	border(-windowWidth, windowWidth, windowHeight, -windowHeight);

	// borders around submenus
	subBorders();

	// text in submenus
	subText();

	// render the cursor(s)
	cursorRender();
}

void ShopMenu::subBorders() {
}

void ShopMenu::subText() {
}

void ShopMenu::cursorRender() {
}
