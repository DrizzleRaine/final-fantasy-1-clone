#ifndef TITLE_SCENE_H
#define TITLE_SCENE_H

#include "textures.h"
#include "font.h"

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

class TitleScene {
	public:
		TitleScene();
		~TitleScene();
		void RenderScene();
		void UpdateCoordinates(int NewWidth, int NewHeight);

		enum Cursor {New, Continue};
		void MoveCursor();
		Cursor GetCursor();
	private:
		Font *SixteenPoint;
		Font *TwentyPoint;
		Textures *SceneTextures;
		Cursor CPosition;
		int WindowWidth, WindowHeight;
};

#endif
