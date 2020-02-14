#include "Resource.h"

Resource::Resource(int type)
{
	m_type = type;
	m_blocked = false;
	m_ready = true;
	m_time = 0.0f;
	m_floatLevel = -FLOAT_RANGE/2.0f;
	m_floatDirection = 1.0f;
	//setScale(0.4f, 0.4f, 0.4f);
	setScale(0.16f, 0.08f, 0.08f);
	setRotation(XMVectorSet(1.0, 0.0, 0.0, 90));

	m_material.diffuse = XMVectorSet(0, 0, 0, -1);
	if (type == 1)
		m_material.emission = XMVectorSet(1, 0, 0, -1);
	else if (type == 2)
		m_material.emission = XMVectorSet(1, 1, 0, -1);
	else if (type == 3)
		m_material.emission = XMVectorSet(1, 0, 1, -1);
	else
		m_material.emission = XMVectorSet(0, 1, 1, -1);

}

Resource::~Resource()
{
}

int Resource::getType()
{
	return m_type;
}

void Resource::setBlocked(bool state)
{
	if (state)
		m_blocked = state;
	else
		m_ready = state;
}

bool Resource::isBlocked()
{
	return m_blocked;
}

void Resource::updateTime(float dt)
{
	if (!m_ready)
	{
		m_time += dt;
		m_material.emission = XMVector3Normalize(m_material.emission) * (float)(fmod(m_time, 0.5f));
		XMVectorSetW(m_material.emission, -1);
		if (m_time > 3.0f)
		{
			m_time = 0.0f;
			m_ready = true;
			m_blocked = false;

			if (m_type == 1)
				m_material.emission = XMVectorSet(1, 0, 0, -1);
			else if (m_type == 2)
				m_material.emission = XMVectorSet(1, 1, 0, -1);
			else if (m_type == 3)
				m_material.emission = XMVectorSet(1, 0, 1, -1);
			else
				m_material.emission = XMVectorSet(0, 1, 1, -1);
		}
	}

	else
	{
		// Update increment
		float increment = m_floatDirection * FLOAT_INTENSITY * dt;

		// Update float level
		m_floatLevel += increment;

		// Change direction if outside float range
		if (m_floatLevel > FLOAT_RANGE / 2.0f)
			m_floatDirection = -1.0f;
		if (m_floatLevel < -FLOAT_RANGE / 2.0f)
			m_floatDirection = 1.0f;

		// Float up and down in Y-axis according to float level
		move(0.0f, increment, 0.0f);
	}
}
