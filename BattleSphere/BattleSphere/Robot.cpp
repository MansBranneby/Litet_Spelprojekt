#include "Robot.h"

Robot::Robot(int playerId)
{
	m_currentMission.index = 0;
	m_currentMission.pos = getPosition();
	m_isAi = false;
	m_robotID = -1;
	m_playerId = playerId;
	m_health = 100;
	m_velocity = 55.0f;
	m_vel = XMVectorSet(0, 0, 0, 0);
	m_currentRotation = 0.0;
	m_currentWeapon[LEFT] = -1;
	m_currentWeapon[RIGHT] = 0;
	//m_weaponPointer = 0;
	m_nextW = -1;
	m_nextnextW = -1;
	m_score = 0;
	m_resource = -1;
	m_currentWeapon[LEFT] = -1;
	m_currentWeapon[RIGHT] = 0;
	Weapon* wep = new Weapon(RIFLE);
	m_weapons.push_back(wep);
	m_ready = true;
	m_time = 0;
	m_material.ambient = XMVectorSet(0.5, 0.5, 0.5, -1);
	m_material.diffuse = XMVectorSet(0.0, 0.0, 0.0, -1);
	m_material.emission = XMVectorSet(0.0, 0.0, 0.0, -1);
	m_colour = XMVectorSet(1.0, 1.0, 1.0, -1);
	//setScale(2.2f, 2.2f, 2.2f);
	// Raise player
	setPosition(XMVECTOR{ 10.0f, 1.0f, 0.0f });
	m_vel = XMVectorSet(0, 0, 0, 0);

	m_lightIndex = Lights::getInstance()->addPointLight(0, 0, 0, 10, 1, 1, 1, 10);
	// Position history
	m_positionHistorySize = 0;
	m_positionHistoryCap = 100;
	m_positionHistoryPtr = 0;
	m_positionHistory = new DirectX::XMVECTOR[m_positionHistoryCap];
	m_positionHistory[m_positionHistoryCap - 1] = getPosition();

	// Particles
	m_timeSinceParticles = 0.0f;
}

bool Robot::isAi()
{
	return m_isAi;
}

void Robot::updateAIWeapon(bool seePlayer)
{
	///int* tierListR;
	int tierList[8];

	if (seePlayer)
	{
	
		tierList[0] = 6;
		tierList[1] = 2;
		tierList[2] = 1;
		tierList[3] = 3;
		tierList[4] = 0;
		tierList[5] = 5;
		tierList[6] = 4;
		tierList[7] = 7;

		
		
	}
	else {
		tierList[0] = 4;
		tierList[1] = 7;
		tierList[2] = 1;
		tierList[3] = 6;
		tierList[4] = 0;
		tierList[5] = 3;
		tierList[6] = 2;
		tierList[7] = 5;

		

	}
	int highestR = -1, highestL = -1;
	int indexR = -1, indexL = -1;
	for (int k = 0; k < (int)getWeapons().size(); k++)
	{
		int tier = tierList[getWeapons()[k]->getType()];
		//int tierR = tierListL[getWeapons()[k]->getType()];
		if (tier > highestR)
		{
			if (indexR != -1 && highestR > highestL)
			{
				highestL = highestR;
				indexL = indexR;
				indexR = k;
				highestR = tier;
			}
			else
			{
				indexR = k;
				highestR = tier;
			}
		}
		else if (tier > highestL)
		{
			indexL = k;
			highestL = tier;
		}

	}
	for (int k = 0; k < (int)getWeapons().size(); k++)
	{
		if (getCurrentWeapon(RIGHT) != -1)
		{
			if (getCurrentWeapon(RIGHT) != indexR)
			{
				changeWeapon(RIGHT);
			}
		}
		if (getCurrentWeapon(LEFT) != -1)
		{
			if (getCurrentWeapon(LEFT) != indexL)
			{
				changeWeapon(LEFT);
			}
		}

	}
	
}

void Robot::setAi(bool ai)
{
	m_isAi = ai;
}

void Robot::setAIGoal(XMVECTOR position, bool update)
{
	if (m_currentMission.index == 0 || update)
	{
		std::vector<XMVECTOR> a = Graph::getInstance()->calculateAIPath(getPosition(), position);
		if (a.size() > 0)
		{
			m_currentMission = m_ai.setMission(a);
		}
	}
	
}

XMVECTOR Robot::getAIRotation()
{
	return m_ai.getAIRotation(m_currentMission);

}

void Robot::setPlayerId(int playerId)
{
	m_playerId = playerId;
}

void Robot::setRobotID(int id)
{
	m_robotID = id;
}

int Robot::getRobotID()
{
	return m_robotID;
}

int Robot::getPlayerId()
{
	return m_playerId;
}

bool Robot::damagePlayer(float damage, XMVECTOR projDir, int projIndex, bool deleteProjectile, bool playSound)
{
	bool explode = true;
	if (m_currentWeapon[RIGHT] != -1)
	{
		damage *= m_weapons[m_currentWeapon[RIGHT]]->getDefense(m_playerId, projDir, getPosition(), m_colour, m_currentRotation, projIndex);
		if (m_weapons[m_currentWeapon[RIGHT]]->getType() == REFLECT && m_weapons[m_currentWeapon[RIGHT]]->getActive())
			explode = false;
	}
	if (m_currentWeapon[LEFT] != -1)
	{
		damage *= m_weapons[m_currentWeapon[LEFT]]->getDefense(m_playerId, projDir, getPosition(), m_colour, m_currentRotation, projIndex);
		if (m_weapons[m_currentWeapon[LEFT]]->getType() == REFLECT && m_weapons[m_currentWeapon[LEFT]]->getActive())
			explode = false;
	}

	if (projIndex != -1 && deleteProjectile)
	{
		ProjectileBank::getInstance()->removeProjectile(projIndex);
	}
	else if (projIndex != -1 && ProjectileBank::getInstance()->getList()[projIndex]->getType() == ENERGY && explode)
		ProjectileBank::getInstance()->getList()[projIndex]->explode();


	if (damage != 0.0f)
	{
		if (playSound)
			Sound::getInstance()->play(soundEffect::e_damage, getPosition(), 0.3f);
		m_health -= damage;
		if (m_health < 0)
		{
			m_health = 0;
			setDrawn(false);
			deathAnimation();
		}
		m_material.emission = m_colour * m_health / 100.0f;
		removeResource();
		return true;
	}
	else if (playSound)
		Sound::getInstance()->play(soundEffect::e_impact, getPosition(), 0.05f);
	return false;
}

void Robot::setHealth(float health)
{
	setDrawn(true);
	m_health = health;
	m_material.emission = m_colour;
}

float Robot::getHealth()
{
	return m_health;
}

void Robot::setVelocity(float velocity)
{
	m_velocity = velocity;
}

float Robot::getVelocity()
{
	float velocity = m_velocity;
	if (m_currentWeapon[RIGHT] != -1)
		velocity *= m_weapons[m_currentWeapon[RIGHT]]->getSpeed();
	if (m_currentWeapon[LEFT] != -1)
		velocity *= m_weapons[m_currentWeapon[LEFT]]->getSpeed();
	return velocity;
}

void Robot::setCurrentRot(float deg)
{
	m_currentRotation = deg;
}

float Robot::getCurrentRot()
{
	return m_currentRotation;
}

bool Robot::isReady(float dt)
{
	if (!m_ready)
	{
		m_time += dt;
		if (m_time > 0.5f)
		{
			m_time = 0;
			m_ready = true;
		}
		return false;
	}
	return true;
}

void Robot::setColour(float x, float y, float z)
{
	m_material.emission = m_colour = XMVectorSet(x, y, z, -1);
}

XMVECTOR Robot::getColour()
{
	return m_colour;
}

void Robot::getSniperLine(int side, XMVECTOR& start, XMVECTOR& end)
{
	m_weapons[m_currentWeapon[side]]->getSniperLine(start, end);
}

void Robot::useWeapon(int side, float dt)
{
	if (m_currentWeapon[side] != -1)
	{
		m_weapons[m_currentWeapon[side]]->shoot(m_playerId, getPosition(), m_colour, m_currentRotation, side, dt);
		m_weapons[m_currentWeapon[side]]->speedUp(getPosition());
		m_weapons[m_currentWeapon[side]]->shield();
		m_weapons[m_currentWeapon[side]]->reflect();
		m_weapons[m_currentWeapon[side]]->spin(dt);
	}
}

int Robot::changeWeapon(int side)
{
	if (m_weapons.size() > 2)
	{
		//m_currentWeapon[side] = (m_currentWeapon[side] + 1) % (int)m_weapons.size();
		//if (m_currentWeapon[side] == m_currentWeapon[(side + 1) % 2])
		//	m_currentWeapon[side] = (m_currentWeapon[side] + 1) % (int)m_weapons.size();


		//m_weaponPointer = (m_weaponPointer + 1) % (int)m_weapons.size(); // Move pointer to next weapon		
		//bool occupied = true;
		//while (occupied)
		//{
		//	if (m_weaponPointer == m_currentWeapon[(side + 1) % 2] || m_weaponPointer == m_currentWeapon[side])
		//		m_weaponPointer = (m_weaponPointer + 1) % (int)m_weapons.size(); // Occupied, move pointer to next index
		//	else
		//	{
		//		m_currentWeapon[side] = m_weaponPointer;
		//		occupied = false;
		//	}

		//}

		bool occupied = true;
		m_currentWeapon[side] = m_nextW;
		if (m_weapons.size() == 3)
		{
			while (occupied)
			{
				m_nextW = (m_nextW + 1) % (int)m_weapons.size();
				if (m_nextW != m_currentWeapon[(side + 1) % 2] && m_nextW != m_currentWeapon[side])
					occupied = false;
			}

		}
		else if (m_weapons.size() > 3)
		{
			m_nextW = m_nextnextW;
			while (occupied)
			{
				m_nextnextW = (m_nextnextW + 1) % (int)m_weapons.size();
				if (m_nextnextW != m_currentWeapon[(side + 1) % 2] && m_nextnextW != m_currentWeapon[side] && m_nextnextW != m_nextW)
					occupied = false;
			}
		}


		//m_currentWeapon[side] = (m_weaponPointer + 1) % (int)m_weapons.size(); // Change to next weapon
		//m_weaponPointer = m_currentWeapon[side]; // Update weapon pointer
		XMVECTOR relPos = m_weapons[m_currentWeapon[side]]->getRelativePos();

		if (side == RIGHT)
		{
			if (XMVectorGetX(relPos) < 0)
				relPos = XMVectorSetX(relPos, XMVectorGetX(relPos) * -1);
			m_weapons[m_currentWeapon[RIGHT]]->setRelativePos(relPos);

			if (m_weapons[m_currentWeapon[RIGHT]]->getType() == REFLECT && XMVectorGetW(m_weapons[m_currentWeapon[RIGHT]]->getData().rotation) > 90.0f)
				m_weapons[m_currentWeapon[RIGHT]]->rotate(0, 1, 0, 180);

			if (m_weapons[m_currentWeapon[RIGHT]]->getType() == SHIELD && XMVectorGetW(m_weapons[m_currentWeapon[RIGHT]]->getData().rotation) > 90.0f)
			{
				m_weapons[m_currentWeapon[RIGHT]]->rotate(0, 0, 1, 180);
			}
		}
		else
		{
			if (XMVectorGetX(relPos) > 0)
				relPos = XMVectorSetX(relPos, XMVectorGetX(relPos) * -1);
			m_weapons[m_currentWeapon[LEFT]]->setRelativePos(relPos);

			if (m_weapons[m_currentWeapon[LEFT]]->getType() == REFLECT && XMVectorGetW(m_weapons[m_currentWeapon[LEFT]]->getData().rotation) < 90.0f)
				m_weapons[m_currentWeapon[LEFT]]->rotate(0, 1, 0, 180);

			if (m_weapons[m_currentWeapon[LEFT]]->getType() == SHIELD && XMVectorGetW(m_weapons[m_currentWeapon[LEFT]]->getData().rotation) < 90.0f)
			{
				m_weapons[m_currentWeapon[LEFT]]->rotate(0, 0, 1, 180);
			}
		}

		m_ready = false;
		return m_weapons[m_currentWeapon[side]]->getType();
	}
	return -1;
}

int Robot::getCurrentWeapon(int side)
{
	return m_currentWeapon[side];
}

int Robot::getNextWeapon()
{
	if (m_weapons.size() > 2)
		return m_weapons[m_nextW]->getType();
	return -1;
}

int Robot::getNextNextWeapon()
{
	if (m_weapons.size() > 3)
		return m_weapons[m_nextnextW]->getType();
	return -1;
}

std::vector<Weapon*> Robot::getWeapons()
{
	return m_weapons;
}

void Robot::addScore(int score)
{
	m_score += score;
}

void Robot::setScore(int score)
{
	m_score = score;
}

void Robot::resetScore()
{
	m_score = 0;
}

int Robot::getScore() const
{
	return m_score;
}

void Robot::addWeapon(int type)
{	Weapon* weapon = new Weapon(type);
	m_weapons.push_back(weapon);
	if (m_weapons.size() == 3)
		m_nextW = 2;
	if (m_weapons.size() == 4)
	{
		m_nextnextW = 3;
	}
	if (m_currentWeapon[LEFT] == -1)
	{
		m_weapons.push_back(m_weapons[0]);
		m_weapons.erase(m_weapons.begin());
		m_currentWeapon[LEFT] = 0;
		m_currentWeapon[RIGHT] = 1;
		//m_weaponPointer = 1;

		XMVECTOR relPos = m_weapons[m_currentWeapon[LEFT]]->getRelativePos();
		m_weapons[m_currentWeapon[LEFT]]->setRelativePos(XMVectorSetX(relPos, XMVectorGetX(relPos) * -1));

		if (type == REFLECT)
			m_weapons[m_currentWeapon[LEFT]]->rotate(0, 1, 0, 180);

		if (type == SHIELD)
		{
			m_weapons[m_currentWeapon[LEFT]]->rotate(0, 0, 1, 180);
		}
	}
	//m_weapons.push_back(weapon);
}

void Robot::deathAnimation()
{
	DX::getInstance()->getParticles()->addParticles(getPosition(), m_colour, XMVectorSet(1.8f, 1.8f, 0, 0),
		4000, 20, 1, 10, XMVectorSet(0, 0, 0, 0), XMVectorSet(0, 1, 0, 0));
	Sound::getInstance()->play(soundEffect::e_death, getPosition(), 0.6f);
	DX::getInstance()->setDeltaTime(0.15f, 2.0f);
}

bool Robot::upgradeWeapon(int type)
{
	for (int i = 0; i < m_weapons.size(); i++)
	{
		if (m_weapons[i]->getType() == type)
		{
			m_weapons[i]->upgrade();
			return true;
		}
	}
	addWeapon(type);
	return  false;
}

void Robot::setResourceIndex(int index)
{
	m_resource = index;
}

int Robot::getResourceIndex()
{
	return m_resource;
}

void Robot::removeResource()
{
	m_resource = -1;
}

void Robot::update(float dt, QuadtreeNode* qtn, XMVECTOR& start, XMVECTOR& end)
{
	
	if (m_currentMission.index != 0)
	{
		m_currentMission = m_ai.update(dt, getVelocity(), m_currentMission);
		move(XMVectorSet(XMVectorGetX(m_currentMission.pos), 2.0f, XMVectorGetZ(m_currentMission.pos), 1.0f) - getPosition());
	}
	
	GameObject::update();
	XMVECTOR position = GameObject::getPosition();
	Lights::getInstance()->setPosition(m_lightIndex, position.m128_f32[0], position.m128_f32[1], position.m128_f32[2]);
	objectData objectData = GameObject::getData();
	XMVECTOR color = objectData.material.emission;
	Lights::getInstance()->setColor(m_lightIndex, color.m128_f32[0], color.m128_f32[1], color.m128_f32[2]);
	for (int i = 0; i < m_weapons.size(); i++)
	{
		m_weapons[i]->updateTime(dt, getPosition());
	}
	if (m_currentWeapon[RIGHT] != -1 && m_weapons[m_currentWeapon[RIGHT]]->getType() == SNIPER)
		m_weapons[m_currentWeapon[RIGHT]]->updateSniperShot(getPosition(), m_colour, m_currentRotation, RIGHT, dt, qtn, start, end);
	if (m_currentWeapon[LEFT] != -1 && m_weapons[m_currentWeapon[LEFT]]->getType() == SNIPER)
		m_weapons[m_currentWeapon[LEFT]]->updateSniperShot(getPosition(), m_colour, m_currentRotation, LEFT, dt, qtn, start, end);
	
	// Particle engine flame
	m_timeSinceParticles += dt;
	if (m_timeSinceParticles > 0.01f)
	{
		// Maxvel 0.16
		m_timeSinceParticles = 0.0f;
		float rotRadian = XM_PI / 2.0f - XMConvertToRadians(m_currentRotation);
		XMVECTOR lookAt = { XMScalarCos(rotRadian), 0.0f, XMScalarSin(rotRadian), 0.0f };
		XMVECTOR robPos = getPosition() - lookAt*1.0f;
		float velocity = XMVector3Length(m_vel).m128_f32[0]/dt;
		velocity *= 45.0f / m_velocity;
		DX::getInstance()->getParticles()->addEngineFlame(robPos, -lookAt, m_colour, velocity);
	}
	
}

void Robot::move(XMVECTOR dPos)
{
	GameObject::move(dPos);

	m_weapons[m_currentWeapon[RIGHT]]->setPosition
	(
		m_weapons[m_currentWeapon[RIGHT]]->getRelativePos()
	);

	if (getCurrentWeapon(LEFT) != -1)
	{
		m_weapons[m_currentWeapon[LEFT]]->setPosition
		(
			m_weapons[m_currentWeapon[LEFT]]->getRelativePos()
		);
	}
}

void Robot::storePositionInHistory(DirectX::XMVECTOR position)
{
	if (m_positionHistorySize < m_positionHistoryCap)
	{
		// size is less than cap
		m_positionHistory[m_positionHistoryPtr] = position; // insert at historyPtr
		m_positionHistoryPtr++;								// step historyPtr
		m_positionHistorySize++;							// increase sizePos
	}
	else
	{
		// size has grown past cap
		m_positionHistoryPtr = 0;							// reset ptr
		m_positionHistorySize = 0;							// reset size
		m_positionHistory[m_positionHistoryPtr] = position; // replace element at ptr 
	}

}

XMVECTOR Robot::getPreviousPosition() const
{
	if (m_positionHistoryPtr == 0)
	{
		return m_positionHistory[m_positionHistoryCap - 1];
	}

	return m_positionHistory[m_positionHistoryPtr - 1];
}

void Robot::setVel(DirectX::XMVECTOR vel)
{
	m_vel = vel;
}

DirectX::XMVECTOR Robot::getVel() const
{
	return m_vel;
}

void Robot::reset()
{
	m_health = 100;
	m_vel = XMVectorSet(0, 0, 0, 0);
	m_currentRotation = 0.0;
	m_currentWeapon[LEFT] = -1;
	m_currentWeapon[RIGHT] = 0;
	m_nextW = -1;
	m_nextnextW = -1;
	m_resource = -1;
	m_currentWeapon[LEFT] = -1;
	m_currentWeapon[RIGHT] = 0;
	Weapon* rifle = new Weapon(RIFLE);
	m_weapons.push_back(rifle);
	m_ready = true;
	m_time = 0;
	m_material.ambient = XMVectorSet(0.5, 0.5, 0.5, -1);
	m_material.diffuse = XMVectorSet(0.0, 0.0, 0.0, -1);
	m_material.emission = XMVectorSet(0.0, 0.0, 0.0, -1);
	m_colour = XMVectorSet(1.0, 1.0, 1.0, -1);
	m_vel = XMVectorSet(0, 0, 0, 0);
	m_score = 0;

	// Position history
	m_positionHistorySize = 0;
	m_positionHistoryCap = 100;
	m_positionHistoryPtr = 0;
	m_positionHistory = new DirectX::XMVECTOR[m_positionHistoryCap];
	m_positionHistory[m_positionHistoryCap - 1] = getPosition();

	// Particles
	m_timeSinceParticles = 0.0f;
	m_currentMission.index = 0;

	setPosition(-100, 0, 0);
}

void Robot::release()
{
	// TODO realease resource?
	for (int i = 0; i < m_weapons.size(); i++)
	{
		delete m_weapons[i];
	}
	m_weapons.clear();

	delete[] m_positionHistory;
}

void Robot::releaseScoreState()
{
	// TODO realease resource?
	for (int i = 0; i < m_weapons.size(); i++)
	{
		if (m_weapons[i])
			delete m_weapons[i];
	}
	m_weapons.clear();
	m_health = 100;
	m_vel = XMVectorSet(0, 0, 0, 0);
	m_currentRotation = 0.0;
	m_nextW = -1;
	m_nextnextW = -1;
	m_resource = -1;
	m_currentWeapon[LEFT] = -1;
	m_currentWeapon[RIGHT] = 0;
	Weapon* rifle = new Weapon(RIFLE);
	m_weapons.push_back(rifle);
	m_ready = true;
	m_time = 0;
}
