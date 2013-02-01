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

		// copy ctor and assignment operator
		Textures(const Textures &cSource);
		Textures &operator=(const Textures &rhs);

		// create count textures from files in textureNames
		void createTextures(int count, std::string *textureNames);
		void deleteTextures();

		// get a texture to bind
		GLuint getTexture(int texID) { return textures[texID]; }
	private:
		// texture count and names
		int textureCount;
		std::string *textureNames;

		// array of textures
		GLuint *textures;

		// load the image and details
		int createTexture(int textureID, const char* filename);

		// make a copy of this texture object
		void makeCopy(const Textures &source);
};

#endif
