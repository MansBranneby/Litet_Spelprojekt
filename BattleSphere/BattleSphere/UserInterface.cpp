#include "UserInterface.h"

void UserInterface::setElementPos()
{
	for (int i = 0; i < NR_OF_ICON_ELEMENTS; i++)
		m_elements[i]->setDrawn(false);
	for (int i = 0; i < m_nrOfPlayers; i++) // Iterate players
	{
		
			for (int j = 0; j < 4; j++) // Iterate icon slots
			{
				if (m_slotID[i * 4 + j] != -1)
				{
					m_elements[i * 8 + m_slotID[i * 4 + j]]->setPos(m_slotPos[i * 4 + j], -500.0f);
					m_elements[i * 8 + m_slotID[i * 4 + j]]->setDrawn(true);
				}
			}
		
		
	}
}

UserInterface::UserInterface(int nrOfPlayers)
{
	// Countdown
	m_countDownTimer = 0.0f;
	m_countDownElements.push_back(new UI_Element(L"Textures\\GameState\\countdown_1.png", false, 0.0f, 0.0f, 400.0f, 400.0f));
	m_countDownElements.push_back(new UI_Element(L"Textures\\GameState\\countdown_2.png", false, 0.0f, 0.0f, 400.0f, 400.0f));
	m_countDownElements.push_back(new UI_Element(L"Textures\\GameState\\countdown_3.png", false, 0.0f, 0.0f, 400.0f, 400.0f));
	m_countDownElements.push_back(new UI_Element(L"Textures\\GameState\\countdown_GO!.png", false, 0.0f, 0.0f, 674.0f, 400.0f));

	// Quit Game
	m_quitGameElements.push_back(new UI_Element(L"Textures\\GameState\\quitGame.png", true, 0.0f, 100.0f, 599.0f, 66.0f));
	m_quitGameElements.push_back(new UI_Element(L"Textures\\GameState\\quitGame_selection.png", true, 0.0f, -100.0f, 420.0f, 69.0f));
	m_quitGameElements.push_back(new UI_Element(L"Textures\\GameState\\quitGame_yes.png", true, 0.0f, -100.0f, 172.0f, 66.0f));
	m_quitGameElements.push_back(new UI_Element(L"Textures\\GameState\\quitGame_no.png", true, -249.0f, -100.0f, 124.0f, 66.0f));

	for (int i = 0; i < m_quitGameElements.size(); i++)
		m_quitGameElements[i]->setDrawn(false);

	m_quitGame = true;

	for (int i = 0; i < 4; i++)
	{
		m_drawPlayer[i] = false;
	}
	// PLAYER 1
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Rifle.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Movement.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Shield.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Dash.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Reflect.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Sniper.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Beyblade.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Energy.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
	// PLAYER 2																		 
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Rifle.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Movement.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Shield.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Dash.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Reflect.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Sniper.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Beyblade.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Energy.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
	// PLAYER 3																		  
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Rifle.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Movement.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Shield.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Dash.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Reflect.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Sniper.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Beyblade.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Energy.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
	// PLAYER 4																
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Rifle.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Movement.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Shield.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Dash.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Reflect.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Sniper.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Beyblade.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Energy.png", true, -960.0f, -508.0f, 60.0f, 60.0f));

	for (int i = 0; i < NR_OF_ICON_SLOTS; i++) // No abilities selected or in queue
		m_slotID[i] = -1;
	//nrOfPlayers = 4;
	switch (nrOfPlayers)
	{
	case 1:
		m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\PlayerBG.png", true, 0.0f, -(540.0f - PLAYER_BG_H / 2.0f), PLAYER_BG_W, PLAYER_BG_H));
		m_healthBarElements.push_back(new UI_Element(L"Textures\\UserInterface\\healthbar.png", true, 88.0f, -(540.0f - PLAYER_BG_H) -13.0f, 167.0f, 26.0f));

		m_slotID[1] = 0;
		m_slotPos[0] = -120.0f;
		//m_elements[0]->setPos(m_slotPos[0], -508.0f);
		for (int i = 1; i < 4; i++)
		{
			m_slotPos[i] = m_slotPos[i - 1] + ICON_PAD + 60.0f;
			//m_elements[i]->setPos(m_slotPos[i], -508.0f);
		}
		break;
	case 2:
		m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\PlayerBG.png", true, -(960.0f - PLAYER_BG_W / 2.0f - PLAYER_BG_PAD) + 1.0f * (PLAYER_BG_PAD + PLAYER_BG_W), -(540.0f - PLAYER_BG_H / 2.0f), PLAYER_BG_W, PLAYER_BG_H));
		m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\PlayerBG1.png", true, -(960.0f - PLAYER_BG_W / 2.0f - PLAYER_BG_PAD) + 2.0f * (PLAYER_BG_PAD + PLAYER_BG_W), -(540.0f - PLAYER_BG_H / 2.0f), PLAYER_BG_W, PLAYER_BG_H));
		m_healthBarElements.push_back(new UI_Element(L"Textures\\UserInterface\\healthbar.png", true, 88.0f -(960.0f - PLAYER_BG_W / 2.0f - PLAYER_BG_PAD) + 1.0f * (PLAYER_BG_PAD + PLAYER_BG_W), -(540.0f - PLAYER_BG_H) - 13.0f, 167.0f, 26.0f));
		m_healthBarElements.push_back(new UI_Element(L"Textures\\UserInterface\\healthbar.png", true, 88.0f -(960.0f - PLAYER_BG_W / 2.0f - PLAYER_BG_PAD) + 2.0f * (PLAYER_BG_PAD + PLAYER_BG_W), -(540.0f - PLAYER_BG_H) - 13.0f, 167.0f, 26.0f));

		m_slotID[1] = 0;
		m_slotPos[0] = -120.0f - PLAYER_BG_W / 2.0f - PLAYER_BG_PAD / 2.0f;
		//m_elements[0]->setPos(m_slotPos[0], -508.0f);
		for (int i = 1; i < 4; i++)
		{
			m_slotPos[i] = m_slotPos[i - 1] + ICON_PAD + 60.0f;
			//m_elements[i]->setPos(m_slotPos[i], -508.0f);
		}
		m_slotID[5] = 0;
		m_slotPos[4] = m_slotPos[0] + PLAYER_BG_W + PLAYER_BG_PAD;
		//m_elements[4]->setPos(m_slotPos[4], -508.0f);
		for (int i = 5; i < 8; i++)
		{
			m_slotPos[i] = m_slotPos[i - 1] + ICON_PAD + 60.0f;
			//m_elements[i]->setPos(m_slotPos[i], -508.0f);
		}
		break;
	case 3:
		m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\PlayerBG.png", true, -PLAYER_BG_W - PLAYER_BG_PAD, -(540.0f - PLAYER_BG_H / 2.0f), PLAYER_BG_W, PLAYER_BG_H));
		m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\PlayerBG1.png", true, 0.0f, -(540.0f - PLAYER_BG_H / 2.0f), PLAYER_BG_W, PLAYER_BG_H));
		m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\PlayerBG2.png", true, PLAYER_BG_W + PLAYER_BG_PAD, -(540.0f - PLAYER_BG_H / 2.0f), PLAYER_BG_W, PLAYER_BG_H));
		m_healthBarElements.push_back(new UI_Element(L"Textures\\UserInterface\\healthbar.png", true, 88.0f - PLAYER_BG_W - PLAYER_BG_PAD, -(540.0f - PLAYER_BG_H) - 13.0f, 167.0f, 26.0f));
		m_healthBarElements.push_back(new UI_Element(L"Textures\\UserInterface\\healthbar.png", true, 88.0f, -(540.0f - PLAYER_BG_H) - 13.0f, 167.0f, 26.0f));
		m_healthBarElements.push_back(new UI_Element(L"Textures\\UserInterface\\healthbar.png", true, 88.0f + PLAYER_BG_W + PLAYER_BG_PAD, -(540.0f - PLAYER_BG_H) - 13.0f, 167.0f, 26.0f));
		
		m_slotID[1] = 0;
		m_slotPos[0] = -120.0f - PLAYER_BG_W - PLAYER_BG_PAD;
		//m_elements[0]->setPos(m_slotPos[0], -508.0f);
		for (int i = 1; i < 4; i++)
		{
			m_slotPos[i] = m_slotPos[i - 1] + ICON_PAD + 60.0f;
			//m_elements[i]->setPos(m_slotPos[i], -508.0f);
		}
		m_slotID[5] = 0;
		m_slotPos[4] = m_slotPos[0] + PLAYER_BG_W + PLAYER_BG_PAD;
		//m_elements[4]->setPos(m_slotPos[4], -508.0f);
		for (int i = 5; i < 8; i++)
		{
			m_slotPos[i] = m_slotPos[i - 1] + ICON_PAD + 60.0f;
			//m_elements[i]->setPos(m_slotPos[i], -508.0f);
		}
		m_slotID[9] = 0;
		m_slotPos[8] = m_slotPos[4] + PLAYER_BG_W + PLAYER_BG_PAD;
		//m_elements[8]->setPos(m_slotPos[8], -508.0f);
		for (int i = 9; i < 12; i++)
		{
			m_slotPos[i] = m_slotPos[i - 1] + ICON_PAD + 60.0f;
			//m_elements[i]->setPos(m_slotPos[i], -508.0f);
		}
		break;
	case 4:
		m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\PlayerBG.png", true, -(960.0f - PLAYER_BG_W / 2.0f - PLAYER_BG_PAD) + 0.0f * (PLAYER_BG_PAD + PLAYER_BG_W), -(540.0f - PLAYER_BG_H / 2.0f), PLAYER_BG_W, PLAYER_BG_H));
		m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\PlayerBG1.png", true, -(960.0f - PLAYER_BG_W / 2.0f - PLAYER_BG_PAD) + 1.0f * (PLAYER_BG_PAD + PLAYER_BG_W), -(540.0f - PLAYER_BG_H / 2.0f), PLAYER_BG_W, PLAYER_BG_H));
		m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\PlayerBG2.png", true, -(960.0f - PLAYER_BG_W / 2.0f - PLAYER_BG_PAD) + 2.0f * (PLAYER_BG_PAD + PLAYER_BG_W), -(540.0f - PLAYER_BG_H / 2.0f), PLAYER_BG_W, PLAYER_BG_H));
		m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\PlayerBG3.png", true, -(960.0f - PLAYER_BG_W / 2.0f - PLAYER_BG_PAD) + 3.0f * (PLAYER_BG_PAD + PLAYER_BG_W), -(540.0f - PLAYER_BG_H / 2.0f), PLAYER_BG_W, PLAYER_BG_H));
		m_healthBarElements.push_back(new UI_Element(L"Textures\\UserInterface\\healthbar.png", true, 88.0f - (960.0f - PLAYER_BG_W / 2.0f - PLAYER_BG_PAD) + 0.0f * (PLAYER_BG_PAD + PLAYER_BG_W), -(540.0f - PLAYER_BG_H) - 13.0f, 167.0f, 26.0f));
		m_healthBarElements.push_back(new UI_Element(L"Textures\\UserInterface\\healthbar.png", true, 88.0f - (960.0f - PLAYER_BG_W / 2.0f - PLAYER_BG_PAD) + 1.0f * (PLAYER_BG_PAD + PLAYER_BG_W), -(540.0f - PLAYER_BG_H) - 13.0f, 167.0f, 26.0f));
		m_healthBarElements.push_back(new UI_Element(L"Textures\\UserInterface\\healthbar.png", true, 88.0f - (960.0f - PLAYER_BG_W / 2.0f - PLAYER_BG_PAD) + 2.0f * (PLAYER_BG_PAD + PLAYER_BG_W), -(540.0f - PLAYER_BG_H) - 13.0f, 167.0f, 26.0f));
		m_healthBarElements.push_back(new UI_Element(L"Textures\\UserInterface\\healthbar.png", true, 88.0f - (960.0f - PLAYER_BG_W / 2.0f - PLAYER_BG_PAD) + 3.0f * (PLAYER_BG_PAD + PLAYER_BG_W), -(540.0f - PLAYER_BG_H) - 13.0f, 167.0f, 26.0f));
		m_slotID[1] = 0;
		m_slotPos[0] = -120.0f -  PLAYER_BG_W / 2.0f - PLAYER_BG_PAD / 2.0f - PLAYER_BG_W - PLAYER_BG_PAD;
		//m_elements[0]->setPos(m_slotPos[0], -508.0f);
		for (int i = 1; i < 4; i++)
		{
			m_slotPos[i] = m_slotPos[i - 1] + ICON_PAD + 60.0f;
			//m_elements[i]->setPos(m_slotPos[i], -508.0f);
		}
		m_slotID[5] = 0;
		m_slotPos[4] = m_slotPos[0] + PLAYER_BG_W + PLAYER_BG_PAD;
		//m_elements[4]->setPos(m_slotPos[4], -508.0f);
		for (int i = 5; i < 8; i++)
		{
			m_slotPos[i] = m_slotPos[i - 1] + ICON_PAD + 60.0f;
			//m_elements[i]->setPos(m_slotPos[i], -508.0f);
		}
		m_slotID[9] = 0;
		m_slotPos[8] = m_slotPos[4] + PLAYER_BG_W + PLAYER_BG_PAD;
		//m_elements[4]->setPos(m_slotPos[4], -508.0f);
		for (int i = 9; i < 12; i++)
		{
			m_slotPos[i] = m_slotPos[i - 1] + ICON_PAD + 60.0f;
			//m_elements[i]->setPos(m_slotPos[i], -508.0f);
		}
		m_slotID[13] = 0;
		m_slotPos[12] = m_slotPos[8] + PLAYER_BG_W + PLAYER_BG_PAD;
		//m_elements[4]->setPos(m_slotPos[4], -508.0f);
		for (int i = 13; i < 16; i++)
		{
			m_slotPos[i] = m_slotPos[i - 1] + ICON_PAD + 60.0f;
			//m_elements[i]->setPos(m_slotPos[i], -508.0f);
		}


		break;
	default:
		break;
	}

	for (int i = 0; i < nrOfPlayers; i++)
		m_playerColours[i] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	m_constantBufferColours = new ConstantBuffer(&m_playerColours[0], sizeof(XMVECTOR));

	m_nrOfPlayers = nrOfPlayers;

	// Setup abilities
	setElementPos();
}

UserInterface::~UserInterface()
{
	for (int i = 0; i < m_elements.size(); i++)
		delete m_elements[i];
	for (int i = 0; i < m_countDownElements.size(); i++)
		delete m_countDownElements[i];
	for (int i = 0; i < m_quitGameElements.size(); i++)
		delete m_quitGameElements[i];
	for (int i = 0; i < m_healthBarElements.size(); i++)
		delete m_healthBarElements[i];
	delete m_constantBufferColours;
}

void UserInterface::addPlayer(int playerIndex)
{
	m_drawPlayer[playerIndex] = true;
}

void UserInterface::setPlayerColours(int playerIndex, XMVECTOR colour)
{
	//Find actual playerIndex
	

	int reduction = 0;
	for (int i = 0; i < playerIndex; i++)
	{
		if (!m_drawPlayer[i]) reduction++;
	}
	playerIndex -= reduction;
	m_playerColours[playerIndex] = colour;
}

void UserInterface::setSlotID(int playerIndex, int abilityType)
{
	int reduction = 0;
	for (int i = 0; i < playerIndex; i++)
	{
		if (!m_drawPlayer[i]) reduction++;
	}
	playerIndex -= reduction;
	for (int i = 0; i < 4; i++) // Iterate slots
	{
		if (m_slotID[playerIndex * 4 + i] == -1) // No ability on slot
		{
			m_slotID[playerIndex * 4 + i] = abilityType; // Set ability to current slot
			break;
		}
	}
}
//
//void UserInterface::setSlotID(int playerIndex, int abilityType, int side, int newIndex)
//{
//	int oldIndex = m_slotID[playerIndex * 4 + side]; // Save old index
//	m_slotID[playerIndex * 4 + side] = abilityType; // Replace with new
//
//	int nrOfSlots = 0;
//	for (int i = 0; i < 4; i++) // Calculate whitch replacement to use
//	{
//		if (m_slotID[playerIndex * 4 + i] != -1)
//			nrOfSlots++;
//	}
//	switch (nrOfSlots)
//	{
//	case 3:
//		m_slotID[playerIndex * 4 + 2] = oldIndex; // Replace new with old
//		break;
//	case 4:
//		m_slotID[playerIndex * 4 + 2] = m_slotID[playerIndex * 4 + 3];
//		m_slotID[playerIndex * 4 + 3] = oldIndex;
//		break;
//	default: // newIndex != -1 and number of abilities > slots
//		m_slotID[playerIndex * 4 + 2] = m_slotID[playerIndex * 4 + 3];
//		m_slotID[playerIndex * 4 + 3] = newIndex;
//		break;
//	}
//}

void UserInterface::setSlotID(int playerIndex, int abilityType, int side, int next, int nextNext)
{
	int reduction = 0;
	for (int i = 0; i < playerIndex; i++)
	{
		if (!m_drawPlayer[i]) reduction++;
	}
	playerIndex -= reduction;
	int oldIndex = m_slotID[playerIndex * 4 + side]; // Save old index
	m_slotID[playerIndex * 4 + side] = abilityType; // Replace with new

	int nrOfSlots = 0;
	for (int i = 0; i < 4; i++) // Calculate whitch replacement to use
	{
		if (m_slotID[playerIndex * 4 + i] != -1)
			nrOfSlots++;
	}
	switch (nrOfSlots)
	{
	case 3:
		m_slotID[playerIndex * 4 + 2] = next; // Replace new with old
		break;
	default: // newIndex != -1 and number of abilities > slots
		m_slotID[playerIndex * 4 + 2] = next;
		m_slotID[playerIndex * 4 + 3] = nextNext;
		break;
	}
}

void UserInterface::update()
{
	setElementPos();
}

bool UserInterface::updateCountDown(float dt)
{
	m_countDownTimer += dt;

	if (m_countDownTimer >= 4.0f)
	{
		m_countDownElements[3]->setDrawn(false);
		return false;
	}
	else
	{
		if (m_countDownTimer < 1.0f)
		{
			if (!m_countDownElements[2]->isDrawn())
			{
				m_countDownElements[2]->fadeIn(0.8f, 0.0f);
				m_countDownElements[2]->fadeOut(0.1f, 0.9f);
				m_countDownElements[2]->setDrawn(true);
			}
			m_countDownElements[2]->setPos(0.0f, 0.0f, m_countDownTimer);
			m_countDownElements[2]->updateElement(dt);
		}
		if (m_countDownTimer > 1.0f && m_countDownTimer < 2.0f)
		{
			if (!m_countDownElements[1]->isDrawn())
			{
				m_countDownElements[1]->fadeIn(0.8f, 0.0f);
				m_countDownElements[1]->fadeOut(0.1f, 0.9f);
				m_countDownElements[1]->setDrawn(true);
			}
			m_countDownElements[1]->setPos(0.0f, 0.0f, m_countDownTimer - 1.0f);
			m_countDownElements[1]->updateElement(dt);
			m_countDownElements[2]->setDrawn(false);
		}
		if (m_countDownTimer > 2.0f && m_countDownTimer < 3.0f)
		{
			if (!m_countDownElements[0]->isDrawn())
			{
				m_countDownElements[0]->fadeIn(0.8f, 0.0f);
				m_countDownElements[0]->fadeOut(0.1f, 0.9f);
				m_countDownElements[0]->setDrawn(true);
			}
			m_countDownElements[0]->setPos(0.0f, 0.0f, m_countDownTimer - 2.0f);
			m_countDownElements[0]->updateElement(dt);
			m_countDownElements[1]->setDrawn(false);
		}
		if (m_countDownTimer > 3.0f)
		{
			if (!m_countDownElements[3]->isDrawn())
			{
				m_countDownElements[3]->fadeIn(0.8f, 0.0f);
				m_countDownElements[3]->fadeOut(0.1f, 0.9f);
				m_countDownElements[3]->setDrawn(true);
			}
			m_countDownElements[3]->setPos(0.0f, 0.0f, (m_countDownTimer - 3.0f));
			m_countDownElements[3]->updateElement(dt);
			m_countDownElements[0]->setDrawn(false);
		}
		return true;
	}
}

void UserInterface::updateQuitGame(float dt)
{
	m_quitGameElements[2]->updateElement(dt);
	m_quitGameElements[3]->updateElement(dt);
}

void UserInterface::setQuitGame(bool quitGame)
{
	m_quitGame = quitGame;
	m_quitGameElements[2]->setPos(0.0f, -100.0f);
	m_quitGameElements[3]->setPos(-250.0f, -100.0f);
	m_quitGameElements[2]->setAbsDestinationX(0.0f, 10000.0f, 2.0f, 0.0f, 0.0f);
	m_quitGameElements[3]->setAbsDestinationX(-250.0f, 10000.0f, 2.0f, 0.0f, 0.0f);
}

void UserInterface::quitGameHI(int dir)
{
	if (m_quitGameElements[2]->isReady() && m_quitGameElements[3]->isReady())
	{
		if (dir) // Right
		{
			if (m_quitGame) // Yes
			{
				m_quitGameElements[3]->setPos(-250.0f, -100.0f);
				m_quitGameElements[2]->setAbsDestinationX(250.0f, 1000.0f, 2.0f, 0.0f, 0.0f);
				m_quitGameElements[3]->setAbsDestinationX(0.0f, 1000.0f, 2.0f, 0.0f, 0.0f);
				m_quitGame = false;
			}
			else
			{
				m_quitGameElements[2]->setPos(-250.0f, -100.0f);
				m_quitGameElements[2]->setAbsDestinationX(0.0f, 1000.0f, 2.0f, 0.0f, 0.0f);
				m_quitGameElements[3]->setAbsDestinationX(250.0f, 1000.0f, 2.0f, 0.0f, 0.0f);
				m_quitGame = true;
			}
		}
		else
		{
			if (m_quitGame) // Yes
			{
				m_quitGameElements[3]->setPos(250.0f, -100.0f);
				m_quitGameElements[2]->setAbsDestinationX(-250.0f, 1000.0f, 2.0f, 0.0f, 0.0f);
				m_quitGameElements[3]->setAbsDestinationX(0.0f, 1000.0f, 2.0f, 0.0f, 0.0f);
				m_quitGame = false;
			}
			else
			{
				m_quitGameElements[2]->setPos(250.0f, -100.0f);
				m_quitGameElements[2]->setAbsDestinationX(0.0f, 1000.0f, 2.0f, 0.0f, 0.0f);
				m_quitGameElements[3]->setAbsDestinationX(-250.0f, 1000.0f, 2.0f, 0.0f, 0.0f);
				m_quitGame = true;
			}
		}
	}
}

void UserInterface::draw()
{
	D3D11_MAPPED_SUBRESOURCE mappedMemory;
	for (int i = NR_OF_ICON_ELEMENTS; i < m_nrOfPlayers + NR_OF_ICON_ELEMENTS; i++)
	{
		DX::getInstance()->getDeviceContext()->Map(*m_constantBufferColours->getConstantBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
		memcpy(mappedMemory.pData, &m_playerColours[i - NR_OF_ICON_ELEMENTS], sizeof(XMVECTOR));
		DX::getInstance()->getDeviceContext()->Unmap(*m_constantBufferColours->getConstantBuffer(), 0);

			DX::getInstance()->getDeviceContext()->PSSetConstantBuffers(0, 1, m_constantBufferColours->getConstantBuffer());
			m_elements[i]->draw();
		
		
	}
	DX::getInstance()->getDeviceContext()->Map(*m_constantBufferColours->getConstantBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, &XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), sizeof(XMVECTOR));
	DX::getInstance()->getDeviceContext()->Unmap(*m_constantBufferColours->getConstantBuffer(), 0);
	for (int i = 0; i < m_countDownElements.size(); i++)
	{
		if (m_countDownElements[i]->isDrawn())
			m_countDownElements[i]->draw();
	}
}

void UserInterface::drawAbility(int playerIndex, int abilityType, float cd)
{
	int reduction = 0;
	for (int i = 0; i < playerIndex; i++)
	{
		if (!m_drawPlayer[i]) reduction++;
	}
	playerIndex -= reduction;
	D3D11_MAPPED_SUBRESOURCE mappedMemory;
	int elementIndex = playerIndex * 8 + abilityType;

	DX::getInstance()->getDeviceContext()->Map(*m_constantBufferColours->getConstantBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, &XMVectorSet(0.0f, 0.0f, 0.0f, cd), sizeof(XMVECTOR));
	DX::getInstance()->getDeviceContext()->Unmap(*m_constantBufferColours->getConstantBuffer(), 0);

	DX::getInstance()->getDeviceContext()->PSSetConstantBuffers(0, 1, m_constantBufferColours->getConstantBuffer());
	m_elements[elementIndex]->draw();
}

void UserInterface::drawHealthbar(int playerIndex, float hp)
{
	int reduction = 0;
	for (int i = 0; i < playerIndex; i++)
	{
		if (!m_drawPlayer[i]) reduction++;
	}
	playerIndex -= reduction;
	D3D11_MAPPED_SUBRESOURCE mappedMemory;

	DX::getInstance()->getDeviceContext()->Map(*m_constantBufferColours->getConstantBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, &XMVectorSet(m_playerColours[playerIndex].m128_f32[0], m_playerColours[playerIndex].m128_f32[1], m_playerColours[playerIndex].m128_f32[2], hp), sizeof(XMVECTOR));
	DX::getInstance()->getDeviceContext()->Unmap(*m_constantBufferColours->getConstantBuffer(), 0);

	DX::getInstance()->getDeviceContext()->PSSetConstantBuffers(0, 1, m_constantBufferColours->getConstantBuffer());
	m_healthBarElements[playerIndex]->draw();
}

void UserInterface::drawQuitGame()
{
	m_quitGameElements[0]->setDrawn(true);
	m_quitGameElements[1]->setDrawn(true);
	m_quitGameElements[2]->setDrawn(true);
	m_quitGameElements[3]->setDrawn(true);
	//if (m_quitGame)
	//else
	
	D3D11_MAPPED_SUBRESOURCE mappedMemory;
	DX::getInstance()->getDeviceContext()->Map(*m_constantBufferColours->getConstantBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, &XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), sizeof(XMVECTOR));
	DX::getInstance()->getDeviceContext()->Unmap(*m_constantBufferColours->getConstantBuffer(), 0);

	DX::getInstance()->getDeviceContext()->PSSetConstantBuffers(0, 1, m_constantBufferColours->getConstantBuffer());
	
	m_quitGameElements[0]->draw();
	m_quitGameElements[1]->draw();

	DX::getInstance()->getDeviceContext()->Map(*m_constantBufferColours->getConstantBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, &XMVectorSet(DX::getInstance()->getWidth(), 0.0f, 0.0f, -1.0f), sizeof(XMVECTOR));
	DX::getInstance()->getDeviceContext()->Unmap(*m_constantBufferColours->getConstantBuffer(), 0);

	m_quitGameElements[2]->draw();
	m_quitGameElements[3]->draw();
}

bool UserInterface::getQuitGame()
{
	return m_quitGame;
}
