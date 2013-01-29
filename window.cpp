#include "window.h"

Window::Window() {
	videoInfo = SDL_GetVideoInfo();
	if (!videoInfo) {
		// not fatal, but will assume very low resolution settings
		printf("Warning: Could not retrieve video information: %s\n", SDL_GetError());
		resize(640, 480);
	} else {
		int sceneWidth = 720, sceneHeight = 480;	// the default resolution
		if (videoInfo->current_w < sceneWidth) {
			// untested
			// desired: if screen x res < sceneWidth
			// 			set window width = screen x res
			float ratio = videoInfo->current_w / sceneWidth;
			sceneHeight = sceneHeight * ratio;
			resize(videoInfo->current_w, sceneHeight);
		} else if (videoInfo->current_h < sceneHeight) {
			// untested
			// desired: if screen y res < sceneHeight
			// 			set window height = screen y res
			float ratio = videoInfo->current_h / sceneHeight;
			sceneWidth = sceneWidth * ratio;
			resize(sceneWidth, videoInfo->current_h);
		} else { 
			// screen res is >= SceneWidth x SceneHeight 
			// so set that to be window size
			resize(sceneWidth, sceneHeight);
		}
	}
	SDL_WM_SetCaption("One", 0);
}

bool Window::resize(int newWidth, int newHeight) {
	// make sure no 0 lengths
	if (newHeight == 0) {
		newHeight = 1;
	}
	if (newWidth == 0) {
		newWidth = 1;
	}

	// update size
	width = newWidth;
	height = newHeight;

	// set flags
	videoFlags = SDL_OPENGL | SDL_RESIZABLE;	// for testing
	//videoFlags = SDL_OPENGL;

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// and resize the window
	if (videoInfo) {
		if (!SDL_SetVideoMode(width, height, videoInfo->vfmt->BitsPerPixel, videoFlags)) {
			printf("Unable to set video mode: %s\n", SDL_GetError());
			return false;
		}
	} else {
		if (!SDL_SetVideoMode(width, height, 32, videoFlags)) {
			printf("Unable to set video mode: %s\n", SDL_GetError());
			return false;
		}
	}

	// set up the viewport
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// using a coordinate system with 0 in the center
	// each quadrants size is width x height
	glOrtho (-width, width, -height, height, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	return true;
}
