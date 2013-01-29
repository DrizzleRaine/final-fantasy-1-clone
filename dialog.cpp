#include "dialog.h"

Dialog::Dialog() {
}

Dialog::~Dialog() {
}

void Dialog::push(std::string message, unsigned int duration) {
	// \n\n is what i use for new dialog delimeter
	size_t doubleNewLine = message.find("\\n\\n");

	// push each dialog in to message queue
	while (doubleNewLine != std::string::npos) {
		// get everything until the double new line
		std::string msg = message.substr(0, doubleNewLine);

		// remove everything, including the delimeter from original
		message = message.substr(doubleNewLine + 4);

		// convert \n to newline
		convertNewLines(msg);

		// push the message
		messages.push(msg);

		// find the next
		doubleNewLine = message.find("\\n\\n");
	}
	convertNewLines(message);	// convert \n to newline
	messages.push(message);		// push the final (or only) dialog

	if (duration) {	// dialog with time limit
		expiration = SDL_GetTicks() + duration;
	} else {		// dialog without time limit
		expiration = 0;
	}
}

void Dialog::pop() {
	messages.pop();
}

bool Dialog::exists() {
	return messages.size() > 0;
}

void Dialog::update() {
	if (messages.empty()) {
		return;
	}

	if (expiration && expiration < SDL_GetTicks()) {
		messages.pop();
	}
}

void Dialog::render(int windowWidth, int windowHeight) {
	if (messages.empty()) {
		return;
	}

	if (expiration) {	// timed windows are thinner
		window(-windowWidth + 20, windowWidth - 20, 
				windowHeight - 50, windowHeight - 200);

		// find the width of the next message to center it
		SDL_Rect r = {0, 0, 0, 0};
		twenty.textSize(messages.front().c_str(), &r);
		const int CENTERED = -(r.w / 2);

		// draw the next message centered
		twenty.drawText(CENTERED, windowHeight - 155, messages.front().c_str());
	} else {			// npc dialog windows are larger
		window(-windowWidth + 20, windowWidth - 20, 
				windowHeight, windowHeight - 275);

		// draw the next message left aligned
		twenty.drawText(-windowWidth + 70, windowHeight - 100, 
				messages.front().c_str());
	}
}

void Dialog::convertNewLines(std::string &msg) {
	// strings read from files read \n as two seperate chars
	// rather than as the newline character
	size_t newline = msg.find("\\n");
	while (newline != std::string::npos) {
		msg.replace(newline, 2, "\n");	// convert \n to newline
		newline = msg.find("\\n");		// find the next
	}
}
