#pragma once

#include "DX.h"
#include "GameObject.h"

using namespace DirectX;

class Resource : public GameObject
{
private:
	int m_type;

public:
	Resource(int type = 0);
	~Resource();

	int getType();
};

