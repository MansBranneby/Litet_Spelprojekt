#include "Billboard.h"

Billboard::Billboard()
{
	m_variant = 0;
	m_modelNr = 0;
	m_subModelNr = 0;

	m_UVincrement = { 0.0f, 0.0f, 0.0f };
	m_velocityUV = { 0.0f, 0.0f, 0.0f };
	m_blinkSpeed = 0.0f;
	m_blinkFactor = 0.0f;
	m_type = 0.0f;
	m_colourChangeSpeed = 0.0f;
	m_colourChangeFactor = 0.0f;
	m_colourDecider = 1.0f;
}

Billboard::Billboard(int variant, int modelNr, int subModelNr)
{
	m_variant = variant;
	m_modelNr = modelNr;
	m_subModelNr = subModelNr;

	m_UVincrement = { 0.0f, 0.0f, 0.0f };
	m_velocityUV = { 0.0f, 0.0f, 0.0f };
	m_blinkSpeed = 0.0f;
	m_blinkFactor = 0.0f;
	m_type = 0.0f;
	m_colourChangeSpeed = 0.0f;
	m_colourChangeFactor = 0.0f;
	m_colourDecider = 1.0f;
}

Billboard::Billboard(DirectX::XMVECTOR velocityUV, float blinkSpeed, float colourChangeSpeed, float type, int variant, int modelNr, int subModelNr)
{

	m_variant = variant;
	m_modelNr = modelNr;
	m_subModelNr = subModelNr;

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

int Billboard::getVariant() const
{
	return m_variant;
}

int Billboard::getModelNr() const
{
	return m_modelNr;
}

int Billboard::getSubModelNumber() const
{
	return m_subModelNr;
}

DirectX::XMVECTOR Billboard::getVelocityUV() const
{
	return m_velocityUV;
}

BillboardData Billboard::getBillboardData() const
{
	BillboardData data;
	data.velocityUV = m_UVincrement;
	data.blinkFactor= m_blinkFactor;
	data.colourChangeFactor = m_colourChangeFactor;
	data.colourDecider = m_colourDecider;
	data.type = m_type;

	return data;
}

void Billboard::update(float dt)
{
	if (m_type == 1)
	{
		moveUV(dt);
		changeColour(dt);
	}
}
