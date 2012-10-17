#ifndef MENU_JOB_H
#define MENU_JOB_H

#include "submenu.h"

class MenuJob : public SubMenu {
	public:
		MenuJob(Input *I, Textures *T, Font *F, Character *C, int *CPos);
		~MenuJob();

		int Process();
		void RenderScene();
	private:
};

#endif
