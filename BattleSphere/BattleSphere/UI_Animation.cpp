#include "UI_Animation.h"

bool UI_Animation::animateElement(uiType type, float* posX, float* posY)
{
	bool isReady = true;

	//if (m_selectionTimer > 0.0f)
	//{
	//	isReady = false;
	//	m_selectionTimer += dt;
	//	if (m_selectionTimer > 0.2)
	//	{
	//		m_isReady = true;
	//		m_selectionTimer = 0.0f;
	//	}
	//}

	//if (m_destinationX != m_posX || m_destinationY != m_posY) // Only update if element needs to move
	//{
	//	m_isReady = false;
	//	float left, right, top, bottom;
	//	float animationSpeed = 1000.0f;
	//	if (m_destinationY < m_posY) // Down
	//	{
	//		if (m_posY - dt * animationSpeed < m_destinationY)
	//			m_posY = m_destinationY;
	//		else
	//			m_posY -= dt * animationSpeed;
	//	}
	//	if (m_destinationY > m_posY) // Up
	//	{
	//		if (m_posY + dt * animationSpeed > m_destinationY)
	//			m_posY = m_destinationY;
	//		else
	//			m_posY += dt * animationSpeed;
	//	}
	//}
	return isReady;
}

	UI_Animation::UI_Animation()
	{
		m_timer = 0.0f;
	}