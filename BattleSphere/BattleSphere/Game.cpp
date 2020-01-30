#include "Game.h"

void Game::handleMovement(float dt, int id)
{
	m_robots[id].move(
		XMVectorSet(input.getThumbLX(id), 0.0f, input.getThumbLY(id), 0.0f) *
		m_robots[id].getVelocity() * dt * (input.getTriggerR(id) + 0.2) * 8
	);
	m_robots[id].rotate(0, 1, 0, input.getThumbRX(id) * dt * 100);
}

void Game::handleInputs(float dt)
{
	for (int i = 0; i < m_nrOfPlayers; i++)
	{
		if (!input.refresh(i))
		{
			//OutputDebugStringA("Please reconnect controller\n");
			input.reconnectController(i);
		}
		else
		{
			handleMovement(dt, i);
			if (input.isPressed(i, XINPUT_GAMEPAD_A))
			{
				OutputDebugStringA("A\n");
			}
		}
	}
}

Game::Game()
{
	//m_nrOfPlayers = input.getNrOfGamepads();
	//for (int i = 0; i < m_nrOfPlayers; i++)
	//{
	//	Robot robot(i);
	//	m_robots.push_back(robot);
	//	m_robots[i].loadFromFile("BattleSphere");
	//}
}

void Game::update(float dt)
{
	handleInputs(dt);

	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		// TODO remove comment
		//m_gameObjects[i]->update(dt);
	}

	for (int i = 0; i < m_robots.size(); i++)
	{
		// TODO something
		m_robots[i].update();
	}

}

void Game::updateSec()
{
	if (m_nrOfPlayers != 0)
	{
		OutputDebugStringA("X: ");
		OutputDebugStringA(std::to_string(XMVectorGetX(m_robots[0].getPosition())).c_str());
		OutputDebugStringA(" Y: ");
		OutputDebugStringA(std::to_string(XMVectorGetZ(m_robots[0].getPosition())).c_str());
		OutputDebugStringA("\n");
	}
}

void Game::draw()
{
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		//m_gameObjects[i]->draw();
	}

	for (int i = 0; i < m_robots.size(); i++)
	{
		// TODO something
		m_robots[i].draw();
	}
}

void Game::release()
{
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		// TODO release gameobject
		//m_gameObjects[i].release();
	}
}
