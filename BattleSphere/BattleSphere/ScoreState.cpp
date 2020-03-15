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

bool ScoreState::updatePlatforms(Game* game, float dt)
{

	bool exitGame = false;
	if (!m_hasChosen)
	{
		DirectX::XMVECTOR cyan = { 0.0f, 0.4f, 0.3f, 1.0f };
		DirectX::XMVECTOR red = { 1.0f, 0.0f, 0.0f, 1.0f };
		DirectX::XMVECTOR black = { 0.0f, 0.0f, 0.0f, 0.0f };
		float intensity = 1.0f / float(game->getNrOfPlayers());
		std::vector<Billboard> BB = m_billboardHandler.getBillboardsOfType(objectType::e_static_billboard_score_platform);
		std::vector<Billboard> BBBars = m_billboardHandler.getBillboardsOfType(objectType::e_platformbar_billboard);
		// Test each player against collision mesh
		bool hasCollided = false;

		objectData data;
		for (int j = 0; j < BBBars.size(); ++j)
		{
			int index = j;
			data.material.emission = cyan * 0.25f;
			game->getPreLoader()->setSubModelData(BBBars[index].getObjectType(), data, BBBars[index].getModelNr(), BBBars[index].getSubModelNumber(), BBBars[index].getVariant());
		}

		for (int i = 0; i < BB.size(); ++i)
		{
			hasCollided = false;
			int nrOfCollisions = 0;
			// Get collision mesh
			std::vector<DirectX::XMFLOAT3> colMesh = game->getPreLoader()->getCollisionMesh(BB[i].getObjectType(), BB[i].getModelNr(), BB[i].getVariant());
			std::vector<int> playerCol;
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
						hasCollided = true;
						playerCol.push_back(j);
						break; // Collision found so we jump out
					}
				}
			}

			if (hasCollided)
			{
				int index = 0;
				if (i == 0 || i == 1)
					index = 0;
				else if (i == 2 || i == 3)
					index = 8;
				else
					index = 4;
				
				for (int j = 0; j < playerCol.size(); ++j)
				{
					if (playerCol[j] != -1 && m_readyPlayers[playerCol[j]] != -1)
						data.material.emission = m_robots[playerCol[j]]->getColour();
					else
						data.material.emission = cyan;
					game->getPreLoader()->setSubModelData(BBBars[index + j].getObjectType(), data, BBBars[index + j].getModelNr(), BBBars[index + j].getSubModelNumber(), BBBars[index + j].getVariant());
				}
			}
		}


	

		for (int i = 0; i < XUSER_MAX_COUNT && m_robots[i] != nullptr; ++i)
		{
			for (int j = 0; j < BB.size(); ++j)
			{
				// Get collision mesh
				std::vector<DirectX::XMFLOAT3> colMesh = game->getPreLoader()->getCollisionMesh(BB[j].getObjectType(), BB[j].getModelNr(), BB[j].getVariant());
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
					if (testSphereTriangle(m_robots[i]->getPosition(), game->getPreLoader()->getBoundingData(objectType::e_robot, 0, 0).halfWD.x, v0, v1, v2).m_colliding)
					{

						hasCollided = true;
						if (m_input->isPressed(i, XINPUT_GAMEPAD_A)) // press A to get ready
							m_readyPlayers[i] = j; // This player is ready

						break; // Collision found so we jump out
					}
				}

				// If one or more collisions have been detected with platform it will be lit up
				int nrOfReadyPlayers = 0;
				if (hasCollided)
				{
					// Check amount of ready players
					if (m_readyPlayers.size() == 1)
					{
						for (int k = 0; k < m_readyPlayers.size(); ++k)
						{
							if (m_readyPlayers[k] != -1) nrOfReadyPlayers++;
						}
					}
					else
					{
						for (int k = 1; k < m_readyPlayers.size(); ++k)
						{
							if (m_readyPlayers[0] == m_readyPlayers[k] && m_readyPlayers[0] != -1)	
								nrOfReadyPlayers++;
						}
						nrOfReadyPlayers++;
					}

					// If all players are ready

					if (nrOfReadyPlayers == game->getNrOfPlayers())
					{
						switch (m_readyPlayers[0])
						{
						case 1:
							m_ranking.clear();
							m_playerIDs.clear();
							m_hasChosen = true;
							setPaused(true); // Pause this state
							game->changeState(stateType::e_mainMenu); // Change state to mainMenu
							return false;
							break;
						case 3:
							m_hasChosen = true;
							exitGame = true;
							return true;
							break;
						case 5:
							m_hasChosen = true;
							m_ranking.clear();
							m_playerIDs.clear();
							setPaused(true); // Pause this state
							game->changeState(stateType::e_gameState); // Change state to gameState
							return false;
							break;
						}
					}
				}
			}
			if (!hasCollided) // No collision found
				m_readyPlayers[i] = -1; // This player did not collide with any of the platforms and can therefore not be ready

			hasCollided = false; // Reset bool for the next player
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
			float d = robotBD.halfWD.x;

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
	std::vector<objectType> billboardObjectTypes = { objectType::e_billboard, objectType::e_ranking_billboard, objectType::e_platformbar_billboard, objectType::e_number_billboard, objectType::e_static_billboard_score_platform };
	m_billboardHandler = BillboardHandler(game->getPreLoader(), billboardObjectTypes);

	// Scoreboard
	m_scoreTimer = 0.0f;
	m_scoreTimerAcceleration = 1.1f;
	m_hasChosen = false;

	m_transparency.initialize();
	m_transparency.bindConstantBuffer();


	//// Initialize dynamic camera
	//m_zoomingOutToStart = false;
	//m_vecToCam = XMVector3Normalize(DX::getInstance()->getCam()->getPosition() - DX::getInstance()->getCam()->getLookAt());
	//m_camStartPos = DX::getInstance()->getCam()->getPosition();
	//m_camStartLookAt = DX::getInstance()->getCam()->getLookAt();
	//float xFovHalf = DX::getInstance()->getCam()->getXFOV() / 2.0f;
	//float yFovHalf = DX::getInstance()->getCam()->getYFOV() / 2.0f;
	//m_fOVPlanes[0] = XMVector3Rotate(XMVectorSet(0.0f, 1.0f, 0.0, 0.0f), XMQuaternionRotationNormal(XMVectorSet(1.0f, 0.0f, 0.0, 0.0f), -yFovHalf)); // Bottom
	//m_fOVPlanes[1] = XMVector3Rotate(XMVectorSet(1.0f, 0.0f, 0.0, 0.0f), XMQuaternionRotationNormal(XMVectorSet(0.0f, 1.0f, 0.0, 0.0f), xFovHalf)); // Left
	//m_fOVPlanes[2] = XMVector3Rotate(XMVectorSet(0.0f, -1.0f, 0.0, 0.0f), XMQuaternionRotationNormal(XMVectorSet(1.0f, 0.0f, 0.0, 0.0f), yFovHalf)); // Top
	//m_fOVPlanes[3] = XMVector3Rotate(XMVectorSet(-1.0f, 0.0f, 0.0, 0.0f), XMQuaternionRotationNormal(XMVectorSet(0.0f, 1.0f, 0.0, 0.0f), -xFovHalf)); // Right

	//// Rotate plane according to look at
	//float camAngle = XMScalarACos(XMVector3Dot(XMVectorSet(0.0, 0.0, 1.0f, 0.0f), -m_vecToCam).m128_f32[0]);
	//for (int i = 0; i < 4; i++)
	//	m_fOVPlanes[i] = XMVector3Rotate(m_fOVPlanes[i], XMQuaternionRotationNormal(XMVectorSet(1.0f, 0.0f, 0.0, 0.0f), -camAngle)); // Bottom

	//m_fOVPlanes[0].m128_f32[2] *= -1;
	//m_fOVPlanes[1].m128_f32[2] *= -1;
	//m_fOVPlanes[2].m128_f32[2] *= -1;
	//m_fOVPlanes[3].m128_f32[2] *= -1;

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
	m_hasChosen = false;

	// Set camera position and lookAt
	DirectX::XMVECTOR lookAt{ 35.0f, 40.0f, -60.0f };
	lookAt = { 45.0f, 120.0f, -260.0f };
	DirectX::XMVECTOR camPos{ 35.0f, 25.0f, -130.0f };
	camPos = { 45.0f, 130.0f, -330.0f };
	DX::getInstance()->getCam()->setCameraPosition(camPos);
	DX::getInstance()->getCam()->setLookAt(lookAt);

	// Initialize robots
	m_input = game->getInput();
	m_robots = game->getRobots();

	game->updatePlayerStatus();
	// Get player scores, IDs and initalize their readiness
	for (int i = 0; i < XUSER_MAX_COUNT; ++i)
	{
		if (m_robots[i] != nullptr)
		{
			m_ranking.push_back(m_robots[i]->getScore());
			m_playerIDs.push_back(m_robots[i]->getPlayerId());
			m_readyPlayers.push_back(false);
			m_collidedPlatforms.push_back(-1);
		}
	}

	DirectX::XMVECTOR spawnPos = { 44.569f, 102.0f, -300.0f };

	for (int i = 0; i < XUSER_MAX_COUNT; ++i)
	{
		if (m_robots[i] != nullptr)
		{
			m_robots[i]->storePositionInHistory(spawnPos);
			m_robots[i]->setPosition(spawnPos);
			m_robots[i]->setDrawn(true);
			m_robots[i]->setHealth(100);
		}
	}

	// Calculate ranking based of player scores
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
	exitGame = updatePlatforms(game, dt);

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
		std::vector<Billboard> BBRanking = m_billboardHandler.getBillboardsOfType(objectType::e_ranking_billboard);

		for (int i = 0; i < BB.size(); ++i)
		{
			if (BB[i].getObjectType() != objectType::e_number_billboard && BB[i].getObjectType() != objectType::e_ranking_billboard)
				game->getPreLoader()->draw(BB[i].getObjectType(), BB[i].getBillboardData(), BB[i].getModelNr(), BB[i].getSubModelNumber(), BB[i].getVariant());
		}

		int score = 0, digit = 0, displayedScore = 0;
		float distX = 2.0f, distY = 4.0f;

		std::vector<int> digits;
		objectData data, dataRanking;
		dataRanking.pos.m128_f32[0] = -10.0f;
		dataRanking.pos.m128_f32[1] = (float)m_ranking.size();
		data.pos.m128_f32[1] = (float)m_ranking.size();
		for (int i = 0; i < m_playerIDs.size(); ++i)
		{
			int playerID = m_playerIDs[i];
			score = m_robots[playerID]->getScore();

			// Draw ranking
			dataRanking.material.emission = m_robots[playerID]->getData().material.emission * 0.5f;
			game->getPreLoader()->setSubModelData(objectType::e_ranking_billboard, dataRanking, BBRanking[i].getModelNr(), BBRanking[i].getSubModelNumber());
			game->getPreLoader()->draw(BBRanking[i].getObjectType(), BBRanking[i].getBillboardData(), BBRanking[i].getModelNr(), BBRanking[i].getSubModelNumber(), BBRanking[i].getVariant());
			dataRanking.pos.m128_f32[1] -= distY;

			do
			{
				if (score == 10)
					int test = 0;
				digits.push_back(score % 10);
				score /= 10;
			} while (score > 0);

			data.pos.m128_f32[0] = -(float)digits.size();
			data.material.emission = m_robots[playerID]->getData().material.emission * 0.5f;
			for (int j = int(digits.size()) - 1; j >= 0; --j)
			{
				digit = digits[j];

				game->getPreLoader()->setSubModelData(objectType::e_number_billboard, data, BBNumbers[digit].getModelNr(), BBNumbers[digit].getSubModelNumber());
				game->getPreLoader()->draw(BBNumbers[digit].getObjectType(), BBNumbers[digit].getBillboardData(), BBNumbers[digit].getModelNr(), BBNumbers[digit].getSubModelNumber(), BBNumbers[digit].getVariant());
				
				data.pos.m128_f32[0] += 2.0f;
			}
			data.pos.m128_f32[1] -= distY;

			digits.clear();
		}
	}
}
