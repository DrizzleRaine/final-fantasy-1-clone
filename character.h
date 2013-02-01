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
		enum Stats {LEVEL, HP, HPMAX, MP, MPMAX, STATSCOUNT};

		// render at (x, y)
		void render(int x, int y);
		void render(Jobs job, int x, int y);

		// initialize stats
		void initStats();

		// character name
		void setName(std::string newName);
		std::string getName();

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
	private:
		// character name and job
		std::string name;
		Jobs job;

		// character textures
		Textures textures;

		// character attributes
		int attributes[STATSCOUNT];

		// string in format num/denom
		std::string getFraction(Stats num, Stats denom);

		// make a copy of this characacter object
		void makeCopy(const Character &source);
};

#endif
