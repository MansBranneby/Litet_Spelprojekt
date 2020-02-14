#include "MainMenuState.h"

MainMenuState::MainMenuState()
{
	m_type = stateType::e_mainMenu;
	m_activeMenu = activeMainMenu::e_startGame;

	m_selectionTimer = 0.0f;

	m_uiElements.push_back(new UI_Element(L"Textures\\menu_background.png", uiType::e_static, 0.0f, 0.0f, 1920.0f, 1080.0f));
	m_uiElements.push_back(new UI_Element(L"Textures\\menu_selection.png", uiType::e_mainMenuSelection, 0.0f, 30.0f, 844.0f, 67.0f));
}

MainMenuState::~MainMenuState()
{
	for (int i = 0; i < m_uiElements.size(); i++)
	{
		delete m_uiElements[i];
	}
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

	if (game->getInput()->isPressed(0, XINPUT_GAMEPAD_A))
	{
		setPaused(true);
		game->changeState(stateType::e_gameState);
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
				if (m_activeMenu == activeMainMenu::e_startGame)
				{
					m_uiElements[1]->setDestinationY(-168.0f);
					m_activeMenu = activeMainMenu::e_options;
				}
				else if (m_activeMenu == activeMainMenu::e_options)
				{
					m_uiElements[1]->setDestinationY(-168.0f);
					m_activeMenu = activeMainMenu::e_quit;
				}
			}
		}
		else if (game->getInput()->getThumbLY(0) > 0.2f)
		{
			if (m_uiElements[1]->isReady())
			{
				if (m_activeMenu == activeMainMenu::e_options)
				{
					m_uiElements[1]->setDestinationY(168.0f);
					m_activeMenu = activeMainMenu::e_startGame;
				}
				else if (m_activeMenu == activeMainMenu::e_quit)
				{
					m_uiElements[1]->setDestinationY(168.0f);
					m_activeMenu = activeMainMenu::e_options;
				}
			}
		}
	}
}

void MainMenuState::update(Game* game, float dt)
{
	handleInput(game);
	for (int i = 0; i < m_uiElements.size(); i++)
	{
		m_uiElements[i]->updateElement(uiUpdate::e_mainMenuSelection, dt);
	}
}

void MainMenuState::draw(Game* game)
{
	for (int i = 0; i < m_uiElements.size(); i++)
	{
		m_uiElements[i]->draw();
	}
}
