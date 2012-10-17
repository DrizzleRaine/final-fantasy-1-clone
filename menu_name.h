#ifndef MENU_NAME_H
#define MENU_NAME_H

#include <string>
#include "submenu.h"

class MenuName : public SubMenu {
	public:
		MenuName(Input *I, Textures *T, Font *F, Character *C, int *CPos);
		~MenuName();

		int Process();
		void RenderScene();
	private:
		int NameLength;
		std::string NewName;
		bool NameIsEmpty();
};

#endif
