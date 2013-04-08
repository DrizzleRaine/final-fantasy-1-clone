#include "shopmenu.h"

ShopMenu::ShopMenu() : subCursor(1) {
	currentOption = NONE;
}

ShopMenu::~ShopMenu() {
}

void ShopMenu::renderStock() {
	const int LINEHEIGHT = twenty.getLineSkip();
	SDL_Rect r = {0, 0, 0, 0};

	for (int yPos, i = 0; i < stockCount; i++) {
		yPos = windowHeight - 335 - (LINEHEIGHT * i);

		// draw stock name
		twenty.drawText(-windowWidth + 140, yPos, stock[i].c_str());

		// draw price right aligned
		std::string priceStr = std::to_string(price[i]);
		const int RIGHTEDGE = (windowWidth / 2) - 260;
		twenty.textSize(priceStr.c_str(), &r);
		twenty.drawText(RIGHTEDGE - r.w, yPos, priceStr.c_str());
	}
}

void ShopMenu::subWindows() {
	// blue background
	blueBackground(windowWidth, windowHeight);

	// message and shop type borders
	const int LEFTXTOP = windowWidth - 400;
	border(-windowWidth, LEFTXTOP, windowHeight, windowHeight - 124);
	border(LEFTXTOP, windowWidth, windowHeight, windowHeight - 124);

	// menu options and party gil border
	const int LEFTXOPTIONS = (windowWidth / 2) - 350;
	border(-windowWidth, LEFTXOPTIONS, windowHeight - 124, windowHeight - 248);
	border(LEFTXOPTIONS, windowWidth, windowHeight - 124, windowHeight - 248);

	// text in submenus
	subText();
}

void ShopMenu::emptyFormat() {
	// render common subwindows
	subWindows();

	// border the rest of the window
	border(-windowWidth, windowWidth, windowHeight - 248, -windowHeight);
}

void ShopMenu::basicFormat() {
	// render common subwindows
	subWindows();

	// divider x pos between stock and party count
	const int LEFTXBUY = (windowWidth / 2) - 200;

	// shop stock and party count borders
	border(-windowWidth, LEFTXBUY, windowHeight - 248, windowHeight - 600);
	border(LEFTXBUY, windowWidth, windowHeight - 248, windowHeight - 600);

	// party and description borders
	border(-windowWidth, windowWidth, windowHeight - 600, windowHeight - 836);
	border(-windowWidth, windowWidth, windowHeight - 836, -windowHeight);
}

void ShopMenu::sellFormat() {
	// render common subwindows
	subWindows();

	// party items and description borders
	border(-windowWidth, windowWidth, windowHeight - 248, windowHeight - 836);
	border(-windowWidth, windowWidth, windowHeight - 836, -windowHeight);
}

void ShopMenu::subText() {
	// draw current message
	twenty.drawText(-windowWidth + 50, windowHeight - 90, "Welcome!");

	// draw shop type centered in upper right sub window	
	SDL_Rect r = {0, 0, 0, 0};
	twenty.textSize(shopType.c_str(), &r);
	twenty.drawText(windowWidth - (400 / 2) - (r.w / 2), windowHeight - 90, 
			shopType.c_str());

	// menu options
	twenty.drawText(-windowWidth + 100, windowHeight - 214, "Buy");
	twenty.drawText(-windowWidth + 500, windowHeight - 214, "Exit");

	// party gil
	twenty.drawText(windowWidth - 120, windowHeight - 214, "gil");

	// gil amount is right aligned
	const int RIGHTEDGE = windowWidth - 150;
	std::string gilAmount = std::to_string(party->getGil());

	// draw the gil amount right aligned
	twenty.textSize(gilAmount.c_str(), &r);
	twenty.drawText(RIGHTEDGE - r.w, windowHeight - 214, gilAmount.c_str());
}

void ShopMenu::cursorRender() {
	// locations to base cursor drawings
	const int ROW1Y = windowHeight - 265;
	const int COL1X = -windowWidth + 100;	// left col
	const int COL2X = 50;					// right col

	// menu option locations
	const int OPTIONSY = windowHeight - 144;
	const int BUYX = -windowWidth + 100;
	const int SELLX = BUYX + 200;
	const int EXITX = SELLX + 200;

	// how far to move when cursor goes to new line
	const int LINEHEIGHT = twenty.getLineSkip();

	if (currentOption == NONE) {
		// pointing at a menu option
		if (cursor.getSelection() == BUY) {
			cursor.render(BUYX, OPTIONSY);
		} else if (cursor.getSelection() == SELL) {
			cursor.render(SELLX, OPTIONSY);
		} else if (cursor.getSelection() == EXIT) {
			cursor.render(EXITX, OPTIONSY);
		}
	} else if (currentOption == BUY) { 
		// subCursor is pointing at buy option
		subCursor.render(BUYX, OPTIONSY);

		// cursor is pointing at stock
		cursor.render(COL1X, ROW1Y - (cursor.getSelection() * LINEHEIGHT));
	} else if (currentOption == SELL) {
		// subCursor is pointing at sell option
		subCursor.render(SELLX, OPTIONSY);

		// cursor is pointing at item
		const int CURSEL = cursor.getSelection();
		if (CURSEL % 2) {	// right column
			cursor.render(COL2X, ROW1Y - ((CURSEL / 2) * LINEHEIGHT));
		} else {			// left column
			cursor.render(COL1X, ROW1Y - ((CURSEL / 2) * LINEHEIGHT));
		}
	}
}
