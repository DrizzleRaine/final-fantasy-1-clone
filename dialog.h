#ifndef DIALOG_H
#define DIALOG_H

#include "font.h"
#include "textures.h"

#include <string>
#include <queue>
#include <SDL/SDL.h>

class DialogManager {
	public:
		DialogManager();
		~DialogManager();

		void Render();
		bool Exists();
		void CreateDialog(std::string T);
		void DeleteDialog();
		void TimedDialog(std::string T, unsigned int Duration);
		void UpdateCoordinates(int NewWidth, int NewHeight);
	private:
		Font *TwentyPoint;
		Textures *DTextures;

		bool Timing;
		unsigned int Timer;
		std::queue<std::string> Text;
		void ConvertNewLines(std::string &T);
		//std::string Text;

		void BlueBackground(int X1, int X2, int Y1, int Y2);
		void RenderWindow(int X1, int X2, int Y1, int Y2);
		int WindowWidth, WindowHeight;
};

#endif
