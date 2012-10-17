#ifndef NPC_H
#define NPC_H

#include "party.h"
#include "dialog.h"
#include "textures.h"

#include <fstream>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

class NPC {
	public:
		NPC();
		~NPC();

		void Render();
		void Update();
		void Action(int CDir);
		bool Exists(int X, int Y);
		void Initialize(int ID, PlayersParty *P, DialogManager *D, int **TileData, int TSize);
		void UpdateCoordinates(int NewWidth, int NewHeight);
	private:
		Textures *NPCTexture;
		PlayersParty *Party;
		DialogManager *Dialog;
		char *Text;

		int **Tiles;
		int TileSize;

		bool Reverse;
		int SpriteX, SpriteY;
		float SheetWidth, SheetHeight;

		enum Directions {North, East, South, West};
		Directions Direction;
		bool NewDirectionChosen;

		bool Blocked;
		bool PositionBlocked(int PosX, int PosY);

		bool Moves;
		unsigned int Animation, AnimationDelay;
		unsigned int Moving, MoveDelay;
		int CurrentX, CurrentY; // is there any reason this should be tile based (i.e. divided by TileSize)
		float XMovement, YMovement;
		
		int WindowWidth, WindowHeight;
		float WindowTileWidth, WindowTileHeight;
};

#endif
