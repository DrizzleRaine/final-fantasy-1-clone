#include "menustate.h"

MenuState::MenuState() {
	head = 0;
	tail = 0;
}

MenuState::~MenuState() {
	// cleanup any menus
	while (tail) {
		popMenu();
	}
}

void MenuState::pushMenu(Menu *menu) {
	// push and initialize the new menu
	if (tail) {
		tail->setNext(menu);
	}
	menu->setPrev(tail);
	tail = menu;
	tail->init(party, &input, this);
}

void MenuState::popMenu() {
	// cleanup and pop current menu
	if (tail) {
		Menu *toPop = tail;
		tail = tail->getPrev();

		delete toPop;
		toPop = 0;
	}
}

void MenuState::changeMenu(Menu *menu) {
	// pop the current menu
	popMenu();

	// push the new menu
	pushMenu(menu);
}

void MenuState::exitMenus(bool startGame) {
	if (startGame) {
		// start the game!
		stateManager->startGame();
		return;	// done with menu state
	}

	// exit menu state
	stateManager->popState();
}

void MenuState::update() {
	tail->updateMenu();
}

void MenuState::render() {
	tail->renderMenu(windowWidth, windowHeight);
}

void MenuState::pause() {
}

void MenuState::unpause() {
}
