#include "MainMenuState.h"

MainMenuState::MainMenuState()
{
	m_type = stateType::e_mainMenu;
	m_menuState = MenuState::e_mainMenu;
	m_activeMenu = ActiveMainMenu::e_startGame;

	m_selectionTimer = 0.0f;

	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\menu_background3.png", true, 0.0f, 0.0f, 1920.0f, 1080.0f));
	//MAIN MENU
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\menu_selection.png", true, 0.0f, 30.0f, 844.0f, 67.0f));
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\MainMenuAnimation\\box_ani_sprite3.png", true, 0.0f, 33.0f, 2960.0f, 1080.0f, 740.0f, 170.0f, 19));
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\MainMenuAnimation\\box_ani_sprite3.png", true, 0.0f, -140.0f, 2960.0f, 1080.0f, 740.0f, 170.0f, 19));
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\MainMenuAnimation\\box_ani_sprite3.png", true, 0.0f, -309.0f, 2960.0f, 1080.0f, 740.0f, 170.0f, 19));
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\menu_startGame.png", true, 0.0f, 33.0f, 666.0f, 66.0f));
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\menu_options.png", true, 0.0f, -140.0f, 416.0f, 66.0f));
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\menu_quit.png", true, 0.0f, -309.0f, 221.0f, 66.0f));

	//ROBOT SELECTION
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\selection_enterBattle.png", false, 0.0f, 100.0f, 748.0f, 66.0f));
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\selection_pressA.png", false, -696.0f, -400.0f, 297.0f, 47.0f));
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\selection_pressA.png", false, -232.0f, -400.0f, 297.0f, 47.0f));
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\selection_pressA.png", false, 232.0f, -400.0f, 297.0f, 47.0f));
	m_uiElements.push_back(new UI_Element(L"Textures\\MainMenu\\selection_pressA.png", false, 696.0f, -400.0f, 297.0f, 47.0f));

	m_uiElements[2]->setAnimated(true);
}

MainMenuState::~MainMenuState()
{
	for (int i = 0; i < m_uiElements.size(); i++)
	{
		delete m_uiElements[i];
	}
}

void MainMenuState::hi_mainMenu(Game* game)
{
	if (game->getInput()->isPressed(0, XINPUT_GAMEPAD_A) && m_activeMenu == ActiveMainMenu::e_startGame)
	{
		game->getInput()->setBlocked(true);
		m_menuState = MenuState::e_robotSelection;
		
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
			//m_uiElements[i]->setDrawn(true);
			m_uiElements[i]->fadeIn(1.0f, 1.0f);
		}
		//m_uiElements[8]->setDestinationX(2000.0f, 1500.0f, 2000.0f, 1.0f, 0.2f);

		return;
	}

	if (game->getInput()->getThumbLY(0) > -0.2f && game->getInput()->getThumbLY(0) < 0.2f) // Set input to ready if no input is detected
	{
		m_uiElements[1]->setSelectionTimer(0.0f);
		m_uiElements[1]->setReady(true);
		game->getInput()->setBlocked(false);
	}

	if (!game->getInput()->isBlocked()) // Check for inputs if not blocked
	{
		if (game->getInput()->getThumbLY(0) < -0.2f)
		{
			if (m_uiElements[1]->isReady()) // Is element ready
			{
				if (m_activeMenu == ActiveMainMenu::e_startGame)
				{
					m_uiElements[1]->setDestinationY(-168.0f, SELECTIONSPEED, 1.0f, 0.0f, 0.2f);
					m_activeMenu = ActiveMainMenu::e_options;
					m_uiElements[2]->setAnimated(false);
					m_uiElements[3]->setAnimated(true);
				}
				else if (m_activeMenu == ActiveMainMenu::e_options)
				{
					m_uiElements[1]->setDestinationY(-168.0f, SELECTIONSPEED, 1.0f, 0.0f, 0.2f);
					m_activeMenu = ActiveMainMenu::e_quit;
					m_uiElements[3]->setAnimated(false);
					m_uiElements[4]->setAnimated(true);
				}
			}
		}
		else if (game->getInput()->getThumbLY(0) > 0.2f)
		{
			if (m_uiElements[1]->isReady())
			{
				if (m_activeMenu == ActiveMainMenu::e_options)
				{
					m_uiElements[1]->setDestinationY(168.0f, SELECTIONSPEED, 1.0f, 0.0f, 0.2f);
					m_activeMenu = ActiveMainMenu::e_startGame;
					m_uiElements[3]->setAnimated(false);
					m_uiElements[2]->setAnimated(true);
				}
				else if (m_activeMenu == ActiveMainMenu::e_quit)
				{
					m_uiElements[1]->setDestinationY(168.0f, SELECTIONSPEED, 1.0f, 0.0f, 0.2f);
					m_activeMenu = ActiveMainMenu::e_options;
					m_uiElements[4]->setAnimated(false);
					m_uiElements[3]->setAnimated(true);
				}
			}
		}
	}
}

void MainMenuState::hi_robotSelection(Game* game)
{
	if (!game->getInput()->isPressed(0, XINPUT_GAMEPAD_A))
		game->getInput()->setBlocked(false);

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

	if (game->getInput()->isPressed(0, XINPUT_GAMEPAD_A) && !game->getInput()->isBlocked())
	{
		delete game->getRobots()[0];
		game->getRobots()[0] = new Robot(0);
		game->getRobots()[0]->setPosition(0.0f, -6.0f, 0.0f);
	}
}

void MainMenuState::hi_options(Game* game)
{
}

void MainMenuState::pause()
{
}

void MainMenuState::resume()
{
}

void MainMenuState::handleInput(Game* game)
{
	game->getInput()->refresh(0);

	switch (m_menuState)
	{
	case e_mainMenu:
		hi_mainMenu(game);
		break;
	case e_robotSelection:
		hi_robotSelection(game);
		break;
	case e_optionsMenu:
		break;
	default:
		break;
	}

}

void MainMenuState::update(Game* game, float dt)
{
	handleInput(game);
	m_uiElements[1]->updateElement(dt);
	//if(m_activeMenu == ActiveMainMenu::e_startGame)
	//	m_uiElements[2]->updateElement(AnimationType::e_sprite, dt);
	//if (m_activeMenu == ActiveMainMenu::e_options)
	//	m_uiElements[3]->updateElement(AnimationType::e_sprite, dt);
	//if (m_activeMenu == ActiveMainMenu::e_quit)
	//	m_uiElements[4]->updateElement(AnimationType::e_sprite, dt);

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
}

void MainMenuState::draw(Game* game, int index)
{
	if (index == 0)
	{
		for (int i = 0; i < m_uiElements.size(); i++)
		{
			m_uiElements[i]->draw();
		}
	}
	else
	{
		if (game->getRobots()[0] != nullptr)
			game->getPreLoader()->draw(objectType::e_robot, game->getRobots()[0]->getData());
	}
}
