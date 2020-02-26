#include "Billboard.h"

Billboard::Billboard()
{
	m_UVincrement = { 0.0f, 0.0f, 0.0f };
	m_velocityUV = { 1.0f, 0.0f, 0.0f };
	m_blinkSpeed = 0.0f;
	m_blinkValue = 0.0f;
	m_type = 0.0f;
}

Billboard::Billboard(DirectX::XMVECTOR velocityUV, float blinkSpeed, float type)
{
	m_UVincrement = { 0.0f, 0.0f, 0.0f };
	m_velocityUV = velocityUV;
	m_blinkSpeed = blinkSpeed;
	m_blinkValue = 0.0f;
	m_type = type;
}

void Billboard::moveUV(float dt)
{
	m_UVincrement += dt * m_velocityUV;
	
	if (m_blinkValue < 0.0f)
	{
		m_blinkSpeed *= -1.0f;
		m_blinkValue = 0.0f;
	}
	else if (m_blinkValue > 1.0f)
	{
		m_blinkSpeed *= -1.0f;
		m_blinkValue = 1.0f;
	}
		
	m_blinkValue += dt * m_blinkSpeed;
}

DirectX::XMVECTOR Billboard::getVelocityUV() const
{
	return m_velocityUV;
}

TextureAnimationData Billboard::getTextureAnimationData() const
{
	TextureAnimationData data;
	data.velocityUV = m_UVincrement;
	data.blinkValue = m_blinkValue;
	data.type = m_type;

	return data;
}
