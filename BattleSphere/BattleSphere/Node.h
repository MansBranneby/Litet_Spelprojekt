#pragma once
#include "GameObject.h"

#define OFFER_TIME 15.0f // Default 3.0f - Time to offer color transaction
#define BLINKING_TIME 3.5f // Default 3.0f - Time to blink before changing type
#define BLINKING_FREQUENCY 1.0f // Blinks per second
#define DOWN_TIME 1.0f // Time to offer no color
#define TRANSITION_TIME 1.5f // Default 3.0f - Time to transition to new type

class Node : public GameObject
{
private:
	int m_colorType;
	void setMaterial(float colorIntensity = 1.0f);

	// Color offer looping
	int m_oldColor;
	float m_loopTime;
	bool m_newColorPicked;
	void leaveColor();
	void offerNoColor();
	void pickNewColor();
	void transitionToNewColor();

public:
	Node(int colorType = 0);
	~Node();

	bool isType(int type);
	bool updateTime(float dT);
};

