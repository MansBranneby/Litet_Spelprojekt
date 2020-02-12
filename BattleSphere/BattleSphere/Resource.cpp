#include "Resource.h"

Resource::Resource(int type)
{
	m_type = type;
	m_blocked = false;
	m_ready = true;
	m_time = 0.0f;
	setScale(0.4f, 0.4f, 0.4f);
	if (type == 1)
		m_material.diffuse = XMVectorSet(1, 0, 0, -1);
	else if (type == 2)
		m_material.diffuse = XMVectorSet(1, 1, 0, -1);
	else if (type == 3)
		m_material.diffuse = XMVectorSet(1, 0, 1, -1);
	else
		m_material.diffuse = XMVectorSet(0, 1, 1, -1);

	m_material.emission = m_material.diffuse;
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
		m_material.diffuse = XMVector3Normalize(m_material.diffuse) * (0.4f + fmod(m_time, 0.5f));
		XMVectorSetW(m_material.diffuse, -1);
		m_material.emission = m_material.diffuse;
		if (m_time > 3.0f)
		{
			m_time = 0.0f;
			m_ready = true;
			m_blocked = false;
			if (m_type == 1)
				m_material.diffuse = XMVectorSet(1, 0, 0, -1);
			else if (m_type == 2)
				m_material.diffuse = XMVectorSet(1, 1, 0, -1);
			else if (m_type == 3)
				m_material.diffuse = XMVectorSet(1, 0, 1, -1);
			else
				m_material.diffuse = XMVectorSet(0, 1, 1, -1);
			m_material.emission = m_material.diffuse;
		}
	}
}
