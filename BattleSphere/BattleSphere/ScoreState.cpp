#include "ScoreState.h"

ScoreState::ScoreState()
{
	m_type = stateType::e_scoreState;
	Robot** m_robots;

	// Background
	m_uiElements.push_back(new UI_Element(L"Textures\\ScoreState\\ScoreScreen_background.png", true, 0.0f, 0.0f, 1920.0f, 1080.0f));
	
	// Texture for the numbers (one to nine) that represent players' scores
	m_uiElements.push_back(new UI_Element(L"Textures\\ScoreState\\Score_0.png", true, 0.0f, 0.0f, 200.0f, 200.0f));
	m_uiElements.push_back(new UI_Element(L"Textures\\ScoreState\\Score_1.png", true, 0.0f, 0.0f, 200.0f, 200.0f));
	m_uiElements.push_back(new UI_Element(L"Textures\\ScoreState\\Score_2.png", true, 0.0f, 0.0f, 200.0f, 200.0f));
	m_uiElements.push_back(new UI_Element(L"Textures\\ScoreState\\Score_3.png", true, 0.0f, 0.0f, 200.0f, 200.0f));
	m_uiElements.push_back(new UI_Element(L"Textures\\ScoreState\\Score_4.png", true, 0.0f, 0.0f, 200.0f, 200.0f));
	m_uiElements.push_back(new UI_Element(L"Textures\\ScoreState\\Score_5.png", true, 0.0f, 0.0f, 200.0f, 200.0f));
	m_uiElements.push_back(new UI_Element(L"Textures\\ScoreState\\Score_6.png", true, 0.0f, 0.0f, 200.0f, 200.0f));
	m_uiElements.push_back(new UI_Element(L"Textures\\ScoreState\\Score_7.png", true, 0.0f, 0.0f, 200.0f, 200.0f));
	m_uiElements.push_back(new UI_Element(L"Textures\\ScoreState\\Score_8.png", true, 0.0f, 0.0f, 200.0f, 200.0f));
	m_uiElements.push_back(new UI_Element(L"Textures\\ScoreState\\Score_9.png", true, 0.0f, 0.0f, 200.0f, 200.0f));


	Lights::getInstance()->addPointLight(-10, 0, -10, 50, 0.6, 0.0, 0.6, 5.0);
}

ScoreState::~ScoreState()
{
	for (int i = 0; i < m_uiElements.size(); i++)
		delete m_uiElements[i];
	
}

void ScoreState::pause()
{
}

void ScoreState::resume()
{
}

void ScoreState::handleInput(Game* game)
{
}

bool ScoreState::update(Game* game, float dt)
{
	return false;
}

void ScoreState::draw(Game* game, renderPass pass)
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
	}
	else
	{
		/*for (int i = 0; i < XUSER_MAX_COUNT; i++)
		{
			if (game->getRobots()[i] != nullptr && game->getRobots()[i]->isDrawn())
			{

				game->getPreLoader()->setSubModelData(ObjectType::e_robot, game->getRobots()[i]->getData(), 1, 0);
				game->getPreLoader()->setSubModelData(ObjectType::e_robot, game->getRobots()[i]->getData(), 0, 6);

				game->getPreLoader()->draw(ObjectType::e_robot);
			}
		}*/
	}
}
