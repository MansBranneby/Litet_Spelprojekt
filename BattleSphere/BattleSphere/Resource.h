#pragma once

#include "DX.h"
#include "GameObject.h"

class Resource : public GameObject
{
private:
	int m_type;
	bool m_blocked;
	float m_time;
	bool m_ready;

public:
	Resource(int type = 0);
	~Resource();

	int getType();
	void setBlocked(bool state);
	bool isBlocked();

	void updateTime(float dt);
};

