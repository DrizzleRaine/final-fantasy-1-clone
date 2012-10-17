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
		Font(const char *fontName, int pointSize);
		~Font();

		void initFont();
		int getLineSkip();
		int getHeight();
		void textSize(char *text, SDL_Rect *r);
		float drawText(int x, int y, const char *fmt, ...);
	private:
		static const int minGlyph = ' ';
		static const int maxGlyph = 126;
		static int initCounter;

		typedef struct {
			int minx, maxx;
			int miny, maxy;
			int advance;
			SDL_Surface *pic;
			GLuint tex;
			GLfloat texMinX, texMinY;
			GLfloat texMaxX, texMaxY;
		} glyph;

		const char *fontName;
		float fgRed, fgGreen, fgBlue;
		int height, pointSize, lineSkip;
		int ascent, descent;

		TTF_Font *ttfFont;
		SDL_Color foreground;
		glyph glyphs[maxGlyph + 1];

		int power_of_two(int input);
		void loadChar(char c);
		GLuint SDL_GL_LoadTexture(SDL_Surface *surface, GLfloat *texcoord);
};

#endif
