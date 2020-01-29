#include "Game.h"

Game::Game()
{
}

void Game::update(float dt)
{
	for (int i = 0; i < m_gameObjects.size(); i++) {
		// TODO remove comment
		//m_gameObjects[i]->update(dt);
	}
}

void Game::draw()
{
	for (int i = 0; i < m_gameObjects.size(); i++) {
		m_gameObjects[i].draw();
	}
}

void Game::release()
{
	for (int i = 0; i < m_gameObjects.size(); i++) {
		// TODO release gameobject
		//m_gameObjects[i].release();
	}
}
