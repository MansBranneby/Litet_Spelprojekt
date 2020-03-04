#pragma once

#include "UI_Element.h"
#include "ConstantBuffer.h"

#define PLAYER_BG_PAD 110.4f
#define PLAYER_BG_H 86.0f
#define PLAYER_BG_W 342.0f
#define ICON_PAD 20.0f

class UserInterface
{
private:
	std::vector<UI_Element*> m_elements;
	int m_nrOfPlayers;

	ConstantBuffer* m_constantBufferColours;
	XMVECTOR m_playerColours[4];
	int m_slotID[16]; // index for ability type
	float m_slotPos[16]; // array for icon positions

	void setElementPos();

public:
	UserInterface(int nrOfPlayers);
	~UserInterface();

	void setPlayerColours(int playerIndex, XMVECTOR colour);
	void setSlotID(int playerIndex, int abilityType);
	void setSlotID(int playerIndex, int abilityType, int side, int newIndex);

	void update();
	void draw();
};

