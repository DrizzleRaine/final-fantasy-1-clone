#ifndef GUI_H
#define GUI_H

#include "font.h"
#include "textures.h"

class GUIState;

class GUI {
	public:
		GUI();
		~GUI();
	protected:
		// every gui has a font
		Font twenty;

		// render a bordered window
		// (X1, Y1) - top left corner
		// (X2, Y2) - bottom right corner
		void window(int X1, int X2, int Y1, int Y2);

		// render a white border
		void border(int X1, int X2, int Y1, int Y2);

		// render a blue background
		void blueBackground(int X1, int X2, int Y1, int Y2);

		// render a blue background, centered on screen
		void blueBackground(int width, int height);

		// current width and height of window
		int windowWidth, windowHeight;
	private:
		// border texture ids and count
		enum TexInfo {NWCORNER, NWEDGE, NECORNER, SEEDGE, SECORNER, SWCORNER, COUNT};
		// every gui needs border textures
		Textures textures;
};

#endif
