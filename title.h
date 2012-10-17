#ifndef TITLE_H
#define TITLE_H

#include "input.h"
#include "title_scene.h"

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

class Title {
	public:
		Title(Input *GInput);
		~Title();
		void Update();
		bool Process();
		void UpdateCoordinates(int NewWidth, int NewHeight);
	private:
		Input *TInput;
		TitleScene *TScene;
};


#endif
