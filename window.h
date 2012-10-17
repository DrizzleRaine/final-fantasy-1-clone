#ifndef WINDOW_H
#define WINDOW_H

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

class Window {
	public:
		Window();
		bool Resize(int Width, int Height);
		bool ToggleFullscreen();

		int GetWindowWidth() { return WindowWidth; }
		int GetWindowHeight() { return WindowHeight; }
	private:
		bool Fullscreen;
		const SDL_VideoInfo *VideoInfo;
		int VideoFlags, WindowWidth, WindowHeight;
};

#endif
