#ifndef INGAMESKILLS_H
#define INGAMESKILLS_H

#include "menu/MenuState.h"

class InGameSkills: public MenuState
{
public:
	InGameSkills();
	void LoadAssets();
	void Update();
	void Render();
	~InGameSkills();
};

#endif // INGAMESKILLS_H
