#ifndef ENEMY_H
#define ENEMY_H

#include <fstream>

#include "entity.h"
#include "textures.h"

class Enemy : public Entity {
	public:
		Enemy();
		~Enemy();

		// initialize enemy
		void init(std::string name);

		// render enemy at (x, y)
		void render(int x, int y);

		// start blinking darker before executing turn
		void blink();

		// get enemy attribute
		int getAttribute(Stats s);

		// get sprite dimensions
		int getSpriteDim() { return spriteDim; }
	private:
		// enemy sprite dimensions
		int spriteDim;

		// ticks when started blinking
		unsigned int blinkTicks;

		// is enemy blinking
		bool blinking();

		// enemy sprite texture
		Textures textures;
};

#endif
