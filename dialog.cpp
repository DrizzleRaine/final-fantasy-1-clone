#include "dialog.h"

DialogManager::DialogManager() {
	const char* TextureArray[] = {"img/menu_nw_corner.tga", "img/menu_nw_edge.tga", "img/menu_ne_corner.tga", "img/menu_se_edge.tga", "img/menu_se_corner.tga", "img/menu_sw_corner.tga"};
	DTextures = new Textures(6, TextureArray);;
	TwentyPoint = new Font("fonts/VarelaRound-Regular.ttf", 24);
	Timing = 0;
}

DialogManager::~DialogManager() {
	delete DTextures;
	DTextures = 0;
	
	delete TwentyPoint;
	TwentyPoint = 0;
}

void DialogManager::ConvertNewLines(std::string &T) {
	// strings read from files seem to read \n as two seperate chars
	// rather than interpret them as a newline, fix it
	size_t NewLine = T.find("\\n");
	while (NewLine != std::string::npos) {
		T.replace(NewLine, 2, "\n");
		NewLine = T.find("\\n");
	}
}

void DialogManager::CreateDialog(std::string T) {
	size_t DoubleNewLine = T.find("\\n\\n"); // \n\n is delimeter for new dialog
	while (DoubleNewLine != std::string::npos) { // push all dialogs seperately
		std::string NewText = T.substr(0, DoubleNewLine);
		T = T.substr(DoubleNewLine + 4);
		ConvertNewLines(NewText);
		Text.push(NewText);
		DoubleNewLine = T.find("\\n\\n");
	}
	ConvertNewLines(T);
	Text.push(T); // push final (or only) dialog
	Timing = 0;
}

void DialogManager::DeleteDialog() {
	Text.pop();
}

bool DialogManager::Exists() {
	return Text.size();
}

void DialogManager::TimedDialog(std::string T, unsigned int Duration) {
	Text.push(T);
	Timing = 1;
	Timer = SDL_GetTicks() + Duration;
}


void DialogManager::Render() {
	if (Timing && SDL_GetTicks() > Timer) {
		Timing = 0;
		Text.pop();
	}

	if (Timing && Text.size()) { // assuming ths is town name dialog
		RenderWindow(-WindowWidth + 20, WindowWidth - 20, WindowHeight - 50, WindowHeight - 200);
		TwentyPoint->drawText(-110, WindowHeight - 160, Text.front().c_str()); // need a good way to center text
	} else if (Text.size()) {
		RenderWindow(-WindowWidth + 20, WindowWidth - 20, WindowHeight, WindowHeight - 275);
		TwentyPoint->drawText(-WindowWidth + 70, WindowHeight - 100, Text.front().c_str());
	}
}

void DialogManager::BlueBackground(int X1, int X2, int Y1, int Y2) {
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);

	X1 += 14;
	X2 -= 14;
	Y1 -= 14;
	Y2 += 14;
	glBegin(GL_QUADS);
		glColor3f(0.2f, 0.2f, 0.8f); // lighter blue
		glVertex3f(X1, Y2, 0.0f);
		glVertex3f(X2, Y2, 0.0f);
		glColor3f(0.0f, 0.0f, 0.4f); // darker blue
		glVertex3f(X2, Y1, 0.0f);
		glVertex3f(X1, Y1, 0.0f);
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glColor3f(1.0f, 1.0f, 1.0f);
}

void DialogManager::RenderWindow(int X1, int X2, int Y1, int Y2) {
	// (X1, Y1) = top left corner
	// (X2, Y2) = bottom right corner
	BlueBackground(X1, X2, Y1, Y2);

	glColor3f(1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, DTextures->GLTextures[0]); // nw corner
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

	glBindTexture(GL_TEXTURE_2D, DTextures->GLTextures[1]); // n edge
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

	glBindTexture(GL_TEXTURE_2D, DTextures->GLTextures[2]); // ne corner
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

	glBindTexture(GL_TEXTURE_2D, DTextures->GLTextures[3]); // e edge
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

	glBindTexture(GL_TEXTURE_2D, DTextures->GLTextures[4]); // se corner
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

	glBindTexture(GL_TEXTURE_2D, DTextures->GLTextures[5]); // sw corner
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

void DialogManager::UpdateCoordinates(int NewWidth, int NewHeight) {
	WindowWidth = NewWidth;
	WindowHeight = NewHeight;
}
