#include "menustate.h"
#include "world.h"

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
	while (tail) {
		popMenu();	// exit menus
	}

	// exit menu state
	stateManager->popState();

	if (startGame) {
		// exit title screen
		stateManager->popState();

		// push map state and world map
		MapState *ms = new MapState();
		stateManager->pushState(ms);
		ms->pushMap(new World());
	}
}

void MenuState::update() {
	tail->updateMenu();

	// input handled
	input.resetAll();
}

void MenuState::render() {
	tail->renderMenu(windowWidth, windowHeight);
}
