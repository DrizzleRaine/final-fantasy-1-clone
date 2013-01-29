#ifndef TITLESTATE_H
#define TITLESTATE_H

#include "font.h"
#include "textures.h"
#include "cursor.h"

#include "state.h"
#include "menustate.h"
#include "newmenu.h"

class TitleState : public State {
	public:
		TitleState();
		~TitleState();
	private:
		// operations
		void update();
		void render();

		// texture ids and texture count
		enum TexInfo {LOGO, COUNT};
		Textures textures;

		// fonts 
		Font sixteen, twenty;

		// available options for the cursor
		enum Options {NEW, CONTINUE};
		Cursor cursor;
};

#endif
