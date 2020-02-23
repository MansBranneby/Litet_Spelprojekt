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
	XMVECTOR m_originalScale;
	XMVECTOR m_smallScale;

	void setMaterial();

	// Spawning information
	bool m_spawning;
	bool m_heightReset;
	int m_spawnPosIndex;

	// Floating and spin animation
	float m_floatRadian;
	float m_spinDegree;

	void updateFloating(float dT);
	void updateSpinning(float dT, float modifier = 1.0f);
	void updateSpawningAnimation(float dT);

public:
	Resource(bool blocked, int spawnIndex, int type = 0, float scale = 1.0f);
	~Resource();

	int getType();
	void setBlocked(bool state);
	bool isBlocked();

	void updateTime(float dt);
	int getSpawnIndex() const;
};

