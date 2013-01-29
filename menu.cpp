#include "menu.h"

Menu::Menu() {
	input = 0;
	party = 0;
	menuState = 0;

	next = 0;
	prev = 0;
}

Menu::~Menu() {
}

void Menu::init(Party *p, Input *in, MenuState *ms) {
	// initialize shared resources
	party = p;
	input = in;
	menuState = ms;
}

void Menu::updateMenu() {
	update();
}

void Menu::renderMenu(int width, int height) {
	// update window dimensions
	windowWidth = width;
	windowHeight = height;

	// render scene
	render();
}
