#pragma once

#include "DX.h"
#include "GameObject.h"

#define FLOAT_INTENSITY 6.0f // Default 6
#define FLOAT_HEIGHT 3.0f // Default 3
#define SPIN_INTENSITY 20.0f // Default 20
#define SMALL_SCALE 0.7f // Default 0.9
#define SPAWN_ANIMATION_TIME 3.5f
#define SPAWN_HEIGHT 170.0f
#define FINAL_HEIGHT 0.6f


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
	Resource(int spawnIndex, int type = 0, float scale = 1.0f);
	~Resource();

	int getType();
	void setBlocked(bool state);
	bool isBlocked();

	void updateTime(float dt);
	int getSpawnIndex() const;
};

