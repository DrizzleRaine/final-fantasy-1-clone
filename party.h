#ifndef PARTY_H
#define PARTY_H

#include "textures.h"
#include "character.h"

class Party {
	public:
		Party();
		~Party();

		// the characters in the party
		enum Characters {FIRST, SECOND, THIRD, FOURTH, SIZE};

		// directions party can face
		enum Directions {NORTH, EAST, SOUTH, WEST, DIRECTIONS};

		// the currently active character
		void setActive(int character);
		Characters getActive() { return active; }

		// pause party
		void pause() { paused = 1; }
		void unpause() { paused = 0; }
		bool isPaused() { return paused; }

		// party position
		int getX() { return x; }
		int getY() { return y; }
		void setX(int newX);
		void setY(int newY);

		// get exact position, even during step
		float getX(int tileSize);
		float getY(int tileSize);

		// direction the party is facing
		void setDirection(int dir);
		Directions getDirection() { return direction; }

		// step in current direction for stepDelay ticks
		void startStep() { step = SDL_GetTicks(); }
		bool stepping() { return step; }
		void setStepDelay(float delay) { stepDelay = delay; }

		// current map
		int getMap() { return map; }
		void setMap(int mapID) { map = mapID; }
		std::string getRegion() { return region; }
		void setRegion(std::string name) { region = name; }

		// render the party, character, or given job
		void renderParty(int width, int height);		// first char, cur pos
		void render(Characters c, int x, int y);		// character c at (x, y)
		void render(Character::Jobs job, int x, int y);	// given job at (x, y)

		// swap character c1 and c2 positions
		void swapCharacters(int c1, int c2);

		// change the name of the active character to newName
		void setName(std::string newName);
		void randomName();

		// get characters name
		std::string getName();							// active character's name
		std::string getName(Characters c);				// character c's name

		// change the job of the active character to newJob
		void setJob(int newJob);

		// get string of job name
		std::string getJob(Characters c);				// character c's job
		std::string getJob(Character::Jobs job);		// given job

		// has character c been given a name?
		bool emptyName(Characters c);

		// party created, initialize it
		void initialize();

		// get character attribute
		int getAttribute(Characters c, Character::Stats s);

		// get string of hp/hpmax or mp/mpmax
		std::string getHPFraction(Characters c);
		std::string getMPFraction(Characters c);
		// future: dont need setters for hpmax/mpmax
		// just have a level up function?

		// get spell character c has at level and slot
		int getSpell(Characters c, int level, int slot);

		// get equipment character c has in given slot
		int getEquip(Characters c, int lot);
	
		// how much gil party has	
		unsigned int getGil() { return gil; }
		void addGil(int g) { gil += g; }

		// party inventory
		static const int INVENTORY_SIZE = 256;
		int getItem(int pos) { return items[pos]; }
		int getItemCount(int id);
		void addItem(int id, int amount = 1);
		void swapItems(int pos1, int pos2);
		void sortItems();

		// get string representing game time, HH:MM
		std::string getTime();
	private:
		// characters in the party
		Character characters[SIZE];

		// texture ids and count
		enum TexInfo {CHARMAPSPRITES, TEXTURECOUNT};
		Textures textures;

		// the active character
		Characters active;

		// map party is on at (x, y)
		int map, x, y;
		std::string region;

		// which direction party is facing
		Directions direction;

		// pause the party
		bool paused;

		// ticks since character started taking a step
		unsigned int step;	// 0 if not walking

		// how long it takes to take a step
		float stepDelay;

		// finish taking a step of distance tileSize
		void stopStep(int tileSize);

		// partys gil
		unsigned int gil;

		// party inventory
		int items[INVENTORY_SIZE];

		// item counts in seperate array
		// to quickly check if party has a specific item
		int itemCount[INVENTORY_SIZE];

		// ticks played, ticks at last update
		unsigned int ticksPlayed, ticksUpdate;
};

#endif
