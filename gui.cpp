#include "gui.h"

GUI::GUI() {
	twenty.initFont("fonts/VarelaRound-Regular.ttf", 20);
	std::string textureNames[] = {"img/menu_nw_corner.tga", "img/menu_nw_edge.tga", "img/menu_ne_corner.tga", "img/menu_se_edge.tga", "img/menu_se_corner.tga", "img/menu_sw_corner.tga"};
	textures.createTextures(COUNT, textureNames);
}

GUI::~GUI() {
}

void GUI::window(int X1, int X2, int Y1, int Y2) {
	// the border is rounded so the blue 
	// background needs to be shrunk a little
	X1 += 14; X2 -= 14;
	Y1 -= 14; Y2 += 14;

	// draw a blue background
	blueBackground(X1, X2, Y1, Y2);

	// undo the shrinking
	X1 -= 14; X2 += 14;
	Y1 += 14; Y2 -= 14;

	// and draw the rounded border on top
	border(X1, X2, Y1, Y2);
}

void GUI::border(int X1, int X2, int Y1, int Y2) {
	glColor3f(1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, textures.getTexture(NWCORNER)); // nw corner
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

	glBindTexture(GL_TEXTURE_2D, textures.getTexture(NWEDGE)); // n edge
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

	glBegin(GL_QUADS); // render w edge while correct texture already bound
		glTexCoord2f(0, 0);
		glVertex3f(X1, Y1 - 25, 0.0f);
		glTexCoord2f(1, 0);
		glVertex3f(X1, Y2 + 25, 0.0f);
		glTexCoord2f(1, 1);
		glVertex3f(X1 + 20, Y2 + 25, 0.0f);
		glTexCoord2f(0, 1);
		glVertex3f(X1 + 20, Y1 - 25, 0.0f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textures.getTexture(NECORNER)); // ne corner
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

	glBindTexture(GL_TEXTURE_2D, textures.getTexture(SEEDGE)); // e edge
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

	glBegin(GL_QUADS); // render s edge while correct texture already bound
		glTexCoord2f(0, 0);
		glVertex3f(X1 + 25, Y2 + 25, 0.0f);
		glTexCoord2f(0, 1);
		glVertex3f(X1 + 25, Y2, 0.0f);
		glTexCoord2f(1, 1);
		glVertex3f(X2 - 25, Y2, 0.0f);
		glTexCoord2f(1, 0);
		glVertex3f(X2 - 25, Y2 + 25, 0.0f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textures.getTexture(SECORNER)); // se corner
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

	glBindTexture(GL_TEXTURE_2D, textures.getTexture(SWCORNER)); // sw corner
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

void GUI::blueBackground(int X1, int X2, int Y1, int Y2) {
	glDisable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
		glColor3f(0.2f, 0.2f, 0.8f); // lighter blue
		glVertex3f(X1, Y1, 0.0f);
		glVertex3f(X2, Y1, 0.0f);
		glColor3f(0.0f, 0.0f, 0.4f); // darker blue
		glVertex3f(X2, Y2, 0.0f);
		glVertex3f(X1, Y2, 0.0f);
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glColor3f(1.0f, 1.0f, 1.0f);
}

void GUI::blueBackground(int width, int height) {
	glDisable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
		glColor3f(0.2f, 0.2f, 0.8f); // lighter blue
		glVertex3f(-width, height, 0.0f);
		glVertex3f(width, height, 0.0f);
		glColor3f(0.0f, 0.0f, 0.4f); // darker blue
		glVertex3f(width, -height, 0.0f);
		glVertex3f(-width, -height, 0.0f);
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glColor3f(1.0f, 1.0f, 1.0f);
}
