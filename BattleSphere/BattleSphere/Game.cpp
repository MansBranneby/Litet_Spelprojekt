#include "Game.h"

void Game::handleMovement(float dt, int id)
{
	// Robot movement
	m_robots[id]->move(
		XMVectorSet(m_input.getThumbLX(id), 0.0f, m_input.getThumbLY(id), 0.0f) *
		m_robots[id]->getVelocity() * dt * ((m_input.isPressed(id, XINPUT_GAMEPAD_Y)) ? 2.0f : 1.0f) // TODO remove trigger
	);
	
	//Weapon movement
	std::vector<Weapon*> weapons = m_robots[id]->getWeapons();

	weapons[m_robots[id]->getCurrentWeapon(RIGHT)]->setPosition(
		weapons[m_robots[id]->getCurrentWeapon(RIGHT)]->getRelativePos()
	);

	if (m_robots[id]->getCurrentWeapon(LEFT) != -1)
	{
		weapons[m_robots[id]->getCurrentWeapon(LEFT)]->setPosition(
			weapons[m_robots[id]->getCurrentWeapon(LEFT)]->getRelativePos()
		);
	}

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

	// Resource movement
	if (m_robots[id]->getResourceIndex() != -1)
		m_resources[m_robots[id]->getResourceIndex()]->setPosition(m_robots[id]->getPosition() + XMVectorSet(0.0f, 1.5f, 0.0f, 0.0f));
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

				// Shoot
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
				if (m_robots[i]->getCurrentWeapon(LEFT) != -1)
				{
					if (m_input.getTriggerL(i) > 0.2 && weapons[m_robots[i]->getCurrentWeapon(LEFT)]->shoot())
					{

						Projectile* bullet = new Projectile(weapons[m_robots[i]->getCurrentWeapon(LEFT)]->getType(), weapons[m_robots[i]->getCurrentWeapon(LEFT)]->getDamage());

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

						bullet->setRotation(0.0, 1.0, 0.0f, m_robots[i]->getCurrentRot());

						// TODO add recoil here 
						bullet->setDirection(XMVector3Cross(XMVectorSet(0, 1, 0, 0), bullet->getPosition() - m_robots[i]->getPosition()));
						m_projectiles.push_back(bullet);
					}
				}

				// Movement speed
				if (m_input.getTriggerR(i) > 0.2 && weapons[m_robots[i]->getCurrentWeapon(RIGHT)]->speedUp())
				{
					m_robots[i]->setVelocity(20.0f * weapons[m_robots[i]->getCurrentWeapon(RIGHT)]->getSpeed());
				}
				if (!weapons[m_robots[i]->getCurrentWeapon(RIGHT)]->getActive() && weapons[m_robots[i]->getCurrentWeapon(RIGHT)]->getType() == MOVEMENT) {
					m_robots[i]->setVelocity(20.0f);
				}
				if (m_robots[i]->getCurrentWeapon(LEFT) != -1)
				{
					if (m_input.getTriggerL(i) > 0.2 && weapons[m_robots[i]->getCurrentWeapon(LEFT)]->speedUp())
					{
						m_robots[i]->setVelocity(20.0f * weapons[m_robots[i]->getCurrentWeapon(LEFT)]->getSpeed());
					}
					if (!weapons[m_robots[i]->getCurrentWeapon(LEFT)]->getActive() && weapons[m_robots[i]->getCurrentWeapon(LEFT)]->getType() == MOVEMENT) {
						m_robots[i]->setVelocity(20.0f);
					}
				}

				// Pick up resources
				for (int j = 0; j < m_resources.size() && m_robots[i]->getResourceIndex() == -1; j++)
				{
					// TODO: collision yo
					if (XMVectorGetX(XMVector3Length(m_robots[i]->getPosition() - m_resources[j]->getPosition())) < 1.5f)
					{
						bool hasAlready = false;
						for (int k = 0; k < XUSER_MAX_COUNT; k++)
						{
							if (m_robots[k] != nullptr && k != i)
							{
								if (m_robots[k]->getResourceIndex() == j)
									hasAlready = true;
							}
						}
						if (!hasAlready) {
							m_robots[i]->setResourceIndex(j);
						}
					}
				}

				// Turn in resources
				if (m_input.isPressed(i, XINPUT_GAMEPAD_A))
				{
					for (int j = 0; j < m_nodes.size() && m_robots[i]->getResourceIndex() != -1; j++)
					{
						// TODO: change range or way to calc?
						if (XMVectorGetX(XMVector3Length(m_robots[i]->getPosition() - m_nodes[j]->getPosition())) < 5.0f &&
							m_resources[m_robots[i]->getResourceIndex()]->getType() == m_nodes[j]->getType())
						{
							m_robots[i]->upgradeWeapon(m_nodes[j]->getType());

							for (int k = 0; k < XUSER_MAX_COUNT; k++)
							{
								if (m_robots[k] != nullptr && k != i)
								{
									if (m_robots[k]->getResourceIndex() > m_robots[i]->getResourceIndex())
										m_robots[k]->setResourceIndex(m_robots[k]->getResourceIndex() - 1);
								}
							}

							delete m_resources[m_robots[i]->getResourceIndex()];
							m_resources.erase(m_resources.begin() + m_robots[i]->getResourceIndex());
							m_robots[i]->removeResource();

							break;
						}
					}
				}

				// Change weapons
				if (m_robots[i]->isReady(dt) && m_input.isPressed(i, XINPUT_GAMEPAD_RIGHT_SHOULDER))
				{
					if (weapons[m_robots[i]->getCurrentWeapon(RIGHT)]->getType() == MOVEMENT)
					{
						m_robots[i]->setVelocity(20.0f);
					}

					m_robots[i]->changeWeapon(RIGHT);
				}
				
				if (m_robots[i]->isReady(dt) && m_input.isPressed(i, XINPUT_GAMEPAD_LEFT_SHOULDER) && m_robots[i]->getCurrentWeapon(LEFT) != -1)
				{
					if (weapons[m_robots[i]->getCurrentWeapon(LEFT)]->getType() == MOVEMENT)
					{
						m_robots[i]->setVelocity(20.0f);
					}

					m_robots[i]->changeWeapon(LEFT);
				}
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
	srand(time(NULL));

	m_nrOfPlayers = 0;
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
		m_robots[i] = nullptr;
	updatePlayerStatus();

	objectData sceneData;
	sceneData.pos = XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);
	sceneData.rotation = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	sceneData.scale = XMVectorSet(0.6f, 0.6f, 0.6f, 0.6f);
	m_preLoader.setStaticData(objectType::e_scene, sceneData);

	for (int i = 0; i < 25; i++)
	{
		Resource* resource = new Resource(i % 4);
		resource->setPosition(XMVectorSet((float)(rand() % 30 - 15), -0.4f, (float)(rand() % 20 - 40), 0.0f));
		m_resources.push_back(resource);
	}

	for (int i = 0; i < 4; i++)
	{
		Node* node = new Node(i % 4);
		node->setPosition(XMVectorSet((float)(i * 8 - 15), -0.2f, (float)(10.0f), 0.0f));
		m_nodes.push_back(node);
	}
}

returnInfo Game::update(float dt)
{
	handleInputs(dt);
	updatePlayerStatus();

	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		if (m_robots[i] != nullptr)
		{
			m_robots[i]->update();

			for (int j = 0; j < m_robots[i]->getWeapons().size(); j++)
			{
				m_robots[i]->getWeapons()[j]->updateTime(dt);
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
	returnInfo info;
	if (m_robots[0] != nullptr) {
		XMVECTOR pos = m_robots[0]->getPosition();
		info.x = pos.m128_f32[0];
		info.y = pos.m128_f32[1];
		info.z = pos.m128_f32[2];
	}
	else {
		info.x = 0;
		info.y = 0;
		info.z = 0;
	}
	
	
	
	return info;
}

void Game::updateSec()
{
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		if (m_input.isPressed(i, XINPUT_GAMEPAD_B))
		{
			m_robots[i]->upgradeWeapon(RIFLE);
		}
	}
}

void Game::draw()
{
	//m_preLoader.draw(objectType::e_scene);
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		if (m_robots[i] != nullptr)
		{
			std::vector<Weapon*> weapons = m_robots[i]->getWeapons();

			m_preLoader.draw(objectType::e_robot, m_robots[i]->getData(), 1, 2);
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

	for (int i = 0; i < m_resources.size(); i++)
	{
		m_preLoader.draw(objectType::e_resource, m_resources[i]->getData(), 0, 0);
	}

	for (int i = 0; i < m_nodes.size(); i++)
	{
		m_preLoader.draw(objectType::e_node, m_nodes[i]->getData(), 0, 0);
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

	for (int i = 0; i < m_resources.size(); i++)
	{
		delete m_resources[i];
	}

	for (int i = 0; i < m_nodes.size(); i++)
	{
		delete m_nodes[i];
	}
}
