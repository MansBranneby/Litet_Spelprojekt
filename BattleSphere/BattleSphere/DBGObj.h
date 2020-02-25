#pragma once

#include "GameObject.h"
#include <math.h>

enum Animation
{
	e_Freeway
};

class DBGObj : public GameObject // Dynamic background object
{
private:
	Animation m_animation;
	float m_t;
	int m_activeCurve, m_nrOfCurves;
	std::vector<XMVECTOR> m_weights;
	std::vector<float> m_curveLenght;
	float m_speed;
	float m_driftFactor;
	XMVECTOR m_prevPos;

	void calcCurveLength(int nrOfsegments);
	XMVECTOR getCoord(int curveIndex, float t);

public:
	DBGObj(Animation animation, bool tokyoDirver = false, float speed = 0.0f);
	~DBGObj();

	void animate(float dt);
	void bezier3(float dt);
};

