#ifndef TEXTURES_H
#define TEXTURES_H

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

class Textures {
	public:
		Textures(int NumberOfTextures, const char *TextureNames[]);
		~Textures();
		GLuint *GLTextures;
	private:
		int TextureCount;
		// TODO int gltIsExtSupported(const char *extension); // ANISOTROPY
		int CreateTexture(int TextureNumber, const char* FileName);
		bool DeleteTextures();
};

#endif
