#include "SpawnDrone.h"

void SpawnDrone::loadLists()
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
	m_normalSpawnAmount = (int)(m_spawns.size());

	// Special spawns
	m_spawns.push_back({ -163, 120 });
	m_spawns.push_back({ -60, 85 });
	m_spawns.push_back({ 56, 50 });
	m_spawns.push_back({ 155, 28 });
	m_specialSpawnAmount = (int)(m_spawns.size() - m_normalSpawnAmount);

	// Free spawn bool list
	for (int i = 0; i < m_spawns.size(); i++)
		m_freeSpawns.push_back(true);
}

void SpawnDrone::startSpawn()
{
	for (int i = 0; i < START_SPAWNED_RESOURCES; i++)
	{
		int spawnIndex = getSpawnIndex(); // TODO:: Change spawn types
		//Resource* resource = new Resource(false, spawnIndex, i % BIGGEST_NORMAL_INDEX, 0.8f);
		//Resource* resource = new Resource(false, spawnIndex, (rand() % 5) + 2, 0.8f);
		Resource* resource = new Resource(false, spawnIndex, 6, 0.8f);
		XMFLOAT2 pos = m_spawns[spawnIndex];
		resource->setPosition(XMVectorSet((float)(pos.x), 0.6f, (float)(pos.y), 0.0f));
		m_resources->push_back(resource);
	}
}

int SpawnDrone::getSpawnIndex()
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

int SpawnDrone::getSpecialSpawnIndex()
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

void SpawnDrone::setTravelTarget(XMVECTOR target)
{
	// Set travel destination
	m_spawnDroneTravelling = true;
	m_travelTarget = target;
	m_travelDirection = XMVector3Normalize(m_travelTarget - m_spawnDroneBody.getPosition());
}

void SpawnDrone::setRotationTarget(XMVECTOR target)
{
	// Enable rotation to target if wanted
	m_spawnDroneRotating = true;
	XMVECTOR direction = target - m_spawnDroneBody.getPosition();
	direction.m128_f32[1] = 0.0f;
	direction.m128_f32[3] = 0.0f;
	m_transportDirection = XMVector3Normalize(direction);
	m_transportDirection.m128_f32[1] = 0.0f; // Skip y-axis
}

bool SpawnDrone::travelAndCheck(float dT, bool fastTravel)
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
				(*m_resources)[m_heldResourceIndex]->setPosition(XMVectorSet
				(
					(float)(pos.m128_f32[0]),
					(float)(pos.m128_f32[1]) + RESOURCE_OFFSET,
					(float)(pos.m128_f32[2]),
					0.0f)
				);
			}

			// Set light position
			Lights::getInstance()->setPosition(
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

bool SpawnDrone::assignMission(Robot** robots)
{
	// Calculate number of players
	int nrOfPlayers = 0;
	for (int i = 0; i < 4; i++)
	{
		if (robots[i] != nullptr && robots[i]->isDrawn())
			nrOfPlayers++;
	}

	// Spawn resources according to max allowed
	int maxResources = MAX_RESOURCES_OUT_PER_PLAYER * nrOfPlayers;
	if (maxResources > m_resources->size()) // If there's room for a resource, spawn one
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
				if (m_freeSpawns[(int)((m_normalSpawnAmount)+i)])
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
		m_resources->push_back(resource);
		m_heldResourceIndex = (int)m_resources->size() - 1;

		// Set transport destination
		XMFLOAT2 destination = m_spawns[spawnIndex];
		m_transportDestination = XMVectorSet(destination.x, FINAL_HEIGHT, destination.y, 0.0f);

		return true;
	}

	return  false;
}

SpawnDrone::SpawnDrone(std::vector<Resource*>* m_resourcesPtr)
{
	m_resources = m_resourcesPtr;
	m_collectedTime = 0.0f;

	// Initialize resource spawning lists
	loadLists();
	startSpawn();

	// Initialize spawning drone
	m_droneLightIndex = Lights::getInstance()->addSpotLight(0, 150, 0, 150, 0, -1, 0, 1, 1, 1, 40, 7);
	//m_droneLightIndex = Lights::getInstance()->addAreaLight(-119, 3, 99, TRAVEL_HEIGHT, 1, 1, 1, 30);
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
	Lights::getInstance()->setPosition(
		m_droneLightIndex,
		droneStart.m128_f32[0],
		droneStart.m128_f32[1] + LIGHT_OFFSET,
		droneStart.m128_f32[2]
	);
	m_spawnDronePropeller[0].setPosition(7.0692f, 0.64566f, -4.934334f);
	m_spawnDronePropeller[1].setPosition(7.0692f, 0.64566f, 4.934334f);
	m_spawnDronePropeller[2].setPosition(-7.0692f, 0.64566f, -4.934334f);
	m_spawnDronePropeller[3].setPosition(-7.0692f, 0.64566f, 4.934334f);
}

SpawnDrone::~SpawnDrone()
{
}

void SpawnDrone::update(Robot** robots, float dT)
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
			if (assignMission(robots)) // If mission assignable, advance state
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
		(*m_resources)[m_heldResourceIndex]->setPosition(m_transportDestination);
		(*m_resources)[m_heldResourceIndex]->setBlocked(false);
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

void SpawnDrone::freeSpawn(int index)
{
	m_freeSpawns[index] = true;
}

void SpawnDrone::ifHeldDecreaseResourceIndex()
{
	if (m_heldResourceIndex != -1)
		m_heldResourceIndex--;
}

objectData SpawnDrone::getData(int model)
{
	switch (model)
	{
	case 0:
		return m_spawnDronePropeller[0].getData();
		break;

	case 1:
		return m_spawnDronePropeller[1].getData();
		break;

	case 2:
		return m_spawnDronePropeller[2].getData();
		break;

	case 3:
		return m_spawnDronePropeller[3].getData();
		break;

	default:
		return m_spawnDroneBody.getData();
		break;
	}
}
