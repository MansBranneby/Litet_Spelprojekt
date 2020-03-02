#include "Billboard.h"

Billboard::Billboard()
{
	m_variant = 0;
	m_modelNr = 0;
	m_subModelNr = 0;

	m_state = BillboardState::e_none;
	m_UVincrement = { 0.0f, 0.0f, 0.0f };
	m_velocityUV = { 0.0f, 0.0f, 0.0f };
	m_flashSpeed = 0.0f;
	m_flashFactor = 0.0f;
	m_colourChangeSpeed = 0.0f;
	m_colourChangeFactor = 0.0f;
	m_colourDecider = 1.0f;
	m_colourA = DirectX::XMVectorZero();
	m_colourB = DirectX::XMVectorZero();
}

Billboard::Billboard(int variant, int modelNr, int subModelNr)
{
	m_variant = variant;
	m_modelNr = modelNr;
	m_subModelNr = subModelNr;

	m_state = BillboardState::e_none;
	m_UVincrement = { 0.0f, 0.0f, 0.0f };
	m_velocityUV = { -0.01f, 0.0f, 0.0f };
	m_flashSpeed = 0.0f;
	m_flashFactor = 0.0f;
	m_colourChangeSpeed = 0.0f;
	m_colourChangeFactor = 0.0f;
	m_colourDecider = 1.0f;
	m_colourA = DirectX::XMVectorZero();
	m_colourB = DirectX::XMVectorZero();
}

void Billboard::setState(BillboardState state)
{
	m_state = state;
}

void Billboard::setFlashState(float flashSpeed)
{
	m_state = BillboardState::e_flashing;
	m_flashSpeed = flashSpeed;
}

void Billboard::setInterpolateState(float colourChangeSpeed, DirectX::XMVECTOR colourA, DirectX::XMVECTOR colourB)
{
	m_state = BillboardState::e_interpolating;
	m_colourChangeSpeed = colourChangeSpeed;
	m_colourA = colourA;
	m_colourB = colourB;
}

void Billboard::setTranslateState(DirectX::XMVECTOR velocityUV)
{
	m_velocityUV = velocityUV;
}

void Billboard::setFlashInterpolateState(float flashSpeed, float colourChangeSpeed, DirectX::XMVECTOR colourA, DirectX::XMVECTOR colourB)
{
	m_state = BillboardState::e_flashing_interpolating;
	m_flashSpeed = flashSpeed;
	m_colourChangeSpeed = colourChangeSpeed;
	m_colourA = colourA;
	m_colourB = colourB;
}

void Billboard::setFlashTranslateState(float flashSpeed, DirectX::XMVECTOR velocityUV)
{
	m_state = BillboardState::e_flashing_translating;
	m_flashSpeed = flashSpeed;
	m_velocityUV = velocityUV;
}

void Billboard::setInterpolateTranslateState(float colourChangeSpeed, DirectX::XMVECTOR colourA, DirectX::XMVECTOR colourB, DirectX::XMVECTOR velocityUV)
{
	m_state = BillboardState::e_interpolating_translating;
	m_colourChangeSpeed = colourChangeSpeed;
	m_colourA = colourA;
	m_colourB = colourB;
	m_velocityUV = velocityUV;
}

void Billboard::setAllStates(float flashSpeed, float colourChangeSpeed, DirectX::XMVECTOR colourA, DirectX::XMVECTOR colourB, DirectX::XMVECTOR velocityUV)
{
	m_state = BillboardState::e_all;
	m_flashSpeed = flashSpeed;
	m_colourChangeSpeed = colourChangeSpeed;
	m_colourA = colourA;
	m_colourB = colourB;
	m_velocityUV = velocityUV;
}

void Billboard::setFlashSpeed(float flashSpeed)
{
	m_flashSpeed = flashSpeed;
}

void Billboard::setColourChangeSpeed(float colourChangeSpeed)
{
	m_colourChangeSpeed = colourChangeSpeed;
}

void Billboard::setVelocityUV(DirectX::XMVECTOR velocityUV)
{
	m_velocityUV = velocityUV;
}

void Billboard::flashColour(float dt)
{
	if (m_flashFactor < 0.0f)
	{
		m_flashSpeed *= -1.0f; // Make flashSpeed positive
		m_flashFactor = 0.0f;  // Clamp to 0
	}
	else if (m_flashFactor > 1.0f)
	{
		m_flashSpeed *= -1.0f; // Make flashSpeed negative
		m_flashFactor = 1.0f;  // Clamp to 1
	}

	// increase or decrease flashFactor depending on the sign (+ or -) of flashSpeed
	m_flashFactor += dt * m_flashSpeed;
}

void Billboard::interpolateColours(float dt)
{
	// Once colour change factor reaches 1.0f we reset it
	if (m_colourChangeFactor > 1.0f)
	{
		m_colourDecider *= -1.0f; // Flip the sign of m_coulourDecider to switch colour in pixelshader
		m_colourChangeFactor = 0.0f; // Clamp to 0
	}

	// Increase m_colourChangeFactor
	m_colourChangeFactor += dt * m_colourChangeSpeed;
}

void Billboard::translateUV(float dt)
{
	m_UVincrement += dt * m_velocityUV;
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

BillboardData Billboard::getBillboardData() const
{
	BillboardData data;
	data.velocityUV = m_UVincrement;
	data.flashFactor= m_flashFactor;
	data.colourChangeFactor = m_colourChangeFactor;
	data.colourDecider = m_colourDecider;
	data.state = m_state;
	data.colourA = m_colourA;
	data.colourB = m_colourB;

	return data;
}

void Billboard::update(float dt)
{
	switch (m_state)
	{
	case BillboardState::e_flashing:
		flashColour(dt);
		break;
	case BillboardState::e_interpolating:
		interpolateColours(dt);
		break;
	case BillboardState::e_translating:
		translateUV(dt);
		break;
	case BillboardState::e_flashing_interpolating:
		flashColour(dt);
		interpolateColours(dt);
		break;
	case BillboardState::e_flashing_translating:
		flashColour(dt);
		translateUV(dt);
		break;
	case BillboardState::e_interpolating_translating:
		interpolateColours(dt);
		translateUV(dt);
		break;
	case BillboardState::e_all:
		flashColour(dt);
		interpolateColours(dt);
		translateUV(dt);
		break;
	}
}
