#pragma once

#include "StructsAndEnums.h"

class UI_Animation
{
private:
	//bool m_isReady;
	float m_timer;
public:
	UI_Animation();
	//~UI_Animation();

	bool animateElement(uiType type, float* posX, float* posY);
};

