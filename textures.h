#ifndef TEXTURES_H
#define TEXTURES_H

#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

class Textures {
	public:
		Textures();
		~Textures();

		// create count textures from files in textureNames
		void createTextures(int count, std::string *textureNames);
		void deleteTextures();

		// get a texture to bind
		GLuint getTexture(int texID) { return textures[texID]; }
	private:
		int textureCount;
		GLuint *textures;

		// load the image and details
		int createTexture(int textureID, const char* filename);
};

#endif
