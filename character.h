#ifndef CHARACTER_H
#define CHARACTER_H

#include "entity.h"
#include "textures.h"

class Character : public Entity {
	public:
		Character();
		~Character();

		// copy ctor and assignment operator
		Character(const Character &cSource);
		Character &operator=(const Character &rhs);

		// character jobs
		enum Jobs {WARRIOR, THIEF, WHITE, BLACK, MONK, RED, 
				   KNIGHT, NINJA, WHITEW, BLACKW, MASTER, REDW, JOBSCOUNT};

		// render at (x, y)
		void render(int x, int y);
		void render(Jobs job, int x, int y);

		// initialize stats
		void initStats();

		// character name
		void setRandomName();

		// character job
		void setJob(Jobs newJob);
		Jobs getJob();
		
		// get job name
		std::string getJobName();
		std::string getJobName(Jobs job);

		// get character attribute
		int getAttribute(Stats s);

		// add exp
		void addExp(int exp);

		// get character hp/mp fraction
		std::string getHPFraction();
		std::string getMPFraction();

		// get exp to next level
		int expToNext();

		// get spell id at given level and slot
		int getSpell(int level, int slot);

		// add given spell at first open slot in level
		bool addSpell(int id, int level);

		// remove spell at given level and slot
		bool removeSpell(int level, int slot);

		// does character know given spell
		bool hasSpell(int id);

		// get equipment id at given slot
		int getEquip(int slot);

		// can character equip item type
		bool canEquip(int type);

		// set/remove equipment at given slot
		void setEquip(int slot, int id, int values[4]);
		void removeEquip(int slot);

		// in battle step forward/backward
		// for deciding action/spell casting/using items
		void stepForward();
		void stepBackward();

		// is player stepped forward/taking step
		bool forward();
		bool stepping() { return step; }

		// execute turn, return damage done
		int act();
	private:
		// character job
		Jobs job;

		// 10 random names available for each job
		std::string randomNames[JOBSCOUNT][10];

		// texture ids and count
		enum TexInfo { CHARBATTLESPRITES, TEXTURECOUNT };
		Textures textures;

		// increase characters level by one
		void levelUp();

		// helper function to fill in attributes and equipment arrays
		// so they can be initialized easier
		void fillValues(int initAttrs[STATSCOUNT], int initEquip[5][5]);

		// string in format num/denom
		std::string getFraction(Stats num, Stats denom);

		// 5 equipment slots, store id and 4 stats
		int equipment[5][5];

		// IDs of spells this character knows
		int spells[8][3];

		// ticks since character started taking a step
		unsigned int step;		// 0 if not walking
		bool steppedForward;	// 1 if stepping/stepped forward

		// how long it takes to take a step
		float stepDelay;

		// make a copy of this characacter object
		void makeCopy(const Character &source);
};

#endif
