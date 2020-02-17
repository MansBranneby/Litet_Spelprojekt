#include "UI_Animation.h"

bool UI_Animation::translateElement(vertex* vertexList, float* posX, float* posY, float sizeX, float sizeY, float destX, float destY, float dt)
{
	bool isReady = false;

	/*if (m_timer > 0.0f)
	{
		isReady = false;
		m_timer += dt;
		if (m_timer > 0.2)
		{
			isReady = true;
			m_timer = 0.0f;
		}
	}*/
	if (m_delay > 0.0f) // Waiting...
	{
		m_delay -= dt;
	}
	else if (destX != *posX || destY != *posY) // Translating
	{
		m_animationSpeed += m_acceleration * dt;
		float left, right, top, bottom;

		if (destY < *posY) // Down
		{
			if (*posY - dt * m_animationSpeed < destY)
				*posY = destY;
			else
				*posY -= dt * m_animationSpeed;
		}
		if (destY > * posY) // Up
		{
			if (*posY + dt * m_animationSpeed > destY)
				*posY = destY;
			else
				*posY += dt * m_animationSpeed;
		}
		if (destX < *posX) // Left
		{
			if (*posX - dt * m_animationSpeed < destX)
				*posX = destX;
			else
				*posX -= dt * m_animationSpeed;
		}
		if (destX > * posX) // Right
		{
			if (*posX + dt * m_animationSpeed > destX)
				*posX = destX;
			else
				*posX += dt * m_animationSpeed;
		}

		if (m_spriteSizeX != 0.0f || m_spriteSizeY != 0.0f)
		{
			left = *posX - m_spriteSizeX / 2.0f;
			right = left + m_spriteSizeX;
			top = *posY + m_spriteSizeY / 2.0f;
			bottom = top - m_spriteSizeY;
		}
		else
		{
			left = *posX - sizeX / 2.0f;
			right = left + sizeX;
			top = *posY + sizeY / 2.0f;
			bottom = top - sizeY;
		}
	

		vertexList[0].posX = left;
		vertexList[0].posY = top;

		vertexList[1].posX = right;
		vertexList[1].posY = bottom;

		vertexList[2].posX = left;
		vertexList[2].posY = bottom;

		vertexList[3].posX = left;
		vertexList[3].posY = top;

		vertexList[4].posX = right;
		vertexList[4].posY = top;

		vertexList[5].posX = right;
		vertexList[5].posY = bottom;

		//if (destX == *posX && destY == *posY)
		//	m_timer += dt;
	}
	else // Resting...
	{
		m_rest -= dt;
		if (m_rest < 0.0f)
			isReady = true;
	}

	return isReady;
}

void UI_Animation::animateElement(vertex* vertexList, float dt)
{
	// Sprite
	if (m_spriteSizeX != 0.0f || m_spriteSizeY != 0.0f)
	{
		m_timer += dt;
		if (m_timer >= 0.1f)
		{
			m_timer = 0.0f;
			float u = m_spriteSizeX / m_sizeX;
			float v = m_spriteSizeY / m_sizeY;
			int activeFrame = m_texU / (u) * 6 + m_texV / (v)-6;
			if (activeFrame == m_nrOfFrames) // Reset sprite sheet
			{
				m_texU = u;
				m_texV = v;
			}
			else
			{
				if (m_texV + v > 1)
				{
					m_texU += u;
					m_texV = v;
				}
				else
				{
					m_texV += v;
				}
			}

			vertexList[0].u = m_texU - u;
			vertexList[0].v = m_texV - v;

			vertexList[1].u = m_texU;
			vertexList[1].v = m_texV;

			vertexList[2].u = m_texU - u;
			vertexList[2].v = m_texV;

			vertexList[3].u = m_texU - u;
			vertexList[3].v = m_texV - v;

			vertexList[4].u = m_texU;
			vertexList[4].v = m_texV - v;

			vertexList[5].u = m_texU;
			vertexList[5].v = m_texV;
		}
	}
	if (m_fadeOut > 0.0f)
	{
		vertexList[0].normX = 1.0f - (1 - m_fadeOut/m_initialFadeTime);
		vertexList[1].normX = 1.0f - (1 - m_fadeOut/m_initialFadeTime);
		vertexList[2].normX = 1.0f - (1 - m_fadeOut/m_initialFadeTime);
		vertexList[3].normX = 1.0f - (1 - m_fadeOut/m_initialFadeTime);
		vertexList[4].normX = 1.0f - (1 - m_fadeOut/m_initialFadeTime);
		vertexList[5].normX = 1.0f - (1 - m_fadeOut/m_initialFadeTime);

		m_fadeOut -= dt;
	}
}

UI_Animation::UI_Animation(float sizeX, float sizeY, float spriteSizeX, float spriteSizeY, u_int nrOfFrames)
{
	m_timer = 0.0f;
	m_delay = 0.0f;
	m_rest = 0.0f;
	m_sizeX = sizeX;
	m_sizeY = sizeY;
	m_spriteSizeX = spriteSizeX;
	m_spriteSizeY = spriteSizeY;
	m_nrOfFrames = nrOfFrames;
	m_animationSpeed = 0.0f;
	m_acceleration = 0.0f;

	m_texU = m_spriteSizeX / m_sizeX;
	m_texV = m_spriteSizeY / m_sizeY;

	m_isAnimated = false;
	m_fadeIn = false;
	m_fadeOut = false;
}

UI_Animation::~UI_Animation()
{
}

bool UI_Animation::isAnimated()
{
	return m_isAnimated || m_fadeIn || m_fadeOut;
}

void UI_Animation::setAnimated(bool isAnimated)
{
	m_isAnimated = isAnimated;
}

bool UI_Animation::isFadeOut()
{
	return m_fadeOut;
}

bool UI_Animation::isFadeIn()
{
	return m_fadeIn;
}

void UI_Animation::setFadeOut(float fadeOut)
{
	m_fadeOut = fadeOut;
	m_initialFadeTime = fadeOut;
}

void UI_Animation::setFadeIn(float fadeIn)
{
	m_fadeIn = fadeIn;
	m_initialFadeTime = fadeIn;
}

void UI_Animation::setAnimationData(float speed, float acceleration, float delay, float rest)
{
	m_animationSpeed = speed;
	m_acceleration = acceleration;
	m_delay = delay;
	m_rest = rest;
}
