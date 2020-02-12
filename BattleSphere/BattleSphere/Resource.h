#pragma once

#include "DX.h"
#include "GameObject.h"

class Resource : public GameObject
{
private:
	int m_type;

public:
	Resource(int type = 0);
	~Resource();

	int getType();
};

