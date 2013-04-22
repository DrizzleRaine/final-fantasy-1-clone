#ifndef SPELLSHOP_H
#define SPELLSHOP_H

#include "spells.h"
#include "shopmenu.h"

class SpellShop : public ShopMenu {
	public:
		SpellShop(std::string type, std::string file);
		~SpellShop();
	private:
		void update();
		void render();

		Spells spells;

		// TODO circle, filled circle
		Textures textures;

		// second sub cursor always points at Buy
		Cursor subCursorBuy;

		// currently selected spell to buy/sell
		int spellSelected;

		// type of spells, black (0) or white (1)
		bool magicType;
};

#endif
