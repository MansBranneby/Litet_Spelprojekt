#include "DBGObj.h"

DBGObj::DBGObj(Animation animation, std::vector<XMVECTOR> weights, float speed)
	: GameObject()
{
	m_t = 0.0f;
	m_weights = weights;
	m_speed = speed;


}

DBGObj::~DBGObj()
{
}

void DBGObj::animate(float dt)
{
	switch (m_animation)
	{
	case 0:
		bezier3(dt);
		break;
	default:
		break;
	}
}

void DBGObj::bezier3(float dt)
{
	m_t += dt * m_speed;
	if (m_t > 1.0f)
	m_t = 0.0f;
	float mt = (1 - m_t);
	float mt2 = mt * mt;
	float mt3 = mt2 * mt;
	float t = m_t;
	float t2 = t * t;
	float t3 = t2 * t;
	float x = m_weights[0].m128_f32[0] * mt3 + 3.0f * m_weights[1].m128_f32[0] * mt2 * t + m_weights[2].m128_f32[0] * mt * t2 + m_weights[3].m128_f32[0] * t3;
	float y = m_weights[0].m128_f32[1] * mt3 + 3.0f * m_weights[1].m128_f32[1] * mt2 * t + m_weights[2].m128_f32[1] * mt * t2 + m_weights[3].m128_f32[1] * t3;
	float z = m_weights[0].m128_f32[2] * mt3 + 3.0f * m_weights[1].m128_f32[2] * mt2 * t + m_weights[2].m128_f32[2] * mt * t2 + m_weights[3].m128_f32[2] * t3;

	setPosition(XMVectorSet(x, y, z, 1.0f));
}
