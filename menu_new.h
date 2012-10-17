#ifndef MENU_NEW_H
#define MENU_NEW_H

#include <string>
#include "submenu.h"

class MenuNew : public SubMenu {
	public:
		MenuNew(Input *I, Textures *T, Font *F, Character *C, int *CPos);
		~MenuNew();

		int Process();
		void RenderScene();
	private:
		bool StartConfirm; // start game confirmation
		unsigned int StartError; // forgot to name a character(s)
		std::string RandomNames[4][10];
};

#endif
