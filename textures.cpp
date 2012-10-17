#include "textures.h"

Textures::Textures(int NumberOfTextures, const char *TextureNames[]) {
	glEnable(GL_TEXTURE_2D);
	TextureCount = NumberOfTextures;

	GLTextures = new GLuint[TextureCount];
	glGenTextures(TextureCount, GLTextures);
	for (int i = 0; i < TextureCount; i++) {
		CreateTexture(i, TextureNames[i]);
	}
}

Textures::~Textures() {
	DeleteTextures();
}

bool Textures::DeleteTextures() {
	if (GLTextures) {
		glDeleteTextures(TextureCount, GLTextures);
		delete[] GLTextures;
		GLTextures = 0;
		return 1;
	}
	return 0;
}

int Textures::CreateTexture(int TextureNumber, const char* FileName) {
	GLenum TextureFormat;
	GLint Components;

	if (SDL_Surface* Surface = IMG_Load(FileName)) {
		if (Surface->format->BytesPerPixel == 4) {
			TextureFormat = GL_BGRA_EXT;	
			Components = GL_RGBA8;
		} else if (Surface->format->BytesPerPixel == 3) {
			TextureFormat = GL_BGR_EXT;
			Components = GL_RGB8;
		} else if (Surface->format->BytesPerPixel == 1) {
			TextureFormat = GL_LUMINANCE;
			Components = GL_LUMINANCE;
		} else {
			printf("Error: Unsupported format\n");
			SDL_FreeSurface(Surface);
			return 0;
		}
	
		glBindTexture(GL_TEXTURE_2D, GLTextures[TextureNumber]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

		GLfloat HighestAni;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &HighestAni);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, HighestAni);

		glTexImage2D(GL_TEXTURE_2D, 0, Components, Surface->w, Surface->h, 0, TextureFormat, GL_UNSIGNED_BYTE, Surface->pixels);
		SDL_FreeSurface(Surface);
		return 1;
	}
	printf("SDL Image Load Error %s\n", SDL_GetError());
	return 0;
}
