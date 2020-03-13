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
}

bool ScoreState::updateScoreScorePlatforms(Game* game)
{
	bool exitGame = false;
	DirectX::XMVECTOR cyan = { 0.0f, 0.4f, 0.3f, 1.0f };
	DirectX::XMVECTOR red = { 1.0f, 0.0f, 0.0f, 1.0f };
	DirectX::XMVECTOR black = { 0.0f, 0.0f, 0.0f, 0.0f };
	float intensity = 1 / (game->getNrOfPlayers() * 2.0f);
	//m_billboardHandler.setAllStates(3, 0.1f, 1.0f, cyan, cyan, cyan);
	std::vector<Billboard> BB = m_billboardHandler.getBillboards();
	for (int i = 0; i < BB.size(); ++i)
	{
		if (BB[i].getObjectType() == objectType::e_static_billboard_score_platform)
		{
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
					if (testSphereTriangle(m_robots[j]->getPosition(), game->getPreLoader()->getBoundingData(objectType::e_robot, 0, 0).halfWD.x, v0, v1, v2).m_colliding)
					{
						// If collision detected tick up nr of collisions
						nrOfCollisions++;
						break;
					}
				}
			}

			if (nrOfCollisions == game->getNrOfPlayers())
			{
				if(i == 10)
					m_billboardHandler.setAllStates(12, 0.1f, 0.1f, cyan, cyan * nrOfCollisions * intensity, black);
				if (i == 12)
					m_billboardHandler.setAllStates(10, 0.1f, 1.0f, cyan, cyan, black);
				if (i == 14)
					m_billboardHandler.setAllStates(14, 0.1f, 1.0f, cyan, cyan, black);

				if (m_input->isPressed(i, XINPUT_GAMEPAD_A))
				{
					switch (i)
					{
					case 10:
						exitGame = true;
						break;
					case 12:
						m_ranking.clear();
						m_playerIDs.clear();
						//setPaused(true); // Pause this state
						game->changeState(stateType::e_mainMenu); // Change state to mainMenu
						break;
					case 14:
						m_ranking.clear();
						m_playerIDs.clear();
						//setPaused(true); // Pause this state
						game->changeState(stateType::e_gameState); // Change state to gameState
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
	std::vector<objectType> billboardObjectTypes = { objectType::e_billboard, objectType::e_number_billboard, objectType::e_static_billboard_score_platform };
	m_billboardHandler = BillboardHandler(game->getPreLoader(), billboardObjectTypes);

	// Scoreboard
	m_scoreTimer = 0.0f;
	m_scoreTimerAcceleration = 1.1f;

	m_transparency.initialize();
	m_transparency.bindConstantBuffer();
	/*
	m_lights = Lights::getInstance();
	int index = m_lights->addPointLight(-10, 25, 0, 55, 1, 0.5f, 0.125f, 1);
	m_lights->setColor(index, float(255) / 255, float(0) / 255, float(97) / 255);
	index = m_lights->addSpotLight(-2.5f, 11.67f, -67, 17, -0.33f, -1, 0.0f, 1.0f, 1.0f, 0.0f, 27, 20);
	index = m_lights->addSpotLight(2.5f, 11.67f, -67, 17, 0.33f, -1, 0.0f, 1.0f, 1.0f, 0.0f, 27, 20);
	//index = m_lights->addVolumetricSpotLight(133.0f, 38.0f, -29.0f, 70.0f, -0.6f, -0.8f, -0.3f, 0.15f, 0.97f, 1.0f, 20.0f, 13.0f); // Headlights construction
	//m_lights->addAreaLight(-52, 11.67f, -72, 17, 1, 1, 0, 5);
	//m_lights->addAreaLight(46, 8, -60, 17, 1, 0, 1, 5);
	//m_lights->addAreaLight(78, 18, 70, 50, 1, 0.5f, 0, 25);
	//m_lights->addAreaLight(-5, 18, 75, 33, 0, 1, 1, 10);
	//m_lights->addAreaLight(33, 10, 67, 50, 0, 0, 1, 15);
	//m_lights->addAreaLight(178, 10, 67, 50, 1, 1, 0, 20);
	//m_lights->addAreaLight(150, 10, 55, 17, 1, 0, 0, 20);
	//m_lights->addAreaLight(-119, 3, 99, 17, 1, 0.6f, 0, 10);
	//index = m_lights->addVolumetricSpotLight(133.0f, 38.0f, -29.0f, 90.0f, -0.6f, -0.8f, -0.3f, 0.15f, 0.97f, 1.0f, 20.0f, 13.0f); // Headlights construction
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
	*/
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

void ScoreState::firstTimeSetUp(Game* game)
{
	// Set camera position and lookAt
	DirectX::XMVECTOR lookAt{ 35.0f, 40.0f, -60.0f };
	lookAt = { 45.0f, 120.0f, -260.0f };
	DirectX::XMVECTOR camPos{ 35.0f, 25.0f, -130.0f };
	camPos = { 45.0f, 140.0f, -330.0f };
	DX::getInstance()->getCam()->setCameraPosition(camPos);
	DX::getInstance()->getCam()->setLookAt(lookAt);

	// Initialize robots
	m_input = game->getInput();
	m_robots = game->getRobots();
	m_robots[1] = new Robot(1);
	m_robots[2] = new Robot(2);
	m_robots[3] = new Robot(3);
	m_robots[0]->storePositionInHistory({ 45.0f, 102.0f, -300.0f });
	m_robots[0]->setPosition({ 45.0f, 102.0f, -300.0f });
	m_robots[1]->setColour(0.5f, 0.5f, 0.5f);
	m_robots[2]->setColour(0.8f, 0.0f, 0.8f);
	m_robots[3]->setColour(0.0f, 0.2f, 0.3f);
	m_robots[1]->setScore(10);
	m_robots[2]->setScore(17);
	m_robots[3]->setScore(27);

	// Calculate ranking based of player scores
	for (int i = 0; i < XUSER_MAX_COUNT; ++i)
	{
		if (m_robots[i] != nullptr)
		{
			m_ranking.push_back(m_robots[i]->getScore());
			m_playerIDs.push_back(m_robots[i]->getPlayerId());
		}
	}

	for (int i = 0; i < int(m_ranking.size()) - 1; ++i)
	{
		for (int j = 0; j < int(m_ranking.size()) - 1; ++j)
		{
			int index = j + 1;
			if (m_ranking[j] < m_ranking[index])
			{
				// Order ranking
				int tempRank = m_ranking[j];
				m_ranking[j] = m_ranking[index];
				m_ranking[index] = tempRank;

				// Order the vector
				int tempPlayerID = m_playerIDs[j];
				m_playerIDs[j] = m_playerIDs[index];
				m_playerIDs[index] = tempPlayerID;
			}
		}
	}
}

void ScoreState::handleInput(Game* game)
{
}

bool ScoreState::update(Game* game, float dt)
{
	bool exitGame = false;

	handleInputs(game, dt);
	game->updatePlayerStatus();

	// Update sounds
	Sound::getInstance()->update(dt);

	// Update dynamic camera
	updateDynamicCamera(dt);

	// Update spawning drone
	m_spawnDrone->update(m_robots, dt);

	// Update billboards
	m_billboardHandler.updateBillboards(dt);

	//Dynamic background objects
	m_dboHandler->update(dt);

	// Update platforms
	exitGame = updateScoreScorePlatforms(game);
	
	m_scoreTimer += dt;

	return exitGame;
}

void ScoreState::draw(Game* game, renderPass pass)
{
	if (pass == renderPass::e_opaque)
	{
		for (int i = 0; i < XUSER_MAX_COUNT; i++)
		{
			if (m_robots[i] != nullptr && m_robots[i]->isDrawn())
			{
				game->getPreLoader()->setSubModelData(objectType::e_robot, game->getRobots()[i]->getData(), 0, 1);
				game->getPreLoader()->draw(objectType::e_robot);
			}
		}

		game->getPreLoader()->draw(objectType::e_ground);
	}
	else if (pass == renderPass::e_transparent)
	{
		//Static
		for (int i = 0; i < game->getPreLoader()->getNrOfVariants(objectType::e_static); i++)
			game->getPreLoader()->draw(objectType::e_static, i);

		game->getPreLoader()->drawOneModel(objectType::e_drone, m_spawnDrone->getData(), 0);
		game->getPreLoader()->drawOneModel(objectType::e_drone, m_spawnDrone->getData(0), m_spawnDrone->getData(), 1);
		game->getPreLoader()->drawOneModel(objectType::e_drone, m_spawnDrone->getData(1), m_spawnDrone->getData(), 1);
		game->getPreLoader()->drawOneModel(objectType::e_drone, m_spawnDrone->getData(2), m_spawnDrone->getData(), 1);
		game->getPreLoader()->drawOneModel(objectType::e_drone, m_spawnDrone->getData(3), m_spawnDrone->getData(), 1);
	
		// Tokyo drift
		for (int i = 0; i < OBJECT_NR_1; i++)
		{
			if (m_dboHandler->isDrawn(i))
				game->getPreLoader()->draw(objectType::e_extra, m_dboHandler->getData(i));
		}
		// Scene (Background objects without collision)
		for (int i = 0; i < game->getPreLoader()->getNrOfVariants(objectType::e_scene); i++)
			game->getPreLoader()->draw(objectType::e_scene, i);

		for (int i = 0; i < game->getPreLoader()->getNrOfVariants(objectType::e_score_scene); i++)
			game->getPreLoader()->draw(objectType::e_score_scene, i);
	}
	else if (pass == renderPass::e_billboard)
	{
		std::vector<Billboard> BB = m_billboardHandler.getBillboards();
		std::vector<Billboard> BBNumbers = m_billboardHandler.getBillboardsOfType(objectType::e_number_billboard);

		for (int i = 0; i < BB.size(); ++i)
		{
			if(BB[i].getObjectType() != objectType::e_number_billboard)
				game->getPreLoader()->draw(BB[i].getObjectType(), BB[i].getBillboardData(), BB[i].getModelNr(), BB[i].getSubModelNumber(), BB[i].getVariant());
		}

		int score = 0, digit = 0, displayedScore = 0;
		float distX = 2.0f, distY = 4.0f;

		std::vector<int> digits;
		objectData data;
		data.pos.m128_f32[1] = game->getNrOfPlayers() * 3.0f;
		for (int i = 0; i < m_playerIDs.size(); ++i)
		{
			int playerID = m_playerIDs[i];
			score = m_robots[playerID]->getScore();
			do 
			{
				if (score == 10)
					int test = 0;
				digits.push_back(score % 10);
				score /= 10;
			} while (score > 0);

			data.pos.m128_f32[0] = -(float)digits.size() * 0.5f * 2.0f;
			data.material.emission = m_robots[playerID]->getData().material.emission;
			for (int j = int(digits.size()) - 1; j >= 0; --j)
			{
				digit = digits[j];
				
				game->getPreLoader()->setSubModelData(objectType::e_number_billboard, data, BBNumbers[digit].getModelNr(), BBNumbers[digit].getSubModelNumber());
				game->getPreLoader()->draw(BBNumbers[digit].getObjectType(), BBNumbers[digit].getBillboardData(), BBNumbers[digit].getModelNr(), BBNumbers[digit].getSubModelNumber(), BBNumbers[digit].getVariant());

				data.pos.m128_f32[0] += 2.0f;
			}
		
			data.pos.m128_f32[1] -= 4.0f;
			digits.clear();	
		}
	}
}
