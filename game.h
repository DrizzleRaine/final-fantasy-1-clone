#ifndef GAME_H
#define GAME_H

#include "input.h"
#include "title.h"
#include "menu.h"
#include "map.h"

#include "party.h"
#include "character.h"

#include <SDL/SDL.h>

class Game {
	public:
		Game();
		~Game();
		void Update();
		void Process(SDL_Event &Event);
		void UpdateCoordinates(int NewWidth, int NewHeight);
	private:
		int WindowWidth, WindowHeight;
		Input *GameInput;

		PlayersParty *Party;
		Character Characters[4];
		
		Title *TitleState;
		Menu *MenuState;
		Map *MapState;
	
		// can each of the subsystems get there own input?
		// instead of passing them a pointer to the same one?	
		// and maybe there own window also?
		// since each subsystem controls its own rendering
		// why do i need it here?
		enum States {TitleS, MenuS, MapS, BattleS};
		States GameState;
};

#endif
