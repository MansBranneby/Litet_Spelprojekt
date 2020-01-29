#include "Game.h"

void Game::handleMovement(float dt, int id)
{
	m_robots[id].setPosition(
		m_robots[id].getPosition() +
		XMVectorSet(input.getThumbLX(id), input.getThumbLY(id), 0.0f, 0.0f) *
		m_robots[id].getVelocity() * dt
	);
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
	m_nrOfPlayers = input.getNrOfGamepads();
	for (int i = 0; i < m_nrOfPlayers; i++)
	{
		Robot robot(i);
		m_robots.push_back(robot);
	}
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
	OutputDebugStringA("X: ");
	OutputDebugStringA(std::to_string(XMVectorGetX(m_robots[0].getPosition())).c_str());
	OutputDebugStringA(" Y: ");
	OutputDebugStringA(std::to_string(XMVectorGetY(m_robots[0].getPosition())).c_str());
	OutputDebugStringA("\n");
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
