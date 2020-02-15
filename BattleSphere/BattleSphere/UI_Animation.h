#pragma once

#include "StructsAndEnums.h"

enum AnimationType
{
	e_translate,
	e_sprite
};

class UI_Animation
{
private:
	//bool m_isReady;
	float m_timer;
	float m_animationSpeed;
	float m_sizeX, m_sizeY;
	float m_spriteSizeX, m_spriteSizeY, m_nrOfFrames;
	float m_texU, m_texV;

public:
	UI_Animation(float sizeX, float sizeY, float spriteSizeX, float spriteSizeY, u_int nrOfFrames, float animationSpeed);
	~UI_Animation();

	bool animateElement(AnimationType animationType, vertex* vertexList, float* posX, float* posY, float sizeX, float sizeY, float destX, float destY, float dt);
	void animateElement(AnimationType anumationType, vertex* vertexList, float dt);
};

