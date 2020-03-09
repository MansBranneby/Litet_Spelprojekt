#include "ScoreState.h"

void ScoreState::spawnNodes()
{
	Node* node = new Node(rand() % 3);
	//node->setPosition(XMVectorSet(150.0f, 0.2f, 120.0f, 0.0f));
	//node->setRotation(0.0f, 1.0f, 0.0f, 0.0f);
	m_nodes.push_back(node);
	node = new Node(rand() % 3);
	//node->setPosition(XMVectorSet(106.0f, 0.2f, -18.0f, 0.0f));
	//node->setRotation(0.0f, 1.0f, 0.0f, 90.0f);
	m_nodes.push_back(node);
	node = new Node(rand() % 3);
	//node->setPosition(XMVectorSet(-100.0f, 0.3f, -50.0f, 0.0f));
	//node->setRotation(0.0f, 1.0f, 0.0f, 90.0f);
	m_nodes.push_back(node);
	node = new Node(rand() % 3);
	//node->setPosition(XMVectorSet(-120.0f, 0.2f, -12.0f, 0.0f));
	//node->setRotation(0.0f, 1.0f, 0.0f, 0.0f);
	m_nodes.push_back(node);
}

void ScoreState::updateDynamicCamera(float dT)
{
	//// Calculate number of players
	//int nrOfPlayers = 0;
	//for (int i = 0; i < XUSER_MAX_COUNT; i++)
	//{
	//	if (m_robots[i] != nullptr && m_robots[i]->isDrawn())
	//		nrOfPlayers++;
	//}

	//if (nrOfPlayers >= 1)
	//{
	//	// Get new look at and get min and max of x and z
	//	XMVECTOR oldCamLookAt = DX::getInstance()->getCam()->getLookAt();
	//	XMVECTOR oldCamPos = DX::getInstance()->getCam()->getPosition();
	//	XMVECTOR newLookAt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	//	XMVECTOR robPos;
	//	float minX = 2000;
	//	float maxX = -2000;
	//	float minZ = 2000;
	//	float maxZ = -2000;
	//	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	//	{
	//		if (m_robots[i] != nullptr && m_robots[i]->isDrawn())
	//		{
	//			robPos = m_robots[i]->getPosition();
	//			newLookAt += robPos;
	//			robPos = XMVector3Normalize(m_robots[i]->getPosition() - oldCamPos);

	//			if (minX > robPos.m128_f32[0])
	//				minX = robPos.m128_f32[0];

	//			if (maxX < robPos.m128_f32[0])
	//				maxX = robPos.m128_f32[0];

	//			if (minZ > robPos.m128_f32[2])
	//				minZ = robPos.m128_f32[2];

	//			if (maxZ < robPos.m128_f32[2])
	//				maxZ = robPos.m128_f32[2];
	//		}
	//	}

	//	// Set look at between players and move it upp the z-axis slightly
	//	newLookAt /= (float)nrOfPlayers;
	//	newLookAt.m128_f32[3] -= 100.0f;

	//	// Calculate biggest distance
	//	float xDifference = maxX - minX;
	//	float zDifference = maxZ - minZ;
	//	float biggestDifference;
	//	if (xDifference > zDifference)
	//		biggestDifference = xDifference;
	//	else
	//		biggestDifference = zDifference;

	//	// Set new camera position and look at

	//	XMVECTOR newPos;
	//	if (m_zoomingOutToStart) // If zooming out, have harder criteria to zoom in again
	//	{
	//		if (MINIMUM_CAM_DISTANCE + biggestDifference * 250.0f < MAXIMUM_CAM_DISTANCE)
	//		{
	//			newPos = newLookAt + m_vecToCam * (MINIMUM_CAM_DISTANCE + biggestDifference * 50.0f);
	//			if (newPos.m128_f32[2] < -105.0f) // Limit camera movement in z-axis
	//			{
	//				float difference = -105.0f - newPos.m128_f32[2];
	//				newPos.m128_f32[2] += difference;
	//				newLookAt.m128_f32[2] += difference;
	//			}
	//			m_zoomingOutToStart = false;
	//		}
	//	}
	//	else // If not zooming out
	//	{
	//		if (MINIMUM_CAM_DISTANCE + biggestDifference * 120.0f < MAXIMUM_CAM_DISTANCE)
	//		{
	//			newPos = newLookAt + m_vecToCam * (MINIMUM_CAM_DISTANCE + biggestDifference * 50.0f);
	//			if (newPos.m128_f32[2] < -105.0f) // Limit camera movement in z-axis
	//			{
	//				float difference = -105.0f - newPos.m128_f32[2];
	//				newPos.m128_f32[2] += difference;
	//				newLookAt.m128_f32[2] += difference;
	//			}
	//		}
	//		else
	//		{
	//			m_zoomingOutToStart = true;
	//			newPos = m_camStartPos;
	//			newLookAt = m_camStartLookAt;
	//		}
	//	}

	//	// Project each robot onto FOV planes and find smallest distances
	//	float closest = INFINITY;
	//	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	//	{
	//		if (m_robots[i] != nullptr && m_robots[i]->isDrawn())
	//		{
	//			robPos = m_robots[i]->getPosition();
	//			XMVECTOR camToBot = XMVector3Normalize(robPos - oldCamPos);
	//			for (int plane = 0; plane < 4; plane++) // Bottom, left, top, right
	//			{
	//				// Project
	//				float distance = XMVector3Dot(camToBot, m_fOVPlanes[plane]).m128_f32[0];

	//				if (distance < closest)
	//					closest = distance;
	//			}
	//		}
	//	}

	//	float changeSpeed = dT * CHANGE_SPEED;
	//	if (closest < 0.01f) // Limit closest to avoid zero and negative speeds
	//		closest = 0.01f;

	//	if (m_zoomingOutToStart || m_devZoomOut) // If zooming out to start pos is true, set the position and look at, also increase speeds
	//	{
	//		changeSpeed *= 2.0f;
	//		newPos = m_camStartPos;
	//		newLookAt = m_camStartLookAt;
	//	}
	//	else if (closest < 0.6f) // Change "changeSpeed" according to how close a player is to a camera plane
	//	{
	//		changeSpeed *= 0.8f / closest;
	//		newPos += m_vecToCam * (biggestDifference * 90.0f);
	//	}

	//	XMVECTOR vecToNewCamPos = newPos - oldCamPos;
	//	XMVECTOR vecToNewCamLookAt = newLookAt - oldCamLookAt;
		DirectX::XMVECTOR lookAt{ 35.0f, 40.0f, -60.0f };
		DirectX::XMVECTOR camPos{ 35.0f, 25.0f, -130.0f };
	//	//DX::getInstance()->getCam()->movePosAndLook(vecToNewCamPos * changeSpeed, vecToNewCamLookAt * changeSpeed);
		//DX::getInstance()->getCam()->movePosAndLook({ 0.0f, 0.0f, -10.0f }, { 0.0f, 0.0f, 0.0f });
	//}
		DX::getInstance()->getCam()->setCameraPosition(camPos);
		DX::getInstance()->getCam()->setLookAt(lookAt);
}

void ScoreState::handleMovement(Game* game, float dt, int id)
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
}

void ScoreState::handleInputs(Game* game, float dt)
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
			
				handleMovement(game, dt, i);


				// Camera dev zoom
				if (m_input->isPressed(i, XINPUT_GAMEPAD_BACK))
				{
					m_devZoomOut = true;
				}
				if (m_input->isPressed(i, XINPUT_GAMEPAD_START))
				{
					m_devZoomOut = false;
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
			boundingData robotBD = game->getPreLoader()->getBoundingData(ObjectType::e_robot, 1, 0);
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
			// TODO GLENN
			//newPos = { 165.0f, 46.0f, 135.0f };
			m_robots[i]->setPosition(newPos);
			m_robots[i]->storePositionInHistory(newPos);
		}
	}
}

ScoreState::ScoreState(Game* game)
{
	m_devZoomOut = false;
	srand((unsigned int)time(NULL));

	m_type = stateType::e_scoreState;
	m_input = nullptr;
	m_robots = nullptr;

	// Spawn preset nodes and initialize spawning drone
	m_spawnDrone = new SpawnDrone(&m_resources);
	spawnNodes();

	// Create billboards
	std::vector<ObjectType> billboardObjectTypes = {ObjectType::e_static_billboard_score, ObjectType::e_billboard, ObjectType::e_number_billboard};
	m_billboardHandler = BillboardHandler(game->getPreLoader(), billboardObjectTypes);

	m_transparency.initialize();
	m_transparency.bindConstantBuffer();
	m_lights = Lights::getInstance();
	int index = m_lights->addPointLight(-10, 25, 0, 55, 1, 0.5f, 0.125f, 1);
	m_lights->setColor(index, float(255) / 255, float(0) / 255, float(97) / 255);
	index = m_lights->addSpotLight(-2.5f, 11.67f, -67, 17, -0.33f, -1, 0.0f, 1.0f, 1.0f, 0.0f, 27, 20);
	index = m_lights->addSpotLight(2.5f, 11.67f, -67, 17, 0.33f, -1, 0.0f, 1.0f, 1.0f, 0.0f, 27, 20);
	index = m_lights->addVolumetricSpotLight(133.0f, 38.0f, -29.0f, 70.0f, -0.6f, -0.8f, -0.3f, 0.15f, 0.97f, 1.0f, 20.0f, 13.0f); // Headlights construction
	//m_lights->addAreaLight(-52, 11.67f, -72, 17, 1, 1, 0, 5);
	//m_lights->addAreaLight(46, 8, -60, 17, 1, 0, 1, 5);
	//m_lights->addAreaLight(78, 18, 70, 50, 1, 0.5f, 0, 25);
	//m_lights->addAreaLight(-5, 18, 75, 33, 0, 1, 1, 10);
	//m_lights->addAreaLight(33, 10, 67, 50, 0, 0, 1, 15);
	//m_lights->addAreaLight(178, 10, 67, 50, 1, 1, 0, 20);
	//m_lights->addAreaLight(150, 10, 55, 17, 1, 0, 0, 20);
	//m_lights->addAreaLight(-119, 3, 99, 17, 1, 0.6f, 0, 10);
	index = m_lights->addVolumetricSpotLight(133.0f, 38.0f, -29.0f, 90.0f, -0.6f, -0.8f, -0.3f, 0.15f, 0.97f, 1.0f, 20.0f, 13.0f); // Headlights construction
	//m_lights->addAreaLight(-52, 11.67f, -72, 17, 1, 1, 0, 5);
	//m_lights->addAreaLight(46, 8, -60, 17, 1, 0, 1, 5);
	//m_lights->addAreaLight(-5, 18, 75, 33, 0, 1, 1, 10);
	//m_lights->addAreaLight(33, 10, 67, 50, 0, 0, 1, 15);
	//m_lights->addAreaLight(178, 10, 67, 50, 1, 1, 0, 20);
	//m_lights->addAreaLight(150, 10, 55, 17, 1, 0, 0, 20);
	//m_lights->addAreaLight(-119, 3, 99, 17, 1, 0.6f, 0, 10);

	// Skyscrapers
	m_lights->addAreaLight(85, 30, 75, 75, 0.0f, 0.6f, 0.8f, 25);
	m_lights->addAreaLight(85, 10, 75, 30, 1.0f, 1.0f, 1.0f, 25);
	m_lights->addAreaLight(35, 20, 77, 60, 0.5f, 0.0f, 0.8f, 25);
	m_lights->addAreaLight(172, 20, 71, 50, 0.5f, 0.0f, 0.8f, 25);
	m_lights->addAreaLight(10, 20, 80, 55, 0.0f, 0.6f, 0.8f, 23);

	// Right tunnels
	m_lights->addAreaLight(238, 8, 31, 60, 1.0f, 1.0f, 1.0f, 25);
	m_lights->addAreaLight(238, 8, 120, 30, 1.0f, 1.0f, 1.0f, 25);
	m_lights->addAreaLight(193, 47, 118, 50, 0.2f, 0.7f, 1.0f, 10);

	m_lights->addAreaLight(172, -30, 27, 50, 0.2f, 0.7f, 1.0f, 10); // Under map
	m_lights->addAreaLight(95, -30, 27, 50, 0.2f, 0.7f, 1.0f, 10);
	m_lights->addAreaLight(32, -30, 27, 50, 0.2f, 0.7f, 1.0f, 10);
	m_lights->addAreaLight(32, -30, 69, 50, 0.2f, 0.7f, 1.0f, 10);
	m_lights->addAreaLight(-20, -30, 85, 50, 0.2f, 0.7f, 1.0f, 10);
	m_lights->addAreaLight(-100, -30, 85, 50, 0.2f, 0.7f, 1.0f, 10);

	m_lights->addAreaLight(-125, 18, -9.4f, 50, 0.06f, 0.9f, 0.9f, 10); // Golden duck

	m_lights->addAreaLight(22.0f, 3.3f, 10.0f, 20.0f, 0.8f, 0.12f, 0.0f, 20.0f); // Gas station orange 1
	m_lights->addAreaLight(36.0f, 13.0f, 10.0f, 20.0f, 0.8f, 0.12f, 0.0f, 20.0f); // Gas station orange 2
	m_lights->addAreaLight(46.0f, 4.0f, -6.5f, 12.0f, 0.0f, 1.0f, 0.35f, 15.0f); // Gas station cyan
	m_lights->addAreaLight(53.0f, 11.8f, 10.0f, 20.0f, 0.8f, 0.8f, 0.8f, 15.0f); // Gas station white
	index = m_lights->addSpotLight(47.5f, 14.7f, -0.34f, 20.0f, -0.0f, -0.9f, -0.3f, 0.8f, 0.8f, 0.8f, 30.0f, 5.0f); // Gas station spotlight
	m_lights->addPointLight(-67, 12, -1.6f, 50, 1, 1, 0.6f, 15);

	// Initialize dynamic camera
	m_zoomingOutToStart = false;
	m_vecToCam = XMVector3Normalize(DX::getInstance()->getCam()->getPosition() - DX::getInstance()->getCam()->getLookAt());
	m_camStartPos = DX::getInstance()->getCam()->getPosition();
	m_camStartLookAt = DX::getInstance()->getCam()->getLookAt();
	float xFovHalf = DX::getInstance()->getCam()->getXFOV() / 2.0f;
	float yFovHalf = DX::getInstance()->getCam()->getYFOV() / 2.0f;
	m_fOVPlanes[0] = XMVector3Rotate(XMVectorSet(0.0f, 1.0f, 0.0, 0.0f), XMQuaternionRotationNormal(XMVectorSet(1.0f, 0.0f, 0.0, 0.0f), -yFovHalf)); // Bottom
	m_fOVPlanes[1] = XMVector3Rotate(XMVectorSet(1.0f, 0.0f, 0.0, 0.0f), XMQuaternionRotationNormal(XMVectorSet(0.0f, 1.0f, 0.0, 0.0f), xFovHalf)); // Left
	m_fOVPlanes[2] = XMVector3Rotate(XMVectorSet(0.0f, -1.0f, 0.0, 0.0f), XMQuaternionRotationNormal(XMVectorSet(1.0f, 0.0f, 0.0, 0.0f), yFovHalf)); // Top
	m_fOVPlanes[3] = XMVector3Rotate(XMVectorSet(-1.0f, 0.0f, 0.0, 0.0f), XMQuaternionRotationNormal(XMVectorSet(0.0f, 1.0f, 0.0, 0.0f), -xFovHalf)); // Right

	// Rotate plane according to look at
	float camAngle = XMScalarACos(XMVector3Dot(XMVectorSet(0.0, 0.0, 1.0f, 0.0f), -m_vecToCam).m128_f32[0]);
	for (int i = 0; i < 4; i++)
		m_fOVPlanes[i] = XMVector3Rotate(m_fOVPlanes[i], XMQuaternionRotationNormal(XMVectorSet(1.0f, 0.0f, 0.0, 0.0f), -camAngle)); // Bottom

	m_fOVPlanes[0].m128_f32[2] *= -1;
	m_fOVPlanes[1].m128_f32[2] *= -1;
	m_fOVPlanes[2].m128_f32[2] *= -1;
	m_fOVPlanes[3].m128_f32[2] *= -1;

	// Dynamic background object
	m_dboHandler = new DBOHandler();
}

ScoreState::~ScoreState()
{
	for (int i = 0; i < m_resources.size(); i++)
		delete m_resources[i];
	for (int i = 0; i < m_nodes.size(); i++)
		delete m_nodes[i];
	if (m_dboHandler)
		delete m_dboHandler;
	if (m_spawnDrone)
		delete m_spawnDrone;


}

void ScoreState::pause()
{
}

void ScoreState::resume()
{
}

void ScoreState::handleInput(Game* game)
{
}

bool ScoreState::update(Game* game, float dt)
{
	bool exitGame = false;
	m_input = game->getInput();
	m_robots = game->getRobots();
	handleInputs(game, dt);
	game->updatePlayerStatus();

	// Update sounds
	Sound::getInstance()->update(dt);

	// Update dynamic camera
	updateDynamicCamera(dt);

	// Update spawning drone
	m_spawnDrone->update(m_robots, dt);

	// Update particles
	m_particles.update(dt);

	// Update billboards
	m_billboardHandler.updateBillboards(dt);

	//Dynamic background objects
	m_dboHandler->update(dt);

	DirectX::XMVECTOR cyan = { 0.0f, 0.4f, 0.3f, 1.0f };
	DirectX::XMVECTOR black = { 0.0f, 0.0f, 0.0f, 0.0f };
	//m_billboardHandler.setAllStates(3, 0.1f, 1.0f, cyan, cyan, cyan);
	std::vector<Billboard> BB = m_billboardHandler.getBillboards();
	for (int i = 0; i < BB.size(); ++i)
	{	
		if (BB[i].getObjectType() == ObjectType::e_static_billboard_score)
		{
			if (i == 1.0f)
				float test = 10.0f;
			// Test each player against collision mesh
			int nrOfCollisions = 0; // Keep track of number of collisions for each  collision mesh
			std::vector<DirectX::XMFLOAT3> colMesh = game->getPreLoader()->getCollisionMesh(BB[i].getObjectType(), BB[i].getModelNr(), BB[i].getVariant()); // collision mesh
			for (int j = 0; j < XUSER_MAX_COUNT && m_robots[j] != nullptr; ++j)
			{
				for (int k = 0; k < colMesh.size(); k += 3)
				{
					// Get indices
					unsigned int ind1 = k + 1;
					unsigned int ind2 = k + 2;

					// Get vertices
					DirectX::XMVECTOR v0 = { colMesh[k].x, colMesh[k].y, colMesh[k].z };
					DirectX::XMVECTOR v1 = { colMesh[ind1].x, colMesh[ind1].y, colMesh[ind1].z };
					DirectX::XMVECTOR v2 = { colMesh[ind2].x, colMesh[ind2].y, colMesh[ind2].z };

					// Test collision between player and collision mesh
					if (testSphereTriangle(m_robots[j]->getPosition(), game->getPreLoader()->getBoundingData(ObjectType::e_robot, 0, 0).halfWD.x, v0, v1, v2).m_colliding)
					{
						// If collision detected tick up nr of collisions
						nrOfCollisions++;
						break;
					}
				}
			}
			if (nrOfCollisions == game->getNrOfPlayers())
			{
				m_billboardHandler.setAllStates(i, 0.1f, 1.0f, cyan, cyan, cyan);
				if (m_input->isPressed(0, XINPUT_GAMEPAD_A))
				{
					switch (i)
					{
					case 0:
						exitGame = true;
						break;
					case 1:
						break;
					case 2:
						setPaused(true); // Pause this state
						game->changeState(stateType::e_mainMenu); // Change state to ScoreState
						break;
					}
				}
				break;
			}
			else
			{
				m_billboardHandler.setNoneState(i);
			}
		}
	}

	return exitGame;
}

void ScoreState::draw(Game* game, renderPass pass)
{
	m_input = game->getInput();
	m_robots = game->getRobots();

	if (pass == renderPass::e_opaque)
	{
		for (int i = 0; i < XUSER_MAX_COUNT; i++)
		{
			if (m_robots[i] != nullptr && m_robots[i]->isDrawn())
			{
				game->getPreLoader()->setSubModelData(ObjectType::e_robot, game->getRobots()[i]->getData(), 1, 0);
				game->getPreLoader()->setSubModelData(ObjectType::e_robot, game->getRobots()[i]->getData(), 0, 6);
				game->getPreLoader()->draw(ObjectType::e_robot);
			
			}
		}

	}
	else if (pass == renderPass::e_transparent)
	{
		//Static
		for (int i = 0; i < game->getPreLoader()->getNrOfVariants(ObjectType::e_static); i++)
			game->getPreLoader()->draw(ObjectType::e_static, i);

		game->getPreLoader()->drawOneModel(ObjectType::e_drone, m_spawnDrone->getData(), 0);
		game->getPreLoader()->drawOneModel(ObjectType::e_drone, m_spawnDrone->getData(0), m_spawnDrone->getData(), 1);
		game->getPreLoader()->drawOneModel(ObjectType::e_drone, m_spawnDrone->getData(1), m_spawnDrone->getData(), 1);
		game->getPreLoader()->drawOneModel(ObjectType::e_drone, m_spawnDrone->getData(2), m_spawnDrone->getData(), 1);
		game->getPreLoader()->drawOneModel(ObjectType::e_drone, m_spawnDrone->getData(3), m_spawnDrone->getData(), 1);
	
		// Tokyo drift
		for (int i = 0; i < OBJECT_NR_1; i++)
		{
			if (m_dboHandler->isDrawn(i))
				game->getPreLoader()->draw(ObjectType::e_extra, m_dboHandler->getData(i));
		}
		// Scene (Background objects without collision)
		for (int i = 0; i < game->getPreLoader()->getNrOfVariants(ObjectType::e_scene); i++)
			game->getPreLoader()->draw(ObjectType::e_scene, i);

		m_particles.draw();
	}
	else if (pass == renderPass::e_billboard)
	{
		std::vector<Billboard> BB = m_billboardHandler.getBillboards();
		std::vector<Billboard> BBNumbers = m_billboardHandler.getBillboardsOfType(ObjectType::e_number_billboard);

		for (int i = 0; i < BB.size(); ++i)
		{
			if(BB[i].getObjectType() != ObjectType::e_number_billboard)
				game->getPreLoader()->draw(BB[i].getObjectType(), BB[i].getBillboardData(), BB[i].getModelNr(), BB[i].getSubModelNumber(), BB[i].getVariant());
		}

		objectData test;
		test.material = game->getRobots()[0]->getData().material;
		test.pos = { -5.0f, 0.0f, 0.0f };
		game->getPreLoader()->setSubModelData(ObjectType::e_number_billboard, test, BBNumbers[0].getModelNr(), BBNumbers[0].getSubModelNumber());
		game->getPreLoader()->draw(BBNumbers[0].getObjectType(), BBNumbers[0].getBillboardData(), BBNumbers[0].getModelNr(), BBNumbers[0].getSubModelNumber(), BBNumbers[0].getVariant());

		test.pos = { 0.0f, 0.0f, 0.0f };
		game->getPreLoader()->setSubModelData(ObjectType::e_number_billboard, test, BBNumbers[0].getModelNr(), BBNumbers[0].getSubModelNumber());
		game->getPreLoader()->draw(BBNumbers[0].getObjectType(), BBNumbers[0].getBillboardData(), BBNumbers[0].getModelNr(), BBNumbers[0].getSubModelNumber(), BBNumbers[0].getVariant());
		//game->getPreLoader()->draw(BBNumbers[0].getObjectType(), BBNumbers[0].getBillboardData(), BBNumbers[0].getModelNr(), BBNumbers[0].getSubModelNumber(), BBNumbers[0].getVariant());
		////test.pos = { 35.0f, 40.0f, -60.0f };
		test.pos = { 5.0f, 0.0f, 0.0f };
		game->getPreLoader()->setSubModelData(ObjectType::e_number_billboard, test, BBNumbers[1].getModelNr(), BBNumbers[1].getSubModelNumber());
		game->getPreLoader()->draw(BBNumbers[1].getObjectType(), BBNumbers[1].getBillboardData(), BBNumbers[1].getModelNr(), BBNumbers[1].getSubModelNumber(), BBNumbers[1].getVariant());
	}
}
