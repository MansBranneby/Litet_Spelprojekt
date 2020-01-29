#include "Light.h"

Light::Light(XMVECTOR position, XMVECTOR colour)
{
	m_lightData.position = position;
	m_lightData.colour = colour;

	m_constantBuffer = new ConstantBuffer(&m_lightData, sizeof(m_lightData));
}

Light::~Light()
{
	delete m_constantBuffer;
}

ConstantBuffer* Light::getConstantuffer()
{
	return m_constantBuffer;
}
