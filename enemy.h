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

		// execute turn, return damage done
		int act();

		// get enemy attribute
		int getAttribute(Stats s);

		// get sprite dimensions
		int getSpriteDim() { return spriteDim; }
	private:
		// enemy sprite dimensions
		int spriteDim;

		// is enemy animating
		bool animating();

		// enemy sprite texture
		Textures textures;
};

#endif
