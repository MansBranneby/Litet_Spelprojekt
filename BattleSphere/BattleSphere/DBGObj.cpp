#include "DBGObj.h"

void DBGObj::calcCurveLength(int nrOfsegments)
{
	float dt = 1.0f / nrOfsegments;
	float t;
	std::vector<XMVECTOR> curveCoords;
	curveCoords.resize(m_nrOfCurves * nrOfsegments);

	for (int i = 0; i < m_nrOfCurves; i++) // Calculate length for all curves
	{
		m_curveLenght[i] = 0.0f; // initialize length
		curveCoords[0] = XMVectorSet(m_weights[(int)(i * 4)].m128_f32[0], m_weights[(int)(i * 4)].m128_f32[1], m_weights[(int)(i * 4)].m128_f32[2], 1.0f);
		for (int j = 1; j <= nrOfsegments; j++)
		{
			t = j * dt;
			curveCoords[j] = getCoord(i, t);

			m_curveLenght[i] += XMVector3Length(curveCoords[j] - curveCoords[j - 1]).m128_f32[0];
		}
	}
}

XMVECTOR DBGObj::getCoord(int curveIndex, float t)
{
	float mt = (1 - t);
	float mt2 = mt * mt;
	float mt3 = mt2 * mt;
	float t2 = t * t;
	float t3 = t2 * t;

	// Position
	float x = m_weights[0 + curveIndex * 4].m128_f32[0] * mt3 + 3.0f * m_weights[1 + curveIndex * 4].m128_f32[0] * mt2 * t + 3.0f * m_weights[2 + curveIndex * 4].m128_f32[0] * mt * t2 + m_weights[3 + curveIndex * 4].m128_f32[0] * t3;
	float y = m_weights[0 + curveIndex * 4].m128_f32[1] * mt3 + 3.0f * m_weights[1 + curveIndex * 4].m128_f32[1] * mt2 * t + 3.0f * m_weights[2 + curveIndex * 4].m128_f32[1] * mt * t2 + m_weights[3 + curveIndex * 4].m128_f32[1] * t3;
	float z = m_weights[0 + curveIndex * 4].m128_f32[2] * mt3 + 3.0f * m_weights[1 + curveIndex * 4].m128_f32[2] * mt2 * t + 3.0f * m_weights[2 + curveIndex * 4].m128_f32[2] * mt * t2 + m_weights[3 + curveIndex * 4].m128_f32[2] * t3;

	return XMVectorSet(x, y, z, 1.0f);;
}

DBGObj::DBGObj(Animation animation, bool tokyoDriver, float speed)
	: GameObject()
{
	m_isDrawn = false;
	m_t = 0.0f;
	m_speed = speed;
	m_animation = animation;
	if (tokyoDriver)
		m_driftFactor = 1.2f;
	else
		m_driftFactor = 1.0f;
	m_activeCurve = 0;

	switch (animation)
	{
	case e_FreewayL:
		m_nrOfCurves = 5;
		// Curve 1
		m_weights.push_back(XMVectorSet(250.0f, 18.0f, 5.0f, 1.0f));
		m_weights.push_back(XMVectorSet(220.0f, 18.0f, 5.0f, 1.0f));
		m_weights.push_back(XMVectorSet(190.0f, 18.0f, 5.0f, 1.0f));
		m_weights.push_back(XMVectorSet(162.0f, 18.0f, 5.0f, 1.0f));
		// Curve 2
		m_weights.push_back(XMVectorSet(162.0f, 18.0f, 5.0f, 1.0f));
		m_weights.push_back(XMVectorSet(139.0f, 16.0f, 5.0f, 1.0f));
		m_weights.push_back(XMVectorSet(116.0f, 1.0f, 5.0f, 1.0f));
		m_weights.push_back(XMVectorSet(93.0f, 1.0f, 5.0f, 1.0f));
		// Curve 3
		m_weights.push_back(XMVectorSet(93.0f, 1.0f, 5.0f, 1.0f));
		m_weights.push_back(XMVectorSet(65.0f, 1.0f, 5.0f, 1.0f));
		m_weights.push_back(XMVectorSet(53.0f, -5.0f, 30.0f, 1.0f));
		m_weights.push_back(XMVectorSet(53.0f, -8.0f, 49.0f, 1.0f));
		// Curve 4
		m_weights.push_back(XMVectorSet(53.0f, -8.0f, 49.0f, 1.0f));
		m_weights.push_back(XMVectorSet(60.0f, 2.0f, 100.0f, 1.0f));
		m_weights.push_back(XMVectorSet(10.0f, 2.0f, 170.0f, 1.0f));
		m_weights.push_back(XMVectorSet(-31.0f, 2.0f, 165.0f, 1.0f));
		// Curve 5
		m_weights.push_back(XMVectorSet(-31.0f, 2.0f, 165.0f, 1.0f));
		m_weights.push_back(XMVectorSet(-98.0f, 2.0f, 160.0f, 1.0f));
		m_weights.push_back(XMVectorSet(-165.0f, 2.0f, 164.0f, 1.0f));
		m_weights.push_back(XMVectorSet(-233.0f, 2.0f, 164.0f, 1.0f));
		break;
	default:
	case e_FreewayR:
		m_nrOfCurves = 5;
		// Curve 5
		m_weights.push_back(XMVectorSet(-233.0f, 2.0f, 174.0f, 1.0f));
		m_weights.push_back(XMVectorSet(-165.0f, 2.0f, 174.0f, 1.0f));
		m_weights.push_back(XMVectorSet(-98.0f, 2.0f, 170.0f, 1.0f));
		m_weights.push_back(XMVectorSet(-31.0f, 2.0f, 175.0f, 1.0f));
		// Curve 4
		m_weights.push_back(XMVectorSet(-31.0f, 2.0f, 175.0f, 1.0f));
		m_weights.push_back(XMVectorSet(40.0f, 2.0f, 170.0f, 1.0f));
		m_weights.push_back(XMVectorSet(65.0f, 2.0f, 100.0f, 1.0f));
		m_weights.push_back(XMVectorSet(65.0f, -8.0f, 49.0f, 1.0f));
		// Curve 3
		m_weights.push_back(XMVectorSet(65.0f, -8.0f, 49.0f, 1.0f));
		m_weights.push_back(XMVectorSet(65.0f, -5.0f, 30.0f, 1.0f));
		m_weights.push_back(XMVectorSet(65.0f, 1.0f, 16.0f, 1.0f));
		m_weights.push_back(XMVectorSet(93.0f, 1.0f, 16.0f, 1.0f));
		// Curve 2
		m_weights.push_back(XMVectorSet(93.0f, 1.0f, 16.0f, 1.0f));
		m_weights.push_back(XMVectorSet(116.0f, 1.0f, 16.0f, 1.0f));
		m_weights.push_back(XMVectorSet(139.0f, 16.0f, 16.0f, 1.0f));
		m_weights.push_back(XMVectorSet(162.0f, 18.0f, 16.0f, 1.0f));
		// Curve 1
		m_weights.push_back(XMVectorSet(162.0f, 18.0f, 16.0f, 1.0f));
		m_weights.push_back(XMVectorSet(190.0f, 18.0f, 16.0f, 1.0f));
		m_weights.push_back(XMVectorSet(220.0f, 18.0f, 16.0f, 1.0f));
		m_weights.push_back(XMVectorSet(250.0f, 18.0f, 16.0f, 1.0f));
		break;
		m_nrOfCurves = 0;
		break;
	}

	m_curveLenght.resize(m_nrOfCurves);
	calcCurveLength(10);
	m_lightIndex = Lights::getInstance()->addSpotLight(m_weights[0].m128_f32[0], m_weights[0].m128_f32[1], m_weights[0].m128_f32[2], 20.0f, 0.0f, 1.0f, 0.0f, 0.15f, 0.97f, 1.0f, 45.0f, 50.0f);
	//m_lightIndex = Lights::getInstance()->addSpotLight(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 45.0f, 0.0f);
	//m_lightIndex = Lights::getInstance()->addPointLight(0, 0, 0, 10, 1, 1, 1, 10);
	//m_lightIndex = Lights::getInstance()->addDirectionalLight(0, -1, 0, 1, 1, 1, 1);

}

DBGObj::~DBGObj()
{
}

bool DBGObj::animate(float dt)
{
	bool hideCar = false;
	switch (m_animation)
	{
	case e_FreewayL:
		hideCar = bezier3(dt);
		break;
	case e_FreewayR:
		hideCar = bezier3(dt);
		break;
	default:
		break;
	}
	return hideCar;
}

bool DBGObj::bezier3(float dt)
{
	bool hideCar = false;
	m_prevPos = getPosition(); // Previous position for rotation calculation
	if (m_t == 0.0f)
	{
		m_prevPos = m_weights[0];
	}

	m_t += dt * m_speed / m_curveLenght[m_activeCurve];
	if (m_t >= 1.0f) // Start on new curve or restart animation
	{
		if (m_nrOfCurves - 1 > m_activeCurve)
			m_activeCurve++;
		else
		{
			m_activeCurve = 0;
			hideCar = true;
			m_t = 0.0f;
			return hideCar;
		}
		m_t = 0.0f;
	}
	float mt = (1 - m_t);
	float mt2 = mt * mt;
	float mt3 = mt2 * mt;
	float t = m_t;
	float t2 = t * t;
	float t3 = t2 * t;

	// Position
	float x = m_weights[(int)(0 + m_activeCurve * 4)].m128_f32[0] * mt3 + 3.0f * m_weights[(int)(1 + m_activeCurve * 4)].m128_f32[0] * mt2 * t + 3.0f * m_weights[(int)(2 + m_activeCurve * 4)].m128_f32[0] * mt * t2 + m_weights[(int)(3 + m_activeCurve * 4)].m128_f32[0] * t3;
	float y = m_weights[(int)(0 + m_activeCurve * 4)].m128_f32[1] * mt3 + 3.0f * m_weights[(int)(1 + m_activeCurve * 4)].m128_f32[1] * mt2 * t + 3.0f * m_weights[(int)(2 + m_activeCurve * 4)].m128_f32[1] * mt * t2 + m_weights[(int)(3 + m_activeCurve * 4)].m128_f32[1] * t3;
	float z = m_weights[(int)(0 + m_activeCurve * 4)].m128_f32[2] * mt3 + 3.0f * m_weights[(int)(1 + m_activeCurve * 4)].m128_f32[2] * mt2 * t + 3.0f * m_weights[(int)(2 + m_activeCurve * 4)].m128_f32[2] * mt * t2 + m_weights[(int)(3 + m_activeCurve * 4)].m128_f32[2] * t3;

	setPosition(XMVectorSet(x, y, z, 1.0f));

	// Rotatation
	XMVECTOR translation = getPosition() - m_prevPos;

	switch (m_animation)
	{
	case e_FreewayL:
		// rotation in y (around x)
		setRotation(1.0f, 0.0f, 0.0f, (acos(XMVector3Dot(translation, XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f)).m128_f32[0] / XMVector3Length(translation).m128_f32[0])) * (180.0f / XM_PI) - 90.0f);
		// rotation in x (around y)
		rotate(0.0f, 1.0f, 0.0f, -(acos(XMVector3Dot(translation, XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f)).m128_f32[0] / XMVector3Length(translation).m128_f32[0])) * (180.0f / XM_PI) * m_driftFactor);
		break;
	case e_FreewayR:
		// rotation in y (around x)
		setRotation(1.0f, 0.0f, 0.0f, (acos(XMVector3Dot(translation, XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f)).m128_f32[0] / XMVector3Length(translation).m128_f32[0])) * (180.0f / XM_PI) - 90.0f);
		// rotation in x (around y)
		rotate(0.0f, 1.0f, 0.0f, (acos(XMVector3Dot(translation, XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f)).m128_f32[0] / XMVector3Length(translation).m128_f32[0])) * (180.0f / XM_PI) * m_driftFactor);
		break;
	default:
		break;
	}

	translation = XMVector3Normalize(translation);
	Lights::getInstance()->setPosition(m_lightIndex, x + translation.m128_f32[0] * 5.0f, y + translation.m128_f32[1] * 5.0f + 2.0f, z + translation.m128_f32[2] * 5.0f);
	Lights::getInstance()->setDirection(m_lightIndex, translation.m128_f32[0], translation.m128_f32[1] -0.5f, translation.m128_f32[2]);

	return hideCar;
}

void DBGObj::setDrawn(bool isDrawn)
{
	m_isDrawn = isDrawn;
}

bool DBGObj::isDrawn()
{
	return m_isDrawn;
}
