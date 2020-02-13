#include "MainMenuState.h"

MainMenuState::MainMenuState()
{
	m_type = stateType::e_mainMenu;

	m_uiElements.push_back(new UI_Element(L"Textures\\pink.png", 0.0f, 0.0f, 700.0f, 100.0f));
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
}

void MainMenuState::update(Game* game, float dt)
{
}

void MainMenuState::draw(Game* game)
{
	for (int i = 0; i < m_uiElements.size(); i++)
	{
		m_uiElements[i]->draw();
	}
}
