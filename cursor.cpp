#include "cursor.h"

Cursor::Cursor(bool blink) : CURSORDIM(100), CURSORPADDING(10), BLINKING(blink) {
	selection = 0;
	std::string textureNames[] = {"img/cursor.tga"};
	textures.createTextures(COUNT, textureNames);
}

Cursor::~Cursor() {
	textures.deleteTextures();
}

void Cursor::setSelection(int selection) {
	this->selection = selection;
}

int Cursor::getSelection() {
	return selection;
}

void Cursor::render(int x, int y) {
	if (!BLINKING || (BLINKING && SDL_GetTicks() % 20 > 9)) {
		int cursorTop = y - CURSORPADDING;
		int cursorBottom = y - CURSORPADDING - CURSORDIM;

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, textures.getTexture(CURSOR));
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0);
			glVertex3f(x - CURSORDIM, cursorTop, 0.0f);
			glTexCoord2f(0, 1);
			glVertex3f(x - CURSORDIM, cursorBottom, 0.0f);
			glTexCoord2f(1, 1);
			glVertex3f(x, cursorBottom, 0.0f);
			glTexCoord2f(1, 0);
			glVertex3f(x, cursorTop, 0.0f);
		glEnd();
	}
}
