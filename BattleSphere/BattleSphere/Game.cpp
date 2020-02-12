#include "Game.h"

void Game::handleMovement(float dt, int id)
{
	// Robot and weapon movement
	m_robots[id]->move(
		XMVectorSet(m_input.getThumbLX(id), 0.0f, m_input.getThumbLY(id), 0.0f) *
		m_robots[id]->getVelocity() * dt * ((m_input.isPressed(id, XINPUT_GAMEPAD_Y)) ? 2.0f : 1.0f) // TODO remove trigger
	);

	// Rotation
	if (abs(m_input.getThumbRX(id)) > 0.1f || abs(m_input.getThumbRY(id)) > 0.1f)
	{
		m_robots[id]->setCurrentRot(XMConvertToDegrees(atan2(m_input.getThumbRX(id), m_input.getThumbRY(id))));

		m_robots[id]->setRotation(0, 1, 0, m_robots[id]->getCurrentRot());
	}

	// Projectile movement
	ProjectileBank::getInstance()->moveProjectiles(dt);

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
				handleMovement(dt, i);

				// Shoot
				if (m_input.getTriggerR(i) > 0.2)
				{
					m_robots[i]->useWeapon(RIGHT, dt);
				}
				if (m_input.getTriggerL(i) > 0.2)
				{
					m_robots[i]->useWeapon(LEFT, dt);
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
				if (m_robots[i]->isReady(dt))
				{
					if (m_input.isPressed(i, XINPUT_GAMEPAD_RIGHT_SHOULDER))
					{
						m_robots[i]->changeWeapon(RIGHT);
					}

					if (m_input.isPressed(i, XINPUT_GAMEPAD_LEFT_SHOULDER))
					{
						m_robots[i]->changeWeapon(LEFT);
					}
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

void Game::update(float dt)
{
	handleInputs(dt);
	updatePlayerStatus();

	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		if (m_robots[i] != nullptr)
			m_robots[i]->update(dt);
	}

	// TODO: remove with collision instead aswell as game field?
	for (int i = 0; i < ProjectileBank::getInstance()->getList().size(); i++)
	{
		if (XMVectorGetX(XMVector3Length(ProjectileBank::getInstance()->getList()[i]->getPosition())) > 50.0f)
		{
			ProjectileBank::getInstance()->removeProjectile(i);
		}
	}
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
	m_preLoader.draw(objectType::e_scene);
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

	for (int i = 0; i < ProjectileBank::getInstance()->getList().size(); i++)
	{
		m_preLoader.draw(objectType::e_projectile, ProjectileBank::getInstance()->getList()[i]->getData());
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

	for (int i = 0; i < m_resources.size(); i++)
	{
		delete m_resources[i];
	}

	for (int i = 0; i < m_nodes.size(); i++)
	{
		delete m_nodes[i];
	}
}
