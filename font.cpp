#include "font.h"

int Font::fontsInitialized = 0;
Font::Font() {
	ttfFont = 0;

	if (!fontsInitialized) {
		if (TTF_Init() < 0) {
			printf("Unable to initialize SDL_TTF: %s\n", TTF_GetError());
			return;
		}
	}
	fontsInitialized++;

	for (int i = MINGLYPH; i <= MAXGLYPH; i++) {
		glyphs[i].pic = 0;
		glyphs[i].tex = 0;
	}

	// initialize font to white
	foreground.r = (Uint8)(255 * 1.0);
	foreground.g  = (Uint8)(255 * 1.0);
	foreground.b = (Uint8)(255 * 1.0);
}

Font::~Font() {
	cleanupFont();

	fontsInitialized--;
	if (!fontsInitialized) {
		TTF_Quit();
	}
}

void Font::initFont(const char *fontName, int size) {
	pointSize = size / 0.4;	// to better match normal point size

	ttfFont = TTF_OpenFont(fontName, pointSize);
	if (ttfFont == 0) {
		printf("Unable to open font file: %s\n", fontName);
		return;
	}

	// store the line height
	lineSkip = TTF_FontLineSkip(ttfFont);

}

void Font::cleanupFont() {
	if (ttfFont) {
		for (int i = MINGLYPH; i <= MAXGLYPH; i++) {
			SDL_FreeSurface(glyphs[i].pic);
		}
		TTF_CloseFont(ttfFont);
		ttfFont = 0;
	}
}

int Font::getLineSkip() {
	return lineSkip;
}

void Font::textSize(const char *text, SDL_Rect *r) {
	int maxx = 0;
	int advance = 0;
	int w_largest = 0;
	char lastchar = 0;

	r->x = 0; r->y = 0;
	r->w = 0; r->h = TTF_FontHeight(ttfFont);

	int i = 0;
	while (text[i]) {
		if ((MINGLYPH <= text[i]) && (text[i] <= MAXGLYPH)) {
			lastchar = text[i];
			if (text[i] == '\n') {
				r->h += lineSkip;
				r->w = r->w - advance + maxx;
				if (r->w > w_largest) {
					w_largest = r->w;
				}
				r->w = 0;
			} else {
				loadChar(text[i]);
				maxx = glyphs[((int)text[i])].maxx;
				advance = glyphs[((int)text[i])].advance;
				r->w += advance;
			}
		}
		i++;
	}
	if (lastchar != '\n') {
		r->w = r->w - advance + maxx;
		if (r->w > w_largest) {
			w_largest = r->w;
		}
	} else {
		r->h -= lineSkip;
	}
	if (w_largest > r->w) {
		r->w = w_largest;
	}
}

float Font::drawText(int x, int y, const char *fmt, ...) {
	char TempText[256];	// Holds Our String
	va_list	ap;			// Pointer To List Of Arguments

	if (fmt == NULL)	// If There's No Text
		*TempText=0;	// Do Nothing
	else {
		va_start(ap, fmt);			// Parses The String For Variables
		vsprintf(TempText, fmt, ap);// And Converts Symbols To Actual Numbers
		va_end(ap);					// Results Are Stored In Text
	}
	const char *text = TempText;
	GLfloat baseleft = x;

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

	while (*text) {
		if (*text == '\n') {
			x = baseleft;
			y -= lineSkip;
		} else if ((MINGLYPH <= *text) && (*text <= MAXGLYPH)) {
			loadChar(*text);
			glyph *g = &glyphs[((int)*text)];

			GLfloat left   = x + g->minx;
			GLfloat right  = x + g->pic->w + g->minx;
			GLfloat bottom = y;
			GLfloat top    = y + g->pic->h;

			glBindTexture(GL_TEXTURE_2D, g->tex);
			glBegin(GL_TRIANGLE_STRIP);
				glTexCoord2f(g->texMinX, g->texMinY); glVertex2f( left,    top);
				glTexCoord2f(g->texMaxX, g->texMinY); glVertex2f(right,    top);
				glTexCoord2f(g->texMinX, g->texMaxY); glVertex2f( left, bottom);
				glTexCoord2f(g->texMaxX, g->texMaxY); glVertex2f(right, bottom);
	        glEnd();

			x += g->advance;
		}
		text++;
	}
	glPopAttrib();
	return x;
}

void Font::setColor(float r, float g, float b) {
	foreground.r = (Uint8)(255 * r);
	foreground.g  = (Uint8)(255 * g);
	foreground.b = (Uint8)(255 * b);
}

void Font::loadChar(char c) {
	GLfloat texcoord[4];
	char letter[2] = {c, 0};

	glyph *g = &glyphs[((int)c)];
	if (c >= MINGLYPH && c <= MAXGLYPH && !g->pic) {
		TTF_GlyphMetrics(ttfFont, (Uint16)c, &g->minx, &g->maxx, &g->miny, &g->maxy, &g->advance);

		SDL_Surface *g0 = 0;
		SDL_Surface *g1 = 0;

		g0 = TTF_RenderText_Blended(ttfFont, letter, foreground);

		if (g0) {
			g1 = SDL_DisplayFormatAlpha(g0);
			SDL_FreeSurface(g0);
		}

		if (g1) {
			g->pic = g1;
			g->tex = loadTexture(g1, texcoord);
			g->texMinX = texcoord[0];
			g->texMinY = texcoord[1];
			g->texMaxX = texcoord[2];
			g->texMaxY = texcoord[3];
		}
	}
}

GLuint Font::loadTexture(SDL_Surface *surface, GLfloat *texcoord) {
	/* Use the surface width and height expanded to powers of 2 */
	int w = power_of_two(surface->w);
	int h = power_of_two(surface->h);
	texcoord[0] = 0.0f;						/* Min X */
	texcoord[1] = 0.0f;						/* Min Y */
	texcoord[2] = (GLfloat)surface->w / w;	/* Max X */
	texcoord[3] = (GLfloat)surface->h / h;	/* Max Y */

	SDL_Surface *image;
	image = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN /* OpenGL RGBA masks */
			0x000000FF, 
			0x0000FF00, 
			0x00FF0000, 
			0xFF000000
#else
			0xFF000000,
			0x00FF0000, 
			0x0000FF00, 
			0x000000FF
#endif
	);
	if (!image) {
		return 0;
	}

	// Save the alpha blending attributes
	Uint32 saved_flags = surface->flags&(SDL_SRCALPHA|SDL_RLEACCELOK);
	Uint8 saved_alpha = surface->format->alpha;
	if ((saved_flags & SDL_SRCALPHA) == SDL_SRCALPHA) {
		SDL_SetAlpha(surface, 0, 0);
	}

	// Copy the surface into the GL texture image
	SDL_Rect area;
	area.x = 0;
	area.y = 0;
	area.w = surface->w;
	area.h = surface->h;
	SDL_BlitSurface(surface, &area, image, &area);

	// Restore the alpha blending attributes
	if ((saved_flags & SDL_SRCALPHA) == SDL_SRCALPHA) {
		SDL_SetAlpha(surface, saved_flags, saved_alpha);
	}

	// Create an OpenGL texture for the image
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

	// free the surface
	SDL_FreeSurface(image);

	return texture;
}

int Font::power_of_two(int input) {
	int value = 1;

	while (value < input) {
		value <<= 1;
	}
	return value;
}

