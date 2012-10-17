#include "title.h"

Title::Title(Input *GInput) {
	TInput = GInput;
	TScene = new TitleScene;
}

Title::~Title() {
	delete TScene;
	TScene = 0;
}

void Title::Update() {
	TScene->RenderScene();
	SDL_GL_SwapBuffers();
}

bool Title::Process() {
	if (TInput->Keys[68]) {
		if (TScene->GetCursor() == TitleScene::New) {
			return 0;
		} else {
			printf("TODO: Continue :)\n");
		}
		TInput->Keys[68] = 0; // handled
	}

	if (TInput->UpPressed() || TInput->DownPressed()) {
		TScene->MoveCursor();
	}
	return 1;
}

void Title::UpdateCoordinates(int NewWidth, int NewHeight) {
	TScene->UpdateCoordinates(NewWidth, NewHeight);
}
