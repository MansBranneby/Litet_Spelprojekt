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
		if (m_input.getId(i) != -1 && m_robots[i] == nullptr)
		{
			Robot* robot = new Robot(i);
			m_robots[i] = robot;
		}
	}
	m_nrOfPlayers = m_input.getNrOfGamepads();
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

Game::Game()
{
	m_nrOfPlayers = 0;
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		m_playerId[i] = -1;
		m_robots[i] = nullptr;
	}
	//updatePlayerStatus();

	objectData sceneData;
	sceneData.pos = XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);
	sceneData.rotation = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	sceneData.scale = XMVectorSet(0.6f, 0.6f, 0.6f, 0.6f);
	m_preLoader.setStaticData(objectType::e_scene, sceneData);
}

void Game::update(float dt)
{
	for (int i = 0; i < m_states.size(); i++)
	{
		if (!m_states[i]->isPaused())
			m_states[i]->update(this, dt);
	}
}

//void Game::updateSec()
//{
//	for (int i = 0; i < XUSER_MAX_COUNT; i++)
//	{
//		if (m_input.isPressed(i, XINPUT_GAMEPAD_A))
//		{
//			m_robots[i]->upgradeWeapon(RIFLE);
//		}
//	}
//}

void Game::draw(int index)
{
	for (int i = 0; i < m_states.size(); i++)
	{
		if (!m_states[i]->isPaused())
			m_states[i]->draw(this, index);
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
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		if (m_robots[i] != nullptr)
		{
			m_robots[i]->release();
			delete m_robots[i];
		}
	}
}
