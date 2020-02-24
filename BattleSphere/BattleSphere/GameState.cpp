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
		//Resource* resource = new Resource(false, spawnIndex, i % BIGGEST_NORMAL_INDEX, 0.8f);
		Resource* resource = new Resource(false, spawnIndex, (rand() % 4) + 2, 0.8f);
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

void GameState::spawnNodes()
{
	Node* node = new Node(rand() % 3);
	node->setPosition(XMVectorSet(-118.0f, 0.2f, -10.0f, 0.0f));
	node->setRotation(0.0f, 1.0f, 0.0f, 0.0f);
	m_nodes.push_back(node);
	node = new Node(rand() % 3);
	node->setPosition(XMVectorSet(150.0f, 0.2f, 122.0f, 0.0f));
	node->setRotation(0.0f, 1.0f, 0.0f, 0.0f);
	m_nodes.push_back(node);
	node = new Node(rand() % 3);
	node->setPosition(XMVectorSet(105.0f, 0.2f, -18.0f, 0.0f));
	node->setRotation(0.0f, 1.0f, 0.0f, 90.0f);
	m_nodes.push_back(node);
	node = new Node(rand() % 3);
	node->setPosition(XMVectorSet(-101.0f, 0.3f, -49.0f, 0.0f));
	node->setRotation(0.0f, 1.0f, 0.0f, 90.0f);
	m_nodes.push_back(node);
}

void GameState::updateSpawnDrone(float dT)
{
	// Update propellers
	m_spawnDronePropeller[0].rotate(0.0f, 1.0f, 0.0f, -dT * PROPELLER_SPEED * (rand() % 2 + 1.0f));
	m_spawnDronePropeller[1].rotate(0.0f, 1.0f, 0.0f, -dT * PROPELLER_SPEED * (rand() % 2 + 1.0f));
	m_spawnDronePropeller[2].rotate(0.0f, 1.0f, 0.0f, -dT * PROPELLER_SPEED * (rand() % 2 + 1.0f));
	m_spawnDronePropeller[3].rotate(0.0f, 1.0f, 0.0f, -dT * PROPELLER_SPEED * (rand() % 2 + 1.0f));

	// Update time since last mission was assigned
	m_collectedTime += dT;

	// Update drone states
	switch (m_spawnDroneState)
	{
	case -1: // Wait for mission
		if (m_collectedTime >= SPAWN_INTERVAL)
		{
			if (assignMission()) // If mission assignable, advance state
				m_spawnDroneState++;
		}
		break;

	case 0: // Set target to rising point
		XMVECTOR target = m_spawnDroneBody.getPosition();
		target.m128_f32[1] = TRAVEL_HEIGHT;
		setTravelTarget(target);
		setRotationTarget(m_transportDestination);
		m_spawnDroneState++;
		break;
	case 1: // Rise 
		if (travelAndCheck(dT, false))
			m_spawnDroneState++;
		break;

	case 2: // Set target above transport location
		target = m_transportDestination;
		target.m128_f32[1] = TRAVEL_HEIGHT;
		setTravelTarget(target);
		m_spawnDroneState++;
		break;

	case 3: // Travel
		if (travelAndCheck(dT, true))
			m_spawnDroneState++;
		break;

	case 4: // Set target to decline down to spawn point
		target = m_transportDestination;
		target.m128_f32[1] -= RESOURCE_OFFSET;
		setTravelTarget(target);
		m_spawnDroneState++;
		break;

	case 5: // Decline 
		if (travelAndCheck(dT, false))
			m_spawnDroneState++;
		break;

	case 6: // Leave resource
		m_resources[m_heldResourceIndex]->setPosition(m_transportDestination);
		m_resources[m_heldResourceIndex]->setBlocked(false);
		m_heldResourceIndex = -1;
		m_spawnDroneState++;
		break;

	case 7: // Set target above transport location, rotate to drone start
		target = m_transportDestination;
		target.m128_f32[1] = TRAVEL_HEIGHT;
		setTravelTarget(target);
		setRotationTarget(DRONE_START);
		m_spawnDroneState++;
		break;

	case 8: // Rise 
		if (travelAndCheck(dT, false))
			m_spawnDroneState++;
		break;

	case 9: // Set target above drone start
		target = DRONE_START;
		target.m128_f32[1] = TRAVEL_HEIGHT;
		setTravelTarget(target);
		m_spawnDroneState++;
		break;

	case 10: // Travel
		if (travelAndCheck(dT, true))
			m_spawnDroneState++;
		break;

	case 11: // Set target to decline down to drone start  
		setTravelTarget(DRONE_START);
		m_spawnDroneState++;
		break;

	case 12: // Decline
		if (travelAndCheck(dT, false))
			m_spawnDroneState++;
		break;

	case 13: // Reset switch
		m_spawnDroneState++;
		m_spawnDroneState = -1;
		break;
	}
}

void GameState::updateDynamicCamera()
{
	// Calculate number of players
	int nrOfPlayers = 0;
	for (int i = 0; i < 4; i++)
	{
		if (m_robots[i] != nullptr)
			nrOfPlayers++;
	}

	if (nrOfPlayers >= 1)
	{
		// Get new look at 
		XMVECTOR newLookAt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR robPos;
		for (int i = 0; i < nrOfPlayers; i++)
			newLookAt += robPos = m_robots[i]->getPosition();
		newLookAt /= (float)nrOfPlayers;

		// Get min and max of x and z
		robPos = m_robots[0]->getPosition();
		float maxRobotDistance = -1;
		float minX = robPos.m128_f32[0];
		float maxX = robPos.m128_f32[0];
		float minZ = robPos.m128_f32[2];
		float maxZ = robPos.m128_f32[2];
		for (int i = 1; i < nrOfPlayers; i++)
		{
			robPos = m_robots[i]->getPosition();

			if (minX > robPos.m128_f32[0])
				minX = robPos.m128_f32[0];

			else if (maxX < robPos.m128_f32[0])
				maxX = robPos.m128_f32[0];

			if (minZ > robPos.m128_f32[2])
				minZ = robPos.m128_f32[2];

			else if (maxZ < robPos.m128_f32[2])
				maxZ = robPos.m128_f32[2];
		}

		// Calculate biggest distance
		float xDifference = maxX - minX;
		float zDifference = maxZ - minZ;
		float biggestDifference;
		if (xDifference > zDifference)
			biggestDifference = xDifference;
		else
			biggestDifference = zDifference;

		// Set new camera position and look at
		XMVECTOR newPos = newLookAt + m_vecToCam * (ADDED_CAM_DISTANCE + biggestDifference);
		DX::getInstance()->getCam()->setPosAndLook(newPos, newLookAt);
	}
}

void GameState::handleMovement(Game* game, float dt, int id)
{
	// Save velocity for collision
	m_robots[id]->setVel(XMVectorSet(m_input->getThumbLX(id), 0.0f, m_input->getThumbLY(id), 0.0f) *
		m_robots[id]->getVelocity() * dt * ((m_input->isPressed(id, XINPUT_GAMEPAD_Y)) ? 2.0f : 1.0f)); // TODO remove trigger

	// Robot and weapon movement
	m_robots[id]->move(m_robots[id]->getVel());

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
			boundingData robotBD = game->getPreLoader()->getBoundingData(objectType::e_robot, 1, 0);
			robotBD.pos = m_robots[i]->getPosition();
			XMVECTOR v = m_robots[i]->getPosition() - m_robots[i]->getPreviousPosition();
			XMVECTOR newPos = m_robots[i]->getPosition();
			float l = XMVectorGetX(XMVector3Length(v));
			float d = robotBD.halfWD.x * 2.0f;

			// if robot moved further than its diameter
			if (d < l)
			{
				float tIncrement = 1.0f - l / (l + d);
				for (float t = tIncrement; t < 1.0f; t += tIncrement)
				{
					robotBD.pos = m_robots[i]->getPreviousPosition() + (v * t);
					collisionInfo = game->getQuadtree()->testCollision(robotBD, m_robots[i]->getPreviousPosition());

					if (collisionInfo.m_colliding)
					{
						// Collision found, stop tests
						newPos = m_robots[i]->getPreviousPosition() + (v * t) + collisionInfo.m_normal;
						break;
					}
				}
			}
			for (int j = 0; j < 10; ++j)
			{
				// Normal collision
				robotBD.pos = newPos;
				collisionInfo = game->getQuadtree()->testCollision(robotBD, m_robots[i]->getPreviousPosition());
				if (collisionInfo.m_colliding)
					newPos += collisionInfo.m_normal;
				else break;
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

	// Spawn preset nodes
	spawnNodes();

	m_transparency.initialize();
	m_transparency.bindConstantBuffer();
	m_lights = Lights::getInstance();
	int index = m_lights->addPointLight(-10, 25, 0, 55, 1, 0.5f, 0.125f, 1);
	m_lights->setColor(index, float(255) / 255, float(0) / 255, float(97) / 255);
	index = m_lights->addSpotLight(-2.5f, 11.67f, -67, 17, -0.33f, -1, 0.0f, 1.0f, 1.0f, 0.0f, 27, 20);
	index = m_lights->addSpotLight(2.5f, 11.67f, -67, 17, 0.33f, -1, 0.0f, 1.0f, 1.0f, 0.0f, 27, 20);
	m_lights->addAreaLight(-52, 11.67f, -72, 17, 1, 1, 0, 5);
	m_lights->addAreaLight(46, 8, -60, 17, 1, 0, 1, 5);
	m_lights->addAreaLight(78, 18, 70, 50, 1, 0.5f, 0, 25);
	m_lights->addAreaLight(-5, 18, 75, 33, 0, 1, 1, 10);
	m_lights->addAreaLight(33, 10, 67, 50, 0, 0, 1, 15);
	m_lights->addAreaLight(178, 10, 67, 50, 1, 1, 0, 20);
	m_lights->addAreaLight(150, 10, 55, 17, 1, 0, 0, 20);
	m_lights->addAreaLight(-119, 3, 99, 17, 1, 0.6f, 0, 10);
	m_lights->addPointLight(-67, 12, -1.6f, 50, 1, 1, 0.6f, 15);


	// Initialize resource spawning lists
	loadLists();
	startSpawn();

	// Initialize spawning drone
	m_droneLightIndex = m_lights->addSpotLight(0, 150, 0, 150, 0, -1, 0, 1, 1, 1, 40, 7);
	//m_droneLightIndex = m_lights->addAreaLight(-119, 3, 99, TRAVEL_HEIGHT, 1, 1, 1, 30);
	m_spawnDroneState = -1;
	m_heldResourceIndex = -1;
	m_spawnDroneRotating = false;
	m_spawnDroneTravelling = false;
	m_transportDestination = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	m_transportDirection = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	m_travelTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	m_travelDirection = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	m_spawnDroneBody.setPosition(DRONE_START);
	XMVECTOR droneStart = DRONE_START;
	m_lights->setPosition(
		m_droneLightIndex,
		droneStart.m128_f32[0],
		droneStart.m128_f32[1] + LIGHT_OFFSET,
		droneStart.m128_f32[2]
	);
	m_spawnDronePropeller[0].setPosition(7.0692f, 0.64566f, -4.934334f);
	m_spawnDronePropeller[1].setPosition(7.0692f, 0.64566f, 4.934334f);
	m_spawnDronePropeller[2].setPosition(-7.0692f, 0.64566f, -4.934334f);
	m_spawnDronePropeller[3].setPosition(-7.0692f, 0.64566f, 4.934334f);

	// Initialize dynamic camera
	m_vecToCam = XMVector3Normalize(DX::getInstance()->getCam()->getPosition() - DX::getInstance()->getCam()->getLookAt());
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

void GameState::setTravelTarget(XMVECTOR target)
{
	// Set travel destination
	m_spawnDroneTravelling = true;
	m_travelTarget = target;
	m_travelDirection = XMVector3Normalize(m_travelTarget - m_spawnDroneBody.getPosition());
}

void GameState::setRotationTarget(XMVECTOR target)
{
	// Enable rotation to target if wanted
	m_spawnDroneRotating = true;
	XMVECTOR direction = target - m_spawnDroneBody.getPosition();
	direction.m128_f32[1] = 0.0f;
	direction.m128_f32[3] = 0.0f;
	m_transportDirection = XMVector3Normalize(direction);
	m_transportDirection.m128_f32[1] = 0.0f; // Skip y-axis
}

bool GameState::travelAndCheck(float dT, bool fastTravel)
{
	if (m_spawnDroneTravelling)
	{
		XMVECTOR pos = m_spawnDroneBody.getPosition();
		pos = XMVector3Length(m_travelTarget - pos);
		float distanceToTarget = pos.m128_f32[0];
		if (distanceToTarget > TRAVEL_THRESHOLD)
		{
			XMVECTOR movement = m_travelDirection * dT;
			// Decide movement speed
			if (fastTravel)
				movement *= FAST_TRAVEL_SPEED;
			else
				movement *= SLOW_TRAVEL_SPEED;



			// If target passed, teleport to target
			float movedDistance = XMVector3Length(movement).m128_f32[0];
			if (movedDistance > distanceToTarget)
				m_spawnDroneBody.setPosition(m_travelTarget); // Set drone
			else
				m_spawnDroneBody.move(movement); // Move drone

			pos = m_spawnDroneBody.getPosition();

			// Set resource position
			if (m_heldResourceIndex != -1)
			{

				m_resources[m_heldResourceIndex]->setPosition(XMVectorSet
				(
					(float)(pos.m128_f32[0]),
					(float)(pos.m128_f32[1]) + RESOURCE_OFFSET,
					(float)(pos.m128_f32[2]),
					0.0f)
				);
			}

			// Set light position
			m_lights->setPosition(
				m_droneLightIndex,
				pos.m128_f32[0],
				pos.m128_f32[1] + LIGHT_OFFSET,
				pos.m128_f32[2]
			);
		}
		else
			m_spawnDroneTravelling = false;
	}

	if (m_spawnDroneRotating)
	{
		// Check what direction to rotate
		objectData temp = m_spawnDroneBody.getData();
		float rotation = XM_PI * temp.rotation.m128_f32[3] / 180.0f;
		XMVECTOR droneOrientation = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		droneOrientation.m128_f32[0] = XMScalarSin(rotation);
		droneOrientation.m128_f32[2] = XMScalarCos(rotation);
		XMVECTOR tempVec = XMVector3Dot(m_transportDirection, droneOrientation);
		float dotProduct = tempVec.m128_f32[0];
		float deltaRotation = XMScalarACos(dotProduct);

		// Check if rotation is sufficient, if so disable rotation, else rotate
		if (deltaRotation < ROTATION_THRESHOLD)
			m_spawnDroneRotating = false;
		else
		{
			// Rotate
			XMVECTOR cross = XMVector3Cross(droneOrientation, m_transportDirection);
			float increment;
			if (cross.m128_f32[1] > 0)
				increment = ROTATION_SPEED * dT;

			else
				increment = -ROTATION_SPEED * dT;

			m_spawnDroneBody.rotate(0.0f, 1.0f, 0.0f, increment);
		}
	}

	return !m_spawnDroneTravelling && !m_spawnDroneRotating;
}

bool GameState::assignMission()
{
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
		// Reset time since last spawn
		while (m_collectedTime >= SPAWN_INTERVAL)
			m_collectedTime -= SPAWN_INTERVAL;

		// Randomize whether it is a normal or special resource
		bool isSpecial = false;
		if (rand() % 100 < SPECIAL_RESOURCE_CHANCE)
		{
			// Only make resource special if there are available spots
			for (int i = 0; i < m_specialSpawnAmount && !isSpecial; i++)
			{
				if (m_freeSpawns[(int)(m_normalSpawnAmount)+(int)i])
					isSpecial = true;
			}
		}


		Resource* resource;
		int spawnIndex;
		if (isSpecial)
		{
			spawnIndex = getSpecialSpawnIndex(); // TODO:: Change spawn types
			//resource = new Resource(true, spawnIndex, rand() % BIGGEST_NORMAL_INDEX, 3.0f);
			resource = new Resource(true, spawnIndex, RIFLE, 3.0f);
		}

		else
		{
			spawnIndex = getSpawnIndex(); // TODO: Edit INDEX FOR SPECIAL BELOW
			//resource = new Resource(true, spawnIndex, rand() % BIGGEST_NORMAL_INDEX, 1.2f);
			resource = new Resource(true, spawnIndex, (rand() % 4) + 2, 1.2f);
		}

		// Set resource under drone
		XMVECTOR pos = m_spawnDroneBody.getPosition();
		resource->setPosition(XMVectorSet
		(
			(float)(pos.m128_f32[0]),
			(float)(pos.m128_f32[1]) + RESOURCE_OFFSET,
			(float)(pos.m128_f32[2]),
			0.0f)
		);
		m_resources.push_back(resource);
		m_heldResourceIndex = (int)m_resources.size() - 1;

		// Set transport destination
		XMFLOAT2 destination = m_spawns[spawnIndex];
		m_transportDestination = XMVectorSet(destination.x, FINAL_HEIGHT, destination.y, 0.0f);

		return true;
	}

	return  false;
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

bool GameState::update(Game* game, float dt)
{
	m_input = game->getInput();
	m_robots = game->getRobots();
	handleInputs(game, dt);
	game->updatePlayerStatus();

	// Update dynamic camera
	updateDynamicCamera();

	// Update spawning drone
	updateSpawnDrone(dt);

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

	// COLLISION PROJECTILES VS STATIC OBJECTS
	boundingData projectileBD = game->getPreLoader()->getBoundingData(objectType::e_projectile, 0, 0);
	boundingData robotBD = game->getPreLoader()->getBoundingData(objectType::e_robot, 1, 0);
	for (int i = 0; i < ProjectileBank::getInstance()->getList().size(); i++)
	{
		// Save projectile pointer
		Projectile* projectile = ProjectileBank::getInstance()->getList()[i];

		// Update bounding data
		projectileBD.pos = projectile->getData().pos;

		// Test collision
		CollisionInfo collisionInfo = game->getQuadtree()->testCollision(projectileBD);

		// Remove based on conditions
		if (collisionInfo.m_colliding)
		{
			// Collision against static object found, remove projectile
			ProjectileBank::getInstance()->removeProjectile(i);
		}
		else if (XMVectorGetX(XMVector3Length(projectile->getPosition())) > 200.0f)
		{
			// Remove after a certain distance from origin
			ProjectileBank::getInstance()->removeProjectile(i);
		}
		else
		{
			// Bullet has not yet been removed by previous collision
			// COLLISION PROJECTILE VS PLAYERS
			for (int j = 0; j < XUSER_MAX_COUNT; j++)
			{
				if (m_robots[j] != nullptr && m_robots[j]->isDrawn())
				{
					robotBD.pos = m_robots[j]->getPosition();
					collisionInfo = testMovingSphereSphere(robotBD.pos, projectileBD.pos, robotBD.halfWD.x, projectileBD.halfWD.x, m_robots[j]->getVel() * dt, projectile->getDirection() * projectile->getVelocity() * dt);


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
						break;
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



	return 0;
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
		game->getPreLoader()->drawOneModel(objectType::e_drone, m_spawnDroneBody.getData(), 0);
		game->getPreLoader()->drawOneModel(objectType::e_drone, m_spawnDronePropeller[0].getData(), m_spawnDroneBody.getData(), 1);
		game->getPreLoader()->drawOneModel(objectType::e_drone, m_spawnDronePropeller[1].getData(), m_spawnDroneBody.getData(), 1);
		game->getPreLoader()->drawOneModel(objectType::e_drone, m_spawnDronePropeller[2].getData(), m_spawnDroneBody.getData(), 1);
		game->getPreLoader()->drawOneModel(objectType::e_drone, m_spawnDronePropeller[3].getData(), m_spawnDroneBody.getData(), 1);
		for (int i = 0; i < m_nodes.size(); i++)
		{
			game->getPreLoader()->draw(objectType::e_node, m_nodes[i]->getData(), 0, 0);
		}
	}

}
