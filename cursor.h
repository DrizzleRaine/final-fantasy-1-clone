#ifndef CURSOR_H
#define CURSOR_H

#include "textures.h"

class Cursor {
	public:
		Cursor(bool blink = 0);
		~Cursor();

		void setSelection(int selection);
		int getSelection();

		void render(int x, int y);
	private:
		// texture ids and count
		enum TexInfo {CURSOR, COUNT};
		Textures textures;

		// selection is interpreted by the object using it
		// and has no meaning within the cursor class
		int selection;

		// cursor dimensions
		const int CURSORDIM;

		// to align better with text
		const int CURSORPADDING;

		// is this cursor blinking?
		const bool BLINKING;
};

#endif
