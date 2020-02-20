#include "GameState.h"

void GameState::loadLists()
{
	// Normal spawns
	m_spawns.push_back({ -62, 120 });
	m_spawns.push_back({ -120, 100 });
	m_spawns.push_back({ -74, 85 });
	m_spawns.push_back({ -46, 85 });
	m_spawns.push_back({ -15, 85 });
	m_spawns.push_back({ 120, 39 });
	m_spawns.push_back({ -15, 29 });
	m_spawns.push_back({ 15, 29 });
	m_spawns.push_back({ -15, -3 });
	m_spawns.push_back({ 15, -3 });
	m_spawns.push_back({ -15, -34 });
	m_spawns.push_back({ 15, -34 });
	m_spawns.push_back({ -15, -63 });
	m_spawns.push_back({ 15, -63 });
	m_spawns.push_back({ -67, 0 });
	m_spawns.push_back({ -80, 0 });
	m_spawns.push_back({ -54, 0 });
	m_spawns.push_back({ 40, -16 });
	m_spawns.push_back({ 56, -5 });
	m_spawns.push_back({ 113, -56 });
	m_spawns.push_back({ 109, -98 });
	m_spawns.push_back({ 75, -98 });
	m_spawns.push_back({ 40, -98 });
	m_spawns.push_back({ 5, -98 });
	m_spawns.push_back({ -30, -98 });
	m_spawns.push_back({ -65, -98 });
	m_spawns.push_back({ 44, -65 });
	m_spawns.push_back({ -61, -70 });
	m_spawns.push_back({ -39, -70 });
	m_spawns.push_back({ -50, -81 });
	m_spawns.push_back({ -50, -59 });
	m_normalSpawnAmount = (int)m_spawns.size();

	// Special spawns
	m_spawns.push_back({ -163, 120 });
	m_spawns.push_back({ -60, 85 });
	m_spawns.push_back({ 56, 50 });
	m_spawns.push_back({ 155, 28 });
	m_specialSpawnAmount = (int)m_spawns.size() - m_normalSpawnAmount;

	// Free spawn bool list
	for (int i = 0; i < m_spawns.size(); i++)
		m_freeSpawns.push_back(true);
}

void GameState::startSpawn()
{
	for (int i = 0; i < START_SPAWNED_RESOURCES; i++)
	{
		int spawnIndex = getSpawnIndex(); // TODO:: Change spawn types
		//Resource* resource = new Resource(m_spawnLightIndex, spawnIndex, i % BIGGEST_NORMAL_INDEX, 0.8f, false);
		Resource* resource = new Resource(m_spawnLightIndex, spawnIndex, (rand() % 4) + 2, 0.8f, false);
		XMFLOAT2 pos = m_spawns[spawnIndex];
		resource->setPosition(XMVectorSet((float)(pos.x), 0.6f, (float)(pos.y), 0.0f));
		m_resources.push_back(resource);
	}
}

int GameState::getSpawnIndex()
{
	// Find free index
	int index;
	do
		index = rand() % m_normalSpawnAmount;
	while (!m_freeSpawns[index]);

	// Taken spawn is set to not free
	m_freeSpawns[index] = false;

	return index;
}

int GameState::getSpecialSpawnIndex()
{
	// Find free index
	int index;
	do
		index = (rand() % m_specialSpawnAmount) + m_normalSpawnAmount;
	while (!m_freeSpawns[index]);

	// Taken spawn is set to not free
	m_freeSpawns[index] = false;

	return index;
}

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

	// Resource movement
	if (m_robots[id]->getResourceIndex() != -1)
		m_resources[m_robots[id]->getResourceIndex()]->setPosition(m_robots[id]->getPosition() + XMVectorSet(0.0f, 1.5f, 0.0f, 0.0f));
}

void GameState::handleInputs(Game* game, float dt)
{
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		if (game->getRobots()[i] != nullptr)
		{
			if (!m_input->refresh(i, dt))
			{
				m_input->reconnectController(i);
			}
			else if (game->getRobots()[i]->isDrawn())
			{
				// Use weapon
				if (m_input->getTriggerR(i) > 0.2)
				{
					m_robots[i]->useWeapon(RIGHT, dt);
				}
				if (m_input->getTriggerL(i) > 0.2)
				{
					m_robots[i]->useWeapon(LEFT, dt);
				}

				handleMovement(game, dt, i);

				// Pick up resources
				for (int j = 0; j < m_resources.size() && m_robots[i]->getResourceIndex() == -1; j++)
				{
					// TODO: collision yo
					XMVECTOR rob = m_robots[i]->getPosition();
					rob.m128_f32[1] = 0.0f; // Set Y to 0;
					XMVECTOR resource = m_resources[j]->getPosition();
					resource.m128_f32[1] = 0.0f; // Set Y to 0; 
					if (XMVectorGetX(XMVector3Length(rob - resource)) < 1.5f &&
						!m_resources[j]->isBlocked())
					{
						m_freeSpawns[m_resources[j]->getSpawnIndex()] = true;
						m_robots[i]->setResourceIndex(j);
						m_resources[j]->setBlocked(true);
					}
				}

				// Drop resources
				if (m_input->isPressed(i, XINPUT_GAMEPAD_B))
				{
					if (m_robots[i]->getResourceIndex() != -1)
					{
						m_resources[m_robots[i]->getResourceIndex()]->setPosition(m_robots[i]->getPosition());
						m_resources[m_robots[i]->getResourceIndex()]->setBlocked(false);
						m_robots[i]->removeResource();
					}
				}

				// Turn in resources
				if (m_input->isPressed(i, XINPUT_GAMEPAD_A))
				{
					for (int j = 0; j < m_nodes.size() && m_robots[i]->getResourceIndex() != -1; j++)
					{
						// TODO: change range or way to calc?
						if (XMVectorGetX(XMVector3Length(m_robots[i]->getPosition() - m_nodes[j]->getPosition())) < 5.0f &&
							m_nodes[j]->isType(m_resources[m_robots[i]->getResourceIndex()]->getType()))
						{
							m_robots[i]->upgradeWeapon(m_resources[m_robots[i]->getResourceIndex()]->getType());

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
					int resourceIndex = m_robots[i]->getResourceIndex();
					if (m_robots[i]->damagePlayer(1, XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), -1))
					{
						m_input->setVibration(i, 0.5f);
						if (resourceIndex != -1)
						{
							m_resources[resourceIndex]->setPosition(m_robots[i]->getPosition());
							m_resources[resourceIndex]->setBlocked(false);
						}
					}
				}

				if (m_input->isPressed(i, XINPUT_GAMEPAD_X))
				{
					m_robots[i]->upgradeWeapon(RIFLE);
				}
			}

			// TODO: remove cheats yooo
			if (m_input->isPressed(i, XINPUT_GAMEPAD_DPAD_UP))
			{
				m_robots[i]->setHealth(100);
				m_input->setVibration(i, 0.0f);
			}

			// COLLISION PLAYER VS STATIC OBJECTS
			CollisionInfo collisionInfo;
			BoundingSphere* robotBV = static_cast <BoundingSphere*> (game->getPreLoader()->getDynamicBoundingVolume(objectType::e_robot, m_robots[i]->getData(), 0, 0));
			XMVECTOR v = m_robots[i]->getPosition() - m_robots[i]->getPreviousPosition();
			float l = XMVectorGetX(XMVector3Length(v));
			float r = robotBV->getRadius();
			XMVECTOR newPos = m_robots[i]->getPosition();
			// If robot moved farther than its radius
			if (r < l)
			{
				float tIncrement = r / l;
				for (float t = tIncrement; t < 1.0f && !collisionInfo.m_colliding; t += tIncrement)
				{
					robotBV->setPos(m_robots[i]->getPreviousPosition() + (v * t));
					collisionInfo = game->getQuadtree()->testCollision(robotBV);

					if (collisionInfo.m_colliding)
						newPos = m_robots[i]->getPreviousPosition() + (v * t) + collisionInfo.m_normal;
				}
			}
			else
			{
				// Normal collision
				collisionInfo = game->getQuadtree()->testCollision(robotBV);
				if (collisionInfo.m_colliding)
					newPos = m_robots[i]->getPosition() + collisionInfo.m_normal;
			}

			m_robots[i]->setPosition(newPos);
			m_robots[i]->storePositionInHistory(newPos);
		}
	}
}

GameState::GameState()
{
	srand((unsigned int)time(NULL));

	m_type = stateType::e_gameState;
	m_input = nullptr;
	m_robots = nullptr;
	m_collectedTime = 0.0f;

	for (int i = 0; i < 6; i++)
	{
		Node* node = new Node(i % 6);
		node->setPosition(XMVectorSet((float)(i * 10), 0.2f, (float)(-90.0f), 0.0f));
		m_nodes.push_back(node);
	}
	m_transparency.initialize();
	m_transparency.bindConstantBuffer();
	m_lights = Lights::getInstance();
	int index = m_lights->addPointLight(-10, 25, 0, 55, 1, 0.5f, 0.125f, 1);
	m_lights->setColor(index, float(255) / 255, float(0) / 255, float(97) / 255);
	index = m_lights->addDirectionalLight(0.5f, -1, 0.5f, 0.6f, 0.6f, 1.0f, 4.0f);
	index = m_lights->addSpotLight(-2.5f, 11.67f, -67, 17, -0.33f, -1, 0.0f, 1.0f, 1.0f, 0.0f, 27, 20);
	index = m_lights->addSpotLight(2.5f, 11.67f, -67, 17, 0.33f, -1, 0.0f, 1.0f, 1.0f, 0.0f, 27, 20);
	m_lights->addAreaLight(-52, 11.67f, -72, 17, 1, 1, 0, 5);
	m_lights->addAreaLight(46, 8, -60, 17, 1, 0, 1, 5);
	m_lights->addAreaLight(78, 18, 70, 50, 1, 0.5f, 0, 25);
	m_lights->addAreaLight(-5, 18, 75, 33, 0, 1, 1, 10);
	m_lights->addAreaLight(33, 10, 67, 50, 0, 0, 1, 15);
	m_lights->addAreaLight(178, 10, 67, 50, 1, 1, 0, 20);
	m_lights->addAreaLight(150, 10, 55, 17, 1, 0, 0, 20);
	m_lights->addPointLight(-67, 12, -1.6f, 50, 1, 1, 0.6f, 15);
	m_lights->addAreaLight(-119, 3, 99, 17, 1, 0.6f, 0, 10);


	// Initialize resource spawning lists
	m_spawnLightIndex = m_lights->addSpotLight(0, 150, 0, 0, 0, -1, 0, 1, 1, 1, 13, 15);
	loadLists();
	startSpawn();
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

void GameState::regularSpawn(float dT)
{
	m_collectedTime += dT;

	// Spawn if spawn interval pasted
	if (m_collectedTime >= SPAWN_INTERVAL)
	{
		// Reset spawn time
		while (m_collectedTime >= SPAWN_INTERVAL)
			m_collectedTime -= SPAWN_INTERVAL;

		// Calculate number of players
		int nrOfPlayers = 0;
		for (int i = 0; i < 4; i++)
		{
			if (m_robots[i] != nullptr)
				nrOfPlayers++;
		}

		// Spawn resources according to max allowed
		int maxResources = MAX_RESOURCES_OUT_PER_PLAYER * nrOfPlayers;
		if (maxResources > m_resources.size()) // If there's room for a resource, spawn one
		{


			// Randomize whether it is a normal or special resource
			bool isSpecial = false;
			if (rand() % 100 < SPECIAL_RESOURCE_CHANCE)
			{
				// Only make resource special if there are available spots
				for (int i = 0; i < m_specialSpawnAmount && !isSpecial; i++)
				{
					if (m_freeSpawns[(int)(m_normalSpawnAmount)+(char)i])
						isSpecial = true;
				}
			}


			int spawnIndex;
			Resource* resource;
			if (isSpecial)
			{
				spawnIndex = getSpecialSpawnIndex(); // TODO:: Change spawn types
				//resource = new Resource(m_spawnLightIndex, spawnIndex, rand() % BIGGEST_NORMAL_INDEX, 3.0f);
				resource = new Resource(m_spawnLightIndex, spawnIndex, RIFLE, 3.0f);
			}
			else
			{
				spawnIndex = getSpawnIndex(); // TODO: Edit INDEX FOR SPECIAL BELOW
				//resource = new Resource(m_spawnLightIndex, spawnIndex, rand() % BIGGEST_NORMAL_INDEX, 1.1f);
				resource = new Resource(m_spawnLightIndex, spawnIndex, (rand() % 4) + 2, 1.2f);
			}


			XMFLOAT2 pos = m_spawns[spawnIndex];
			resource->setPosition(XMVectorSet((float)(pos.x), 0.6f, (float)(pos.y), 0.0f));
			m_resources.push_back(resource);
		}
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
	regularSpawn(dt);

	// Projectile movement
	ProjectileBank::getInstance()->moveProjectiles(dt);

	Robot** robots = game->getRobots();
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		if (m_robots[i] != nullptr && m_robots[i]->isDrawn())
		{
			m_robots[i]->update(dt);
			XMVECTOR pos = m_robots[i]->getPosition();
			pos.m128_f32[3] = 1;
			m_transparency.update(pos, DX::getInstance()->getCam()->getViewMatrix(), DX::getInstance()->getCam()->getProjectionMatrix(), i);
		}

	}

	// TODO remove with collision instead aswell as game field?
	for (int i = 0; i < ProjectileBank::getInstance()->getList().size(); i++)
	{
		// Normal collision
		BoundingSphere* projBV = static_cast <BoundingSphere*> (game->getPreLoader()->getDynamicBoundingVolume(objectType::e_projectile, ProjectileBank::getInstance()->getList()[i]->getData(), 0, 0));
		CollisionInfo collisionInfo = game->getQuadtree()->testCollision(projBV);
		if (collisionInfo.m_colliding)
			ProjectileBank::getInstance()->removeProjectile(i);
		else if (XMVectorGetX(XMVector3Length(ProjectileBank::getInstance()->getList()[i]->getPosition())) > 200.0f)
			ProjectileBank::getInstance()->removeProjectile(i);
		else
		{
			// COLLISION PROJECTILE VS PLAYERS
			for (int j = 0; j < XUSER_MAX_COUNT; j++)
			{
				if (m_robots[j] != nullptr && m_robots[j]->isDrawn())
				{
					BoundingSphere* robotBV = static_cast <BoundingSphere*> (game->getPreLoader()->getDynamicBoundingVolume(objectType::e_robot, m_robots[j]->getData(), 0, 0));
					collisionInfo = robotBV->intersects(projBV);

					// TODO: Find solution to projectiles colliding with its "owner" and is immediately removed
					if (collisionInfo.m_colliding)
					{
						int resourceIndex = m_robots[j]->getResourceIndex();
						if (m_robots[j]->damagePlayer(ProjectileBank::getInstance()->getList()[i]->getDamage(), ProjectileBank::getInstance()->getList()[i]->getDirection(), i))
						{
							m_input->setVibration(j, 0.5f);
							if (resourceIndex != -1)
							{
								m_resources[resourceIndex]->setPosition(m_robots[j]->getPosition());
								m_resources[resourceIndex]->setBlocked(false);
							}
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < m_resources.size(); i++)
	{
		m_resources[i]->updateTime(dt);
	}

	for (int i = 0; i < m_nodes.size(); i++)
	{
		m_nodes[i]->updateTime(dt);
	}
}

void GameState::draw(Game* game, renderPass pass)
{
	m_input = game->getInput();
	m_robots = game->getRobots();

	if (pass != renderPass::e_transparent)
	{

		for (int i = 0; i < XUSER_MAX_COUNT; i++)
		{
			if (m_robots[i] != nullptr && m_robots[i]->isDrawn())
			{
				std::vector<Weapon*> weapons = m_robots[i]->getWeapons();

				game->getPreLoader()->draw(objectType::e_robot, m_robots[i]->getData(), 1, 2);
				game->getPreLoader()->draw(objectType::e_weapon, weapons[m_robots[i]->getCurrentWeapon(RIGHT)]->getData(), m_robots[i]->getData(), 0, 0);

				if (game->getRobots()[i]->getCurrentWeapon(LEFT) != -1)
				{
					game->getPreLoader()->draw(objectType::e_weapon, weapons[game->getRobots()[i]->getCurrentWeapon(LEFT)]->getData(), game->getRobots()[i]->getData());
				}
			}
		}
		for (int i = 0; i < ProjectileBank::getInstance()->getList().size(); i++)
		{
			game->getPreLoader()->draw(objectType::e_projectile, ProjectileBank::getInstance()->getList()[i]->getData(), 0, 0);
		}
		for (int i = 0; i < m_resources.size(); i++)
		{
			game->getPreLoader()->draw(objectType::e_resource, m_resources[i]->getData(), 0, 0);
		}
	}
	if (pass != renderPass::e_opaque)
	{

		game->getPreLoader()->draw(objectType::e_scene);
		game->getPreLoader()->draw(objectType::e_scene, 1);
		for (int i = 0; i < m_nodes.size(); i++)
		{
			game->getPreLoader()->draw(objectType::e_node, m_nodes[i]->getData(), 0, 0);
		}
	}

}
