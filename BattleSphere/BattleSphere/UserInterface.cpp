#include "UserInterface.h"

void UserInterface::setElementPos()
{
	for (int i = 0; i < 36; i++)
		m_elements[i]->setDrawn(false);
	for (int i = 0; i < m_nrOfPlayers; i++) // Iterate players
	{
		for (int j = 0; j < 4; j++) // Iterate icon slots
		{
			if (m_slotID[i * 4 + j] != -1)
			{
				m_elements[i * 9 + m_slotID[i * 4 + j]]->setPos(m_slotPos[i * 4 + j], -508.0f);
				m_elements[i * 9 + m_slotID[i * 4 + j]]->setDrawn(true);
			}
		}
	}
}

UserInterface::UserInterface(int nrOfPlayers)
{
	// PLAYER 1
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Rifle.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
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
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Rifle.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Movement.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Shield.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Dash.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Reflect.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Sniper.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Beyblade.png", true, -960.0f, -508.0f, 60.0f, 60.0f));
	m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\Energy.png", true, -960.0f, -508.0f, 60.0f, 60.0f));

	for (int i = 0; i < 16; i++) // No abilities selected or in queue
		m_slotID[i] = -1;
	//nrOfPlayers = 4;
	switch (nrOfPlayers)
	{
	case 1:
		m_elements.push_back(new UI_Element(L"Textures\\UserInterface\\PlayerBG.png", true, 0.0f, -(540.0f - PLAYER_BG_H / 2.0f), PLAYER_BG_W, PLAYER_BG_H));

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
	delete m_constantBufferColours;
}

void UserInterface::setPlayerColours(int playerIndex, XMVECTOR colour)
{
	m_playerColours[playerIndex] = colour;
}

void UserInterface::setSlotID(int playerIndex, int abilityType)
{
	for (int i = 0; i < 4; i++) // Iterate slots
	{
		if (m_slotID[playerIndex * 4 + i] == -1) // No ability on slot
		{
			m_slotID[playerIndex * 4 + i] = abilityType; // Set ability to current slot
			break;
		}
	}
}

void UserInterface::setSlotID(int playerIndex, int abilityType, int side, int newIndex)
{
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
		m_slotID[playerIndex * 4 + 2] = oldIndex; // Replace new with old
		break;
	case 4:
		m_slotID[playerIndex * 4 + 2] = m_slotID[playerIndex * 4 + 3];
		m_slotID[playerIndex * 4 + 3] = oldIndex;
		break;
	default: // newIndex != -1 and number of abilities > slots
		m_slotID[playerIndex * 4 + 2] = m_slotID[playerIndex * 4 + 3];
		m_slotID[playerIndex * 4 + 3] = newIndex;
		break;
	}
}

void UserInterface::setSlotID(int playerIndex, int abilityType, int side, int next, int nextNext)
{
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
	case 4:
		m_slotID[playerIndex * 4 + 2] = next;
		m_slotID[playerIndex * 4 + 3] = nextNext;
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

void UserInterface::draw()
{
	D3D11_MAPPED_SUBRESOURCE mappedMemory;
	for (int i = 36; i < m_nrOfPlayers + 36; i++)
	{
		DX::getInstance()->getDeviceContext()->Map(*m_constantBufferColours->getConstantBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
		memcpy(mappedMemory.pData, &m_playerColours[i - 36], sizeof(XMVECTOR));
		DX::getInstance()->getDeviceContext()->Unmap(*m_constantBufferColours->getConstantBuffer(), 0);

		DX::getInstance()->getDeviceContext()->PSSetConstantBuffers(0, 1, m_constantBufferColours->getConstantBuffer());
		m_elements[i]->draw();
	}
	//for (int i = 0; i < 36; i++)
	//{
	//	DX::getInstance()->getDeviceContext()->Map(*m_constantBufferColours->getConstantBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	//	memcpy(mappedMemory.pData, &XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), sizeof(XMVECTOR));
	//	DX::getInstance()->getDeviceContext()->Unmap(*m_constantBufferColours->getConstantBuffer(), 0);

	//	DX::getInstance()->getDeviceContext()->PSSetConstantBuffers(0, 1, m_constantBufferColours->getConstantBuffer());
	//	m_elements[i]->draw();
	//}
}

void UserInterface::drawAbility(int playerIndex, int abilityType, float cd)
{
	D3D11_MAPPED_SUBRESOURCE mappedMemory;
	int elementIndex = playerIndex * 9 + abilityType;

	DX::getInstance()->getDeviceContext()->Map(*m_constantBufferColours->getConstantBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, &XMVectorSet(0.0f, 0.0f, 0.0f, cd), sizeof(XMVECTOR));
	DX::getInstance()->getDeviceContext()->Unmap(*m_constantBufferColours->getConstantBuffer(), 0);

	DX::getInstance()->getDeviceContext()->PSSetConstantBuffers(0, 1, m_constantBufferColours->getConstantBuffer());
	m_elements[elementIndex]->draw();
}
