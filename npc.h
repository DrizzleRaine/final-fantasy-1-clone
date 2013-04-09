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
		void init(std::string mapFile, int tSize, Party *p, Dialog *d);

		// interact with an npc
		void interact(int npcID);

		// get npc at (x, y) if there is one (-1 if none)
		int exists(int x, int y);

		// move faster when player is 'pushing'
		void doubleSpeed(int npcID);
		void defaultSpeed();

		// npc operations
		void update(int **tiles);
		void render(int windowWidth, int windowHeight);
	
		// pause/unpause npc steps/animation	
		void pause();
		void unpause();

		// possible shops types for npcs
		enum ShopTypes {NONE = 0, ITEM, WEAPON, ARMOR, WHITE, BLACK, COUNT};

		// shop type and name accessors
		ShopTypes getShopType(int npcID);
		std::string getShopName(int npcID);
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

			// what npc will say to player
			// if npc is a shop, this contains shop name
			std::string message;

			// dimensions of the sprite sheet
			float sheetWidth, sheetHeight;

			// does npc move/animate or just stand still
			bool moves, animates;

			// ticks since last step/animation
			unsigned int step, animation;

			// elapsed ticks when npc was paused
			unsigned int stepPause, animationPause;

			// delay between steps/animation
			float stepDelay;

			// is npc moving faster
			bool doubleSpeed;

			// current position and direction
			int x, y, direction;

			// npc shop type
			ShopTypes shopType;

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
