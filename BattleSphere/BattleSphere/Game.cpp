#include "Game.h"

void Game::handleMovement(float dt, int id)
{
	// Robot movement
	m_robots[id]->move(
		XMVectorSet(m_input.getThumbLX(id), 0.0f, m_input.getThumbLY(id), 0.0f) *
		m_robots[id]->getVelocity() * dt * (m_input.getTriggerL(id) + 0.2f) * 8 // TODO remove trigger
	);
	
	//Weapon movement
	std::vector<Weapon*> weapons = m_robots[id]->getWeapons();

	weapons[m_robots[id]->getCurrentWeapon(RIGHT)]->setPosition(
		weapons[m_robots[id]->getCurrentWeapon(RIGHT)]->getRelativePos()
	);

	//m_robots[id]->getWeapons()[m_robots[id]->getCurrentWeapon(RIGHT)]->setPosition(
	//	m_robots[id]->getWeapons()[m_robots[id]->getCurrentWeapon(RIGHT)]->getRelativePos()
	//);

	//weapons[m_robots[id]->getCurrentWeapon(RIGHT)]->setPositionRelative(
	//	m_robots[id]->getPosition()
	//);

	//if (m_robots[id]->getCurrentWeapon(LEFT) != -1)
	//{
	//	weapons[m_robots[id]->getCurrentWeapon(LEFT)]->setPosition(
	//		m_robots[id]->getPosition()
	//	);
	//
	//	weapons[m_robots[id]->getCurrentWeapon(LEFT)]->setPositionRelative(
	//		weapons[m_robots[id]->getCurrentWeapon(LEFT)]->getRelativePos()
	//	);
	//}

	// Rotation
	if (abs(m_input.getThumbRX(id)) > 0.1f || abs(m_input.getThumbRY(id)) > 0.1f)
	{
		m_robots[id]->setCurrentRot(XMConvertToDegrees(atan2(m_input.getThumbRX(id), m_input.getThumbRY(id))));

		m_robots[id]->setRotation(0, 1, 0, m_robots[id]->getCurrentRot());
		
		//weapons[m_robots[id]->getCurrentWeapon(RIGHT)]->setRotation(0, 1, 0, m_robots[id]->getCurrentRot());
		//if (m_robots[id]->getCurrentWeapon(LEFT) != -1)
		//{
		//	weapons[m_robots[id]->getCurrentWeapon(LEFT)]->setRotation(0, 1, 0, m_robots[id]->getCurrentRot());
		//}
	}

	// Projectile movement
	for (int i = 0; i < m_projectiles.size(); i++)
	{
		m_projectiles[i]->move(m_projectiles[i]->getDirection() * dt);
	}
}

void Game::handleInputs(float dt)
{
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		if (m_robots[i] != nullptr)
		{
			if (!m_input.refresh(i))
			{
				//OutputDebugStringA("Please reconnect controller\n");
				m_input.reconnectController(i);
			}
			else
			{
				std::vector<Weapon*> weapons = m_robots[i]->getWeapons();

				handleMovement(dt, i);

				if (m_input.getTriggerR(i) > 0.2 && weapons[m_robots[i]->getCurrentWeapon(RIGHT)]->shoot())
				{

					Projectile* bullet = new Projectile(weapons[m_robots[i]->getCurrentWeapon(RIGHT)]->getType(), weapons[m_robots[i]->getCurrentWeapon(RIGHT)]->getDamage());

					float rotInRad = XMConvertToRadians(m_robots[i]->getCurrentRot());
					
					bullet->setPosition(
							XMVector3Rotate(
								XMVectorSet(
									XMVectorGetX(weapons[m_robots[i]->getCurrentWeapon(RIGHT)]->getRelativePos()),
									XMVectorGetY(weapons[m_robots[i]->getCurrentWeapon(RIGHT)]->getRelativePos()),
									0.0f, 0.0f),
								XMVectorSet(0, sin(rotInRad/2), 0, cos(rotInRad/2))
							) + m_robots[i]->getPosition()
						);

					bullet->setRotation(0.0, 1.0, 0.0f, m_robots[i]->getCurrentRot());

					// TODO add recoil here 
					bullet->setDirection(XMVector3Cross(bullet->getPosition() - m_robots[i]->getPosition(), XMVectorSet(0,1,0,0)));
					m_projectiles.push_back(bullet);
				}

				/*
				if (input.getTriggerL(i) > 0.2 && weapons[m_robots[i]->getCurrentWeapon(LEFT)]->shoot())
				{

					Projectile* bullet = new Projectile(weapons[m_robots[i]->getCurrentWeapon(LEFT)]->getType(), weapons[m_robots[i]->getCurrentWeapon(RIGHT)]->getDamage());

					float rotInRad = XMConvertToRadians(m_robots[i]->getCurrentRot());

					bullet->setPosition(
						XMVector3Rotate(
							XMVectorSet(
								XMVectorGetX(weapons[m_robots[i]->getCurrentWeapon(LEFT)]->getRelativePos()),
								XMVectorGetY(weapons[m_robots[i]->getCurrentWeapon(LEFT)]->getRelativePos()),
								0.0f, 0.0f),
							XMVectorSet(0, sin(rotInRad / 2), 0, cos(rotInRad / 2))
						) + m_robots[i]->getPosition()
					);

					bullet->setDirection(XMVector3Cross(bullet->getPosition() - m_robots[i]->getPosition(), XMVectorSet(0, 1, 0, 0)));
					m_projectiles.push_back(bullet);
				}
				*/
				//if (m_input.isPressed(i, XINPUT_GAMEPAD_A))
				//{
				//	OutputDebugStringA("A\n");
				//}
			}
		}
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

Game::Game()
{
	m_nrOfPlayers = 0;
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
		m_robots[i] = nullptr;
	updatePlayerStatus();

	objectData sceneData;
	sceneData.pos = XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);
	sceneData.rotation = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	sceneData.scale = XMVectorSet(0.6f, 0.6f, 0.6f, 0.6f);
	m_preLoader.setStaticData(objectType::e_scene, sceneData);
}

void Game::update(float dt)
{
	handleInputs(dt);
	updatePlayerStatus();

	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		if (m_robots[i] != nullptr)
		{
			m_robots[i]->update();

			std::vector<Weapon*> weapons = m_robots[i]->getWeapons();
			weapons[m_robots[i]->getCurrentWeapon(RIGHT)]->updateTime(dt);
			if (m_robots[i]->getCurrentWeapon(LEFT) != -1)
			{
				weapons[m_robots[i]->getCurrentWeapon(LEFT)]->updateTime(dt);
			}
		}
	}

	// TODO remove with collision instead aswell as game field?
	for (int i = 0; i < m_projectiles.size(); i++)
	{
		if (XMVectorGetX(XMVector3Length(m_projectiles[i]->getPosition())) > 50.0f)
		{
			delete m_projectiles[i];
			m_projectiles.erase(m_projectiles.begin()+i);
		}
	}
}

void Game::updateSec()
{
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		if (m_input.isPressed(i, XINPUT_GAMEPAD_A))
		{
			m_robots[i]->upgradeWeapon(RIFLE);
		}
	}
}

void Game::draw()
{
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		if (m_robots[i] != nullptr)
		{
			std::vector<Weapon*> weapons = m_robots[i]->getWeapons();

			m_preLoader.draw(objectType::e_robot, m_robots[i]->getData());
			m_preLoader.draw(objectType::e_weapon, weapons[m_robots[i]->getCurrentWeapon(RIGHT)]->getData(), m_robots[i]->getData());
			if (m_robots[i]->getCurrentWeapon(LEFT) != -1)
			{
				m_preLoader.draw(objectType::e_weapon, weapons[m_robots[i]->getCurrentWeapon(LEFT)]->getData(), m_robots[i]->getData());
			}
		}
	}

	for (int i = 0; i < m_projectiles.size(); i++)
	{
		m_preLoader.draw(objectType::e_projectile, m_projectiles[i]->getData());
	}
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

	for (int i = 0; i < m_projectiles.size(); i++)
	{
		delete m_projectiles[i];
	}
}
