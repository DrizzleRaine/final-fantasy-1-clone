#ifndef MAP_H
#define MAP_H

#include "input.h"
#include "textures.h"

#include "npc.h"
#include "party.h"
#include "character.h"
#include "dialog.h"

#include <fstream>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

class Map {
	public:
		Map(Input *I, Character *C, PlayersParty *P);
		~Map();
		void LoadMap();
		void LoadTiles(const char *FileName);

		int Process();
		bool ProcessMovement();
		void RenderScene();
		void UpdateCoordinates(int NewWidth, int NewHeight);
	private:
		Input *MInput;
		Textures *CharTexture;
		Textures *MapTexture;
		DialogManager *Dialog;

		Character *Characters;
		PlayersParty *Party;
		int NPCCount;
		NPC *NPCs;
		void NPCAction(int XPos, int YPos);

		// TODO i should probably process moving in party.h
		enum Directions {North, East, South, West};
		Directions Direction; // which way character is facing/moving
		unsigned int Walking;

		int **Tiles; // 0 = blocked, >0 = ID of map to change to
					 // -1 = no battles, <-1 = enemy group
		int TileSize, XTiles, YTiles, MapWidth, MapHeight;
		bool BlockedTile(int XPos, int YPos);
		int WindowWidth, WindowHeight;

		bool Reverse;
		float WalkDelay;
		int SpriteX, SpriteY;

		void DrawMap();
		void DrawCharacter();
};

#endif
