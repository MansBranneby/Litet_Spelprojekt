#pragma once

#include "GameObject.h"

enum Animation
{
	e_Freeway
};

class DBGObj : public GameObject // Dynamic background object
{
private:
	Animation m_animation;
	float m_t;
	std::vector<XMVECTOR> m_weights;
	float m_speed;
	XMVECTOR m_prevPos;
public:
	DBGObj(Animation animation, std::vector<XMVECTOR> weight, float speed = 0.0f);
	~DBGObj();

	void animate(float dt);
	void bezier3(float dt);
};

