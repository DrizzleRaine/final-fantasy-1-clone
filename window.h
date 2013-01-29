#ifndef WINDOW_H
#define WINDOW_H

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

class Window {
	public:
		Window();

		bool resize(int newWidth, int newHeight);

		int getWidth() { return width; }
		int getHeight() { return height; }
	private:
		const SDL_VideoInfo *videoInfo;
		int videoFlags, width, height;
};

#endif
