#include "submenu.h"

SubMenu::SubMenu(Input *I, Textures *T, Font *F, Character *C, int *CPos) {
	MInput = I;
	SceneTextures = T;
	TwentyPoint = F;
	Characters = C;
	CPositions = CPos;
}

SubMenu::~SubMenu() {
}

void SubMenu::DrawCursor(float CursorX, float CursorY, bool Blinking) {
	if ((Blinking && SDL_GetTicks() % 20 > 9) || !Blinking) {
		glPushAttrib(GL_ENABLE_BIT);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_ALPHA_TEST);
		glBindTexture(GL_TEXTURE_2D, SceneTextures->GLTextures[6]);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0);
			glVertex2f(CursorX, CursorY);
			glTexCoord2f(0, 1);
			glVertex2f(CursorX, CursorY - 100);
			glTexCoord2f(1, 1);
			glVertex2f(CursorX + 100, CursorY - 100);
			glTexCoord2f(1, 0);
			glVertex2f(CursorX + 100, CursorY);
		glEnd();
		glPopAttrib();
	}
}

void SubMenu::RenderWindow(int X1, int X2, int Y1, int Y2) {
	// (X1, Y1) = top left corner
	// (X2, Y2) = bottom right corner
	glColor3f(1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, SceneTextures->GLTextures[0]); // nw corner
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(X1, Y1, 0.0f);
		glTexCoord2f(0, 1);
		glVertex3f(X1, Y1 - 26, 0.0f);
		glTexCoord2f(1, 1);
		glVertex3f(X1 + 26, Y1 - 26, 0.0f);
		glTexCoord2f(1, 0);
		glVertex3f(X1 + 26, Y1, 0.0f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, SceneTextures->GLTextures[1]); // n edge
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(X1 + 25, Y1, 0.0f);
		glTexCoord2f(0, 1);
		glVertex3f(X1 + 25, Y1 - 20, 0.0f);
		glTexCoord2f(1, 1);
		glVertex3f(X2 - 25, Y1 - 20, 0.0f);
		glTexCoord2f(1, 0);
		glVertex3f(X2 - 25, Y1, 0.0f);
	glEnd();

	glBegin(GL_QUADS); // draw w edge while correct texture already bound
		glTexCoord2f(0, 0);
		glVertex3f(X1, Y1 - 25, 0.0f);
		glTexCoord2f(1, 0);
		glVertex3f(X1, Y2 + 25, 0.0f);
		glTexCoord2f(1, 1);
		glVertex3f(X1 + 20, Y2 + 25, 0.0f);
		glTexCoord2f(0, 1);
		glVertex3f(X1 + 20, Y1 - 25, 0.0f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, SceneTextures->GLTextures[2]); // ne corner
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(X2 - 26, Y1, 0.0f);
		glTexCoord2f(0, 1);
		glVertex3f(X2 - 26, Y1 - 26, 0.0f);
		glTexCoord2f(1, 1);
		glVertex3f(X2, Y1 - 26, 0.0f);
		glTexCoord2f(1, 0);
		glVertex3f(X2, Y1, 0.0f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, SceneTextures->GLTextures[3]); // e edge
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(X2 - 25, Y1 - 25, 0.0f);
		glTexCoord2f(1, 0);
		glVertex3f(X2 - 25, Y2 + 25, 0.0f);
		glTexCoord2f(1, 1);
		glVertex3f(X2, Y2 + 25, 0.0f);
		glTexCoord2f(0, 1);
		glVertex3f(X2, Y1 - 25, 0.0f);
	glEnd();

	glBegin(GL_QUADS); // draw s edge while correct texture already bound
		glTexCoord2f(0, 0);
		glVertex3f(X1 + 25, Y2 + 25, 0.0f);
		glTexCoord2f(0, 1);
		glVertex3f(X1 + 25, Y2, 0.0f);
		glTexCoord2f(1, 1);
		glVertex3f(X2 - 25, Y2, 0.0f);
		glTexCoord2f(1, 0);
		glVertex3f(X2 - 25, Y2 + 25, 0.0f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, SceneTextures->GLTextures[4]); // se corner
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(X2 - 26, Y2 + 26, 0.0f);
		glTexCoord2f(0, 1);
		glVertex3f(X2 - 26, Y2, 0.0f);
		glTexCoord2f(1, 1);
		glVertex3f(X2, Y2, 0.0f);
		glTexCoord2f(1, 0);
		glVertex3f(X2, Y2 + 26, 0.0f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, SceneTextures->GLTextures[5]); // sw corner
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(X1, Y2 + 26, 0.0f);
		glTexCoord2f(0, 1);
		glVertex3f(X1, Y2, 0.0f);
		glTexCoord2f(1, 1);
		glVertex3f(X1 + 26, Y2, 0.0f);
		glTexCoord2f(1, 0);
		glVertex3f(X1 + 26, Y2 + 26, 0.0f);
	glEnd();
}

void SubMenu::DrawCharacter(int Type, float XPos, float YPos) {
	glBindTexture(GL_TEXTURE_2D, SceneTextures->GLTextures[Type+7]);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(XPos, YPos, 0.0f);
		glTexCoord2f(0, 1);
		glVertex3f(XPos, YPos - 148, 0.0f);
		glTexCoord2f(1, 1);
		glVertex3f(XPos + 120, YPos - 148, 0.0f);
		glTexCoord2f(1, 0);
		glVertex3f(XPos + 120, YPos, 0.0f);
	glEnd();
}

void SubMenu::BlueBackground() {
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
		glColor3f(0.2f, 0.2f, 0.8f); // lighter blue
		glVertex3f(-WindowWidth, WindowHeight, 0.0f);
		glVertex3f(WindowWidth, WindowHeight, 0.0f);
		glColor3f(0.0f, 0.0f, 0.4f); // darker blue
		glVertex3f(WindowWidth, -WindowHeight, 0.0f);
		glVertex3f(-WindowWidth, -WindowHeight, 0.0f);
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glColor3f(1.0f, 1.0f, 1.0f);
}

const char *SubMenu::JobName(int Job) {
	switch(Job) {
		case 1:
			return "Thief";
		case 2:
			return "White Mage";
		case 3:
			return "Black Mage";
		case 4:
			return "Monk";
		case 5:
			return "Red Mage";
		default:
			return "Warrior";
	}
};

void SubMenu::UpdateCoordinates(int NewWidth, int NewHeight) {
	WindowWidth = NewWidth;
	WindowHeight = NewHeight;
}
