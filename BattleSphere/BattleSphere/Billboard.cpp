#include "Billboard.h"

Billboard::Billboard()
{
	m_UVincrement = { 0.0f, 0.0f, 0.0f };
	m_velocityUV = { 0.0f, 0.0f, 0.0f };
	m_blinkSpeed = 0.0f;
	m_blinkFactor = 0.0f;
	m_type = 0.0f;
	m_colourChangeSpeed = 0.0f;
	m_colourChangeFactor = 0.0f;
}

Billboard::Billboard(DirectX::XMVECTOR velocityUV, float blinkSpeed, float colourChangeSpeed, float type)
{
	m_UVincrement = { 0.0f, 0.0f, 0.0f };
	m_velocityUV = velocityUV;
	m_blinkSpeed = blinkSpeed;
	m_blinkFactor = 0.0f;
	m_type = type;
	m_colourChangeSpeed = colourChangeSpeed;
	m_colourChangeFactor = 0.0f;
	m_colourDecider = 1.0f;
}

void Billboard::moveUV(float dt)
{
	m_UVincrement += dt * m_velocityUV;
	
	if (m_blinkFactor < 0.0f)
	{
		m_blinkSpeed *= -1.0f;
		m_blinkFactor = 0.0f;
	}
	else if (m_blinkFactor > 1.0f)
	{
		m_blinkSpeed *= -1.0f;
		m_blinkFactor = 1.0f;
	}
		
	m_blinkFactor += dt * m_blinkSpeed;
}

void Billboard::changeColour(float dt)
{
	// add onto m_colourChangeFactor
	m_colourChangeFactor += dt * m_colourChangeSpeed;

	// Once colour change factor reaches 1.0f we reset it
	if (m_colourChangeFactor >= 1.0f)
	{
		m_colourChangeFactor = 0.0f;
		m_colourDecider *= -1.0f;
	}
}

DirectX::XMVECTOR Billboard::getVelocityUV() const
{
	return m_velocityUV;
}

TextureAnimationData Billboard::getTextureAnimationData() const
{
	TextureAnimationData data;
	data.velocityUV = m_UVincrement;
	data.blinkFactor= m_blinkFactor;
	data.colourChangeFactor = m_colourChangeFactor;
	data.colourDecider = m_colourDecider;
	data.type = m_type;

	return data;
}
