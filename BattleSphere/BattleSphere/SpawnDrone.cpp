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
	m_spawns.push_back({ 109, -85 });
	m_spawns.push_back({ 75, -85 });
	m_spawns.push_back({ 40, -85 });
	m_spawns.push_back({ 5, -85 });
	m_spawns.push_back({ -30, -85 });
	m_spawns.push_back({ -65, -85 });
	m_spawns.push_back({ 44, -65 });
	m_spawns.push_back({ -61, -70 });
	m_spawns.push_back({ -39, -70 });
	m_spawns.push_back({ -50, -81 });
	m_spawns.push_back({ -50, -59 });

	// Special spawns
	m_spawns.push_back({ -163, 120 });
	m_spawns.push_back({ -60, 85 });
	m_spawns.push_back({ 56, 50 });
	m_spawns.push_back({ 155, 28 });
	m_normalSpawnAmount = (int)(m_spawns.size());
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
		Resource* resource = new Resource(false, spawnIndex, rand() % BIGGEST_NORMAL_INDEX + 1, 0.8f);
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
	Sound::getInstance()->play(soundAmbient::e_drone, m_spawnDroneBody.getPosition(), 0.2f, 0.65f);
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
		/*
		if (rand() % 100 < SPECIAL_RESOURCE_CHANCE)
		{
			// Only make resource special if there are available spots
			for (int i = 0; i < m_specialSpawnAmount && !isSpecial; i++)
			{
				if (m_freeSpawns[(int)((m_normalSpawnAmount)+i)])
					isSpecial = true;
			}
		}
		*/

		Resource* resource;
		int spawnIndex;
		if (isSpecial)
		{
			spawnIndex = getSpecialSpawnIndex(); // TODO:: Change spawn types
			//resource = new Resource(true, spawnIndex, rand() % BIGGEST_NORMAL_INDEX, 3.0f);
			resource = new Resource(true, spawnIndex, ENERGY, 3.0f);
		}

		else
		{
			spawnIndex = getSpawnIndex(); // TODO: Edit INDEX FOR SPECIAL BELOW
			resource = new Resource(true, spawnIndex, rand() % BIGGEST_NORMAL_INDEX, 1.2f);
			//resource = new Resource(true, spawnIndex, 6, 1.2f);
		}

		// Set vector for icon warning
		m_nextSpawnWarning = XMVectorSet((float)resource->getType() + 1.0f, 0.0f, 0.0f, 0.0f);

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

bool SpawnDrone::translateDoor(float dt, bool open)
{
	if (open)
	{
		m_BSPDdoor.setPosition(0.0f, m_BSPDdoor.getPosition().m128_f32[1] - dt * 15.0f, 0.0f);
		if (m_BSPDdoor.getPosition().m128_f32[1] <= BSPD_DOOR_OPEN)
		{
			m_BSPDdoor.setPosition(0.0f, BSPD_DOOR_OPEN, 0.0f);
			return true;
		}
	}
	else
	{
		m_BSPDdoor.setPosition(0.0f, m_BSPDdoor.getPosition().m128_f32[1] + dt * 15.0f, 0.0f);
		if (m_BSPDdoor.getPosition().m128_f32[1] >= BSPD_DOOR_CLOSED)
		{
			m_BSPDdoor.setPosition(0.0f, BSPD_DOOR_CLOSED, 0.0f);
			return true;
		}
	}
	return false;
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
	m_spawnDroneBody.rotate(0.0f, 1.0f, 0.0f, 150.0f);
	m_spawnDroneBody.scale(-0.2f, -0.2f, -0.2f);
	m_spawnDronePropeller[0].scale(-0.2f, -0.2f, -0.2f);
	m_spawnDronePropeller[1].scale(-0.2f, -0.2f, -0.2f);
	m_spawnDronePropeller[2].scale(-0.2f, -0.2f, -0.2f);
	m_spawnDronePropeller[3].scale(-0.2f, -0.2f, -0.2f);
	m_spawnDroneBody.setPosition(DRONE_START);
	m_BSPDdoor.setPosition(0.0f, BSPD_DOOR_CLOSED, 0.0f);

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

	m_nextSpawnWarning = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	m_constantBufferIcons = new ConstantBuffer(&m_nextSpawnWarning, sizeof(XMVECTOR));
}

SpawnDrone::~SpawnDrone()
{
	Lights::getInstance()->disable(m_droneLightIndex);
	delete m_constantBufferIcons;
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
	case 0:
		if (translateDoor(dT, true))
			m_spawnDroneState++;
		break;
	case 1: // Set to rise spot
		XMVECTOR target = DRONE_RISE_START;
		setTravelTarget(target);
		m_spawnDroneState++;
		break;
	case 2: // Travel to rise spot 
		translateDoor(dT, false);
		if (travelAndCheck(dT, false))

			m_spawnDroneState++;
		break;
	case 3: // Set target to rising
		setRotationTarget(m_transportDestination);
		if (m_transportDestination.m128_f32[0] < 0.0f && m_transportDestination.m128_f32[2] > 30.0f) // Upper left
		{
			if (travelAndCheck(dT, false))
				m_spawnDroneState = 20;
		}
		else if (m_transportDestination.m128_f32[0] > 0.0f && m_transportDestination.m128_f32[2] > 0.0f) // Upper right
		{
			setRotationTarget(DRONE_MIDPOINT);
			if (travelAndCheck(dT, false))
			{
				m_spawnDroneState = 29;
				target = DRONE_MIDPOINT;
				target.m128_f32[1] = TRAVEL_HEIGHT;
				setTravelTarget(target);
			}
		}
		else
		{
			target = m_spawnDroneBody.getPosition();
			target.m128_f32[1] = TRAVEL_HEIGHT;
			setTravelTarget(target);
			m_spawnDroneState++;
		}
		break;
	case 4: // Rise 
		translateDoor(dT, false);
		if (travelAndCheck(dT, false))
			m_spawnDroneState++;
		break;

	case 5: // Set target above transport location
		target = m_transportDestination;
		target.m128_f32[1] = TRAVEL_HEIGHT;
		setTravelTarget(target);
		m_spawnDroneState++;
		break;

	case 6: // Travel
		translateDoor(dT, false);
		if (travelAndCheck(dT, true))
			m_spawnDroneState++;
		break;

	case 7: // Set target to decline down to spawn point
		target = m_transportDestination;
		target.m128_f32[1] -= RESOURCE_OFFSET;
		setTravelTarget(target);
		m_spawnDroneState++;
		break;

	case 8: // Decline 
		if (travelAndCheck(dT, false))
			m_spawnDroneState++;
		break;

	case 9: // Leave resource
		(*m_resources)[m_heldResourceIndex]->setPosition(m_transportDestination);
		(*m_resources)[m_heldResourceIndex]->setBlocked(false);
		m_heldResourceIndex = -1;
		m_nextSpawnWarning = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		m_spawnDroneState++;
		break;

	case 10: // Set target above transport location, rotate to drone start or midpoint
		target = m_transportDestination;
		target.m128_f32[1] = TRAVEL_HEIGHT;
		setTravelTarget(target);
		if (m_transportDestination.m128_f32[0] > 0.0f && m_transportDestination.m128_f32[2] > 0.0f)
		{
			setRotationTarget(DRONE_MIDPOINT);
			m_spawnDroneState = 30;
		}
		else
			setRotationTarget(DRONE_RISE_START);
		m_spawnDroneState++;
		break;

	case 11: // Rise 
		if (travelAndCheck(dT, false))
			m_spawnDroneState++;
		break;

	case 12: // Set target above drone rise start
		target = DRONE_RISE_START;
		target.m128_f32[1] = TRAVEL_HEIGHT;
		setTravelTarget(target);
		m_spawnDroneState++;
		break;

	case 13: // Travel
		if (travelAndCheck(dT, true))
			m_spawnDroneState++;
		break;

	case 14: // Set target to decline down to drone rise start  
		setRotationTarget(-DRONE_START);
		setTravelTarget(DRONE_RISE_START);
		m_spawnDroneState++;
		break;

	case 15: // Decline
		if (travelAndCheck(dT, false))
			m_spawnDroneState++;
		break;

	case 16: // Set target to BSPD building
		setTravelTarget(DRONE_START);
		m_spawnDroneState++;
		break;

	case 17: // Open door and then travel to start
		if (translateDoor(dT, true))
		{
			if (travelAndCheck(dT, false))
				m_spawnDroneState++;
		}
		break;
	case 18:
		if (translateDoor(dT, false))
			m_spawnDroneState++;
		break;

	case 19: // Reset switch
		//m_spawnDroneState++;
		m_spawnDroneState = -1;
		Sound::getInstance()->stop(soundAmbient::e_drone);
		break;

	case 20: // Set travel (without rise)
		target = m_transportDestination;
		setTravelTarget(target);
		m_spawnDroneState++;
		break;

	case 21: // Travel (without rise)
		if (travelAndCheck(dT, false))
			m_spawnDroneState++;
		break;

	case 22: // Leave (without rise)
		(*m_resources)[m_heldResourceIndex]->setPosition(m_transportDestination);
		(*m_resources)[m_heldResourceIndex]->setBlocked(false);
		m_heldResourceIndex = -1;
		m_nextSpawnWarning = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		m_spawnDroneState++;
		break;

	case 23: // Set rotate to rise start
		setRotationTarget(DRONE_RISE_START);
		m_spawnDroneState++;
		break;

	case 24: // Rotate to rise start
		if (travelAndCheck(dT, false))
			m_spawnDroneState++;

	case 25:
		setTravelTarget(DRONE_RISE_START);
		m_spawnDroneState++;
		break;

	case 26:
		if (travelAndCheck(dT, false))
			m_spawnDroneState++;
		break;

	case 27:
		setRotationTarget(-DRONE_START);
		m_spawnDroneState++;
		break;

	case 28:
		translateDoor(dT, true);
		if (travelAndCheck(dT, false))
			m_spawnDroneState = 16;
		break;

	case 29: // Rise to mid point 
		translateDoor(dT, false);
		setRotationTarget(m_transportDestination);
		if (travelAndCheck(dT, false))
			m_spawnDroneState = 5;
		break;

	case 30: // Rise 
		if (travelAndCheck(dT, false))
			m_spawnDroneState++;
		break;

	case 31: // Travel to midpoint
		target = DRONE_MIDPOINT;
		target.m128_f32[1] = TRAVEL_HEIGHT;
		setTravelTarget(target);
		m_spawnDroneState++;
		break;

	case 32: 
		if (travelAndCheck(dT, false))
			m_spawnDroneState = 14;
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

	case 4:
		return m_BSPDdoor.getData();
		break;
	default:
		return m_spawnDroneBody.getData();
		break;
	}
}

void SpawnDrone::setConstantBuffer(bool on)
{
	D3D11_MAPPED_SUBRESOURCE mappedMemory;
	if (on)
	{
		DX::getInstance()->getDeviceContext()->Map(*m_constantBufferIcons->getConstantBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
		memcpy(mappedMemory.pData, &m_nextSpawnWarning, sizeof(XMVECTOR));
		DX::getInstance()->getDeviceContext()->Unmap(*m_constantBufferIcons->getConstantBuffer(), 0);

		DX::getInstance()->getDeviceContext()->PSSetConstantBuffers(6, 1, m_constantBufferIcons->getConstantBuffer());
	}
	else
	{
		DX::getInstance()->getDeviceContext()->Map(*m_constantBufferIcons->getConstantBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
		memcpy(mappedMemory.pData, &XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f), sizeof(XMVECTOR));
		DX::getInstance()->getDeviceContext()->Unmap(*m_constantBufferIcons->getConstantBuffer(), 0);

		DX::getInstance()->getDeviceContext()->PSSetConstantBuffers(6, 1, m_constantBufferIcons->getConstantBuffer());
	}
}
