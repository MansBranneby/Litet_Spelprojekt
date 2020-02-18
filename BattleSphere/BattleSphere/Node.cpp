#include "Node.h"

void Node::setMaterial(float colorIntensity)
{
	if (m_colorType == RED)
	{
		m_material.diffuse = RED_DIFFUSE;
		m_material.diffuse.m128_f32[0] *= colorIntensity;
		m_material.emission = RED_EMISSION;
		m_material.emission.m128_f32[0] *= colorIntensity;
	}
	else if (m_colorType == GREEN)
	{
		m_material.diffuse = GREEN_DIFFUSE;
		m_material.diffuse.m128_f32[1] *= colorIntensity;
		m_material.emission = GREEN_EMISSION;
		m_material.emission.m128_f32[1] *= colorIntensity;
	}
	else
	{
		m_material.diffuse = BLUE_DIFFUSE;
		m_material.diffuse.m128_f32[2] *= colorIntensity;
		m_material.emission = BLUE_EMISSION;
		m_material.emission.m128_f32[2] *= colorIntensity;
	}
}

void Node::leaveColor()
{
	// Calculate time relevant to this loop segment
	float segmentTime = m_loopTime - OFFER_TIME;

	// Change material color according to blinking frequency and time
	float intensity = (1.0f + XMScalarCosEst(segmentTime * XM_2PI * BLINKING_FREQUENCY)) / 2.0f;
	setMaterial(intensity);
}

void Node::offerNoColor()
{
	m_colorType = -1;
}

void Node::pickNewColor()
{
	int oneOrTwo = (rand() % 2) + 1;
	m_colorType = (m_oldColor + oneOrTwo) % 3;
	m_oldColor = m_colorType;
}

void Node::transitionToNewColor()
{
	// Calculate time relevant to this loop segment
	float segmentTime = m_loopTime - (OFFER_TIME + BLINKING_TIME + DOWN_TIME);

	// Change material color according to transition speed
	float intensity = (1.0f - XMScalarCosEst(segmentTime * XM_PI / TRANSITION_TIME)) / 2.0f;
	setMaterial(intensity);
}

Node::Node(int colorType)
{
	m_colorType = m_oldColor = colorType;
	m_loopTime = 0.0f;
	m_newColorPicked = false;
	setScale(0.3f, 0.3f, 0.3f);
	setRotation(0, 1, 0, 90);
	setMaterial();
}

Node::~Node()
{
}

bool Node::isType(int type)
{
	if (type == PISTOL || type == RIFLE)
	{
		if (m_colorType == RED)
			return true;
	}
	else if (type == MOVEMENT)
	{
		if (m_colorType == GREEN)
			return true;
	}
	else
	{
		if (m_colorType == BLUE)
			return true;
	}

	return false;
}

void Node::updateTime(float dT)
{
	// Update loop time
	m_loopTime += dT;

	if (m_loopTime < OFFER_TIME) // Offer color
	{

	}
	else if (m_loopTime < OFFER_TIME + BLINKING_TIME) // Leave current color
	{
		leaveColor();
	}
	else if (m_loopTime < OFFER_TIME + BLINKING_TIME + DOWN_TIME) // Offer no color
	{
		offerNoColor();
	}
	else if (!m_newColorPicked) // Pick new color
	{
		pickNewColor();
		m_newColorPicked = true;
	}
	else if (m_loopTime < OFFER_TIME + BLINKING_TIME + DOWN_TIME + TRANSITION_TIME) // Transition to new color
	{
		transitionToNewColor();
	}
	else // Total loop time overshot - Reset loop
	{
		m_loopTime = 0.0f;
		m_newColorPicked = false;
	}
}
