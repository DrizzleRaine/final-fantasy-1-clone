#include "title_scene.h"

TitleScene::TitleScene() {
  	glAlphaFunc(GL_GREATER,0.9f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	const char* TextureArray[] = {"img/cursor.tga", "img/title_screen_logo.tga"};
	SceneTextures = new Textures(2, TextureArray);

	SixteenPoint = new Font("fonts/VarelaRound-Regular.ttf", 16);
	TwentyPoint = new Font("fonts/VarelaRound-Regular.ttf", 20);
	CPosition = New;
}

TitleScene::~TitleScene() {
	delete SceneTextures;
	SceneTextures = 0;

	delete SixteenPoint;
	SixteenPoint = 0;
	delete TwentyPoint;
	TwentyPoint = 0;
}

void TitleScene::UpdateCoordinates(int NewWidth, int NewHeight) {
	WindowWidth = NewWidth;
	WindowHeight = NewHeight;
}

void TitleScene::MoveCursor() {
	if (CPosition == New) {
		CPosition = Continue;
	} else {
		CPosition = New;
	}
}

TitleScene::Cursor TitleScene::GetCursor() {
	return CPosition;
}

void TitleScene::RenderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	// draw final fantasy logo
	glBindTexture(GL_TEXTURE_2D, SceneTextures->GLTextures[1]);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(-WindowWidth, WindowHeight, 0.0f);
		glTexCoord2f(0, 1);
		glVertex3f(-WindowWidth, 0.0f, 0.0f);
		glTexCoord2f(1, 1);
		glVertex3f(WindowWidth, 0.0f, 0.0f);
		glTexCoord2f(1, 0);
		glVertex3f(WindowWidth, WindowHeight, 0.0f);
	glEnd();

	// underline it
	glBegin(GL_QUADS);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-WindowWidth + (WindowWidth / 30.0), -10.0f, 0.0f);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-WindowWidth + (WindowWidth / 30.0), -30.0f, 0.0f);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(WindowWidth - (WindowWidth / 30.0), -30.0f, 0.0f);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(WindowWidth - (WindowWidth / 30.0), -10.0f, 0.0f);
	glEnd();

	// write some text
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	TwentyPoint->drawText(-150, -WindowHeight / 2.5, "New Game");
	TwentyPoint->drawText(-150, -WindowHeight / 2.5 - 70, "Continue");
	SixteenPoint->drawText(-WindowWidth + (WindowWidth / 10.0), -WindowHeight, "C 1987, 2004 SQUARE ENIX CO., LTD. All rights reserved.");

	float CursorTop, CursorBottom;
	if (CPosition == Continue) {
		CursorTop = -WindowHeight / 2.5 - 10;
		CursorBottom = -WindowHeight / 2.5 - 110;
	} else {
		CursorTop = -WindowHeight / 2.5 + 60;
		CursorBottom = -WindowHeight / 2.5 - 40;
	}

	// cursor
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_ALPHA_TEST);
	glBindTexture(GL_TEXTURE_2D, SceneTextures->GLTextures[0]);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(-250, CursorTop, 0.0f);
		glTexCoord2f(0, 1);
		glVertex3f(-250, CursorBottom, 0.0f);
		glTexCoord2f(1, 1);
		glVertex3f(-150, CursorBottom, 0.0f);
		glTexCoord2f(1, 0);
		glVertex3f(-150, CursorTop, 0.0f);
	glEnd();
	glDisable(GL_ALPHA_TEST);

	glLoadIdentity();
}
