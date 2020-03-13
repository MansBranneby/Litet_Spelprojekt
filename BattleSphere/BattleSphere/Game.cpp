#include "Game.h"
#include "GameState.h"
#include "MainMenuState.h"

int Game::setPlayerIdIndex(int id)
{
	int returnValue = -1;
	for (int i = 0; i < XUSER_MAX_COUNT && returnValue == -1; i++)
	{
		if (m_playerId[i] == -1)
		{
			m_playerId[i] = id;
			returnValue = i;
		}
	}
	return returnValue;
}

int Game::getPlayerIdIndex(int id)
{
	int returnValue = -1;
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		if (m_playerId[i] == id)
			returnValue = i;
	}
	return returnValue;
}

int Game::getNrOfPlayers() const
{
	return m_nrOfPlayers;
}

void Game::leavePlayerIdIndex(int id)
{
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		if (m_playerId[i] == id)
			m_playerId[i] = -1;
	}
}

void Game::updatePlayerStatus()
{
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		if (m_robots[i] == nullptr && m_input.getId(i) != -1)
		{
			Robot* robot = new Robot(i);
			robot->setPosition(XMVectorSet(-1000, 0, 0, 1));
			m_robots[i] = robot;
			m_nrOfPlayers++;
		}
	}
}

Robot** Game::getRobots()
{
	return m_robots;
}

Input* Game::getInput()
{
	return &m_input;
}

PreLoader* Game::getPreLoader()
{
	return &m_preLoader;
}

QuadtreeNode* Game::getQuadtree()
{
	return m_quadtree;
}

Game::Game()
{
	m_nrOfPlayers = 0;
	m_playerId[0] = -1;
	m_robots[0] = nullptr;
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		m_playerId[i] = -1;
		m_robots[i] = nullptr;
	}
	//updatePlayerStatus();

	//m_preLoader.cull(objectType::e_scene);
	// TODO: this ruins collision tests because we don't recalculate bounding volume data
	//objectData sceneData;
	//sceneData.pos = XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);
	//sceneData.rotation = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	//sceneData.scale = XMVectorSet(0.6f, 0.6f, 0.6f, 0.6f);
	//m_preLoader.setStaticData(objectType::e_scene, sceneData);
	m_quadtree = new QuadtreeNode(XMFLOAT3{ 0.0f, 0.0f, 0.0f }, XMFLOAT2{250.0f, 250.0f}, &m_preLoader, 2, 0);
	Graph::getInstance()->setQuadtree(m_quadtree);
}

bool Game::update(float dt)
{
	bool returnValue = true;
	for (int i = 0; i < m_states.size(); i++)
	{
		if (!m_states[i]->isPaused())
			returnValue = m_states[i]->update(this, dt);
	}
	return returnValue;
}

void Game::draw(renderPass pass)
{
	for (int i = 0; i < m_states.size(); i++)
	{
		if (!m_states[i]->isPaused())
			m_states[i]->draw(this, pass);
	}
}

void Game::pushState(State* state)
{
	m_states.push_back(state);
}

void Game::changeState(stateType state)
{
	if (state == stateType::e_mainMenu)
	{
		Sound::getInstance()->play(soundMusic::e_menu, 0.01f);
	}
	if (state == stateType::e_gameState)
	{
		Sound::getInstance()->stop(soundMusic::e_menu);
		Sound::getInstance()->play(soundMusic::e_game, 0.01f);
		Sound::getInstance()->play(soundAmbient::e_background, 0.05f);
	}
	
	if (isActive(stateType::e_mainMenu))
	{
		if (state == stateType::e_gameState)
		{
			for (int i = 0; i < m_states.size(); i++)
			{
				if (m_states[i]->getType() == stateType::e_mainMenu)
				{
					MainMenuState* s = dynamic_cast<MainMenuState*>(m_states[i]);
					if (s != nullptr)
						s->reset();
					m_states[i]->setPaused(true);
				}
			}
		}
	}

	if (isActive(stateType::e_gameState))
	{
		if (state == stateType::e_mainMenu)
		{
			for (int i = 0; i < m_states.size(); i++)
			{
				if (m_states[i]->getType() == stateType::e_gameState)
				{
					//delete m_states[i];
					//m_states.erase(m_states.begin() + i);
					//m_states.push_back(new GameState(this));
					m_states[i]->setPaused(true);
					Sound::getInstance()->stop(soundMusic::e_game);
					Sound::getInstance()->stop(soundAmbient::e_background);

					GameState* s = dynamic_cast<GameState*>(m_states[i]);
					if (s != nullptr) 
						s->reset();
					ProjectileBank::getInstance()->release();

					for (int j = 0; j < XUSER_MAX_COUNT; j++)
					{
						if (m_robots[j] != nullptr)
						{
							m_robots[j]->release();
							m_robots[j]->reset();

							//m_playerId[j] = -1;
							//m_robots[j] = nullptr;
						}
					}
					break;
				}
			}
		}
	}
	
	for (int i = 0; i < m_states.size(); i++)
	{
		if (m_states[i]->getType() == state)
		{
			m_states[i]->setPaused(false);
			m_states[i]->firstTimeSetUp(this);
		}
	}
}

bool Game::isActive(stateType state)
{
	for (int i = 0; i < m_states.size(); i++)
	{
		if (m_states[i]->getType() == state)
		{
			if (!m_states[i]->isPaused())
				return true;
		}
	}
	return false;
}

int Game::getPlayerId(int robotNr)
{
	return m_playerId[robotNr];
}

void Game::release()
{
	ProjectileBank::getInstance()->release();
	delete ProjectileBank::getInstance();

	Sound::getInstance()->release();
	delete Sound::getInstance();

	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		if (m_robots[i] != nullptr)
		{
			m_robots[i]->release();
			delete m_robots[i];
		}
	}

	for (int i = 0; i < m_states.size(); i++)
	{
		delete m_states[i];
	}

	delete m_quadtree;
}
