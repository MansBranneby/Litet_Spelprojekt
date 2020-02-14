#pragma once

#include "DX.h"
#include "GameObject.h"

#define FLOAT_INTENSITY 3.0f
#define FLOAT_RANGE 3.0f

class Resource : public GameObject
{
private:
	int m_type;
	bool m_blocked;
	float m_time;
	bool m_ready;

	// Floating animation
	float m_floatLevel;
	float m_floatDirection;

public:
	Resource(int type = 0);
	~Resource();

	int getType();
	void setBlocked(bool state);
	bool isBlocked();

	void updateTime(float dt);
};

