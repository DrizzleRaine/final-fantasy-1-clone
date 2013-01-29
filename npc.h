#ifndef NPC_H
#define NPC_H

#include "party.h"
#include "dialog.h"

#include <string>
#include <fstream>

class NPC {
	public:
		NPC();
		~NPC();

		// initialize the npcs
		void init(std::string mapName, int tSize, Party *p, Dialog *d);

		// interact with an npc
		void interact(int x, int y);

		// get npc at (x, y) if there is one (-1 if none)
		int exists(int x, int y);

		// move faster when player is 'pushing'
		void doubleSpeed(int npcID);
		void defaultSpeed();

		// npc operations
		void update(int **tiles);
		void render(int windowWidth, int windowHeight);
	private:
		// npcs have access to the party
		Party *party;

		// npcs can create dialogs
		Dialog *dialog;

		// NPC count
		int npcCount;

		struct NPCData {
			// which texture represents this npc
			int textureID; 

			//what npc will say to player
			std::string message;

			// dimensions of the sprite sheet
			float sheetWidth, sheetHeight;

			// does npc move/animate or just stand still
			bool moves, animates;

			// ticks since last step/animation
			unsigned int step, animation;

			// delay between steps/animation
			float stepDelay;

			// is npc moving faster
			bool doubleSpeed;

			// current position and direction
			int x, y, direction;

			// next direction to walk chosen
			bool newDirection;

			// is the newDirection blocked
			bool blocked;

			// how far into a step npc is
			float xStepDistance, yStepDistance;
		};

		// array of data represting npcs
		NPCData *npcs;

		// get npc at (x, y) if there is one (-1 if none)
		// npc at index exception will not be checked
		// negative value for exception mean no exceptions
		int exists(int x, int y, int exception);

		// size of current maps tiles
		int tileSize;

		// can npc move to (x, y) tile
		bool tileBlocked(int **tiles, int npc, int x, int y);

		// npc sprite sheets
		Textures textures;
};

#endif
