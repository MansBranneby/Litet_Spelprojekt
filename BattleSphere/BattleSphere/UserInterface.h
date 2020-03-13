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
	std::vector<UI_Element*> m_countDownElements;
	std::vector<UI_Element*> m_quitGameElements;
	int m_nrOfPlayers;

	ConstantBuffer* m_constantBufferColours;
	XMVECTOR m_playerColours[4];
	int m_slotID[16]; // index for ability type
	float m_slotPos[16]; // array for icon positions
	float m_countDownTimer;
	bool m_quitGame;

	bool m_drawPlayer[4];
	void setElementPos();

public:
	UserInterface(int nrOfPlayers);
	~UserInterface();
	void addPlayer(int playerIndex);
	void setPlayerColours(int playerIndex, XMVECTOR colour);
	void setSlotID(int playerIndex, int abilityType);
	void setSlotID(int playerIndex, int abilityType, int side, int next, int nextNext);

	void update();
	bool updateCountDown(float dt);
	void updateQuitGame(float dt);
	void setQuitGame(bool quitGame);
	void quitGameHI(int dir);
	void draw();
	void drawAbility(int playerIndex, int abilityType, float cd);
	void drawQuitGame();

	bool getQuitGame();
};

