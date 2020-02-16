#include "Resource.h"

void Resource::updateFloating(float dT)
{
	// Update increment
	float increment = FLOAT_INTENSITY * dT;

	// Calculate relative movement
	float newRadian = m_floatRadian + increment;
	while (newRadian >= XM_2PI) // Make sure it stays within float bounds
		newRadian -= XM_2PI;
	float oldFloatLevel = FLOAT_HEIGHT * (1.0f - XMScalarCos(m_floatRadian)) / 2.0f;
	float newFloatLevel = FLOAT_HEIGHT * (1.0f - XMScalarCos(newRadian)) / 2.0f;
	float moveLevel = newFloatLevel - oldFloatLevel;
	m_floatRadian = newRadian;

	// Float up and down in Y-axis according to float level
	move(0.0f, moveLevel, 0.0f);
}

void Resource::updateSpinning(float dT, float modifier)
{
	// Increment
	float newSpinDegree = m_spinDegree + SPIN_INTENSITY * modifier * dT;
	
	// Calculate relative spin
	while (newSpinDegree >= 360.0f)
		newSpinDegree -= 360.0f;
	float relativeSpin = newSpinDegree - m_spinDegree;
	m_spinDegree = newSpinDegree;

	// Spin resource
	rotate(0.0f, 1.0f, 0.0f, -relativeSpin);
}

Resource::Resource(int type)
{
	m_type = type;
	m_blocked = false;
	m_ready = true;
	m_time = 0.0f;
	m_floatRadian = 0;
	m_spinDegree = 0;
	m_originalScale = XMVectorSet(0.16f, 0.08f, 0.08f, 1.0f);
	m_smallScale = XMVectorMultiply(m_originalScale, XMVectorSet(SMALL_SCALE, SMALL_SCALE, SMALL_SCALE, 1.0f));

	//setScale(0.4f, 0.4f, 0.4f);
	setScale(m_originalScale);
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
	{
		m_blocked = state;
		setScale(m_smallScale);
	}
	else
	{
		m_ready = state;
		setScale(m_originalScale);
	}
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
			m_floatRadian = 0;

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
		if (!m_blocked) // Move if not held
		{
			updateFloating(dt);
			updateSpinning(dt);
		}
		else
		{
			updateSpinning(dt, 10.0f);
		}
	}
}
