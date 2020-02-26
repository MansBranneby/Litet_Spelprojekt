#include "Game.h"

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
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		m_playerId[i] = -1;
		m_robots[i] = nullptr;
	}
	//updatePlayerStatus();

	m_quadtree = new QuadtreeNode(XMFLOAT3{ 0.0f, 0.0f, 0.0f }, XMFLOAT2{100.0f, 100.0f}, &m_preLoader, 2, 0);
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
	for (int i = 0; i < m_states.size(); i++)
	{
		if (m_states[i]->getType() == state)
			m_states[i]->setPaused(false);
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

void Game::release()
{
	ProjectileBank::getInstance()->release();
	delete ProjectileBank::getInstance();

	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		if (m_robots[i] != nullptr)
		{
			m_robots[i]->release();
			delete m_robots[i];
		}
	}

	delete m_quadtree;
}
