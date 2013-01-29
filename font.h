/** BTW, it is covered by the LGPL  ** Bob Pendleton **/
/** http://www.devolution.com/pipermail/sdl/2004-December/066119.html **/

/** 

  +--------------------------------------+
  + Bob Pendleton: writer and programmer +
  + email: Bob@xxxxxxxxxxxxx             +
  + blog:  www.Stonewolf.net             +
  + web:   www.GameProgrammer.com        +
  +--------------------------------------+

	http://www.oreillynet.com/pub/au/1205

	Edits by Kaolin Fire ( kfire@xxxxxxxx; web: erif.org )
		now takes into account glyph minx so glyphs can overlap as intended
		now takes into account newline character

**/

#ifndef FONT_H
#define FONT_H

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

class Font {
	public:
		Font();
		~Font();

		// initialize/cleanup font
		void initFont(const char *fontName, int size);
		void cleanupFont();

		// get line height
		int getLineSkip();

		// get a rectangle around *text
		void textSize(const char *text, SDL_Rect *r);

		// draw a string of text at (x, y)
		float drawText(int x, int y, const char *fmt, ...);

		// change the text color
		void setColor(float r, float g, float b);
	private:
		// keep track of TTF initialization
		static int fontsInitialized;

		// possible glyphs
		static const int MINGLYPH = ' ';
		static const int MAXGLYPH = 126;

		typedef struct {
			int minx, maxx;
			int miny, maxy;
			int advance;
			SDL_Surface *pic;
			GLuint tex;
			GLfloat texMinX, texMinY;
			GLfloat texMaxX, texMaxY;
		} glyph;

		TTF_Font *ttfFont;
		SDL_Color foreground;
		glyph glyphs[MAXGLYPH + 1];

		// size of font, line height
		int pointSize, lineSkip;

		// load a character glyph
		void loadChar(char c);

		// load character texture and texture coordinates
		GLuint loadTexture(SDL_Surface *surface, GLfloat *texcoord);

		// helper function for texture creation
		int power_of_two(int input);
};

#endif
