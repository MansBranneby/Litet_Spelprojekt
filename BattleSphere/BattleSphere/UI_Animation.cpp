#include "UI_Animation.h"

bool UI_Animation::animateElement(AnimationType animationType, vertex* vertexList, float* posX, float* posY, float sizeX, float sizeY, float destX, float destY, float dt)
{
	bool isReady = false;

	switch (animationType)
	{
	case e_translate:
		if (m_timer > 0.0f)
		{
			isReady = false;
			m_timer += dt;
			if (m_timer > 0.2)
			{
				isReady = true;
				m_timer = 0.0f;
			}
		}

		if (destX != *posX || destY != *posY) // Only update if element needs to move
		{
			isReady = false;
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

			left = *posX - sizeX / 2.0f;
			right = left + sizeX;
			top = *posY + sizeY / 2.0f;
			bottom = top - sizeY;

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

			if (destX == *posX && destY == *posY)
				m_timer += dt;
		}
		break;
	case e_sprite:
		break;
	default:
		break;
	}

	return isReady;
}

void UI_Animation::animateElement(AnimationType anumationType, vertex* vertexList, float dt)
{
	m_timer += dt;
	if (m_timer >= 0.1f)
	{
		m_timer = 0.0f;
		float u = m_spriteSizeX / m_sizeX;
		float v = m_spriteSizeY / m_sizeY;
		int activeFrame = m_texU / (u) * 6 + m_texV / (v) - 6;
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

UI_Animation::UI_Animation(float sizeX, float sizeY, float spriteSizeX, float spriteSizeY, u_int nrOfFrames, float animationSpeed)
{
	m_timer = 0.0f;
	m_sizeX = sizeX;
	m_sizeY = sizeY;
	m_spriteSizeX = spriteSizeX;
	m_spriteSizeY = spriteSizeY;
	m_nrOfFrames = nrOfFrames;
	m_animationSpeed = animationSpeed;

	m_texU = m_spriteSizeX / m_sizeX;
	m_texV = m_spriteSizeY / m_sizeY;
}

UI_Animation::~UI_Animation()
{
}
