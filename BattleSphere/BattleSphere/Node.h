#pragma once
#include "GameObject.h"

class Node : public GameObject
{
private:
	int m_type;

public:
	Node(int type = 0);
	~Node();

	int getType();
};

