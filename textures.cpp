#include "textures.h"

Textures::Textures() {
	textureNames = 0;
	textures = 0;
}

Textures::~Textures() {
	deleteTextures();
}

Textures::Textures(const Textures &cSource) {
	makeCopy(cSource);
}

Textures &Textures::operator=(const Textures &rhs) {
	// check for self-assignment
	if (this == &rhs) {
		return *this;
	}

	// delete any allocated memory before overwriting
	deleteTextures();

	// perform assignment
	makeCopy(rhs);

	// return existing object for chaining
	return *this;
}

void Textures::makeCopy(const Textures &source) {
	textureCount = source.textureCount;
	if (source.textureNames) {
		textureNames = new std::string[textureCount];
		for (int i = 0; i < textureCount; i++) {
			textureNames[i] = source.textureNames[i];
		}
		if (source.textures) {
			createTextures(textureCount, textureNames);
		}
	}
}

void Textures::createTextures(int count, std::string *textureNames) {
	glEnable(GL_TEXTURE_2D);
	textureCount = count;
	this->textureNames = new std::string[textureCount];

	// generate an array of texture names
	textures = new GLuint[textureCount];
	glGenTextures(textureCount, textures);

	// load textures and fill array
	for (int i = 0; i < textureCount; i++) {
		this->textureNames[i] = textureNames[i];
		createTexture(i, textureNames[i].c_str());
	}
}

void Textures::deleteTextures() {
	if (textureNames) {
		// cleanup texture names array
		delete[] textureNames;
		textureNames = 0;
	}
	if (textures) {
		// cleanup textures array
		glDeleteTextures(textureCount, textures);
		delete[] textures;
		textures = 0;
	}
}

int Textures::createTexture(int textureID, const char* filename) {
	GLenum textureFormat;
	GLint components;

	// load the image
	if (SDL_Surface* surface = IMG_Load(filename)) {
		// get image format
		if (surface->format->BytesPerPixel == 4) {
			textureFormat = GL_BGRA_EXT;	
			components = GL_RGBA8;
		} else if (surface->format->BytesPerPixel == 3) {
			textureFormat = GL_BGR_EXT;
			components = GL_RGB8;
		} else if (surface->format->BytesPerPixel == 1) {
			textureFormat = GL_LUMINANCE;
			components = GL_LUMINANCE;
		} else {
			printf("Error: Unsupported image format\n");
			SDL_FreeSurface(surface);
			return 0;
		}

		// bind the texture	
		glBindTexture(GL_TEXTURE_2D, textures[textureID]);

		// when textures need to be shrunk
		// use weighted averages of nearby elements
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		// when textures need to be enlarged
		// use weighted averages of nearby elements
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		// generate pre-shrunk versions of the image
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

		// if anisotropic filtering extension is supported
		if (strstr((char*) glGetString(GL_EXTENSIONS), "GL_EXT_texture_filter_anisotropic")) {
			// get the maximum anisotropy
			GLfloat highestAni;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &highestAni);

			// anisotropic filtering blends sample points together
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, highestAni);
		}

		// define texture image
		glTexImage2D(GL_TEXTURE_2D, 0, components, surface->w, surface->h, 0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);

		// free the surface
		SDL_FreeSurface(surface);
		return 1;
	}
	printf("SDL Image Load Error: %s\n", SDL_GetError());
	return 0;
}
