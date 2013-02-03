#ifndef DIALOG_H
#define DIALOG_H

#include "gui.h"

#include <string>
#include <queue>

class Dialog : public GUI {
	public:
		Dialog();
		~Dialog();

		// create a dialog (optionally with a time limit)
		void push(std::string message, unsigned int duration = 0);

		// remove dialog
		void pop();

		// does a dialog exist
		bool exists();

		// operations
		void update();
		void render(int windowWidth, int windowHeight);

		// pause/unpause expiring dialogs
		void pause();
		void unpause();
	private:
		// messages to display
		std::queue<std::string> messages;

		// when the dialog will be destroyed
		// (if it was given a duration)
		unsigned int expiration;

		// pause time until expiration
		unsigned int pausedExpiration;

		// convert the string \n to the newline char
		void convertNewLines(std::string &msg);
};

#endif
