#ifndef ITEMSHOP_H
#define ITEMSHOP_H

#include "items.h"
#include "shopmenu.h"

class ItemShop : public ShopMenu {
	public:
		ItemShop(std::string type, std::string file);
		~ItemShop();
	private:
		void update();
		void render();

		// draw party stock, # equipped, party members
		// and item description for basic format shop
		void basicDetails(int itemID);

		Items items;

		// down arrow, up arrow, equals sign textures
		Textures textures;

		// how many items scrolled down currently in sell menu
		// (# rows scrolled down) * (2 columns)
		int scrolled;

		// currently selected item to buy/sell
		int itemSelected;

		// amount of itemSelected to buy/sell
		int amountSelected;

		// cursor position before selecting item
		int prevCurPos;
};

#endif
