#include "ScoreState.h"

ScoreState::ScoreState()
{
	m_type = stateType::e_scoreState;

	// Background
	m_uiElements.push_back(new UI_Element(L"Textures\\ScoreState\\ScoreScreen_background.png", true, 0.0f, 0.0f, 1920.0f, 1080.0f));
	
	// Texture for the numbers (one to nine) that represent players' scores
	float scoreNumberYPos = -40.0f;
	m_uiElements_numbers.push_back(new UI_Element(L"Textures\\ScoreState\\Score_0.png", true, 0.0f, scoreNumberYPos, 200.0f, 200.0f));
	m_uiElements_numbers.push_back(new UI_Element(L"Textures\\ScoreState\\Score_1.png", true, 0.0f, scoreNumberYPos, 200.0f, 200.0f));
	m_uiElements_numbers.push_back(new UI_Element(L"Textures\\ScoreState\\Score_2.png", true, 0.0f, scoreNumberYPos, 200.0f, 200.0f));
	m_uiElements_numbers.push_back(new UI_Element(L"Textures\\ScoreState\\Score_3.png", true, 0.0f, scoreNumberYPos, 200.0f, 200.0f));
	m_uiElements_numbers.push_back(new UI_Element(L"Textures\\ScoreState\\Score_4.png", true, 0.0f, scoreNumberYPos, 200.0f, 200.0f));
	m_uiElements_numbers.push_back(new UI_Element(L"Textures\\ScoreState\\Score_5.png", true, 0.0f, scoreNumberYPos, 200.0f, 200.0f));
	m_uiElements_numbers.push_back(new UI_Element(L"Textures\\ScoreState\\Score_6.png", true, 0.0f, scoreNumberYPos, 200.0f, 200.0f));
	m_uiElements_numbers.push_back(new UI_Element(L"Textures\\ScoreState\\Score_7.png", true, 0.0f, scoreNumberYPos, 200.0f, 200.0f));
	m_uiElements_numbers.push_back(new UI_Element(L"Textures\\ScoreState\\Score_8.png", true, 0.0f, scoreNumberYPos, 200.0f, 200.0f));
	m_uiElements_numbers.push_back(new UI_Element(L"Textures\\ScoreState\\Score_9.png", true, 0.0f, scoreNumberYPos, 200.0f, 200.0f));


	Lights::getInstance()->addPointLight(-10.0f, 0.0f, -10.0f, 50.0f, 0.6f, 0.0f, 0.6f, 0.5f);
}

ScoreState::~ScoreState()
{
	for (int i = 0; i < m_uiElements.size(); i++)
		delete m_uiElements[i];
	
	for (int i = 0; i < m_uiElements_numbers.size(); i++)
		delete m_uiElements_numbers[i];
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
	int nrOfPlayers = game->getNrOfPlayers();
	for (int i = 0; i < XUSER_MAX_COUNT; ++i)
	{
		if (game->getRobots()[i] != nullptr)
		{
			
		}
	}


	return false;
}

void ScoreState::draw(Game* game, renderPass pass)
{
	if (pass == renderPass::e_menu)
	{
		// Draw background and text
		for (int i = 0; i < m_uiElements.size(); i++)
		{
			if (m_uiElements[i]->isDrawn())
				m_uiElements[i]->draw();
		}
		// Draw each player's score using number-textures (1 through 9)
		int score = 0;
		for (int i = 0; i < XUSER_MAX_COUNT; ++i)
		{
			if (game->getRobots()[i] != nullptr)
			{
				// Store score for this player	
				score = game->getRobots()[i]->getScore(); 
				while (score > 0) // Extract digits while score is greater than zero
				{
					// Extract each digit with modulus 10
					// Example: 123 % 10 would give digits in the following order 3, 2 and 1
					m_uiElements_numbers[score % 10]->setDrawn(true);
					m_uiElements_numbers[score % 10]->draw();
					score /= 10; // 123 / 10 = 12 (rounds down) 
				} 		
			}
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
