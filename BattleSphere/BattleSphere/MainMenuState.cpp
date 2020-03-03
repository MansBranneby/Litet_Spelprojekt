#include "MainMenuState.h"

MainMenuState::MainMenuState()
{
	m_type = stateType::e_mainMenu;
	m_menuState = MenuState::e_mainMenu;
	m_activeMenu = ActiveMainMenu::e_startGame;

	m_availableColours[0] = 1;
	m_availableColours[1] = 1;
	m_availableColours[2] = 1;
	m_availableColours[3] = 1;
	m_availableColours[4] = 1;

	for (int i = 0; i < XUSER_MAX_COUNT; i++)
		m_readyState[i] = 0;

	m_selectionTimer = 0.0f;

	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\menu_background3.png", true, 0.0f, 0.0f, 1920.0f, 1080.0f));
	//MAIN MENU
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\menu_selection.png", true, 0.0f, 30.0f, 844.0f, 67.0f));
	
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\menu_box.png", true, 0.0f, 33.0f, 738.0f, 169.0f));
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\MainMenuAnimation\\box_ani_sprite3.png", true, 0.0f, -140.0f, 2960.0f, 1080.0f, 740.0f, 170.0f, 19));
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\MainMenuAnimation\\box_ani_sprite3.png", true, 0.0f, -309.0f, 2960.0f, 1080.0f, 740.0f, 170.0f, 19));
	
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\menu_startGame.png", true, 0.0f, 33.0f, 666.0f, 66.0f));
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\menu_options.png", true, 0.0f, -140.0f, 416.0f, 66.0f));
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\menu_quit.png", true, 0.0f, -309.0f, 221.0f, 66.0f));
	//7
	//ROBOT SELECTION
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\selection_enterBattle.png", false, 0.0f, 100.0f, 748.0f, 66.0f));

	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\selection_pressA.png", false, -696.0f, -400.0f, 297.0f, 47.0f));
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\selection_pressA.png", false, -232.0f, -400.0f, 297.0f, 47.0f));
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\selection_pressA.png", false, 232.0f, -400.0f, 297.0f, 47.0f));
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\selection_pressA.png", false, 696.0f, -400.0f, 297.0f, 47.0f));

	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\selection_readyA.png", false, -696.0f, -400.0f, 302.0f, 47.0f));
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\selection_readyA.png", false, -232.0f, -400.0f, 302.0f, 47.0f));
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\selection_readyA.png", false, 232.0f, -400.0f, 302.0f, 47.0f));
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\selection_readyA.png", false, 696.0f, -400.0f, 302.0f, 47.0f));

	m_uiElements[2]->setAnimated(true);
	Lights::getInstance()->addPointLight(0, 0, -10, 50, 1, 1, 1, 10);
}

MainMenuState::~MainMenuState()
{
	for (int i = 0; i < m_uiElements.size(); i++)
	{
		delete m_uiElements[i];
	}
}

bool MainMenuState::hi_mainMenu(Game* game)
{
	for (int j = 0; j < XUSER_MAX_COUNT; j++)
	{
		if (game->getInput()->isPressed(j, XINPUT_GAMEPAD_A) && m_activeMenu == ActiveMainMenu::e_quit)
			return 1;

		if (game->getInput()->getThumbLY(j) > -0.2f && game->getInput()->getThumbLY(j) < 0.2f) // Set input to ready if no input is detected
		{
			m_uiElements[1]->setSelectionTimer(0.0f);
			
			game->getInput()->setBlocked(j, false);
		}

		if (game->getInput()->isPressed(j, XINPUT_GAMEPAD_A) && m_activeMenu == ActiveMainMenu::e_startGame)
		{
			Sound::getInstance()->play(soundUI::e_front, 0.05f, -1.0f);
			game->getInput()->setBlocked(j, true);
			m_menuState = MenuState::e_robotSelection;
			game->updatePlayerStatus();

			for (int i = 0; i < XUSER_MAX_COUNT; i++)
			{
				if (game->getRobots()[i] != nullptr)
					game->getRobots()[i]->setDrawn(false);
			}


			//m_uiElements[1]->fadeOut(1.0f);
			//m_uiElements[2]->fadeOut(1.0f);
			//m_uiElements[3]->fadeOut(1.0f);
			//m_uiElements[4]->fadeOut(1.0f);
			//m_uiElements[5]->setDestinationX(2000.0f, 1500.0f, 2000.0f, 1.0f, 0.2f);
			//m_uiElements[6]->setDestinationX(2000.0f, 1500.0f, 2000.0f, 1.5f, 0.2f);
			//m_uiElements[7]->setDestinationX(2000.0f, 1500.0f, 2000.0f, 2.0f, 0.2f);
			/*m_uiElements[5]->fadeOut(1.0f);
			m_uiElements[6]->fadeOut(1.0f);
			m_uiElements[7]->fadeOut(1.0f);*/

			for (int i = 1; i < 8; i++) // Hide main menu
			{
				//m_uiElements[i]->setDrawn(false);
				m_uiElements[i]->fadeOut(0.5f, 0.0f);
			}
			for (int i = 8; i < 13; i++) // Show robot selection
			{
				m_uiElements[i]->setDrawn(true);
				m_uiElements[i]->fadeIn(1.0f, 1.0f);
			}
			//m_uiElements[8]->setDestinationX(2000.0f, 1500.0f, 2000.0f, 1.0f, 0.2f);
		}

		if (!game->getInput()->isBlocked(j)) // Check for inputs if not blocked
		{
			if (game->getInput()->getThumbLY(j) < -0.2f)
			{
				if (m_uiElements[1]->isReady()) // Is element ready
				{
					if (m_activeMenu == ActiveMainMenu::e_startGame)
					{
						Sound::getInstance()->play(soundUI::e_traverse, 0.4f);
						m_uiElements[1]->setDestinationY(-168.0f, SELECTIONSPEED, 1.0f, 0.0f, 0.2f);
						m_activeMenu = ActiveMainMenu::e_options;
						m_uiElements[2]->setAnimated(false);
						m_uiElements[3]->setAnimated(true);
					}
					else if (m_activeMenu == ActiveMainMenu::e_options)
					{
						Sound::getInstance()->play(soundUI::e_traverse, 0.4f);
						m_uiElements[1]->setDestinationY(-168.0f, SELECTIONSPEED, 1.0f, 0.0f, 0.2f);
						m_activeMenu = ActiveMainMenu::e_quit;
						m_uiElements[3]->setAnimated(false);
						m_uiElements[4]->setAnimated(true);
					}
				}
			}
			else if (game->getInput()->getThumbLY(j) > 0.2f)
			{
				if (m_uiElements[1]->isReady())
				{
					if (m_activeMenu == ActiveMainMenu::e_options)
					{
						Sound::getInstance()->play(soundUI::e_traverse, 0.4f);
						m_uiElements[1]->setDestinationY(168.0f, SELECTIONSPEED, 1.0f, 0.0f, 0.2f);
						m_activeMenu = ActiveMainMenu::e_startGame;
						m_uiElements[3]->setAnimated(false);
						m_uiElements[2]->setAnimated(true);
					}
					else if (m_activeMenu == ActiveMainMenu::e_quit)
					{
						Sound::getInstance()->play(soundUI::e_traverse, 0.4f);
						m_uiElements[1]->setDestinationY(168.0f, SELECTIONSPEED, 1.0f, 0.0f, 0.2f);
						m_activeMenu = ActiveMainMenu::e_options;
						m_uiElements[4]->setAnimated(false);
						m_uiElements[3]->setAnimated(true);
					}
				}
			}
		}
	}
	return 0;
}

void MainMenuState::hi_robotSelection(Game* game)
{
	game->updatePlayerStatus();

	int robotNr = -1;
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		robotNr = game->getPlayerIdIndex(i);

		if (!game->getInput()->isPressed(i, XINPUT_GAMEPAD_A) && !game->getInput()->isPressed(i, XINPUT_GAMEPAD_B) && game->getInput()->getThumbLX(i) < 0.4f && game->getInput()->getThumbLX(i) > -0.4f)
			game->getInput()->setBlocked(i, false);

		//PRESS A//
		if (game->getInput()->isPressed(i, XINPUT_GAMEPAD_A) && !game->getInput()->isBlocked(i))
		{
			Sound::getInstance()->play(soundUI::e_front, 0.05f, -1.0f);
			if (robotNr == -1)
				robotNr = game->setPlayerIdIndex(i);
			game->getInput()->setBlocked(i, true);
			
			switch (m_readyState[robotNr])
			{
			case 0:
				game->getRobots()[i]->setDrawn(true);
				m_uiElements[9 + robotNr]->setDrawn(false);
				m_uiElements[13 + robotNr]->setDrawn(true); // Press A -> Ready A
				m_uiElements[13 + robotNr]->fadeIn(0.1f, 0.0f);
				game->getRobots()[i]->setPosition((float)robotNr * 6.9f - 10.5f, -2.0f, 0.0f);
				changeColour(game, i, true);
				m_readyState[robotNr]++;
				break;
			case 1:
				m_uiElements[13 + robotNr]->setDrawn(false);
				m_readyState[robotNr]++;
				break;
			default:
				break;
			}
		}
		//PRESS B//
		if (game->getInput()->isPressed(i, XINPUT_GAMEPAD_B) && !game->getInput()->isBlocked(i) && game->getPlayerIdIndex(i) != -1)
		{
			Sound::getInstance()->play(soundUI::e_back, 0.05f, -1.0f);
			game->getInput()->setBlocked(i, true);

			switch (m_readyState[robotNr])
			{
			case 1:
				game->getRobots()[i]->setDrawn(false);
				m_uiElements[9 + robotNr]->setDrawn(true);
				m_uiElements[13 + robotNr]->setDrawn(false); // Press A -> Ready A
				m_readyState[robotNr]--;
				game->leavePlayerIdIndex(i);
				break;
			case 2:
				m_uiElements[13 + robotNr]->setDrawn(true);
				m_uiElements[13 + robotNr]->fadeIn(0.1f, 0.0f);
				m_readyState[robotNr]--;
				break;
			default:
				break;
			}
		}
		//Right
		if (game->getInput()->getThumbLX(i) > 0.4f && m_readyState[robotNr] == 1 && !game->getInput()->isBlocked(i) && game->getPlayerIdIndex(i) != -1)
		{
			Sound::getInstance()->play(soundUI::e_traverse, 0.3f, 0.5f);
			game->getInput()->setBlocked(i, true);
			changeColour(game, i, true);
		}
		//Left
		if (game->getInput()->getThumbLX(i) < -0.4f && m_readyState[robotNr] == 1 && !game->getInput()->isBlocked(i) && game->getPlayerIdIndex(i) != -1)
		{
			Sound::getInstance()->play(soundUI::e_traverse, 0.3f, 0.5f);
			game->getInput()->setBlocked(i, true);
			changeColour(game, i, false);
		}

	}

	for (int i = 9; i < 13; i++)
	{
		if (m_uiElements[i]->getAlpha() == 1.0f && m_uiElements[i]->isReady())
		{
			m_uiElements[i]->fadeOut(1.0f, 0.5f);
		}
		else if (m_uiElements[i]->getAlpha() == 0.0f && m_uiElements[i]->isReady())
		{
				m_uiElements[i]->fadeIn(0.5f, 0.0f);
		}
	}
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		if (game->getInput()->isPressed(i, XINPUT_GAMEPAD_B) && !game->getInput()->isBlocked(i) && game->getPlayerIdIndex(i) == -1) // To main menu
		{
			Sound::getInstance()->play(soundUI::e_back, 0.05f, -1.0f);
			game->getInput()->setBlocked(i, true);
			int nrOfPlayers = 0;
			for (int j = 0; j < XUSER_MAX_COUNT; j++)
			{
				if (game->getPlayerIdIndex(j) != -1)
					nrOfPlayers++;
			}
			if (!nrOfPlayers)
			{
				m_menuState = MenuState::e_mainMenu;

				for (int i = 1; i < 8; i++) // Hide main menu
				{
					//m_uiElements[i]->setDrawn(false);
					m_uiElements[i]->fadeIn(0.5f, 0.0f);
				}
				for (int i = 8; i < 17; i++) // Show robot selection
				{
					m_uiElements[i]->fadeOut(0.0f, 0.0f);
					m_uiElements[i]->setDrawn(false);
				}
			}
		}
	}
}

void MainMenuState::hi_options(Game* game)
{
}

void MainMenuState::u_mainMenu(Game* game, float dt)
{
	m_uiElements[2]->updateElement(dt);
	m_uiElements[3]->updateElement(dt);
	m_uiElements[4]->updateElement(dt);

	m_uiElements[5]->updateElement(dt);
	m_uiElements[6]->updateElement(dt);
	m_uiElements[7]->updateElement(dt);
}

void MainMenuState::u_robotSelection(Game* game, float dt)
{
	bool startGame = true;
	int nrOfPlayers = 0;

	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		if (game->getPlayerIdIndex(i) != -1)
		{
			nrOfPlayers++;
			if (m_readyState[game->getPlayerIdIndex(i)] != 2)
				startGame = false;
		}
	}
	if (startGame && nrOfPlayers > 0) // TODO: Change to nrOfPlayers > 0 for debug and testing
	{
		for (int i = 0; i < XUSER_MAX_COUNT; i++)
		{
			if (game->getRobots()[i] != nullptr)
			game->getRobots()[i]->setPosition(0.0f, 2.0f, 0.0f);
		}
		setPaused(true);
		game->changeState(stateType::e_gameState);
	}

	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		if (game->getRobots()[i] != nullptr)
		{
			game->getRobots()[i]->rotate(0.0f, 1.0f, 0.0f, dt * 20);
			game->getRobots()[i]->update(dt);
		}
			
	}

	m_uiElements[2]->updateElement(dt);
	m_uiElements[3]->updateElement(dt);
	m_uiElements[4]->updateElement(dt);

	m_uiElements[5]->updateElement(dt);
	m_uiElements[6]->updateElement(dt);
	m_uiElements[7]->updateElement(dt);

	m_uiElements[8]->updateElement(dt);
	m_uiElements[9]->updateElement(dt);
	m_uiElements[10]->updateElement(dt);
	m_uiElements[11]->updateElement(dt);
	m_uiElements[12]->updateElement(dt);

	m_uiElements[13]->updateElement(dt);
	m_uiElements[14]->updateElement(dt);
	m_uiElements[15]->updateElement(dt);
	m_uiElements[16]->updateElement(dt);
}

void MainMenuState::u_options(Game* game, float dt)
{
}

void MainMenuState::pause()
{
}

void MainMenuState::resume()
{
}

bool MainMenuState::handleInputs(Game* game, float dt)
{
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		game->getInput()->refresh(i, dt);
	}

	switch (m_menuState)
	{
	case e_mainMenu:
		return hi_mainMenu(game);
		break;
	case e_robotSelection:
		hi_robotSelection(game);
		break;
	case e_optionsMenu:
		break;
	default:
		break;
	}
	return 0;
}

void MainMenuState::changeColour(Game* game, int robotNr, bool dir)
{
	// Find colour
	int myColour = m_robotColour[robotNr];
	if (dir) // RIGHT
	{
		if (myColour == -1)
		{
			myColour = 0;
			for (int i = 0; i < 4; i++)
			{
				if (m_availableColours[(myColour + i) % 5] == 1)
				{
					m_availableColours[(myColour + i) % 5] = 0;
					m_robotColour[robotNr] = (myColour + i) % 5;
					break;
				}
			}
		}
		for (int i = 1; i < 5; i++)
		{
			if (m_availableColours[(myColour + i) % 5] == 1)
			{
				m_availableColours[myColour] = 1;
				m_availableColours[(myColour + i) % 5] = 0;
				m_robotColour[robotNr] = (myColour + i) % 5;
				break;
			}
		}
	} // LEFT
	else
	{
		if (myColour == -1)
		{
			myColour = 0;
			for (int i = 0; i > -4; i--)
			{
				int index = (myColour + i) % 5;
				if (index < 0)
					index = 5 + index;
				if (m_availableColours[index] == 1)
				{
					m_availableColours[index] = 0;
					m_robotColour[robotNr] = index;
					break;
				}
			}
		}
		for (int i = -1; i > -5; i--)
		{
			int index = (myColour + i) % 5;
			if (index < 0)
				index = 5 + index;
			if (m_availableColours[index] == 1)
			{
				m_availableColours[myColour] = 1;
				m_availableColours[index] = 0;
				m_robotColour[robotNr] = index;
				break;
			}
		}
	}
	// Set colour
	switch (m_robotColour[robotNr])
	{
	case 0:
		game->getRobots()[robotNr]->setColour(1.0f, 0.0f, 0.0f);
		break;
	case 1:
		game->getRobots()[robotNr]->setColour(0.0f, 1.0f, 0.0f);
		break;
	case 2:
		game->getRobots()[robotNr]->setColour(0.0f, 0.0f, 1.0f);
		break;
	case 3:
		game->getRobots()[robotNr]->setColour(0.0f, 1.0f, 1.0f);
		break;
	case 4:
		game->getRobots()[robotNr]->setColour(1.0f, 0.0f, 1.0f);
		break;
	default:
		break;
	}
	Graph::getInstance()->setColour(robotNr, game->getRobots()[robotNr]->getData().material.emission);
}

void MainMenuState::handleInput(Game* game)
{
}

bool MainMenuState::update(Game* game, float dt)
{
	if (handleInputs(game, dt))
		return 1;
	m_uiElements[1]->updateElement(dt);
	//if(m_activeMenu == ActiveMainMenu::e_startGame)
	//	m_uiElements[2]->updateElement(AnimationType::e_sprite, dt);
	//if (m_activeMenu == ActiveMainMenu::e_options)
	//	m_uiElements[3]->updateElement(AnimationType::e_sprite, dt);
	//if (m_activeMenu == ActiveMainMenu::e_quit)
	//	m_uiElements[4]->updateElement(AnimationType::e_sprite, dt);

	switch (m_menuState)
	{
	case e_mainMenu:
		u_mainMenu(game, dt);
		break;
	case e_robotSelection:
		u_robotSelection(game, dt);
		break;
	case e_optionsMenu:
		break;
	default:
		break;
	}
	return 0;
}

void MainMenuState::draw(Game* game, renderPass pass)
{
	if (pass == renderPass::e_menu)
	{
		for (int i = 0; i < m_uiElements.size(); i++)
		{
			if (m_uiElements[i]->isDrawn() && i != 2)
				m_uiElements[i]->draw();
		}
	}
	else if (pass == renderPass::e_menuAni)
	{
		DX::getInstance()->getDeviceContext()->PSSetConstantBuffers(0, 1, m_uiElements[2]->getConstantBuffer()->getConstantBuffer());
		m_uiElements[2]->draw();
	}
	else
	{
		for (int i = 0; i < XUSER_MAX_COUNT; i++)
		{
			if (game->getRobots()[i] != nullptr && game->getRobots()[i]->isDrawn())
			{
				
				game->getPreLoader()->setSubModelData(objectType::e_robot, game->getRobots()[i]->getData(), 1, 0);
				game->getPreLoader()->setSubModelData(objectType::e_robot, game->getRobots()[i]->getData(), 0, 6);

				game->getPreLoader()->draw(objectType::e_robot);
			}
		}
	}
}
