#include "GameState.h"

void GameState::handleMovement(Game* game, float dt, int id)
{
	// Robot and weapon movement
	m_robots[id]->move(
		XMVectorSet(m_input->getThumbLX(id), 0.0f, m_input->getThumbLY(id), 0.0f) *
		m_robots[id]->getVelocity() * dt * ((m_input->isPressed(id, XINPUT_GAMEPAD_Y)) ? 2.0f : 1.0f) // TODO remove trigger
	);

	// Rotation
	if (abs(m_input->getThumbRX(id)) > 0.1f || abs(m_input->getThumbRY(id)) > 0.1f)
	{
		m_robots[id]->setCurrentRot(XMConvertToDegrees(atan2(m_input->getThumbRX(id), m_input->getThumbRY(id))));

		m_robots[id]->setRotation(0, 1, 0, m_robots[id]->getCurrentRot());
	}

	// Projectile movement
	ProjectileBank::getInstance()->moveProjectiles(dt);

	// Resource movement
	if (m_robots[id]->getResourceIndex() != -1)
		m_resources[m_robots[id]->getResourceIndex()]->setPosition(m_robots[id]->getPosition() + XMVectorSet(0.0f, 1.5f, 0.0f, 0.0f));
}

void GameState::handleInputs(Game* game, float dt)
{
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		if (m_robots[i] != nullptr)
		{
			if (!m_input->refresh(i))
			{
				m_input->reconnectController(i);
			}
			else
			{
				handleMovement(game, dt, i);

				// Shoot
				if (m_input->getTriggerR(i) > 0.2)
				{
					m_robots[i]->useWeapon(RIGHT, dt);
				}
				if (m_input->getTriggerL(i) > 0.2)
				{
					m_robots[i]->useWeapon(LEFT, dt);
				}

				// Pick up resources
				for (int j = 0; j < m_resources.size() && m_robots[i]->getResourceIndex() == -1; j++)
				{
					// TODO: collision yo
					if (XMVectorGetX(XMVector3Length(m_robots[i]->getPosition() - m_resources[j]->getPosition())) < 1.5f &&
						!m_resources[j]->isBlocked())
					{
						m_robots[i]->setResourceIndex(j);
						m_resources[j]->setBlocked(true);
					}
				}

				// Turn in resources
				if (m_input->isPressed(i, XINPUT_GAMEPAD_A))
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
					if (m_input->isPressed(i, XINPUT_GAMEPAD_RIGHT_SHOULDER))
					{
						m_robots[i]->changeWeapon(RIGHT);
					}

					if (m_input->isPressed(i, XINPUT_GAMEPAD_LEFT_SHOULDER))
					{
						m_robots[i]->changeWeapon(LEFT);
					}
				}

				// TODO: add collision and remove projectile
				if (m_input->isPressed(i, XINPUT_GAMEPAD_DPAD_DOWN))
				{
					if (m_robots[i]->getResourceIndex() != -1)
					{
						m_resources[m_robots[i]->getResourceIndex()]->setPosition(m_robots[i]->getPosition());
						m_resources[m_robots[i]->getResourceIndex()]->setBlocked(false);
					}
					m_robots[i]->damagePlayer(1);
				}
				if (m_input->isPressed(i, XINPUT_GAMEPAD_DPAD_UP))
				{
					m_robots[i]->setHealth(100);
				}

				if (m_input->isPressed(i, XINPUT_GAMEPAD_B))
				{
					m_robots[i]->upgradeWeapon(RIFLE);
				}
			}
		}
	}
}

GameState::GameState()
{
	srand(time(NULL));

	m_type = stateType::e_gameState;
	m_input = nullptr;
	m_robots = nullptr;

	for (int i = 0; i < 25; i++)
	{
		Resource* resource = new Resource(i % 4);
		resource->setPosition(XMVectorSet((float)(rand() % 30 - 15), -0.4f, (float)(rand() % 20 - 40), 0.0f));
		m_resources.push_back(resource);
	}

	for (int i = 0; i < 4; i++)
	{
		Node* node = new Node(i % 4);
		node->setPosition(XMVectorSet((float)(i * 10), -0.2f, (float)(-60.0f), 0.0f));
		m_nodes.push_back(node);
	}
	m_transparency.initialize();
	m_transparency.bindConstantBuffer();
	m_lights = Lights::getInstance();
	int index = m_lights->addPointLight(-10, 25, 0, 55, 1, 0.5f, 0.125f, 1);
	m_lights->setColor(index, float(255) / 255, float(0) / 255, float(97) / 255);
	index = m_lights->addDirectionalLight(0.5f, -1, 0.5f, 0.6f, 0.6f, 1.0f, 4.0f);
	index = m_lights->addSpotLight(-1.5f, 6, -40, 10, -0.33f, -1, 0.0f, 1.0f, 1.0f, 0.0f, 27, 20);
	index = m_lights->addSpotLight(1.5f, 6, -40, 10, 0.33f, -1, 0.0f, 1.0f, 1.0f, 0.0f, 27, 20);
	m_lights->addPointLight(-31, 6, -43, 10, 1, 1, 0, 5);
	m_lights->addPointLight(27.5f, 4, -36, 10, 1, 0, 1, 5);
	m_lights->addPointLight(50, 15.0, 42, 30, 1, 0.5f, 0, 25);
	m_lights->addPointLight(-3, 10.0, 45, 20, 0, 1, 1, 10);
	m_lights->addPointLight(20, 5.0, 40, 30, 0, 0, 1, 15);
	m_lights->addPointLight(107, 5.0, 40, 30, 1, 1, 0, 20);
	m_lights->addPointLight(90, 5.0, 33, 10, 1, 0, 0, 20);
	m_lights->addPointLight(-40, 6.0, -1, 30, 1, 1, 0.6f, 15);
	m_lights->addPointLight(-71.5, 1.0, 59.5, 10, 1, 0.6f, 0, 10);

	//m_lights->setColor(index, float(19) / 255, float(62) / 255, float(124) / 255);
	/*
	index = m_lights->addSpotLight(-35, 30, -5, 50, -0.3f, -1, 0.3f, 1.0f, 0.9f, 0.9f, 25, 1);
	m_lights->setColor(index, float(234) / 255, float(185) / 255, float(217) / 255);
	index = m_lights->addSpotLight(0, 5, -45, 50, 0, -0.5f, 1.0f, 1.0f, 0.9f, 0.9f, 25, 1);
	m_lights->setColor(index, float(234) / 255, float(185) / 255, float(217) / 255);*/
	
	
}

GameState::~GameState()
{
	for (int i = 0; i < m_resources.size(); i++)
	{
		delete m_resources[i];
	}

	for (int i = 0; i < m_nodes.size(); i++)
	{
		delete m_nodes[i];
	}
	
}

void GameState::pause()
{
}

void GameState::resume()
{
}

void GameState::handleInput(Game* game)
{
}

void GameState::update(Game* game, float dt)
{
	m_input = game->getInput();
	m_robots = game->getRobots();
	handleInputs(game, dt);
	game->updatePlayerStatus();

	Robot** robots = game->getRobots();
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		if (m_robots[i] != nullptr)
			m_robots[i]->update(dt);
	}
	for (int i = 0; i < 4; i++)
	{
		if (m_robots[i] != nullptr)
		{
			XMVECTOR pos = m_robots[i]->getPosition();
			pos.m128_f32[3] = 1;
			m_transparency.update(pos, DX::getInstance()->getCam()->getViewMatrix(), DX::getInstance()->getCam()->getProjectionMatrix(), i);
		}
		
	}
	

	// TODO remove with collision instead aswell as game field?
	for (int i = 0; i < ProjectileBank::getInstance()->getList().size(); i++)
	{
		if (XMVectorGetX(XMVector3Length(ProjectileBank::getInstance()->getList()[i]->getPosition())) > 50.0f)
		{
			ProjectileBank::getInstance()->removeProjectile(i);
		}
	}

	for (int i = 0; i < m_resources.size(); i++)
	{
		m_resources[i]->updateTime(dt);
	}
}

void GameState::draw(Game* game, renderPass pass)
{
	if (pass != renderPass::e_transparent)
	{

		for (int i = 0; i < XUSER_MAX_COUNT; i++)
		{
			if (m_robots[i] != nullptr)
			{
				std::vector<Weapon*> weapons = m_robots[i]->getWeapons();

			game->getPreLoader()->draw(objectType::e_robot, m_robots[i]->getData(), 1, 2);
			game->getPreLoader()->draw(objectType::e_weapon, weapons[m_robots[i]->getCurrentWeapon(RIGHT)]->getData(), m_robots[i]->getData());

				if (game->getRobots()[i]->getCurrentWeapon(LEFT) != -1)
				{
					game->getPreLoader()->draw(objectType::e_weapon, weapons[game->getRobots()[i]->getCurrentWeapon(LEFT)]->getData(), game->getRobots()[i]->getData());
				}
			}
		}
		
	}
	else if(pass != renderPass::e_opaque)
	{
		for (int i = 0; i < ProjectileBank::getInstance()->getList().size(); i++)
		{
			game->getPreLoader()->draw(objectType::e_projectile, ProjectileBank::getInstance()->getList()[i]->getData(), 0, 0);
		}
		for (int i = 0; i < m_nodes.size(); i++)
		{
			game->getPreLoader()->draw(objectType::e_node, m_nodes[i]->getData(), 0, 0);
		}
		for (int i = 0; i < m_resources.size(); i++)
		{
			game->getPreLoader()->draw(objectType::e_resource, m_resources[i]->getData(), 0, 0);
		}
		if (pass != renderPass::e_opaque)
		{
			game->getPreLoader()->draw(objectType::e_scene);
		}
	}
	
}
