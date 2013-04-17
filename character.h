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
		enum Jobs {WARRIOR, THIEF, WHITE, BLACK, MONK, RED, JOBSCOUNT};

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

		// get character hp/mp fraction
		std::string getHPFraction();
		std::string getMPFraction();

		// get exp to next level
		int expToNext();

		// get spell id at given level and slot
		int getSpell(int level, int slot);

		// get equipment id at given slot
		int getEquip(int slot);

		// can character equip item type
		bool canEquip(int type);

		// set/remove equipment at given slot
		void setEquip(int slot, int id, int values[4]);
		void removeEquip(int slot);
	private:
		// character name and job
		std::string name;
		Jobs job;

		// 10 random names available for each job
		std::string randomNames[JOBSCOUNT][10];

		// character textures
		Textures textures;

		// character attributes
		int attributes[STATSCOUNT];

		// helper function to fill in attributes and equipment arrays
		// so they can be initialized easier
		void fillValues(int initAttrs[STATSCOUNT], int initEquip[5][5]);

		// string in format num/denom
		std::string getFraction(Stats num, Stats denom);

		// 5 equipment slots, store id and 4 stats
		int equipment[5][5];

		// IDs of spells this character knows
		int spells[8][3];

		// make a copy of this characacter object
		void makeCopy(const Character &source);
};

#endif
