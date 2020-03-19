#pragma once

#include "UI_Element.h"
#include "ConstantBuffer.h"
#include "Sound.h"

#define PLAYER_BG_PAD 110.4f
#define PLAYER_BG_H 115.0f
#define PLAYER_BG_W 342.0f
#define ICON_PAD 20.0f

class UserInterface
{
private:
	std::vector<UI_Element*> m_elements;
	std::vector<UI_Element*> m_countDownElements;
	std::vector<UI_Element*> m_quitGameElements;
	std::vector<UI_Element*> m_healthBarElements;
	std::vector<UI_Element*> m_winnerElements;
	UI_Element* m_BlackScreenElement;
	int m_nrOfPlayers;

	ConstantBuffer* m_constantBufferColours;
	XMVECTOR m_playerColours[4];
	int m_slotID[16]; // index for ability type
	float m_slotPos[16]; // array for icon positions
	float m_countDownTimer;
	bool m_quitGame;
	float m_winningTimer;
	float m_blackScreenFadeTimer;
	bool m_drawPlayer[4];
	void setElementPos();

public:
	UserInterface(int nrOfPlayers);
	~UserInterface();
	void addPlayer(int playerIndex);
	void setPlayerColours(int playerIndex, XMVECTOR colour);
	void setSlotID(int playerIndex, int abilityType);
	void setSlotID(int playerIndex, int abilityType, int side, int next, int nextNext);

	void adjustForScreen();
	void update();
	bool updateCountDown(float dt);
	bool updateWinning(float dt);
	bool updateScoreWinning(float dt);
	void updateQuitGame(float dt);
	void setQuitGame(bool quitGame);
	void quitGameHI(int dir);
	void draw();
	void drawAbility(int playerIndex, int abilityType, float cd);
	void drawHealthbar(int playerIndex, float hp);
	void drawQuitGame();
	void drawWinning(XMVECTOR winnerColour);
	void drawBlackScreen();

	bool getQuitGame();
};

