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

	m_readyState[0] = 0;
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
		m_readyState[i] = 0;

	m_selectionTimer = 0.0f;

	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\menu_background3.png", true, 0.0f, 0.0f, 1920.0f, 1080.0f));
	//MAIN MENU
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\menu_selection.png", true, 0.0f, 30.0f, 844.0f, 67.0f));

	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\menu_box.png", true, 0.0f, 33.0f, 738.0f, 169.0f));
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\menu_box.png", true, 0.0f, -140.0f, 738.0f, 169.0f));
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\menu_box.png", true, 0.0f, -309.0f, 738.0f, 169.0f));

	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\menu_startGame.png", true, 0.0f, 33.0f, 666.0f, 66.0f));
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\menu_options.png", true, 0.0f, -140.0f, 416.0f, 66.0f));
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\menu_quit.png", true, 0.0f, -309.0f, 221.0f, 66.0f));
	//7
	//ROBOT SELECTION
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\selection_enterBattle.png", false, 0.0f, 100.0f, 748.0f, 66.0f));
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\selection_addBot.png", false, 0.0f, 0.0f, 351.0f, 50.0f));

	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\selection_pressA.png", false, -696.0f, -400.0f, 302.0f, 50.0f));
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\selection_pressA.png", false, -232.0f, -400.0f, 302.0f, 50.0f));
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\selection_pressA.png", false, 232.0f, -400.0f, 302.0f, 50.0f));
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\selection_pressA.png", false, 696.0f, -400.0f, 302.0f, 50.0f));

	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\selection_readyA.png", false, -696.0f, -400.0f, 302.0f, 50.0f));
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\selection_readyA.png", false, -232.0f, -400.0f, 302.0f, 50.0f));
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\selection_readyA.png", false, 232.0f, -400.0f, 302.0f, 50.0f));
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\selection_readyA.png", false, 696.0f, -400.0f, 302.0f, 50.0f));


	m_botElements[0] = new UI_Element(L"Textures\\MainMenu\\BOT.png", false, -696.0f, -400.0f, 145.0f, 47.0f);
	m_botElements[1] = new UI_Element(L"Textures\\MainMenu\\BOT.png", false, -232.0f, -400.0f, 145.0f, 47.0f);
	m_botElements[2] = new UI_Element(L"Textures\\MainMenu\\BOT.png", false, 232.0f, -400.0f, 145.0f, 47.0f);
	m_botElements[3] = new UI_Element(L"Textures\\MainMenu\\BOT.png", false, 696.0f, -400.0f, 145.0f, 47.0f);
	m_startElement = new UI_Element(L"Textures\\MainMenu\\selection_pressStart.png", false, 0, 100, 688.0f, 66.0f);
	m_startElement->fadeIn(0.1f, 0.0f);
	//Lights::getInstance()->addPointLight(0, 0, -10, 50, 1, 1, 1, 10);
}

MainMenuState::~MainMenuState()
{
	for (int i = 0; i < m_uiElements.size(); i++)
	{
		delete m_uiElements[i];
	}
	for (int i = 0; i < 4; i++)
	{
		delete m_botElements[i];
	}
	delete m_startElement;
}

bool MainMenuState::hi_mainMenu(Game* game)
{
	for (int j = 0; j < XUSER_MAX_COUNT; j++)
	{
		if (game->getInput()->isPressed(j, XINPUT_GAMEPAD_A) && m_activeMenu == ActiveMainMenu::e_quit)
			return 1;

		if (game->getInput()->getThumbLY(j) > -0.2f && game->getInput()->getThumbLY(j) < 0.2f && !game->getInput()->isPressed(j, XINPUT_GAMEPAD_A)) // Set input to ready if no input is detected
		{
			//m_uiElements[1]->setSelectionTimer(0.0f);

			game->getInput()->setBlocked(j, false);
		}

		if (game->getInput()->isPressed(j, XINPUT_GAMEPAD_A) && !game->getInput()->isBlocked(j) && m_activeMenu == ActiveMainMenu::e_startGame)
		{
			Sound::getInstance()->play(soundUI::e_front, 0.05f, -1.0f);
			game->getInput()->setBlocked(j, true);
			m_menuState = MenuState::e_robotSelection;

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
			for (int i = 8; i < 14; i++) // Show robot selection
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
	int robotNr = -1;
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		robotNr = game->getPlayerIdIndex(i);
		int robNrPlus9 = robotNr + 10;
		int robNrPlus13 = robotNr + 14;
		if (!game->getInput()->isPressed(i, XINPUT_GAMEPAD_A) && !game->getInput()->isPressed(i, XINPUT_GAMEPAD_B) && game->getInput()->getThumbLX(i) < 0.4f && game->getInput()->getThumbLX(i) > -0.4f && !game->getInput()->isPressed(i, XINPUT_GAMEPAD_DPAD_LEFT) && !game->getInput()->isPressed(i, XINPUT_GAMEPAD_DPAD_RIGHT))
			game->getInput()->setBlocked(i, false);

		//PRESS A//
		if (game->getInput()->isPressed(i, XINPUT_GAMEPAD_A) && !game->getInput()->isBlocked(i))
		{

			game->getInput()->setBlocked(i, true);
			Sound::getInstance()->play(soundUI::e_front, 0.05f, -1.0f);
			int robIdx = i;
			if (robotNr == -1 || game->getRobots()[i]->isAi())
			{
				for (int j = 0; j < 4; j++)
				{
					robIdx = game->getPlayerId(j); // 0 i = 1
					if (robIdx == -1 || (game->getRobots()[robIdx] != nullptr && game->getRobots()[robIdx]->isAi()))
					{
						/*swap robot details*/
						int tempIdx = game->getPlayerIdIndex(i);
						if (tempIdx != -1 && i != j)
						{
							Robot* temp = game->getRobots()[j];
							//game->getRobots()[robIdx]->getRobotID();
							game->getRobots()[j] = game->getRobots()[i];
							game->getRobots()[i] = temp;
							changeColour(game, j, true);

							game->leavePlayerIdIndex(robIdx);
							game->leavePlayerIdIndex(i);
							robotNr = game->setPlayerIdIndex(i);
							if (tempIdx != -1)
							{
								tempIdx = game->setPlayerIdIndex(j);
								game->getRobots()[j]->setRobotID(tempIdx);
							}
						}
						else if (i == j)
						{
							game->getRobots()[i]->setAi(false);
							robotNr = game->getPlayerIdIndex(i);
							if (robotNr == -1)
							{
								robotNr = game->setPlayerIdIndex(i);
							}
							else if (robotNr == j)
							{
								game->getRobots()[i]->setAi(false);
								m_readyState[robotNr] = 0;
							}
							else
							{
								Robot* temp = game->getRobots()[robotNr];
								//game->getRobots()[robIdx]->getRobotID();
								game->getRobots()[robotNr] = game->getRobots()[i];
								game->getRobots()[i] = temp;
								changeColour(game, robotNr, true);
								game->leavePlayerIdIndex(robotNr);
								game->leavePlayerIdIndex(i);
								int a = robotNr;
								robotNr = game->setPlayerIdIndex(i);
								game->getRobots()[a]->setAi(true);
								game->getRobots()[a]->setDrawn(true);
								tempIdx = game->setPlayerIdIndex(a);
								game->getRobots()[a]->setRobotID(tempIdx);
							}
						}
						else
						{
							//Remove current bot
							if (robIdx != -1)
							{
								game->leavePlayerIdIndex(robIdx);
								game->getRobots()[robIdx]->setDrawn(false);
								game->getRobots()[robIdx]->setAi(false);
							}
							robotNr = game->setPlayerIdIndex(i);
						}

						robNrPlus9 = robotNr + 10;
						robNrPlus13 = robotNr + 14;

						m_readyState[robotNr] = 0;
						game->getRobots()[i]->setAi(false);
						//m_botElements[robotNr]->setDrawn(false);
						m_botElements[robotNr]->fadeOut(0.1f, 0.0f);
						break;
					}
				}
			}
			switch (m_readyState[robotNr])
			{
			case 0:
				game->getRobots()[i]->setDrawn(true);
				game->getRobots()[i]->setRobotID(robotNr);
				m_uiElements[robNrPlus9]->setDrawn(false);
				m_uiElements[robNrPlus13]->setDrawn(true); // Press A -> Ready A
				m_uiElements[robNrPlus13]->fadeIn(0.1f, 0.0f);
				game->getRobots()[i]->setPosition((float)robotNr * 6.9f - 10.5f, -2.0f, 0.0f);
				changeColour(game, i, true);
				m_readyState[robotNr]++;
				break;
			case 1:
				m_uiElements[robNrPlus13]->setDrawn(false);
				m_readyState[robotNr]++;
				break;
			default:
				break;
			}
		}
		//PRESS B//
		if (game->getInput()->isPressed(i, XINPUT_GAMEPAD_B) && !game->getInput()->isBlocked(i) && game->getPlayerIdIndex(i) != -1 && !game->getRobots()[i]->isAi())
		{
			Sound::getInstance()->play(soundUI::e_back, 0.05f, -1.0f);
			game->getInput()->setBlocked(i, true);

			switch (m_readyState[robotNr])
			{
			case 1:
				game->getRobots()[i]->setDrawn(false);
				game->getRobots()[i]->setRobotID(-1);
				m_uiElements[robNrPlus9]->setDrawn(true);
				m_uiElements[robNrPlus13]->setDrawn(false); // Press A -> Ready A
				m_readyState[robotNr]--;
				leaveColour(i);
				game->leavePlayerIdIndex(i);
				break;
			case 2:
				m_uiElements[robNrPlus13]->setDrawn(true);
				m_uiElements[robNrPlus13]->fadeIn(0.1f, 0.0f);
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

		//Add bots
		for (int i = 0; i < XUSER_MAX_COUNT; i++)
		{
			if (game->getInput()->isPressed(i, XINPUT_GAMEPAD_DPAD_RIGHT) && !game->getInput()->isBlocked(i))
			{
				game->getInput()->setBlocked(i, true);
				//Add new lol idk
				int robotNr = -1;
				int robNrPlus9 = -1;
				int robNrPlus13 = -1;
				int k;
				for (int j = 0; j < XUSER_MAX_COUNT && robotNr == -1; j++)
				{
					if (game->getPlayerIdIndex(j) == -1)
					{
						robotNr = game->setPlayerIdIndex(j);
						k = j;
						break;
					}
				}

				if (robotNr != -1)
				{
					robNrPlus9 = robotNr + 10;
					robNrPlus13 = robotNr + 14;
					if (game->getRobots()[k] == nullptr)
					{
						game->getRobots()[k] = new Robot(k);
					}
					game->getRobots()[k]->setDrawn(true);
					game->getRobots()[k]->setAi(true);
					game->getRobots()[k]->setRobotID(robotNr);
					m_uiElements[robNrPlus9]->setDrawn(false);
					m_uiElements[robNrPlus13]->setDrawn(false); // Press A -> Ready A
					m_uiElements[robNrPlus13]->fadeIn(0.1f, 0.0f);
					m_botElements[robotNr]->setDrawn(true);
					m_botElements[robotNr]->fadeIn(0.1f, 0.0f);
					game->getRobots()[k]->setPosition((float)robotNr * 6.9f - 10.5f, -2.0f, 0.0f);
					changeColour(game, k, true);
					m_readyState[robotNr] = 2;
				}

			}
			if (game->getInput()->isPressed(i, XINPUT_GAMEPAD_DPAD_LEFT) && !game->getInput()->isBlocked(i))
			{
				game->getInput()->setBlocked(i, true);
				int robotNr = -1;
				int robNrPlus9 = -1;
				int robNrPlus13 = -1;
				int k = -1;
				for (int j = XUSER_MAX_COUNT - 1; j >= 0 && k == -1; j--)
				{
					robotNr = game->getPlayerIdIndex(j);
					if (robotNr != -1 && game->getRobots()[j]->isAi())
					{
						game->leavePlayerIdIndex(j);
						k = j;
						break;
					}
				}

				if (k != -1)
				{
					robNrPlus9 = robotNr + 10;
					robNrPlus13 = robotNr + 14;
					game->getRobots()[k]->setDrawn(false);
					leaveColour(k);
					game->getRobots()[k]->setAi(false);
					game->getRobots()[k]->setRobotID(-1);
					m_uiElements[robNrPlus9]->setDrawn(true);
					m_uiElements[robNrPlus13]->setDrawn(false); // Press A -> Ready A
					m_readyState[robotNr] = 0;
					//m_botElements[robotNr]->setDrawn(false);
					m_botElements[robotNr]->fadeOut(0.1f, 0.0f);
				}
			}
		}

		for (int i = 10; i < 14; i++)
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
					for (int i = 8; i < 18; i++) // Show robot selection
					{
						m_uiElements[i]->fadeOut(0.0f, 0.0f);
						m_uiElements[i]->setDrawn(false);
					}
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
	bool startGame = false;
	bool start = true;
	int nrOfPlayers = 0;
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		if (game->getInput()->isPressed(i, XINPUT_GAMEPAD_START))
		{
			game->getInput()->setBlocked(i, true);
			startGame = true;
		}
	}

	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		if (game->getPlayerIdIndex(i) != -1)
		{
			nrOfPlayers++;
			if (m_readyState[game->getPlayerIdIndex(i)] != 2)
			{
				startGame = false;
				start = false;
			}
		}
	}
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		if (game->getRobots()[i] != nullptr)
		{
			game->getRobots()[i]->rotate(0.0f, 1.0f, 0.0f, dt * 20);
			XMVECTOR x, y; // Just here for the dumb call
			game->getRobots()[i]->update(dt, game->getQuadtree(), x, y);
		}
	}
	if (start && nrOfPlayers > 1)
	{
		m_startElement->setDrawn(true);
		m_uiElements[8]->setDrawn(false);
	}
	else
	{
		m_startElement->setDrawn(false);
		m_uiElements[8]->setDrawn(true);
	}
	if (startGame && nrOfPlayers > 0) // TODO: Change to nrOfPlayers > 0 for debug and testing
	{
		for (int i = 0; i < XUSER_MAX_COUNT; i++)
		{
			if (game->getRobots()[i] != nullptr)
			{
				game->getRobots()[i]->setPosition(ROBOT_START_POS[game->getRobots()[i]->getRobotID()]);
				game->getRobots()[i]->storePositionInHistory(ROBOT_START_POS[game->getRobots()[i]->getRobotID()]);
			}
		}
		//setPaused(true);
		game->changeState(stateType::e_gameState);
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
	m_uiElements[17]->updateElement(dt);

	for (int i = 0; i < 4; i++)
	{
		m_botElements[i]->updateElement(dt);
	}
	m_startElement->updateElement(dt);
	//DX::getInstance()->getParticles()->update(dt);
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

void MainMenuState::reset()
{
	m_menuState = MenuState::e_mainMenu;
	m_activeMenu = ActiveMainMenu::e_startGame;

	m_availableColours[0] = 1;
	m_availableColours[1] = 1;
	m_availableColours[2] = 1;
	m_availableColours[3] = 1;
	m_availableColours[4] = 1;

	m_readyState[0] = 0;
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
		m_readyState[i] = 0;

	m_selectionTimer = 0.0f;

	m_uiElements[2]->setAnimated(true);
}

bool MainMenuState::handleInputs(Game* game, float dt)
{
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		game->getInput()->refresh(i, dt);
	}

	switch (m_menuState)
	{
	case MenuState::e_mainMenu:
		return hi_mainMenu(game);
		break;
	case MenuState::e_robotSelection:
		hi_robotSelection(game);
		break;
	case MenuState::e_optionsMenu:
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
		else
		{
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
		else
		{
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

	}
	// Set colour
	switch (m_robotColour[robotNr])
	{
	case 0:
		game->getRobots()[robotNr]->setColour(0.5f, 1.0f, 0.5f);
		break;
	case 1:
		game->getRobots()[robotNr]->setColour(1.0f, 0.5f, 0.0f);
		break;
	case 2:
		game->getRobots()[robotNr]->setColour(1.0f, 0.8f, 1.0f);
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

void MainMenuState::firstTimeSetUp(Game* game)
{
	// Start at main menu
	m_menuState = MenuState::e_mainMenu;
	m_activeMenu = ActiveMainMenu::e_startGame;

	// SET UP ELEMENTS //

	for (int i = 0; i < 8; i++) // Main menu
	{
		m_uiElements[i]->setDrawn(true);
		if (i != 0)
			m_uiElements[i]->fadeIn(1.0f, 0.0f);
	}
	for (int i = 3; i < 5; i++)
		m_uiElements[i]->setAnimated(false);
	for (int i = 8; i < 17; i++) // Robot selection
	{
		m_uiElements[i]->setDrawn(false);
		//if (i != 0)
			//m_uiElements[i]->fadeIn(1.0f, 0.0f); // Fade out?
	}
	for (int i = 0; i < 4; i++)
	{
		m_botElements[i]->setDrawn(false);
	}
	m_startElement->setDrawn(false);
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
		game->getInput()->setBlocked(i, true);

	// SET UP LOGIC //
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
		m_readyState[i] = 0;
	for (int i = 0; i < 4; i++)
	{
		game->leavePlayerIdIndex(i);
	}
}
void MainMenuState::leaveColour(int robotNr)
{
	m_availableColours[m_robotColour[robotNr]] = 1;
	m_robotColour[robotNr] = -1;

}



void MainMenuState::handleInput(Game* game)
{
}

bool MainMenuState::update(Game* game, float dt)
{
	if (handleInputs(game, dt))
		return 1;
	game->updatePlayerStatus();
	m_uiElements[1]->updateElement(dt);
	//if(m_activeMenu == ActiveMainMenu::e_startGame)
	//	m_uiElements[2]->updateElement(AnimationType::e_sprite, dt);
	//if (m_activeMenu == ActiveMainMenu::e_options)
	//	m_uiElements[3]->updateElement(AnimationType::e_sprite, dt);
	//if (m_activeMenu == ActiveMainMenu::e_quit)
	//	m_uiElements[4]->updateElement(AnimationType::e_sprite, dt);

	switch (m_menuState)
	{
	case MenuState::e_mainMenu:
		u_mainMenu(game, dt);
		break;
	case MenuState::e_robotSelection:
		u_robotSelection(game, dt);
		break;
	case MenuState::e_optionsMenu:
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
			if (m_uiElements[i]->isDrawn() && i != 2 && i != 3 && i != 4)
				m_uiElements[i]->draw();
		}
		for (int i = 0; i < ARRAYSIZE(m_botElements); i++)
		{
			if (m_botElements[i]->isDrawn())
				m_botElements[i]->draw();
		}
		if (m_startElement->isDrawn())
			m_startElement->draw();

	}

	else if (pass == renderPass::e_menuAni)
	{
		DX::getInstance()->getDeviceContext()->PSSetConstantBuffers(0, 1, m_uiElements[2]->getConstantBuffer()->getConstantBuffer());
		m_uiElements[2]->draw();
		DX::getInstance()->getDeviceContext()->PSSetConstantBuffers(0, 1, m_uiElements[3]->getConstantBuffer()->getConstantBuffer());
		m_uiElements[3]->draw();
		DX::getInstance()->getDeviceContext()->PSSetConstantBuffers(0, 1, m_uiElements[4]->getConstantBuffer()->getConstantBuffer());
		m_uiElements[4]->draw();
	}
	else
	{
		for (int i = 0; i < XUSER_MAX_COUNT; i++)
		{
			if (game->getRobots()[i] != nullptr && game->getRobots()[i]->isDrawn())
			{

				game->getPreLoader()->setSubModelData(objectType::e_robot, game->getRobots()[i]->getData(), 0, 1);

				//game->getPreLoader()->setSubModelData(objectType::e_robot, game->getRobots()[i]->getData(), 0, 6);*/

				game->getPreLoader()->draw(objectType::e_robot);
			}
		}
	}

}
