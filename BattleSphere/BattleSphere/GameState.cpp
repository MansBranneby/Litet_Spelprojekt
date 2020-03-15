#include "GameState.h"

void GameState::spawnNodes()
{
	Node* node = new Node(0);
	//Node* node = new Node(rand() % 3);
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

void GameState::updateDynamicCamera(float dT)
{
	// Calculate number of players
	int nrOfPlayers = 0;
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		if (m_robots[i] != nullptr && m_robots[i]->isDrawn())
			nrOfPlayers++;
	}

	if (nrOfPlayers >= 1)
	{
		// Get new look at and get min and max of x and z
		XMVECTOR oldCamLookAt = DX::getInstance()->getCam()->getLookAt();
		XMVECTOR oldCamPos = DX::getInstance()->getCam()->getPosition();
		XMVECTOR newLookAt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR robPos;
		float minX = 2000;
		float maxX = -2000;
		float minZ = 2000;
		float maxZ = -2000;
		for (int i = 0; i < XUSER_MAX_COUNT; i++)
		{
			if (m_robots[i] != nullptr && m_robots[i]->isDrawn())
			{
				robPos = m_robots[i]->getPosition();
				newLookAt += robPos;
				robPos = XMVector3Normalize(m_robots[i]->getPosition() - oldCamPos);

				if (minX > robPos.m128_f32[0])
					minX = robPos.m128_f32[0];

				if (maxX < robPos.m128_f32[0])
					maxX = robPos.m128_f32[0];

				if (minZ > robPos.m128_f32[2])
					minZ = robPos.m128_f32[2];

				if (maxZ < robPos.m128_f32[2])
					maxZ = robPos.m128_f32[2];
			}
		}

		// Set look at between players and move it upp the z-axis slightly
		newLookAt /= (float)nrOfPlayers;
		newLookAt.m128_f32[3] -= 100.0f;

		// Calculate biggest distance
		float xDifference = maxX - minX;
		float zDifference = maxZ - minZ;
		float biggestDifference;
		if (xDifference > zDifference)
			biggestDifference = xDifference;
		else
			biggestDifference = zDifference;

		// Set new camera position and look at

		XMVECTOR newPos;
		if (m_zoomingOutToStart) // If zooming out, have harder criteria to zoom in again
		{
			if (MINIMUM_CAM_DISTANCE + biggestDifference * 250.0f < MAXIMUM_CAM_DISTANCE)
			{
				newPos = newLookAt + m_vecToCam * (MINIMUM_CAM_DISTANCE + biggestDifference * 50.0f);
				if (newPos.m128_f32[2] < -105.0f) // Limit camera movement in z-axis
				{
					float difference = -105.0f - newPos.m128_f32[2];
					newPos.m128_f32[2] += difference;
					newLookAt.m128_f32[2] += difference;
				}
				m_zoomingOutToStart = false;
			}
		}
		else // If not zooming out
		{
			if (MINIMUM_CAM_DISTANCE + biggestDifference * 120.0f < MAXIMUM_CAM_DISTANCE)
			{
				newPos = newLookAt + m_vecToCam * (MINIMUM_CAM_DISTANCE + biggestDifference * 50.0f);
				if (newPos.m128_f32[2] < -105.0f) // Limit camera movement in z-axis
				{
					float difference = -105.0f - newPos.m128_f32[2];
					newPos.m128_f32[2] += difference;
					newLookAt.m128_f32[2] += difference;
				}
			}
			else
			{
				m_zoomingOutToStart = true;
				newPos = m_camStartPos;
				newLookAt = m_camStartLookAt;
			}
		}




		// Project each robot onto FOV planes and find smallest distances
		float closest = INFINITY;
		for (int i = 0; i < XUSER_MAX_COUNT; i++)
		{
			if (m_robots[i] != nullptr && m_robots[i]->isDrawn())
			{
				robPos = m_robots[i]->getPosition();
				XMVECTOR camToBot = XMVector3Normalize(robPos - oldCamPos);
				for (int plane = 0; plane < 4; plane++) // Bottom, left, top, right
				{
					// Project
					float distance = XMVector3Dot(camToBot, m_fOVPlanes[plane]).m128_f32[0];

					if (distance < closest)
						closest = distance;
				}
			}
		}

		float changeSpeed = dT * CHANGE_SPEED;
		if (closest < 0.01f) // Limit closest to avoid zero and negative speeds
			closest = 0.01f;

		if (m_zoomingOutToStart || m_devZoomOut) // If zooming out to start pos is true, set the position and look at, also increase speeds
		{
			changeSpeed *= 2.0f;
			newPos = m_camStartPos;
			newLookAt = m_camStartLookAt;
		}
		else if (closest < 0.6f) // Change "changeSpeed" according to how close a player is to a camera plane
		{
			changeSpeed *= 0.8f / closest;
			newPos += m_vecToCam * (biggestDifference * 90.0f);
		}

		XMVECTOR vecToNewCamPos = newPos - oldCamPos;
		XMVECTOR vecToNewCamLookAt = newLookAt - oldCamLookAt;

		DX::getInstance()->getCam()->movePosAndLook(vecToNewCamPos * changeSpeed, vecToNewCamLookAt * changeSpeed);
	}
}

void GameState::bspdLightUpdate(float dt)
{
	m_BSPDtimer += dt * 0.5f;
	float angle = sin(m_BSPDtimer);
	Lights::getInstance()->setDirection(m_BSPDLightIndex[0], angle * 0.5f, -1.0f, 0.0f);
	Lights::getInstance()->setDirection(m_BSPDLightIndex[1], -angle * 0.3f, -1.0f, 0.0f);
}

void GameState::handleMovement(Game* game, float dt, int id)
{
	// Limit robot look at vector
	XMVECTOR robLookAt = XMVectorSet(m_input->getThumbLX(id), 0.0f, m_input->getThumbLY(id), 0.0f);
	if (XMVector3Length(robLookAt).m128_f32[0] > 1.0f)
		robLookAt = XMVector3Normalize(robLookAt);

	// Save velocity for collision
	if (!m_robots[id]->isAi())
	{
		m_robots[id]->setVel(robLookAt *
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

	// Resource movement
	if (m_robots[id]->getResourceIndex() != -1)
		m_resources[m_robots[id]->getResourceIndex()]->setPosition(m_robots[id]->getPosition() + XMVectorSet(0.0f, 1.5f, 0.0f, 0.0f));
}

bool GameState::handleInputs(Game* game, float dt)
{
	// Unblock player inputs
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		if (!game->getInput()->isPressed(i, XINPUT_GAMEPAD_A) && !game->getInput()->isPressed(i, XINPUT_GAMEPAD_START) && game->getInput()->getThumbLX(i) < 0.2f && game->getInput()->getThumbLX(i) > -0.2f)
			game->getInput()->setBlocked(i, false);
	}

	if (!m_quitGame)
	{
		for (int i = 0; i < XUSER_MAX_COUNT; i++)
		{
			if (game->getRobots()[i] != nullptr)
			{

				if (!m_input->refresh(i, dt) && !m_robots[i]->isAi())
				{
					m_input->reconnectController(i);
				}
				else if (game->getRobots()[i]->isDrawn())
				{
					// Use weapon
					if (m_input->getTriggerR(i) > 0.2 && !m_robots[i]->isAi())
					{
						// dumb stuff for sniper
						XMVECTOR start = XMVectorSet(0, 0, 0, 0);
						XMVECTOR end = XMVectorSet(0, 0, 0, 0);
						if (m_robots[i]->getCurrentWeapon(RIGHT) != -1 && m_robots[i]->getWeapons()[m_robots[i]->getCurrentWeapon(RIGHT)]->getType() == SNIPER &&
							m_robots[i]->getWeapons()[m_robots[i]->getCurrentWeapon(RIGHT)]->getReady())
						{
							m_robots[i]->getSniperLine(RIGHT, start, end);
							m_lineShots.updateLineStatus(i, start, end, true, dt);

							// Add particle effect
							XMVECTOR col = m_robots[i]->getColour();
							XMVECTOR dir = XMVector3Normalize(end - start);
							DX::getInstance()->getParticles()->addSniperSmoke(start, col, end - start);
							boundingData robotBD = game->getPreLoader()->getBoundingData(objectType::e_robot, 1, 0);
							for (int j = 0; j < 4; j++)
							{
								if (i != j && m_robots[j] != nullptr && m_robots[j]->isDrawn())
								{
									XMVECTOR left = XMVector3Cross(dir, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)) * (robotBD.halfWD.x - 0.51f);
									if (testLineSphere(start + left, end + left, m_robots[j]->getPosition(), robotBD.halfWD.x) || testLineSphere(start - left, end - left, m_robots[j]->getPosition(), robotBD.halfWD.x))
									{
										int resourceIndex = m_robots[j]->getResourceIndex();
										if (m_robots[j]->damagePlayer(m_robots[i]->getWeapons()[m_robots[i]->getCurrentWeapon(RIGHT)]->getDamage(), end - start, -1))
										{
											if (!m_robots[j]->isAi())
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

						m_robots[i]->useWeapon(RIGHT, dt);
					}
					if (m_input->getTriggerL(i) > 0.2 && !m_robots[i]->isAi())
					{
						// dumb stuff for sniper
						XMVECTOR start = XMVectorSet(0, 0, 0, 0);
						XMVECTOR end = XMVectorSet(0, 0, 0, 0);
						if (m_robots[i]->getCurrentWeapon(LEFT) != -1 && m_robots[i]->getWeapons()[m_robots[i]->getCurrentWeapon(LEFT)]->getType() == SNIPER &&
							m_robots[i]->getWeapons()[m_robots[i]->getCurrentWeapon(LEFT)]->getReady())
						{

							m_robots[i]->getSniperLine(LEFT, start, end);
							m_lineShots.updateLineStatus(i, start, end, true, dt);

							// Add particle effect
							XMVECTOR col = m_robots[i]->getColour();
							XMVECTOR dir = XMVector3Normalize(end - start);
							DX::getInstance()->getParticles()->addSniperSmoke(start, col, end - start);
							boundingData robotBD = game->getPreLoader()->getBoundingData(objectType::e_robot, 1, 0);
							for (int j = 0; j < 4; j++)
							{
								if (i != j && m_robots[j] != nullptr && m_robots[j]->isDrawn())
								{
									XMVECTOR left = XMVector3Cross(dir, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)) * (robotBD.halfWD.x - 0.51f);
									if (testLineSphere(start + left, end + left, m_robots[j]->getPosition(), robotBD.halfWD.x) || testLineSphere(start - left, end - left, m_robots[j]->getPosition(), robotBD.halfWD.x))
									{
										int resourceIndex = m_robots[j]->getResourceIndex();
										if (m_robots[j]->damagePlayer(m_robots[i]->getWeapons()[m_robots[i]->getCurrentWeapon(LEFT)]->getDamage(), end - start, -1))
										{
											if (!m_robots[j]->isAi())
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
						if (XMVectorGetX(XMVector3Length(rob - resource)) < 2.0f &&
							!m_resources[j]->isBlocked())
						{
							Sound::getInstance()->play(soundEffect::e_pickup, rob, 0.4f);
							m_spawnDrone->freeSpawn(m_resources[j]->getSpawnIndex());
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
					if (m_input->isPressed(i, XINPUT_GAMEPAD_A) || m_robots[i]->isAi())
					{
						for (int j = 0; j < m_nodes.size() && m_robots[i]->getResourceIndex() != -1; j++)
						{
							std::vector<XMFLOAT3> cm = game->getPreLoader()->getCollisionMesh(objectType::e_node, j);
							bool collision = false;
							for (int k = 0; k < cm.size(); k += 3)
							{
								unsigned int ind1 = k + 1;
								unsigned int ind2 = k + 2;
								if (testSphereTriangle(m_robots[i]->getPosition(), game->getPreLoader()->getBoundingData(objectType::e_robot, 0, 0).halfWD.x,
									XMVECTOR{ cm[k].x, cm[k].y, cm[k].z },
									XMVECTOR{ cm[ind1].x, cm[ind1].y, cm[ind1].z },
									XMVECTOR{ cm[ind2].x, cm[ind2].y, cm[ind2].z }).m_colliding)
								{
									collision = true;
									break;
								}
							}
							if (collision && m_nodes[j]->isType(m_resources[m_robots[i]->getResourceIndex()]->getType()))
							{
								if (!m_robots[i]->upgradeWeapon(m_resources[m_robots[i]->getResourceIndex()]->getType()))
								{
									// Update user interface with new ability
									m_userInterface->setSlotID(m_robots[i]->getRobotID(), m_resources[m_robots[i]->getResourceIndex()]->getType());
								}
								Sound::getInstance()->play(soundEffect::e_turnin, m_robots[i]->getPosition(), 0.4f);

								for (int k = 0; k < XUSER_MAX_COUNT; k++)
								{
									if (m_robots[k] != nullptr && k != i)
									{
										if (m_robots[k]->getResourceIndex() > m_robots[i]->getResourceIndex())
											m_robots[k]->setResourceIndex(m_robots[k]->getResourceIndex() - 1);
									}
								}

								m_spawnDrone->ifHeldDecreaseResourceIndex();

								delete m_resources[m_robots[i]->getResourceIndex()];
								m_resources.erase(m_resources.begin() + m_robots[i]->getResourceIndex());
								m_robots[i]->removeResource();

								break;
							}
						}
					}

					// Camera dev zoom
					if (m_input->isPressed(i, XINPUT_GAMEPAD_BACK) && !m_devZoomOut)
					{
						m_devZoomOut = true;
					}
					else if (m_input->isPressed(i, XINPUT_GAMEPAD_BACK) && m_devZoomOut)
					{
						m_devZoomOut = false;
					}

					// Change weapons
					if (m_robots[i]->isReady(dt))
					{
						if (m_input->isPressed(i, XINPUT_GAMEPAD_RIGHT_SHOULDER))
						{
							int type = m_robots[i]->changeWeapon(RIGHT);
							if (type != -1)
							{
								//m_userInterface->setSlotID(i, type, RIGHT, m_robots[i]->getNextWeapon());
								m_userInterface->setSlotID(m_robots[i]->getRobotID(), type, RIGHT, m_robots[i]->getNextWeapon(), m_robots[i]->getNextNextWeapon());
							}
						}

						if (m_input->isPressed(i, XINPUT_GAMEPAD_LEFT_SHOULDER))
						{
							int type = m_robots[i]->changeWeapon(LEFT);
							if (type != -1)
							{
								//m_userInterface->setSlotID(i, type, LEFT, m_robots[i]->getNextWeapon());
								m_userInterface->setSlotID(m_robots[i]->getRobotID(), type, LEFT, m_robots[i]->getNextWeapon(), m_robots[i]->getNextNextWeapon());
							}
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

				// Quit Game
				if (m_input->isPressed(i, XINPUT_GAMEPAD_START) && !m_input->isBlocked(i))
				{
					m_quitGame = true;
					break;
				}

				// TODO: remove cheats yooo
				if (m_input->isPressed(i, XINPUT_GAMEPAD_DPAD_UP))
				{
					m_robots[i]->setHealth(100);
					m_input->setVibration(i, 0.0f);
				}

				if (m_robots[i]->getResourceIndex() != -1)
				{
					bool goal = false;
					std::vector<XMVECTOR> paths[4];
					for (int j = 0; j < (int)m_nodes.size(); j++)
					{
						if (m_nodes[j]->isType(m_resources[m_robots[i]->getResourceIndex()]->getType()))
						{
							paths[j] = Graph::getInstance()->calculateShortestPath(i, m_robots[i]->getPosition(), j);
							goal = true;
						}
					}
					if (goal)
						Graph::getInstance()->setShortestPath(i, paths);
				}

				// COLLISION PLAYER VS STATIC OBJECTS
				CollisionInfo collisionInfo;
				boundingData robotBD = game->getPreLoader()->getBoundingData(objectType::e_robot, 1, 0);
				robotBD.pos = m_robots[i]->getPosition();
				XMVECTOR v = m_robots[i]->getPosition() - m_robots[i]->getPreviousPosition();
				XMVECTOR newPos = m_robots[i]->getPosition();
				float l = XMVectorGetX(XMVector3Length(v));
				float d = robotBD.halfWD.x * 0.5f;

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
	else
	{
		for (int i = 0; i < XUSER_MAX_COUNT; i++)
		{
			if (!m_input->refresh(i, dt))
			{
				m_input->reconnectController(i);
			}
			else
			{
				m_input->setVibration(i, 0.0f);

				if (m_input->isPressed(i, XINPUT_GAMEPAD_A) && m_userInterface->getQuitGame())
				{
					//setPaused(true);
					game->changeState(stateType::e_mainMenu);
					return true;
				}
				if (m_input->isPressed(i, XINPUT_GAMEPAD_A) && !m_userInterface->getQuitGame())
				{
					m_quitGame = false;
					m_userInterface->setQuitGame(true);
				}
				if (game->getInput()->getThumbLX(i) > 0.2f && !game->getInput()->isBlocked(i))
				{
					m_userInterface->quitGameHI(RIGHT);
					game->getInput()->setBlocked(i, true);
				}
				if (game->getInput()->getThumbLX(i) < -0.2f && !game->getInput()->isBlocked(i))
				{
					m_userInterface->quitGameHI(LEFT);
					game->getInput()->setBlocked(i, true);
				}
			}

		}
	}
	return false;
}

GameState::GameState(Game* game)
{
	m_devZoomOut = false;
	srand((unsigned int)time(NULL));

	m_type = stateType::e_gameState;
	m_input = nullptr;
	m_robots = nullptr;
	m_BSPDtimer = 0.0f;

	// Spawn preset nodes and initialize spawning drone
	m_spawnDrone = new SpawnDrone(&m_resources);
	spawnNodes();

	// Create billboards
	std::vector<objectType> billboardObjectTypes = { objectType::e_billboard, objectType::e_static_billboard };
	m_billboardHandler = BillboardHandler(game->getPreLoader(), billboardObjectTypes);
	//m_billboardHandler = BillboardHandler();
	m_transparency.initialize();
	m_transparency.bindConstantBuffer();
	m_lights = Lights::getInstance();
	int index = m_lights->addPointLight(-10, 25, 0, 55, 1, 0.5f, 0.125f, 1);
	m_lights->setColor(index, float(255) / 255, float(0) / 255, float(97) / 255);
	index = m_lights->addSpotLight(-2.5f, 11.67f, -67, 17, -0.33f, -1, 0.0f, 1.0f, 1.0f, 0.0f, 27, 20);
	index = m_lights->addSpotLight(2.5f, 11.67f, -67, 17, 0.33f, -1, 0.0f, 1.0f, 1.0f, 0.0f, 27, 20);
	index = m_lights->addVolumetricSpotLight(133.0f, 38.0f, -29.0f, 70.0f, -0.6f, -0.8f, -0.3f, 0.15f, 0.97f, 1.0f, 20.0f, 1.0f); // Headlights construction
	//m_lights->addAreaLight(-52, 11.67f, -72, 17, 1, 1, 0, 5);
	//m_lights->addAreaLight(46, 8, -60, 17, 1, 0, 1, 5);
	//m_lights->addAreaLight(78, 18, 70, 50, 1, 0.5f, 0, 25);
	//m_lights->addAreaLight(-5, 18, 75, 33, 0, 1, 1, 10);
	//m_lights->addAreaLight(33, 10, 67, 50, 0, 0, 1, 15);
	//m_lights->addAreaLight(178, 10, 67, 50, 1, 1, 0, 20);
	//m_lights->addAreaLight(150, 10, 55, 17, 1, 0, 0, 20);
	//m_lights->addAreaLight(-119, 3, 99, 17, 1, 0.6f, 0, 10);
	//index = m_lights->addVolumetricSpotLight(133.0f, 38.0f, -29.0f, 90.0f, -0.6f, -0.8f, -0.3f, 0.15f, 0.97f, 1.0f, 20.0f, 1.0f); // Headlights construction
	//m_lights->addAreaLight(-52, 11.67f, -72, 17, 1, 1, 0, 5);
	//m_lights->addAreaLight(46, 8, -60, 17, 1, 0, 1, 5);
	//m_lights->addAreaLight(-5, 18, 75, 33, 0, 1, 1, 10);
	//m_lights->addAreaLight(33, 10, 67, 50, 0, 0, 1, 15);
	//m_lights->addAreaLight(178, 10, 67, 50, 1, 1, 0, 20);
	//m_lights->addAreaLight(150, 10, 55, 17, 1, 0, 0, 20);
	//m_lights->addAreaLight(-119, 3, 99, 17, 1, 0.6f, 0, 10);

	index = m_lights->addVolumetricSpotLight(-255.0f, 18.0f, 168.0f, 170.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.5f, 1.0f, 20.0f, 4.0f); // tunnle left
	m_lights->addAreaLight(-229.0f, 18.0f, 174.0f, 60.0f, 0.0f, 0.5f, 1.0f, 50);

	m_BSPDLightIndex[0] = m_lights->addVolumetricSpotLight(-52.0f, 59.0f, 133.0f, 70.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.5f, 1.0f, 7.0f, 2.0f); // BSPD
	m_BSPDLightIndex[1] = m_lights->addVolumetricSpotLight(-101.0f, 59.0f, 110.0f, 70.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.5f, 1.0f, 7.0f, 2.0f);

	// Skyscrapers
	m_lights->addAreaLight(85, 30, 75, 75, 0.0f, 0.6f, 0.8f, 25);
	m_lights->addAreaLight(85, 10, 75, 30, 1.0f, 1.0f, 1.0f, 25);
	m_lights->addAreaLight(35, 20, 77, 60, 0.5f, 0.0f, 0.8f, 25);
	m_lights->addAreaLight(172, 20, 71, 50, 0.5f, 0.0f, 0.8f, 25);
	m_lights->addAreaLight(10, 20, 80, 55, 0.0f, 0.6f, 0.8f, 23);

	m_lights->addAreaLight(-87.0f, 13.0f, 145.0f, 20.0f, 0.0f, 0.5f, 1.0f, 23);

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
	m_lights->addAreaLight(-53, -30, 135, 50, 0.2f, 0.7f, 1.0f, 10);
	m_lights->addAreaLight(-82, -30, 135, 50, 0.2f, 0.7f, 1.0f, 10);

	m_lights->addAreaLight(-125, 18, -9.4f, 50, 0.06f, 0.9f, 0.9f, 10); // Golden duck

	m_lights->addAreaLight(-84, 5.0f, -30.0f, 15.0f, 0.8f, 0.1f, 0.0f, 10); // China town
	m_lights->addAreaLight(-50, 5.0f, -30.0f, 15.0f, 0.8f, 0.1f, 0.0f, 10);

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

	// User interface
	m_userInterface = nullptr;

	// Sniper stuff
	m_lineShots.createVertexBuffer();

	m_sawInterval = 0.0f;

	// Quit Game
	m_quitGame = false;
}

GameState::~GameState()
{
	for (int i = 0; i < m_resources.size(); i++)
		delete m_resources[i];
	for (int i = 0; i < m_nodes.size(); i++)
		delete m_nodes[i];
	if (m_dboHandler)
		delete m_dboHandler;
	if (m_spawnDrone)
		delete m_spawnDrone;



	// User interface
	if (m_userInterface)
		delete m_userInterface;
}

void GameState::pause()
{
}

void GameState::resume()
{
}

void GameState::reset()
{
	// Delete first
	for (int i = 0; i < m_resources.size(); i++)
		delete m_resources[i];
	m_resources.clear();
	for (int i = 0; i < m_nodes.size(); i++)
		delete m_nodes[i];
	m_nodes.clear();
	if (m_spawnDrone)
		delete m_spawnDrone;

	delete m_userInterface;
	m_userInterface = nullptr;

	// Add again
	m_devZoomOut = false;
	m_BSPDtimer = 0.0f;

	// Spawn preset nodes and initialize spawning drone
	m_spawnDrone = new SpawnDrone(&m_resources);
	spawnNodes();

	// Initialize dynamic camera
	m_zoomingOutToStart = false;
	DX::getInstance()->getCam()->setCameraPosition(m_camStartPos);
	DX::getInstance()->getCam()->setLookAt(m_camStartLookAt);
	m_vecToCam = XMVector3Normalize(DX::getInstance()->getCam()->getPosition() - DX::getInstance()->getCam()->getLookAt());
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
}

void GameState::firstTimeSetUp(Game* game)
{
	// Set camera position and look at
	DX::getInstance()->getCam()->setPosition({0.0f, 120.0f, -110.0f});
	DX::getInstance()->getCam()->setLookAt({0.0f, 0.0f, 0.0f});

	m_robots = game->getRobots();
	m_quitGame = false;

	//// Create user interface (based on number of players) ////
	int nrOfPlayers = 0;
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		if (m_robots[i] != nullptr && m_robots[i]->isDrawn())
			nrOfPlayers++;
	}
	m_userInterface = new UserInterface(nrOfPlayers);  // Create user interface
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		if (m_robots[i] != nullptr && m_robots[i]->isDrawn())
		{
			m_userInterface->addPlayer(m_robots[i]->getRobotID());
		}
	}
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		if (m_robots[i] != nullptr && m_robots[i]->isDrawn())
		{
			m_userInterface->setPlayerColours(m_robots[i]->getRobotID(), m_robots[i]->getColour());
			m_lineShots.setColour(i, m_robots[i]->getColour());
		}
	}
	//Spawn AI:s
	const XMVECTOR ROBOT_START_POS[4]
	{
		XMVectorSet(-85.0f, 2.0f, 50.0f, 0),
		XMVectorSet(100.0f, 2.0f, -50.0f, 0),
		XMVectorSet(-85.0f, 2.0f, -50.0f, 0),
		XMVectorSet(120.0f, 2.0f, 50.0f, 0)
	};

	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		m_robots[i]->setDrawn(true);
		m_robots[i]->setPosition(ROBOT_START_POS[i]);
		m_robots[i]->storePositionInHistory(ROBOT_START_POS[i]);
	}
}




void GameState::handleInput(Game* game)
{
}

bool GameState::update(Game* game, float dt)
{
	if (m_quitGame)
		m_userInterface->updateQuitGame(dt);
	// Countdown
	if (m_userInterface->updateCountDown(dt) || m_quitGame)
		dt = 0.0f;


	m_input = game->getInput();
	m_robots = game->getRobots();
	if (handleInputs(game, dt))
		return 0;
	game->updatePlayerStatus();
	m_robots[3];
	// Update sounds
	Sound::getInstance()->update(dt);

	// Update dynamic camera
	updateDynamicCamera(dt);

	// Update spawning drone
	m_spawnDrone->update(m_robots, dt);

	// Update particles
	DX::getInstance()->getParticles()->update(dt);

	// Update billboards
	m_billboardHandler.updateBillboards(dt);
	for (int j = 0; j < 4; j++)
	{
		if (m_robots[j] != nullptr && m_robots[j]->isDrawn() && m_robots[j]->isAi())
		{
			XMVECTOR closestDelta = m_robots[j]->getAIRotation();
			float closestDistance = 10000;
			int closestIndex = -1;
			XMFLOAT2 playerPos;

			playerPos.x = m_robots[j]->getPosition().m128_f32[0];
			playerPos.y = m_robots[j]->getPosition().m128_f32[2];

			bool findPlayer = false;
			//Look at closest player
			for (int i = 0; i < 4; i++)
			{
				if (i != j && m_robots[i] != nullptr && m_robots[i]->isDrawn())
				{
					XMFLOAT2 opponentPos;
					opponentPos.x = m_robots[i]->getPosition().m128_f32[0];
					opponentPos.y = m_robots[i]->getPosition().m128_f32[2];
					XMVECTOR robotPos = XMVectorSet(playerPos.x, 2.0f, playerPos.y, 0.0f);
					XMVECTOR delta = m_robots[i]->getPosition() - robotPos;
					float distance = XMVectorGetX(XMVector3Length(delta));
					if (distance < closestDistance && !game->getQuadtree()->testCollision(playerPos, opponentPos, 3.4f))
					{
						findPlayer = true;
						closestDistance = distance;
						closestDelta = delta;
						closestIndex = i;
					}
				}
			}
			XMVECTOR norDelta = XMVector3Normalize(closestDelta);
			float degree = m_robots[j]->getCurrentRot();
			float newDegree = XMConvertToDegrees(atan2(XMVectorGetX(norDelta), XMVectorGetZ(norDelta)));
			float difference = abs(newDegree - degree);
			if (difference > 180)
			{
				// We need to add on to one of the values.
				if (newDegree > degree)
				{
					// We'll add it on to start...
					degree += 360;
				}
				else
				{
					// Add it on to end.
					newDegree += 360;
				}
			}

			// Interpolate it.
			float value = (degree + ((newDegree - degree) * 0.3f));

			// Wrap it..
			float rangeZero = 360;
			value = fmod(value, rangeZero);
			m_robots[j]->setCurrentRot(value);
			m_robots[j]->setRotation(0, 1, 0, value);
			float closest = 10000;
			int closestNode = -1;
			if (m_robots[j]->getResourceIndex() != -1)
			{
				for (int i = 0; i < m_nodes.size(); i++)
				{
					if (m_nodes[i]->isType(m_resources[m_robots[j]->getResourceIndex()]->getType()) && XMVectorGetX(XMVector3Length(Graph::getInstance()->getNodePos(i) - m_robots[j]->getPosition())) < closest)
					{
						closest = XMVectorGetX(XMVector3Length(Graph::getInstance()->getNodePos(i) - m_robots[j]->getPosition()));
						closestNode = i;
					}
				}
				if (closestNode != -1)
					m_robots[j]->setAIGoal(Graph::getInstance()->getNodePos(closestNode), m_updateMission);
			}
			else
			{
				if (closestDistance < 100 &&
					((m_robots[j]->getCurrentWeapon(RIGHT) != -1 && m_robots[j]->getWeapons()[m_robots[j]->getCurrentWeapon(RIGHT)]->getType() == BEYBLADE) ||
					(m_robots[j]->getCurrentWeapon(LEFT) != -1 && m_robots[j]->getWeapons()[m_robots[j]->getCurrentWeapon(LEFT)]->getType() == BEYBLADE)))
				{
					m_robots[j]->setAIGoal(m_robots[j]->getPosition() + closestDelta, true);
				}
				else if (closestDistance < 100 &&
					(m_robots[j]->getHealth() < 50 ||
					(m_robots[j]->getCurrentWeapon(RIGHT) != -1 && m_robots[j]->getWeapons()[m_robots[j]->getCurrentWeapon(RIGHT)]->getType() == SNIPER) ||
						(m_robots[j]->getCurrentWeapon(LEFT) != -1 && m_robots[j]->getWeapons()[m_robots[j]->getCurrentWeapon(LEFT)]->getType() == SNIPER) ||
						(m_robots[closestIndex]->getCurrentWeapon(RIGHT) != -1 && m_robots[closestIndex]->getWeapons()[m_robots[closestIndex]->getCurrentWeapon(RIGHT)]->getType() == BEYBLADE) ||
						(m_robots[closestIndex]->getCurrentWeapon(LEFT) != -1 && m_robots[closestIndex]->getWeapons()[m_robots[closestIndex]->getCurrentWeapon(LEFT)]->getType() == BEYBLADE)
						))
				{

					m_robots[j]->setAIGoal(m_robots[j]->getPosition() - XMVector3Normalize(closestDelta) * 15, true);
				}
				else if (closestIndex != -1 && m_robots[closestIndex]->getResourceIndex() != -1)
				{
					m_robots[j]->setAIGoal(m_robots[closestIndex]->getPosition(), true);
				}
				else
				{
					int tierList[9] = { 2, 7, 4, 1, 5, 0, 6, 3, 8 };
					float highestResource = -10000;
					int highestIndex = -1;
					for (int i = 0; i < m_resources.size(); i++)
					{
						//if (!m_resources[i]->isBlocked())
						{
							float distance = XMVectorGetX(XMVector3Length(m_robots[j]->getPosition() - m_resources[i]->getPosition()));
							if (tierList[m_resources[i]->getType()] / (distance / 30) - (50 * m_resources[i]->isBlocked()) > highestResource)
							{
								highestResource = tierList[m_resources[i]->getType()] / (distance / 30) - (50 * m_resources[i]->isBlocked());
								highestIndex = i;
							}
						}
					}
					if (highestIndex != -1)
						m_robots[j]->setAIGoal(m_resources[highestIndex]->getPosition(), m_updateMission);
					else
					{
						/* Hunt player*/
						float closestPlayer = 10000;
						int playerIndex = -1;
						for (int i = 0; i < XUSER_MAX_COUNT; i++)
						{
							if (m_robots[i] != nullptr && m_robots[i]->isDrawn())
							{
								float distance = XMVectorGetX(XMVector3Length(m_robots[j]->getPosition() - m_robots[i]->getPosition()));
								if (distance < closestPlayer)
								{
									playerIndex = i;
									closestPlayer = distance;
								}
							}
						}
						if (playerIndex != -1)
							m_robots[j]->setAIGoal(m_robots[playerIndex]->getPosition(), m_updateMission);
					}
				}
			}
			XMVECTOR start = XMVectorSet(0, 0, 0, 0);
			XMVECTOR end = XMVectorSet(0, 0, 0, 0);
			m_robots[j]->updateAIWeapon(findPlayer);
			//update ui
			if (m_robots[j]->getCurrentWeapon(LEFT) != -1)
			{
				int type = m_robots[j]->getWeapons()[m_robots[j]->getCurrentWeapon(LEFT)]->getType();
				m_userInterface->setSlotID(m_robots[j]->getRobotID(), type, LEFT, m_robots[j]->getNextWeapon(), m_robots[j]->getNextNextWeapon());

				if (findPlayer || type == MOVEMENT || type == DASH)
				{
					if (m_robots[j]->getCurrentWeapon(LEFT) != -1 && m_robots[j]->getWeapons()[m_robots[j]->getCurrentWeapon(LEFT)]->getType() == SNIPER &&
						m_robots[j]->getWeapons()[m_robots[j]->getCurrentWeapon(LEFT)]->getReady())
					{
						m_robots[j]->getSniperLine(LEFT, start, end);
						m_lineShots.updateLineStatus(j, start, end, true, dt);
						boundingData robotBD = game->getPreLoader()->getBoundingData(objectType::e_robot, 1, 0);
						for (int i = 0; i < 4; i++)
						{
							if (i != j && m_robots[i] != nullptr && m_robots[i]->isDrawn())
							{
								if (testLineSphere(start, end, m_robots[i]->getPosition(), robotBD.halfWD.x))
								{
									int resourceIndex = m_robots[i]->getResourceIndex();
									if (m_robots[i]->damagePlayer(m_robots[j]->getWeapons()[m_robots[j]->getCurrentWeapon(LEFT)]->getDamage(), end - start, -1))
									{
										if (!m_robots[i]->isAi())
											m_input->setVibration(i, 0.5f);

										if (resourceIndex != -1)
										{
											m_resources[resourceIndex]->setPosition(m_robots[i]->getPosition());
											m_resources[resourceIndex]->setBlocked(false);
										}
									}
								}
							}
						}
					}
					m_robots[j]->useWeapon(LEFT, dt);
				}
			}

			if (m_robots[j]->getCurrentWeapon(RIGHT) != -1)
			{
				int type = m_robots[j]->getWeapons()[m_robots[j]->getCurrentWeapon(RIGHT)]->getType();

				m_userInterface->setSlotID(m_robots[j]->getRobotID(), type, RIGHT, m_robots[j]->getNextWeapon(), m_robots[j]->getNextNextWeapon());
				if (findPlayer || type == MOVEMENT || type == DASH)
				{
					if (m_robots[j]->getCurrentWeapon(RIGHT) != -1 && m_robots[j]->getWeapons()[m_robots[j]->getCurrentWeapon(RIGHT)]->getType() == SNIPER &&
						m_robots[j]->getWeapons()[m_robots[j]->getCurrentWeapon(RIGHT)]->getReady())
					{
						m_robots[j]->getSniperLine(RIGHT, start, end);
						m_lineShots.updateLineStatus(j, start, end, true, dt);
						boundingData robotBD = game->getPreLoader()->getBoundingData(objectType::e_robot, 1, 0);
						for (int i = 0; i < 4; i++)
						{
							if (i != j && m_robots[i] != nullptr && m_robots[i]->isDrawn())
							{
								if (testLineSphere(start, end, m_robots[i]->getPosition(), robotBD.halfWD.x))
								{
									int resourceIndex = m_robots[i]->getResourceIndex();
									if (m_robots[i]->damagePlayer(m_robots[j]->getWeapons()[m_robots[j]->getCurrentWeapon(RIGHT)]->getDamage(), end - start, -1))
									{
										if (m_robots[i]->isAi())
											m_input->setVibration(i, 0.5f);

										if (resourceIndex != -1)
										{
											m_resources[resourceIndex]->setPosition(m_robots[i]->getPosition());
											m_resources[resourceIndex]->setBlocked(false);
										}
									}
								}
							}
						}
					}
					m_robots[j]->useWeapon(RIGHT, dt);
				}
			}
		}
	}
	m_updateMission = false;

	// Projectile movement
	ProjectileBank::getInstance()->moveProjectiles(dt);

	Robot** robots = game->getRobots();
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		XMVECTOR start = XMVectorSet(0, 0, 0, 0);
		XMVECTOR end = XMVectorSet(0, 0, 0, 0);
		if (m_robots[i] != nullptr && m_robots[i]->isDrawn())
		{
			m_robots[i]->update(dt, game->getQuadtree(), start, end);

			// SNIPER STUFF
			bool activated = false;
			for (int side = 0; side < 2; side++)
			{
				if (m_robots[i]->getCurrentWeapon(side) != -1 && m_robots[i]->getWeapons()[m_robots[i]->getCurrentWeapon(side)]->getType() == SNIPER)
				{
					activated = true;
					m_lineShots.setActive(i, true);
					m_lineShots.updateLineStatus(i, start, end, false, dt);
				}
				if (!activated)
					m_lineShots.setActive(i, false);
			}
			XMVECTOR pos = m_robots[i]->getPosition();
			pos.m128_f32[3] = 1;
			m_transparency.update(pos, DX::getInstance()->getCam()->getViewMatrix(), DX::getInstance()->getCam()->getProjectionMatrix(), i);
		}
		else
			m_transparency.update(XMVectorSet(1000, 0, 1000, 0), XMMatrixIdentity(), XMMatrixIdentity(), i);
	}

	// Collision melee weapon
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		if (m_robots[i] != nullptr && m_robots[i]->isDrawn())
		{


			std::vector<Weapon*> weapons = m_robots[i]->getWeapons();
			int bladeIdx = -1;
			int leftSide = m_robots[i]->getCurrentWeapon(LEFT);
			if (weapons[m_robots[i]->getCurrentWeapon(RIGHT)]->getType() == BEYBLADE)
				bladeIdx = m_robots[i]->getCurrentWeapon(RIGHT);
			else if (leftSide != -1 && weapons[m_robots[i]->getCurrentWeapon(LEFT)]->getType() == BEYBLADE)
				bladeIdx = m_robots[i]->getCurrentWeapon(LEFT);

			// If player is using Beyblade
			if (bladeIdx != -1)
			{
				float beybladeRange = weapons[bladeIdx]->getRange();
				XMVECTOR beybladeRobPos = m_robots[i]->getPosition();
				m_sawInterval += dt;

				// Add spark if colliding with static objects
				boundingData beybladeBD{
					beybladeRobPos,
					XMFLOAT2(beybladeRange, beybladeRange),
					XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
					XMVectorSet(0.0f, 0.0f, 0.f, 0.0f)
				};
				CollisionInfo colInfo;
				colInfo = game->getQuadtree()->testCollision(beybladeBD, beybladeRobPos);
				if (m_sawInterval > weapons[bladeIdx]->getSpinTime() && colInfo.m_colliding)
				{
					m_sawInterval = 0.0f;
					Sound::getInstance()->play(soundEffect::e_sawcut, colInfo.m_contactPoint, 0.02f);
					XMVECTOR cutDir = XMVector3Cross((colInfo.m_contactPoint - beybladeRobPos), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
					DX::getInstance()->getParticles()->addCutSpark(colInfo.m_contactPoint, cutDir);
				}


				// Loop through and possibly damage other players
				for (int j = 1; j < XUSER_MAX_COUNT; j++)
				{
					int otherIdx = (i + j) % XUSER_MAX_COUNT;
					if (m_robots[otherIdx] != nullptr && m_robots[otherIdx]->isDrawn())
					{
						XMVECTOR otherRobPos = m_robots[otherIdx]->getPosition();
						XMVECTOR dirToVictim = otherRobPos - beybladeRobPos;
						float disToVic = XMVector3Length(dirToVictim).m128_f32[0];
						float beyBladeDamage = weapons[bladeIdx]->getSpinDPS() * dt;

						// Beyblade is within range, damage player.
						if (beyBladeDamage != 0.0f && disToVic <= beybladeRange)
						{
							// Play sawcut sound and add spark
							if (m_sawInterval > 1.0f / (weapons[bladeIdx]->getSpinPerSec() * 4.0f / 360.0f + 0.01f))
							{
								m_sawInterval = 0.0f;
								Sound::getInstance()->play(soundEffect::e_sawcut, otherRobPos, 0.02f);

								// Add spark
								XMVECTOR dir = XMVector3Cross(dirToVictim, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
								XMVECTOR cutPos = beybladeRobPos + XMVector3Normalize(dirToVictim) * (disToVic - 1.54710078f);
								DX::getInstance()->getParticles()->addCutSpark(cutPos, dir);
							}

							// Set vibration and drop resource
							int resourceIndex = m_robots[otherIdx]->getResourceIndex();
							if (m_robots[otherIdx]->damagePlayer(beyBladeDamage, dirToVictim, -1, false, false))
							{
								if (!m_robots[otherIdx]->isAi())
									m_input->setVibration(otherIdx, 0.5f);
								if (resourceIndex != -1)
								{
									m_updateMission = true;
									m_resources[resourceIndex]->setPosition(m_robots[otherIdx]->getPosition());
									m_resources[resourceIndex]->setBlocked(false);
								}
							}

						}
					}
				}
			}
		}
	}

	// COLLISION PROJECTILES VS STATIC OBJECTS

	for (int i = 0; i < ProjectileBank::getInstance()->getList().size(); i++)
	{
		if (ProjectileBank::getInstance()->getList()[i]->getType() == ENERGY && !ProjectileBank::getInstance()->getList()[i]->isExploding())
		{
			DX::getInstance()->getParticles()->addParticles(ProjectileBank::getInstance()->getList()[i]->getPosition(),
				ProjectileBank::getInstance()->getList()[i]->getData().material.diffuse, XMVectorSet(1.3f, 1.3f, 0, 0), 1, 5.0f
			);
		}

		boundingData projectileBD = game->getPreLoader()->getBoundingData(objectType::e_projectile, 0, 0);
		boundingData robotBD = game->getPreLoader()->getBoundingData(objectType::e_robot, 1, 0);

		// Save projectile pointer
		Projectile* projectile = ProjectileBank::getInstance()->getList()[i];

		// Update bounding data
		projectileBD.pos = projectile->getData().pos;

		// Test collision
		CollisionInfo collisionInfo;
		XMVECTOR nextPos = projectile->getPosition() + (projectile->getDirection() * projectile->getVelocity() * dt * 2.0f);
		//CollisionInfo collisionInfo = game->getQuadtree()->testCollision(projectileBD);
		XMFLOAT2 start, end;
		start.x = XMVectorGetX(projectile->getPosition());
		start.y = XMVectorGetZ(projectile->getPosition());
		end.x = XMVectorGetX(nextPos);
		end.y = XMVectorGetZ(nextPos);
		float t = game->getQuadtree()->testCollisionT(start, end);
		// Remove based on conditions
		if (t != -1.0f) // collisionInfo.m_colliding && 
		{
			// Collision against static object found, remove projectile
			projectile->setPosition(projectile->getPosition() + projectile->getDirection() * t);

			if (ProjectileBank::getInstance()->getList()[i]->getType() == ENERGY)
			{
				if (!ProjectileBank::getInstance()->getList()[i]->isExploding())
				{
					DX::getInstance()->getParticles()->addParticles(ProjectileBank::getInstance()->getList()[i]->getPosition(), ProjectileBank::getInstance()->getList()[i]->getData().material.diffuse, XMVectorSet(1.3f, 1.3f, 0, 0), 25, 40);
					//Damage surrounding players
					for (int k = 0; k < XUSER_MAX_COUNT; k++)
					{
						if (robots[k] != nullptr && robots[k]->isDrawn())
						{
							float distance = XMVectorGetX(XMVector3Length(ProjectileBank::getInstance()->getList()[i]->getPosition() - robots[k]->getPosition()));
							if (distance < ProjectileBank::getInstance()->getList()[i]->getBlastRange())
							{
								int resourceIndex = m_robots[k]->getResourceIndex();
								if (robots[k]->damagePlayer((ProjectileBank::getInstance()->getList()[i]->getDamage() * 0.5f * (1 + ((ProjectileBank::getInstance()->getList()[i]->getBlastRange() - distance) / ProjectileBank::getInstance()->getList()[i]->getBlastRange()))), ProjectileBank::getInstance()->getList()[i]->getDirection(), -1, false))
								{
									if (!robots[k]->isAi())
										m_input->setVibration(k, 1.0f);

									if (resourceIndex != -1)
									{
										m_resources[resourceIndex]->setPosition(m_robots[k]->getPosition());
										m_resources[resourceIndex]->setBlocked(false);
									}
								}


							}
						}
					}
					ProjectileBank::getInstance()->getList()[i]->explode();
				}
			}

			else
			{
				// Add spark particles
				Sound::getInstance()->play(soundEffect::e_impact, ProjectileBank::getInstance()->getList()[i]->getPosition(), 0.05f);
				DX::getInstance()->getParticles()->addSpark(projectile->getData().pos, projectile->getDirection());
				ProjectileBank::getInstance()->removeProjectile(i);
			}
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


					if (collisionInfo.m_colliding && ProjectileBank::getInstance()->getList()[i]->getOwner() != j)
					{
						// Add spark particles
						if (ProjectileBank::getInstance()->getList()[i]->getType() == ENERGY)
						{
							if (!ProjectileBank::getInstance()->getList()[i]->isExploding())
							{


								for (int k = 0; k < XUSER_MAX_COUNT; k++)
								{
									if (robots[k] != nullptr && robots[k]->isDrawn())
									{
										float distance = XMVectorGetX(XMVector3Length(ProjectileBank::getInstance()->getList()[i]->getPosition() - robots[k]->getPosition()));
										if (distance < ProjectileBank::getInstance()->getList()[i]->getBlastRange())
										{
											int resourceIndex = m_robots[k]->getResourceIndex();
											if (robots[k]->damagePlayer(ProjectileBank::getInstance()->getList()[i]->getDamage() * 0.5f * (1 + ((ProjectileBank::getInstance()->getList()[i]->getBlastRange() - distance) / ProjectileBank::getInstance()->getList()[i]->getBlastRange())), ProjectileBank::getInstance()->getList()[i]->getDirection(), -1, false))
											{
												if (!robots[k]->isAi())
													m_input->setVibration(k, 1.0f);

												if (resourceIndex != -1)
												{
													m_updateMission = true;
													m_resources[resourceIndex]->setPosition(m_robots[k]->getPosition());
													m_resources[resourceIndex]->setBlocked(false);
												}
											}


										}
									}
								}

								m_robots[j]->damagePlayer(ProjectileBank::getInstance()->getList()[i]->getDamage(), ProjectileBank::getInstance()->getList()[i]->getDirection(), i, false);
								
								ProjectileBank::getInstance()->getList()[i]->explode();
							}

						}
						else
						{
							DX::getInstance()->getParticles()->addSpark(projectile->getData().pos, projectile->getDirection());

							int resourceIndex = m_robots[j]->getResourceIndex();
							if (m_robots[j]->damagePlayer(ProjectileBank::getInstance()->getList()[i]->getDamage(), ProjectileBank::getInstance()->getList()[i]->getDirection(), i))
							{
								if (!robots[j]->isAi())
									m_input->setVibration(j, 0.5f);
								if (resourceIndex != -1)
								{
									m_updateMission = true;
									m_resources[resourceIndex]->setPosition(m_robots[j]->getPosition());
									m_resources[resourceIndex]->setBlocked(false);
								}
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
		if (m_nodes[i]->updateTime(dt))
			m_updateMission = true;
	}

	// Blade runner lights
	bspdLightUpdate(dt);

	// Update user interface
	m_userInterface->update();

	//Dynamic background objects
	m_dboHandler->update(dt);

	// Check if there's only one player alive
	// If so then add to that player's score and change to ScoreState
	int nrOfPlayersAlive = 0, winner = -1;
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		// Check for nullptr so as to not crash the game when checking for isDrawn
		if (m_robots[i] != nullptr)
		{
			if (m_robots[i]->isDrawn()) // isDrawn indicates whether the player is dead or alive
			{
				winner = i; // Keep track of the winner
				nrOfPlayersAlive++; // Count how many players are alive
			}
		}
	}
	if (nrOfPlayersAlive <= 1) // If zero or one person is alive then change to scorestate 
	{
		if (winner != -1)
			m_robots[winner]->addScore(1); // Award one point to the winning player 
		// If no one is left alive it's a tie and no points are awarded 
		setPaused(true); // Pause this state
		game->changeState(stateType::e_scoreState); // Change state to ScoreState
	}

	return 0;
}

void GameState::draw(Game* game, renderPass pass)
{
	m_input = game->getInput();
	m_robots = game->getRobots();

	if (pass == renderPass::e_particles)
	{
		DX::getInstance()->getParticles()->draw();
	}

	if (pass == renderPass::e_billboard || pass == renderPass::e_shadow)
	{
		std::vector<Billboard> BB = m_billboardHandler.getBillboards();

		m_spawnDrone->setConstantBuffer(true);
		game->getPreLoader()->draw(objectType::e_BSPD_Screen);
		m_spawnDrone->setConstantBuffer(false);

		for (int i = 0; i < m_billboardHandler.getNrOfBillboards(); ++i)
			game->getPreLoader()->draw(BB[i].getObjectType(), BB[i].getBillboardData(), BB[i].getModelNr(), BB[i].getSubModelNumber(), BB[i].getVariant());
	}

	// User interface
	if (pass == renderPass::e_userInterface)
	{
		if (!m_quitGame)
		{
			m_userInterface->draw(); // Draw player box

			for (int i = 0; i < XUSER_MAX_COUNT; i++) // Draw ability icons
			{
				if (m_robots[i] != nullptr && m_robots[i]->isDrawn())
				{
					int size = (int)m_robots[i]->getWeapons().size();
					for (int j = 0; j < size; j++)
					{
						if (m_robots[i]->getWeapons()[j]->getType() != RIFLE) // TODO: fix this bugg
							m_userInterface->drawAbility(m_robots[i]->getRobotID(), m_robots[i]->getWeapons()[j]->getType(), m_robots[i]->getWeapons()[j]->getCD());
						m_userInterface->drawHealthbar(m_robots[i]->getRobotID(), m_robots[i]->getHealth() / 100.0f);
					}
				}
			}
		}
		else
		{
			m_userInterface->drawQuitGame();
		}
	}

	if (pass == renderPass::e_opaque || pass == renderPass::e_shadow)
	{
		for (int i = 0; i < m_nodes.size(); i++)
		{
			game->getPreLoader()->draw(objectType::e_node, m_nodes[i]->getData(), i, 0);
		}

		for (int i = 0; i < XUSER_MAX_COUNT; i++)
		{
			if (m_robots[i] != nullptr && m_robots[i]->isDrawn())
			{
				std::vector<Weapon*> weapons = m_robots[i]->getWeapons();

				game->getPreLoader()->setSubModelData(objectType::e_robot, game->getRobots()[i]->getData(), 0, 1);
				//game->getPreLoader()->setSubModelData(objectType::e_robot, game->getRobots()[i]->getData(), 0, 6);
				game->getPreLoader()->draw(objectType::e_robot);

				// REFLECT AND SHIELD
				for (int side = 0; side < 2; side++)
				{
					if (game->getRobots()[i]->getCurrentWeapon(side) != -1 &&
						(m_robots[i]->getWeapons()[m_robots[i]->getCurrentWeapon(side)]->getType() == REFLECT || m_robots[i]->getWeapons()[m_robots[i]->getCurrentWeapon(side)]->getType() == SHIELD))
					{
						objectData od = m_robots[i]->getWeapons()[m_robots[i]->getCurrentWeapon(side)]->getData();
						od.material.ambient = XMVectorSet(-1.0f, -1.0f, -1.0f, 1.0f);
						od.material.diffuse = m_robots[i]->getColour();
						od.material.emission = m_robots[i]->getColour();

						if (m_robots[i]->getWeapons()[m_robots[i]->getCurrentWeapon(side)]->getType() == REFLECT)
							game->getPreLoader()->setSubModelData(objectType::e_weapon, od, 1, 2, 4);
						if (m_robots[i]->getWeapons()[m_robots[i]->getCurrentWeapon(side)]->getType() == SHIELD)
							game->getPreLoader()->setSubModelData(objectType::e_weapon, od, 1, 1, 5);
					}
				}

				int wepType = weapons[m_robots[i]->getCurrentWeapon(RIGHT)]->getType();
				switch (wepType)
				{
				case BEYBLADE:
					game->getPreLoader()->draw(objectType::e_weapon, weapons[m_robots[i]->getCurrentWeapon(RIGHT)]->getData(), m_robots[i]->getData(), 0, 0, 1, false);
					break;

				case ENERGY:
					game->getPreLoader()->draw(objectType::e_weapon, weapons[m_robots[i]->getCurrentWeapon(RIGHT)]->getData(), m_robots[i]->getData(), 0, 2, 2, false);
					break;

				case SNIPER:
					game->getPreLoader()->draw(objectType::e_weapon, weapons[m_robots[i]->getCurrentWeapon(RIGHT)]->getData(), m_robots[i]->getData(), 0, 3, 3, false);
					break;

				case REFLECT:
					game->getPreLoader()->drawOneModelAndMat(objectType::e_weapon, weapons[m_robots[i]->getCurrentWeapon(RIGHT)]->getData(), m_robots[i]->getData(), 1, 4);
					break;

				case SHIELD:
					game->getPreLoader()->drawOneModelAndMat(objectType::e_weapon, weapons[m_robots[i]->getCurrentWeapon(RIGHT)]->getData(), m_robots[i]->getData(), 1, 5);
					break;

				case MOVEMENT:
					game->getPreLoader()->draw(objectType::e_weapon, weapons[m_robots[i]->getCurrentWeapon(RIGHT)]->getData(), m_robots[i]->getData(), 0, 1, 6, false);
					break;

				case DASH:
					game->getPreLoader()->draw(objectType::e_weapon, weapons[m_robots[i]->getCurrentWeapon(RIGHT)]->getData(), m_robots[i]->getData(), 0, 1, 7, false);
					break;

				default:
					game->getPreLoader()->draw(objectType::e_weapon, weapons[m_robots[i]->getCurrentWeapon(RIGHT)]->getData(), m_robots[i]->getData());
					break;
				}

				if (game->getRobots()[i]->getCurrentWeapon(LEFT) != -1)
				{
					int wepType = weapons[game->getRobots()[i]->getCurrentWeapon(LEFT)]->getType();
					switch (wepType)
					{
					case BEYBLADE:
						game->getPreLoader()->draw(objectType::e_weapon, weapons[game->getRobots()[i]->getCurrentWeapon(LEFT)]->getData(), m_robots[i]->getData(), 0, 0, 1, false);
						break;

					case ENERGY:
						game->getPreLoader()->draw(objectType::e_weapon, weapons[m_robots[i]->getCurrentWeapon(LEFT)]->getData(), m_robots[i]->getData(), 0, 2, 2, false);
						break;

					case SNIPER:
						game->getPreLoader()->draw(objectType::e_weapon, weapons[m_robots[i]->getCurrentWeapon(LEFT)]->getData(), m_robots[i]->getData(), 0, 3, 3, false);
						break;

					case REFLECT:
						game->getPreLoader()->drawOneModelAndMat(objectType::e_weapon, weapons[m_robots[i]->getCurrentWeapon(LEFT)]->getData(), m_robots[i]->getData(), 1, 4);
						break;

					case SHIELD:
						game->getPreLoader()->drawOneModelAndMat(objectType::e_weapon, weapons[m_robots[i]->getCurrentWeapon(LEFT)]->getData(), m_robots[i]->getData(), 1, 5);
						break;

					case MOVEMENT:
						game->getPreLoader()->draw(objectType::e_weapon, weapons[m_robots[i]->getCurrentWeapon(LEFT)]->getData(), m_robots[i]->getData(), 0, 1, 6, false);
						break;

					case DASH:
						game->getPreLoader()->draw(objectType::e_weapon, weapons[m_robots[i]->getCurrentWeapon(LEFT)]->getData(), m_robots[i]->getData(), 0, 1, 7, false);
						break;

					default:
						game->getPreLoader()->draw(objectType::e_weapon, weapons[game->getRobots()[i]->getCurrentWeapon(LEFT)]->getData(), game->getRobots()[i]->getData());
						break;
					}
				}
			}
		}
		game->getPreLoader()->draw(objectType::e_ground);

		objectData tempData;
		tempData.material =
		{
			0.1f, 0.1f, 0.1f, 0.0f,
			0.1f, 0.1f, 0.1f, 0.0f,
			0.1f, 0.1f, 0.1f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
		};

		for (int i = 0; i < m_resources.size(); i++)
		{
			int resType = m_resources[i]->getType();

			switch (resType)
			{
			case BEYBLADE: // Beyblade
				game->getPreLoader()->draw(objectType::e_resource, m_resources[i]->getData(), 0, 0, 1);
				break;

			case ENERGY:
				game->getPreLoader()->setSubModelData(objectType::e_resource, tempData, 0, 2, 2);
				game->getPreLoader()->draw(objectType::e_resource, m_resources[i]->getData(), 0, 0, 2);
				break;

			case SNIPER:
				game->getPreLoader()->draw(objectType::e_resource, m_resources[i]->getData(), 0, 0, 3);
				break;

			case REFLECT:
				game->getPreLoader()->setSubModelData(objectType::e_resource, m_resources[i]->getData(), 1, 2, 4);
				game->getPreLoader()->draw(objectType::e_resource, m_resources[i]->getData(), 0, 0, 4);
				break;

			case SHIELD:
				game->getPreLoader()->setSubModelData(objectType::e_resource, m_resources[i]->getData(), 1, 1, 5);
				game->getPreLoader()->draw(objectType::e_resource, m_resources[i]->getData(), 0, 0, 5);
				//tempData.material.emission = XMVectorSet(0, 0, 0, -1);
				//game->getPreLoader()->setSubModelData(objectType::e_resource, tempData, 1, 0, 5);
				//game->getPreLoader()->drawOneModelAndMat(objectType::e_resource, m_resources[i]->getData(), 1, 5);
				break;

			case MOVEMENT:
				game->getPreLoader()->setSubModelData(objectType::e_resource, tempData, 0, 1, 6);
				game->getPreLoader()->draw(objectType::e_resource, m_resources[i]->getData(), 0, 0, 6);
				break;

			case DASH:
				game->getPreLoader()->setSubModelData(objectType::e_resource, tempData, 0, 1, 7);
				game->getPreLoader()->draw(objectType::e_resource, m_resources[i]->getData(), 0, 0, 7);
				break;

			default:
				game->getPreLoader()->draw(objectType::e_resource, m_resources[i]->getData(), 0, 0);
				break;
			}
		}
	}

	if (pass == renderPass::e_transparent || pass == renderPass::e_shadow)
	{
		m_transparency.bindConstantBuffer();
		// Scene (Background objects without collision)
		for (int i = 0; i < game->getPreLoader()->getNrOfVariants(objectType::e_scene); i++)
			game->getPreLoader()->draw(objectType::e_scene, i);

		game->getPreLoader()->draw(objectType::e_BSPD_Door, m_spawnDrone->getData(4));

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

		// Projectiles
		for (int i = 0; i < ProjectileBank::getInstance()->getList().size(); i++)
		{
			for (int i = 0; i < ProjectileBank::getInstance()->getList().size(); i++)
			{
				if (ProjectileBank::getInstance()->getList()[i]->getType() == ENERGY && ProjectileBank::getInstance()->getList()[i]->isExploding() && pass != renderPass::e_shadow)
					game->getPreLoader()->draw(objectType::e_projectile, ProjectileBank::getInstance()->getList()[i]->getData(), 0, 0, 1);
				else if (ProjectileBank::getInstance()->getList()[i]->getType() == ENERGY && !ProjectileBank::getInstance()->getList()[i]->isExploding())
				{
					objectData xd = ProjectileBank::getInstance()->getList()[i]->getData();
					xd.material.ambient = XMVectorSet(-1, -1, -1, 3.0f);
					xd.material.emission = XMVectorSet(-1, -1, -1, 0.15f);
					game->getPreLoader()->drawOneModelAndMat(objectType::e_projectile, ProjectileBank::getInstance()->getList()[i]->getData(), 1, 2);
					game->getPreLoader()->drawOneModelAndMat(objectType::e_projectile, xd, 0, 2);
				}
				else if (ProjectileBank::getInstance()->getList()[i]->getType() != ENERGY || pass != renderPass::e_shadow)
					game->getPreLoader()->draw(objectType::e_projectile, ProjectileBank::getInstance()->getList()[i]->getData(), 0, 1);
			}
		}

		// Robot stuff
		for (int i = 0; i < XUSER_MAX_COUNT; i++)
		{
			if (m_robots[i] != nullptr && m_robots[i]->isDrawn())
			{
				std::vector<Weapon*> weapons = m_robots[i]->getWeapons();

				// REFLECT AND SHIELD
				for (int side = 0; side < 2; side++)
				{
					if (game->getRobots()[i]->getCurrentWeapon(side) != -1 &&
						(m_robots[i]->getWeapons()[m_robots[i]->getCurrentWeapon(side)]->getType() == REFLECT || m_robots[i]->getWeapons()[m_robots[i]->getCurrentWeapon(side)]->getType() == SHIELD))
					{

						objectData od = m_robots[i]->getWeapons()[m_robots[i]->getCurrentWeapon(side)]->getData();
						od.material.ambient = XMVectorSet(-1.0f, -1.0f, -1.0f, 1.0f);
						od.material.diffuse = m_robots[i]->getColour();
						od.material.emission = XMVectorSet(-1.0f, -1.0f, -1.0f, 0.2f);

						if (m_robots[i]->getWeapons()[m_robots[i]->getCurrentWeapon(side)]->getType() == REFLECT)
							game->getPreLoader()->setSubModelData(objectType::e_weapon, od, 0, 0, 4);
						if (m_robots[i]->getWeapons()[m_robots[i]->getCurrentWeapon(side)]->getType() == SHIELD)
							game->getPreLoader()->setSubModelData(objectType::e_weapon, od, 0, 0, 5);
					}
				}

				int wepType = weapons[m_robots[i]->getCurrentWeapon(RIGHT)]->getType();
				if (weapons[m_robots[i]->getCurrentWeapon(RIGHT)]->getActive())
				{
					switch (wepType)
					{
					case REFLECT:
						game->getPreLoader()->drawOneModelAndMat(objectType::e_weapon, weapons[m_robots[i]->getCurrentWeapon(RIGHT)]->getData(), m_robots[i]->getData(), 0, 4);
						break;

					case SHIELD:
						game->getPreLoader()->drawOneModelAndMat(objectType::e_weapon, weapons[m_robots[i]->getCurrentWeapon(RIGHT)]->getData(), m_robots[i]->getData(), 0, 5);
						break;

					default:
						break;
					}
				}
				if (game->getRobots()[i]->getCurrentWeapon(LEFT) != -1 && weapons[m_robots[i]->getCurrentWeapon(LEFT)]->getActive())
				{
					int wepType = weapons[game->getRobots()[i]->getCurrentWeapon(LEFT)]->getType();
					switch (wepType)
					{
					case REFLECT:
						game->getPreLoader()->drawOneModelAndMat(objectType::e_weapon, weapons[m_robots[i]->getCurrentWeapon(LEFT)]->getData(), m_robots[i]->getData(), 0, 4);
						break;

					case SHIELD:
						game->getPreLoader()->drawOneModelAndMat(objectType::e_weapon, weapons[m_robots[i]->getCurrentWeapon(LEFT)]->getData(), m_robots[i]->getData(), 0, 5);
						break;

					default:
						break;
					}
				}
			}

			if (m_robots[i] != nullptr && m_robots[i]->isDrawn())
			{
				m_lineShots.draw(i);
			}
		}
	}
}