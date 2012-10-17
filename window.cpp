#include "window.h"

Window::Window() {
	Fullscreen = false;
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
	}
	VideoInfo = SDL_GetVideoInfo();
	if (!VideoInfo) {
		printf("Could not retrieve video information: %s\n", SDL_GetError());
		Resize(640, 480);
	} else {
		int SceneWidth = 720;
		int SceneHeight = 480;
		if (VideoInfo->current_w < SceneWidth) {
			// untested, if screen x res < SceneWidth, set window width = screen x res
			float Ratio = VideoInfo->current_w / SceneWidth;
			SceneHeight = SceneHeight * Ratio;
			Resize(VideoInfo->current_w, SceneHeight);
		} else if (VideoInfo->current_h < SceneHeight) {
			// untested, if screen y res < SceneHeight, set window height = screen y res
			float Ratio = VideoInfo->current_h / SceneHeight;
			SceneWidth = SceneWidth * Ratio;
			Resize(SceneWidth, VideoInfo->current_h);
		} else { // screen res is >= SceneWidthxSceneHeight so set that to be window size
			Resize(SceneWidth, SceneHeight);
		}
		//Resize(VideoInfo->current_w, VideoInfo->current_h);
	}
	SDL_WM_SetCaption("One", 0);
}

bool Window::Resize(int Width, int Height) {
	if (Height == 0) {
		Height = 1;
	}
	WindowWidth = Width;
	WindowHeight = Height;

	if (Fullscreen) {
		VideoFlags = SDL_OPENGL | SDL_FULLSCREEN;
	} else {
		VideoFlags = SDL_OPENGL | SDL_RESIZABLE;
	}

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	if (VideoInfo) {
		if (!SDL_SetVideoMode(Width, Height, VideoInfo->vfmt->BitsPerPixel, VideoFlags)) {
			printf("Unable to set video mode: %s\n", SDL_GetError());
			return false;
		}
	} else {
		if (!SDL_SetVideoMode(Width, Height, 32, VideoFlags)) {
			printf("Unable to set video mode: %s\n", SDL_GetError());
			return false;
		}
	}

	glViewport(0, 0, Width, Height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho (-Width, Width, -Height, Height, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	return true;
}

bool Window::ToggleFullscreen() {
	Fullscreen = !Fullscreen;
	if (!Resize(WindowWidth, WindowHeight)) {
		return false;
	}
	return true;
}
