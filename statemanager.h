#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include "state.h"
#include "party.h"

class State;

class StateManager {
	public:
		StateManager();
		~StateManager();

		// push, pop or change states
		void pushState(State *state);
		void popState();
		void changeState(State *state);
		void popAll();

		// state actions
		void process(SDL_Event &event);
		void update();
		void render(int width, int height);
	private:
		// players party, shared across states
		Party party;

		// head and tail of linked list of states
		// the linked list is treated like a stack
		State *head, *tail;
};

#endif
