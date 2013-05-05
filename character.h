#ifndef CHARACTER_H
#define CHARACTER_H

#include "textures.h"

class Character {
	public:
		Character();
		~Character();

		// copy ctor and assignment operator
		Character(const Character &cSource);
		Character &operator=(const Character &rhs);

		// character jobs
		enum Jobs {WARRIOR, THIEF, WHITE, BLACK, MONK, RED, 
				   KNIGHT, NINJA, WHITEW, BLACKW, MASTER, REDW, JOBSCOUNT};

		// character stats
		enum Stats {LEVEL, EXP, MAGLEVEL, HP, MP, HPMAX, MPMAX, 
			STR, AGL, INT, STA, LCK, ATK, ACC, DEF, EVA, STATSCOUNT};

		// render at (x, y)
		void render(int x, int y);
		void render(Jobs job, int x, int y);

		// initialize stats
		void initStats();

		// character name
		void setName(std::string newName);
		std::string getName();
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

		// heal amount hp/mp, return amount healed
		int addHP(int amount);
		int addMP(int amount);

		// check if character has status
		bool hasStatus(unsigned int status);

		// set/remove status, return true if successful
		bool setStatus(unsigned int status);
		bool removeStatus(unsigned int status);

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

		// TODO, enemies and characters both inherit
		// from parent entity class?
		// different types of actions entity can perform
		enum Actions {NONE, ATTACK, SPELL, ITEM, FLEE, COUNT};
		struct Turn {
			// action to take
			Actions action;

			// id of action (spellID/itemID)
			int actionID;

			// action target
			int target;
		};
	
		// set/get entity turn	
		void setTurn(Turn t) { turn = t; }
		Turn getTurn() { return turn; }

		// in battle step forward/backward
		// for deciding action/spell casting/using items
		void stepForward();
		void stepBackward();

		// is player stepped forward/taking step
		bool forward();
		bool stepping() { return step; }
	private:
		// character name and job
		std::string name;
		Jobs job;

		// 10 random names available for each job
		std::string randomNames[JOBSCOUNT][10];

		// texture ids and count
		enum TexInfo { CHARBATTLESPRITES, TEXTURECOUNT };
		Textures textures;

		// character attributes
		int attributes[STATSCOUNT];

		// increase characters level by one
		void levelUp();

		// status effect bits
		// 1 - KO, 2 - poison, 3 - darkness
		// 4 - silence, 5 - stone
		int statusBits;

		// helper function to fill in attributes and equipment arrays
		// so they can be initialized easier
		void fillValues(int initAttrs[STATSCOUNT], int initEquip[5][5]);

		// string in format num/denom
		std::string getFraction(Stats num, Stats denom);

		// 5 equipment slots, store id and 4 stats
		int equipment[5][5];

		// IDs of spells this character knows
		int spells[8][3];

		// entity's turn
		Turn turn;

		// ticks since character started taking a step
		unsigned int step;		// 0 if not walking
		bool steppedForward;	// 1 if stepping/stepped forward

		// how long it takes to take a step
		float stepDelay;

		// make a copy of this characacter object
		void makeCopy(const Character &source);
};

#endif
