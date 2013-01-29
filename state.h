#ifndef STATE_H
#define STATE_H

#include "input.h"
#include "party.h"

#include "statemanager.h"
class StateManager;

class State {
	public:
		State();
		virtual ~State();

		// initialize shared resources
		void init(Party *p, StateManager *sm);

		// linked list node operations
		State *getNext() { return next; }
		State *getPrev() { return prev; }
		void setNext(State *state) { next = state; }
		void setPrev(State *state) { prev = state; }

		// handle state operations
		void processState(SDL_Event &event);
		void updateState();
		void renderState(int width, int height);
	protected:
		// each state has its own input
		Input input;

		// every state shares the same party
		Party *party;

		// states can push/pop/change state stack
		StateManager *stateManager;

		// next and prev states
		State *next, *prev;

		// current width and height of window
		int windowWidth, windowHeight;
	private:
		// state opertions
		virtual void update() = 0;
		virtual void render() = 0;
};

#endif
