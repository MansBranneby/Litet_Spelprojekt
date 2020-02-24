#pragma once

#include "StructsAndEnums.h"
#include "imgui/imgui.h"

enum AnimationType
{
	e_translate,
	e_sprite
};

class UI_Animation
{
private:
	float m_timer; // Keep track of delay and rest
	float m_animationSpeed, m_acceleration ,m_delay, m_delay2, m_rest; // Animation data
	
	float m_sizeX, m_sizeY;
	float m_spriteSizeX, m_spriteSizeY;
	u_int m_nrOfFrames;
	float m_texU, m_texV;

	float m_fadeOut, m_initialFadeTime;
	float m_fadeIn;
	bool m_isAnimated; // Controls if spriteSheet is updated
	bool m_isFadeOut, m_isFadeIn;

public:
	UI_Animation(float sizeX, float sizeY, float spriteSizeX, float spriteSizeY, u_int nrOfFrames);
	~UI_Animation();

	bool isAnimated();
	void setAnimated(bool isAnimated);
	bool isFadeOut();
	bool isFadeIn();
	void setFadeOut(float fadeOut, float delay);
	void setFadeIn(float fadeIn, float delay);
	void setAnimationData(float speed, float acceleration, float delay, float rest);

	bool translateElement(vertex* vertexList, float* posX, float* posY, float sizeX, float sizeY, float destX, float destY, float dt);
	void animateElement(vertex* vertexList, float dt);
	bool fadeElement(vertex* vertexList, float dt);
};

