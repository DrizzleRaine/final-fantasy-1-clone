#include "titlestate.h"

TitleState::TitleState() {
	std::string textureNames[] = {"img/title_screen_logo.tga"};
	textures.createTextures(COUNT, textureNames);

	sixteen.initFont("fonts/VarelaRound-Regular.ttf", 16);
	twenty.initFont("fonts/VarelaRound-Regular.ttf", 20);
}

TitleState::~TitleState() {
}

void TitleState::update() {
	if (input.upPressed() || input.downPressed()) {
		// move cursor
		if (cursor.getSelection() == CONTINUE) {
			cursor.setSelection(NEW);
		} else {
			cursor.setSelection(CONTINUE);
		}
	}

	if (input.getConfirm()) {
		// select option
		if (cursor.getSelection() == NEW) {
			MenuState *ms = new MenuState();
			stateManager->pushState(ms);
			ms->pushMenu(new NewMenu());
		} else {
			// load in party data from some file
			printf("TODO: Continue :)\n");
		}
	}

	// input handled
	input.resetAll();
}

void TitleState::render() {
	// final fantasy logo
	glBindTexture(GL_TEXTURE_2D, textures.getTexture(LOGO));
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(-windowWidth, windowHeight, 0.0f);
		glTexCoord2f(0, 1);
		glVertex3f(-windowWidth, 0.0f, 0.0f);
		glTexCoord2f(1, 1);
		glVertex3f(windowWidth, 0.0f, 0.0f);
		glTexCoord2f(1, 0);
		glVertex3f(windowWidth, windowHeight, 0.0f);
	glEnd();

	// underline the logo
	const float XPADDING = windowWidth / 30.0;
	const float YPADDING = 10.0;
	const float LINEHEIGHT = 20.0;

	glBegin(GL_QUADS);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-windowWidth + XPADDING, -YPADDING, 0.0f);
		glVertex3f(-windowWidth + XPADDING, -YPADDING - LINEHEIGHT, 0.0f);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(windowWidth - XPADDING, -YPADDING - LINEHEIGHT, 0.0f);
		glVertex3f(windowWidth - XPADDING, -YPADDING, 0.0f);
	glEnd();

	// render text
	const int MENUX = -150;
	const int MENUY = -windowHeight / 2.5;
	const int NEWLINE = 70;

	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);	// black text
	twenty.drawText(MENUX, MENUY, "New Game");
	twenty.drawText(MENUX, MENUY - NEWLINE, "Continue");

	// copyright notice
	sixteen.drawText(-windowWidth + (windowWidth / 10.0), -windowHeight, 
				"C 1987, 2004 SQUARE ENIX CO., LTD. All rights reserved.");

	// cursor
	if (cursor.getSelection() == CONTINUE) {
		cursor.render(MENUX, MENUY);
	} else { // default to NEW
		cursor.render(MENUX, MENUY + NEWLINE);
	}
}

void TitleState::pause() {
}

void TitleState::unpause() {
}
