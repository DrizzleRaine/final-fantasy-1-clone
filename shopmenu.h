#ifndef SHOPMENU_H
#define SHOPMENU_H

#include "menu.h"

class ShopMenu : public Menu {
	public:
		ShopMenu();
		virtual ~ShopMenu();
	protected:
		// shop type to display
		std::string shopType;

		// message to display
		std::string message;

		// message to display for a brief time
		unsigned int msgTimer;
		std::string timedMessage;

		// number of available goods
		int stockCount;

		// IDs, prices and names of stock for sale
		int *stockID, *price;
		std::string *stock;

		// the menu options available in all shop menus
		enum Options {NONE = -1, BUY = 0, SELL, EXIT, COUNT};
	
		// the currently active menu option	
		Options currentOption;

		// blinking sub cursor
		Cursor subCursor;

		// render shop stock listing
		void renderStock();

		// shop with no option selected
		void emptyFormat();

		// shop listing stock or buying/selling chosen stock
		void basicFormat();

		// shop listing party items to sell
		void sellFormat();

		// render the cursor(s);
		void cursorRender();
	private:
		// operations
		virtual void update() = 0;
		virtual void render() = 0;

		// render subwindows common to all shops
		void subWindows();

		// text for the submenus
		void subText();
};

#endif
